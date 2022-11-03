#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <datetime.h>
#include <structmember.h>
#include <datetime.h>

typedef struct {
    PyObject_HEAD
    unsigned short year;
    unsigned char month;
    unsigned char leap_month;
    unsigned char day;
} KyurekiObject;

static PyObject *
Kyureki_rokuyou(KyurekiObject *self, PyObject *args);
static PyObject *
Kyureki_new(PyTypeObject *subtype, PyObject *args, PyObject *kwargs);
static void
Kyureki_dealloc(KyurekiObject *self);
static PyObject *
Kyureki_repr(KyurekiObject *self);
static PyObject *
Kyureki_str(KyurekiObject *self);
static PyObject *
Kyureki_richcompare(KyurekiObject *self, KyurekiObject *other, int op);
static Py_hash_t
Kyureki_hash(KyurekiObject *self);

/*[clinic input]
module qreki
class qreki.Kyureki KyurekiObject &Kyureki_Type
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=fe94408bdf966562]*/

static double
normalize_angle(double angle);
static int
kyureki_from_jd(int tm0, double tz, int *kyureki_year, int *kyureki_month,
                int *kyureki_leap, int *kyureki_day);
static void
chuki_from_jd(double tm, double tz, double *chuki, double *longitude);
static void
before_nibun_from_jd(double tm, double tz, double *nibun, double *longitude);
static int
saku_from_jd(double tm, double tz, double *saku);
static double
longitude_of_sun(double t);
static double
longitude_of_moon(double t);
static void
jd2yearmonth(double jd, int *year, int *month);

static int module_exec(PyObject *module);

static const double degToRad = Py_MATH_PI / 180.0;
#define QREKI_JST_TZ 0.375

#include "clinic/qreki.c.h"


static PyMemberDef Kyureki_members[] = {
    {"year", T_USHORT, offsetof(KyurekiObject, year), READONLY, NULL},
    {"month", T_UBYTE, offsetof(KyurekiObject, month), READONLY, NULL},
    {"leap_month", T_UBYTE, offsetof(KyurekiObject, leap_month), READONLY, NULL},
    {"day", T_UBYTE, offsetof(KyurekiObject, day), READONLY, NULL},
    {NULL}
};


/*[clinic input]
@classmethod
qreki.Kyureki.from_ymd as Kyureki_from_ymd

    year: int
    month: int
    day: int
    tz: double(c_default="QREKI_JST_TZ") = qreki.TZ
[clinic start generated code]*/

static PyObject *
Kyureki_from_ymd_impl(PyTypeObject *type, int year, int month, int day,
                      double tz)
/*[clinic end generated code: output=d20af7563e6320d3 input=e36840b1a34e8693]*/
{
    PyObject *date;
    PyObject *self;

    date = PyDate_FromDate(year, month, day);
    if (!date) { return NULL; }

    self = Kyureki_from_date_impl(type, date, tz);
    Py_DECREF(date);
    return self;
}


/*[clinic input]
@classmethod
qreki.Kyureki.from_date as Kyureki_from_date

    date: object
    tz: double(c_default="QREKI_JST_TZ") = qreki.TZ
[clinic start generated code]*/

static PyObject *
Kyureki_from_date_impl(PyTypeObject *type, PyObject *date, double tz)
/*[clinic end generated code: output=dbadf5ced65b3615 input=54443fe0a3ed7d36]*/
{
    PyObject *t, *ret;
    long ordinal, tm0;
    PyObject *ordinal_obj;
    int kyureki_year, kyureki_month, kyureki_leap, kyureki_day, error;

    if ((ordinal_obj = PyObject_CallMethod(date, "toordinal", NULL)) == NULL) {
        return NULL;
    }
    ordinal = PyLong_AsLong(ordinal_obj);
    Py_DECREF(ordinal_obj);
    if (PyErr_Occurred()) { return NULL; }

    tm0 = ordinal + 1721424;

    error = kyureki_from_jd(tm0, tz, &kyureki_year, &kyureki_month,
                            &kyureki_leap, &kyureki_day);
    if (error) { return NULL; }

    t = Py_BuildValue("hbbb", kyureki_year, kyureki_month, kyureki_leap,
                      kyureki_day);
    if (!t) { return NULL; }

    ret = Kyureki_new(type, t, NULL);
    Py_DECREF(t);
    return ret;
}


