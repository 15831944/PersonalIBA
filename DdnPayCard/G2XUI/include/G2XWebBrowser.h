#pragma once
#include <afxhtml.h>

//�¼��ص��ӿڣ������ǽӿ�Ҳ��������

//����ǽӿھ�Ҫ�ڵ�����������ʵ�����е��¼�
//��ʱ����ֻ��Ҫʵ��һ�����¼�����������������ʵ���¼��ص��ӿ�

//�����е��¼��������������

class G2X_CLASSES CG2XWebBrowserEventHandler
{
protected:

	friend class CG2XWebBrowser;	 //���¼�Դ���Ե����¼��ص�����
	
	CG2XWebBrowserEventHandler(){}; //�������캯����ֻ�ܼ̳У�����ʵ����

	//������¼��ص�����
	
	//��һ������Ϊ�¼�Դ��ֻҪ��Ϊ�����־�����¼�Դ

	virtual void WebBrowser_BeforeNavigate2(CG2XWebBrowser& Sender, LPCTSTR lpszURL, DWORD nFlags,
		LPCTSTR lpszTargetFrameName, CByteArray& baPostedData,
		LPCTSTR lpszHeaders, BOOL* pbCancel){}

	virtual void WebBrowser_TitleChange(CG2XWebBrowser& Sender, LPCTSTR lpszText){}

	virtual void WebBrowser_DocumentComplete(CG2XWebBrowser& Sender, LPCTSTR lpszURL){}

	virtual void WebBrowser_NewWindow2(CG2XWebBrowser& Sender, CG2XWebBrowser* ppDisp, BOOL* Cancel){}

	virtual void WebBrowser_NavigateError(CG2XWebBrowser& Sender, LPCTSTR lpszURL, LPCTSTR lpszFrame, DWORD dwError, BOOL *pbCancel){}

	virtual HRESULT WebBrowser_GetExternal(CG2XWebBrowser& Sender, LPDISPATCH *lppDispatch){return S_FALSE;}
};


class G2X_CLASSES CG2XWebBrowser: public CHtmlView
{
	DECLARE_DYNCREATE(CG2XWebBrowser)
	DECLARE_MESSAGE_MAP()

public:

	CG2XWebBrowser();          
	virtual ~CG2XWebBrowser();

	//���û���ȡ���¼��ص��ӿ�
	void SetEventHandler(CG2XWebBrowserEventHandler* lpEventHandler = NULL);

	BOOL IsContextMenuEnabled() const { return m_bContextMenuEnabled; }
	void EnableContextMenu(BOOL newVal = TRUE) { m_bContextMenuEnabled = newVal; }

	BOOL IsFlatStyle() const { return m_bIsFlatStyle; }
	void SetFlatStyle(BOOL newVal = TRUE) { m_bIsFlatStyle = newVal; }

	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
		DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
		UINT nID, CCreateContext* pContext);
protected:

	virtual void PostNcDestroy();

	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	
	BOOL PreCreateWindow(CREATESTRUCT & cs);

	

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
		LPUNKNOWN pcmdtReserved, CG2XWebBrowser pdispReserved);

protected:

	BOOL m_bContextMenuEnabled;

	BOOL m_bIsFlatStyle;

	//����ֻ��һ���¼��ص��ӿڣ���ʵ������������������������¼��ص��ӿڣ�
	//���������һ���¼��ص��ӿھ��Ѿ��㹻��

	CG2XWebBrowserEventHandler* m_lpEventHandler;

};

