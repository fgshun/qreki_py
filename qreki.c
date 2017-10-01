#include "Python.h"
#include <stdlib.h>
#include <math.h>

static double
normalize_angle(double angle);
static PyObject *
kyureki_from_jd(PyObject *self, PyObject *args, PyObject *kwargs);
static PyObject *
chuki_from_jd(PyObject *self, PyObject *args, PyObject *kwargs);
static PyObject *
before_nibun_from_jd(PyObject *self, PyObject *args, PyObject *kwargs);
static PyObject *
saku_from_jd(PyObject *self, PyObject *args, PyObject *kwargs);
static PyObject *
longitude_of_sun(PyObject *self, PyObject *args, PyObject *kwargs);
static PyObject *
longitude_of_moon(PyObject *self, PyObject *args, PyObject *kwargs);
static PyObject *
jd2yearmonth(PyObject *self, PyObject *args, PyObject *kwargs);

static void
chuki_from_jd2(
        double tm, double tz, double *chuki, double *longitude);
static void
before_nibun_from_jd2(
        double tm, double tz, double *nibun, double *longitude);
static int
saku_from_jd2(double tm, double tz, double *saku);
static double
longitude_of_sun2(double t);
static double
longitude_of_moon2(double t);
static void
jd2yearmonth2(double jd, int *year, int *month);

static const double degToRad = Py_MATH_PI / 180.0;

static double
normalize_angle(double angle)
{

    angle = fmod(angle, 360.0);
    if (angle < 0.0) {
        angle += 360.0;
    }

    return angle;
}

static PyObject *
kyureki_from_jd(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = {"tm", "shinreki_ym", "tz", NULL};
    double tm, tz;
    int shinreki_year=0, shinreki_month=0;
    int kyureki_year, kyureki_month, kyureki_leap, kyureki_day;
    int tm0;
    double chu[4][2];
    double saku[5];
    int m[4][3];
    int leap;
    int i, state;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "dd|(ii)", kwlist,
                &tm, &tz, &shinreki_year, &shinreki_month))
        return NULL;

    tm0 = (int)tm;

    before_nibun_from_jd2(tm, tz, &chu[0][0], &chu[0][1]);
    for (i=1; i < 4; i++) {
        chuki_from_jd2(chu[i-1][0] + 32.0, tz, &chu[i][0], &chu[i][1]);
    }

    if (saku_from_jd2(chu[0][0], tz, &saku[0]) == -1)
        return NULL;

    for (i=1; i < 5; i++) {
        if (saku_from_jd2(saku[i-1] + 30.0, tz, &saku[i]) == -1)
            return NULL;
        if (abs((int)saku[i - 1] - (int)saku[i]) <= 26) {
            if (saku_from_jd2(saku[i-1] + 35.0, tz, &saku[i]) == -1)
                return NULL;
        }
    }

    if ((int)(saku[1]) <= (int)(chu[0][0])) {
        for (i=0; i < 4; i++)
            saku[i] = saku[i+1];
        if (saku_from_jd2(saku[3] + 35.0, tz, &saku[i]) == -1)
            return NULL;
    }
    else if((int)saku[0] > (int)chu[0][0]) {
        for (i=4; i > 0; i--)
            saku[i] = saku[i-1];
        if (saku_from_jd2(saku[0] - 27.0, tz, &saku[i]) == -1)
            return NULL;
    }

    leap = ((int)saku[4] <= (int)chu[3][0]) ? 1 : 0;

    m[0][0] = (int)(chu[0][1] / 30.0) + 2;
    m[0][1] = 0;
    m[0][2] = (int)saku[0];

    for (i=1; i < 5; i++) {
        if (leap == 1 && i != 1) {
            if ((int)chu[i-1][0] <= (int)saku[i-1] ||
                (int)chu[i-1][0] >= (int)saku[i]) {
                m[i-1][0] = m[i-2][0];
                m[i-1][1] = 1;
                m[i-1][2] = (int)saku[i-1];
                leap = 0;
            }
        }
        m[i][0] = m[i-1][0] + 1;
        if (m[i][0] > 12)
            m[i][0] -= 12;
        m[i][1] = 0;
        m[i][2] = (int)saku[i];
    }

    state = 0;
    for (i=0; i < 5; i++) {
        if (tm0 < m[i][2]) {
            state = 1;
            break;
        }
        else if (tm0 == m[i][2]) {
            state = 2;
            break;
        }
    }

    if (state == 0 || state == 1)
        i -= 1;

    kyureki_month = m[i][0];
    kyureki_leap = m[i][1];
    kyureki_day = tm0 - m[i][2] + 1;

    if (shinreki_year == 0)
        jd2yearmonth2(tm, &shinreki_year, &shinreki_month);

    kyureki_year = shinreki_year;
    if (kyureki_month > 9 && kyureki_month > shinreki_month)
        kyureki_year -= 1;

    return Py_BuildValue(
            "iiii", kyureki_year, kyureki_month, kyureki_leap, kyureki_day);
}

