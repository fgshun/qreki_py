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
