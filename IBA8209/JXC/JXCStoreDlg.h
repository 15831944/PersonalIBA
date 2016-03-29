#pragma once
#include "..\IBADialog.h"
#include "JXCCommodityDlg.h"
#include "jxccommodityseldlg.h"
#include "..\Servlet\storeinmethod.h"

// CJXCStoreDlg �Ի���

class CJXCStoreDlg : public CIBADialog
{
	DECLARE_DYNAMIC(CJXCStoreDlg)
	DECLARE_MESSAGE_MAP()

public:
	CJXCStoreDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CJXCStoreDlg();

// �Ի�������
	enum { IDD = IDD_JXC_STORE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonCommodity();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedOk();
//	afx_msg void OnEnChangeEditCount();
	afx_msg void OnEnChangeEditPrice();
	afx_msg void OnBnClickedButtonSyncserver();
//	afx_msg void OnEnChangeEditCount2();
	afx_msg void OnEnKillfocusEditCount();
	afx_msg void OnEnKillfocusEditCount2();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnEnUpdateEditCount2();
	afx_msg void OnEnUpdateEditCount();
private:
	//���
	CString m_strCommodityId;
	//��Ʒ����
	CString m_strCommodityName;
	//��Ʒ����
	CString m_strCommodityType;
	//��Ʒ�����
	CString m_strCommodityRemainQuantity;
	//////////////////////////��С��λ/////////////////////////
	// ��ƷС��λ�������˫��λ�����Ǵ�λ��ƽʱ��ʾ
	CString m_strCommodityUnit;
	// ���������С��λ���������˫��λ�����Ǵ�λ��ƽʱ��ʾ
	INT m_nStoreCount;
	// ����,�����˫��λ������С��λ��ƽʱ����
	INT  m_nStoreCount2;
	//�ɹ����ۣ��������ݣ�
	INT m_nPrice;
	// �ɹ����ۣ����󣩣���ΪҪ��ʾ2λС��
	CString m_strCommodityPrice;
	// ��Ʒ�ܼ�,��ΪҪ��ʾС�������Բ����ִ�
	CString m_strCommodityAmount;
	// ��λ,�����˫��λ������С��λ��ƽʱ����
	CString m_strCommodityUnit2;
	// ���۵ĵ�λ
	CString m_strPriceUnit;
	//�����ִ�
	CString m_strUnitMutiple;
	//����
	INT m_nUnitMutiple;
	/////////////////////////////////////

	void ClearData();
	void InitListView();
	void MakeStoreInRecord(NS_SERVLET::CStoreInRecordArray &StoreInRecordArray);
	BOOL AddRecord(NS_SERVLET::CStoreInRecordArray &StoreInRecordArray, INT i);
	BOOL QueryCommodity();
	void GetDataFromDlg(CJXCCommoditySelDlg &CommodityDlg);
	BOOL CheckPopedom(LPCTSTR lpszModuleName);
	
private://�ؼ�����
	//�����Ʒ����
	//CEdit m_edtStoreCount;
	// �ɹ����ۣ����ޣ�
	CEdit m_edtPrice;
	//�����Ʒ�б�
	CBCGPListCtrl m_CommodityList;
	// ��Ʒ���
	CEdit m_edtCommodityId;
	// �������
	//CEdit m_edtStoreCount2;

private:
	// ��������Ϣ
	bool m_bAllowPretranslate;

	CString m_strResult;

	CString m_strStoreCount2;
	CString m_strStoreCount;
};
