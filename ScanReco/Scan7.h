/////////////////////////////////////////////////////////////
///����ɨ���ǽӿ�

#ifndef AFX_SCAN7_H__00A22144_FF5F_4FB0_B6A9_325BBC736A84__INCLUDED_
#define AFX_SCAN7_H__00A22144_FF5F_4FB0_B6A9_325BBC736A84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define SCAN_COLOR_MODE		    1
#define SCAN_GRAY_MODE			2

#define FIRST_IDCARD				1 //һ�����֤
#define SECOND_IDCARD			2 //�������֤
#define CHINESE_PASSPORT			3 //�й�����
#define INTERNATION_PASSPORT	4 //���ʻ���
#define VISA_PAGE					5 //ǩ֤ҳ
#define TAIBAO_CARD				6 //̨��֤
#define GUANGAO_CARD			7 //�۰�ͨ��֤

#define DRIVER_CARD				8 //��ʻ֤
#define XIANGGUANG_CARD			9 //��۾������֤
#define SOLDER_CARD				10 //ʿ��֤
#define CAP_CARD 					11 //����֤



#include "ScanReco.h"
#include "IScan.h"
#include <atlstr.h>

class CScan7: public IScan
{

public:

	typedef struct
	{
		char name [40];				//����
		char SurnameCH[100];		//������
		char nameCH[100];			//������
		char sex[10];					//�Ա�
		char Gender[10];				//�Ա�Ӣ�ģ�
		char birthday[50];				//��������
		char people[20];				//����
		char signdate[50];				//��֤����
		char validterm[50];			//��Ч����
		char address[200];			//��ַ
		char number[40];				//���֤����
		char organs[50];				//��֤����
		char SurnameEN[100];		//Ӣ����(����ƴ��)
		char nameEN[100];			//Ӣ����(����ƴ��)
		char ENfullname[40];			//Ӣ������	
		char Nationality[200];			//����
		char id[100];					//֤������
		char Leavetime[50];			//�뿪ʱ��
		char placeCH[100];			//ǩ���ص�����
		char placeEN[100];			//ǩ���ص�Ӣ��
		char BirthplaceCH[100];		//���������� 
		char BirthplaceEN[100];		//������Ӣ��
		char szCodeOne[256];		//��һ�д���ʶ����
		char szCodeTwo[256];			//�ڶ��д���ʶ����
		char szCodeThree[256];		//�����д���ʶ����
		char Permitnumber_number[40]; //�۰�֤������
		char Vocational[200];			//ְҵ
		char DocumentsCategory[100];	//֤�����
		char Other[200];				//����
	}SYSCAN_RST;

	// ���Ӻ���
	typedef long (PASCAL *FUN_IO_HasScanner)(LPSTR IVS_600DS, HANDLE hPnpWnd);
	
	// ͼ��ʶ����غ���
	typedef long (PASCAL *FUN_IO_CaptureImage)(BYTE *pIR, long lirSize, BYTE *pWh, long lwhSize);
	
	typedef long (PASCAL *FUN_IO_ProcessImage)(const unsigned char *pSrc, long lImageType,
		long lCardType, unsigned char *pRst, long *rst_w, long *rst_h);

	typedef long (PASCAL *FUN_IO_GetIDImage_Buff)(long lImageType, long lCardType,
		unsigned char *pRst, long *rst_w, long *rst_h);

	typedef long (PASCAL *FUN_IO_GetIDImage_File)(long lImageType, long lCardType, 
		long lCompressRet, char *szFileOutPut);

	typedef long (PASCAL *FUN_IO_IdentifyCard_Buff)(unsigned char* pImageBuff, long lWidth, 
		long lHeight, long lImageType, long lCardType, SYSCAN_RST * pResult);

	typedef long (PASCAL *FUN_IO_IdentifyCard_File)(char* szFileIn, long lCardType, 
		char* szPhotoOut, SYSCAN_RST* pResult);

	typedef long (PASCAL *FUN_IO_GetIDResult)(long lCardType, char* szOutPutFileName_IR, 
		char* szOutPutFileName_HR, char* szPhotoFileName_IR, char* szPhotoFileName_HR , 
		long lCompressRet, SYSCAN_RST* pstRst);

	// оƬʶ����غ���
	typedef BOOL (PASCAL *FUN_IO_StartRFID)(void);

	typedef BOOL (PASCAL *FUN_IO_StopRFID)(void);

	typedef long (PASCAL *FUN_IO_SearchCard)(void);

	typedef long (PASCAL *FUN_IO_SelectCard)(void);