static PyObject *
chuki_from_jd(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = {"tm", "tz", NULL};
    double tm, tz;
    double chuki, longitude;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "dd", kwlist, &tm, &tz))
        return NULL;

    chuki_from_jd2(tm, tz, &chuki, &longitude);
    return Py_BuildValue("dd", chuki, longitude);
}

static void
chuki_from_jd2(
        double tm, double tz, double *chuki, double *longitude)
{
    double tm1, tm2, t;
    double rm_sun, rm_sun0;
    double delta_rm, delta_t1, delta_t2;

    tm2 = modf(tm, &tm1);
    tm2 -= tz;

    t = (tm2 + 0.5) / 36525.0 + (tm1 - 2451545.0) / 36525.0;
    rm_sun = longitude_of_sun2(t);
    rm_sun0 = rm_sun - fmod(rm_sun, 30.0);

    delta_t1 = 0.0;
    delta_t2 = 1.0;

    while (fabs(delta_t1 + delta_t2) > 1.0 / 86400.0) {
        t = (tm2 + 0.5) / 36525.0 + (tm1 - 2451545.0)/ 36525.0;
        rm_sun = longitude_of_sun2(t);

        delta_rm = rm_sun - rm_sun0;
        if (delta_rm > 180.0) {
            delta_rm -= 360.0;
        }
        else if (delta_rm < -180.0) {
            delta_rm += 360.0;
        }

        delta_t2 = modf(delta_rm * 365.2 / 360.0, &delta_t1);

        tm1 -= delta_t1;
        tm2 -= delta_t2;

        if (tm2 < 0.0) {
            tm2 += 1.0;
            tm1 -= 1.0;
        }
    }
    
    *chuki = tm1 + tm2 + tz;
    *longitude = rm_sun0;
    return;
}

static PyObject *
before_nibun_from_jd(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = {"tm", "tz", NULL};
    double tm, tz;
    double nibun, longitude;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "dd", kwlist, &tm, &tz))
        return NULL;

    before_nibun_from_jd2(tm, tz, &nibun, &longitude);
    return Py_BuildValue("dd", nibun, longitude);
}

static void
before_nibun_from_jd2(
        double tm, double tz, double *nibun, double *longitude)
{
    double tm1, tm2, t;
    double rm_sun, rm_sun0;
    double delta_rm, delta_t1, delta_t2;

    tm2 = modf(tm, &tm1);

    tm2 -= tz;

    t = (tm2 + 0.5) / 36525.0 + (tm1 - 2451545.0) / 36525.0;
    rm_sun = longitude_of_sun2(t);
    rm_sun0 = rm_sun - fmod(rm_sun, 90.0);

    delta_t1 = 0.0;
    delta_t2 = 1.0;

    while (fabs(delta_t1 + delta_t2) > 1.0 / 86400.0) {
        t = (tm2 + 0.5) / 36525.0 + (tm1 - 2451545.0)/ 36525.0;
        rm_sun = longitude_of_sun2(t);

        delta_rm = rm_sun - rm_sun0;
        if (delta_rm > 180.0) {
            delta_rm -= 360.0;
        }
        else if (delta_rm < -180.0) {
            delta_rm += 360.0;
        }

        delta_t2 = modf(delta_rm * 365.2 / 360.0, &delta_t1);

        tm1 -= delta_t1;
        tm2 -= delta_t2;

        if (tm2 < 0.0) {
            tm2 += 1.0;
            tm1 -= 1.0;
        }
    }
    
    *nibun = tm1 + tm2 + tz;
    *longitude = rm_sun0;
    return;
}

static PyObject *
saku_from_jd(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = {"tm", "tz", NULL};
    double tm, tz;
    double saku;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "dd", kwlist, &tm, &tz))
        return NULL;

    if (saku_from_jd2(tm, tz, &saku) == -1)
        return NULL;

    return PyFloat_FromDouble(saku);
}

