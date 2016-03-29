// Scan2.cpp: implementation of the CScan9 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stdio.h>
#include "Scan9.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static const wchar_t SzUserID[] = L"66915733240624738915";		// �û�ID�������ͨ�Ƽ�����,�̶���
CScan9::CScan9()
	:m_OPort(false), m_nOpenPort(0)
{
	m_bReadOpened = false;
	m_nIdCardType = 2;
	m_bIsScanInitSuccess = FALSE;
	m_hDllScan = NULL;
	m_hDllRead = NULL;
	m_hDllGetBmp = NULL;
	m_nRetScanOpen = ERROR_SCANNER_OTHER;
	m_nRetReadCard = ERROR_SCANNER_OTHER;
	GetTempPath(MAX_PATH, m_cTempPath);

}

CScan9::~CScan9()
{
	CloseDevice();
}

bool CScan9::IsDeviceValid()
{
	return true;
}

int CScan9::OpenDevice()
{
	/*int nRetScan = OpenScanDevice();
	if(ERROR_SCANNER_OK != nRetScan)
	{
		return nRetScan;
	}ɨ��Ĵ��ٶ�̫����,���ܷ�������Ӱ��ɨ���ٶ�*/
	if(ERROR_SCANNER_OK != m_nRetReadCard)
	{
		m_nRetReadCard = OpenReadCardDevice();
	}

	return m_nRetReadCard;
}

int CScan9::OpenScanDevice()
{
	if(!m_bIsScanInitSuccess)
	{
		if(m_hDllScan == NULL)
		{
			char cLibDir[MAX_PATH]={0};
			GetPrivateProfileString("SCAN_9", "LIB_DIR", "", cLibDir, sizeof(cLibDir), g_cIniFile);
			int pathLen=strlen(cLibDir);
			if(pathLen > 0 && cLibDir[pathLen-1]!='\\')
			{
				cLibDir[pathLen]='\\';
				pathLen++;
			}
			strcpy(cLibDir+pathLen,"IDCard.dll");
			m_hDllScan = LoadLibrary(cLibDir);
		}

		if(m_hDllScan == NULL)
			return FALSE;

		//get the pointers of the function
		m_pfInitIDCard = (FUN_InitIDCard)GetProcAddress(m_hDllScan, "InitIDCard");
		m_pfFreeIDCard = (FUN_FreeIDCard)GetProcAddress(m_hDllScan, "FreeIDCard");
		m_pfSetAcquireImageType = (FUN_SetAcquireImageType)GetProcAddress(m_hDllScan, "SetAcquireImageType");

		m_pfSetUserDefinedImageSize = (FUN_SetUserDefinedImageSize)GetProcAddress(m_hDllScan, "SetUserDefinedImageSize");
		m_pfSetAcquireImageResolution = (FUN_SetAcquireImageResolution)GetProcAddress(m_hDllScan, "SetAcquireImageResolution");
		m_pfAcquireImage = (FUN_AcquireImage)GetProcAddress(m_hDllScan, "AcquireImage");

		m_pfSaveImage = (FUN_SaveHeadImage)GetProcAddress(m_hDllScan, "SaveImage");
		m_pfRecogIDCard = (FUN_RecogIDCard)GetProcAddress(m_hDllScan, "RecogIDCard");

		m_pfCheckDeviceOnline = (FUN_CheckDeviceOnline)GetProcAddress(m_hDllScan, "CheckDeviceOnline");
		m_pfGetRecogResult = (FUN_GetRecogResult)GetProcAddress(m_hDllScan, "GetRecogResult");
		m_pfGetFieldName = (FUN_GetFieldName)GetProcAddress(m_hDllScan, "GetFieldName");

		m_pfGetVersionInfo = (FUN_GetVersionInfo)GetProcAddress(m_hDllScan, "GetVersionInfo");
		m_pfGetCurrentDevice = (FUN_GetCurrentDevice)GetProcAddress(m_hDllScan, "GetCurrentDevice");
		m_pfSaveHeadImage = (FUN_SaveHeadImage)GetProcAddress(m_hDllScan, "SaveHeadImage");

		m_pfGetButtonDownType  =( FUN_GetButtonDownType)GetProcAddress(m_hDllScan, "GetButtonDownType");
		m_pfGetSubId = (FUN_GetSubId)GetProcAddress(m_hDllScan, "GetSubID");

		m_pfSetIDCardID = (FUN_SetIDCardID)GetProcAddress(m_hDllScan, "SetIDCardID");
		m_pfAddIDCardID= (FUN_AddIDCardID)GetProcAddress(m_hDllScan, "AddIDCardID");
		m_pfRecogIDCardEx = (FUN_RecogIDCardEx)GetProcAddress(m_hDllScan, "RecogIDCardEX");
		m_pfGetSubId = (FUN_GetSubId)GetProcAddress(m_hDllScan, "GetSubID");
		//////////////////
		m_pfGetGrabSignalType=(FUN_GetGrabSignalType)GetProcAddress(m_hDllScan,"GetGrabSignalType");
		m_pfSetIDCardID = (FUN_SetIDCardID)GetProcAddress(m_hDllScan, "SetIDCardID");	
		m_pfAddIDCardID = (FUN_AddIDCardID)GetProcAddress(m_hDllScan, "AddIDCardID");
		m_pfSetSpecialAttribute=(FUN_SetSpecialAttribute)GetProcAddress(m_hDllScan,"SetSpecialAttribute");
		//fail to get the output functions
		if( m_pfInitIDCard == NULL 
			|| m_pfFreeIDCard == NULL
			|| m_pfSetAcquireImageType == NULL
			|| m_pfSetUserDefinedImageSize == NULL
			|| m_pfSetAcquireImageResolution == NULL
			|| m_pfAcquireImage == NULL
			|| m_pfSaveImage == NULL
			|| m_pfSaveHeadImage == NULL
			|| m_pfRecogIDCard == NULL
			|| m_pfCheckDeviceOnline == NULL
			|| m_pfGetRecogResult == NULL
			|| m_pfGetFieldName == NULL
			|| m_pfGetCurrentDevice == NULL
			|| m_pfGetVersionInfo == NULL
			|| m_pfGetSubId == NULL
			|| m_pfGetButtonDownType == NULL
			|| m_pfSetIDCardID == NULL
			|| m_pfAddIDCardID == NULL
			|| m_pfRecogIDCardEx == NULL
			|| m_pfGetSubId == NULL
			|| m_pfGetGrabSignalType==NULL
			|| m_pfSetIDCardID==NULL
			|| m_pfAddIDCardID==NULL
			|| m_pfSetSpecialAttribute==NULL)
		{
			::FreeLibrary(m_hDllScan);
			m_hDllScan = NULL;
			return FALSE;
		}

		m_bIsScanInitSuccess = FALSE;

		//init the id card
		int nRet = m_pfInitIDCard(SzUserID, 0, NULL);		
		/*CString strRetCode;
		strRetCode.Format(_T("����ֵ��%d\r\n"),nRet);
		m_strResult = strRetCode;*/
		//SetCurrentDirectory(szOld);
		if (nRet==0)
		{
			m_bIsScanInitSuccess = TRUE;
			m_pfSetSpecialAttribute(1,1);//���ö�ȡ�й����ӻ���ʱ����ʶ������㣬ǩ���ص㣬ǩ������
			//m_strResult = _T("ʶ����ļ��سɹ�");
			return ERROR_SCANNER_OK;
		}
		else if (nRet==1)
		{
			m_bIsScanInitSuccess = FALSE;
			//m_strResult += _T("��Ч���û�ID");
		}
		else
		{
			m_bIsScanInitSuccess = FALSE;
			//m_strResult += _T("���ļ���ʧ��");

		}
	}
	else
	{
		return ERROR_SCANNER_OK;
	}
	
	return ERROR_SCANNER_OTHER;
}

