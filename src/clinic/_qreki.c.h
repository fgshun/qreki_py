/*[clinic input]
preserve
[clinic start generated code]*/

#if defined(Py_BUILD_CORE) && !defined(Py_BUILD_CORE_MODULE)
#  include "pycore_gc.h"            // PyGC_Head
#  include "pycore_runtime.h"       // _Py_ID()
#endif


PyDoc_STRVAR(Kyureki_from_ymd__doc__,
"from_ymd($type, /, year, month, day, tz=qreki.TZ)\n"
"--\n"
"\n");

#define KYUREKI_FROM_YMD_METHODDEF    \
    {"from_ymd", _PyCFunction_CAST(Kyureki_from_ymd), METH_FASTCALL|METH_KEYWORDS|METH_CLASS, Kyureki_from_ymd__doc__},

static PyObject *
Kyureki_from_ymd_impl(PyTypeObject *type, int year, int month, int day,
                      double tz);

static PyObject *
Kyureki_from_ymd(PyTypeObject *type, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    #if defined(Py_BUILD_CORE) && !defined(Py_BUILD_CORE_MODULE)

    #define NUM_KEYWORDS 4
    static struct {
        PyGC_Head _this_is_not_used;
        PyObject_VAR_HEAD
        PyObject *ob_item[NUM_KEYWORDS];
    } _kwtuple = {
        .ob_base = PyVarObject_HEAD_INIT(&PyTuple_Type, NUM_KEYWORDS)
        .ob_item = { &_Py_ID(year), &_Py_ID(month), &_Py_ID(day), &_Py_ID(tz), },
    };
    #undef NUM_KEYWORDS
    #define KWTUPLE (&_kwtuple.ob_base.ob_base)

    #else  // !Py_BUILD_CORE
    #  define KWTUPLE NULL
    #endif  // !Py_BUILD_CORE

    static const char * const _keywords[] = {"year", "month", "day", "tz", NULL};
    static _PyArg_Parser _parser = {
        .keywords = _keywords,
        .fname = "from_ymd",
        .kwtuple = KWTUPLE,
    };
    #undef KWTUPLE
    PyObject *argsbuf[4];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 3;
    int year;
    int month;
    int day;
    double tz = QREKI_JST_TZ;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 3, 4, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    year = _PyLong_AsInt(args[0]);
    if (year == -1 && PyErr_Occurred()) {
        goto exit;
    }
    month = _PyLong_AsInt(args[1]);
    if (month == -1 && PyErr_Occurred()) {
        goto exit;
    }
    day = _PyLong_AsInt(args[2]);
    if (day == -1 && PyErr_Occurred()) {
        goto exit;
    }
    if (!noptargs) {
        goto skip_optional_pos;
    }
    if (PyFloat_CheckExact(args[3])) {
        tz = PyFloat_AS_DOUBLE(args[3]);
    }
    else
    {
        tz = PyFloat_AsDouble(args[3]);
        if (tz == -1.0 && PyErr_Occurred()) {
            goto exit;
        }
    }
skip_optional_pos:
    return_value = Kyureki_from_ymd_impl(type, year, month, day, tz);

exit:
    return return_value;
}

PyDoc_STRVAR(Kyureki_from_date__doc__,
"from_date($type, /, date, tz=qreki.TZ)\n"
"--\n"
"\n");

#define KYUREKI_FROM_DATE_METHODDEF    \
    {"from_date", _PyCFunction_CAST(Kyureki_from_date), METH_FASTCALL|METH_KEYWORDS|METH_CLASS, Kyureki_from_date__doc__},

static PyObject *
Kyureki_from_date_impl(PyTypeObject *type, PyObject *date, double tz);

static PyObject *
Kyureki_from_date(PyTypeObject *type, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    #if defined(Py_BUILD_CORE) && !defined(Py_BUILD_CORE_MODULE)

    #define NUM_KEYWORDS 2
    static struct {
        PyGC_Head _this_is_not_used;
        PyObject_VAR_HEAD
        PyObject *ob_item[NUM_KEYWORDS];
    } _kwtuple = {
        .ob_base = PyVarObject_HEAD_INIT(&PyTuple_Type, NUM_KEYWORDS)
        .ob_item = { &_Py_ID(date), &_Py_ID(tz), },
    };
    #undef NUM_KEYWORDS
    #define KWTUPLE (&_kwtuple.ob_base.ob_base)

    #else  // !Py_BUILD_CORE
    #  define KWTUPLE NULL
    #endif  // !Py_BUILD_CORE

    static const char * const _keywords[] = {"date", "tz", NULL};
    static _PyArg_Parser _parser = {
        .keywords = _keywords,
        .fname = "from_date",
        .kwtuple = KWTUPLE,
    };
    #undef KWTUPLE
    PyObject *argsbuf[2];
    Py_ssize_t noptargs = nargs + (kwnames ? PyTuple_GET_SIZE(kwnames) : 0) - 1;
    PyObject *date;
    double tz = QREKI_JST_TZ;

    args = _PyArg_UnpackKeywords(args, nargs, NULL, kwnames, &_parser, 1, 2, 0, argsbuf);
    if (!args) {
        goto exit;
    }
    date = args[0];
    if (!noptargs) {
        goto skip_optional_pos;
    }
    if (PyFloat_CheckExact(args[1])) {
        tz = PyFloat_AS_DOUBLE(args[1]);
    }
    else
    {
        tz = PyFloat_AsDouble(args[1]);
        if (tz == -1.0 && PyErr_Occurred()) {
            goto exit;
        }
    }
skip_optional_pos:
    return_value = Kyureki_from_date_impl(type, date, tz);

exit:
    return return_value;
}

