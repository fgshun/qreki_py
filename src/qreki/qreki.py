from __future__ import annotations

import datetime
import math
from typing import Sequence

DEG_TO_RAD: float = math.pi / 180.0  # （角度の）度からラジアンに変換する係数
TZ: float = 0.375  # +9.0/24.0 (JST)


class Kyureki:
    """旧暦を表すクラス"""

    __slots__ = ('_year', '_month', '_leap_month', '_day')

    ROKUYOU: Sequence[str] = ('大安', '赤口', '先勝', '友引', '先負', '仏滅')
    _year: int
    _month: int
    _leap_month: int
    _day: int

    def __new__(cls, year: int, month: int, leap_month: int, day: int) -> Kyureki:
        self = super().__new__(cls)
        self._year = year
        self._month = month
        self._leap_month = leap_month
        self._day = day

        return self

    @classmethod
    def from_ymd(cls, year: int, month: int, day: int, tz: float = TZ) -> Kyureki:
        """新暦を表す 3 整数より旧暦を得る"""

        date = datetime.date(year, month, day)
        return cls.from_date(date, tz)

    @classmethod
    def from_date(cls, date: datetime.date, tz: float = TZ) -> Kyureki:
        """datetime.date より旧暦を得る"""
        kyureki = _kyureki_from_date(date, tz)
        return cls(*kyureki)

    @property
    def year(self) -> int:
        """旧暦の年"""
        return self._year

    @property
    def month(self) -> int:
        """旧暦の月"""
        return self._month

    @property
    def leap_month(self) -> int:
        """閏月フラグ

        0 ならば通常の月、 1 ならば閏月を表す。"""
        return self._leap_month

    @property
    def day(self) -> int:
        """旧暦の日"""
        return self._day

    @property
    def rokuyou(self) -> str:
        """六曜を得る

        戻り値は六曜 (大安, 赤口, 先勝, 友引, 先負, 仏滅) の文字列。"""
        return self.ROKUYOU[(self.month + self.day) % 6]

    def __repr__(self) -> str:
        return '{:s}({:d}, {:d}, {:d}, {:d})'.format(
                type(self).__name__,
                self._year, self._month, self._leap_month, self._day)

    def __str__(self) -> str:
        return '{:d}年{:s}{:d}月{:d}日'.format(
                self._year,
                '閏' if self._leap_month else '',
                self._month,
                self._day)

    def __lt__(self, other: Kyureki) -> bool:
        if not isinstance(other, type(self)):
            return NotImplemented

        if (self._year < other._year):
            return True
        if (self._year > other._year):
            return False
        if (self._month < other._month):
            return True
        if (self._month > other._month):
            return False
        if (self._leap_month < other._leap_month):
            return True
        if (self._leap_month > other._leap_month):
            return False
        return self._day < other._day

    def __le__(self, other: Kyureki) -> bool:
        if not isinstance(other, type(self)):
            return NotImplemented

        if (self._year < other._year):
            return True
        if (self._year > other._year):
            return False
        if (self._month < other._month):
            return True
        if (self._month > other._month):
            return False
        if (self._leap_month < other._leap_month):
            return True
        if (self._leap_month > other._leap_month):
            return False
        return self._day <= other._day

    def __eq__(self, other: object) -> bool:
        if not isinstance(other, type(self)):
            return NotImplemented

        return (self._year == other._year and
                self._month == other._month and
                self._leap_month == other._leap_month and
                self._day == other._day)

    def __ne__(self, other: object) -> bool:
        if not isinstance(other, type(self)):
            return NotImplemented

        return (self._year != other._year or
                self._month != other._month or
                self._leap_month != other._leap_month or
                self._day != other._day)

    def __gt__(self, other: Kyureki) -> bool:
        if not isinstance(other, type(self)):
            return NotImplemented

        if (self._year > other._year):
            return True
        if (self._year < other._year):
            return False
        if (self._month > other._month):
            return True
        if (self._month < other._month):
            return False
        if (self._leap_month > other._leap_month):
            return True
        if (self._leap_month < other._leap_month):
            return False
        return self._day > other._day

    def __ge__(self, other: Kyureki) -> bool:
        if not isinstance(other, type(self)):
            return NotImplemented

        if (self._year > other._year):
            return True
        if (self._year < other._year):
            return False
        if (self._month > other._month):
            return True
        if (self._month < other._month):
            return False
        if (self._leap_month > other._leap_month):
            return True
        if (self._leap_month < other._leap_month):
            return False
        return self._day >= other._day

    def __hash__(self) -> int:
        return hash((self._year, self._month, self._leap_month, self._day))


