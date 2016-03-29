#pragma once

#include "IBA.h"
#include "NDDialog.h"
#include "Servlet\BundleTimeListMethod.h"
#include "BCGPLabalButton.h"
#include "BCGPLinkStatic.h"

//2015-0825 liyajun ��ʵ��������б�ؼ�
class CSortListCtrl : public CListCtrl
{
public:
	CSortListCtrl():m_bAsc(TRUE),m_nSortedCol(0){};
	static int CALLBACK ListCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

public:
	BOOL m_bAsc;
	int m_nSortedCol;
};

//
//class:CDlgBundleTime ��ʱ�Ի���
//
class CDlgBundleTime : public CNDDialog
{
	DECLARE_DYNAMIC(CDlgBundleTime)
	DECLARE_MESSAGE_MAP()
public:
	CDlgBundleTime(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgBundleTime();

	static const TimeOfOnlineBundTime = 1003;//���߰�ʱ�ĵȴ�ʱ��
// �Ի�������
	enum { IDD = IDD_ND_BUNDLETIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMClickListBundletime(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonCancelbt();	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonContinuebt();
	afx_msg void OnBnClickedButtonForwardbt();
	afx_msg void OnBnClickedButtonChangebt();
	afx_msg void OnBnClickedSelfbundtime();
	afx_msg void OnBnClickedButtonReprint();
	afx_msg void OnBnClickedCheckPrint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStnClickedStaticAll();
	afx_msg void OnBnClickedButtonCredit();
	afx_msg void OnBnClickedButtonBalance();
	afx_msg void OnColumnListClicked(NMHDR* pNMhdr, LRESULT* pResult);  //2015-0825 liyajun����

	//2015-0825 liyajun ����ListCtrl
	CSortListCtrl m_BundleTimeList;

	CBCGPButton m_btnPrint;
	
	BOOL m_bAfterBundleTime; //ʶ���Ƿ��ʱ�ɹ��ı�־

	CBCGPLinkStatic m_btnAllBalance;

	CBCGPLabelButton m_btnCredit;
	
	CBCGPLabelButton m_btnBalance;

private:

	UINT m_nLastMemberID;

	INT m_nCurrBundleTimeIndex;

	CString m_strBundleTimeID;

	CString m_strBundleTimeStartTime;

	CString m_strBundleTimeEndTime;

	CString m_strBundleTimeType;

	CString m_strBundleTimeMoney;

	CString m_strBundleTimeMoney2;

	CString m_strBundleTimeArea;

	INT m_nSelBundleTimeId;

	UINT m_nSelBundleTimePrice;

	NS_SERVLET::CBundleTimeListMethod BundleTimeList;

	// 2011/07/05-gxx: �洢��ʱ���
	INT m_nBudleTimeMoney; 
	//CFont m_MoneyFont;
	CString m_strMoneyCredit;			// ��Ҫ��ֵ��Ǯ

private :

	void InitListView();
	void GetBundleTimeList();
	CString BundleTimePayType(UINT nBundleType);
	void GetBundleTimeAction();
	void ClearData();
	BOOL DoBundleTime(UINT nType);
	void PrintTicket();
	void CheckBundleTimeCondition(INT nBtIndex, BOOL& bNowCanBT, BOOL& bNowCanForwardBT, BOOL& bNowChangeBT, BOOL& bNowCanContinueBT);

	void OnGetBalanceOK(NS_SERVLET::CBalanceMethod & Balance);

	//Ȩ���ж�
	BOOL CheckPopedom(LPCTSTR lpszModuleName);
	BOOL DoCredit();
	BOOL CheckInput();
	BOOL DoLocalModeCredit(UINT nCreditMoney);

public:


	afx_msg void OnBnClickedButtonQuery();
};
