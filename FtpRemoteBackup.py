#!/usr/bin/python
# -*- coding: UTF-8 -*-

import os
import datetime
import time
import socket
from ftplib import FTP

#目标FTP服务器IP地址
global FtpServerList
FtpServerList = [
					'192.168.1.200',
					'192.168.1.201',
					'192.168.1.202',
					'192.168.1.203',
					'192.168.1.204',
					'192.168.1.205',
					'192.168.1.206',
				]

#备份文件名存储数组
global TargetFileList

#本地备份目录
global LocalBackupDir
LocalBackupDir = 'D:\\RobotBackupDir\\'

#探测局域网是否存在FTP服务器
def DetectFtpServer(TargetAddr):

	#检测目标主机21端口是否开放
	#如果开放，判定目标主机为机器人
	#上述判断条件可能不充分，后续需要修改
	PortDetect = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	PortDetect.settimeout(1)
	try:
		PortDetect.connect(TargetAddr,21)
		print('Server port 21 of ' + TargetAddr + ' is Open!')
		FtpServerList.append(TargetAddr)
	except Exception:
		print('Server port 21 of ' + TargetAddr + ' is Open!')
		return 0
	PortDetect.close()
	return

def PrintFtpServerListToGUI():

	for i in range(0,len(FtpServerList)):
		print(FtpServerList[i])
	return
	
# def ChooseFtpServer():
# 	return

#扫描局域网FTP服务器，显示可用列表至GUI，供使用者勾选
#打印选择列表至主机名数组，逐个开始执行下述功能，空则推出循环
def SetTargetFtpServer():

	for i in range(0,255):
		DetectFtpServer('192.168.1.' + str(i))
# 	PrintFtpServerListToGUI()
# 	ChooseFtpServer()

#从ftp下载文件
def DownloadFile(ftp, RemotePath, LocalPath):

    bufsize = 1024
    FileHandle = open(LocalPath, 'wb')
    ftp.retrbinary('RETR ' + RemotePath, FileHandle.write, bufsize)
    FileHandle.close()

#逐个机器人进行FTP程序备份
def BackupFromFtpServer():

	for i in range(0,len(FtpServerList)):

		print('Backup for ' + FtpServerList[i] + ' Processing !')

		#实例化FTP对象
		ftp = FTP()
		#设定调试等级
		ftp.set_debuglevel(0)
		#连接FTP
		ftp.connect(FtpServerList[i],21)
		#打印欢迎信息
		ftp.getwelcome()
		#登录FTP
		try:
			ftp.login()
			#print('Succeed to login into ' + FtpServerList[i])
		except Exception:
			#print('Failed to login into ' + FtpServerList[i])
			return 0

		# #切换到根目录
		# ftp.cwd('\\')
		# #切换至备份目录
		# try:
		# 	ftp.cwd('md:\\')
		# except Exception:
		# 	print('Failed to cd to mc fir')
		# 	return 0
		
		#ftp.retrlines('LIST')

		#打印文件列表至预定义文件名数组
		TargetFileList = ftp.nlst()

		#设定机器人名称目录
		TargetRobotDir = LocalBackupDir + FtpServerList[i]

		#设定备份日期时间目录
		DateTimeStamp = datetime.datetime.now()
		DateTimeStamp = DateTimeStamp.strftime('%Y-%m-%d-%H-%M-%S')
		TargetBackupDir = TargetRobotDir + '\\' + DateTimeStamp

		#创建备份目录
		try:
			os.makedirs(TargetBackupDir)
		except Exception:
			print(TargetBackupDir + 'is already exist,delete or move it first')
			return 0

		#查询文件名数组，下载指定文件至本地文件夹，循环直到数组元素为空
		for j in range(0,len(TargetFileList)):
			DownloadFile(ftp,TargetFileList[j],TargetBackupDir + '\\' + TargetFileList[j])

		#断开连接
		ftp.quit()

		print('Backup for ' + FtpServerList[i] + ' Complete !')

	print('Backup is Complete !')

	#退出并打印报告
	return

#主程序
if __name__ == '__main__':
		
	#SetTargetFtpServer()
	BackupFromFtpServer()

	time.sleep(1)