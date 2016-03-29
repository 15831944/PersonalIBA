
#pragma once

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
struct IDENTITY_INFO
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
}; 

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

int WINAPI OpenDevice();
int WINAPI SetCallBack(CallBack pFunc);
int WINAPI ScanImage(int idType, char* szImgFile);
int WINAPI RecogniseImage(int idType, IDENTITY_INFO &info, char* szHeadImage);
int WINAPI IsSupportCallBack();
int WINAPI IsSupportRecognise(int idType);
int WINAPI IsSupportHeadImage(int idType);
void WINAPI GetErrorMessage(int errCode, char* szErrMsg);
int WINAPI CloseDevice();
int WINAPI IsDeviceValid();
int WINAPI IsSupportReader();
int WINAPI IsSupportScanner();
int WINAPI ReadCard(IDENTITY_INFO &info, char* szHeadImage);

}