# Copyright (c) 2019 leosocy. All rights reserved.
# Use of this source code is governed by a MIT-style license
# that can be found in the LICENSE file.

"""An efficient and accurate algorithm for palmprint-recognition"""

__author__ = "Leosocy <leosocy@gmail.com>"
__version__ = "0.2.0"

import enum
import ctypes


@enum.unique
class EdccRuntimeErrorCode(enum.IntEnum):
    InvalidArgument = 1
    LackingCodeBuffer = 2
    CodeCfgNEWhenComparing = 3


class EdccRuntimeException(Exception):
    """Base exception for edcc"""

    def __init__(self, errcode, errmsg):
        """
        :param errcode: Error code
        :param errmsg: Error message
        """
        self.errcode = errcode
        self.errmsg = errmsg

    def __str__(self):
        return "Error code: {code}, message: {msg}".format(
            code=self.errcode, msg=self.errmsg
        )

    def __repr__(self):
        return self.__str__()


class EncoderConfig(object):
    """Config which is used to initialize `Encoder`"""

    def __init__(
        self, image_size, gabor_kernel_size, laplace_kernel_size, gabor_directions
    ):
        self.image_size = image_size
        self.gabor_kernel_size = gabor_kernel_size
        self.laplace_kernel_size = laplace_kernel_size
        self.gabor_directions = gabor_directions


class Encoder(object):
    """Encode palmprint image to `Palmprint code`."""

    def __init__(self, config, lib=_edcc_lib):
        self._cfg = config
        self._lib = lib


class PalmprintCode(object):
    """Store code, compare to another code."""

    pass


class EdccLibrary(object):
    """Use ctypes to call functions in so."""

    LIB_NAME = "libedcc"
    LIB_SUFFIX = "so" if platform.uname()[0] == "Linux" else "dylib"
    LIB_INSTALLATION_URL = (
        "https://github.com/Leosocy/EDCC-Palmprint-Recognition#installation"
    )

    def __init__(self):
        try:
            self._lib = ctypes.cdll.LoadLibrary(
                "{}.{}".format(self.LIB_NAME, self.LIB_SUFFIX)
            )
        except OSError:
            raise OSError(
                "Library [{:s}] not found.\nPlease see {:s}".format(
                    self.LIB_NAME, self.LIB_INSTALLATION_URL
                )
            )

        # init edcc lib apis.
        self._lib.new_encoder_with_config.argtypes = [
            ctypes.c_uint8,
            ctypes.c_uint8,
            ctypes.c_uint8,
            ctypes.c_uint8,
            ctypes.c_char_p,
        ]
        self._lib.new_encoder_with_config.restype = ctypes.c_int

        self._lib.get_size_of_code_buffer_required.argtypes = [ctypes.c_int]
        self._lib.get_size_of_code_buffer_required.restype = ctypes.c_ulong

        self._lib.encode_palmprint_bytes.argtypes = [
            ctypes.c_int,
            ctypes.c_char_p,
            ctypes.c_ulong,
            ctypes.c_char_p,
            ctypes.c_ulong,
            ctypes.c_char_p,
        ]
        self._lib.encode_palmprint_bytes.restype = None

        self._lib.calculate_codes_similarity.argtypes = [
            ctypes.c_char_p,
            ctypes.c_char_p,
            ctypes.c_char_p,
        ]
        self._lib.calculate_codes_similarity.restype = ctypes.c_double

    def new_encoder(self, config: EncoderConfig):
        status = ctypes.create_string_buffer(128)
        encoder_id = self._lib.new_encoder_with_config(
            config.image_size,
            config.gabor_kernel_size,
            config.laplace_kernel_size,
            config.gabor_directions,
            status,
        )
        self._check_status(status)
        return encoder_id

    @staticmethod
    def _check_status(status):
        code = status.raw[0]
        if code != 0:
            raise EdccRuntimeException(errcode=code, errmsg=status.raw[1:].decode())


default_encoder_config = EncoderConfig(29, 5, 5, 10)
_edcc_lib = EdccLibrary()
