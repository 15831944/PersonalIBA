// Scan2.cpp: implementation of the CScan2 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stdio.h>
#include "Scan2.h"
#include <tchar.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScan2::CScan2()
{
	m_bOpened = false;
	m_hDll = NULL;
	memset(m_cImageFilePath, 0, sizeof(m_cImageFilePath));
	memset(m_cColorImageFilePath, 0, sizeof(m_cColorImageFilePath));
	GetTempPath(MAX_PATH, m_cTempPath);
	sprintf(m_cImageFilePath, "%sSCANTEMP.BMP", m_cTempPath);
	sprintf(m_cHeadImageFilePath, "%sHEADTEMP.BMP", m_cTempPath);
	sprintf(m_cColorImageFilePath, "%sSCANCOLTEMP.BMP", m_cTempPath);
	sprintf(m_cColorHeadImageFilePath, "%sHEADCOLTEMP.BMP", m_cTempPath);
	
	Device_Open = NULL;
	Device_Close = NULL;
	Get_IdcPic = NULL;
	Get_IdcData = NULL;
	Format_ErrMsg = NULL;
	BmpToJpg = NULL;
	Get_TermbData = NULL;

}

CScan2::~CScan2()
{
	CloseDevice();
}

bool CScan2::IsDeviceValid()
{
	return true;
}

int CScan2::OpenDevice()
{
	int iError = ERROR_SCANNER_OTHER;
	if(m_hDll == NULL)
	{
		char cLibDir[MAX_PATH]={0};
		GetPrivateProfileString("SCAN_2", "LIB_DIR", "", cLibDir, sizeof(cLibDir), g_cIniFile);
		int pathLen=strlen(cLibDir);
		if(pathLen > 0 && cLibDir[pathLen-1]!='\\')
		{
			cLibDir[pathLen]='\\';
			pathLen++;
		}
		strcpy(cLibDir+pathLen,"IDRCore.dll");
		m_hDll = LoadLibrary(cLibDir);
	}
	if(m_hDll == NULL)
	{
		return ERROR_SCANNER_LOADDLL;
	}
	
	Device_Open = (Fun_Device_Open *)GetProcAddress(m_hDll, "Device_Open");
	Device_Close = (Fun_Device_Close *)GetProcAddress(m_hDll, "Device_Close");
	Get_IdcPic = (Fun_Get_IdcPic *)GetProcAddress(m_hDll, "Get_IdcPic");
	Get_MiscPic = (Fun_Get_MiscPic *)GetProcAddress(m_hDll, "Get_MiscPic");
	Get_MiscData = (Fun_Get_MiscData*)GetProcAddress(m_hDll, "Get_MiscData");
	Get_ColorPic = (Fun_Get_ColorPic *)GetProcAddress(m_hDll, "Get_ColorPic");
	Get_IdcData = (Fun_Get_IdcData *)GetProcAddress(m_hDll, "Get_IdcData");

	Format_ErrMsg = (Fun_Format_ErrMsg *)GetProcAddress(m_hDll, "Format_ErrMsg");
	BmpToJpg = (Fun_BmpToJpg *)GetProcAddress(m_hDll, "BmpToJpg");
	Get_TermbData = (Fun_Get_TermbData *)GetProcAddress(m_hDll, "Get_TermbData");

	if(IsBadCodePtr( (FARPROC)Get_ColorPic ))
	{
		m_iVer = 0x01;
	}
	else
	{
		m_iVer = 0x02;
	}

	if(IsBadCodePtr( (FARPROC)Device_Open ) || 
		IsBadCodePtr( (FARPROC)Device_Close ) || 
		IsBadCodePtr( (FARPROC)Get_IdcPic ) || 
		IsBadCodePtr( (FARPROC)Get_MiscPic ) || 
		IsBadCodePtr( (FARPROC)Get_MiscData ) ||
		IsBadCodePtr( (FARPROC)Get_IdcData ) || 
		IsBadCodePtr( (FARPROC)Format_ErrMsg ) ||
		IsBadCodePtr( (FARPROC)BmpToJpg ))
	{
		return ERROR_SCANNER_LOADDLL;
	}
	
	if(!m_bOpened)
	{
		if(Device_Open() == 0)
		{
			iError=ERROR_SCANNER_OK;
			m_bOpened = true;
		}
	}

	return iError;
}

