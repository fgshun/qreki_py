import datetime
from collections.abc import Sequence
from typing import ClassVar

class Kyureki:
    ROKUYOU: ClassVar[Sequence[str]]

    def __new__(cls, year: int, month: int, leap_month: int, day: int) -> Kyureki:
        ...

    @classmethod
    def from_ymd(cls, year: int, month: int, day: int, tz: float = ...) -> Kyureki:
        ...

    @classmethod
    def from_date(cls, date: datetime.date, tz: float = ...) -> Kyureki:
        ...

    @property
    def year(self) -> int:
        ...

    @property
    def month(self) -> int:
        ...

    @property
    def leap_month(self) -> int:
        ...

    @property
    def day(self) -> int:
        ...

    @property
    def rokuyou(self) -> str:
        ...

    def __lt__(self, other: Kyureki) -> bool:
        ...

    def __le__(self, other: Kyureki) -> bool:
        ...

    def __gt__(self, other: Kyureki) -> bool:
        ...

    def __ge__(self, other: Kyureki) -> bool:
        ...