int CScan9::OpenReadCardDevice()
{
	if(m_hDllRead == NULL)
	{
		char cLibDir[MAX_PATH]={0};
		GetPrivateProfileString("SCAN_2", "LIB_DIR", "", cLibDir, sizeof(cLibDir), g_cIniFile);
		int pathLen=strlen(cLibDir);
		if(pathLen > 0 && cLibDir[pathLen-1]!='\\')
		{
			cLibDir[pathLen]='\\';
			pathLen++;
		}
		strcpy(cLibDir+pathLen, "sdtapi.dll");
		m_hDllRead = LoadLibrary(cLibDir);
	}
	if(m_hDllGetBmp == NULL)
	{
		char cLibDir[MAX_PATH]={0};
		GetPrivateProfileString("SCAN_2", "LIB_DIR", "", cLibDir, sizeof(cLibDir), g_cIniFile);
		int pathLen=strlen(cLibDir);
		if(pathLen > 0 && cLibDir[pathLen-1]!='\\')
		{
			cLibDir[pathLen]='\\';
			pathLen++;
		}
		strcpy(cLibDir+pathLen,"WltRS.dll");
		m_hDllGetBmp = LoadLibrary(cLibDir);
	}
	if(m_hDllRead == NULL ||
		m_hDllGetBmp == NULL)
	{
		return ERROR_SCANNER_LOADDLL;
	}

	// оƬʶ��
	m_pfSDT_OpenPort=(FUN_SDT_OpenPort)GetProcAddress(m_hDllRead,"SDT_OpenPort");
	m_pfSDT_ClosePort=(FUN_SDT_ClosePort)GetProcAddress(m_hDllRead,"SDT_ClosePort");
	m_pfSDT_StartFindIDCard=(FUN_SDT_StartFindIDCard)GetProcAddress(m_hDllRead,"SDT_StartFindIDCard");
	m_pfSDT_SelectIDCard=(FUN_SDT_SelectIDCard)GetProcAddress(m_hDllRead,"SDT_SelectIDCard");
	m_pfSDT_ReadBaseMsgToFile=(FUN_SDT_ReadBaseMsgToFile)GetProcAddress(m_hDllRead,"SDT_ReadBaseMsgToFile");
	m_pfSDT_ReadBaseMsg=(FUN_SDT_ReadBaseMsg)GetProcAddress(m_hDllRead,"SDT_ReadBaseMsg");//=(int (WINAPI *)(int,unsigned char*,unsigned int *, unsigned char*��unsigned int *,int))GetProcAddress(m_NIDapi,"SDT_ReadBaseMsg");
	m_pfSDT_ReadNewAppMsg=(FUN_SDT_ReadNewAppMsg)GetProcAddress(m_hDllRead,("SDT_ReadNewAppMsg"));
	if(m_pfSDT_OpenPort==NULL
		||m_pfSDT_ClosePort==NULL
		||m_pfSDT_StartFindIDCard==NULL
		||m_pfSDT_SelectIDCard==NULL
		||m_pfSDT_ReadBaseMsgToFile==NULL
		||m_pfSDT_ReadBaseMsg==NULL
		||m_pfSDT_ReadNewAppMsg==NULL)
	{
		//AfxMessageBox(_T("��������ʧ��"));
		return  ERROR_SCANNER_LOADDLL;
	}


	m_pfGetBmp = (FUN_GetBmp)GetProcAddress(m_hDllGetBmp,"GetBmp");
	if(!m_pfGetBmp)
	{
		// AfxMessageBox(_T("WltRS.dll����ʧ��"));
		return  ERROR_SCANNER_LOADDLL;
	}

	int nRes=1;
	if(!m_bReadOpened)
	{
		for (int iPort=1001; iPort<=1016; iPort++)
		{
			nRes = m_pfSDT_OpenPort(iPort);
			if (nRes==0x90)
			{	
				m_nOpenPort = iPort;
				m_OPort = true;

				m_bReadOpened = true;

				return  ERROR_SCANNER_OK;
			}
		}

		if(nRes!=0x90)
		{
			// AfxMessageBox(_T("�򿪶˿�ʧ��"));
		}
	}
	return  ERROR_SCANNER_OTHER;
}

