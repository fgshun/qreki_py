import datetime

import pytest

import qreki
from qreki.qreki import Kyureki, _Kyureki

classes = [_Kyureki]
ids = ['python']
if _Kyureki is not Kyureki:
    classes.append(Kyureki)
    ids.append('c_extension')


@pytest.fixture(scope='module', params=classes, ids=ids)
def kyureki_cls(request):
    yield request.param


def date_range(start, end, delta=datetime.timedelta(days=1)):
    d = start
    while d < end:
        yield d
        d += delta


@pytest.fixture()
def dates_iter():
    start = datetime.date(2017, 1, 1)
    end = datetime.date(2018, 1, 1)
    return date_range(start, end)


def test_diff_cextension_purepython(dates_iter):
    if _Kyureki is Kyureki:
        pytest.skip("c extension is not installed")

    for date in dates_iter:
        p = Kyureki.from_date(date)
        c = _Kyureki.from_date(date)
        assert p.year == c.year
        assert p.month == c.month
        assert p.leap_month == c.leap_month
        assert p.day == c.day
        assert p.rokuyou == c.rokuyou
        assert str(p) == str(c)
        assert repr(p) == repr(c)
        assert hash(p) == hash(c)


def test_from_ymd(kyureki_cls):
    o = kyureki_cls.from_ymd(2017, 10, 15)
    assert o.year == 2017
    assert o.month == 8
    assert o.leap_month == 0
    assert o.day == 26


def test_from_date(kyureki_cls):
    o = kyureki_cls.from_date(datetime.date(2017, 10, 15))
    assert o.year == 2017
    assert o.month == 8
    assert o.leap_month == 0
    assert o.day == 26


def test_rokuyou(kyureki_cls):
    assert kyureki_cls.ROKUYOU == ('大安', '赤口', '先勝', '友引', '先負', '仏滅')

    o = kyureki_cls.from_date(datetime.date(2017, 10, 15))
    assert o.rokuyou == '先負'


def test_repr(kyureki_cls):
    o = kyureki_cls.from_date(datetime.date(2017, 10, 15))
    assert repr(o) == "Kyureki(2017, 8, 0, 26)"


def test_str(kyureki_cls):
    o = kyureki_cls.from_date(datetime.date(2017, 10, 15))
    assert str(o) == "2017年8月26日"


def test_cmp(kyureki_cls):
    o = kyureki_cls.from_date(datetime.date(2017, 10, 15))
    ob = kyureki_cls.from_date(datetime.date(2017, 10, 16))
    os = kyureki_cls.from_date(datetime.date(2017, 10, 14))
    oe = kyureki_cls.from_date(datetime.date(2017, 10, 15))

    assert o < ob
    assert o <= ob
    assert not o == ob
    assert o != ob
    assert not o > ob
    assert not o >= ob

    assert not o < os
    assert not o <= os
    assert not o == os
    assert o != os
    assert o > os
    assert o >= os

    assert not o < oe
    assert o <= oe
    assert o == oe
    assert not o != oe
    assert not o > oe
    assert o >= oe


def test_hash(kyureki_cls):
    o = kyureki_cls.from_date(datetime.date(2017, 10, 15))
    ob = kyureki_cls.from_date(datetime.date(2017, 10, 16))
    oe = kyureki_cls.from_date(datetime.date(2017, 10, 15))

    assert hash(o) != hash(ob)
    assert hash(o) == hash(oe)

    d = {}
    d[o] = 1
    d[ob] = 2
    d[oe] = 3
    assert len(d) == 2
    assert d[ob] == 2
    assert d[o] == 3


def test_module_func():
    assert qreki.rokuyou_from_ymd(2017, 10, 15) == '先負'
    assert qreki.rokuyou_from_date(datetime.date(2017, 10, 15)) == '先負'
