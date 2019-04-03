// OlpPostProcess.cpp : implementation file
//

#include	"stdafx.h"
#include	"afxdialogex.h"
#include	"RobotApplicationAssistant.h"
#include	"Spot.h"
#include	"OlpPostProcess.h"

// OlpPostProcess dialog
IMPLEMENT_DYNAMIC(OlpPostProcess, CDialogEx)

OlpPostProcess::OlpPostProcess(CWnd* pParent /*=NULL*/)
	: CDialogEx(OlpPostProcess::IDD, pParent)
	, OlpPostProcessOlpDir(_T(""))
	, OlpPostProcessParaFileDir(_T(""))
	, SelectSpotParaUpdate(FALSE)
{

}

OlpPostProcess::~OlpPostProcess()
{
}

void OlpPostProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MFCEditBrowseOlpPostProcessOlpDir, OlpPostProcessOlpDir);
	DDX_Text(pDX, IDC_MFCEditBrowseOlpPostProcessParaFileDir, OlpPostProcessParaFileDir);
	DDX_Check(pDX, IDC_SpotParaUpdate, SelectSpotParaUpdate);
}


BEGIN_MESSAGE_MAP(OlpPostProcess, CDialogEx)
	ON_BN_CLICKED(IDC_ButtonOlpPostProcess, &OlpPostProcess::OnBnClickedButtonolppostprocess)
END_MESSAGE_MAP()


// OlpPostProcess message handlers
void OlpPostProcess::OnBnClickedButtonolppostprocess()
{
	//get the choice from the group of radio button
	UpdateData(TRUE);

	//check if the input file path is avaliable
	if (OlpPostProcessOlpDir == "")
	{
		AfxMessageBox(_T("Failed to Get The Input OLP Dir !"), MB_OK, MB_ICONQUESTION);
		return;
	}
	else
	{
		AfxMessageBox(_T("The Target OLP Dir is :\n\n") + OlpPostProcessOlpDir, MB_OK, MB_ICONQUESTION);
	}

	//IF 'SelectSpotParaUpdate" Checked,then perform the fuction
	if(SelectSpotParaUpdate == TRUE)
	{
		//read the csv file and store the para list to vector
		SpotParaFileToVectorTemp(OlpPostProcessParaFileDir);

		//vector data check
		//SpotOrderParaVectorDataCheck(SpotOrderParaList);

		//write the data from vector to Olp file
		UpdateOlpOrderParaFromVector(OlpPostProcessOlpDir);

		AfxMessageBox(TEXT("OLP Spot Para Update Complete !"), MB_OK, MB_ICONQUESTION);
	}
}

