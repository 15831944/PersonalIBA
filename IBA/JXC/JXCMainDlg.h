#pragma once

#include "..\IBADialog.h"
#include "JXCSellDlg.h"
#include "JXCReturnDlg.h"
#include "JXCQueryDlg.h"
#include "JXCExchangeDlg.h"
#include "JXCPresentDlg.h"
#include "jxcstoredlg.h"
#include "jxcretailonlinedlg.h"
// CJXCMainDlg �Ի���

class CJXCMainDlg : public CIBADialog
{
	DECLARE_DYNAMIC(CJXCMainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CJXCMainDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CJXCMainDlg();

// �Ի�������
	enum { IDD = IDD_JXC};

public:

	static CJXCMainDlg* lpJXCMainDlg;

	static void ShowJxcMainDlg();

	static void DestroyJxcMainDlg();

	static BOOL IsJxcMainDlgCreated();

	static void AutoChangeSkin();

	static void RetailOnline(CString strTermId, CString strNetId);

protected:
	
	void CreateTab();
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnNcDestroy();

private:

	CBCGPTabWnd	m_wndTab;
	
	CJXCSellDlg m_SellDlg;

	CJXCReturnDlg m_ReturnDlg;
	
	CJXCQueryDlg m_QueryDlg;

	CJXCExchangeDlg m_ExchangeDlg;

	CJXCPresentDlg  m_PresentDlg;

	CJXCStoreDlg  m_StoreDlg;

	CJXCRetailOnlineDlg m_RetailOnlineDlg;

public:



};