static PyObject *
Kyureki_new_impl(PyTypeObject *type, short year, unsigned char month,
                 unsigned char leap_month, unsigned char day);

static PyObject *
Kyureki_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
    PyObject *return_value = NULL;
    #if defined(Py_BUILD_CORE) && !defined(Py_BUILD_CORE_MODULE)

    #define NUM_KEYWORDS 4
    static struct {
        PyGC_Head _this_is_not_used;
        PyObject_VAR_HEAD
        PyObject *ob_item[NUM_KEYWORDS];
    } _kwtuple = {
        .ob_base = PyVarObject_HEAD_INIT(&PyTuple_Type, NUM_KEYWORDS)
        .ob_item = { &_Py_ID(year), &_Py_ID(month), &_Py_ID(leap_month), &_Py_ID(day), },
    };
    #undef NUM_KEYWORDS
    #define KWTUPLE (&_kwtuple.ob_base.ob_base)

    #else  // !Py_BUILD_CORE
    #  define KWTUPLE NULL
    #endif  // !Py_BUILD_CORE

    static const char * const _keywords[] = {"year", "month", "leap_month", "day", NULL};
    static _PyArg_Parser _parser = {
        .keywords = _keywords,
        .fname = "Kyureki",
        .kwtuple = KWTUPLE,
    };
    #undef KWTUPLE
    PyObject *argsbuf[4];
    PyObject * const *fastargs;
    Py_ssize_t nargs = PyTuple_GET_SIZE(args);
    short year;
    unsigned char month;
    unsigned char leap_month;
    unsigned char day;

    fastargs = _PyArg_UnpackKeywords(_PyTuple_CAST(args)->ob_item, nargs, kwargs, NULL, &_parser, 4, 4, 0, argsbuf);
    if (!fastargs) {
        goto exit;
    }
    {
        long ival = PyLong_AsLong(fastargs[0]);
        if (ival == -1 && PyErr_Occurred()) {
            goto exit;
        }
        else if (ival < SHRT_MIN) {
            PyErr_SetString(PyExc_OverflowError,
                            "signed short integer is less than minimum");
            goto exit;
        }
        else if (ival > SHRT_MAX) {
            PyErr_SetString(PyExc_OverflowError,
                            "signed short integer is greater than maximum");
            goto exit;
        }
        else {
            year = (short) ival;
        }
    }
    {
        long ival = PyLong_AsLong(fastargs[1]);
        if (ival == -1 && PyErr_Occurred()) {
            goto exit;
        }
        else if (ival < 0) {
            PyErr_SetString(PyExc_OverflowError,
                            "unsigned byte integer is less than minimum");
            goto exit;
        }
        else if (ival > UCHAR_MAX) {
            PyErr_SetString(PyExc_OverflowError,
                            "unsigned byte integer is greater than maximum");
            goto exit;
        }
        else {
            month = (unsigned char) ival;
        }
    }
    {
        long ival = PyLong_AsLong(fastargs[2]);
        if (ival == -1 && PyErr_Occurred()) {
            goto exit;
        }
        else if (ival < 0) {
            PyErr_SetString(PyExc_OverflowError,
                            "unsigned byte integer is less than minimum");
            goto exit;
        }
        else if (ival > UCHAR_MAX) {
            PyErr_SetString(PyExc_OverflowError,
                            "unsigned byte integer is greater than maximum");
            goto exit;
        }
        else {
            leap_month = (unsigned char) ival;
        }
    }
    {
        long ival = PyLong_AsLong(fastargs[3]);
        if (ival == -1 && PyErr_Occurred()) {
            goto exit;
        }
        else if (ival < 0) {
            PyErr_SetString(PyExc_OverflowError,
                            "unsigned byte integer is less than minimum");
            goto exit;
        }
        else if (ival > UCHAR_MAX) {
            PyErr_SetString(PyExc_OverflowError,
                            "unsigned byte integer is greater than maximum");
            goto exit;
        }
        else {
            day = (unsigned char) ival;
        }
    }
    return_value = Kyureki_new_impl(type, year, month, leap_month, day);

exit:
    return return_value;
}
/*[clinic end generated code: output=95ef85dc2dcd8eca input=a9049054013a1b77]*/
