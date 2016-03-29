#pragma once

#include "NDDialog.h"
// CNDUpgradeDlg �Ի���

class CNDUpgradeDlg : public CNDDialog
{
	DECLARE_DYNAMIC(CNDUpgradeDlg)
	DECLARE_MESSAGE_MAP()

public:

	CNDUpgradeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNDUpgradeDlg();

// �Ի�������
	enum { IDD = IDD_ND_UPGRADE };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	void OnOldCard(UINT nCardId, UINT nMemberId);
	void OnGetBalanceOK(NS_SERVLET::CBalanceMethod & Balance);
	afx_msg void OnBnClickedButtonUpgrade();

private:

	CString m_strMemberID;
	CString m_strNetBar;
	CString m_strDeposit;
	CString m_strBalanceW;
	CString m_strBalanceY;
	CString m_strBalanceZ;
	CString m_strMemberName;

	CString m_strReason;

	CBCGPComboBox m_cboUserClass;
};
