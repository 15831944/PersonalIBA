#pragma once

#include "IBAFormView.h"

// CComsumeListView ������ͼ

class CComsumeListView : public CIBAFormView
{
	DECLARE_DYNCREATE(CComsumeListView)
	DECLARE_MESSAGE_MAP()

protected:
	CComsumeListView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CComsumeListView();

public:
	enum { IDD = IDV_CONSUMELIST };
	virtual void OnInitialUpdate();

protected:
	afx_msg void OnBnClickedButtonConsumelist();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnNMClickListConsumelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListConsumelist(NMHDR *pNMHDR, LRESULT *pResult);

private:

	CBCGPListCtrl m_ConsumeList;
	CBCGPComboBox m_cboConsumeType;
	CBCGPDateTimeCtrl m_StartTime;
	CBCGPDateTimeCtrl m_EndTime;

	CString m_strName;
	CString m_strPersonalID;
	CString m_strNetId;
	UINT m_nMemberId;
	
	CString m_strResult;

	INT m_nCreditMoney;
	INT m_nReturnMoney;
	INT m_nConsumeMoney;

	UINT GetConsumeType();
	void QueryConsumeList(UINT nMemberId, CString strNetId);
	void InitListView();
	void CheckCard(LPCTSTR strNetId, UINT nMemberId = 0);
	void InitData();

public:
	//ֻ���Ѿ��˿���û���ָ����MemberId����Ч������������������˺�������
	void QueryUserConsumeList(LPCTSTR lpszNetId, UINT nMemberId = 0);

};


