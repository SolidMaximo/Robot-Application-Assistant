#!/usr/bin/python
# -*- coding: UTF-8 -*-

# GUI
from tkinter import *
from tkinter import filedialog
from tkinter import StringVar

# 
import os
import shutil
import numpy

# xls xlsx operation
import xlwt
import xlrd

# Global Var for input
FormerVersionSpotFile = []
CurrentVersionSpotFile = []

FormerVersionSpotList = []
CurrentVersionSpotList = []

# Global Var for Process
FormerUnChanged = []
FormerIdChanged = []
FormerDeChanged = []

CurrentUnChanged = []
CurrentIdChanged = []
CurrentAdChanged = []

# Global Var for Output
CombineList = []

# DebugFlag
DebugFlag = False

#
StatInfo  = "Xlsx Format for input : \n"
StatInfo += "-------------------------------------------------\n"
StatInfo += "|ID		|X	|Y	|Z	|\n"
StatInfo += "-------------------------------------------------\n"
StatInfo += "|STUCB001041	|1050.87	|82.4241	|146.989	|\n"
StatInfo += "-------------------------------------------------\n"
StatInfo += "|STUCB001253	|843.541	|84.9093	|161.66	|\n"
StatInfo += "-------------------------------------------------\n"
StatInfo += "|		And so on		|\n"
StatInfo += "-------------------------------------------------\n"

#Read Spot Info from xls
def ReadSpotInfo(SpotFileList,SpotInfoList):

	for i in range(0,len(SpotFileList)):
		
		data = xlrd.open_workbook(SpotFileList[i])

		names = data.sheet_names()
		table = data.sheet_by_name(names[0])

		nrows = table.nrows

		for x in range(0,nrows):

			ID = table.cell_value(x,0)
			Cx = table.cell_value(x,1)
			Cy = table.cell_value(x,2)
			Cz = table.cell_value(x,3)

			SingleSpotInfo = [ID,Cx,Cy,Cz]

			SpotInfoList.append(SingleSpotInfo)

	if DebugFlag == True:

		for x in range(1,len(SpotInfoList)):
			
			print("%10s	%10.3f	%10.3f	%10.3f" % (SpotInfoList[x][0],SpotInfoList[x][1],SpotInfoList[x][2],SpotInfoList[x][3]))

#Compare the position of two spot
def SpotPositionCompare(Spot_A,Spot_B):
	
	# X Y Z dist flag 
	CmpFlag = [False,False,False,False]

	# ID
	if (Spot_A[0] == Spot_B[0]):
		CmpFlag[0] = True

	# XYZ
	for x in range(1,len(CmpFlag)):

		if (numpy.square(Spot_A[x] - Spot_B[x]) <= 100):
			CmpFlag[x] = True

	# return value
	# 0 : same
	# 1 : pos same ,ID diff
	# 2 : all diff
	if ((CmpFlag[0] == True) and (CmpFlag[1] == True) and (CmpFlag[2] == True) and (CmpFlag[3] == True)):
		return 0
	elif ((CmpFlag[0] == False) and (CmpFlag[1] == True) and (CmpFlag[2] == True) and (CmpFlag[3] == True)):
		return 1
	else :
		return 2

#Compare Spot Info
def Compare():

	#
	FormerVersionSpotList.pop(0)
	CurrentVersionSpotList.pop(0)

	#
	TempList = FormerVersionSpotList
	
	#
	for i in range(0,len(CurrentVersionSpotList)):
		
		#
		TempSpot = CurrentVersionSpotList[i]

		#
		SetFlag = False

		#
		for j in range(0,len(FormerVersionSpotList)):

			#
			FlagTemp = SpotPositionCompare(CurrentVersionSpotList[i],FormerVersionSpotList[j])

			#
			if (FlagTemp == 0):
				FormerUnChanged.append(FormerVersionSpotList[j])
				CurrentUnChanged.append(CurrentVersionSpotList[i])
				TempList.pop(j)
				SetFlag = True
				break
			elif(FlagTemp == 1):
				FormerIdChanged.append(FormerVersionSpotList[j])
				CurrentIdChanged.append(CurrentVersionSpotList[i])
				TempList.pop(j)
				SetFlag = True
				break
			else:
				pass

		#
		if (SetFlag == False):
			CurrentAdChanged.append(CurrentVersionSpotList[i])
			pass

	for x in range(0,len(TempList)):
		FormerDeChanged.append(TempList[x])