//***********************************************************
//read the Para file��and store the para to vector structure
//***********************************************************
void	OlpPostProcess::SpotParaFileToVectorTemp(CString	SpotParaFileDir)
{
	//check if the input file path is avaliable
	if (SpotParaFileDir == "")
	{
		AfxMessageBox(TEXT("Failed to Get The Input Spot Para Dir !"), MB_OK, MB_ICONQUESTION);
		return;
	}
	else
	{
		AfxMessageBox(TEXT("The Target Spot Para Dir is :\n\n") + SpotParaFileDir, MB_OK, MB_ICONQUESTION);
	}

	//create Para file ergodicity variable
	CFileFind	SpotParaFileFinder;

	SpotParaFileDir = SpotParaFileDir + _T("\\*.*");

	BOOL bWorking = SpotParaFileFinder.FindFile(SpotParaFileDir);
	//AfxMessageBox(SpotParaFileFinder.GetFilePath(), MB_OK, MB_ICONQUESTION);

	while (bWorking)
	{
		//��ȡ��һ����������
		bWorking = SpotParaFileFinder.FindNextFile();
		//AfxMessageBox(SpotParaFileFinder.GetFileName(), MB_OK, MB_ICONQUESTION);

		//����������Ķ���ʱ�ļ��У���������ѭ����������һѭ��
		if (SpotParaFileFinder.IsDirectory())
		{
			//AfxMessageBox(SpotParaFileFinder.GetFilePath(), MB_OK, MB_ICONQUESTION);
			//AfxMessageBox(TEXT("SpotParaFileFinder.IsDirectory() == TRUE"), MB_OK, MB_ICONQUESTION);
			continue;
		}
		//��ȡ�ļ���
		else if (SpotParaFileFinder.GetFileName().MakeLower().Right(3) != _T("csv"))
		{
			//AfxMessageBox(SpotParaFileFinder.GetFileName(), MB_OK, MB_ICONQUESTION);
			//AfxMessageBox(TEXT("#5_SpotParaFileFinder.GetFileName().MakeLower().Right(3) == csv"), MB_OK, MB_ICONQUESTION);
			continue;
		}
		else
		{
			//AfxMessageBox(SpotParaFileFinder.GetFileName(), MB_OK, MB_ICONQUESTION);

			//���庸�Ӳ����ļ�����
			CStdioFile	SpotParaCsvFile;

			//�������ַ�����������
			CString	SingleLineString;

			//�򿪺��Ӳ����ļ�
			SpotParaCsvFile.Open(SpotParaFileFinder.GetFilePath(), CStdioFile::modeRead | CStdioFile::typeText);
			//AfxMessageBox(SpotParaFileFinder.GetFilePath(), MB_OK, MB_ICONQUESTION);

			//��ʼ����ָ��λ��
			SpotParaCsvFile.SeekToBegin();

			//ѭ����ȡÿ�����ݣ���ת��
			//arg01:���岢��ʼ���м�������
			//arg02:��SpotPara.csv�ж�ȡһ�У�������ؿ�������ѭ��
			//arg03:������+1
			for (int RowCounter = 0; SpotParaCsvFile.ReadString(SingleLineString); RowCounter++)
			{
				//�������У���Ϊ����Ϊ������
				if (RowCounter == 0)
				{
					continue;
				}

				//"��SpotPara.csv�ļ��ж�һ��"����arg02���Ѿ����
				//AfxMessageBox(SingleLineString, MB_OK, MB_ICONQUESTION);

				//������ʱ�������
				SpotOrderPara	Temp;

				//���嶺��λ�ü�����
				int Comma_Start = 0;
				int Comma_counter = 0;

				//�����
				Comma_counter = SingleLineString.Find(',', Comma_Start);
				Temp.SpotNumder = SingleLineString.Mid(Comma_Start, Comma_counter);
				Comma_Start = Comma_counter + 1;
				//AfxMessageBox(TEXT("SpotOrderPara.SpotNumber = ") + Temp.SpotNumder, MB_OK, MB_ICONQUESTION);

				//����P����
				Comma_counter = SingleLineString.Find(',', Comma_Start);
				Temp.P = SingleLineString.Mid(Comma_Start, Comma_counter - Comma_Start);
				Comma_Start = Comma_counter + 1;
				//AfxMessageBox(TEXT("SpotOrderPara.P = ") + Temp.P, MB_OK, MB_ICONQUESTION);

				//����t����
				Comma_counter = SingleLineString.Find(',', Comma_Start);
				Temp.t = SingleLineString.Mid(Comma_Start, Comma_counter - Comma_Start);
				Comma_Start = Comma_counter + 1;
				//AfxMessageBox(TEXT("SpotOrderPara.t = ") + Temp.t, MB_OK, MB_ICONQUESTION);

				//����S����
				Temp.S = SingleLineString.Mid(Comma_Start, SingleLineString.GetLength());
				//AfxMessageBox(TEXT("SpotOrderPara.S= ") + Temp.S, MB_OK, MB_ICONQUESTION);

				//��ÿ�еĸ���Ԫ�ر��浽Swap��
				SpotOrderParaList.push_back(Temp);
			}

			//�ر������ļ�
			SpotParaCsvFile.Close();
		}
	}

	////���vector�����������Ƿ���ȷ
	//for (int i = 0; i < SpotOrderParaList.size(); i++)
	//{
	//	AfxMessageBox(SpotOrderParaList[i].SpotNumder, MB_OK, MB_ICONQUESTION);
	//}
}

