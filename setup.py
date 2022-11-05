from setuptools import Extension, setup

ext_modules = [Extension('qreki._qreki', sources=['_qreki.c'])]

setup(ext_modules=ext_modules)
