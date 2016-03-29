#pragma once

#include ".\NDDialog.h"
// CNDCreditDirectDlg �Ի���

class CNDCreditDirectDlg : public CNDDialog
{
	DECLARE_DYNAMIC(CNDCreditDirectDlg)
	DECLARE_MESSAGE_MAP()

public:
	CNDCreditDirectDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNDCreditDirectDlg();

	// �Ի�������
	enum { IDD = IDD_ND_CREDITDIRECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	virtual void ClearData();
	virtual void OnGetBalanceOK(NS_SERVLET::CBalanceMethod & Balance);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	virtual BOOL DoLocalModeQuery(); // 2011/08/04-8201-gxx: 

private :

	CBCGPMaskEdit m_edtCreditMoney;
	CBCGPComboBox m_cboAccount;
	CBCGPComboBox m_cboCreditKind;

	CString m_strNetBar;
	CString m_strBalanceW;
	CString m_strBalanceY;
	CString m_strBalanceZ;
public:
	afx_msg void OnTimer(UINT nIDEvent);
};
