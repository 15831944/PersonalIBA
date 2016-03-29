// Scan3.h: interface for the CScan3 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Scan3_H__00A22144_FF5F_4FB0_B6A9_325BBC736A84__INCLUDED_)
#define AFX_Scan3_H__00A22144_FF5F_4FB0_B6A9_325BBC736A84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "stdio.h"
#include "ScanReco.h"
#include "IScan.h"
#include "3/CardAll.h"

//�廪��ͨ��E��ͨFS531
class CScan3  : public IScan
{
public:
	virtual bool IsDeviceValid();										//�豸�Ƿ���Ч, �˴����Լ�����������
	virtual int OpenDevice();											//���豸
	virtual int SetCallBack(CallBack pFunc);							//���ûص�
	virtual int ScanImage(int idType,char* szImgFile);                  //ɨ��ͼƬ
	virtual int RecogniseImage(int idType, IDENTITY_INFO &info,char* szHeadImage); //ʶ��ͼƬ
	virtual int ReadCard(IDENTITY_INFO &info,char* szHeadImage);        //��ȡ����֤
	virtual bool IsSupportReader();										//�Ƿ�֧�ֶ���֤����������ɨ���ǲ�֧�ֶ���֤������
	virtual bool IsSupportScanner();									//�Ƿ�֧��֤��ɨ�蹦�ܣ�������֤��������֧��ɨ��
	virtual bool IsSupportCallBack();									//�Ƿ�֧�ֻص�
	virtual bool IsSupportRecognise(int idType);						//�Ƿ�֧���ض�֤��ʶ��
	virtual bool IsSupportHeadImage(int idType);						//�Ƿ�֧���ض�ͷ���ȡ
	virtual int CloseDevice();                                          //�ر��豸
	virtual void GetErrorMessage(int errCode,char* szErrMsg);           //��ȡ������Ϣ

	CScan3();
	virtual ~CScan3();

private:
	static CScan3 *m_pThis;

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

	Fun_V3_LoadIdcardLibrary *LoadIdcardLibrary;
	Fun_V3_FreeIdcardLibrary *FreeIdcardLibrary;
	Fun_V3_ScanImageEx *ScanImageEx;
	Fun_V3_RecogIdcardExALL *RecogIdcardExALL;
	Fun_V3_RecogIdcardOnlyALL *RecogIdcardOnlyALL;
	Fun_V3_RecogDPOnlyALL *RecogDPOnlyALL;
	Fun_V3_RecogDPALL *RecogDPALL;
	Fun_V3_RecogPassPortOnlyALL *RecogPassPortOnlyALL;
	Fun_V3_RecogPassPortALL *RecogPassPortALL;
	Fun_V3_RecogNewIdcardOnlyALL *RecogNewIdcardOnlyALL;
	Fun_V3_RecogNewIdcardALL *RecogNewIdcardALL;
	Fun_V3_GetButtonDownType *GetButtonDownType;
};

#endif // !defined(AFX_Scan3_H__00A22144_FF5F_4FB0_B6A9_325BBC736A84__INCLUDED_)