def _kyureki_from_date(date: datetime.date, tz: float):
    """新暦に対応する、旧暦を求める

    引数:
        date: 新暦
        tz: タイムゾーン
    戻り値:
        旧暦を表すタプル
            0: 旧暦年
            1: 旧暦月
            2: 閏月フラグ (平月: 0, 閏月: 1）
            3: 旧暦日"""
    # ローカル補正込みのユリウス通日
    tm0 = date.toordinal() + 1721424
    tm = float(tm0)

    # 計算対象の直前にあたる二分二至の時刻を求める
    # chu[0][0]:二分二至の時刻  chu[0][1]:その時の太陽黄経
    chu = []
    chu.append(_before_nibun_from_jd(tm, tz))

    # 中気の時刻を計算
    for i in range(1, 4):
        chu.append(_chuki_from_jd(chu[i - 1][0] + 32.0, tz))

    # 計算対象の直前にあたる二分二至の直前の朔の時刻を求める
    saku = []
    saku.append(_saku_from_jd(chu[0][0], tz))

    # 朔の時刻を求める
    for i in range(1, 5):
        saku.append(_saku_from_jd(saku[i - 1] + 30.0, tz))

        if abs(int(saku[i - 1]) - int(saku[i])) <= 26:
            # 前と同じ時刻を計算した場合（両者の差が26日以内）には、
            # 初期値を +33日にして再実行させる
            saku[i] = _saku_from_jd(saku[i - 1] + 35.0, tz)

    if int(saku[1]) <= int(chu[0][0]):
        # saku[1]が二分二至の時刻以前になってしまった場合には、
        # 朔をさかのぼり過ぎたと考えて、朔の時刻を繰り下げて修正する。
        # その際、計算もれ（saku[5]）になっている部分を補うため、
        # 朔の時刻を計算する。
        # （近日点通過の近辺で朔があると起こる事があるようだ...？）
        for i in range(0, 4):
            saku[i] = saku[i + 1]
        else:
            saku[4] = _saku_from_jd(saku[3] + 35.0, tz)
    elif int(saku[0]) > int(chu[0][0]):
        # saku[0]が二分二至の時刻以後になってしまった場合には、
        # 朔をさかのぼり足りないと見て、朔の時刻を繰り上げて修正する。
        # その際、計算もれ（saku[0]）になっている部分を補うため、
        # 朔の時刻を計算する。
        # （春分点の近辺で朔があると起こる事があるようだ...？）
        for i in range(4, 0, -1):
            saku[i] = saku[i - 1]
        else:
            saku[0] = _saku_from_jd(saku[0] - 27.0, tz)

    # 閏月検索Ｆｌａｇセット
    # （節月で４ヶ月の間に朔が５回あると、閏月がある可能性がある。）
    # leap=0:平月 leap=1:閏月
    leap = 1 if int(saku[4]) <= int(chu[3][0]) else 0

    # 朔日行列の作成
    # m[i][0] ... 月名（1:正月 2:２月 3:３月 ....）
    # m[i][1] ... 閏フラグ（0:平月 1:閏月）
    # m[i][2] ... 朔日のjd
    m: list[list[int]] = []
    for i in range(5):
        m.append([0, 0, 0])

    m[0][0] = int(chu[0][1] / 30.0) + 2
    m[0][1] = 0
    m[0][2] = int(saku[0])

    for i in range(1, 5):
        if leap == 1 and i != 1:
            if int(chu[i - 1][0]) <= int(saku[i - 1]) or \
               int(chu[i - 1][0]) >= int(saku[i]):
                m[i - 1][0] = m[i - 2][0]
                m[i - 1][1] = 1
                m[i - 1][2] = int(saku[i - 1])
                leap = 0
        m[i][0] = m[i - 1][0] + 1
        if m[i][0] > 12:
            m[i][0] -= 12
        m[i][1] = 0
        m[i][2] = int(saku[i])

    # 朔日行列から旧暦を求める。
    state = 0
    for i in range(0, 5):
        if tm0 < m[i][2]:
            state = 1
            break
        elif tm0 == m[i][2]:
            state = 2
            break
    else:
        # break しなかったときに期待する i の値は 4 ではなく 5
        i += 1

    if state == 0 or state == 1:
        i -= 1

    kyureki_month = m[i][0]
    kyureki_leap = m[i][1]
    kyureki_day = tm0 - m[i][2] + 1

    # 旧暦年の計算
    # （旧暦月が10以上でかつ新暦月より大きい場合には、
    #   まだ年を越していないはず...）
    shinreki_year, shinreki_month = date.year, date.month
    kyureki_year = shinreki_year
    if kyureki_month > 9 and kyureki_month > shinreki_month:
        kyureki_year -= 1

    return kyureki_year, kyureki_month, kyureki_leap, kyureki_day


