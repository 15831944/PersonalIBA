#pragma once

#include "BCGPLabalButton.h"
//
//class: CDlgIBAAgent ����Ի���
//
class CDlgIBAAgent : public CStringLoaderDlg<CBCGPDialog>
{
	DECLARE_DYNAMIC(CDlgIBAAgent)
	DECLARE_MESSAGE_MAP()

public:
	CDlgIBAAgent(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgIBAAgent();

// �Ի�������
	enum { IDD = IDD_DIALOG_AGENT };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonEmail();
	afx_msg void OnBnClickedButtonBbs();
	afx_msg void OnBnClickedButtonOnline();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

private:
	// ��ʼ������������ֵ
	void InitData();
	// ��������
	CString m_strAgentName;
	// �ֻ�����
	CString m_strMobilePhone;
	// �̶��绰
	CString m_strRegularTel;

	CBCGPLabelButton m_btnEmail;

	CBCGPLabelButton m_btnBBS;

	CBCGPLabelButton m_btnOnline;

	CFont m_font;
	
};
