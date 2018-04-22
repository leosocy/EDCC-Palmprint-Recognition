#!/usr/bin/env python3 
# -*- coding: utf-8 -*

import os
from edcc_adapter import *
from PalmprintImageFactory import *
from PalmprintCodeRepository import *
import time

class PalmprintCodeDTO(object):
    def __init__(self, ID, instanceID, imagePath, code):
        self.palmprint = PalmprintDTO(ID, instanceID, imagePath)
        self.code = code

class EDCCSample(object):
    def __init__(self):
        self._edcc_api = EdccApi()
        self._factory = PalmprintImageFactory(4)
        self._dbPath = "./R_PALMPRINTCODE.db"
        if os.path.exists(self._dbPath):
            os.remove(self._dbPath)
        self._palmprintcodeRepository = PalmprintCodeRepo(self._dbPath)
        self._configPath = os.path.normpath(os.path.join(os.getcwd(), "edcc_config/config.json"))
        self._succNum = 0
        self._failNum = 0
        self._wrong_list = []

    def runSample(self):
        self.__initDB__()
        self.__readDB__()
        predictGroup = self._factory.predictGroup
        total_cost_time = 0.0
        total_match_count = 0
        for predict in predictGroup:
            predictPalmprintCode, codingLen = self._edcc_api.GetEDCCCoding(predict.imagePath, self._configPath)
            K = 3
            topKMatchScore = []
            timeBegin = time.time()
            for trainPalmprintCode in self.palmprintcodelist:
                matchScore = self._edcc_api.GetTwoPalmprintCodingMatchScore(predictPalmprintCode, trainPalmprintCode.code)
                dictTmp = {}
                dictTmp["instance"] = trainPalmprintCode
                dictTmp["score"] = matchScore
                topKMatchScore.append(dictTmp)
            timeEnd = time.time()
            costTime = (timeEnd - timeBegin) * 1000
            total_cost_time += costTime
            total_match_count += 1

            topKMatchScore = sorted(topKMatchScore, key=lambda p:p["score"], reverse = True)
            while len(topKMatchScore) > K:
                topKMatchScore.pop()
            self.statisticsResult(predict, topKMatchScore, costTime)
        print("\n\nPredict Over. Total:%d\tPredictCorrect:%d\tAccuracy:%lf%%" % (len(predictGroup), self._succNum, float(self._succNum) / len(predictGroup) * 100))
        print("Total Cost Time:%lf\tMatch Count:%d\tPer Cost Time:%lf" % (total_cost_time, total_match_count, total_cost_time / total_match_count))
        print("Wrong List:")
        for record in self._wrong_list:
            print(record)

    def statisticsResult(self, predict, topKMatchScore, costTime):
        resultsDict = {}
        idCountDict = {}
        for result in topKMatchScore:
            palmprintCodeInstance = result["instance"]
            ID = palmprintCodeInstance.palmprint.id
            InstanceID = palmprintCodeInstance.palmprint.instanceID
            score = result["score"]
            if ID not in resultsDict.keys():
                resultsDict[ID] = 0.0
                idCountDict[ID] = 0
            if score > resultsDict[ID]:
                resultsDict[ID] = score
            idCountDict[ID] = idCountDict[ID] + 1
        #for ID in resultsDict.keys():
        #    resultsDict[ID] = resultsDict[ID] / float(idCountDict[ID])
        resultsDict = sorted(resultsDict.items(), key=lambda r:r[1], reverse=True)
        bestMatchID = resultsDict[0][0]
        bestMatchScore = resultsDict[0][1]
        resultStr = "Predict:\tID:%s\tInstanceID:%s\t\nBestMatch:\tID:%s\t\nMatchScore:%lf\tCostTime:%lfms" % (predict.id, predict.instanceID, bestMatchID, bestMatchScore, costTime)
        print(resultStr)
        if bestMatchID == predict.id:
            self._succNum = self._succNum + 1
            print("Correct Match\n\n")
        else:
            self._failNum = self._failNum + 1
            self._wrong_list.append(resultStr)
            print("Error Match\n\n")

    def __initDB__(self):
        print("Init PalmprintCodeDB")
       
        self._palmprintcodeRepository.startExecute()
        for train in self._factory.trainGroup:
            results = self._palmprintcodeRepository.selectPalmprintByIDInstamceID(train.id, train.instanceID)
            if len(results):
                continue
            codingBytes, codingLen = self._edcc_api.GetEDCCCoding(train.imagePath, self._configPath)
            self._palmprintcodeRepository.insertPalmprint(train, codingBytes)
            print("Insert ID:%s\tinstanceID:%s\tImagePath:%s" % (train.id, train.instanceID, train.imagePath))
        self._palmprintcodeRepository.endExecute()
    
    def __readDB__(self):
        print("Read PalmprintCodeDB")
        self._palmprintcodeRepository.startExecute()
        allPalmprintCodeData = self._palmprintcodeRepository.selectAllPalmprint()
        self.palmprintcodelist = []
        for palmprint in allPalmprintCodeData:
            palmprintcode = PalmprintCodeDTO(palmprint[0], palmprint[1], palmprint[2], palmprint[3])
            self.palmprintcodelist.append(palmprintcode)
        self._palmprintcodeRepository.endExecute()

if __name__ == '__main__':
    sample = EDCCSample()
    sample.runSample()
