# Copyright (c) 2019 leosocy. All rights reserved.
# Use of this source code is governed by a MIT-style license
# that can be found in the LICENSE file.

"""
An efficient and accurate algorithm for palmprint-recognition.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

usage:

   >>> import edcc
   >>> config  = edcc.EncoderConfig(29, 5, 5 ,10)
   >>> encoder = edcc.create_encoder(config)
   >>> one_palmprint_code = encoder.encode_using_file("./palmprint_one.bmp")
   >>> another_palmprint_code = encoder.encode_using_file("./palmprint_another.bmp")
   >>> one_palmprint_code.compare_to(another_palmprint_code)
   0.24910820451843044

"""

__author__ = "Leosocy <leosocy@gmail.com>"
__version__ = "0.2.1"

from .adapter import EdccAdapter
from .config import EncoderConfig, default_encoder_config
from .encoder import Encoder
from .code import PalmprintCode
from .exceptions import EdccExceptionBase


adapter = EdccAdapter()


def create_encoder(config):
    return Encoder(config, adapter)


default_encoder = create_encoder(default_encoder_config)

