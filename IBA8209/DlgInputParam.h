#pragma once

#include "IBADialog.h"
// CDlgInputParam �Ի���

class CDlgInputParam : public CIBADialog
{
	DECLARE_DYNAMIC(CDlgInputParam)

public:
	CDlgInputParam(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgInputParam();

// �Ի�������
	enum { IDD = IDD_DLG_INPUT_PARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	void InitParam(CString strCaption,CString strTitle,CString strParam=_T(""));
public:
	CString m_strParam; // ������ֵ
	CString m_strTitle; // ����
	CString m_strCaption; // �Ի���ı���
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();


};