//****************************************************
//�������vector�ṹ�еĺ���������������ظ������޳�ͻ
//****************************************************
void	OlpPostProcess::SpotOrderParaVectorDataCheck(vector<SpotOrderPara>	Temp)
{
	//�������ǿ�
	//��麸��������ظ�
}

//*****************************************************************
//��ȡOLP�ļ������ݺ���ţ���vector���ݽṹ�в�ѯ��������д�뵽OLP�ļ���
//*****************************************************************
void	OlpPostProcess::UpdateOlpOrderParaFromVector(CString	OlpFileDir)
{
	//create log file and get it for ready
	OlpLsFilePostProcessLogFile.Open(OlpPostProcessOlpDir + TEXT("\\OlpPostProcess.log"), CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::typeText);

	//����Para�ļ���������
	CFileFind	OlpFileFinder;

	OlpFileDir = OlpFileDir + _T("\\*.*");

	BOOL bWorking = OlpFileFinder.FindFile(OlpFileDir);
	//AfxMessageBox(OlpFileFinder.GetFilePath(), MB_OK, MB_ICONQUESTION);

	while (bWorking)
	{
		//��ȡ��һ����������
		bWorking = OlpFileFinder.FindNextFile();
		//AfxMessageBox(OlpFileFinder.GetFileName(), MB_OK, MB_ICONQUESTION);

		//����������Ķ���ʱ�ļ��У���������ѭ����������һѭ��
		if (OlpFileFinder.IsDirectory())
		{
			//AfxMessageBox(OlpFileFinder.GetFilePath(), MB_OK, MB_ICONQUESTION);
			//AfxMessageBox(_T("OlpFileFinder.IsDirectory()"), MB_OK, MB_ICONQUESTION);
			continue;
		}

		//get the file name of Olp ls file
		else if (OlpFileFinder.GetFileName().MakeLower().Right(2) != _T("ls"))
		{
			//AfxMessageBox(OlpFileFinder.GetFileName(), MB_OK, MB_ICONQUESTION);
			//AfxMessageBox(TEXT("OlpFileFinder.GetFileName().MakeLower().Right(3) == ls"), MB_OK, MB_ICONQUESTION);
			continue;
		}

		//Update the para in ls file
		else
		{
			//AfxMessageBox(OlpFileFinder.GetFileName(), MB_OK, MB_ICONQUESTION);

			OlpLsFilePostProcessLogFile.WriteString(TEXT("In ") + OlpFileFinder.GetFileName() + TEXT(":\n"));

			CString	OlpLsName;
			vector<CString>	OlpLsTempForEdit;
			int LineCounterOfOlpLsFile = 0;

			//**********************************************
			//store OLP ls file to vector<CString>
			//**********************************************
			CStdioFile OlpLsFile;
			CString	SingleLineOfOlpLsFile;
			CString	OlpLsFileNameTemp;

			OlpLsFileNameTemp = OlpFileFinder.GetFileName();

			if (OlpLsFile.Open(OlpFileFinder.GetFilePath(), CStdioFile::modeRead | CStdioFile::typeText))
			{
				for (int i = 0; ; i++)
				{
					OlpLsFile.ReadString(SingleLineOfOlpLsFile);

					if (!SingleLineOfOlpLsFile.IsEmpty())
					{
						OlpLsTempForEdit.push_back(SingleLineOfOlpLsFile);
					}

					//SingleLineOfOlpLsFile = TEXT("");
					LineCounterOfOlpLsFile++;

					if (SingleLineOfOlpLsFile.Left(4) == TEXT("/END"))
					{
						OlpLsFile.Close();
						break;
					}
				}

				OlpLsFile.Close();
			}

			//delete the Olp ls file (regenerate it later)
			OlpLsFile.Remove(OlpLsFile.GetFilePath());

			//CString   temp;
			//temp.Format(_T("%d"), LineCounterOfOlpLsFile);
			//AfxMessageBox(temp, MB_OK, MB_ICONQUESTION);

			////���vector�����������Ƿ���ȷ
			//for (int i = 0; i < LineCounterOfOlpLsFile; i++)
			//{
			//	AfxMessageBox(OlpLsTempForEdit[i], MB_OK, MB_ICONQUESTION);
			//}

			//***************************************************************************
			//Update the para in Olp ls vector
			//***************************************************************************

			////check if the vector data is ok
			//CString   temp;
			//temp.Format(_T("%d"), LineCounterOfOlpLsFile);
			//AfxMessageBox(TEXT("LineCounterOfOlpLsFile = ") + temp, MB_OK, MB_ICONQUESTION);

			//update the para in vector
			for (int i = 0; i < LineCounterOfOlpLsFile; i++)
			{
				CString	SingleLineOfOlpLsFile;

				//get single line date from vector
				SingleLineOfOlpLsFile = OlpLsTempForEdit[i];

				int LineNumberOfOlpLsFile = i;
				//AfxMessageBox(SingleLineOfOlpLsFile, MB_OK, MB_ICONQUESTION);

				int	SpotNumStartCounter;
				int	SpotNumEndCounter;
				CString	SpotNumberTemp;

				//search for target line
				if (SingleLineOfOlpLsFile.Find(TEXT("/POS")) != -1)
				{
					break;
				}
				else if ((SingleLineOfOlpLsFile.Find(TEXT("P[")) != -1))
				{
					//search for target SpotNumber
					SpotNumStartCounter = SingleLineOfOlpLsFile.Find(TEXT("P["), 0);

					SpotNumStartCounter = SingleLineOfOlpLsFile.Find(TEXT(":"), SpotNumStartCounter);
					if (SpotNumStartCounter == -1)
					{
						//AfxMessageBox(TEXT("Syntax error detected !\nMissing ':' !"), MB_OK, MB_ICONQUESTION);
						continue;
					}

					SpotNumEndCounter = SingleLineOfOlpLsFile.Find(TEXT("]"), SpotNumStartCounter);
					if (SpotNumEndCounter == -1)
					{
						//AfxMessageBox(TEXT("Syntax error detected !\nMissing ']' !"), MB_OK, MB_ICONQUESTION);
						continue;
					}

					SpotNumberTemp = SingleLineOfOlpLsFile.Mid(SpotNumStartCounter + 1, SpotNumEndCounter - SpotNumStartCounter - 1);
					//AfxMessageBox(TEXT("SpotNumberTemp = ") + SpotNumberTemp, MB_OK, MB_ICONQUESTION);

					//search for target SpotNumber in Para vector
					int	SpotParaIndex;

					for (int i=0;i < SpotOrderParaList.size();i++)
					{
						if (SpotOrderParaList[i].SpotNumder == SpotNumberTemp)
						{
							CString   temp;
							temp.Format(_T("%d"), i);
							//AfxMessageBox(TEXT("SpotOrderParaList[")+ temp +TEXT("].SpotNumder = ") + SpotOrderParaList[i].SpotNumder, MB_OK, MB_ICONQUESTION);

							SpotParaIndex = i;

							//***********************************************
							//fetch the para from Para vector
							//***********************************************

							//locate and extract

							//get single line date from vector
							SingleLineOfOlpLsFile = OlpLsTempForEdit[LineNumberOfOlpLsFile + 1];

							int	SpotParaStartCounter = 0;
							int	SpotParaEndCounter = 0;
							CString	SpotParaReplaceTemp;

							//search for target SpotNumber and Update(replace) it

							//P
							SpotParaStartCounter = SingleLineOfOlpLsFile.Find(TEXT("P="), 0);
							SpotParaEndCounter = SingleLineOfOlpLsFile.Find(TEXT(","), SpotParaStartCounter);
							SpotParaReplaceTemp = SingleLineOfOlpLsFile.Mid(SpotParaStartCounter + 2, SpotParaEndCounter - SpotParaStartCounter - 2);
							//SpotParaStartCounter = SpotParaEndCounter + 1;
							//AfxMessageBox(TEXT("SpotParaReplaceTemp_P = ") + SpotParaReplaceTemp, MB_OK, MB_ICONQUESTION);
							//AfxMessageBox(TEXT("SingleLineOfOlpLsFile = ") + SingleLineOfOlpLsFile, MB_OK, MB_ICONQUESTION);

							//Update P para
							SingleLineOfOlpLsFile.Replace(TEXT("P=") + SpotParaReplaceTemp, TEXT("P=") + SpotOrderParaList[SpotParaIndex].P);
							SpotParaStartCounter = SingleLineOfOlpLsFile.Find(TEXT("P="), 0);
							SpotParaEndCounter = SingleLineOfOlpLsFile.Find(TEXT(","), SpotParaStartCounter);
							SpotParaReplaceTemp = SingleLineOfOlpLsFile.Mid(SpotParaStartCounter + 2, SpotParaEndCounter - SpotParaStartCounter - 2);
							//SpotParaStartCounter = SpotParaEndCounter + 1;
							//AfxMessageBox(TEXT("SpotParaReplaceTemp_P = ") + SpotParaReplaceTemp, MB_OK, MB_ICONQUESTION);
							//AfxMessageBox(TEXT("SingleLineOfOlpLsFile = ") + SingleLineOfOlpLsFile, MB_OK, MB_ICONQUESTION);

							//t
							SpotParaStartCounter = SingleLineOfOlpLsFile.Find(TEXT("t="), 0);
							SpotParaEndCounter = SingleLineOfOlpLsFile.Find(TEXT(","), SpotParaStartCounter);
							SpotParaReplaceTemp = SingleLineOfOlpLsFile.Mid(SpotParaStartCounter + 2, SpotParaEndCounter - SpotParaStartCounter - 2);
							//SpotParaStartCounter = SpotParaEndCounter + 1;
							//AfxMessageBox(TEXT("SpotParaReplaceTemp_t = ") + SpotParaReplaceTemp, MB_OK, MB_ICONQUESTION);
							//AfxMessageBox(TEXT("SingleLineOfOlpLsFile = ") + SingleLineOfOlpLsFile, MB_OK, MB_ICONQUESTION);

							//Update t para
							SingleLineOfOlpLsFile.Replace(TEXT("t=") + SpotParaReplaceTemp, TEXT("t=") + SpotOrderParaList[SpotParaIndex].t);
							SpotParaStartCounter = SingleLineOfOlpLsFile.Find(TEXT("t="), 0);
							SpotParaEndCounter = SingleLineOfOlpLsFile.Find(TEXT(","), SpotParaStartCounter);
							SpotParaReplaceTemp = SingleLineOfOlpLsFile.Mid(SpotParaStartCounter + 2, SpotParaEndCounter - SpotParaStartCounter - 2);
							//SpotParaStartCounter = SpotParaEndCounter + 1;
							//AfxMessageBox(TEXT("SpotParaReplaceTemp_t = ") + SpotParaReplaceTemp, MB_OK, MB_ICONQUESTION);
							//AfxMessageBox(TEXT("SingleLineOfOlpLsFile = ") + SingleLineOfOlpLsFile, MB_OK, MB_ICONQUESTION);

							//S
							SpotParaStartCounter = SingleLineOfOlpLsFile.Find(TEXT("S="), 0);
							SpotParaEndCounter = SingleLineOfOlpLsFile.Find(TEXT(","), SpotParaStartCounter);
							SpotParaReplaceTemp = SingleLineOfOlpLsFile.Mid(SpotParaStartCounter + 2, SpotParaEndCounter - SpotParaStartCounter - 2);
							//SpotParaStartCounter = SpotParaEndCounter + 1;
							//AfxMessageBox(TEXT("SpotParaReplaceTemp_S = ") + SpotParaReplaceTemp, MB_OK, MB_ICONQUESTION);
							//AfxMessageBox(TEXT("SingleLineOfOlpLsFile = ") + SingleLineOfOlpLsFile, MB_OK, MB_ICONQUESTION);

							//Update S para
							SingleLineOfOlpLsFile.Replace(TEXT("S=") + SpotParaReplaceTemp, TEXT("S=") + SpotOrderParaList[SpotParaIndex].S);
							SpotParaStartCounter = SingleLineOfOlpLsFile.Find(TEXT("S="), 0);
							SpotParaEndCounter = SingleLineOfOlpLsFile.Find(TEXT(","), SpotParaStartCounter);
							SpotParaReplaceTemp = SingleLineOfOlpLsFile.Mid(SpotParaStartCounter + 2, SpotParaEndCounter - SpotParaStartCounter - 2);
							//SpotParaStartCounter = SpotParaEndCounter + 1;
							//AfxMessageBox(TEXT("SpotParaReplaceTemp_S = ") + SpotParaReplaceTemp, MB_OK, MB_ICONQUESTION);
							//AfxMessageBox(TEXT("SingleLineOfOlpLsFile = ") + SingleLineOfOlpLsFile, MB_OK, MB_ICONQUESTION);

							//write back to vector
							//OlpLsTempForEdit[i + 1] = TEXT("");
							OlpLsTempForEdit[LineNumberOfOlpLsFile + 1] = SingleLineOfOlpLsFile;

							break;
						}
						
						if ((i >= (SpotOrderParaList.size() - 1)) && (SpotOrderParaList[i].SpotNumder != SpotNumberTemp))
						{
							//LOG ,can't find the spot number in para vector
							CString   temp;
							temp.Format(_T("%d"), LineNumberOfOlpLsFile + 1);
							OlpLsFilePostProcessLogFile.WriteString(TEXT("	Failed to find the Spot Para infomation of Spot Number ") + SpotNumberTemp + TEXT(","));
							OlpLsFilePostProcessLogFile.WriteString(TEXT("in Line Number ") + temp + TEXT(","));
							OlpLsFilePostProcessLogFile.WriteString(TEXT("please consult the supplyer !\n"));
							//AfxMessageBox(TEXT("Failed to fine the target para in Para File !"), MB_OK, MB_ICONQUESTION);
							continue;
						}
					}
				}
			}

			//*******************************************************************************************
			//create ls with origin name,and write vector<CString> back into ls file
			//*******************************************************************************************

			CStdioFile	OlpLsFileDest;

			//AfxMessageBox(TEXT("OlpLsFileDest.path = ") + OlpPostProcessOlpDir + TEXT("\\") + OlpLsFileNameTemp, MB_OK, MB_ICONQUESTION);

			if (OlpLsFileDest.Open(OlpPostProcessOlpDir + TEXT("\\") + OlpLsFileNameTemp, CStdioFile::modeCreate | CStdioFile::modeWrite | CStdioFile::typeText))
			{
				for (int i = 0; i < LineCounterOfOlpLsFile ; i++)
				{
					//AfxMessageBox(TEXT("OlpLsTempForEdit[i] = ") + OlpLsTempForEdit[i], MB_OK, MB_ICONQUESTION);
					OlpLsFileDest.WriteString(OlpLsTempForEdit[i] + TEXT("\n"));
				}

				OlpLsFileDest.Close();
			}

			OlpLsFilePostProcessLogFile.WriteString(TEXT("\n"));
		}
	}

	OlpLsFilePostProcessLogFile.Close();
}