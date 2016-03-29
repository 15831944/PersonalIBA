#pragma once

#include "resource.h"
#include "WebBrowser.h"

// CPayCardDlg �Ի���

class CPayCardDlg : public CG2XDialog, public CWebBrowserEventHandler
{
	DECLARE_DYNAMIC(CPayCardDlg)
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()

public:
	CPayCardDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPayCardDlg();

// �Ի�������
	enum { IDD = IDD_PAYCARD_DIALOG };

	HICON m_hIcon;

	BOOL AnalyzeParamString(LPCTSTR lpszParams);

	void OpenHomePage(BOOL bCashWay,BOOL bOpened);

	//CString EncodeURL(LPCTSTR lpszUrl);

private:
	CString m_strCashID; // ����̨ID

	CString m_strCasherPwd; // ����Ա����

	CString m_strNetbarId;

	CString m_strDomainId;

	CString m_strCashier;

	CString m_strDutyId;

	CString m_strDdnServer;


private:
	CString m_strURLMain;
	CString m_strURLBakup;

	CWebBrowser* m_lpWebBrowser;

	BOOL CreateWebBrower();

	CString GetHomePageURL();

	CString GetLocalIP();

public:
	BOOL m_bOldNetbar; // �Ƿ�Ϊ�ϵ�����̨

protected:

	void EnableRemoteInput(BOOL bEnable);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnCancel();
	virtual void OnOK();
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	virtual void WebBrowser_BeforeNavigate2(CWebBrowser& Sender, LPCTSTR lpszURL, DWORD nFlags,
		LPCTSTR lpszTargetFrameName, CByteArray& baPostedData,
		LPCTSTR lpszHeaders, BOOL* pbCancel);

	virtual HRESULT WebBrowser_GetExternal(CWebBrowser& Sender, LPDISPATCH *lppDispatch);

	BOOL PrintText(INT nFontSize, LPCTSTR lpszText);
	void SplitLine(CString& strLine, CStringArray& strArray, TCHAR ch /*= '|'*/);
	
public:
	afx_msg void OnPaint();
};
