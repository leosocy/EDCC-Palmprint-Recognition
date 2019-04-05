# Copyright (c) 2019 leosocy. All rights reserved.
# Use of this source code is governed by a MIT-style license
# that can be found in the LICENSE file.


from .adapter import EdccAdapter


class PalmprintCode(object):
    """Store code, compare to another code."""

    def __init__(self, code_bytes, adapter: EdccAdapter):
        self._code = code_bytes
        self._adapter = adapter

    def compare_to(self, another):
        return self._adapter.calc_score(self._code, another._code)