	typedef long (PASCAL *FUN_IO_ReadCard)(SYSCAN_RST * pResult); 

	typedef long (PASCAL *FUN_IO_SSReadCard)(SYSCAN_RST * pResult);

	// ��������
	typedef void  (PASCAL *FUN_IO_Notify)(unsigned long time);

	typedef void  (PASCAL *FUN_IO_NotifyBeep)(unsigned long time);

	typedef long  (PASCAL *FUN_O_GetVersion)(BYTE *pVersion,DWORD nLen);

	typedef long  (PASCAL *FUN_IO_SetFTLight)(int lType, int lSwitch);

	typedef long  (PASCAL *FUN_IO_CreateCalibrateData)(BOOL bBK);

	typedef long  (PASCAL *FUN_IO_UpdateCalibrateData)();

	typedef void  (PASCAL *FUN_IO_CoincideCamera)(BOOL bCoincide);

	typedef HWND (WINAPI *PROCGETTASKMANWND)();



	CScan7();

	virtual ~CScan7();

	virtual bool IsDeviceValid();										//�豸�Ƿ���Ч, �˴����Լ�����������
	
	virtual int OpenDevice();											//���豸
	
	virtual int SetCallBack(CallBack pFunc);							//���ûص�
	
	virtual int ScanImage(int idType,char* szImgFile);                  //ɨ��ͼƬ
	
	virtual int RecogniseImage(int idType, IDENTITY_INFO &info,char* szHeadImage); //ʶ��ͼƬ
	
	virtual int ReadCard(IDENTITY_INFO &info,char* szHeadImage);        //��ȡ����֤
	
	virtual bool IsSupportReader();						//�Ƿ�֧�ֶ���֤����������ɨ���ǲ�֧�ֶ���֤������
	
	virtual bool IsSupportScanner();						//�Ƿ�֧��֤��ɨ�蹦�ܣ�������֤��������֧��ɨ��
	
	virtual bool IsSupportCallBack();						//�Ƿ�֧�ֻص�
	
	virtual bool IsSupportRecognise(int idType);						//�Ƿ�֧���ض�֤��ʶ��
	
	virtual bool IsSupportHeadImage(int idType);						//�Ƿ�֧���ض�ͷ���ȡ
	
	virtual int CloseDevice();                                          //�ر��豸
	
	virtual void GetErrorMessage(int errCode,char* szErrMsg);           //��ȡ������Ϣ

protected:

	PROCGETTASKMANWND GetTaskmanWindow;

	// ���Ӻ���
    FUN_IO_HasScanner m_pfHasScanner;

	// ͼ��ʶ����غ���
	FUN_IO_CaptureImage m_pfCaptureImage;
	FUN_IO_ProcessImage m_pfProcessImage;
	FUN_IO_GetIDImage_Buff m_pfGetIDImage_Buff;
	FUN_IO_GetIDImage_File m_pfGetIDImage_File;
	FUN_IO_IdentifyCard_Buff m_pfIdentifyCard_Buff;
	FUN_IO_IdentifyCard_File m_pfIdentifyCard_File;
	FUN_IO_GetIDResult m_pfGetIDResult;

	// оƬʶ��
	FUN_IO_StartRFID m_pfStartRFID;
	FUN_IO_StopRFID  m_pfStopRFID;
	FUN_IO_SearchCard m_pfSearchCard;
	FUN_IO_SelectCard m_pfSelectCard;
	FUN_IO_ReadCard   m_pfReadCard;
	FUN_IO_SSReadCard m_pfSSReadCard;

	// ��������
	FUN_IO_Notify m_pfNotify;
	FUN_IO_NotifyBeep m_pfNotifyBeep;
	FUN_O_GetVersion  m_pfGetVersion;
	FUN_IO_SetFTLight m_pfSetFTLight;
	FUN_IO_CreateCalibrateData m_pfCreateCalibrateData;
	FUN_IO_UpdateCalibrateData m_pfUpdateCalibrateData;
	FUN_IO_CoincideCamera  m_pfCoincideCamera;

	//int SaveImg(long iCardType);


private:

	static CScan7 *m_pThis;

	int m_iVer;

	HINSTANCE m_hDll;

	bool m_bOpened;

	HWND m_hMainWnd;

	char m_cImageFilePath[MAX_PATH];

	char m_cHeadImageFilePath[MAX_PATH];

	char m_cColorImageFilePath[MAX_PATH];

	char m_cColorHeadImageFilePath[MAX_PATH];

	char m_cTempPath[MAX_PATH];

	void FormatErrorMsg(int iErrorCode,CString strErrorMsg);

	void CreateMyWindow();

};


#endif

