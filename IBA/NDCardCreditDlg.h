#pragma once

#include "NDDialog.h"
#include "CreditCard.h"
// CNDCardCreditDlg �Ի���

class CNDCardCreditDlg : public CNDDialog
{
	DECLARE_DYNAMIC(CNDCardCreditDlg)
	DECLARE_MESSAGE_MAP()
public:
	CNDCardCreditDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNDCardCreditDlg();

// �Ի�������
	enum { IDD = IDD_ND_CARDCREDIT };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonCredit();
	afx_msg void OnBnClickedButtonReadcard();


	void OnOldCard(UINT nCardId, UINT nMemberId);
	void OnNewCard(UINT nCardId, UINT nMemberId);
	void ClearData();
	void OnGetBalanceOK(NS_SERVLET::CBalanceMethod & Balance);

	CString m_strMemberID;
	CString m_strNetBar;
	CString m_strDeposit;
	CString m_strBalanceW;
	CString m_strBalanceY;
	CString m_strBalanceZ;

	CString m_strCardId;
	CString m_strPwd;
	CString m_strMoney;
	CBCGPComboBox m_cboCardType;

	CCreditCard m_CreditCard;
	
	CString m_strPoint;

public:
	afx_msg void OnTimer(UINT nIDEvent);
};
