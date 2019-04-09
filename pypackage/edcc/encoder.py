# Copyright (c) 2019 leosocy. All rights reserved.
# Use of this source code is governed by a MIT-style license
# that can be found in the LICENSE file.

from .adapter import EdccAdapter
from .config import EncoderConfig
from .code import PalmprintCode


class Encoder(object):
    """Encode palmprint image to `Palmprint code`."""

    def __init__(self, config: EncoderConfig, adapter: EdccAdapter):
        self._cfg = config
        self._adapter = adapter
        self._id = self._adapter.new_encoder(self._cfg)

    def encode_using_file(self, filepath):
        with open(filepath, "rb") as palmprint:
            return self.encode_using_bytes(palmprint.read())

    def encode_using_bytes(self, image_bytes):
        code_bytes = self._adapter.do_encode(self._id, image_bytes)
        return PalmprintCode(code_bytes, self._adapter)
