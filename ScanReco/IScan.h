// IScan.h: interface for the IScan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISCAN_H__B1772FDD_0F9C_4E19_BA32_5B36F1694B99__INCLUDED_)
#define AFX_ISCAN_H__B1772FDD_0F9C_4E19_BA32_5B36F1694B99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class IScan  
{
public:
	virtual bool IsDeviceValid() = 0;										//�豸�Ƿ���Ч, �˴����Լ�����������
	virtual int OpenDevice() = 0;											//���豸
	virtual int SetCallBack(CallBack pFunc) = 0;							//���ûص�
	virtual int ScanImage(int idType,char* szImgFile) = 0;                  //ɨ��ͼƬ
	virtual int RecogniseImage(int idType, IDENTITY_INFO &info,char* szHeadImage) = 0; //ʶ��ͼƬ
	virtual int ReadCard(IDENTITY_INFO &info,char* szHeadImage) =0;			//��ȡ����֤
	virtual bool IsSupportReader() = 0;										//�Ƿ�֧�ֶ���֤����������ɨ���ǲ�֧�ֶ���֤������
	virtual bool IsSupportScanner() = 0;									//�Ƿ�֧��֤��ɨ�蹦�ܣ�������֤��������֧��ɨ��
	virtual bool IsSupportCallBack() = 0;									//�Ƿ�֧�ֻص�
	virtual bool IsSupportRecognise(int idType) = 0;						//�Ƿ�֧��ʶ��
	virtual bool IsSupportHeadImage(int idType) = 0;						//�Ƿ�֧��ͷ���ȡ
	virtual int CloseDevice() = 0;                                          //�ر��豸
	virtual void GetErrorMessage(int errCode,char* szErrMsg) = 0;           //��ȡ������Ϣ
public:

};

#endif // !defined(AFX_ISCAN_H__B1772FDD_0F9C_4E19_BA32_5B36F1694B99__INCLUDED_)