#
def Combine():

	#
	Title = ["ID","X","Y","Z","Status","ID","X","Y","Z",]
	CombineList.append(Title)
	
	# UnChanged
	for i in range(0,len(CurrentUnChanged)):

		TempList = []
		
		for j in range(0,len(FormerUnChanged[i])):
			TempList.append(FormerUnChanged[i][j])

		TempList.append("无变化")

		for j in range(0,len(CurrentUnChanged[i])):
			TempList.append(CurrentUnChanged[i][j])

		CombineList.append(TempList)

	# IdChanged
	for i in range(0,len(CurrentIdChanged)):

		TempList = []
		
		for j in range(0,len(FormerIdChanged[i])):				
			TempList.append(FormerIdChanged[i][j])

		TempList.append("ID改变")

		for j in range(0,len(CurrentIdChanged[i])):				
			TempList.append(CurrentIdChanged[i][j])

		CombineList.append(TempList)

	# AdChanged
	for i in range(0,len(CurrentAdChanged)):

		TempList = []
		
		for j in range(0,4):
			
			TempList.append("")

		TempList.append("新增焊点")

		for j in range(0,len(CurrentAdChanged[i])):
			TempList.append(CurrentAdChanged[i][j])

		CombineList.append(TempList)

	# DeChanged
	for i in range(0,len(FormerDeChanged)):

		TempList = []
		
		for j in range(0,len(FormerDeChanged[i])):
			TempList.append(FormerDeChanged[i][j])

		TempList.append("删除焊点")

		for j in range(0,4):
			
			TempList.append("")

		CombineList.append(TempList)

#Report
def Result():

	#
	OutputFile = xlwt.Workbook()

	#
	table = OutputFile.add_sheet('Result')

	# 
	for i in range(0,len(CombineList)):
		for j in range(0,len(CombineList[i])):
			table.write(i,j,CombineList[i][j])

	#
	OutputFile.save('E:\\Output.xls')

#
def SFVC():

	# GUI Init
	SFVC = Tk()

	# GUI Setting
	SFVC.title("Spot File Version Compare")
	SFVC.geometry()
	SFVC.resizable(width=False, height=False)

	# GUI

	ForPreSimu = LabelFrame(SFVC,text='Information')
	DisplayInfo = Label(ForPreSimu, text=StatInfo,width=40, height=21, justify='left', anchor='nw').pack()
	ForPreSimu.pack(padx=10,pady=10,side=LEFT)

	ForPreSimu = LabelFrame(SFVC,text='Former Version File')
	button = Button(ForPreSimu,text="Click To Add",command=AddFormerSpotFile,width=15, height=1).pack(padx=10,pady=10)
	ForPreSimu.pack(padx=10,pady=10,side=TOP)

	ForPreSimu = LabelFrame(SFVC,text='Current Version File')
	button = Button(ForPreSimu,text="Click To Add",command=AddCurrentSpotFile,width=15, height=1).pack(padx=10,pady=10)
	ForPreSimu.pack(padx=10,pady=10,side=TOP)

	ForPreSimu = LabelFrame(SFVC,text='Choose Save Dir')
	button = Button(ForPreSimu,text="Click To Set",command=Btn_SetSaveDir,width=15, height=1).pack(padx=10,pady=10)
	ForPreSimu.pack(padx=10,pady=10,side=TOP)

	ForPreSimu = LabelFrame(SFVC,text='CrossDex')
	button = Button(ForPreSimu,text="Compare",command=Button_Compare,width=15, height=2).pack(padx=10,pady=10)
	ForPreSimu.pack(padx=10,pady=10,side=BOTTOM)

	# Main Loop
	SFVC.mainloop()

# 
def Report():
	
	# StatInfo  = "Former Version Spot File Count : "
	# StatInfo += len(FormerVersionSpotFile) + "\n"
	# StatInfo += "Curretn Version Spot File Count : "
	# StatInfo += len(CurrentVersionSpotFile) + "\n"
	# StatInfo += "\n"

	# StatInfo += "Former Version Spot Counter : "
	# StatInfo += len(FormerVersionSpotList) + "\n"
	# StatInfo += "Curretn Version Spot Counter : "
	# StatInfo += len(CurrentVersionSpotList) + "\n"
	# StatInfo += "\n"

	# StatInfo += "Spot UnChanged Counter : "
	# StatInfo += len(CurrentUnChanged) + "\n"
	# StatInfo += "Spot IdChanged Counter : "
	# StatInfo += len(CurrentIdChanged) + "\n"
	# StatInfo += "Spot AdChanged Counter : "
	# StatInfo += len(CurrentAdChanged) + "\n"
	# StatInfo += "Spot DeChanged Counter : "
	# StatInfo += len(FormerDeChanged) + "\n"
	# StatInfo += "\n"

	# StatInfo += "Output File : "
	# StatInfo += "E:\\Output.xls\n"

	pass

# 
def AddFormerSpotFile():

	SpotFile = filedialog.askopenfilename()
	FormerVersionSpotFile.append(SpotFile)
	ReadSpotInfo(FormerVersionSpotFile,FormerVersionSpotList)

# 
def AddCurrentSpotFile():

	SpotFile = filedialog.askopenfilename()
	CurrentVersionSpotFile.append(SpotFile)
	ReadSpotInfo(CurrentVersionSpotFile,CurrentVersionSpotList)

#
def Btn_SetSaveDir():
	pass

# 
def Button_Compare():
	Compare()
	Combine()
	Report()
	Result()

#
if __name__ == "__main__":

	SFVC()