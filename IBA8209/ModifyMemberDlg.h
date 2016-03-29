#pragma once

#include "IBADialog.h"
#include "NDDialog.h"
// CModifyMemberDlg �Ի���

class CModifyMemberDlg : public CNDDialog
{
	DECLARE_DYNAMIC(CModifyMemberDlg)
	DECLARE_MESSAGE_MAP()
public:
	CModifyMemberDlg(CWnd* pParent = NULL, BOOL bMSuccessClose = FALSE, BOOL bCanEditCardSN = TRUE);   // ��׼���캯��
	virtual ~CModifyMemberDlg();
	CString GetTelNum() { return m_strTelNum;}
	void SetUserInfo(CBalanceMethod & Balance);
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
	
	void UpdateUI();
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

	CString m_strTelNum;
	CString m_strNation;
	CString m_strAddress;
	CString m_strIdDepart;

	UINT m_nNetbarID;

	CString m_strOldPersonalId;
	BOOL	m_bMSuccessClose;		// �޸���ɺ��Ƿ�رնԻ���
	BOOL	m_bCanEditCardSN;		// �ܷ�༭�˺�
	int		m_nSex;
	int		m_nIdType;
};
