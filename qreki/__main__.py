import argparse
import datetime

from qreki import VERSION, Kyureki


def _print_date(shinreki, kyureki):
    print('{0.year:d}年{0.month:d}月{0.day:d}日 {1}'.format(
          shinreki, kyureki))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('year', nargs='?', type=int)
    parser.add_argument('month', nargs='?', type=int)
    parser.add_argument('day', nargs='?', type=int)
    parser.add_argument('--version',
                        action='version',
                        version='%(prog)s ' + VERSION)
    args = parser.parse_args()

    if args.year is None:
        d = datetime.date.today()
        k = Kyureki.from_date(d)
        _print_date(d, k)

    elif args.month is None:
        d = datetime.date(args.year, 1, 1)
        d1 = datetime.timedelta(1)
        while d.year == args.year:
            k = Kyureki.from_date(d)
            _print_date(d, k)
            d += d1

    elif args.day is None:
        d = datetime.date(args.year, args.month, 1)
        d1 = datetime.timedelta(1)
        while d.month == args.month:
            k = Kyureki.from_date(d)
            _print_date(d, k)
            d += d1

    else:
        d = datetime.date(args.year, args.month, args.day)
        k = Kyureki.from_date(d)
        _print_date(d, k)


if __name__ == '__main__':
    main()
