#pragma once

#include "NDDialog.h"

class CDlgChangeNetId : public CNDDialog
{
	DECLARE_DYNAMIC(CDlgChangeNetId)
	DECLARE_MESSAGE_MAP()

public:
	CDlgChangeNetId(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgChangeNetId();

// �Ի�������
	enum { IDD = IDD_ND_CHANGEUSERSN };

protected:
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonChangeusersn();
	afx_msg void OnBnClickedButtonQuery();

	virtual void ClearData();
	virtual void OnOldCard(UINT nCardId, UINT nMemberId);
	virtual void OnGetBalanceOK(NS_SERVLET::CBalanceMethod & Balance);
	void CheckCard(LPCTSTR strNetId);

private:

	CString m_strMemberID;

	CString m_strNetBar;

	CString m_strPwd;

	CString m_strIdCardSN2;

	UINT m_nOpenNetbarId;
};
