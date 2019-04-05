# Copyright (c) 2019 leosocy. All rights reserved.
# Use of this source code is governed by a MIT-style license
# that can be found in the LICENSE file.

import platform
import ctypes

from .exceptions import EdccExceptionBase
from .config import EncoderConfig


class EdccAdapter(object):
    """An adapter for c apis in edcc library."""

    LIB_NAME = "libedcc.{suffix}".format(
        suffix="so" if platform.uname()[0] == "Linux" else "dylib"
    )
    LIB_INSTALLATION_URL = (
        "https://github.com/Leosocy/EDCC-Palmprint-Recognition#installation"
    )

    def __init__(self):
        try:
            self._lib = ctypes.cdll.LoadLibrary(self.LIB_NAME)
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
        """Create a new encoder and return it's id."""
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

    def do_encode(self, encoder_id, palmprint_bytes):
        """Encode palmprint image bytes to code bytes."""
        status = ctypes.create_string_buffer(128)
        code_bytes_size = self._lib.get_size_of_code_buffer_required(encoder_id)
        code_bytes = ctypes.create_string_buffer(code_bytes_size)
        self._lib.encode_palmprint_bytes(
            encoder_id,
            palmprint_bytes,
            len(palmprint_bytes),
            code_bytes,
            code_bytes_size,
            status,
        )
        self._check_status(status)
        return code_bytes

    def calc_score(self, lhs_code_bytes, rhs_code_bytes):
        """Calculate two codes similarity score."""
        status = ctypes.create_string_buffer(128)
        score = self._lib.calculate_codes_similarity(
            lhs_code_bytes, rhs_code_bytes, status
        )
        self._check_status(status)
        return score

    @staticmethod
    def _check_status(status):
        code = status.raw[0]
        if code != 0:
            raise EdccExceptionBase(errcode=code, errmsg=status.raw[1:].decode())
