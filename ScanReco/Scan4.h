// Scan4.h: interface for the CScan4 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Scan4_H__00A22144_FF5F_4FB0_B6A9_325BBC736A84__INCLUDED_)
#define AFX_Scan4_H__00A22144_FF5F_4FB0_B6A9_325BBC736A84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "stdio.h"
#include "ScanReco.h"
#include "IScan.h"

#define CMD_HOSTID	  0xFA	//������Ϣ�����дα�ʶ���������

#define CMD_CALIBRATE 0x10	//У��ɨ����
#define CMD_INIT	  0x11	//��ʼ��
#define CMD_SCAN	  0x12	//����ʱע��ucDataBuf�б������USHORT���͵�֤������
#define CMD_GetVALUE  0x13	//��չʹ�ã�Ŀǰδ��

//����ɨ����
class CScan4 : public IScan
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

	CScan4();
	virtual ~CScan4();

private:
	HANDLE m_hScanPicCompleteEvent;
	HANDLE m_hCompleteEvent;
	bool m_bOpened;
    PROCESS_INFORMATION m_stProcessInfo;

	BOOL SendHWCmd(USHORT usCmd, DWORD usIDType);
	BOOL StartupHWConnect();
};

#endif // !defined(AFX_Scan4_H__00A22144_FF5F_4FB0_B6A9_325BBC736A84__INCLUDED_)