int CScan2::CloseDevice()
{
	if(m_bOpened)
	{
		Device_Close();
		m_bOpened = false;
	}
	if(m_hDll != NULL)
	{
		FreeLibrary(m_hDll);
		m_hDll = NULL;
	}

	return ERROR_SCANNER_OK;
}

int CScan2::ScanImage(int idType, char *szImgFile)
{
	if(!m_bOpened)
	{
		return ERROR_SCANNER_HARDWARE;
	}

	int iError = -1;
	char cColorBuf[MAX_PATH] = "\0";
	strncpy(cColorBuf, szImgFile, MAX_PATH);
	char *p = strrchr(cColorBuf, '.');
	if(p != NULL)
	{
		strcpy(p, "_COL.JPG");
	}
	else
	{
		strcpy(cColorBuf, "SCANCOLOR.JPG");
	}

	switch(idType)
	{
	case IDENTITY_ID1:// 10 һ�����֤
		{
			iError = Get_IdcPic(1, m_cImageFilePath);
			if(iError == 0)
			{
				BmpToJpg(m_cImageFilePath, szImgFile);
			}
			
			if(m_iVer == 0x02)
			{
				iError = Get_ColorPic(1, m_cColorImageFilePath,"");
				if(iError == 0)
				{
//					BmpToJpg(m_cColorImageFilePath, cColorBuf);
					BmpToJpg(m_cColorImageFilePath, szImgFile);
				}
			}
			
		}
		break;

	case IDENTITY_ID2_BACK:// ����֤����	
	case IDENTITY_ID2:// 11 �������֤
		{
			iError = Get_IdcPic(2, m_cImageFilePath);
			if(iError == 0)
			{
				BmpToJpg(m_cImageFilePath, szImgFile);
			}
			
			if(m_iVer == 0x02)
			{
				iError = Get_ColorPic(2, m_cColorImageFilePath,"");
				if(iError == 0)
				{
//					BmpToJpg(m_cColorImageFilePath, cColorBuf);
					BmpToJpg(m_cColorImageFilePath, szImgFile);
				}
			}
			
		}
		break;
				
	case IDENTITY_PASSPORT:// 94 ����
		{
			iError = Get_MiscPic(3, m_cImageFilePath);
			if(iError == 0)
			{
				BmpToJpg(m_cImageFilePath, szImgFile);
			}
			
			if(m_iVer == 0x02)
			{
				iError = Get_ColorPic(3, m_cColorImageFilePath,"");
				if(iError == 0)
				{
//					BmpToJpg(m_cColorImageFilePath, cColorBuf);
					BmpToJpg(m_cColorImageFilePath, szImgFile);
				}
			}
		}
		break;
		
	case IDENTITY_TAIBAO:// 95 ̨��֤
		{
			iError = Get_MiscPic(9, m_cImageFilePath);
			if(iError == 0)
			{
				BmpToJpg(m_cImageFilePath, szImgFile);
			}
			
			if(m_iVer == 0x02)
			{
				iError = Get_ColorPic(9, m_cColorImageFilePath,"");
				if(iError == 0)
				{
//					BmpToJpg(m_cColorImageFilePath, cColorBuf);
					BmpToJpg(m_cColorImageFilePath, szImgFile);
				}
			}
		}
		break;

	case IDENTITY_GANGHAO:// �۰�ͨ��֤
		{
			iError = Get_MiscPic(6, m_cImageFilePath);
			if(iError == 0)
			{
				BmpToJpg(m_cImageFilePath, szImgFile);
			}

			if(m_iVer == 0x02)
			{
				iError = Get_ColorPic(6, m_cColorImageFilePath,"");
				if(iError == 0)
				{
					//					BmpToJpg(m_cColorImageFilePath, cColorBuf);
					BmpToJpg(m_cColorImageFilePath, szImgFile);
				}
			}
		}
		break;
		
	default:
		iError = Get_MiscPic(11, m_cImageFilePath);
		if(iError == 0)
		{
			BmpToJpg(m_cImageFilePath, szImgFile);
		}
		
		if(m_iVer == 0x02)
		{
			iError = Get_ColorPic(11, m_cColorImageFilePath,"");
			if(iError == 0)
			{
//					BmpToJpg(m_cColorImageFilePath, cColorBuf);
					BmpToJpg(m_cColorImageFilePath, szImgFile);
			}
		}
		
		break;
	}

	return iError==0 ? ERROR_SCANNER_OK : ERROR_SCANNER_OTHER;
}