static PyObject *
Kyureki_rokuyou(KyurekiObject *self, PyObject *args)
{
    PyObject *rokuyou_tuple, *rokuyou;
    rokuyou_tuple = PyObject_GetAttrString((PyObject *)self, "ROKUYOU");
    if (!rokuyou_tuple) { return NULL; }
    rokuyou = PySequence_GetItem(rokuyou_tuple, (self->month + self->day) % 6);
    Py_DECREF(rokuyou_tuple);
    return rokuyou;
}


static PyMethodDef Kyureki_methods[] = {
    KYUREKI_FROM_YMD_METHODDEF
    KYUREKI_FROM_DATE_METHODDEF
    {NULL, NULL, 0, NULL} /* Sentinel */
};


static PyGetSetDef Kyureki_getset[] = {
    {"rokuyou", (getter)Kyureki_rokuyou, NULL, NULL, NULL},
    {NULL} /* Sentinel */
};


/*[clinic input]
@classmethod
qreki.Kyureki.__new__ as Kyureki_new

    year: short
    month: unsigned_char
    leap_month: unsigned_char
    day: unsigned_char
[clinic start generated code]*/

static PyObject *
Kyureki_new_impl(PyTypeObject *type, short year, unsigned char month,
                 unsigned char leap_month, unsigned char day)
/*[clinic end generated code: output=108b84348be570a1 input=d389517ceadf68fa]*/
{
    KyurekiObject *self;

    self = PyObject_New(KyurekiObject, type);
    if (!self) { return NULL; }
    self->year = year;
    self->month = month;
    self->leap_month = leap_month;
    self->day = day;

    return (PyObject *)self;
}


static void
Kyureki_dealloc(KyurekiObject *self)
{
    PyObject_Del(self);
}


static PyObject *
Kyureki_repr(KyurekiObject *self)
{
    PyObject *classname = NULL, *ret = NULL;
    classname = PyObject_GetAttrString((PyObject *)Py_TYPE(self), "__name__");
    if (!classname) { return NULL; }
    ret = PyUnicode_FromFormat("%U(%u, %u, %u, %u)",
                               classname,
                               self->year, self->month,
                               self->leap_month, self->day);
    Py_DECREF(classname);
    return ret;
}


static PyObject *
Kyureki_str(KyurekiObject *self)
{
    PyObject *template = NULL, *ret = NULL;
    if (self->leap_month) {
        template = PyObject_GetAttrString((PyObject *)self, "_str_leap_template");
    } else {
        template = PyObject_GetAttrString((PyObject *)self, "_str_template");
    }
    if (!template) { return NULL; }
    ret = PyObject_CallMethod(template, "format", "hbb", self->year, self->month, self->day);
    Py_DECREF(template);
    return ret;
}


static PyObject *
Kyureki_richcompare(KyurekiObject *self, KyurekiObject *other, int op)
{
    PyObject *result;
    int c;

    if (!PyObject_IsInstance((PyObject *)other, (PyObject *)Py_TYPE(self))) {
        Py_RETURN_NOTIMPLEMENTED;
    }

    switch (op) {
        case Py_LT:
            if (self->year < other->year) { c = 1; break; }
            if (self->year > other->year) { c = 0; break; }
            if (self->month < other->month) { c = 1; break; }
            if (self->month > other->month) { c = 0; break; }
            if (self->leap_month < other->leap_month) { c = 1; break; }
            if (self->leap_month > other->leap_month) { c = 0; break; }
            c = self->day < other->day;
            break;
        case Py_LE:
            if (self->year < other->year) { c = 1; break; }
            if (self->year > other->year) { c = 0; break; }
            if (self->month < other->month) { c = 1; break; }
            if (self->month > other->month) { c = 0; break; }
            if (self->leap_month < other->leap_month) { c = 1; break; }
            if (self->leap_month > other->leap_month) { c = 0; break; }
            c = self->day <= other->day;
            break;
        case Py_EQ:
            c = ((self->year == other->year) &&
                 (self->month == other->month) &&
                 (self->leap_month == other->leap_month) &&
                 (self->day == other->day));
            break;
        case Py_NE:
            c = ((self->year != other->year) ||
                 (self->month != other->month) ||
                 (self->leap_month != other->leap_month) ||
                 (self->day != other->day));
            break;
        case Py_GT:
            if (self->year > other->year) { c = 1; break; }
            if (self->year < other->year) { c = 0; break; }
            if (self->month > other->month) { c = 1; break; }
            if (self->month < other->month) { c = 0; break; }
            if (self->leap_month > other->leap_month) { c = 1; break; }
            if (self->leap_month < other->leap_month) { c = 0; break; }
            c = self->day > other->day;
            break;
        case Py_GE:
            if (self->year > other->year) { c = 1; break; }
            if (self->year < other->year) { c = 0; break; }
            if (self->month > other->month) { c = 1; break; }
            if (self->month < other->month) { c = 0; break; }
            if (self->leap_month > other->leap_month) { c = 1; break; }
            if (self->leap_month < other->leap_month) { c = 0; break; }
            c = self->day >= other->day;
            break;
        default:
            Py_RETURN_NOTIMPLEMENTED;
    }

    result = c ? Py_True: Py_False;
    Py_INCREF(result);
    return result;
}


