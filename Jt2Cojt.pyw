#!/usr/bin/python
# -*- coding: UTF-8 -*-

import os
import shutil

JtList = []
CojtList = []

def GetJtList():

	JtListTemp = os.listdir()

	for file in JtListTemp:
		if file.endswith(".jt"):
			JtList.append(file)

def CojtNameGet(filename):
	(filepath,tempfilename) = os.path.split(filename)
	(shotname,extension) = os.path.splitext(tempfilename)
	return shotname

def MakeDirCojt():

	for file in JtList:
		CojtName = CojtNameGet(file) + ".cojt"
		CojtList.append(CojtName)

	for file in CojtList:
		if os.path.isdir(file) == False:
			os.mkdir(file)

def MoveJtToCojt():
	for x in range(len(JtList)):
		shutil.move(JtList[x],CojtList[x])

def Jt2Cojt():
	GetJtList()
	MakeDirCojt()
	MoveJtToCojt()

if __name__ == "__main__":

	Jt2Cojt()