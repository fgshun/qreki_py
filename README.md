# 新暦、旧暦変換 qreki.py
ある新暦より対応する旧暦と六曜を算出します。

## 使用例
```
>>> from qreki import Kyureki
>>> k = Kyureki.from_ymd(2017, 10, 17)
>>> str(k)
'2017年8月28日'
>>> k.rokuyou
'大安'
>>> from datetime import date
>>> d = date(2017, 10, 21)
>>> k = Kyureki.from_date(d)
>>> str(k)
'2017年9月2日'
>>> k.rokuyou
'仏滅'
```

## Install
```
pip install git+https://github.com/fgshun/qreki_py.git@v0.6.1#egg=qreki
```

### Install on Windows
```
# Python 3.13
pip install https://github.com/fgshun/qreki_py/releases/download/v0.6.1/qreki-0.6.1-cp313-cp313-win_amd64.whl
# Python 3.12
pip install https://github.com/fgshun/qreki_py/releases/download/v0.6.1/qreki-0.6.1-cp312-cp312-win_amd64.whl
# Python 3.11
pip install https://github.com/fgshun/qreki_py/releases/download/v0.6.1/qreki-0.6.1-cp311-cp311-win_amd64.whl
# Python 3.10
pip install https://github.com/fgshun/qreki_py/releases/download/v0.6.1/qreki-0.6.1-cp310-cp310-win_amd64.whl
# Python 3.9
pip install https://github.com/fgshun/qreki_py/releases/download/v0.6.1/qreki-0.6.1-cp39-cp39-win_amd64.whl
```

### Building on Windows
Python 3.9 以降と Visual Studio 2017 以降を用意してください。 また、 環境変数 CL に /utf-8 を設定しておいてください。

## qreki.py の元となった QREKI.AWK について
qreki.py で用いている旧暦算出方法は高野 英明氏の QREKI.AWK から得たものです。

旧暦計算サンプルプログラム  $Revision:   1.1  $  
Coded by H.Takano 1993,1994  
http://www.vector.co.jp/soft/dos/personal/se016093.html

QREKI.AWK と qreki.py は新暦1年1月1日から9999年12月31日までの間で
同じ結果が得られることを確認しています。
過去や遠い未来にたいして適用する是非はともかく。

配布規定に従い QREKI.AWK と QREKI.DOC を同梱します。
QREKI.DOC には計算方法だけでなく、扱う暦と天保暦との相違点、
六曜、朔、二十四節気、ユリウス通日など多数の知見が含まれています。


Copyright (C) fgshun 2009, 2022
https://github.com/fgshun/qreki_py  
http://d.hatena.ne.jp/fgshun/
