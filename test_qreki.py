import datetime
import qreki


def test_from_ymd():
    o1 = qreki._Kyureki.from_ymd(2017, 10, 15)
    assert o1.year == 2017
    assert o1.month == 8
    assert o1.leap_month == 0
    assert o1.day == 26

    o2 = qreki.Kyureki.from_ymd(2017, 10, 15)
    assert o2.year == o1.year
    assert o2.month == o1.month
    assert o2.leap_month == o1.leap_month
    assert o2.day == o1.day


def test_from_date():
    o1 = qreki._Kyureki.from_date(datetime.date(2017, 10, 15))
    assert o1.year == 2017
    assert o1.month == 8
    assert o1.leap_month == 0
    assert o1.day == 26

    o2 = qreki.Kyureki.from_date(datetime.date(2017, 10, 15))
    assert o2.year == o1.year
    assert o2.month == o1.month
    assert o2.leap_month == o1.leap_month
    assert o2.day == o1.day


def test_rokuyou():
    assert qreki._Kyureki.ROKUYOU == ('大安', '赤口', '先勝', '友引', '先負', '仏滅')
    assert qreki._Kyureki.ROKUYOU == qreki.Kyureki.ROKUYOU

    o1 = qreki._Kyureki.from_date(datetime.date(2017, 10, 15))
    assert o1.rokuyou == '先負';

    o2 = qreki.Kyureki.from_date(datetime.date(2017, 10, 15))
    assert o2.rokuyou == o1.rokuyou

    assert qreki.rokuyou_from_ymd(2017, 10, 15) == '先負'
    assert qreki.rokuyou_from_date(datetime.date(2017, 10, 15)) == '先負'


def test_repr():
    o1 = qreki._Kyureki.from_date(datetime.date(2017, 10, 15))
    assert repr(o1) == "Kyureki(2017, 8, 0, 26)"

    o2 = qreki.Kyureki.from_date(datetime.date(2017, 10, 15))
    assert repr(o2) == repr(o1)


def test_str():
    o1 = qreki._Kyureki.from_date(datetime.date(2017, 10, 15))
    assert str(o1) == "2017年8月26日"

    o2 = qreki.Kyureki.from_date(datetime.date(2017, 10, 15))
    assert str(o2) == str(o1)


def test_cmp():
    o1 = qreki._Kyureki.from_date(datetime.date(2017, 10, 15))
    o1b = qreki._Kyureki.from_date(datetime.date(2017, 10, 16))
    o1s = qreki._Kyureki.from_date(datetime.date(2017, 10, 14))
    o1e = qreki._Kyureki.from_date(datetime.date(2017, 10, 15))

    o2 = qreki.Kyureki.from_date(datetime.date(2017, 10, 15))
    o2b = qreki.Kyureki.from_date(datetime.date(2017, 10, 16))
    o2s = qreki.Kyureki.from_date(datetime.date(2017, 10, 14))
    o2e = qreki.Kyureki.from_date(datetime.date(2017, 10, 15))

    assert o1 < o1b
    assert o1 <= o1b
    assert not o1 == o1b
    assert o1 != o1b
    assert not o1 > o1b
    assert not o1 >= o1b

    assert not o1 < o1s
    assert not o1 <= o1s
    assert not o1 == o1s
    assert o1 != o1s
    assert o1 > o1s
    assert o1 >= o1s

    assert not o1 < o1e
    assert o1 <= o1e
    assert o1 == o1e
    assert not o1 != o1e
    assert not o1 > o1e
    assert o1 >= o1e

    assert o2 < o2b
    assert o2 <= o2b
    assert not o2 == o2b
    assert o2 != o2b
    assert not o2 > o2b
    assert not o2 >= o2b

    assert not o2 < o2s
    assert not o2 <= o2s
    assert not o2 == o2s
    assert o2 != o2s
    assert o2 > o2s
    assert o2 >= o2s

    assert not o2 < o2e
    assert o2 <= o2e
    assert o2 == o2e
    assert not o2 != o2e
    assert not o2 > o2e
    assert o2 >= o2e


def test_hash():
    o1 = qreki._Kyureki.from_date(datetime.date(2017, 10, 15))
    o1b = qreki._Kyureki.from_date(datetime.date(2017, 10, 16))
    o1e = qreki._Kyureki.from_date(datetime.date(2017, 10, 15))

    o2 = qreki.Kyureki.from_date(datetime.date(2017, 10, 15))
    o2b = qreki.Kyureki.from_date(datetime.date(2017, 10, 16))
    o2e = qreki.Kyureki.from_date(datetime.date(2017, 10, 15))


    assert hash(o1) != hash(o1b)
    assert hash(o1) == hash(o1e)
    assert hash(o2) != hash(o2b)
    assert hash(o2) == hash(o2e)
    assert hash(o1) == hash(o2)

    d = {}
    d[o1] = 1
    d[o1b] = 2
    d[o1e] = 3
    assert len(d) == 2
    assert d[o1b] == 2
    assert d[o1] == 3

    d = {}
    d[o1] = 1
    d[o1b] = 2
    d[o1e] = 3
    assert len(d) == 2
    assert d[o1b] == 2
    assert d[o1] == 3

    d = {}
    d[o1] = 1
    d[o2] = 2
    assert len(d) == 2
    assert d[o1] == 1
    assert d[o2] == 2
