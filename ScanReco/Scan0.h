// Scan0.h: interface for the CScan0 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCAN0_H__1D85AD8C_3CD6_46B8_B4C3_8054EA17CBF3__INCLUDED_)
#define AFX_SCAN0_H__1D85AD8C_3CD6_46B8_B4C3_8054EA17CBF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdio.h"
#include "ScanReco.h"
#include "IScan.h"

//���豸��
class CScan0  : public IScan
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

	CScan0();
	virtual ~CScan0();

};

#endif // !defined(AFX_SCAN0_H__1D85AD8C_3CD6_46B8_B4C3_8054EA17CBF3__INCLUDED_)
