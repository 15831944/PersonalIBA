#pragma once

#include "IBADialog.h"
#include "NDDialog.h"
// CModifyMemberDlg �Ի���

class CModifyMemberDlg : public CNDDialog
{
	DECLARE_DYNAMIC(CModifyMemberDlg)
	DECLARE_MESSAGE_MAP()
public:
	CModifyMemberDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CModifyMemberDlg();

// �Ի�������
	enum { IDD = IDD_ND_MODIFYMEMBER };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual void OnOldCard(UINT nCardId, UINT nMemberId);
	virtual void OnNewCard(UINT nCardId);
	virtual void ClearData();
	virtual void OnGetBalanceOK(NS_SERVLET::CBalanceMethod & Balance);
	afx_msg void OnBnClickedButtonModify();
private:

	CBCGPComboBox m_cboSex;
	CBCGPComboBox m_cboIDType;

	CString m_strPwd;

	CString m_strIdCardSN;

	CString m_strCardID;

	CString m_strName;

	CString m_strMemberID;

	CString m_strUserClass;

	CString m_strPersonalID;

	CString m_strNetBar;

	CString m_strAllBalance;

	//��ʱ���� 
	
	CString m_strMemberName;

	UINT m_nNetbarID;

	CString m_strOldPersonalId;

};
