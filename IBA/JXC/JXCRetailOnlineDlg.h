
#pragma once

#include "..\Servlet\RetailMethod.h"
#include "..\Servlet\BalanceMethod.h"
#include "..\NDDialog.h"
#include "JXCCommodityDlg.h"

// CJXCRetailOnlineDlg �Ի���
class CJXCRetailOnlineDlg : public CNDDialog
{
	DECLARE_DYNAMIC(CJXCRetailOnlineDlg)
	DECLARE_MESSAGE_MAP()

public:
	CJXCRetailOnlineDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CJXCRetailOnlineDlg();

// �Ի�������
	enum { IDD = IDD_JXC_RETAILONLINE };

	void AddSellRecord(NS_SERVLET::CSellRecord& SellRecord);

	void InitUserData(CString strTermId, CString strNetId);

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonCommodity();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedButtonQuery();
	afx_msg void OnTimer(UINT nIDEvent);

	void OnOldCard(UINT nCardId, UINT nMemberId);
	void OnGetBalanceOK(NS_SERVLET::CBalanceMethod & Balance);
	void ClearData();

private:
	// ��������Ϣ
	bool m_bAllowPretranslate;
	/////////////��Ʒ��Ϣ////////////
	CString m_strCommodityId;
	CString m_strCommodityName;
	CString m_strCommodityType;
	CString m_strCommodityPrice;
	CString m_strCommodityRemainQuantity;
	UINT m_nCommodityCount;

	CString m_strTermId;
	
	//�����˽��
	CString m_strAllMoney;
	//��ѯ�м����

	UINT m_bOnline;//�Ƿ����ߵı�־
	//////////////�ؼ�����////////////
	NS_SERVLET::CSellRecordArray m_SellRecordArray;

	CBCGPMaskEdit m_edtCount;

	CBCGPListCtrl m_CommodityList;

	CJXCCommodityDlg m_CommodityDlg;

	void InitListView();
	void ShowSellRecordInfo();
	void SaveRetailData(NS_SERVLET::CRetailOnlineMethod &RetailOnline);
	BOOL QueryCommodity();
	void ClearData2();

	void PrintTicket(CString strAccount);
};
