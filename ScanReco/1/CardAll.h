
#ifndef _CARDALL_H_
#define _CARDALL_H_


struct Scan_Para
{

	enum Scan_Mode
	{
		SCAN_GRAY_MODE      = 1,
		SCAN_BW_MODE		= 2,
		SCAN_TRUECOLOR_MODE = 4,
		SCAN_FILTER_MODE	= 64,
	};
	enum Scan_DPI
	{
		SCAN_200_DPI   = 200,
		SCAN_300_DPI   = 300,
		SCAN_400_DPI   = 400,
		SCAN_600_DPI   = 600,	
		SCAN_1200_DPI  = 1200,
	};


	int nLeft;   // seletecd scanning range in 1/100 inch for general scanner or 1/1000 for film scanner
	int nTop;
	int nRight;
	int nBottom;
	Scan_Mode sMode; 
	int nBrightness;         // from -100 to 100%
	int nContrast;          // from -100 to 100%
	Scan_DPI Resolution;
	int nGamma;     	    // from 1 to 99��default 10
	int nHighlight;         // from 1 to 255,default 255
	int nQualityscan;       // 0 : speed scan 1:quality scan,default 1
};

typedef struct
{
	char name[40];   //����
	char sex[10];     //�Ա�
	char people[20];  //���壬����ʶ��ʱ����Ϊ��
	char birthday[50]; //��������
	char address[200]; //��ַ����ʶ����ʱ�������ǹ������롢
	char signdate[50]; //ǩ�����ڣ���ʶ����ʱ����������Ч���� 
	char validterm[50];  //��Ч�ڣ���ʶ����ʱΪ�գ�
	char number[40];   //֤�����룬��ʶ����ʱΪ���պ� ��ʶ��۰�ͨ��֤��ҳʱΪ�۰�֤������
	char other[100];    //���ã���ʶ����ʱ������������,ʶ��۰�ͨ��֤��ҳʱ֤���ţ�
	char szCodeOne[256];//���ա�̨��֤���۰�ͨ��֤��ҳ��һ�л����룬
	char szCodeTwo[256];//���ա�̨��֤���۰�ͨ��֤��ҳ�ڶ��л����룬
}IDCARD_ALL;


typedef char *(WINAPI Fun_GetVersionInfo)();
typedef int   (WINAPI Fun_LoadIdcardLibrary)();
typedef void  (WINAPI Fun_FreeIdcardLibrary)();
typedef void  (WINAPI Fun_CalibrateScanner)();
typedef int   (WINAPI Fun_GetButtonDownType)(void);
typedef int   (WINAPI Fun_ScanImageEx)( char* pchFilePath, short nButtonType );
typedef int   (WINAPI Fun_ScanImageEx_NoBack)( char* pchFilePath, short nButtonType );
typedef int   (WINAPI Fun_ScanImageOther)( char* pchFilePath, Scan_Para& sP );
typedef void  (WINAPI Fun_FeedBackScanner)();
typedef int   (WINAPI Fun_RecogIdcardExALL)( LPTSTR imgfile_name, IDCARD_ALL & card, bool bSaveResultFile, int nHandPrint, int nContent, char* HeadImgFName, short nButtonType );
typedef int   (WINAPI Fun_RecogIdcardOnlyALL)( LPCTSTR imgfile_name, IDCARD_ALL & card, bool bSaveResultFile, int nHandPrint, int nContent, char* HeadImgFName );
typedef int   (WINAPI Fun_RecogIdcardRe)( LPTSTR imgfile_name, IDCARD_ALL& card, bool bSaveResultFile, int nHandPrint, int nContent, char* HeadImgFName, short nType, bool bIsNeedRevers );
typedef int   (WINAPI Fun_RecogDPOnlyALL)( LPCTSTR imgfile_name, IDCARD_ALL & card, bool bSaveResultFile, int nContent, char* HeadImgFName );
typedef int   (WINAPI Fun_RecogDPALL)( LPTSTR imgfile_name, IDCARD_ALL & card, bool bSaveResultFile, int nContent, char* HeadImgFName );
typedef int   (WINAPI Fun_RecogPassPortOnlyALL)( LPCTSTR imgfile_name, IDCARD_ALL & card, bool bSaveResultFile, int nContent, char* HeadImgFName );
typedef int   (WINAPI Fun_RecogPassPortALL)( LPTSTR imgfile_name, IDCARD_ALL & card, bool bSaveResultFile, int nContent, char* HeadImgFName );
typedef int   (WINAPI Fun_RecogNewIdcardOnlyALL)( LPCTSTR imgfile_name, IDCARD_ALL & card, bool bSaveResultFile, int nContent, char* HeadImgFName );
typedef int   (WINAPI Fun_RecogNewIdcardALL)( LPTSTR imgfile_name, IDCARD_ALL& card, bool bSaveResultFile, int nContent, char* HeadImgFName );
typedef int   (WINAPI Fun_RecogPassPortOnlyALL_Region)( LPCTSTR imgfile_name, IDCARD_ALL & card, bool bSaveResultFile, int nContent );
typedef int   (WINAPI Fun_RecogPassPortALL_Region)( LPTSTR imgfile_name, IDCARD_ALL & card, bool bSaveResultFile, int nContent );
typedef int   (WINAPI Fun_RecogTBZOnlyALL)(LPCTSTR imgfile_name,IDCARD_ALL & card,bool bSaveResultFile, int nContent,char* HeadImgFName);
typedef int   (WINAPI Fun_RecogTBZALL)(LPTSTR imgfile_name,IDCARD_ALL & card,bool bSaveResultFile, int nContent,char* HeadImgFName);
typedef int   (WINAPI Fun_RecogFXZOnlyALL)(LPCTSTR imgfile_name,IDCARD_ALL & card,bool bSaveResultFile, int nContent,char* HeadImgFName);
typedef int   (WINAPI Fun_RecogFXZALL)(LPTSTR imgfile_name,IDCARD_ALL & card,bool bSaveResultFile, int nContent,char* HeadImgFName);
typedef int   (WINAPI Fun_RecogHXZOnlyALL)(LPCTSTR imgfile_name,IDCARD_ALL & card,bool bSaveResultFile, int nContent,char* HeadImgFName);
typedef int   (WINAPI Fun_RecogHXZALL)(LPTSTR imgfile_name,IDCARD_ALL & card,bool bSaveResultFile, int nContent,char* HeadImgFName);
typedef int   (WINAPI Fun_RecogMacaoHKFaceALL)(LPTSTR imgfile_name,IDCARD_ALL & card,bool bSaveResultFile, int nContent,char* HeadImgFName);
typedef int   (WINAPI Fun_RecogMacaoHKFaceOnlyALL)(LPTSTR imgfile_name,IDCARD_ALL & card,bool bSaveResultFile, int nContent,char* HeadImgFName);

#endif