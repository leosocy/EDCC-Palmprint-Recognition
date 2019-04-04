# Copyright (c) 2018 leosocy. All rights reserved.
# Use of this source code is governed by a MIT-style license
# that can be found in the LICENSE file.

import io
import os

from setuptools import setup

import edcc

# Package meta-data.
NAME = 'edcc'
DESCRIPTION = 'EDCC: An efficient and accurate algorithm for palmprint recognition.'
URL = 'https://github.com/Leosocy/EDCC-Palmprint-Recognition'
EMAIL = 'leosocy@gmail.com'
AUTHOR = 'Leosocy'
VERSION = edcc.__version__

root = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))

edcc_classifiers = [
    # Trove classifiers
    # Full list: https://pypi.python.org/pypi?%3Aaction=list_classifiers
    'Development Status :: 2 - Pre-Alpha',
    'Programming Language :: Python',
    'Programming Language :: Python :: 3',
    'Programming Language :: Python :: 3.4',
    'Programming Language :: Python :: 3.5',
    'Programming Language :: Python :: 3.6',
    'Intended Audience :: Developers',
    'License :: OSI Approved :: MIT License',
    'Topic :: Software Development :: Libraries',
]

try:
    with io.open(os.path.join(root, 'README.md'), encoding='utf-8') as f:
        long_description = '\n' + f.read()
except FileNotFoundError:
    long_description = DESCRIPTION

setup(
    name=NAME,
    version=VERSION,
    description=DESCRIPTION,
    long_description=long_description,
    long_description_content_type='text/markdown',
    author=AUTHOR,
    author_email=EMAIL,
    python_requires='>=3',
    url=URL,
    py_modules=['edcc'],
    include_package_data=True,
    license='MIT',
    classifiers=edcc_classifiers
)
