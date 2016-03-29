#pragma once

#include "NDDialog.h"

// CNDPointExchangeDlg �Ի���

class CNDPointExchangeDlg : public CNDDialog
{
	DECLARE_DYNAMIC(CNDPointExchangeDlg)
	DECLARE_MESSAGE_MAP()

public:
	CNDPointExchangeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNDPointExchangeDlg();

// �Ի�������
	enum { IDD = IDD_JXC_POINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog(); 
	virtual void OnOldCard(UINT nCardId, UINT nMemberId);
	virtual void OnGetBalanceOK(NS_SERVLET::CBalanceMethod & Balance);
	virtual void ClearData();
	afx_msg void OnBnClickedOk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

private:

	// �����ܶ�
	CString m_strPoint;
	// �˻��ܶ�
	CString m_strAllMoney;
	// �������
	UINT m_nExchangePoint;
	// ��Ʒ����
	CString m_strPresentName;
	// ����
	CEdit m_edtPassWord;
	// �һ�����
	UINT m_nCount;

	CString m_strNetBar;

};
