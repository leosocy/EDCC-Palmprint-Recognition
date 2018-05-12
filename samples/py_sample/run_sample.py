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
        self._configPath = os.path.normpath(
            os.path.join(os.getcwd(), "edcc_config/config.json"))
        self._succNum = 0
        self._failNum = 0
        self._wrong_list = []

    def runSample(self):
        self._initDB()
        self._readDB()
        predictGroup = self._factory.predictGroup
        total_cost_time = 0.0
        total_match_count = 0
        for predict in predictGroup:
            predictPalmprintCode, codingLen = self._edcc_api.GetEDCCCoding(
                predict.imagePath, self._configPath)
            K = 3
            topKMatchScore = []
            timeBegin = time.time()
            for trainPalmprintCode in self.palmprintcodelist:
                matchScore = self._edcc_api.GetTwoPalmprintCodingMatchScore(
                    predictPalmprintCode, trainPalmprintCode.code)
                dictTmp = {}
                dictTmp["instance"] = trainPalmprintCode
                dictTmp["score"] = matchScore
                topKMatchScore.append(dictTmp)
            timeEnd = time.time()
            costTime = (timeEnd - timeBegin) * 1000
            total_cost_time += costTime
            total_match_count += 1

            topKMatchScore = sorted(
                topKMatchScore, key=lambda p: p["score"], reverse=True)
            while len(topKMatchScore) > K:
                topKMatchScore.pop()
            self.statisticsResult(predict, topKMatchScore, costTime)
        print(
            "\n\n=========================================================================")
        print("Predict Over.\nTotal:%d\tPredictCorrect:%d\tAccuracy:%lf%%" % (
            len(predictGroup), self._succNum, float(self._succNum) / len(predictGroup) * 100))
        print("Total Cost Time:%lf\tMatch Count:%d\tPer Cost Time:%lf" % (
            total_cost_time, total_match_count, total_cost_time / total_match_count))
        if self._wrong_list:
            print("Wrong List:")
            for record in self._wrong_list:
                print(record)
        print(
            "=========================================================================\n\n")

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
        # for ID in resultsDict.keys():
        #    resultsDict[ID] = resultsDict[ID] / float(idCountDict[ID])
        resultsDict = sorted(resultsDict.items(),
                             key=lambda r: r[1], reverse=True)
        bestMatchID = resultsDict[0][0]
        bestMatchScore = resultsDict[0][1]
        predictHeadStr = "Predict:"
        trainStr = "ID:%s   InstanceID:%s" % (predict.id, predict.instanceID)
        predictStr = "BestMatch:  ID:%s   MatchScore:%lf  CostTime:%lfms" % (
            bestMatchID, bestMatchScore, costTime)
        resultLineMaxLen = max(len(trainStr), len(
            predictStr), len(predictStr)) + 5
        print('-'*resultLineMaxLen)
        print('| '+predictHeadStr+(resultLineMaxLen-len(predictHeadStr)-3)*' '+'|')
        print('| '+trainStr+(resultLineMaxLen-len(trainStr)-3)*' '+'|')
        print('| '+predictStr+(resultLineMaxLen-len(predictStr)-3)*' '+'|')
        if bestMatchID == predict.id:
            self._succNum = self._succNum + 1
            print('| '+'Correct Match' +
                  (resultLineMaxLen-len('Correct Match')-3)*' '+'|')
        else:
            self._failNum = self._failNum + 1
            self._wrong_list.append(resultStr)
            print('| '+'Error Match'+(resultLineMaxLen-len('Error Match')-3)*' '+'|')
        print('-'*resultLineMaxLen+'\n\n')

    def _initDB(self):
        print("Init PalmprintCodeDB")

        self._palmprintcodeRepository.startExecute()
        for train in self._factory.trainGroup:
            results = self._palmprintcodeRepository.selectPalmprintByIDInstanceID(
                train.id, train.instanceID)
            if len(results):
                continue
            codingBytes, codingLen = self._edcc_api.GetEDCCCoding(
                train.imagePath, self._configPath)
            self._palmprintcodeRepository.insertPalmprint(train, codingBytes)
            print("Insert ID:%s\tinstanceID:%s\tImagePath:%s" %
                  (train.id, train.instanceID, train.imagePath))
        self._palmprintcodeRepository.endExecute()

    def _readDB(self):
        print("Read PalmprintCodeDB")
        self._palmprintcodeRepository.startExecute()
        allPalmprintCodeData = self._palmprintcodeRepository.selectAllPalmprint()
        self.palmprintcodelist = []
        for palmprint in allPalmprintCodeData:
            palmprintcode = PalmprintCodeDTO(
                palmprint[0], palmprint[1], palmprint[2], palmprint[3])
            self.palmprintcodelist.append(palmprintcode)
        self._palmprintcodeRepository.endExecute()


if __name__ == '__main__':
    sample = EDCCSample()
    sample.runSample()
