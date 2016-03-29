#pragma once

#include "NetBarConfig2.h"

// CClientConfigBundleTimePage �Ի���

class CClientConfigBundleTimePage : public CIBAPage
{
	DECLARE_DYNAMIC(CClientConfigBundleTimePage)
	DECLARE_MESSAGE_MAP()
public:
	CClientConfigBundleTimePage();
	virtual ~CClientConfigBundleTimePage();

// �Ի�������
	enum { IDD = IDD_CONFIGCLIENT_BUNDLETIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual void OnOK();

private:

	CNetBarConfig2 m_NetBarConfig2;

	BOOL m_bAllowBundTime;

	
public:
	UINT m_nBTLimitTime;
};
