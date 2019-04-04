# Copyright (c) 2019 leosocy. All rights reserved.
# Use of this source code is governed by a MIT-style license
# that can be found in the LICENSE file.

from .clib import EdccLibrary
from .config import EncoderConfig
from .code import PalmprintCode


class Encoder(object):
    """Encode palmprint image to `Palmprint code`."""

    def __init__(self, config: EncoderConfig, lib: EdccLibrary):
        self._cfg = config
        self._lib = lib
        self._id = self._lib.new_encoder(self._cfg)

    def encode_using_filename(self, filepath):
        with open(filepath, "rb") as palmprint:
            return self.encode_using_image_bytes(palmprint.read())

    def encode_using_image_bytes(self, image_bytes):
        code_bytes = self._lib.do_encode(self._id, image_bytes)
        return PalmprintCode(code_bytes, self._lib)