def _chuki_from_jd(tm: float, tz: float):
    """中気の時刻を求める

    引数:
        tm: 計算対象となる時刻（ローカル補正込みのユリウス通日）
        tz: タイムゾーン
    戻り値:
        中気の時刻（ローカル補正込みのユリウス通日）と
        その時の黄経のタプル"""
    # 時刻引数を分解する
    tm2, tm1 = math.modf(tm)

    # JST ==> DT （補正時刻=0.0sec と仮定して計算）
    tm2 -= tz

    # 中気の黄経 λsun0 を求める
    t = (tm2 + 0.5) / 36525.0 + (tm1 - 2451545.0) / 36525.0
    rm_sun = _longitude_of_sun(t)
    rm_sun0 = rm_sun - rm_sun % 30.0

    # 繰り返し計算によって中気の時刻を計算する
    # （誤差が±1.0 sec以内になったら打ち切る。）
    delta_t1 = 0.0
    delta_t2 = 1.0
    while abs(delta_t1 + delta_t2) > 1.0 / 86400.0:
        # λsun を計算
        t = (tm2 + 0.5) / 36525.0 + (tm1 - 2451545.0) / 36525.0
        rm_sun = _longitude_of_sun(t)

        # 黄経差 Δλ＝λsun －λsun0
        delta_rm = rm_sun - rm_sun0

        # Δλの引き込み範囲（±180°）を逸脱した場合には、補正を行う
        if delta_rm > 180.0:
            delta_rm -= 360.0
        elif delta_rm < -180.0:
            delta_rm += 360.0

        # 時刻引数の補正値 Δt
        # delta_t = delta_rm * 365.2 / 360.0;
        delta_t2, delta_t1 = math.modf(delta_rm * 365.2 / 360.0)

        # 時刻引数の補正
        # tm -= delta_t;
        tm1 -= delta_t1
        tm2 -= delta_t2
        if tm2 < 0.0:
            tm2 += 1.0
            tm1 -= 1.0

    return (tm1 + tm2 + tz, rm_sun0)


