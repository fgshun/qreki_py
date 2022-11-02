from setuptools import Extension, setup

ext_modules = [Extension('qreki._qreki', sources=['qreki.c'], optional=True)]

setup(ext_modules=ext_modules)
