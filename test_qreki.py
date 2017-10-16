import datetime
import qreki

def test_from_date():
    o1 = qreki._Kyureki.from_date(datetime.date(2018, 10, 15))
    assert o1.year == 2018
    assert o1.month == 9
    assert o1.leap_month == 0
    assert o1.day == 7

    o2 = qreki.Kyureki.from_date(datetime.date(2018, 10, 15))
    assert o2.year == o1.year
    assert o2.month == o1.month
    assert o2.leap_month == o1.leap_month
    assert o2.day == o1.day
