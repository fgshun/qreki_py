from setuptools import setup, Extension

py_modules = ['qreki']
ext_modules = [
        Extension('_qreki', sources = ['qreki.c'], optional=True)]

setup(
    name='qreki',
    version='0.5.1',
    description='Qreki',
    py_modules=py_modules,
    ext_modules=ext_modules)