static Py_hash_t
Kyureki_hash(KyurekiObject *self)
{
    Py_hash_t ret;
    PyObject *t;

    t = Py_BuildValue("hbbb", self->year, self->month, self->leap_month, self->day);
    if (!t) { return -1; }
    ret = PyObject_Hash(t);
    Py_DECREF(t);

    return ret;
}


static PyType_Slot Kyureki_Type_slots[] = {
    {Py_tp_doc, "Kyureki Type"},
    {Py_tp_members, Kyureki_members},
    {Py_tp_getset, Kyureki_getset},
    {Py_tp_methods, Kyureki_methods},
    {Py_tp_new, Kyureki_new},
    {Py_tp_dealloc, Kyureki_dealloc},
    {Py_tp_repr, Kyureki_repr},
    {Py_tp_str, Kyureki_str},
    {Py_tp_richcompare, Kyureki_richcompare},
    {Py_tp_hash, Kyureki_hash},
    {0, 0},
};


static PyType_Spec Kyureki_Type_spec = {
    "_qreki.Kyureki",
    sizeof(KyurekiObject),
    0,
    Py_TPFLAGS_DEFAULT,
    Kyureki_Type_slots
};


static double
normalize_angle(double angle)
{

    angle = fmod(angle, 360.0);
    if (angle < 0.0) {
        angle += 360.0;
    }

    return angle;
}