int CScan9::CloseDevice()
{
	if(m_bReadOpened)
	{
		m_bReadOpened = false;
	}
	CloseScan();	
	CloseReadCard();

	return ERROR_SCANNER_OK;
}

int CScan9::ScanImage(int idType, char *szImgFile)
{
	// 2014-3-18 - qsc
	if(ERROR_SCANNER_OK != m_nRetScanOpen)
	{
		m_nRetScanOpen = OpenScanDevice();
	}

	if(ERROR_SCANNER_OK != m_nRetScanOpen)
	{
		return m_nRetScanOpen;
	}

	/*if(!m_bOpened)
	{
		return ERROR_SCANNER_HARDWARE;
	}*/
	
	if(!m_bIsScanInitSuccess)
	{
		//m_strResult = _T("δ�ɹ�����ʶ����ģ����ܽ���ʶ��");
		//UpdateData(FALSE);
		return ERROR_SCANNER_OTHER;
	}

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

	int nImageSizeType = 0;
	switch(idType)
	{
	case IDENTITY_ID1:// 10 һ�����֤
		{
			nImageSizeType = 1;
			m_nIdCardType = 1;
		}
		break;
		
	case IDENTITY_ID2:// 11 �������֤
		{
			nImageSizeType = 2;	
			m_nIdCardType = 2;
		}
		break;
	case IDENTITY_OFFICER://90, //����֤	
		{
			nImageSizeType = 4;
			m_nIdCardType = 7;
		}
	case IDENTITY_HUKOUBU:	// 93, //���ڲ�
		{
			nImageSizeType = 5;
			m_nIdCardType = 16;
		}
	case IDENTITY_PASSPORT:// 94 ����
		{
			m_nIdCardType = 13;
			nImageSizeType = 3;
		}
		break;
	case IDENTITY_TAIBAO:// 95 ̨��֤
		{

		}
		break;
	case IDENTITY_GANGHAO:
		{
			m_nIdCardType = 9;
			nImageSizeType = 3;
		}
		break;
	// ��������û����
	//IDENTITY_STUDENT   = 15, //ѧ��֤
	//IDENTITY_SOLDIER   = 92, //ʿ��֤
	//IDENTITY_OTHER   = 99, //����֤��		
	default:
		{
			nImageSizeType = 0;
		}
		break;
	}
	//1���Ȳɼ�ͼ��
	//�жϲɼ�ͼ������Ƿ���ȷ
	if (nImageSizeType<0)
	{
		//m_strResult = _T("��Ч��֤���ߴ�����");
		return ERROR_SCANNER_OTHER;
	}
	//����ͼ��ɼ��ӿڻ�ȡ�ڴ�ͼ��
	int nRet = m_pfAcquireImage(nImageSizeType);

	if(nRet != 0)
	{
		/*CString strErrCode;
		strErrCode.Format(_T("����ֵ��%d\r\n"),nRet);
		m_strResult = strErrCode;
		m_strResult += _T("�ɼ�ͼ��ʧ��");
		UpdateData(FALSE);*/
		return ERROR_SCANNER_OTHER;
	}

	//3������Ҫ���Ա���ɼ���ͼ���֤��ͷ��
	//����ɼ���ͼ�񵽱��أ��˽ӿڵ���ǰ�����ȵ��òɼ��ӿ�fpAcquireImage��ȡͼ��

	CString strSrcImg(szImgFile);
	if(0 != m_pfSaveImage(CT2W(strSrcImg)))
	{
		return ERROR_SCANNER_OTHER;
	}

	//����ʶ���ȡ��ͷ�񵽱���
	//CString strHeadImg(m_cTempPath);
	//strHeadImg += _T("IDCard_Head.jpg");
	//// 	if(0!=fpSaveHeadImage(strHeadImg))
	//// 	{
	//// 		//CString strErrCode;
	//// 		//strErrCode.Format(_T("����ֵ��%d\r\n"),nRet);
	//// 		//m_strResult = strErrCode;
	//// 		/m_strResult += _T("����ͷ��ʧ��");
	//// 	}
	//m_pfSaveHeadImage(CT2W(strHeadImg));

	return ERROR_SCANNER_OK ;
}

