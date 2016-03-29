#pragma once

#include "NDDialog.h"
#include "ColorGridCtrl.h"

// CNDCheckInDlg �Ի���

class CNDCheckInDlg : public CNDDialog
{
	DECLARE_DYNAMIC(CNDCheckInDlg)
	DECLARE_MESSAGE_MAP()

public:
	CNDCheckInDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNDCheckInDlg();

// �Ի�������
	enum { IDD = IDD_ND_CHECKINDLG };

protected:
	
	virtual void OnGetBalanceOK(CBalanceMethod & Balance);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonRefresh();

	virtual BOOL DoLocalModeQuery();

	BOOL m_bCheckIn;

	CString m_strPwd;

	BOOL m_bPowerOn;

	CIBAGrid m_wndGridTree;

	void InitGridTree();
	void FillGridTree();

	void CheckoutOldComputer();
	BOOL CheckTakeUp(CString strTermId);



public:
	
	BOOL GetCheckIn() const { return m_bCheckIn; }
	void SetCheckIn(BOOL newVal) { m_bCheckIn = newVal; }

	void SetNetId(CString strNetId);
	
};
