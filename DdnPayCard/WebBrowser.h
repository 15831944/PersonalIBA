#pragma once

//�¼��ص��ӿڣ������ǽӿ�Ҳ��������

//����ǽӿھ�Ҫ�ڵ�����������ʵ�����е��¼�
//��ʱ����ֻ��Ҫʵ��һ�����¼�����������������ʵ���¼��ص��ӿ�

//�����е��¼��������������

class CWebBrowserEventHandler
{
protected:

	friend class CWebBrowser;	 //���¼�Դ���Ե����¼��ص�����
	
	CWebBrowserEventHandler(){}; //�������캯����ֻ�ܼ̳У�����ʵ����

	//������¼��ص�����
	
	//��һ������Ϊ�¼�Դ��ֻҪ��Ϊ�����־�����¼�Դ

	virtual void WebBrowser_BeforeNavigate2(CWebBrowser& Sender, LPCTSTR lpszURL, DWORD nFlags,
		LPCTSTR lpszTargetFrameName, CByteArray& baPostedData,
		LPCTSTR lpszHeaders, BOOL* pbCancel){}

	virtual void WebBrowser_TitleChange(CWebBrowser& Sender, LPCTSTR lpszText){}

	virtual void WebBrowser_DocumentComplete(CWebBrowser& Sender, LPCTSTR lpszURL){}

	virtual void WebBrowser_NewWindow2(CWebBrowser& Sender, LPDISPATCH* ppDisp, BOOL* Cancel){}

	virtual void WebBrowser_NavigateError(CWebBrowser& Sender, LPCTSTR lpszURL, LPCTSTR lpszFrame, DWORD dwError, BOOL *pbCancel){}

	virtual HRESULT WebBrowser_GetExternal(CWebBrowser& Sender, LPDISPATCH *lppDispatch){return S_FALSE;}
};

class CWebBrowser : public CHtmlView
{
	DECLARE_DYNCREATE(CWebBrowser)
	DECLARE_MESSAGE_MAP()

public:

	CWebBrowser();          
	virtual ~CWebBrowser();

	//���û���ȡ���¼��ص��ӿ�
	void SetEventHandler(CWebBrowserEventHandler* lpEventHandler = NULL);

	BOOL IsContextMenuEnabled() const { return m_bContextMenuEnabled; }
	void EnableContextMenu(BOOL newVal = TRUE) { m_bContextMenuEnabled = newVal; }

	BOOL IsFlatStyle() const { return m_bIsFlatStyle; }
	void SetFlatStyle(BOOL newVal = TRUE) { m_bIsFlatStyle = newVal; }

protected:

	virtual void PostNcDestroy();

	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();

protected:

	//CHtmlView�Ѿ����麯��ʵ���¼�����Ӧ
	
	//���������ʵ���¼�����Ӧ���Բ鿴CHtmlView��Դ��

	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags,
		LPCTSTR lpszTargetFrameName, CByteArray& baPostedData,
		LPCTSTR lpszHeaders, BOOL* pbCancel);

	virtual void OnTitleChange(LPCTSTR lpszText);

	virtual void OnDocumentComplete(LPCTSTR lpszURL);

	virtual void OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel);

	virtual void OnNavigateError(LPCTSTR lpszURL, LPCTSTR lpszFrame, DWORD dwError, BOOL *pbCancel);

	virtual HRESULT OnGetExternal(LPDISPATCH *lppDispatch);
	virtual HRESULT OnGetHostInfo(DOCHOSTUIINFO *pInfo);
	virtual HRESULT OnShowContextMenu(DWORD dwID, LPPOINT ppt,
		LPUNKNOWN pcmdtReserved, LPDISPATCH pdispReserved);

protected:

	BOOL m_bContextMenuEnabled;

	BOOL m_bIsFlatStyle;

	//����ֻ��һ���¼��ص��ӿڣ���ʵ������������������������¼��ص��ӿڣ�
	//���������һ���¼��ص��ӿھ��Ѿ��㹻��

	CWebBrowserEventHandler* m_lpEventHandler;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