int CScan9::RecogniseImage(int idType, IDENTITY_INFO &info, char *szHeadImage)
{
	if(ERROR_SCANNER_OK != m_nRetScanOpen)
	{
		return m_nRetScanOpen;
	}
	/*if(!m_bOpened)
	{
		return ERROR_SCANNER_HARDWARE;
	}*/

	////2��ʶ��
	////�ж�ʶ������Ƿ���ȷ
	//if (m_nCarTypeID<0)
	//{
	//	m_strResult = _T("��Ч��֤�����ͣ�������������Ч��֤������");
	//	UpdateData(FALSE);
	//	return;
	//}
	////����ʶ��ӿڽ���ʶ��
	////m_nCarTypeID���Ա�ʾһ��֤����Ҳ���Ա�ʾ����֤������1��ʾһ��֤��2��ʾ����֤��3��ʾ����֤���棬��ôm_nCarTypeID=1��ʾʶ��һ��֤
	int tmp[1]={0};
	m_pfSetIDCardID(m_nIdCardType,tmp,1);
	////��Ҫʵ��֤�����Զ�����ʶ�𣬿�ʹ��SetIDCardID��AddIDCardID�������֤�֣�������ʶ��һ��֤�����գ���ʻ֤Ϊ��
	////nCardTypeID=1;
	////fpSetIDCardID(nCardType,tmp,1);/*�����ǰ֤�ֲ����һ��֤*/
	////int nCardType=5;
	////fpAddIDCardID(nCardType,tmp,1);/*��Ӽ���*/
	////nCardType=6;
	////fpAddIDCardID(nCardType,tmp,1);/*�����ʻ֤*/
	////nRet=fpRecogIDCard();
	int nRet = m_pfRecogIDCard();
	if(nRet <= 0)
	{
		CString strErrCode;
		strErrCode.Format(_T("����ֵ��%d\r\n"),nRet);
		//m_strResult = strErrCode;
		//m_strResult += _T("ʶ��ʧ��");
		//UpdateData(FALSE);
		return ERROR_SCANNER_OTHER;
	}

	//��ȡʶ����

	CONST int MAX_CH_NUM = 256;
	wchar_t szFieldValue[MAX_CH_NUM];
	wchar_t szFieldName[MAX_CH_NUM];
	int nBufLen = MAX_CH_NUM*sizeof(wchar_t);
	int nIndex = 1;
	//m_strResult = _T("ʶ��ɹ�\r\n");
	//CString m_strResult;
	//while(TRUE)
	//{
	//	memset(szFieldValue,0,nBufLen);
	//	memset(szFieldName,0,nBufLen);
	//	//��ȡnIndex��Ӧ��ʶ������
	//	nRet = m_pfGetRecogResult(nIndex, szFieldValue, nBufLen);
	//	if(nRet == 3)
	//		break;
	//	//��ȡʶ����ֶ�����,nIndex��Ӧ���ֶ����ƿ��Բ鿪���ĵ�
	//	m_pfGetFieldName(nIndex, szFieldName, nBufLen);
	//	++nIndex;

	//	////������ʾ
	//	CString strTmp = _T("");
	//	strTmp = (CString(szFieldName) + _T(" : "));	
	//	strTmp += (CString(szFieldValue) + _T("\r\n"));
	//	m_strResult += strTmp;
	//}
	// 1����
	// 2�Ա�
	// 3����
	// 4����
	// 5סַ
	// 6���֤
	if(1 == m_nIdCardType || 2 == m_nIdCardType)
	{
		memset(szFieldValue,0,nBufLen);
		memset(szFieldName,0,nBufLen);
		//��ȡnIndex��Ӧ��ʶ������
		nIndex = 1;
		nRet = m_pfGetRecogResult(nIndex, szFieldValue, nBufLen);
		if(nRet == 3)
			return ERROR_SCANNER_OK;
		WideCharToMultiByte (CP_ACP,NULL,szFieldValue,-1,info.name,40,NULL,FALSE);
		memset(szFieldValue,0,nBufLen);

		nIndex = 2;
		nRet = m_pfGetRecogResult(nIndex, szFieldValue, nBufLen);
		if(nRet == 3)
			return ERROR_SCANNER_OK;
		WideCharToMultiByte (CP_ACP,NULL,szFieldValue,-1,info.sex,2,NULL,FALSE);
		memset(szFieldValue,0,nBufLen);

		nIndex = 3;
		nRet = m_pfGetRecogResult(nIndex, szFieldValue, nBufLen);
		if(nRet == 3)
			return ERROR_SCANNER_OK;
		WideCharToMultiByte (CP_ACP,NULL,szFieldValue,-1,info.nation,20,NULL,FALSE);	
		memset(szFieldValue,0,nBufLen);

		nIndex = 4;
		nRet = m_pfGetRecogResult(nIndex, szFieldValue, nBufLen);
		if(nRet == 3)
			return ERROR_SCANNER_OK;
		WideCharToMultiByte (CP_ACP,NULL,szFieldValue,-1,info.birthday,10,NULL,FALSE);	
		memset(szFieldValue,0,nBufLen);

		nIndex = 5;
		nRet = m_pfGetRecogResult(nIndex, szFieldValue, nBufLen);
		if(nRet == 3)
			return ERROR_SCANNER_OK;
		WideCharToMultiByte (CP_ACP,NULL,szFieldValue,-1,info.address,200,NULL,FALSE);	
		memset(szFieldValue,0,nBufLen);

		nIndex = 6;
		nRet = m_pfGetRecogResult(nIndex, szFieldValue, nBufLen);
		if(nRet == 3)
			return ERROR_SCANNER_OK;
		WideCharToMultiByte (CP_ACP,NULL,szFieldValue,-1,info.number,40,NULL,FALSE);	
		memset(szFieldValue,0,nBufLen);
	}
	else if(7 == m_nIdCardType)		// ����֤
	{
		memset(szFieldValue,0,nBufLen);
		memset(szFieldName,0,nBufLen);
		//��ȡnIndex��Ӧ��ʶ������
		nIndex = 2;
		nRet = m_pfGetRecogResult(nIndex, szFieldValue, nBufLen);
		if(nRet == 3)
			return ERROR_SCANNER_OK;
		WideCharToMultiByte (CP_ACP,NULL,szFieldValue,-1,info.name,40,NULL,FALSE);
		memset(szFieldValue,0,nBufLen);

		nIndex = 4;
		nRet = m_pfGetRecogResult(nIndex, szFieldValue, nBufLen);
		if(nRet == 3)
			return ERROR_SCANNER_OK;
		WideCharToMultiByte (CP_ACP,NULL,szFieldValue,-1,info.sex,2,NULL,FALSE);
		memset(szFieldValue,0,nBufLen);

		nIndex = 6;
		nRet = m_pfGetRecogResult(nIndex, szFieldValue, nBufLen);
		if(nRet == 3)
			return ERROR_SCANNER_OK;
		WideCharToMultiByte (CP_ACP,NULL,szFieldValue,-1,info.nation,20,NULL,FALSE);	
		memset(szFieldValue,0,nBufLen);

		nIndex = 3;
		nRet = m_pfGetRecogResult(nIndex, szFieldValue, nBufLen);
		if(nRet == 3)
			return ERROR_SCANNER_OK;
		WideCharToMultiByte (CP_ACP,NULL,szFieldValue,-1,info.birthday,10,NULL,FALSE);	
		memset(szFieldValue,0,nBufLen);

		nIndex = 1;
		nRet = m_pfGetRecogResult(nIndex, szFieldValue, nBufLen);
		if(nRet == 3)
			return ERROR_SCANNER_OK;
		WideCharToMultiByte (CP_ACP,NULL,szFieldValue,-1,info.number,40,NULL,FALSE);	
		memset(szFieldValue,0,nBufLen);

	}
	else if(13 == m_nIdCardType)		// ����
	{
		memset(szFieldValue,0,nBufLen);
		memset(szFieldName,0,nBufLen);
		//��ȡnIndex��Ӧ��ʶ������
		nIndex = 2;
		nRet = m_pfGetRecogResult(nIndex, szFieldValue, nBufLen);
		if(nRet == 3)
			return ERROR_SCANNER_OK;
		WideCharToMultiByte (CP_ACP,NULL,szFieldValue,-1,info.name,40,NULL,FALSE);
		memset(szFieldValue,0,nBufLen);

		nIndex = 4;
		nRet = m_pfGetRecogResult(nIndex, szFieldValue, nBufLen);
		if(nRet == 3)
			return ERROR_SCANNER_OK;
		WideCharToMultiByte (CP_ACP,NULL,szFieldValue,-1,info.sex,2,NULL,FALSE);
		memset(szFieldValue,0,nBufLen);

		// ����û������
		/*nIndex = 6;
		nRet = m_pfGetRecogResult(nIndex, szFieldValue, nBufLen);
		if(nRet == 3)
			return ERROR_SCANNER_OK;
		WideCharToMultiByte (CP_ACP,NULL,szFieldValue,-1,info.nation,20,NULL,FALSE);	
		memset(szFieldValue,0,nBufLen);*/

		nIndex = 5;
		nRet = m_pfGetRecogResult(nIndex, szFieldValue, nBufLen);
		if(nRet == 3)
			return ERROR_SCANNER_OK;
		WideCharToMultiByte (CP_ACP,NULL,szFieldValue,-1,info.birthday,10,NULL,FALSE);	
		memset(szFieldValue,0,nBufLen);

		// ���պ���
		nIndex = 1;
		nRet = m_pfGetRecogResult(nIndex, szFieldValue, nBufLen);
		if(nRet == 3)
			return ERROR_SCANNER_OK;
		WideCharToMultiByte (CP_ACP,NULL,szFieldValue,-1,info.number,40,NULL,FALSE);	
		memset(szFieldValue,0,nBufLen);

		nIndex = 5;
		nRet = m_pfGetRecogResult(nIndex, szFieldValue, nBufLen);
		if(nRet == 3)
			return ERROR_SCANNER_OK;
		WideCharToMultiByte (CP_ACP,NULL,szFieldValue,-1,info.birthday,10,NULL,FALSE);	
		memset(szFieldValue,0,nBufLen);

		nIndex = 16;
		nRet = m_pfGetRecogResult(nIndex, szFieldValue, nBufLen);
		if(nRet == 3)
			return ERROR_SCANNER_OK;
		WideCharToMultiByte (CP_ACP,NULL,szFieldValue,-1,info.signdate,10,NULL,FALSE);	
		memset(szFieldValue,0,nBufLen);

		nIndex = 6;
		nRet = m_pfGetRecogResult(nIndex, szFieldValue, nBufLen);
		if(nRet == 3)
			return ERROR_SCANNER_OK;
		WideCharToMultiByte (CP_ACP,NULL,szFieldValue,-1,info.validterm,10,NULL,FALSE);	
		memset(szFieldValue,0,nBufLen);
	}
	else if(9 == m_nIdCardType)		// �۰�ͨ��֤
	{
		memset(szFieldValue,0,nBufLen);
		memset(szFieldName,0,nBufLen);
		//��ȡnIndex��Ӧ��ʶ������
		nIndex = 2;
		nRet = m_pfGetRecogResult(nIndex, szFieldValue, nBufLen);
		if(nRet == 3)
			return ERROR_SCANNER_OK;
		WideCharToMultiByte (CP_ACP,NULL,szFieldValue,-1,info.name,40,NULL,FALSE);
		memset(szFieldValue,0,nBufLen);

		nIndex = 4;
		nRet = m_pfGetRecogResult(nIndex, szFieldValue, nBufLen);
		if(nRet == 3)
			return ERROR_SCANNER_OK;
		WideCharToMultiByte (CP_ACP,NULL,szFieldValue,-1,info.sex,2,NULL,FALSE);
		memset(szFieldValue,0,nBufLen);

		// �۰�ͨ��֤û������
		/*nIndex = 6;
		nRet = m_pfGetRecogResult(nIndex, szFieldValue, nBufLen);
		if(nRet == 3)
			return ERROR_SCANNER_OK;
		WideCharToMultiByte (CP_ACP,NULL,szFieldValue,-1,info.nation,20,NULL,FALSE);	
		memset(szFieldValue,0,nBufLen);*/

		nIndex = 5;
		nRet = m_pfGetRecogResult(nIndex, szFieldValue, nBufLen);
		if(nRet == 3)
			return ERROR_SCANNER_OK;
		WideCharToMultiByte (CP_ACP,NULL,szFieldValue,-1,info.birthday,10,NULL,FALSE);	
		memset(szFieldValue,0,nBufLen);

		// �۰�ͨ��֤����
		nIndex = 1;
		nRet = m_pfGetRecogResult(nIndex, szFieldValue, nBufLen);
		if(nRet == 3)
			return ERROR_SCANNER_OK;
		WideCharToMultiByte (CP_ACP,NULL,szFieldValue,-1,info.number,40,NULL,FALSE);	
		memset(szFieldValue,0,nBufLen);

		nIndex = 5;
		nRet = m_pfGetRecogResult(nIndex, szFieldValue, nBufLen);
		if(nRet == 3)
			return ERROR_SCANNER_OK;
		WideCharToMultiByte (CP_ACP,NULL,szFieldValue,-1,info.birthday,10,NULL,FALSE);	
		memset(szFieldValue,0,nBufLen);

		nIndex = 16;
		nRet = m_pfGetRecogResult(nIndex, szFieldValue, nBufLen);
		if(nRet == 3)
			return ERROR_SCANNER_OK;
		WideCharToMultiByte (CP_ACP,NULL,szFieldValue,-1,info.signdate,10,NULL,FALSE);	
		memset(szFieldValue,0,nBufLen);

		nIndex = 6;
		nRet = m_pfGetRecogResult(nIndex, szFieldValue, nBufLen);
		if(nRet == 3)
			return ERROR_SCANNER_OK;
		WideCharToMultiByte (CP_ACP,NULL,szFieldValue,-1,info.validterm,10,NULL,FALSE);	
		memset(szFieldValue,0,nBufLen);
	}
	return ERROR_SCANNER_OK;
}

