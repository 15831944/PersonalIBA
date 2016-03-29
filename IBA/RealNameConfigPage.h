#pragma once

#include "IBADialog.h"
#include "NetBarConfig2.h"

class CConfigOfRealName : public CIBADialog
{
	DECLARE_DYNAMIC(CConfigOfRealName)
	DECLARE_MESSAGE_MAP()

public:
	CConfigOfRealName(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CConfigOfRealName();

// �Ի�������
	enum { IDD = IDD_REALNAMECONFIG };

protected:
	
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnBnClickedOk();
	
private:

	CNetBarConfig2 m_NetBarConfig2;

	CString m_ProxyIP;
	CString m_WWMIP;

	CString m_strWWMIPTIP;

	BOOL m_bNetIdentityManage;

};
