#pragma once

#include "NetBarConfig2.h"

// CClientCOnfigUnlockPage �Ի���

class CClientCOnfigUnlockPage : public CIBAPage
{
	DECLARE_DYNAMIC(CClientCOnfigUnlockPage)
	DECLARE_MESSAGE_MAP()

public:
	CClientCOnfigUnlockPage();
	virtual ~CClientCOnfigUnlockPage();

// �Ի�������
	enum { IDD = IDD_CLIENTCONFIG_UNLOCK };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual void OnOK();

private:

	CString m_strPwd;
	// ���ά��ʱ��
	UINT m_nMaxKeepTime;
	// Ӳ��ά��ʱ��
	UINT m_nMaxKeepTimeOfUKey;

	CNetBarConfig2 m_NetBarConfig2;
	
};