def _before_nibun_from_jd(tm: float, tz: float):
    """直前の二分二至の時刻を求める

    引数:
        tm: 計算対象となる時刻（ローカル補正込みのユリウス通日）
        tz: タイムゾーン
    戻り値:
        二分二至の時刻（ローカル補正込みのユリウス通日）と
        その時の黄経のタプル"""
    # 時刻引数を分解する
    tm2, tm1 = math.modf(tm)

    # JST ==> DT （補正時刻=0.0sec と仮定して計算）
    tm2 -= tz

    # 直前の二分二至の黄経 λsun0 を求める
    t = (tm2 + 0.5) / 36525.0 + (tm1 - 2451545.0) / 36525.0
    rm_sun = _longitude_of_sun(t)
    rm_sun0 = rm_sun - rm_sun % 90.0

    # 繰り返し計算によって直前の二分二至の時刻を計算する
    # （誤差が±1.0 sec以内になったら打ち切る。）
    delta_t1 = 0.0
    delta_t2 = 1.0
    while abs(delta_t1 + delta_t2) > (1.0 / 86400.0):
        # λsun を計算
        t = (tm2 + 0.5) / 36525.0 + (tm1 - 2451545.0) / 36525.0
        rm_sun = _longitude_of_sun(t)

        # 黄経差 Δλ＝λsun －λsun0
        delta_rm = rm_sun - rm_sun0

        # Δλの引き込み範囲（±180°）を逸脱した場合には、補正を行う
        if delta_rm > 180.0:
            delta_rm -= 360.0
        elif delta_rm < -180.0:
            delta_rm += 360.0

        # 時刻引数の補正値 Δt
        # delta_t = delta_rm * 365.2 / 360.0;
        delta_t2, delta_t1 = math.modf(delta_rm * 365.2 / 360.0)

        # 時刻引数の補正
        # tm -= delta_t;
        tm1 -= delta_t1
        tm2 -= delta_t2
        if tm2 < 0.0:
            tm2 += 1.0
            tm1 -= 1.0

    # [0] 時刻引数を合成するのと、DT ==> JST 変換を行い、戻り値とする
    # （補正時刻=0.0sec と仮定して計算）
    # [1] 黄経
    return (tm1 + tm2 + tz, rm_sun0)


def _saku_from_jd(tm: float, tz: float):
    """与えられた時刻の直近の朔の時刻（JST）を求める

    引数:
        tm: 計算対象となる時刻（ローカル補正込みのユリウス通日）
    戻り値:
        朔の時刻（ローカル補正込みのユリウス通日）"""
    # 時刻引数を分解する
    tm2, tm1 = math.modf(tm)

    # JST ==> DT （補正時刻=0.0sec と仮定して計算）
    tm2 -= tz

    # 繰り返し計算によって朔の時刻を計算する
    # （誤差が±1.0 sec以内になったら打ち切る。）
    delta_t1 = 0.0
    delta_t2 = 1.0
    for lc in range(1, 30):
        # 太陽の黄経λsun ,月の黄経λmoon を計算
        # t = (tm - 2451548.0 + 0.5)/36525.0;
        t = (tm2 + 0.5) / 36525.0 + (tm1 - 2451545.0) / 36525.0
        rm_sun = _longitude_of_sun(t)
        rm_moon = _longitude_of_moon(t)

        # 月と太陽の黄経差Δλ
        # Δλ＝λmoon－λsun
        delta_rm = rm_moon - rm_sun

        if lc == 1 and delta_rm < 0.0:
            # ループの１回目（lc=1）で delta_rm < 0.0 の場合には
            # 引き込み範囲に入るように補正する
            delta_rm = delta_rm % 360.0
        elif rm_sun >= 0.0 and rm_sun <= 20.0 and rm_moon >= 300.0:
            # 春分の近くで朔がある場合（0 ≦λsun≦ 20）で、
            # 月の黄経λmoon≧300 の場合には、Δλ＝ 360.0 － Δλ と
            # 計算して補正する
            delta_rm = delta_rm % 360.0
            delta_rm = 360.0 - delta_rm
        elif abs(delta_rm) > 40.0:
            # Δλの引き込み範囲（±40°）を逸脱した場合には、補正を行う
            delta_rm = delta_rm % 360.0

        # 時刻引数の補正値 Δt
        # delta_t = delta_rm * 29.530589 / 360.0;
        delta_t2, delta_t1 = math.modf(delta_rm * 29.530589 / 360.0)

        # 時刻引数の補正
        # tm -= delta_t;
        tm1 -= delta_t1
        tm2 -= delta_t2
        if tm2 < 0.0:
            tm2 += 1.0
            tm1 -= 1.0

        if abs(delta_t1 + delta_t2) > 1.0 / 86400.0:
            if lc == 15:
                # ループ回数が15回になったら、初期値 tm を tm-26 とする。
                tm1 = tm - 26.0
                tm2 = 0.0
        else:
            # 朔の時刻の算出が終了した
            break
    else:
        # 初期値を補正したにも関わらず、
        # 振動を続ける場合には異常終了させる。
        raise ValueError(u'朔の計算が収束せず')

    # 時刻引数を合成するのと、DT ==> JST 変換を行い、戻り値とする
    # （補正時刻=0.0sec と仮定して計算）
    return tm1 + tm2 + tz


