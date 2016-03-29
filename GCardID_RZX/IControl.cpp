#include "StdAfx.h"
#include ".\icontrol.h"

#define WM_IDCARD_MESSAGE		WM_USER	+ 0x2000 + 0x51

typedef struct IDCARD_INFO_tag
{
	char szIDNo[51];
	char szUserNo[51];
	char szPassword[51];
	BYTE IDCardType;
	BOOL bValidate;
	char szName[31];
	char szCertificateType[3];
	char szCertificateID[19];
	char szCompany[31];
	char szCountry[11];
	char szDetail[31];
}IDCARD_INFO;

HANDLE	OpenMemFile(DWORD fMode);
BOOL	MemFileWrite(IDCARD_INFO & stIDCardInfo);
BOOL	MemFileRead(IDCARD_INFO & stIDCardInfo);


int ICC_IDUser(char * pszIDNo,
			char * pszUserNo,
			char * pszPassword,
			BYTE IDCardType,
			BOOL bValidate,
			const REQ_REGISTER_INFO & RegInfo)
{

	IDCARD_INFO stIDCardInfo;
	memset(&stIDCardInfo,0,sizeof(IDCARD_INFO));
	strncpy(stIDCardInfo.szIDNo,pszIDNo,51);
	strncpy(stIDCardInfo.szUserNo,pszUserNo,51);
	strncpy(stIDCardInfo.szPassword,pszPassword,51);
	stIDCardInfo.IDCardType = IDCardType;
	stIDCardInfo.bValidate = bValidate;
	strncpy(stIDCardInfo.szName,RegInfo.szName,31);
	strncpy(stIDCardInfo.szCertificateType,RegInfo.szCertificateType,3);
	strncpy(stIDCardInfo.szCertificateID,RegInfo.szCertificateID,19);
	strncpy(stIDCardInfo.szCompany,RegInfo.szCompany,31);
	strncpy(stIDCardInfo.szCountry,RegInfo.szCountry,11);
	strncpy(stIDCardInfo.szDetail,RegInfo.szDetail,31);

	MemFileWrite(stIDCardInfo);

	HWND hWnd;
	if((hWnd=FindWindow(NULL,"��ţ������Ӫ����ƽ̨"))==NULL &&
		(hWnd=FindWindow(NULL,"��ţ�ŵ���Ӫ����ƽ̨"))==NULL &&
		(hWnd=FindWindow(NULL,"��ţ��Ӫ����ƽ̨"))==NULL &&
		(hWnd=FindWindow(NULL,"��ϢEվ�ŵ���Ӫ����ƽ̨"))==NULL)
	{
		AfxMessageBox(_T("����������ţ����̨��"));
		//MessageBox(NULL,"[��ţ��Ӫ����ƽ̨]δ������\r\n\r\n������[��ţ��Ӫ����ƽ̨]֮����ˢ����","[��ţ��Ӫ����ƽ̨]ID���ӿ�",MB_ICONQUESTION|MB_OK);
	} 
	else
	{
		::SendMessage(hWnd,WM_IDCARD_MESSAGE,0x02,0);
	}

	return 1;
}

int ICC_AddToMemberRZX(char * pszIDNo,
						char*pszUserNo,
						char*pszPassword,
						BYTE IDCardType,
						const REQ_REGISTER_INFO & RegInfo)
{

	IDCARD_INFO stIDCardInfo;
	memset(&stIDCardInfo,0,sizeof(IDCARD_INFO));
	strncpy(stIDCardInfo.szIDNo,pszIDNo,51);
	strncpy(stIDCardInfo.szUserNo,pszUserNo,51);
	strncpy(stIDCardInfo.szPassword,pszPassword,51);
	stIDCardInfo.IDCardType = IDCardType;
	stIDCardInfo.bValidate = 1;
	strncpy(stIDCardInfo.szName,RegInfo.szName,31);
	strncpy(stIDCardInfo.szCertificateType,RegInfo.szCertificateType,3);
	strncpy(stIDCardInfo.szCertificateID,RegInfo.szCertificateID,19);
	strncpy(stIDCardInfo.szCompany,RegInfo.szCompany,31);
	strncpy(stIDCardInfo.szCountry,RegInfo.szCountry,11);
	strncpy(stIDCardInfo.szDetail,RegInfo.szDetail,31);

	MemFileWrite(stIDCardInfo);

	HWND hWnd;
	if((hWnd=FindWindow(NULL,"��ţ������Ӫ����ƽ̨"))==NULL &&
		(hWnd=FindWindow(NULL,"��ţ�ŵ���Ӫ����ƽ̨"))==NULL &&
		(hWnd=FindWindow(NULL,"��ţ��Ӫ����ƽ̨"))==NULL &&
		(hWnd=FindWindow(NULL,"��ϢEվ�ŵ���Ӫ����ƽ̨"))==NULL)
	{
		AfxMessageBox(_T("����������ţ����̨��"));
		//MessageBox(NULL,"[��ţ��Ӫ����ƽ̨]δ������\r\n\r\n������[��ţ��Ӫ����ƽ̨]֮����ˢ����","[��ţ��Ӫ����ƽ̨]ID���ӿ�",MB_ICONQUESTION|MB_OK);
	} 
	else
	{
		::SendMessage(hWnd,WM_IDCARD_MESSAGE,0x02,0);
	}

	return 1;
}

