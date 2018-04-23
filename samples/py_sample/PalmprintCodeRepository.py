#!/usr/bin/python  
# -*- coding: utf-8 -*

import os
import sqlite3
from PalmprintImageFactory import *

class PalmprintCodeRepo(object):
    def __init__(self, dbPath):
        self._dbPath = dbPath
        self._createDB()
        self._createTable()
        self._bStartExecute = False
        pass

    def _createDB(self):
        if not os.path.isfile(self._dbPath):
            open(self._dbPath, 'w')
    
    def _createTable(self):
        conn = sqlite3.connect(self._dbPath)
        create_tb_cmd="\
        CREATE TABLE IF NOT EXISTS PALMPRINTCODE\
        (ID TEXT,\
        InstanceID TEXT,\
        ImagePath TEXT,\
        Code BLOB,\
        PRIMARY KEY(ID, InstanceID));\
        "
        conn.execute(create_tb_cmd)
        conn.commit()
        conn.close()

    def startExecute(self):
        self.conn = sqlite3.connect(self._dbPath)
        self._bStartExecute = True
    def endExecute(self):
        self.conn.commit()
        self.conn.close()
        self._bStartExecute = False

    def insertPalmprint(self, palmprint, code):
        if not self._bStartExecute:
            raise RuntimeError("Please confirm that you have called \"startExecute\"")
        insert_tb_cmd="\
        INSERT INTO PALMPRINTCODE(ID, InstanceID, ImagePath, Code)\
        VALUES (\"%s\", \"%s\", \"%s\", ?)\
        " % (palmprint.id, palmprint.instanceID, palmprint.imagePath)
        self.conn.execute(insert_tb_cmd, (bytes(code),))
    
    def delPalmprint(self):
        if not self._bStartExecute:
            raise RuntimeError("Please confirm that you have called \"startExecute\"")
    
    def selectPalmprintByID(self, ID):
        if not self._bStartExecute:
            raise RuntimeError("Please confirm that you have called \"startExecute\"")
        try:
            cursor = self.conn.cursor()
            select_tb_cmd="\
            SELECT * FROM PALMPRINTCODE WHERE ID=\"%s\"\
            " % ID
            results = cursor.execute(select_tb_cmd)
            allPalmprint = results.fetchall()
            return allPalmprint
        except:
            return []
    
    def selectPalmprintByIDInstanceID(self, ID, InstanceID):
        if not self._bStartExecute:
            raise RuntimeError("Please confirm that you have called \"startExecute\"")
        try:
            cursor = self.conn.cursor()
            select_tb_cmd="\
            SELECT * FROM PALMPRINTCODE WHERE ID=\"%s\" AND InstanceID=\"%s\"\
            " % (ID, InstanceID)
            results = cursor.execute(select_tb_cmd)
            allPalmprint = results.fetchall()
            return allPalmprint
        except:
            return []
    
    def selectAllPalmprint(self):
        if not self._bStartExecute:
            raise RuntimeError("Please confirm that you have called \"startExecute\"")
        try:
            cursor = self.conn.cursor()
            select_tb_cmd="\
            SELECT * FROM PALMPRINTCODE\
            "
            results = cursor.execute(select_tb_cmd)
            allPalmprint = results.fetchall()
            return allPalmprint
        except:
            raise RuntimeError("selectAllPalmprint failed")