bool CScan9::IsSupportHeadImage(int /*idType*/)
{
	return true;
}

bool CScan9::IsSupportRecognise(int idType)
{
	bool bSupport = false;

	switch(idType)
	{
	case IDENTITY_ID1:// 10 һ�����֤
		{
			bSupport = true;
		}
		break;
		
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
			bSupport = true;
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
			bSupport = true;
		}
		break;
		
	case IDENTITY_PASSPORT:// 94 ����
		{
		}
		break;
		
	case IDENTITY_TAIBAO:// 95 ̨��֤
		{
		}
		break;
		
	case IDENTITY_OTHER:// 99 ����֤��
		{
		}
		break;
		
	default:
		break;
	}

	return 	bSupport;
}

bool CScan9::IsSupportCallBack()
{
	return false;
}

bool CScan9::IsSupportScanner()
{
	return true;
}

int CScan9::SetCallBack(CallBack /*pFunc*/)
{
	return 9;
}

void CScan9::GetErrorMessage(int errCode,char* szErrMsg)
{
	if(szErrMsg)
	{
		if(errCode < 0)
		{
			strcpy(szErrMsg, "�������ʧ��!");
		}
		else
		{
			//Format_ErrMsg(errCode, &szErrMsg);
		}
	}
}

bool CScan9::IsSupportReader()
{
	return true;
}

