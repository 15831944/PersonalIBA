#pragma once

#include "NDDialog.h"
#include "afxwin.h"
// CMergeMemberDlg �Ի���

class CMergeMemberDlg : public CNDDialog
{
	DECLARE_DYNAMIC(CMergeMemberDlg)
	DECLARE_MESSAGE_MAP()

public:
	CMergeMemberDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMergeMemberDlg();

// �Ի�������
	enum { IDD = IDD_ND_MERGEMEMBER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

private:
	//******************���˻�����********************//
	// �����˺�
	CString m_strNetId;
	// �û�����
	CString m_strUserName;
	// �û���
	CString m_strMemberId;
	// ֤����
	CString m_strPersonalId;
	// ��������
	CString m_strNetbarName;
	// �û�����
	CString m_strClassName;
	// �����˺�
	CString m_strWSAmount;
	// �����˻�
	CString m_strYSAmount;
	// �����˻�
	CString m_strZSAmount;
	// �ϼ��˻�
	CString m_strAllAmount;
	//****************���ϲ��˻�����*****************//
	// �����˺�
	CString m_strNetId2;
	// �û�����
	CString m_strUserName2;
	// �û���
	CString m_strMemberId2;
	// ֤����
	CString m_strPersonalId2;
	// ��������
	CString m_strNetbarName2;
	// �û�����
	CString m_strClassName2;
	// �����˺�
	CString m_strWSAmount2;
	// �����˻�
	CString m_strYSAmount2;
	// �����˻�
	CString m_strZSAmount2;
	// �ϼ��˻�
	CString m_strAllAmount2;

private:

	CFont m_FontAllMoney;

	BOOL m_bPrimary;

	CString m_strCardID;
	CString m_strMemberName;
	UINT m_nUserClassID;
	INT m_nAllMoney;

	CString m_strCardID2;
	CString m_strMemberName2;
	UINT m_nUserClassID2;
	INT m_nAllMoney2;

private:
	void InitControls();
	void OnGetBalanceOK(NS_SERVLET::CBalanceMethod & Balance);
	void OnOldCard(UINT nCardId, UINT nMemberId);

	void OnNewCard(UINT nCardId);
	BOOL CheckInput();

	void ClearData();
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnSetfocusEditIdcardsno();
	afx_msg void OnEnSetfocusEditIdcardsno2();
	afx_msg void OnBnClickedButtonQuery();
	void DoQuery(CString strNetId);
private:
	// ��������������ܽ��ܿؼ���Ϣ
	CEdit m_edtNetId;
	// ��������������ܽ��ܿؼ���Ϣ
	CEdit m_edtNetId2;

public:
	// ���ϲ��˻�����
	CString m_strPassWord;
};