def _longitude_of_sun(t: float):
    """太陽の黄経 λsun を計算する

    引数:
        tm: 計算対象となる時刻 (AJD) / 36525.0
    戻り値:
        太陽の黄経 λsun"""
    k = DEG_TO_RAD
    cos = math.cos

    # 摂動項の計算
    ang = (31557.0 * t + 161.0) % 360.0
    th = .0004 * cos(k * ang)
    ang = (29930.0 * t + 48.0) % 360.0
    th += .0004 * cos(k * ang)
    ang = (2281.0 * t + 221.0) % 360.0
    th += .0005 * cos(k * ang)
    ang = (155.0 * t + 118.0) % 360.0
    th += .0005 * cos(k * ang)
    ang = (33718.0 * t + 316.0) % 360.0
    th += .0006 * cos(k * ang)
    ang = (9038.0 * t + 64.0) % 360.0
    th += .0007 * cos(k * ang)
    ang = (3035.0 * t + 110.0) % 360.0
    th += .0007 * cos(k * ang)
    ang = (65929.0 * t + 45.0) % 360.0
    th += .0007 * cos(k * ang)
    ang = (22519.0 * t + 352.0) % 360.0
    th += .0013 * cos(k * ang)
    ang = (45038.0 * t + 254.0) % 360.0
    th += .0015 * cos(k * ang)
    ang = (445267.0 * t + 208.0) % 360.0
    th += .0018 * cos(k * ang)
    ang = (19.0 * t + 159.0) % 360.0
    th += .0018 * cos(k * ang)
    ang = (32964.0 * t + 158.0) % 360.0
    th += .0020 * cos(k * ang)
    ang = (71998.1 * t + 265.1) % 360.0
    th += .0200 * cos(k * ang)

    ang = (35999.05 * t + 267.52) % 360.0
    th -= 0.0048 * t * cos(k * ang)
    th += 1.9147 * cos(k * ang)

    # 比例項の計算
    ang = (36000.7695 * t) % 360.0
    ang = (ang + 280.4659) % 360.0
    th = (th + ang) % 360.0

    return th


