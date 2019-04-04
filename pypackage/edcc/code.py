# Copyright (c) 2019 leosocy. All rights reserved.
# Use of this source code is governed by a MIT-style license
# that can be found in the LICENSE file.


from .clib import EdccLibrary


class PalmprintCode(object):
    """Store code, compare to another code."""

    def __init__(self, code_bytes, lib: EdccLibrary):
        self._code = code_bytes
        self._lib = lib

    def compare_to(self, another):
        return self._lib.calc_score(self._code, another._code)
