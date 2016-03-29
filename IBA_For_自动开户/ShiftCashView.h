#pragma once


#include "IBAFormView.h"
// CShiftCashView ������ͼ

class CShiftCashView : public CIBAFormView
{
	DECLARE_DYNCREATE(CShiftCashView)
	DECLARE_MESSAGE_MAP()

protected:
	CShiftCashView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CShiftCashView();

public:
	enum { IDD = IDV_SHIFTCASH };
	virtual void OnInitialUpdate();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonQuery();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	void InitListView();
	void InitData();

	void QueryLocalDB();
	void QueryCenter();

	BOOL m_bUseDeposit;

	CBCGPListCtrl m_List;

	CString m_strOperator;
	CString m_strResult;

	INT m_nCreditMoney;
	INT m_nReturnedMoney;
	INT m_nPayout;
	INT m_nCashSoldAmount;
	INT m_nTotalAmount;
	INT m_nDiankaSold; // 2011/08/03-8201-gxx: �㿨�����ܺ�
};