static int
saku_from_jd2(double tm, double tz, double *saku)
{
    double tm1, tm2, t;
    double rm_sun, rm_moon;
    double delta_rm, delta_t1, delta_t2;
    int lc;

    tm2 = modf(tm, &tm1);

    tm2 -= tz;

    delta_t1 = 0.0;
    delta_t2 = 1.0;

    for (lc = 1; lc < 30; lc++) {
        t = (tm2 + 0.5) / 36525.0 + (tm1 - 2451545.0)/ 36525.0;
        rm_sun = longitude_of_sun2(t);
        rm_moon = longitude_of_moon2(t);

        delta_rm = rm_moon - rm_sun;
        if (lc == 1 && delta_rm < 0.0) {
            delta_rm = normalize_angle(delta_rm);
        }
        else if (rm_sun >= 0.0 && rm_sun <= 20.0 && rm_moon >= 300.0) {
            delta_rm = normalize_angle(delta_rm);
            delta_rm = 360.0 - delta_rm;
        }
        else if (fabs(delta_rm) > 40.0) {
            delta_rm = normalize_angle(delta_rm);
        }

        delta_t2 = modf(delta_rm * 29.530589 / 360.0, &delta_t1);

        tm1 -= delta_t1;
        tm2 -= delta_t2;

        if (tm2 < 0.0) {
            tm2 += 1.0;
            tm1 -= 1.0;
        }

        if (fabs(delta_t1 + delta_t2) > 1.0 / 86400.0) {
            if (lc == 15) {
                tm1 = tm - 26.0;
                tm2 = 0.0;
            }
        }
        else {
            break;
        }
    }
    
    if (lc >= 30) {
        PyErr_SetString(PyExc_ValueError, "");
        return -1;
    }

    *saku = tm1 + tm2 + tz;
    return 0;
}

static PyObject *
longitude_of_sun(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = {"t", NULL};
    double t;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "d", kwlist, &t))
        return NULL;

    return PyFloat_FromDouble(longitude_of_sun2(t));
}

static double
longitude_of_sun2(double t)
{
    double ang, th;

    ang = normalize_angle(31557.0 * t + 161.0);
    th = .0004 * cos(degToRad * ang);
    ang = normalize_angle(29930.0 * t + 48.0);
    th += .0004 * cos(degToRad * ang);
    ang = normalize_angle(2281.0 * t + 221.0);
    th += .0005 * cos(degToRad * ang);
    ang = normalize_angle(155.0 * t + 118.0);
    th += .0005 * cos(degToRad * ang);
    ang = normalize_angle(33718.0 * t + 316.0);
    th += .0006 * cos(degToRad * ang);
    ang = normalize_angle(9038.0 * t + 64.0);
    th += .0007 * cos(degToRad * ang);
    ang = normalize_angle(3035.0 * t + 110.0);
    th += .0007 * cos(degToRad * ang);
    ang = normalize_angle(65929.0 * t + 45.0);
    th += .0007 * cos(degToRad * ang);
    ang = normalize_angle(22519.0 * t + 352.0);
    th += .0013 * cos(degToRad * ang);
    ang = normalize_angle(45038.0 * t + 254.0);
    th += .0015 * cos(degToRad * ang);
    ang = normalize_angle(445267.0 * t + 208.0);
    th += .0018 * cos(degToRad * ang);
    ang = normalize_angle(19.0 * t + 159.0);
    th += .0018 * cos(degToRad * ang);
    ang = normalize_angle(32964.0 * t + 158.0);
    th += .0020 * cos(degToRad * ang);
    ang = normalize_angle(71998.1 * t + 265.1);
    th += .0200 * cos(degToRad * ang);

    ang = normalize_angle(35999.05 * t + 267.52);
    th -= 0.0048 * t * cos(degToRad * ang);
    th += 1.9147 * cos(degToRad * ang);

    ang = normalize_angle(36000.7695 * t);
    ang = normalize_angle(ang + 280.4659);
    th = normalize_angle(th + ang);

    return th;
}

static PyObject *
longitude_of_moon(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *kwlist[] = {"t", NULL};
    double t;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "d", kwlist, &t))
        return NULL;

    return PyFloat_FromDouble(longitude_of_moon2(t));
}