int CScan2::RecogniseImage(int idType, IDENTITY_INFO &info, char *szHeadImage)
{
	if(!m_bOpened)
	{
		return ERROR_SCANNER_HARDWARE;
	}

	int iError = -1;
	int iConvertIdType = 11;
	struct ID_CARD_S stID_CARD_S = {0};
	struct JL_ITEM stJL_ITEM[256] = {0};

	switch(idType)
	{
	case IDENTITY_ID1:// 10 һ�����֤
		{
			iError = Get_IdcData(1, 
				m_cImageFilePath, 
				"", 
				m_cHeadImageFilePath, 
				&stID_CARD_S);
			
			if(iError == 0)
			{
				if(BmpToJpg(m_cHeadImageFilePath, szHeadImage) == 0)
				{
					SetFileAttributes(m_cHeadImageFilePath, FILE_ATTRIBUTE_NORMAL); 
					DeleteFile(m_cHeadImageFilePath);
				}
				SetFileAttributes(m_cImageFilePath, FILE_ATTRIBUTE_NORMAL); 
				DeleteFile(m_cImageFilePath);
			}
		}
		break;

	case IDENTITY_ID2_BACK:// ����֤����
	case IDENTITY_ID2:// 11 �������֤
		{
			iError = Get_IdcData(2, 
				m_cImageFilePath, 
				"", 
				m_cHeadImageFilePath, 
				&stID_CARD_S);
			
			if(iError == 0)
			{
				if(BmpToJpg(m_cHeadImageFilePath, szHeadImage) == 0)
				{
					SetFileAttributes(m_cHeadImageFilePath, FILE_ATTRIBUTE_NORMAL); 
					DeleteFile(m_cHeadImageFilePath);
				}
				SetFileAttributes(m_cImageFilePath, FILE_ATTRIBUTE_NORMAL); 
				DeleteFile(m_cImageFilePath);
			}
		}
		break;

	case IDENTITY_STUDENT:// 15 ѧ��֤
		{
			iConvertIdType = 11;
		}
		break;
		
	case IDENTITY_OFFICER:// 90 ����֤
		{
			iConvertIdType = 11;
		}
		break;
		
	case IDENTITY_POLICE:// 91 ����֤
		{
			iConvertIdType = 11;
		}
		break;
		
	case IDENTITY_SOLDIER:// 92 ʿ��֤
		{
			iConvertIdType = 11;
		}
		break;
		
	case IDENTITY_HUKOUBU:// 93 ���ڲ�
		{
			iConvertIdType = 11;
		}
		break;
		
	case IDENTITY_PASSPORT:// 94 ����
		{
			iConvertIdType = 3;
		}
		break;
		
	case IDENTITY_TAIBAO:// 95 ̨��֤
		{
			iConvertIdType = 9;
		}
		break;

	case IDENTITY_GANGHAO:
		{
			iConvertIdType = 6;
		}
		break;
		
	case IDENTITY_OTHER:// 99 ����֤��
		{
			iConvertIdType = 11;
		}
		break;
		
	default:
		break;
	}
	
	if(idType != IDENTITY_ID1 && idType != IDENTITY_ID2 && idType != IDENTITY_ID2_BACK)
	{
		iError = Get_MiscData(iConvertIdType, 
			m_cImageFilePath, 
			"", 
			m_cHeadImageFilePath, 
			stJL_ITEM);

		if(iError == 0)
		{
			if(BmpToJpg(m_cHeadImageFilePath, szHeadImage) == 0)
			{
				SetFileAttributes(m_cHeadImageFilePath, FILE_ATTRIBUTE_NORMAL); 
				DeleteFile(m_cHeadImageFilePath);
			}
			SetFileAttributes(m_cImageFilePath, FILE_ATTRIBUTE_NORMAL); 
			DeleteFile(m_cImageFilePath);
		}
	}

	if(iError == 0)
	{
		if(idType == IDENTITY_ID1 || idType == IDENTITY_ID2 || idType == IDENTITY_ID2_BACK)
		{
			strncpy(info.name, stID_CARD_S.szText[1], 20);     //����
			strncpy(info.sex, stID_CARD_S.szText[2], 2);        //�Ա�
			strncpy(info.number, stID_CARD_S.szText[8], 40);   //֤������
			strncpy(info.nation, stID_CARD_S.szText[4], 20);    //����
			strncpy(info.birthday, stID_CARD_S.szText[3], 10);  //��������
			strncpy(info.address, stID_CARD_S.szText[7], 200);  //��ַ
			strncpy(info.signdate, stID_CARD_S.szText[5], 10);  //ǩ������
			strncpy(info.validterm, stID_CARD_S.szText[6], 30);  //��Ч��
			strncpy(info.department, stID_CARD_S.szText[9], 40); //��֤����
		}
		else
		{
			//0-������
			//1-Ӣ����
			//2-������
			//3-Ӣ����
			//4-�Ա�(Ӣ��) �����M��F
			//5-��������
			//6-ǩ������
			//7-ǩ���ص�(����)
			//8-ǩ���ص�(Ӣ��)
			//9-���֤��
			//10-������(����)
			//11-������(Ӣ��)
			//12-��Ч����
			//13-֤������
			//14-���Ҵ���
			//15-��һ�л�������
			//16-��һ�л�������
			
			strncpy(info.name, stJL_ITEM[0].pzTxt, 20);     //��
			strncat(info.name, stJL_ITEM[2].pzTxt, 20);     //����

			if(strcmp(stJL_ITEM[4].pzTxt, "M") == 0)
			{
				strncpy(info.sex, "��", 2);        //�Ա�
			}
			else
			{
				strncpy(info.sex, "Ů", 2);        //�Ա�
			}
			strncpy(info.number, stJL_ITEM[13].pzTxt, 40);   //֤������

			strncpy(info.birthday, stJL_ITEM[5].pzTxt, 10);  //��������
			strncpy(info.address, stJL_ITEM[10].pzTxt, 200);  //��ַ
			strncpy(info.signdate, stJL_ITEM[6].pzTxt, 10);  //ǩ������
			strncpy(info.validterm, stJL_ITEM[12].pzTxt, 30);  //��Ч��
			strncpy(info.department, stJL_ITEM[7].pzTxt, 40); //��֤����
		}
		return ERROR_SCANNER_OK;
	}
	else
	{
		char szErrorMsg[256];
		GetErrorMessage(iError, szErrorMsg);
		int i = 0;
		i++;
	}
	
	return ERROR_SCANNER_OTHER;
}

