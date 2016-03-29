// Scan5.h: interface for the CScan5 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Scan5_H__1DFB2A07_476D_447B_956B_4554AF7E211C__INCLUDED_)
#define AFX_Scan5_H__1DFB2A07_476D_447B_956B_4554AF7E211C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "scanreco.h"
#include "HX_FDX3CardReader.h"
#include "IScan.h"

//����֤������
class CScan5 : public IScan, public CHX_FDX3CardReader  
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

	CScan5();
	virtual ~CScan5();

};

#endif // !defined(AFX_Scan5_H__1DFB2A07_476D_447B_956B_4554AF7E211C__INCLUDED_)
