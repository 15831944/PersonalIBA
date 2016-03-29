#pragma once

#include "Servlet\ShiftCreditMethod.h"
#include ".\NDDialog.h"
#include "BCGPLinkStatic.h"

#include ".\common\GxxListCtrl.h"
//
//class:CCreditReverseDlg ��ֵ�����Ի���
//
class CCreditReverseDlg : public CNDDialog
{
	DECLARE_DYNAMIC(CCreditReverseDlg)
	DECLARE_MESSAGE_MAP()

public:
	CCreditReverseDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCreditReverseDlg();

// �Ի�������
	enum { IDD = IDD_ND_CREDITREVERSE };

	enum eColumns
	{
		eCreditTime = 0,   // ��ֵʱ��
		eCreditMoney = 1,  // ��ֵ���
		ePresentMoney = 2, // ���ͽ��
		eModify = 3,       // �޸�
		eDelete = 4        // ɾ��
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	void InitControls();
	void ClearData();
	void GetCreditInfos(UINT nMemberId);

	afx_msg void OnNMClickListCredit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonQuery();
	afx_msg void OnBnClickedButtonRecentcredit();

	void OnOldCard
		(UINT nCardId, UINT nMemberId);
	void OnGetBalanceOK(NS_SERVLET::CBalanceMethod & Balance);

	BOOL DeleteCredit(BOOL bSendMsgToClient=TRUE);


private:
//	CBCGPListCtrl m_CreditList;
	NS_COMMON::CGxxListCtrl m_CreditList;

	CString m_strCreditBalance;

	CString m_strMemberID;

	CString m_strNetBar;

	CString m_strDeposit;

	CString m_strBalanceW;

	CString m_strBalanceY;

	CString m_strBalanceZ;

	CString m_strCreditTime;

	CString m_strCreditMoney;

	CString m_strRefNo;

	CString m_strLastUserID;

	NS_SERVLET::CShiftCreditMethod ShiftCredit;
public:
	afx_msg void OnTimer(UINT nIDEvent);

	afx_msg void OnGLCNotify(LPNMHDR lpHdr, LRESULT *pResult);
};
