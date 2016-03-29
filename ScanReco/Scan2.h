// Scan2.h: interface for the CScan2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCAN2_H__00A22144_FF5F_4FB0_B6A9_325BBC736A84__INCLUDED_)
#define AFX_SCAN1_H__00A22144_FF5F_4FB0_B6A9_325BBC736A84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "stdio.h"
#include "ScanReco.h"
#include "IScan.h"
#include "2/IDRCore.h"

//��ʶ A2��A3
class CScan2  : public IScan
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

	CScan2();
	virtual ~CScan2();

private:
	int m_iVer;
	HINSTANCE m_hDll;
	bool m_bOpened;
	char m_cImageFilePath[MAX_PATH];
	char m_cHeadImageFilePath[MAX_PATH];
	char m_cColorImageFilePath[MAX_PATH];
	char m_cColorHeadImageFilePath[MAX_PATH];
	char m_cTempPath[MAX_PATH];
	
	Fun_Device_Open *Device_Open;
	Fun_Device_Close *Device_Close;
	Fun_Get_IdcPic *Get_IdcPic;
	Fun_Get_MiscPic *Get_MiscPic;
	Fun_Get_MiscData *Get_MiscData;
	Fun_Get_ColorPic *Get_ColorPic;
	Fun_Get_IdcData *Get_IdcData;
	Fun_Format_ErrMsg *Format_ErrMsg;
	Fun_BmpToJpg *BmpToJpg;
	Fun_Get_TermbData *Get_TermbData;

};

#endif // !defined(AFX_SCAN2_H__00A22144_FF5F_4FB0_B6A9_325BBC736A84__INCLUDED_)
