#pragma once

#include "IBADialog.h"
#include "NetBarConfig2.h"
//
//class:CClientConfigCommonPage �ͻ�������->��������
//
class CClientConfigCommonPage : public CIBAPage
{
	DECLARE_DYNAMIC(CClientConfigCommonPage)
	DECLARE_MESSAGE_MAP()
public:
	CClientConfigCommonPage();
	virtual ~CClientConfigCommonPage();

// �Ի�������
	enum { IDD = IDD_CONFIG_CLIENT };

private:
	
	CNetBarConfig2 m_NetBarConfig2;

	INT m_nCheckInPwdType;
	INT m_nAfterCheckOut;

	UINT m_nShutDownMinutes;

	// ��ʱ�û���������
	BOOL m_bTmpUserChangeComputer;
	// ��Ա�û���������
	BOOL m_bMemberUserChangeComputer;
	// �ͻ�����ʾʹ��ʱ��
	BOOL m_bShowClientUseTime;

	// ����ͻ��˹һ�
	BOOL m_bAllowSuspend;

	//2011-03-15-gxx-����ͻ��������Զ���¼
	BOOL m_bAllowAutoCheckIn;

	UINT m_nNoEnoughTime;


protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual void OnOK();

};
