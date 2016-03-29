#pragma once

#include "IBADialog.h"
#include "ColorGridCtrl.h"

// CStoreDlg �Ի���

class CStoreDlg : public CIBADialog
{
	DECLARE_DYNAMIC(CStoreDlg)
	DECLARE_MESSAGE_MAP()

public:
	CStoreDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStoreDlg();

// �Ի�������
	enum { IDD = IDD_DUTY_STORECHECK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	CBCGPGridCtrl*  m_lpCommodityGrid;

	CIBAGrid m_CommodityGrid;

	virtual BOOL OnInitDialog();

private:
	//��䰴����ͼ۸��������
	void FillSortPriceGird();
	//��䰴�۸��������
	void FillPriceGird();
	// ��ʼ���ؼ�
	BOOL InitCtrl(void);

	BOOL InitSortGrid(void);

	BOOL InitPriceGrid(void);

	BOOL InitSortPriceGrid(void);
	// ��ʼ��GRID
	BOOL SetGrid(UINT nType = 0);
	//��䰴�����������
	void FillSortGird();
	// ���������
	CBCGPComboBox m_cboSortType;

public:
	afx_msg void OnCbnSelchangeComboSort();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonPrintcommodity();
};