int CScan9::ReadCard(IDENTITY_INFO &info, char *szHeadImage)
{
	if(!IsSupportReader())
	{
		Beep(800, 500);	
		return ERROR_SCANNER_HARDWARE;
	}

	if (!m_OPort)
	{
		return ERROR_SCANNER_HARDWARE;

	}

	//unsigned char* pRAPDU='\0';
	CString path;
	unsigned int a,b;
	unsigned int *len1,*len2;
	len1=&a;
	len2=&b;
	unsigned int len;
	unsigned char pucApp[320] = {0};
	int nRet = m_pfSDT_ReadNewAppMsg(m_nOpenPort,pucApp,&len,0);
	if (nRet == 0x90 || nRet==0x91)
	{
		// AfxMessageBox(_T("�˿��Ѷ����������·ſ�!"));
		return ERROR_SCANNER_OTHER;
	}
	unsigned char pucIIN[4];
	if(m_pfSDT_StartFindIDCard(m_nOpenPort,pucIIN,0)!=0x9F)
	{  
		//int x=m_pfSDT_StartFindIDCard(0,pRAPDU,0);
		//AfxMessageBox(_T("Ѱ�ҿ�ʧ��"));
		return ERROR_SCANNER_OTHER;
	}
	unsigned char pucSN[8];
	if(m_pfSDT_SelectIDCard(m_nOpenPort,pucSN,0)!=0x90)
	{
		//AfxMessageBox(_T("ѡ�����֤ʧ��"));
		return ERROR_SCANNER_OTHER;
	}

	//����ȡ����Ϣ��������
	memset(m_readCardPucCHMsg,0,sizeof(unsigned char)*512);
	memset(m_readCardPucPHMsg,0,sizeof(unsigned char)*1024);
	m_readCardUiCHMsgLen = 0;
	m_readCardUiPHMsgLen = 0;
	int m = m_pfSDT_ReadBaseMsg(m_nOpenPort, m_readCardPucCHMsg,& m_readCardUiCHMsgLen, m_readCardPucPHMsg,&m_readCardUiPHMsgLen,0);
	if(m!=0x90)
	{
		// AfxMessageBox(_T("�洢����ʧ��"));
		return ERROR_SCANNER_OTHER;
	}

	strncpy(info.name,		CT2A(GetName()), 20);     //����
	strncpy(info.sex,		CT2A(GetSex()), 2);        //�Ա�
	strncpy(info.number,	CT2A(GetIDCode()), 40);   //֤������
	strncpy(info.nation,	CT2A(GetNation()), 20);    //����
	strncpy(info.birthday,	CT2A(GetBirthday()), 10);  //��������
	strncpy(info.address,	CT2A(GetAddress()), 200);  //��ַ
	strncpy(info.signdate,	CT2A(GetIssueDay()), 10);  //ǩ������
	strncpy(info.validterm, CT2A(GetExpityDay()), 30);  //��Ч��
	strncpy(info.department,CT2A(GetAuthority()), 40); //��֤����

	CString filePath = m_cTempPath;
	filePath += _T("head");

	SavePhoto((TCHAR*)filePath.GetBuffer(),2);

	//int x=m_lpGetBmp((char*)photopath,2);
	CString bmpPath = filePath + _T(".bmp");
	DeleteFile(CString(szHeadImage));
	if(CopyFile(bmpPath, CString(szHeadImage), FALSE))
	{
		return ERROR_SCANNER_OK;
	}
	return ERROR_SCANNER_OTHER;
}


