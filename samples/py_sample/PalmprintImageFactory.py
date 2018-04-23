#!/usr/bin/python  
# -*- coding: utf-8 -*

import os
from random import *

class PalmprintDTO(object):
    def __init__(self, ID, instanceID, imagePath):
        self.id = ID
        self.instanceID = instanceID
        self.imagePath = imagePath

edcc_database_base_path = os.path.normpath(os.path.join(os.getcwd(), "../../database"))

'''
Construct palmprint image list
'''
tongji_palmprint_image_dir = os.path.normpath(os.path.join(edcc_database_base_path, "Tongji/ROI"))
tongji_people_num = 10
tongji_one_session_image_num = 10

polyu_palmprint_image_dir = os.path.normpath(os.path.join(edcc_database_base_path, "PolyU"))
polyu_one_spec_people_num = 10

class PalmprintImageFactory(object):
    def __init__(self, trainSetNum = 4):
        self.trainSetNum = trainSetNum
        if self.trainSetNum >= 12:
            raise RuntimeError("TrainSetNum can't be bigger than 12")
        self._palmprintAllGroup = []
        self._palmprintDict = {}
        self.trainGroup = []
        self.predictGroup = []
        
        self._loadTongjiPalmprintImageList()
        self._loadPolyUPalmprintImageList()
        self._genPalmprintGroupDict()
        self._genTrainAndPredictGroup()
    
    def _loadTongjiPalmprintImageList(self):
        if not os.path.exists(tongji_palmprint_image_dir):
            raise RuntimeError("%s palmprint data dir not exsits" % tongji_palmprint_image_dir)
        for p in range(0, tongji_people_num):
            for i in range(0, tongji_one_session_image_num):
                for session in ["session1", "session2"]:
                    sessionDir = os.path.join(tongji_palmprint_image_dir, session)
                    ID = "tongji_%d" % (p+1)
                    instanceID = "%s_%d" % (session, i+1)
                    imageName = "%05d.bmp" % (p * tongji_one_session_image_num + i + 1)
                    palmprint = PalmprintDTO(ID, instanceID, os.path.join(sessionDir, imageName))
                    #print("ID:%s\tinstanceID:%s\tImagePath:%s" % (palmprint.id, palmprint.instanceID, palmprint.imagePath))
                    self._palmprintAllGroup.append(palmprint)
        
    def _loadPolyUPalmprintImageList(self):
        if not os.path.exists(polyu_palmprint_image_dir):
            raise RuntimeError("%s palmprint data dir not exsits" % polyu_palmprint_image_dir)
        for spec in ['Multispectral_I', 'Multispectral_G']:
            specDir = os.path.join(polyu_palmprint_image_dir, spec)
            for p in range(0, polyu_one_spec_people_num):
                specIDDir = os.path.join(specDir, "%03d" % (p + 1))
                for j in range(0, 6):
                    for i in ['1', '2']:
                        ID = "%s_%d" % (spec, p+1)
                        instanceID = "%s_%02d" % (i, j+1)
                        imageName = "%s_%02d_s.bmp" % (i, j+1)
                        palmprint = PalmprintDTO(ID, instanceID, os.path.join(specIDDir, imageName))
                        #print("ID:%s\tinstanceID:%s\tImagePath:%s" % (palmprint.id, palmprint.instanceID, palmprint.imagePath))
                        self._palmprintAllGroup.append(palmprint)

    def _genPalmprintGroupDict(self):
        for palmprint in self._palmprintAllGroup:
            if palmprint.id not in self._palmprintDict.keys():
                self._palmprintDict[palmprint.id] = []
            self._palmprintDict[palmprint.id].append(palmprint)
        #print(self._palmprintDict)

    def _genTrainAndPredictGroup(self):
        for ID, palmprints in self._palmprintDict.items():
            if len(palmprints) <= self.trainSetNum:
                raise RuntimeError("ID:%s palmprint number samller than %d" % (ID, self.trainSetNum))
            x = [i for i in range(len(palmprints))]
            #shuffle(x)
            for n in range(len(palmprints)):
                if n < self.trainSetNum:
                    self.trainGroup.append(palmprints[x[n]])
                else:
                    self.predictGroup.append(palmprints[x[n]])

if __name__ == '__main__':
    factory = PalmprintImageFactory()
