# Copyright (c) 2019 leosocy. All rights reserved.
# Use of this source code is governed by a MIT-style license
# that can be found in the LICENSE file.


import enum


@enum.unique
class EdccErrorCode(enum.IntEnum):
    InvalidArgument = 1
    LackingCodeBuffer = 2
    CodeCfgNEWhenComparing = 3


ReversedEdccErrorCode = {f.value: f.name for f in EdccErrorCode}
