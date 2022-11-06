from setuptools import Extension, setup

ext_modules = [Extension('qreki._qreki', sources=['src/_qreki.c'], optional=True)]

setup(ext_modules=ext_modules)
