#pragma once

#include ".\IBADialog.h"
#include ".\MyListCtrl\ListCtrlCl.h"
#include ".\common\GxxListCtrl.h"
// CDlgChoseForwardBundle �Ի���

class CDlgChoseForwardBundle : public CIBADialog
{
	DECLARE_DYNAMIC(CDlgChoseForwardBundle)

public:
	CDlgChoseForwardBundle(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgChoseForwardBundle();

	// �Ի�������
	enum { IDD = IDD_DLG_BUNDTIME_CHOSEFORWARD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CListCtrlCl m_ListPCClass;
	//NS_COMMON::CGxxListCtrl m_ListPCClass;
	//CListCtrl m_ListPCClass;
	double m_dAllBalance;
	int m_nUserClassID;
	int m_nSelBundleTimeID;
	int m_nForwardBundleMoney;
	BOOL m_bIsMember;

	virtual BOOL OnInitDialog();
	void InsertPCClassItem();
	void ListPCClassInit();

public:
	void SetForwarBundleInfo(double dAllBalance, int nUserClassID , int nSelBundleTimeId, BOOL bIsMember);
	int GetForwardBundleMoney();
	afx_msg void OnBnClickedOk();
};