bool CScan2::IsSupportHeadImage(int /*idType*/)
{
	return true;
}

bool CScan2::IsSupportRecognise(int idType)
{
	bool bSupport = false;

	switch(idType)
	{
	case IDENTITY_ID1:// 10 һ�����֤
		{
			bSupport = true;
		}
		break;

	case IDENTITY_ID2_BACK:
	case IDENTITY_ID2:// 11 �������֤
		{
			bSupport = true;
		}
		break;
		
	case IDENTITY_STUDENT:// 15 ѧ��֤
		{
		}
		break;
		
	case IDENTITY_OFFICER:// 90 ����֤
		{
		}
		break;
		
	case IDENTITY_POLICE:// 91 ����֤
		{
		}
		break;
		
	case IDENTITY_SOLDIER:// 92 ʿ��֤
		{
		}
		break;
		
	case IDENTITY_HUKOUBU:// 93 ���ڲ�
		{
		}
		break;
		
	case IDENTITY_PASSPORT:// 94 ����
		{
			bSupport = true;
		}
		break;
		
	case IDENTITY_TAIBAO:// 95 ̨��֤
		{
		}
		break;
		
	case IDENTITY_OTHER:// 99 ����֤��
		{
			bSupport = true;
		}
		break;
		
	default:
		break;
	}

	return 	bSupport;
}

