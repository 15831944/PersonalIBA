// Scan1.h: interface for the CScan1 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCAN1_H__00A22144_FF5F_4FB0_B6A9_325BBC736A84__INCLUDED_)
#define AFX_SCAN1_H__00A22144_FF5F_4FB0_B6A9_325BBC736A84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "stdio.h"
#include "ScanReco.h"
#include "IScan.h"
#include "1/CardAll.h"

//E��ͨ FS533
class CScan1 : public IScan
{
public:

	virtual bool IsDeviceValid();										//�豸�Ƿ���Ч, �˴����Լ�����������
	virtual int OpenDevice();											//���豸
	virtual int SetCallBack(CallBack pFunc);							//���ûص�
	virtual int ScanImage(int idType,char* szImgFile);                  //ɨ��ͼƬ
	virtual int RecogniseImage(int idType, IDENTITY_INFO &info,char* szHeadImage); //ʶ��ͼƬ
	virtual int ReadCard(IDENTITY_INFO &info,char* szHeadImage);        //��ȡ����֤
	virtual bool IsSupportReader();									//�Ƿ�֧�ֶ���֤����������ɨ���ǲ�֧�ֶ���֤������
	virtual bool IsSupportScanner();									//�Ƿ�֧��֤��ɨ�蹦�ܣ�������֤��������֧��ɨ��
	virtual bool IsSupportCallBack();									//�Ƿ�֧�ֻص�
	virtual bool IsSupportRecognise(int idType);						//�Ƿ�֧���ض�֤��ʶ��
	virtual bool IsSupportHeadImage(int idType);						//�Ƿ�֧���ض�ͷ���ȡ
	virtual int CloseDevice();                                          //�ر��豸
	virtual void GetErrorMessage(int errCode,char* szErrMsg);           //��ȡ������Ϣ

	CScan1();
	virtual ~CScan1();

private:
	static CScan1 *m_pThis;

	CallBack m_pfnCallBack;
	HANDLE m_hQuitEvent;
	static VOID CALLBACK CheckButtonTimerProc(HWND hwnd,
		UINT uMsg,
		UINT_PTR idEvent,
		DWORD dwTime
		);

	bool m_bOpened;
	char m_cImageFilePath[MAX_PATH];
	HINSTANCE m_hDll;

	Fun_GetVersionInfo *GetVersionInfo;
	Fun_LoadIdcardLibrary *LoadIdcardLibrary;
	Fun_FreeIdcardLibrary *FreeIdcardLibrary;
	Fun_CalibrateScanner *CalibrateScanner;
	Fun_GetButtonDownType *GetButtonDownType;
	Fun_ScanImageEx *ScanImageEx;
	Fun_ScanImageEx_NoBack *ScanImageEx_NoBack;
	Fun_ScanImageOther *ScanImageOther;
	Fun_FeedBackScanner *FeedBackScanner;
	Fun_RecogIdcardExALL *RecogIdcardExALL;
	Fun_RecogIdcardOnlyALL *RecogIdcardOnlyALL;
	Fun_RecogIdcardRe *RecogIdcardRe;
	Fun_RecogDPOnlyALL *RecogDPOnlyALL;
	Fun_RecogDPALL *RecogDPALL;
	Fun_RecogPassPortOnlyALL *RecogPassPortOnlyALL;
	Fun_RecogPassPortALL *RecogPassPortALL;
	Fun_RecogNewIdcardOnlyALL *RecogNewIdcardOnlyALL;
	Fun_RecogNewIdcardALL *RecogNewIdcardALL;
	Fun_RecogPassPortOnlyALL_Region *RecogPassPortOnlyALL_Region;
	Fun_RecogPassPortALL_Region *RecogPassPortALL_Region;
	Fun_RecogTBZOnlyALL *RecogTBZOnlyALL;
	Fun_RecogTBZALL *RecogTBZALL;
	Fun_RecogFXZOnlyALL *RecogFXZOnlyALL;
	Fun_RecogFXZALL *RecogFXZALL;
	Fun_RecogHXZOnlyALL *RecogHXZOnlyALL;
	Fun_RecogHXZALL *RecogHXZALL;
	Fun_RecogMacaoHKFaceALL *RecogMacaoHKFaceALL;
	Fun_RecogMacaoHKFaceOnlyALL *RecogMacaoHKFaceOnlyALL;
};

#endif // !defined(AFX_SCAN1_H__00A22144_FF5F_4FB0_B6A9_325BBC736A84__INCLUDED_)
