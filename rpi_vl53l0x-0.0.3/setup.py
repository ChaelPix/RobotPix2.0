#!/usr/bin/env python
# Python wrapper for the rpi_vl53l0x library.
# Author: Peter Yang (turmary@126.com)

from setuptools import setup, find_packages, Extension
from setuptools.command.build_py import build_py
import subprocess

class CustomInstallCommand(build_py):
    """Customized install to run library Makefile"""
    def run(self):
        print("Compiling vl53l0x library...")
        proc = subprocess.Popen(["make"], \
                 stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        print(proc.stderr.read())
        build_py.run(self)


setup(name              = 'rpi_vl53l0x',
      version           = '0.0.3',
      author            = 'Peter Yang <turmary@126.com>',
      author_email      = 'turmary@126.com',
      description       = 'Userspace Raspberry Pi VL53l0X library',
      long_description  = open('README.md').read() + "\n",
      license           = 'MIT',
      url               = 'https://github.com/turmary/VL53L0X_rasp',
      cmdclass          = {'build_py': CustomInstallCommand},
      packages          = ['rpi_vl53l0x'],
      include_package_data = True,
      # py_modules        = ['vl53l0x_api'],
      ext_modules       = [Extension('_rpi_vl53l0x', 
                                     include_dirs=['platform/inc', 'VL53L0X_1.0.2/Api/core/inc'],
                                     sources=['rpi_vl53l0x_wrap.c'],
                                     library_dirs=['lib'],
                                     libraries=['vl53l0x', 'rt'])])