static int
kyureki_from_jd(int tm0, double tz, int *kyureki_year, int *kyureki_month,
                int *kyureki_leap, int *kyureki_day)
{
    int shinreki_year, shinreki_month;
    double tm;
    double chu[4][2];
    double saku[5];
    int m[5][3];
    int leap;
    int i, state;

    tm = (double)tm0;

    before_nibun_from_jd(tm, tz, &chu[0][0], &chu[0][1]);
    for (i=1; i < 4; i++) {
        chuki_from_jd(chu[i-1][0] + 32.0, tz, &chu[i][0], &chu[i][1]);
    }

    if (saku_from_jd(chu[0][0], tz, &saku[0]) == -1)
        return -1;

    for (i=1; i < 5; i++) {
        if (saku_from_jd(saku[i-1] + 30.0, tz, &saku[i]) == -1)
            return -1;
        if (abs((int)saku[i - 1] - (int)saku[i]) <= 26) {
            if (saku_from_jd(saku[i-1] + 35.0, tz, &saku[i]) == -1)
                return -1;
        }
    }

    if ((int)(saku[1]) <= (int)(chu[0][0])) {
        for (i=0; i < 4; i++)
            saku[i] = saku[i+1];
        if (saku_from_jd(saku[3] + 35.0, tz, &saku[i]) == -1)
            return -1;
    }
    else if((int)saku[0] > (int)chu[0][0]) {
        for (i=4; i > 0; i--)
            saku[i] = saku[i-1];
        if (saku_from_jd(saku[0] - 27.0, tz, &saku[i]) == -1)
            return -1;
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
        if (m[i][0] > 12) {
            m[i][0] -= 12;
        }
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

    *kyureki_month = m[i][0];
    *kyureki_leap = m[i][1];
    *kyureki_day = tm0 - m[i][2] + 1;

    jd2yearmonth(tm, &shinreki_year, &shinreki_month);

    *kyureki_year = shinreki_year;
    if (*kyureki_month > 9 && *kyureki_month > shinreki_month)
        *kyureki_year -= 1;

    return 0;
}

static void
chuki_from_jd(double tm, double tz, double *chuki, double *longitude)
{
    double tm1, tm2, t;
    double rm_sun, rm_sun0;
    double delta_rm, delta_t1, delta_t2;

    tm2 = modf(tm, &tm1);
    tm2 -= tz;

    t = (tm2 + 0.5) / 36525.0 + (tm1 - 2451545.0) / 36525.0;
    rm_sun = longitude_of_sun(t);
    rm_sun0 = rm_sun - fmod(rm_sun, 30.0);

    delta_t1 = 0.0;
    delta_t2 = 1.0;

    while (fabs(delta_t1 + delta_t2) > 1.0 / 86400.0) {
        t = (tm2 + 0.5) / 36525.0 + (tm1 - 2451545.0)/ 36525.0;
        rm_sun = longitude_of_sun(t);

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


static void
before_nibun_from_jd(double tm, double tz, double *nibun, double *longitude)
{
    double tm1, tm2, t;
    double rm_sun, rm_sun0;
    double delta_rm, delta_t1, delta_t2;

    tm2 = modf(tm, &tm1);

    tm2 -= tz;

    t = (tm2 + 0.5) / 36525.0 + (tm1 - 2451545.0) / 36525.0;
    rm_sun = longitude_of_sun(t);
    rm_sun0 = rm_sun - fmod(rm_sun, 90.0);

    delta_t1 = 0.0;
    delta_t2 = 1.0;

    while (fabs(delta_t1 + delta_t2) > 1.0 / 86400.0) {
        t = (tm2 + 0.5) / 36525.0 + (tm1 - 2451545.0)/ 36525.0;
        rm_sun = longitude_of_sun(t);

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


static int
saku_from_jd(double tm, double tz, double *saku)
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
        rm_sun = longitude_of_sun(t);
        rm_moon = longitude_of_moon(t);

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


static double
longitude_of_sun(double t)
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


static double
longitude_of_moon(double t)
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


static void
jd2yearmonth(double jd, int *year, int *month)
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


static int module_exec(PyObject *module)
{
    int ret = -1;
    PyObject *kyureki_type = NULL;
    PyObject *rokuyou = NULL;
    PyObject *str_template = NULL;
    PyObject *str_leap_template = NULL;

    kyureki_type = PyType_FromSpec(&Kyureki_Type_spec);
    if (!kyureki_type) { goto cleanup; }

    /* Kyureki.ROKUYOU */
    rokuyou = Py_BuildValue("ssssss", "大安", "赤口", "先勝", "友引", "先負", "仏滅");
    if (!rokuyou) { goto cleanup; }
    if (PyObject_SetAttrString(kyureki_type, "ROKUYOU", rokuyou)) { goto cleanup; }

    /* Kyureki._str_template */
    str_template = PyUnicode_FromString("{:d}年{:d}月{:d}日");
    if (!str_template) { goto cleanup; }
    if (PyObject_SetAttrString(kyureki_type, "_str_template", str_template)) { goto cleanup; }

    /* Kyureki._str_leap_template */
    str_leap_template = PyUnicode_FromString("{:d}年閏{:d}月{:d}日");
    if (!str_leap_template) { goto cleanup; }
    if (PyObject_SetAttrString(kyureki_type, "_str_leap_template", str_leap_template)) { goto cleanup; }

    if (PyObject_SetAttrString(module, "Kyureki", kyureki_type)) { goto cleanup; }

    if (!(PyDateTime_IMPORT)) { goto cleanup; }
    ret = 0;
cleanup:
    Py_XDECREF(str_leap_template);
    Py_XDECREF(str_template);
    Py_XDECREF(rokuyou);
    Py_XDECREF(kyureki_type);
    if (ret) { Py_XDECREF(module); }

    return ret;
}


static PyModuleDef_Slot module_slots[] = {
    {Py_mod_exec, module_exec},
    {0, NULL}
};


static struct PyModuleDef qreki_module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "_qreki",
    .m_slots = module_slots,
};


PyMODINIT_FUNC PyInit__qreki(void)
{
    if (!(PyDateTime_IMPORT)) { return NULL; }
    return PyModuleDef_Init(&qreki_module);
}
