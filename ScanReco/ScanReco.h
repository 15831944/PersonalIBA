
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SCANRECO_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SCANRECO_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef SCANRECO_EXPORTS
#define SCANRECO_API __declspec(dllexport)
#else
#define SCANRECO_API __declspec(dllimport)
#endif

#ifndef _SCANRECO_H
#define _SCANRECO_H

enum IDENTITY_TYPE
{
		IDENTITY_ID1   = 10, //һ�����֤
		IDENTITY_ID2   = 11, //�������֤
		IDENTITY_STUDENT   = 15, //ѧ��֤
		IDENTITY_OFFICER   = 90, //����֤
		IDENTITY_POLICE   = 91, //����֤
		IDENTITY_SOLDIER   = 92, //ʿ��֤
		IDENTITY_HUKOUBU   = 93, //���ڲ�
		IDENTITY_PASSPORT   = 94, //����
		IDENTITY_TAIBAO   = 95, //̨��֤
		IDENTITY_GANGHAO  = 96,	// �۰�ͨ��֤
		IDENTITY_OTHER   = 99, //����֤��
		IDENTITY_ID2_BACK  = 200	// ����֤����
};	

//2.2	�����Ϣ�ṹ 
typedef struct
{
	char name[20];     //����
	char sex[2];        //�Ա�
	char number[40];   //֤������
	char nation[20];    //����
	char birthday[10];  //��������
	char address[200];  //��ַ
	char signdate[10];  //ǩ������
	char validterm[30];  //��Ч��
	char department[40]; //��֤����
	
}IDENTITY_INFO; 


//typedef struct
//{
//	char name[30];     //����
//	char sex[4];        //�Ա�
//	char nation[4];    //����
//	char birthday[16];  //��������
//	char address[70];  //��ַ
//	char number[36];   //֤������
//	char department[30]; //��֤����
//	char signdate[16];  //ǩ������
//	char validterm[40];  //��Ч��
//}IDENTITY_INFO; 

#define ERROR_SCANNER_OTHER 0
#define ERROR_SCANNER_OK 1
#define ERROR_SCANNER_LOADDLL 2
#define ERROR_SCANNER_HARDWARE 3
#define ERROR_SCANNER_PROCESS 4
#define ERROR_SCANNER_FILEPATH 5
#define ERROR_SCANNER_RECOGNISE 6
#define ERROR_SCANNER_OFFLINE   7
#define ERROR_SCANNER_UNSUPPORT 8

extern "C" 
{
	typedef VOID (CALLBACK *CallBack)(int idType);
	
	
	SCANRECO_API int WINAPI OpenDevice();
	SCANRECO_API int WINAPI SetCallBack(CallBack pFunc);
	SCANRECO_API int WINAPI ScanImage(int idType,char* szImgFile);
	SCANRECO_API int WINAPI RecogniseImage(int idType, IDENTITY_INFO &info,char* szHeadImage);
	SCANRECO_API int WINAPI IsSupportCallBack();
	SCANRECO_API int WINAPI IsSupportRecognise(int idType);
	SCANRECO_API int WINAPI IsSupportHeadImage(int idType);
	SCANRECO_API void WINAPI GetErrorMessage(int errCode,char* szErrMsg);
	SCANRECO_API int WINAPI CloseDevice();
	SCANRECO_API int WINAPI IsDeviceValid();
	SCANRECO_API int WINAPI IsSupportReader();
	SCANRECO_API int WINAPI IsSupportScanner();
	SCANRECO_API int WINAPI ReadCard(IDENTITY_INFO &info,char* szHeadImage);
	
}

extern char g_cIniFile[MAX_PATH];
extern const char *g_cErrorDict[][2];

#endif//_SCANRECO_H