# Copyright (c) 2019 leosocy. All rights reserved.
# Use of this source code is governed by a MIT-style license
# that can be found in the LICENSE file.

from .constants import EdccErrorCode, ReversedEdccErrorCode


class EdccExceptionBase(Exception):
    """Base exception for edcc"""

    def __init__(self, errcode, errmsg):
        """
        :param errcode: Error code
        :param errmsg: Error message
        """
        self.errcode = errcode
        self.errmsg = errmsg

    def __str__(self):
        return "Error code: {code}, means: {code_name} message: {msg}".format(
            code=self.errcode,
            code_name=ReversedEdccErrorCode[self.errcode],
            msg=self.errmsg,
        )

    def __repr__(self):
        return self.__str__()