CString CScan9::GetName(void)
{
	if (m_readCardUiCHMsgLen==0)
	{
		return _T("");
	}

	wchar_t szName[30];
	memset(szName,0,30*sizeof(wchar_t));
	wcscpy(szName, L"");
	wcsncpy(szName,(wchar_t*)m_readCardPucCHMsg,sizeof(unsigned char)*15);
	CString str(szName);
	str.TrimRight();
	return str;
}

CString CScan9::GetSex(void)
{
	if (m_readCardUiCHMsgLen==0)
	{
		return _T("");
	}
	wchar_t szTmp[30];
	memset(szTmp,0,30*sizeof(wchar_t));

	wcsncpy(szTmp,(wchar_t*)(m_readCardPucCHMsg+30*sizeof(unsigned char)),sizeof(unsigned char));
	CString str(szTmp);
	str.TrimRight();
	if (str==_T("1"))
	{
		return _T("��");
	}
	else 
	{
		return _T("Ů");
	}
}

CString CScan9::GetNation(void)
{
	if (m_readCardUiCHMsgLen==0)
	{
		return _T("");
	}	
	USES_CONVERSION;
	wchar_t szTmp[30];
	memset(szTmp,0,30*sizeof(wchar_t));
	wcsncat(szTmp,(wchar_t*)(m_readCardPucCHMsg+32*sizeof(unsigned char)),sizeof(unsigned char)*2);

	CString str(szTmp);
	str.TrimRight();
	int code = atoi(str.GetBuffer(str.GetLength()));
	str.ReleaseBuffer();

	switch(code)
	{
	case 01:  return _T("��");  
	case 02:  return _T("�ɹ�");
	case 03:  return _T("��");
	case 04:  return _T("��");
	case 05:  return _T("ά���");
	case 06:  return _T("��");
	case 07:  return _T("��");
	case 8:   return _T("׳");
	case 9:   return _T("����");
	case 10:  return _T("����");
	case 11:  return _T("��");
	case 12:  return _T("��");
	case 13:  return _T("��");
	case 14:  return _T("��");
	case 15:  return _T("����");
	case 16:  return _T("����");
	case 17:  return _T("������");
	case 18:  return _T("��");
	case 19:  return _T("��");
	case 20:  return _T("����");
	case 21:  return _T("��");
	case 22:  return _T("�");
	case 23:  return _T("��ɽ");
	case 24:  return _T("����");
	case 25:  return _T("ˮ");
	case 26:  return _T("����");
	case 27:  return _T("����");
	case 28:  return _T("����");
	case 29:  return _T("�¶�����");
	case 30:  return _T("��");
	case 31:  return _T("���Ӷ�");
	case 32:  return _T("����");
	case 33:  return _T("Ǽ");
	case 34:  return _T("����");
	case 35:  return _T("����");
	case 36:  return _T("ë��");
	case 37:  return _T("����");
	case 38:  return _T("����");
	case 39:  return _T("����");
	case 40:  return _T("����");
	case 41:  return _T("������");
	case 42:  return _T("ŭ");
	case 43:  return _T("���α��");
	case 44:  return _T("����˹");
	case 45:  return _T("���¿�");
	case 46:  return _T("�°�");
	case 47:  return _T("����");
	case 48:  return _T("ԣ��");
	case 49:  return _T("��");
	case 50:  return _T("������");
	case 51:  return _T("����");
	case 52:  return _T("���״�");
	case 53:  return _T("����");
	case 54:  return _T("�Ű�");
	case 55:  return _T("���");
	case 56:  return _T("��ŵ");
	case 97:  return _T("����");
	case 98:  return _T("���Ѫͳ�й�����ʿ");
	default : return _T("");
	}   
}

