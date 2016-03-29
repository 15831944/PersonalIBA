
#ifndef _CARDALL3_H_
#define _CARDALL3_H_



typedef struct
{
	char name[40];   //����
	char sex[10];     //�Ա�
	char people[20];  //���壬����ʶ��ʱ����Ϊ��
	char birthday[50]; //��������
	char address[200]; //��ַ����ʶ����ʱ�������ǹ������롢
	char signdate[50]; //ǩ�����ڣ���ʶ����ʱ����������Ч���� 
	char validterm[10];  //��Ч�ڣ���ʶ����ʱΪ�գ�
	char number[40];   //֤�����룬��ʶ����ʱΪ���պ� ��ʶ��۰�ͨ��֤��ҳʱΪ�۰�֤������
	char other[100];    //���ã���ʶ����ʱ������������,ʶ��۰�ͨ��֤��ҳʱ֤���ţ�
}IDCARD_ALL3;

/*
//�����з���Ϣ�ṹ
typedef struct
{
	int nSRow; //������ʼλ��
	int nERow; //������ֹλ��
	int nSCol; //������ʼλ��
	int nECol; //������ֹλ��
	int nWidth; //ͼ�����
	int nHeight; //ͼ���߶�

}SQ_RECOGCHARINFO, far* LPSQ_RECOGCHARINFO;

//����ʶ����Ϣ�ṹ
typedef struct
{
	BOOL bAccept; //ʶ�����Ƿ���Խ���
	BYTE FontType; // --û��
	WORD wCode [10]; //ʶ������
	int nErrDist[10]; //ʶ�������Ŷȣ�ֵԽС���Ŷ�Խ�ߡ�

}ZQ_RECOGCHARRESULT, far* LPZQ_RECOGCHARRESULT;

//������Ϣ�ṹ
typedef struct
{
	RECT rc; //��������ͼ��������λ��
	int nCharCount; //���������ַ���Ŀ
	SQ_RECOGCHARINFO RecogInfo[50]; //�������������ַ����з���Ϣ
	ZQ_RECOGCHARRESULT RecogResult[50]; //�������������ַ���ʶ����Ϣ

}ST_REGION, FAR* LPST_REGION;

//���֤�ṹ
typedef struct
{
	RECT rcPhoto;
	ST_REGION name;
	ST_REGION sex;
	ST_REGION people;
	ST_REGION birthday;
	ST_REGION address[5];
	ST_REGION signdate;
	ST_REGION validterm;
	ST_REGION number;
	int address_line_count;
}ST_IDCARD, FAR* LPST_IDCARD;
*/


typedef int   (WINAPI Fun_V3_LoadIdcardLibrary)();
typedef void  (WINAPI Fun_V3_FreeIdcardLibrary)();
typedef int   (WINAPI Fun_V3_ScanImageEx)( char* pchFilePath, short nButtonType );
typedef int   (WINAPI Fun_V3_RecogIdcardExALL)( LPTSTR imgfile_name, IDCARD_ALL3 & card, bool bSaveResultFile, int nHandPrint, int nContent, char* HeadImgFName, short nButtonType );
typedef int   (WINAPI Fun_V3_RecogIdcardOnlyALL)( LPCTSTR imgfile_name, IDCARD_ALL3 & card, bool bSaveResultFile, int nHandPrint, int nContent, char* HeadImgFName );
typedef int   (WINAPI Fun_V3_RecogDPOnlyALL)( LPCTSTR imgfile_name, IDCARD_ALL3 & card, bool bSaveResultFile, int nContent, char* HeadImgFName );
typedef int   (WINAPI Fun_V3_RecogDPALL)( LPTSTR imgfile_name, IDCARD_ALL3 & card, bool bSaveResultFile, int nContent, char* HeadImgFName );
typedef int   (WINAPI Fun_V3_RecogPassPortOnlyALL)( LPCTSTR imgfile_name, IDCARD_ALL3 & card, bool bSaveResultFile, int nContent, char* HeadImgFName );
typedef int   (WINAPI Fun_V3_RecogPassPortALL)( LPTSTR imgfile_name, IDCARD_ALL3 & card, bool bSaveResultFile, int nContent, char* HeadImgFName );
typedef int   (WINAPI Fun_V3_RecogNewIdcardOnlyALL)( LPCTSTR imgfile_name, IDCARD_ALL3 & card, bool bSaveResultFile, int nContent, char* HeadImgFName );
typedef int   (WINAPI Fun_V3_RecogNewIdcardALL)( LPTSTR imgfile_name, IDCARD_ALL3& card, bool bSaveResultFile, int nContent, char* HeadImgFName );
typedef int   (WINAPI Fun_V3_GetButtonDownType)(void);

#endif