#pragma once
#include "IBA.h"
#include "IBADialog.h"


// CDlgNotice �Ի���

class CDlgNotice : public CIBADialog
{
	DECLARE_DYNAMIC(CDlgNotice)

public:
	CDlgNotice(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgNotice();

	void SetParams(LPCTSTR lpTitle,LPCTSTR lpInfo,LPCTSTR lpLink,BOOL bShouldSign, BOOL bUpdate=FALSE);

// �Ի�������
	enum { IDD = IDD_DIALOG_NOTICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	CString m_strTitle;
	CString m_strInfo;
	CString m_strLink;
	BOOL m_bShouldSign;

	CBCGPButton m_btn;

	BOOL m_bMouseAt;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnStnClickedStaticInfo();
	afx_msg void OnBnClickedOk();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
