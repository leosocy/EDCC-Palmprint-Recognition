#!/usr/bin/python  
# -*- coding: utf-8 -*

import os
import platform
from ctypes import *
import sys

runtime_path = os.getcwd()
edcc_windows_dll_path = os.path.join(os.getcwd(), "windows-lib")
edcc_unix_so_path = os.path.join(os.getcwd(), "unix-lib")
edcc_windows_dll_name = os.path.join(edcc_windows_dll_path, "EDCC.dll")
edcc_unix_so_name = os.path.join(edcc_unix_so_path, "EDCC.so")

class EDCC_API(object):
    def __init__(self):
        self.edcc_api = None
        self.__loadEDCCLib__()
    
    def __loadEDCCLib__(self):
        if self.__isWindows__():
            os.chdir(edcc_windows_dll_path)
            self.edcc_api = cdll.LoadLibrary(edcc_windows_dll_name)
            os.chdir(runtime_path)
        elif self.__isUnix__():
            os.chdir(edcc_unix_so_path)
            self.edcc_api = cdll.LoadLibrary(edcc_unix_so_name)
            os.chdir(runtime_path)
        else:
            raise RuntimeError("Unkown system platform.")
    
    def __isWindows__(self):
        return 'Windows' in platform.system()
    
    def __isUnix__(self):
        return 'Linux' in platform.system()

    '''
    @in str:palmprintImagePath str:configPath int:codingBufMaxLen
    @out bytes:EDCCCodingBuffer int:Bufferlen
    '''
    def GetEDCCCoding(self,
                      palmprintImagePath,
                      configPath,
                      codingBufMaxLen = 1024 * 32):
        if not isinstance(palmprintImagePath, str) or not isinstance(configPath, str) or not isinstance(codingBufMaxLen, int):
            raise RuntimeError("EDCC API:GetEDCCCoding. Params in error")
        pPalmprintImagePath = c_char_p(bytes(palmprintImagePath, encoding = "utf8"))
        pConfigPath = c_char_p(bytes(configPath, encoding = "utf8"))
        codingBuf = create_string_buffer(codingBufMaxLen)
        bufMaxLen = c_uint(codingBufMaxLen)
        bufLen = c_uint(0)
        pBufLen = POINTER(c_uint)(bufLen)
        iterfaceRet = self.edcc_api.GetEDCCCoding(pPalmprintImagePath, pConfigPath, codingBuf, bufMaxLen, pBufLen)
        if iterfaceRet != 0:
            raise RuntimeError("EDCC API:GetEDCCCoding. Call API result error.")
        retBufLen = pBufLen.contents.value
        return codingBuf.raw[:retBufLen], retBufLen

    '''
    @in bytes:firstPalmprintCodingBuf bytes:secondPalmprintCodingBuf
    @out double:matchScore
    '''
    def GetTwoPalmprintCodingMatchScore(self,
                                        firstPalmprintCodingBuf,
                                        secondPalmprintCodingBuf):
        if not isinstance(firstPalmprintCodingBuf, bytes) or not isinstance(secondPalmprintCodingBuf, bytes):
            raise RuntimeError("EDCC API:GetTwoPalmprintCodingMatchScore. Params in error")
        pFirstPalmprintCodingBuf = c_char_p(firstPalmprintCodingBuf)
        pSecondPalmprintCodingBuf = c_char_p(secondPalmprintCodingBuf)
        matchScore = c_double(0.0)
        pScore = POINTER(c_double)(matchScore)
        iterfaceRet = self.edcc_api.GetTwoPalmprintCodingMatchScore(pFirstPalmprintCodingBuf, pSecondPalmprintCodingBuf, pScore)
        if iterfaceRet != 0:
            raise RuntimeError("EDCC API:GetTwoPalmprintCodingMatchScore. Call API result error.")
        return pScore.contents.value

    '''
    @in str:firstPalmprintImagePath str:secondPalmprintImagePath str:configPath
    @out double:matchScore
    '''
    def GetTwoPalmprintMatchScore(self,
                                  firstPalmprintImagePath,
                                  secondPalmprintImagePath,
                                  configPath):
        if not isinstance(firstPalmprintImagePath, str) or not isinstance(secondPalmprintImagePath, str) or not isinstance(configPath, str):
            raise RuntimeError("EDCC API:GetTwoPalmprintMatchScore. Params in error")
        pFirstPalmprintImagePath = c_char_p(bytes(firstPalmprintImagePath, encoding = "utf8"))
        pSecondPalmprintImagePath = c_char_p(bytes(secondPalmprintImagePath, encoding = "utf8"))
        pConfigPath = c_char_p(bytes(configPath, encoding = "utf8"))
        matchScore = c_double(0.0)
        pScore = POINTER(c_double)(matchScore)
        iterfaceRet = self.edcc_api.GetTwoPalmprintMatchScore(pFirstPalmprintImagePath, pSecondPalmprintImagePath, pConfigPath, pScore)
        if iterfaceRet != 0:
            raise RuntimeError("EDCC API:GetTwoPalmprintMatchScore. Call API result error.")
        return pScore.contents.value

def trans(s):
    return "b'%s'" % ''.join('%.2x' % x for x in s)

if __name__ == '__main__':
    edcc_api = EDCC_API()

    firstPalmprintPath = 'E:\\GraduationProject\\Database\PolyU\Multispectral_I\\010\\1_01_s.bmp'
    secondPalmprintPath = 'E:\\GraduationProject\\Database\PolyU\Multispectral_I\\010\\1_06_s.bmp'
    configPath = "E:\\MyselfProject\\EDCC\\samples\\py_sample\\edcc_config\\config_correct.json"

    matchScore1 = edcc_api.GetTwoPalmprintMatchScore(firstPalmprintPath, secondPalmprintPath, configPath)
    firstCodingBytes, codingLen = edcc_api.GetEDCCCoding(firstPalmprintPath, configPath)
    secondCodingBytes, codingLen = edcc_api.GetEDCCCoding(secondPalmprintPath, configPath)
    matchScore2 = edcc_api.GetTwoPalmprintCodingMatchScore(firstCodingBytes, secondCodingBytes)
    print("Score1:%.10f\tScore2:%.10f" % (matchScore1, matchScore2))
