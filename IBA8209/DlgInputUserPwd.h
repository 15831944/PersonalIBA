#pragma once

#include "IBADialog.h"

// CDlgInputUserPwd �Ի���

class CDlgInputUserPwd : public CIBADialog
{
	DECLARE_DYNAMIC(CDlgInputUserPwd)

public:
	CDlgInputUserPwd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgInputUserPwd();

	CString GetPwd() { return m_strPwd; }
// �Ի�������
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedOk();
private:
	CString m_strPwd;
};