static double
longitude_of_moon2(double t)
{
    double ang, th;

    ang = normalize_angle(2322131.0 * t + 191.0);
    th = .0003 * cos(degToRad * ang);
    ang = normalize_angle(4067.0 * t + 70.0);
    th += .0003 * cos(degToRad * ang);
    ang = normalize_angle(549197.0 * t + 220.0);
    th += .0003 * cos(degToRad * ang);
    ang = normalize_angle(1808933.0 * t + 58.0);
    th += .0003 * cos(degToRad * ang);
    ang = normalize_angle(349472.0 * t + 337.0);
    th += .0003 * cos(degToRad * ang);
    ang = normalize_angle(381404.0 * t + 354.0);
    th += .0003 * cos(degToRad * ang);
    ang = normalize_angle(958465.0 * t + 340.0);
    th += .0003 * cos(degToRad * ang);
    ang = normalize_angle(12006.0 * t + 187.0);
    th += .0004 * cos(degToRad * ang);
    ang = normalize_angle(39871.0 * t + 223.0);
    th += .0004 * cos(degToRad * ang);
    ang = normalize_angle(509131.0 * t + 242.0);
    th += .0005 * cos(degToRad * ang);
    ang = normalize_angle(1745069.0 * t + 24.0);
    th += .0005 * cos(degToRad * ang);
    ang = normalize_angle(1908795.0 * t + 90.0);
    th += .0005 * cos(degToRad * ang);
    ang = normalize_angle(2258267.0 * t + 156.0);
    th += .0006 * cos(degToRad * ang);
    ang = normalize_angle(111869.0 * t + 38.0);
    th += .0006 * cos(degToRad * ang);
    ang = normalize_angle(27864.0 * t + 127.0);
    th += .0007 * cos(degToRad * ang);
    ang = normalize_angle(485333.0 * t + 186.0);
    th += .0007 * cos(degToRad * ang);
    ang = normalize_angle(405201.0 * t + 50.0);
    th += .0007 * cos(degToRad * ang);
    ang = normalize_angle(790672.0 * t + 114.0);
    th += .0007 * cos(degToRad * ang);
    ang = normalize_angle(1403732.0 * t + 98.0);
    th += .0008 * cos(degToRad * ang);
    ang = normalize_angle(858602.0 * t + 129.0);
    th += .0009 * cos(degToRad * ang);
    ang = normalize_angle(1920802.0 * t + 186.0);
    th += .0011 * cos(degToRad * ang);
    ang = normalize_angle(1267871.0 * t + 249.0);
    th += .0012 * cos(degToRad * ang);
    ang = normalize_angle(1856938.0 * t + 152.0);
    th += .0016 * cos(degToRad * ang);
    ang = normalize_angle(401329.0 * t + 274.0);
    th += .0018 * cos(degToRad * ang);
    ang = normalize_angle(341337.0 * t + 16.0);
    th += .0021 * cos(degToRad * ang);
    ang = normalize_angle(71998.0 * t + 85.0);
    th += .0021 * cos(degToRad * ang);
    ang = normalize_angle(990397.0 * t + 357.0);
    th += .0021 * cos(degToRad * ang);
    ang = normalize_angle(818536.0 * t + 151.0);
    th += .0022 * cos(degToRad * ang);
    ang = normalize_angle(922466.0 * t + 163.0);
    th += .0023 * cos(degToRad * ang);
    ang = normalize_angle(99863.0 * t + 122.0);
    th += .0024 * cos(degToRad * ang);
    ang = normalize_angle(1379739.0 * t + 17.0);
    th += .0026 * cos(degToRad * ang);
    ang = normalize_angle(918399.0 * t + 182.0);
    th += .0027 * cos(degToRad * ang);
    ang = normalize_angle(1934.0 * t + 145.0);
    th += .0028 * cos(degToRad * ang);
    ang = normalize_angle(541062.0 * t + 259.0);
    th += .0037 * cos(degToRad * ang);
    ang = normalize_angle(1781068.0 * t + 21.0);
    th += .0038 * cos(degToRad * ang);
    ang = normalize_angle(133.0 * t + 29.0);
    th += .0040 * cos(degToRad * ang);
    ang = normalize_angle(1844932.0 * t + 56.0);
    th += .0040 * cos(degToRad * ang);
    ang = normalize_angle(1331734.0 * t + 283.0);
    th += .0040 * cos(degToRad * ang);
    ang = normalize_angle(481266.0 * t + 205.0);
    th += .0050 * cos(degToRad * ang);
    ang = normalize_angle(31932.0 * t + 107.0);
    th += .0052 * cos(degToRad * ang);
    ang = normalize_angle(926533.0 * t + 323.0);
    th += .0068 * cos(degToRad * ang);
    ang = normalize_angle(449334.0 * t + 188.0);
    th += .0079 * cos(degToRad * ang);
    ang = normalize_angle(826671.0 * t + 111.0);
    th += .0085 * cos(degToRad * ang);
    ang = normalize_angle(1431597.0 * t + 315.0);
    th += .0100 * cos(degToRad * ang);
    ang = normalize_angle(1303870.0 * t + 246.0);
    th += .0107 * cos(degToRad * ang);
    ang = normalize_angle(489205.0 * t + 142.0);
    th += .0110 * cos(degToRad * ang);
    ang = normalize_angle(1443603.0 * t + 52.0);
    th += .0125 * cos(degToRad * ang);
    ang = normalize_angle(75870.0 * t + 41.0);
    th += .0154 * cos(degToRad * ang);
    ang = normalize_angle(513197.9 * t + 222.5);
    th += .0304 * cos(degToRad * ang);
    ang = normalize_angle(445267.1 * t + 27.9);
    th += .0347 * cos(degToRad * ang);
    ang = normalize_angle(441199.8 * t + 47.4);
    th += .0409 * cos(degToRad * ang);
    ang = normalize_angle(854535.2 * t + 148.2);
    th += .0458 * cos(degToRad * ang);
    ang = normalize_angle(1367733.1 * t + 280.7);
    th += .0533 * cos(degToRad * ang);
    ang = normalize_angle(377336.3 * t + 13.2);
    th += .0571 * cos(degToRad * ang);
    ang = normalize_angle(63863.5 * t + 124.2);
    th += .0588 * cos(degToRad * ang);
    ang = normalize_angle(966404.0 * t + 276.5);
    th += .1144 * cos(degToRad * ang);
    ang = normalize_angle(35999.05 * t + 87.53);
    th += .1851 * cos(degToRad * ang);
    ang = normalize_angle(954397.74 * t + 179.93);
    th += .2136 * cos(degToRad * ang);
    ang = normalize_angle(890534.22 * t + 145.7);
    th += .6583 * cos(degToRad * ang);
    ang = normalize_angle(413335.35 * t + 10.74);
    th += 1.2740 * cos(degToRad * ang);
    ang = normalize_angle(477198.868 * t + 44.963);
    th += 6.2888 * cos(degToRad * ang);

    ang = normalize_angle(481267.8809 * t);
    ang = normalize_angle(ang + 218.3162);
    th = normalize_angle(th + ang);

    return th;
}