def _longitude_of_moon(t: float):
    """月の黄経 λmoon を計算する

    引数:
        tm: 計算対象となる時刻 (AJD) / 36525.0
    戻り値:
        月の黄経 λmoon"""
    k = DEG_TO_RAD
    cos = math.cos

    # 摂動項の計算
    ang = (2322131.0 * t + 191.0) % 360.0
    th = .0003 * cos(k * ang)
    ang = (4067.0 * t + 70.0) % 360.0
    th += .0003 * cos(k * ang)
    ang = (549197.0 * t + 220.0) % 360.0
    th += .0003 * cos(k * ang)
    ang = (1808933.0 * t + 58.0) % 360.0
    th += .0003 * cos(k * ang)
    ang = (349472.0 * t + 337.0) % 360.0
    th += .0003 * cos(k * ang)
    ang = (381404.0 * t + 354.0) % 360.0
    th += .0003 * cos(k * ang)
    ang = (958465.0 * t + 340.0) % 360.0
    th += .0003 * cos(k * ang)
    ang = (12006.0 * t + 187.0) % 360.0
    th += .0004 * cos(k * ang)
    ang = (39871.0 * t + 223.0) % 360.0
    th += .0004 * cos(k * ang)
    ang = (509131.0 * t + 242.0) % 360.0
    th += .0005 * cos(k * ang)
    ang = (1745069.0 * t + 24.0) % 360.0
    th += .0005 * cos(k * ang)
    ang = (1908795.0 * t + 90.0) % 360.0
    th += .0005 * cos(k * ang)
    ang = (2258267.0 * t + 156.0) % 360.0
    th += .0006 * cos(k * ang)
    ang = (111869.0 * t + 38.0) % 360.0
    th += .0006 * cos(k * ang)
    ang = (27864.0 * t + 127.0) % 360.0
    th += .0007 * cos(k * ang)
    ang = (485333.0 * t + 186.0) % 360.0
    th += .0007 * cos(k * ang)
    ang = (405201.0 * t + 50.0) % 360.0
    th += .0007 * cos(k * ang)
    ang = (790672.0 * t + 114.0) % 360.0
    th += .0007 * cos(k * ang)
    ang = (1403732.0 * t + 98.0) % 360.0
    th += .0008 * cos(k * ang)
    ang = (858602.0 * t + 129.0) % 360.0
    th += .0009 * cos(k * ang)
    ang = (1920802.0 * t + 186.0) % 360.0
    th += .0011 * cos(k * ang)
    ang = (1267871.0 * t + 249.0) % 360.0
    th += .0012 * cos(k * ang)
    ang = (1856938.0 * t + 152.0) % 360.0
    th += .0016 * cos(k * ang)
    ang = (401329.0 * t + 274.0) % 360.0
    th += .0018 * cos(k * ang)
    ang = (341337.0 * t + 16.0) % 360.0
    th += .0021 * cos(k * ang)
    ang = (71998.0 * t + 85.0) % 360.0
    th += .0021 * cos(k * ang)
    ang = (990397.0 * t + 357.0) % 360.0
    th += .0021 * cos(k * ang)
    ang = (818536.0 * t + 151.0) % 360.0
    th += .0022 * cos(k * ang)
    ang = (922466.0 * t + 163.0) % 360.0
    th += .0023 * cos(k * ang)
    ang = (99863.0 * t + 122.0) % 360.0
    th += .0024 * cos(k * ang)
    ang = (1379739.0 * t + 17.0) % 360.0
    th += .0026 * cos(k * ang)
    ang = (918399.0 * t + 182.0) % 360.0
    th += .0027 * cos(k * ang)
    ang = (1934.0 * t + 145.0) % 360.0
    th += .0028 * cos(k * ang)
    ang = (541062.0 * t + 259.0) % 360.0
    th += .0037 * cos(k * ang)
    ang = (1781068.0 * t + 21.0) % 360.0
    th += .0038 * cos(k * ang)
    ang = (133.0 * t + 29.0) % 360.0
    th += .0040 * cos(k * ang)
    ang = (1844932.0 * t + 56.0) % 360.0
    th += .0040 * cos(k * ang)
    ang = (1331734.0 * t + 283.0) % 360.0
    th += .0040 * cos(k * ang)
    ang = (481266.0 * t + 205.0) % 360.0
    th += .0050 * cos(k * ang)
    ang = (31932.0 * t + 107.0) % 360.0
    th += .0052 * cos(k * ang)
    ang = (926533.0 * t + 323.0) % 360.0
    th += .0068 * cos(k * ang)
    ang = (449334.0 * t + 188.0) % 360.0
    th += .0079 * cos(k * ang)
    ang = (826671.0 * t + 111.0) % 360.0
    th += .0085 * cos(k * ang)
    ang = (1431597.0 * t + 315.0) % 360.0
    th += .0100 * cos(k * ang)
    ang = (1303870.0 * t + 246.0) % 360.0
    th += .0107 * cos(k * ang)
    ang = (489205.0 * t + 142.0) % 360.0
    th += .0110 * cos(k * ang)
    ang = (1443603.0 * t + 52.0) % 360.0
    th += .0125 * cos(k * ang)
    ang = (75870.0 * t + 41.0) % 360.0
    th += .0154 * cos(k * ang)
    ang = (513197.9 * t + 222.5) % 360.0
    th += .0304 * cos(k * ang)
    ang = (445267.1 * t + 27.9) % 360.0
    th += .0347 * cos(k * ang)
    ang = (441199.8 * t + 47.4) % 360.0
    th += .0409 * cos(k * ang)
    ang = (854535.2 * t + 148.2) % 360.0
    th += .0458 * cos(k * ang)
    ang = (1367733.1 * t + 280.7) % 360.0
    th += .0533 * cos(k * ang)
    ang = (377336.3 * t + 13.2) % 360.0
    th += .0571 * cos(k * ang)
    ang = (63863.5 * t + 124.2) % 360.0
    th += .0588 * cos(k * ang)
    ang = (966404.0 * t + 276.5) % 360.0
    th += .1144 * cos(k * ang)
    ang = (35999.05 * t + 87.53) % 360.0
    th += .1851 * cos(k * ang)
    ang = (954397.74 * t + 179.93) % 360.0
    th += .2136 * cos(k * ang)
    ang = (890534.22 * t + 145.7) % 360.0
    th += .6583 * cos(k * ang)
    ang = (413335.35 * t + 10.74) % 360.0
    th += 1.2740 * cos(k * ang)
    ang = (477198.868 * t + 44.963) % 360.0
    th += 6.2888 * cos(k * ang)

    # 比例項の計算
    ang = (481267.8809 * t) % 360.0
    ang = (ang + 218.3162) % 360.0
    th = (th + ang) % 360.0

    return th