bool CScan2::IsSupportCallBack()
{
	return false;
}

bool CScan2::IsSupportScanner()
{
	return true;
}

int CScan2::SetCallBack(CallBack /*pFunc*/)
{
	return 8;
}

void CScan2::GetErrorMessage(int errCode,char* szErrMsg)
{
	if(szErrMsg)
	{
		if(errCode < 0)
		{
			strcpy(szErrMsg, "�������ʧ��!");
		}
		else
		{
			Format_ErrMsg(errCode, &szErrMsg);
		}
	}
}

bool CScan2::IsSupportReader()
{
	if(IsBadCodePtr( (FARPROC)Get_TermbData ))
	{
		return false;
	}
	return true;
}

int CScan2::ReadCard(IDENTITY_INFO &info, char *szHeadImage)
{
	if(!IsSupportReader())
	{
		Beep(800, 500);	
		return ERROR_SCANNER_HARDWARE;
	}

	TERMB_ITEM stTERMB_ITEM = {0};
	//sprintf(m_cHeadImageFilePath, "%sHEADTEMP.BMP", "C:\\");
	TCHAR szOldCurDir[MAX_PATH], szTmp[MAX_PATH], szNewCurDir[MAX_PATH];
	ZeroMemory(szOldCurDir, sizeof(TCHAR) * MAX_PATH);
	ZeroMemory(szTmp, sizeof(TCHAR) * MAX_PATH);
	ZeroMemory(szNewCurDir, sizeof(TCHAR) * MAX_PATH);


	GetCurrentDirectory(MAX_PATH, szOldCurDir);
	GetModuleFileName(NULL, szTmp, MAX_PATH);

	TCHAR* pFind = _tcsrchr( szTmp, '\\' );
	if(pFind)
	{
		_tcsncpy(szNewCurDir, szTmp, pFind - szTmp);
	}
	SetCurrentDirectory(szNewCurDir);

	// ֻ�������˵�ǰĿ¼����Ĭ��Ŀ¼�����ڴ��ݽ�ȥ�ĵ�ַm_cHeadImageFilePath������ͼƬ
	// �����ǲ����õ�ǰĿ¼�����ڵ�ǰĿ¼����һ��Ŀ¼����photo.bmp

	int iError = Get_TermbData(m_cHeadImageFilePath, &stTERMB_ITEM);
	if(iError == 0)
	{
		strncpy(info.name, stTERMB_ITEM.pzTxt[0], 20);     //����
		strncpy(info.sex, stTERMB_ITEM.pzTxt[1], 2);        //�Ա�
		strncpy(info.number, stTERMB_ITEM.pzTxt[5], 40);   //֤������
		strncpy(info.nation, stTERMB_ITEM.pzTxt[2], 20);    //����
		strncpy(info.birthday, stTERMB_ITEM.pzTxt[3], 10);  //��������
		strncpy(info.address, stTERMB_ITEM.pzTxt[4], 200);  //��ַ
		strncpy(info.signdate, stTERMB_ITEM.pzTxt[7], 10);  //ǩ������
		strncpy(info.validterm, stTERMB_ITEM.pzTxt[8], 30);  //��Ч��
		strncpy(info.department, stTERMB_ITEM.pzTxt[6], 40); //��֤����
		
		if(BmpToJpg(m_cHeadImageFilePath, szHeadImage) == 0)
		{
			SetFileAttributes(m_cHeadImageFilePath, FILE_ATTRIBUTE_NORMAL); 
			DeleteFile(m_cHeadImageFilePath);
		}

		return ERROR_SCANNER_OK;
	}

	// �ָ�����
	SetCurrentDirectory(szOldCurDir);
	return ERROR_SCANNER_OTHER;
}
