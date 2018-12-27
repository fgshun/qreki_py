/*[clinic input]
preserve
[clinic start generated code]*/

PyDoc_STRVAR(Kyureki_from_ymd__doc__,
"from_ymd($type, /, year, month, day, tz=qreki.jst_tz)\n"
"--\n"
"\n");

#define KYUREKI_FROM_YMD_METHODDEF    \
    {"from_ymd", (PyCFunction)Kyureki_from_ymd, METH_FASTCALL|METH_KEYWORDS|METH_CLASS, Kyureki_from_ymd__doc__},

static PyObject *
Kyureki_from_ymd_impl(PyTypeObject *type, int year, int month, int day,
                      double tz);

static PyObject *
Kyureki_from_ymd(PyTypeObject *type, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"year", "month", "day", "tz", NULL};
    static _PyArg_Parser _parser = {"iii|d:from_ymd", _keywords, 0};
    int year;
    int month;
    int day;
    double tz = QREKI_JST_TZ;

    if (!_PyArg_ParseStackAndKeywords(args, nargs, kwnames, &_parser,
        &year, &month, &day, &tz)) {
        goto exit;
    }
    return_value = Kyureki_from_ymd_impl(type, year, month, day, tz);

exit:
    return return_value;
}

PyDoc_STRVAR(Kyureki_from_date__doc__,
"from_date($type, /, date, tz=qreki.jst_tz)\n"
"--\n"
"\n");

#define KYUREKI_FROM_DATE_METHODDEF    \
    {"from_date", (PyCFunction)Kyureki_from_date, METH_FASTCALL|METH_KEYWORDS|METH_CLASS, Kyureki_from_date__doc__},

static PyObject *
Kyureki_from_date_impl(PyTypeObject *type, PyObject *date, double tz);

static PyObject *
Kyureki_from_date(PyTypeObject *type, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"date", "tz", NULL};
    static _PyArg_Parser _parser = {"O|d:from_date", _keywords, 0};
    PyObject *date;
    double tz = QREKI_JST_TZ;

    if (!_PyArg_ParseStackAndKeywords(args, nargs, kwnames, &_parser,
        &date, &tz)) {
        goto exit;
    }
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
    static const char * const _keywords[] = {"year", "month", "leap_month", "day", NULL};
    static _PyArg_Parser _parser = {"hbbb:Kyureki", _keywords, 0};
    short year;
    unsigned char month;
    unsigned char leap_month;
    unsigned char day;

    if (!_PyArg_ParseTupleAndKeywordsFast(args, kwargs, &_parser,
        &year, &month, &leap_month, &day)) {
        goto exit;
    }
    return_value = Kyureki_new_impl(type, year, month, leap_month, day);

exit:
    return return_value;
}

PyDoc_STRVAR(qreki_from_date__doc__,
"from_date($module, /, date, tz=qreki.jst_tz)\n"
"--\n"
"\n");

#define QREKI_FROM_DATE_METHODDEF    \
    {"from_date", (PyCFunction)qreki_from_date, METH_FASTCALL|METH_KEYWORDS, qreki_from_date__doc__},

static PyObject *
qreki_from_date_impl(PyObject *module, PyObject *date, double tz);

static PyObject *
qreki_from_date(PyObject *module, PyObject *const *args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    static const char * const _keywords[] = {"date", "tz", NULL};
    static _PyArg_Parser _parser = {"O|d:from_date", _keywords, 0};
    PyObject *date;
    double tz = QREKI_JST_TZ;

    if (!_PyArg_ParseStackAndKeywords(args, nargs, kwnames, &_parser,
        &date, &tz)) {
        goto exit;
    }
    return_value = qreki_from_date_impl(module, date, tz);

exit:
    return return_value;
}
/*[clinic end generated code: output=6febc5c44a23b340 input=a9049054013a1b77]*/