CString CScan9::GetBirthday(void)
{
	if (m_readCardUiCHMsgLen==0)
	{
		return _T("");
	}
	wchar_t szTmp[30];
	memset(szTmp,0,30*sizeof(wchar_t));
	wcsncat(szTmp,(wchar_t*)(m_readCardPucCHMsg+36*sizeof(unsigned char)),sizeof(unsigned char)*8);

	CString str(szTmp);
	str.TrimRight();
	return str;	
}

CString CScan9::GetAddress(void)
{
	if (m_readCardUiCHMsgLen==0)
	{
		return _T("");
	}
	wchar_t szTmp[100];
	memset(szTmp,0,100*sizeof(wchar_t));
	wcsncat(szTmp,(wchar_t*)(m_readCardPucCHMsg+52*sizeof(unsigned char)),sizeof(unsigned char)*35);

	CString str(szTmp);
	str.TrimRight();
	return str;
}

CString CScan9::GetAuthority(void)
{
	if (m_readCardUiCHMsgLen==0)
	{
		return _T("");
	}
	wchar_t szTmp[50];
	memset(szTmp,0,50*sizeof(wchar_t));
	wcsncat(szTmp,(wchar_t*)(m_readCardPucCHMsg+158*sizeof(unsigned char)),sizeof(unsigned char)*15);

	CString str(szTmp);
	str.TrimRight();
	return str;
}

CString CScan9::GetIDCode(void)
{
	if (m_readCardUiCHMsgLen==0)
	{
		return _T("");
	}
	wchar_t szTmp[20];
	memset(szTmp,0,20*sizeof(wchar_t));
	wcsncat(szTmp,(wchar_t*)(m_readCardPucCHMsg+122*sizeof(unsigned char)),sizeof(unsigned char)*18); 
	CString str(szTmp);
	str.TrimRight();
	return str;
}


CString CScan9::GetIssueDay(void)
{
	if (m_readCardUiCHMsgLen==0)
	{
		return _T("");
	}
	wchar_t szTmp[30];
	memset(szTmp,0,30*sizeof(wchar_t));
	wcsncat(szTmp,(wchar_t*)(m_readCardPucCHMsg+188*sizeof(unsigned char)),sizeof(unsigned char)*8);

	CString str(szTmp);
	str.TrimRight();
	return str;
}

CString CScan9::GetExpityDay(void)
{
	if (m_readCardUiCHMsgLen==0)
	{
		return _T("");
	}
	wchar_t szTmp[30];
	memset(szTmp,0,30*sizeof(wchar_t));
	wcsncat(szTmp,(wchar_t*)(m_readCardPucCHMsg+204*sizeof(unsigned char)),sizeof(unsigned char)*8);

	CString str(szTmp);
	str.TrimRight();
	return str;
}


int CScan9::GetBmp(char * photopath, int nType)
{
	if (m_pfGetBmp==NULL)
	{
		return -1;
	}
	int nRes = m_pfGetBmp(photopath,nType);

	return nRes;	
}


int CScan9::SavePhoto(TCHAR* retFileName, int nType)
{
	USES_CONVERSION;

	CString strTmp = retFileName;
	CString strWlt = strTmp.Left(strTmp.GetLength());
	strWlt += _T(".wlt");
	OFSTRUCT ofStruct;
	DWORD dwRealWrite = 0;
	HANDLE hFile = (HANDLE)OpenFile(strWlt, &ofStruct, OF_CREATE | OF_WRITE );
	if(hFile == INVALID_HANDLE_VALUE || !hFile)
	{
		return -1;
	}
	WriteFile(hFile, m_readCardPucPHMsg,m_readCardUiPHMsgLen, &dwRealWrite, NULL);
	CloseHandle(hFile);
	hFile = INVALID_HANDLE_VALUE;

	/*f.Write(m_readCardPucPHMsg, m_readCardUiPHMsgLen);
	f.Close();*/
	int nRet = m_pfGetBmp(CT2A(strWlt),nType);
	/*f.Open (strWlt,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);
	FilePath=f.GetFilePath ();*/
	return nRet;	
}

void CScan9::CloseReadCard()
{
	if (m_OPort!=false)
	{
		//m_pfSDT_ClosePort(0x00);
		m_pfSDT_ClosePort(m_nOpenPort);
		if(m_hDllRead!=NULL)
		{
			FreeLibrary(m_hDllRead);
			m_hDllRead=NULL;		
		}
		if(m_hDllGetBmp!=NULL)
		{
			FreeLibrary(m_hDllGetBmp);
			m_hDllGetBmp=NULL;
		}  
		m_OPort=false;
	}
}

void CScan9::CloseScan()
{
	if(m_bIsScanInitSuccess)
	{
		if(m_pfFreeIDCard!=NULL)
		{
			m_pfFreeIDCard();
		}
		if(m_hDllScan != NULL)
		{
			FreeLibrary(m_hDllScan);
			m_hDllScan = NULL;
		}
	}
	m_bIsScanInitSuccess = FALSE;
}
