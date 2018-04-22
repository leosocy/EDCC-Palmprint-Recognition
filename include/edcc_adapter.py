#!/usr/bin/env python  
# -*- coding: utf-8 -*

from ctypes import *

class EdccApi(object):
    def __init__(self):
        self._edcc_lib_name = "libedcc.so"
        self._edcc_lib = CDLL(self._edcc_lib_name)

    '''
    @in str:palmprint_image_path str:config_path int:coding_buffer_max_len
    @out bytes:edcc_coding_buffer int:buffer_len
    '''
    def GetEDCCCoding(self,
                      palmprint_image_path,
                      config_path,
                      coding_buffer_max_len = 1024 * 32):
        if not isinstance(palmprint_image_path, str) \
            or not isinstance(config_path, str) \
            or not isinstance(coding_buffer_max_len, int):
            raise RuntimeError("EDCC API:GetEDCCCoding. Params in error")
        c_pointer_palmprint_image_path = c_char_p(bytes(palmprint_image_path, encoding="utf-8"))
        c_pointer_config_path = c_char_p(bytes(config_path, encoding="utf-8"))
        c_coding_buffer = create_string_buffer(coding_buffer_max_len)
        c_buffer_max_len = c_uint(coding_buffer_max_len)
        buffer_len = c_uint(0)
        c_pointer_buffer_len = POINTER(c_uint)(buffer_len)
        iterface_ret = self.edcc_api.GetEDCCCoding(c_pointer_palmprint_image_path,
                                                   c_pointer_config_path,
                                                   c_buffer_max_len,
                                                   c_coding_buffer,
                                                   c_pointer_buffer_len)
        if iterface_ret != 0:
            raise RuntimeError("EDCC API:GetEDCCCoding. Call API result error[%d]." % (iterface_ret))
        buffer_len_ret = c_pointer_buffer_len.contents.value
        return c_coding_buffer.raw[:buffer_len_ret], buffer_len_ret

    '''
    @in bytes:first_palmprint_coding_buffer bytes:second_palmprint_coding_buffer
    @out double:matchScore
    '''
    def GetTwoPalmprintCodingMatchScore(self,
                                        first_palmprint_coding_buffer,
                                        second_palmprint_coding_buffer):
        if not isinstance(first_palmprint_coding_buffer, bytes) or not isinstance(second_palmprint_coding_buffer, bytes):
            raise RuntimeError("EDCC API:GetTwoPalmprintCodingMatchScore. Params in error")
        c_pointer_first_palmprint_coding_buffer = c_char_p(first_palmprint_coding_buffer)
        c_pointer_second_palmprint_coding_buffer = c_char_p(second_palmprint_coding_buffer)
        match_score = c_double(0.0)
        c_pointer_match_score = POINTER(c_double)(match_score)
        iterface_ret = self.edcc_api.GetTwoPalmprintCodingMatchScore(c_pointer_first_palmprint_coding_buffer,
                                                                     c_pointer_second_palmprint_coding_buffer,
                                                                     c_pointer_match_score)
        if iterface_ret != 0:
            raise RuntimeError("EDCC API:GetTwoPalmprintCodingMatchScore. Call API result error[%d]." % (iterface_ret))
        return c_pointer_match_score.contents.value

    '''
    @in str:firstpalmprint_image_path str:secondpalmprint_image_path str:config_path
    @out double:matchScore
    '''
    def GetTwoPalmprintMatchScore(self,
                                  firstpalmprint_image_path,
                                  secondpalmprint_image_path,
                                  config_path):
        if not isinstance(firstpalmprint_image_path, str)
            or not isinstance(secondpalmprint_image_path, str)
            or not isinstance(config_path, str):
            raise RuntimeError("EDCC API:GetTwoPalmprintMatchScore. Params in error")
        c_pointer_firstpalmprint_image_path = c_char_p(bytes(firstpalmprint_image_path, encoding="utf-8"))
        c_pointer_secondpalmprint_image_path = c_char_p(bytes(secondpalmprint_image_path, encoding="utf-8"))
        c_pointer_config_path = c_char_p(bytes(config_path, encoding="utf-8"))
        match_score = c_double(0.0)
        c_pointer_match_score = POINTER(c_double)(match_score)
        iterface_ret = self.edcc_api.GetTwoPalmprintMatchScore(c_pointer_firstpalmprint_image_path,
                                                               c_pointer_secondpalmprint_image_path,
                                                               c_pointer_config_path,
                                                               c_pointer_match_score)
        if iterface_ret != 0:
            raise RuntimeError("EDCC API:GetTwoPalmprintMatchScore. Call API result error[%d]." % (iterface_ret))
        return c_pointer_match_score.contents.value
