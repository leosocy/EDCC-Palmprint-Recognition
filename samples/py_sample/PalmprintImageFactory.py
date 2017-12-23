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

'''
同济大学掌纹数据库
包含了来自 300 个人（男 192 人，女 108 人）的手掌图像，其中 235名受试者为 20~30 岁，其余为 30~50 岁）
整个采集过程分为两次，第一次和第二次之间的平均时间间隔约为 61 天，最大和最短时间间隔分别为 106 天和 21 天
每次采集两只手掌的20 张图片，因此每个受试者采集了来自 2 只手掌的 40 张图像，一共包含了 12000 个图像样本。
可以当作600个人，每个人20张图片。
'''
tongji_palmprint_image_dir = os.path.normpath(os.path.join(edcc_database_base_path, "Tongji/ROI"))
tongji_people_num = 600
tongji_one_session_image_num = 10

'''
多光谱掌纹数据库：多光谱掌纹数据库包含四个光谱（红、绿、蓝和近红外 NIR）。
从250 个受试者（男 195 人，女 55 人）收集了每个频谱的数据库。年龄分布为 20 至 60 岁。
整个采集过程分为两次，每次采集两只手掌的共 12 张图像。因此，对每名志愿者在每个光谱下采集了来自 2 只手掌的 24 张图像。
总的来说数据库包含 4 个光谱，每个光谱包括 500 个不同手掌的 6000 张图像。
取G I当作两组不同的人，所以，PolyU数据库可以当作1000个人，每个人12张图像。
'''
polyu_palmprint_image_dir = os.path.normpath(os.path.join(edcc_database_base_path, "PolyU"))
polyu_one_spec_people_num = 500

class PalmprintImageFactory(object):
    def __init__(self, trainSetNum = 8):
        self.trainSetNum = trainSetNum
        self._palmprintAllGroup = []
        self._palmprintDict = {}
        self.trainGroup = []
        self.predictGroup = []
        
        #self.__loadTongjiPalmprintImageList__()
        self.__loadPolyUPalmprintImageList__()
        self.__genPalmprintGroupDict__()
        self.__genTrainAndPredictGroup__()
    
    def __loadTongjiPalmprintImageList__(self):
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
        
    def __loadPolyUPalmprintImageList__(self):
        if not os.path.exists(polyu_palmprint_image_dir):
            raise RuntimeError("%s palmprint data dir not exsits" % polyu_palmprint_image_dir)
        for spec in ['Multispectral_I']:
            specDir = os.path.join(polyu_palmprint_image_dir, spec)
            for p in range(0, polyu_one_spec_people_num):
                specIDDir = os.path.join(specDir, "%03d" % (p + 1))
                for i in ['1', '2']:
                    for j in range(0, 6):
                        ID = "%s_%d" % (spec, p+1)
                        instanceID = "%s_%02d" % (i, j+1)
                        imageName = "%s_%02d_s.bmp" % (i, j+1)
                        palmprint = PalmprintDTO(ID, instanceID, os.path.join(specIDDir, imageName))
                        #print("ID:%s\tinstanceID:%s\tImagePath:%s" % (palmprint.id, palmprint.instanceID, palmprint.imagePath))
                        self._palmprintAllGroup.append(palmprint)

    def __genPalmprintGroupDict__(self):
        for palmprint in self._palmprintAllGroup:
            if palmprint.id not in self._palmprintDict.keys():
                self._palmprintDict[palmprint.id] = []
            self._palmprintDict[palmprint.id].append(palmprint)
        #print(self._palmprintDict)

    def __genTrainAndPredictGroup__(self):
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