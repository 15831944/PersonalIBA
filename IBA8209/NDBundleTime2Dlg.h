#pragma once

#include "Servlet\BundleTimeList2Method.h"
#include "NDDialog.h"

// CNDBundleTime2Dlg �Ի���

class CNDBundleTime2Dlg : public CNDDialog
{
	DECLARE_DYNAMIC(CNDBundleTime2Dlg)
	DECLARE_MESSAGE_MAP()

public:
	CNDBundleTime2Dlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNDBundleTime2Dlg();

// �Ի�������
	enum { IDD = IDD_ND_BUNDLETIME2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnNMClickListBundletime(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonCancelbt();	
	afx_msg void OnBnClickedOk();

	CBCGPListCtrl m_BundleTimeList;

	BOOL m_bAfterBundleTime;

private:

	CString m_strBalance;

	CString m_strBalanceW;

	CString m_strMemberID;

	CString m_strNetBar;

	CString m_strLastMemberID;

	CString m_strBundleTimeID;

	CString m_strBundleTimeStartTime;

	CString m_strBundleTimeEndTime;

	CString m_strBundleTimeType;

	CString m_strBundleTimeMoney;

	CString m_strBundleTimeMoney2;

	CString m_strBundleTimeArea;

	DWORD m_nSelBundleTimeId;

	CString m_strAllBalance;

	BOOL m_bOnLine;

	NS_SERVLET::CBundleTimeList2Method BundleTimeList2;

private :

	void InitListView();
	
	void GetBundleTimeList();
	CString BundleTimePayType(UINT nBundleType);
	void GetBundleTimeAction();
	
	void ClearData();
	void DoBundleTime(UINT nType);

	void PrintTicket();

	void OnOldCard(UINT nCardId, UINT nMemberId);
	void OnGetBalanceOK(NS_SERVLET::CBalanceMethod & Balance);

};
