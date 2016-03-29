// sdtapi.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "sdtapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef struct tag_TextInfo
{
	//unsigned char ucUndefine[14];
	unsigned char ucName[30];
	unsigned char ucSex[2];
	unsigned char ucNation[4];
	unsigned char ucBirthDay[16];
	unsigned char ucAddress[70];
	unsigned char ucID[36];
	unsigned char ucIssue[30];
	unsigned char ucExpirStart[16];
	unsigned char ucExpirEnd[16];

}TAG_TEXT_INFO;

//
//	ע�⣡
//
//		����� DLL ��̬���ӵ� MFC
//		DLL���Ӵ� DLL ������
//		���� MFC ���κκ����ں�������ǰ��
//		��������� AFX_MANAGE_STATE �ꡣ
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CsdtapiApp

BEGIN_MESSAGE_MAP(CsdtapiApp, CWinApp)
END_MESSAGE_MAP()


// CsdtapiApp ����

CsdtapiApp::CsdtapiApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CsdtapiApp ����

CsdtapiApp theApp;


// CsdtapiApp ��ʼ��

BOOL CsdtapiApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

void ReadIniValue(char* sestion,char* keyName,char* out, int outlen, char* iniFile)
{
	char buff[256]="\0";
	GetPrivateProfileString(sestion,keyName,"",buff,sizeof buff,iniFile);
	int len=MultiByteToWideChar(CP_ACP,0,buff,(int)strlen(buff),NULL,0);
	MultiByteToWideChar(CP_ACP,0,buff,(int)strlen(buff),(LPWSTR)out,len);
}

extern "C" int WINAPI EXPORT SDT_OpenPort(int iPort)
{
	return 0x90;
}

extern "C" int WINAPI EXPORT SDT_ClosePort(int iPort)
{
	return 0x90;
}

extern "C" int WINAPI EXPORT SDT_StartFindIDCard(int iPort , unsigned char * pucManaInfo, int iIfOpen )
{
	return 0x9F;
}

extern "C" int WINAPI EXPORT SDT_SelectIDCard(int iPort , unsigned char * pucManaInfo, int iIfOpen )
{
	return 0x90;
}

extern "C" int WINAPI EXPORT SDT_ReadBaseMsg(int iPort, unsigned char * pucCHMsg, unsigned int * puiCHMsgLen, unsigned char * pucPHMsg, unsigned int * puiPHMsgLen, int iIfOpen)
{
	TAG_TEXT_INFO info;
	char path[MAX_PATH] = {0};
	char sestion[]="P";char buff[256]="\0";
	int index=0;

	GetModuleFileName(NULL, path, MAX_PATH);
	*(strrchr(path, '\\') + 1) = '\0';	
	strcat(path,"sdtapi.ini");

	index=::GetPrivateProfileInt("BASE","index",0,path);
	if(index==0)
	{
		srand( (unsigned)time( NULL ) );	
		index=rand() % 100 + 1;
	}
	sprintf(sestion+1,"%d",index);

	memset(&info,0,sizeof info);

	ReadIniValue(sestion,"Name",(char *)info.ucName,sizeof info.ucName,path);
	ReadIniValue(sestion,"Sex",(char *)info.ucSex,sizeof info.ucSex,path);
	ReadIniValue(sestion,"Nation",(char *)info.ucNation,sizeof info.ucNation,path);
	ReadIniValue(sestion,"BirthDay",(char *)info.ucBirthDay,sizeof info.ucBirthDay,path);
	ReadIniValue(sestion,"Address",(char *)info.ucAddress,sizeof info.ucAddress,path);
	ReadIniValue(sestion,"ID",(char *)info.ucID,sizeof info.ucID,path);
	ReadIniValue(sestion,"Issue",(char *)info.ucIssue,sizeof info.ucIssue,path);
	ReadIniValue(sestion,"ExpirStart",(char *)info.ucExpirStart,sizeof info.ucExpirStart,path);
	ReadIniValue(sestion,"ExpirEnd",(char *)info.ucExpirEnd,sizeof info.ucExpirEnd,path);
	
	DeleteFile("sfreg\\imgid.bmp");
	if(::GetPrivateProfileInt(sestion,"Sex",0,path)==1)
	{
		CopyFile("1.jpg", "sfreg\\imgid.jpg", FALSE);
	}
	else
	{
		CopyFile("0.jpg", "sfreg\\imgid.jpg", FALSE);
	}

	memcpy(pucCHMsg,(char*)&info,sizeof TAG_TEXT_INFO);


	return 0x90;
}

/*
typedef int (WINAPI FUN_SDT_OpenPort)( int iPort );
typedef int (WINAPI FUN_SDT_ClosePort)( int iPort );
typedef int (WINAPI FUN_SDT_StartFindIDCard )( int iPort , unsigned char * pucManaInfo, int iIfOpen );
typedef int (WINAPI FUN_SDT_SelectIDCard )( int iPort , unsigned char * pucManaMsg, int iIfOpen );
typedef int (WINAPI FUN_SDT_ReadBaseMsg )( int iPort, unsigned char * pucCHMsg, unsigned int * puiCHMsgLen, unsigned char * pucPHMsg, unsigned int * puiPHMsgLen, int iIfOpen);

typedef int (PASCAL FUN_GetBmp)(char * Wlt_File, int intf);
*/