static PyObject *
jd2yearmonth(PyObject *self, PyObject *args, PyObject *kwargs) {
    static char *kwlist[] = {"jd", NULL};
    double jd;
    int year, month;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "d", kwlist, &jd))
        return NULL;

    jd2yearmonth2(jd, &year, &month);

    return Py_BuildValue("ii", year, month);
}

static void
jd2yearmonth2(double jd, int *year, int *month)
{
    double f0, f1, f2, f3, f4, f5, f6;
    int i1, i3, i5, i6;

    f0 = floor(jd + 68570.0);
    f1 = floor(f0 / 36524.25);
    f2 = f0 - floor(36524.25 * f1 + 0.75);
    f3 = floor((f2 + 1.0) / 365.2425);
    f4 = f2 - floor(365.25 * f3) + 31.0;
    f5 = floor(f4 / 30.59);
    f6 = floor(f5 / 11.0);

    i1 = (int)f1;
    i3 = (int)f3;
    i5 = (int)f5;
    i6 = (int)f6;

    *year = 100 * (i1 - 49) + i3 + i6;
    *month = i5 - 12 * i6 + 2;

    return;
}

static PyMethodDef module_methods[] = {
    {"_kyureki_from_jd", (PyCFunction)kyureki_from_jd,
     METH_VARARGS | METH_KEYWORDS,
     "calculate kyureki."},
    {"_chuki_from_jd", (PyCFunction)chuki_from_jd,
     METH_VARARGS | METH_KEYWORDS,
     "calculate chuki."},
    {"_before_nibun_from_jd", (PyCFunction)before_nibun_from_jd,
     METH_VARARGS | METH_KEYWORDS,
     "calculate before nibun."},
    {"_saku_from_jd", (PyCFunction)saku_from_jd,
     METH_VARARGS | METH_KEYWORDS,
     "calculate saku."},
    {"_longitude_of_sun", (PyCFunction)longitude_of_sun,
     METH_VARARGS | METH_KEYWORDS,
     "calculate longitude of sun."},
    {"_longitude_of_moon", (PyCFunction)longitude_of_moon,
     METH_VARARGS | METH_KEYWORDS,
     "calculate longitude of moon."},
    {"_jd2yearmonth", (PyCFunction)jd2yearmonth,
     METH_VARARGS | METH_KEYWORDS,
     "calculate shinreki year and shinreki month from jd."},
    {NULL, NULL, 0, NULL} /* Sentinel */
};

PyMODINIT_FUNC
init_qreki(void) {
    PyObject* m;

    m = Py_InitModule3("_qreki", module_methods,
                       "calculate longitude of sun, moon.");
    if (m == NULL) return;
}
