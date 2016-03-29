
#pragma once

#include "LogListCtrl.h"
#include "operationinfo.h"
#include ".\Excel\ExportOutputListThread.h"

class CIBAOutputBar : public CBCGPDockingControlBar
{
	DECLARE_MESSAGE_MAP()
public:

	CIBAOutputBar();
	virtual ~CIBAOutputBar();


// Attributes
protected:
	//������¼�б�
	COutputListCtrl m_wndListInfo;
	//�����¼�б�
	COutputListCtrl m_wndListActive;
	//��Ϣ��¼
	COutputListCtrl m_wndMsgInfo;
	//�˿��¼
	COutputListCtrl m_wndConsumeInfo;

	// 2011/10/17-8206-gxx: ���˼�¼	
	COutputListCtrl m_listCheckOut;

	CFont m_Font;

// Generated message map functions
protected:
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg LRESULT OnOperationLog(WPARAM, LPARAM);
	
	BOOL CreateOperationList();

	BOOL CreateActiveList();

	BOOL CreateMsgList();

	BOOL CreateConsumeList();

	// 2011/10/17-8206-gxx: 
	BOOL CreateCheckOutList();

	CString FormatNetId(CString strNetId);

	//�����Ĳ�ѯ���Ѽ�¼
	void QueryConsume(COperationInfo &OperationInfo, BOOL bNeedReturnRecord = FALSE, UINT nConsumeType=0);
	
	void SetCreditInfo(COperationInfo &OperationInfo);

	void AddActiveLog();

public:
	void AddMsgLog(CClientShortMessage& strMsgInfo);

public :
	//TAB�ؼ��ĸ���
	CBCGPTabWnd	m_wndTabs;

	void AddOperationLog(COperationInfo& OperationInfo);

	BOOL AddCheckOutLog(COperationInfo& OperationInfo);
	
	void UpdateMsgLog();

	// 2011/06/16-gxx: 
	void RefreshAllInfoList();

	// 2011/06/16-gxx: 
	void RefreshInfoList(CString strFileSuffix, COutputListCtrl& listCtrl);

	// 2011/03/10/gxx: �ϰ���ô˺�������ʾ����Ա����Ĳ�����¼
	void RefreshOperationList();

	// 2011/06/16-gxx: ��ε�ˢ����Ϣ����ʾ����Ա�����ˢ����¼
	void RefreshCardList();

	void SaveToFile(CString strFileSuffix, COutputListCtrl& listCtrl);

	// 2011/06/14-gxx: �°�󣬵��ô˺�������������ص��ļ�
	void DeleteDutyFile();

	// 2011/06/16-gxx: 
	void DeleteListFile(CString strFileSuffix);

	// 2013-3-8 qsc
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);

	void OnExportExcel();

private:
	CExportOutputListThread m_ExpThread;
};

