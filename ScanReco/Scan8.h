/////////////////////////////////////////////////////////////
///����ɨ���ǽӿ�

#ifndef AFX_SCAN7_H__00A22144_FF5F_4FB0_B6A9_325BBC736A85__INCLUDED_
#define AFX_SCAN7_H__00A22144_FF5F_4FB0_B6A9_325BBC736A85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000 

#include "stdio.h"
#include "ScanReco.h"
#include "IScan.h"
#include <atlstr.h>

class CScan8: public IScan
{

public:
	CScan8();
	virtual ~CScan8();
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
	void HandleFile();



	typedef int(PASCAL *lpCVR_InitComm)(int port);			//ָ��dll����CVR_InitComm(int port)������ָ��
	typedef int(PASCAL *lpCVR_CloseComm)();					//ָ��dll����CVR_CloseComm()������ָ��
	typedef int(PASCAL *lpCVR_Authenticate)();				//ָ��dll����CVR_Authenticate()������ָ��
	typedef int(PASCAL *lpCVR_Read_Content)(int active);    //ָ��dll����CVR_Read_Content(int active)������ָ��
	typedef int(PASCAL *lpCVR_ReadBaseMsg)(unsigned char *pucCHMsg, unsigned int *puiCHMsgLen, 								
									   unsigned char *pucPHMsg, unsigned int *puiPHMsgLen, 
								       int nMode
                                       );    //��������(�����ڴ�)
	lpCVR_InitComm CVR_InitComm;
	lpCVR_CloseComm CVR_CloseComm;
	lpCVR_Authenticate CVR_Authenticate;
	lpCVR_Read_Content CVR_Read_Content;
	lpCVR_ReadBaseMsg CVR_ReadBaseMsg;

private:
	static CScan8 *m_pThis;
	HINSTANCE m_hDll;
	
	bool m_bOpened ;
	int m_iRetUSB;

};


#endif

