#pragma once

//��Ҫ�ı�����ĺ궨��

#define WM_REALNAME WM_USER + 100

// RZX

#define RN_RZX_IDUSER 1
#define RN_RZX_ADDMEMBER 2

//GLW
#define RN_GLW_IDUSER 101

//******************************************************************************

//!< RZX��غ����ӿڣ���ʼ��

#define WM_USER_IDCARD_MANAGE    WM_USER + 2000

//WPARM ����
#define IDCARD_MANAGE_ADD        	1 // ����
#define IDCARD_MANAGE_LOSD       	2 // ��ʧ
#define IDCARD_MANAGE_MODIFY     	3 // �޸�����
#define IDCARD_MANAGE_MEMBER     	4 // �����Ա
#define IDCARD_MANAGE_CLOSE     	5 // �رճ���
#define IDCARD_MANAGE_GETPWD   		6//�������루���������
#define IDCARD_MANAGE_DOWNLOST		7//�ֶ����ع�ʧ��
#define IDCARD_MANAGE_ABOUT			8 //����
#define IDCARD_MANAGE_VIEWCARD   	9//�鿴������¼
#define IDCARD_MANAGE_VIEWLOST		10//�鿴��ʧ��¼


typedef struct REQ_REGISTER_INFO_tag
{
	char szName[31];		 //�û���
	char szCertificateType[3];//֤������
	char szCertificateID[19]; //֤������
	char szCompany[31];   //��˾���ַ����
	char szCountry[11]; 	//������
	BYTE bFolk; // ����
	char szDetail[30]; 		//������Ϣ
}REQ_REGISTER_INFO;


int IDUser(char *pszIDNo, char *pszUserNo, char *pszPassword,
			BYTE IDCardType, BOOL bValidate, const REQ_REGISTER_INFO& RegInfo);

int AddToMemberRZX(char *pszIDNo, char *pszUserNo, 
					char *pszPassword,BYTE IDCardType, const REQ_REGISTER_INFO& RegInfo);

int RzxNotiFy(int nNotifyType, const char * lpNotifyMsg);

//!< RZX��غ����ӿڣ�������

//******************************************************************************