#pragma once

#include "IBADialog.h"
// CDlgRNZdCrc �Ի���

class CDlgRNZdCrc : public CIBADialog
{
	DECLARE_DYNAMIC(CDlgRNZdCrc)

public:
	CDlgRNZdCrc(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgRNZdCrc();

	DWORD GetCRC() { return m_dwCRC; }
// �Ի�������
	enum { IDD = IDD_DLG_REALNAME_ZDcrc };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedOk();

public:
	DWORD m_dwCRC;
};
