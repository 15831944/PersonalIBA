
#pragma once

#include "IBADoc.h"

class CActiveMemberView;
class CComputerListView;

class CIBAView : public CBCGPTabView
{
	DECLARE_DYNCREATE(CIBAView)
	DECLARE_MESSAGE_MAP()

protected: // create from serialization only
	CIBAView();

// Attributes
public:
	CIBADoc* GetDocument();
	virtual ~CIBAView();

// Generated message map functions
protected:

	virtual void OnInitialUpdate();
	virtual void OnActivateView (CView* pView);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnChildClose(WPARAM wp, LPARAM lp);

private:

	static  const COMPUTERLIST_VIEW = 0;   // ������ͼ��tab����
	static  const ACTIVEMEMBER_VIEW = 1;   // �û���ͼ��tab����

	HWND m_hTermConsumeView;
	HWND m_hComsumeListView;
	HWND m_hOperationView;
	HWND m_hShiftCashView;
	HWND m_hSeatView;
	HWND m_hShortMsgView;
	HWND m_hCardOpenConsumeView;
public:
	HWND m_hDodonewView;

public:

	void ShowOperationView();
	void ShowShiftCashView();
	void ShowSeatLayoutView();
	// ����δ���Ѳ�ѯ��ʾ
	void ShowCardOpenConsumeView(void);

	void ShowTermConsumeView();
	void QueryTermConsumeList(LPCTSTR lpszTermId);

	void ShowComsumeListView();
	void QueryUserComsumeList(LPCTSTR lpszNetId, UINT nMemberId = 0);

	BOOL FindComputer(UINT nKeyType, LPCTSTR lpszKeyValue);
	// ��ʾ�ͻ�����Ϣ
	void ShowShortMsgView(void);
	void QueryShortMsgList();

	// 2011/05/06-gxx: �����ţ���ձ���ͼ
	void ShowDodonewView();

	// 2011/05/23-gxx: ��ȡ�û���ͼ
	CActiveMemberView* GetActiveMemberView();

	// 2011/05/23-gxx: ��ȡ������ͼ
	CComputerListView* GetComputerListView();

	// 2011/05/23-gxx: �жϵ�ǰ��ͼ�Ƿ�Ϊ������ͼ
	BOOL IsCurrentComputerView();

	//{ 2011/05/23-gxx: ������λ����
	BOOL GotoActiveMember(CString strNetID,BOOL bActiveView=TRUE);  // ���������˺Ŷ�λ���û�(�û���ͼ)
	// 2011/07/05-gxx:  �����ն˺Ŷ�λ���û�, ��������û���Ӧ���ն�ID
	BOOL GotoActiveMemberEx(CString strTermID, CString* pBestTernID=NULL);

	void GotoComputer(CString strTermID,BOOL bActiveView=TRUE); // ��λ������
	void GotoComputerByIP(CString strIP,BOOL bActiveView=TRUE); // IP ��λ������
	void GotoComputerByNetID(CString strNetId,BOOL bActiveView=TRUE); // �����˺� ��λ������

	// 2014-10-27 - qsc
	LRESULT OnUpdateSeat(WPARAM wParam, LPARAM lParam);
	//}
	
	
};