BOOL ReadIDCardInfo(char * pszIDNo,
					char * pszUserNo,
					char * pszPassword,
					char * pszIDCardType,
					char * pszValidate,
					char * pszName,
					char * pszCertificateType,
					char * pszCertificateID,
					char * pszCompany,
					char * pszCountry,
					char * pszDetail)
{
	IDCARD_INFO stIDCardInfo;
	
	if (!MemFileRead(stIDCardInfo)) return FALSE;

	strcpy(pszIDNo,stIDCardInfo.szIDNo);
	strcpy(pszUserNo,stIDCardInfo.szUserNo);
	strcpy(pszPassword,stIDCardInfo.szPassword);

	sprintf(pszIDCardType,"%d",stIDCardInfo.IDCardType);
	sprintf(pszValidate,"%d",stIDCardInfo.bValidate);

	strcpy(pszName,stIDCardInfo.szName);
	strcpy(pszCertificateType,stIDCardInfo.szCertificateType);
	strcpy(pszCertificateID,stIDCardInfo.szCertificateID);
	strcpy(pszCompany,stIDCardInfo.szCompany);
	strcpy(pszCountry,stIDCardInfo.szCountry);
	strcpy(pszDetail,stIDCardInfo.szDetail);

	return TRUE;
}

HANDLE OpenMemFile(DWORD fMode)
{
	HANDLE	hMemFile = NULL;
	char szMapFile[]="IDCARD_INFO";
	if((hMemFile = OpenFileMapping(fMode, FALSE, szMapFile))!=NULL)
	{
		return hMemFile;
	} else
	{
		hMemFile = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0L, sizeof(IDCARD_INFO_tag), szMapFile); 
		if(hMemFile!=NULL)
			return hMemFile;
		else
			return NULL;
	}
}

BOOL MemFileWrite(IDCARD_INFO & stIDCardInfo)
{
	HANDLE	hMemFile = NULL;
	LPVOID	hpMemory = NULL;
	BOOL	bRetValue = FALSE;
	if((hMemFile=OpenMemFile(FILE_MAP_WRITE))!=NULL)
	{
		hpMemory=MapViewOfFile(hMemFile, FILE_MAP_WRITE, 0, 0, 0);
		if(hpMemory!=NULL)
		{
			memcpy(hpMemory,&stIDCardInfo,sizeof(IDCARD_INFO));
			UnmapViewOfFile(hpMemory);
			bRetValue = TRUE;
		}
	}
	return bRetValue;
}

BOOL MemFileRead(IDCARD_INFO & stIDCardInfo)
{
	HANDLE	hMemFile = NULL;
	LPVOID	hpMemory = NULL;
	BOOL	bRetValue = FALSE;
	if((hMemFile=OpenMemFile(FILE_MAP_READ))!=NULL)
	{
		hpMemory=MapViewOfFile(hMemFile, FILE_MAP_READ, 0, 0, 0);
		if(hpMemory!=NULL)
		{
			memcpy(&stIDCardInfo,hpMemory,sizeof(IDCARD_INFO));
			UnmapViewOfFile(hpMemory);
			bRetValue = TRUE;
		}
	}

	return bRetValue;
}

int ICC_RzxNotiFy(int nNotifyType, const char * lpNotifyMsg)
{
	if (nNotifyType == 1) //�Ʒ�ϵͳ�˳�
	{
		HWND hWnd;
		if((hWnd=FindWindow(NULL,"��ţ������Ӫ����ƽ̨"))==NULL &&
			(hWnd=FindWindow(NULL,"��ţ�ŵ���Ӫ����ƽ̨"))==NULL &&
			(hWnd=FindWindow(NULL,"��ţ��Ӫ����ƽ̨"))==NULL &&
			(hWnd=FindWindow(NULL,"��ϢEվ�ŵ���Ӫ����ƽ̨"))==NULL)
		{
			AfxMessageBox(_T("����������ţ����̨��"));
			//MessageBox(NULL,"[��ţ��Ӫ����ƽ̨]δ����!","[��ţ��Ӫ����ƽ̨]ID���ӿ�",MB_ICONQUESTION|MB_OK);
		} 
		else
		{
			MessageBox(NULL,lpNotifyMsg,"[��ţ��Ӫ����ƽ̨]ID���ӿ�",MB_ICONQUESTION|MB_OK);

			::SendMessage(hWnd,WM_IDCARD_MESSAGE,0x0B,0);
		}
	}

	return 0;
}