# スピードアップ用の C 言語版が存在すればそちらを使う
_Kyureki = Kyureki
try:
    import qreki._qreki
    Kyureki = qreki._qreki.Kyureki  # type: ignore
except ImportError:
    pass


def rokuyou_from_ymd(year: int, month: int, day: int) -> str:
    """六曜算出ショートカット

    引数:
        新暦年月日
    戻り値:
        六曜 (大安, 赤口, 先勝, 友引, 先負, 仏滅) の文字列

    六曜を求めるにあたって、旧暦を算出している。
    旧暦も必要とする場合、 Kyureki.from_ymd(year, month, day) で
    旧暦オブジェクトをつくり、
    これの rokuyou() メソッドを呼ぶほうが効率がよい。"""
    kyureki = Kyureki.from_ymd(year, month, day)
    return kyureki.rokuyou


def rokuyou_from_date(date: datetime.date) -> str:
    """六曜算出ショートカット
    引数:
        datetime.date （新暦）
    戻り値:
        六曜 (大安, 赤口, 先勝, 友引, 先負, 仏滅) の文字列

    六曜を求めるにあたって、旧暦を算出している。
    旧暦も必要とする場合、 Kyureki.from_date(date) で
    旧暦オブジェクトをつくり、
    これの rokuyou() メソッドを呼ぶほうが効率がよい。"""
    kyureki = Kyureki.from_date(date)
    return kyureki.rokuyou
