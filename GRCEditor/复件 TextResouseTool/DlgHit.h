#pragma once


// CDlgHit �Ի���

class CDlgHit : public CDialog
{
	DECLARE_DYNAMIC(CDlgHit)

public:
	CDlgHit(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgHit();

// �Ի�������
	enum { IDD = IDD_DLG_HIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL m_bNextNotHit;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
