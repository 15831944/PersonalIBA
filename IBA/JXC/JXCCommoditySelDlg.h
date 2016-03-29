#pragma once

#include "..\ColorGridCtrl.h"
#include "..\IBADialog.h"
#include "..\Dal\DALJXC.h"
// CJXCCommoditySelDlg �Ի���

class CJXCCommoditySelDlg : public CIBADialog
{
	DECLARE_DYNAMIC(CJXCCommoditySelDlg)
	DECLARE_MESSAGE_MAP()

public:
	CJXCCommoditySelDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CJXCCommoditySelDlg();

// �Ի�������
	enum { IDD = IDD_JXC_COMMODITYSEL };

protected:
	static const UINT INVALID_COMMODITYID = 65535;

	virtual void OnCancel();
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

private:

	static const UINT TimerIdOfSync = 1000;

	//��Ʒ��ǰѡ�еı��
	INT m_nCurSel;
	//������
	CIBAGrid m_wndGridTree;
	//��ʼ����
	void InitGridTree();
	//�����
	void FillGird();

	void QueryCommodity();
	
	//ȡ�õ�ǰѡ����Ʒ������
	BOOL GetCurSelData(INT nCurSelIndex = INVALID_COMMODITYID);

public:
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonSyncserver();
	afx_msg void OnTimer(UINT nIDEvent);

private:
	
	//��Ʒ����
	CString m_strCommodityName;
	//��Ʒ����
	CString m_strCommodityType;
	//��Ʒ����
	CString m_strCommodityCode;
	//��Ʒ���ۣ�С��λ��
	UINT m_nPrice;
	//��Ʒ�������
	UINT m_nDepositCount;
	//��ƷС��λ
	CString m_strCommodityRetailUint;
	//��Ʒ��λ
	CString m_strCommodityStoreUint;
	//��λ������
	UINT m_nUnitMutiple;

public:
	//��Ʒ����
	CCommodityArray m_CommodityArray;

	CString GetCommodityName() const { return m_strCommodityName; }

	CString GetCommodityType() const { return m_strCommodityType; }

	void SetCommodityCode(CString val) { m_strCommodityCode = val; }
	CString GetCommodityCode() const { return m_strCommodityCode; }

	UINT GetPrice() const { return m_nPrice; }

	UINT GetDepositCount() const { return m_nDepositCount; }

	CString GetCommodityRetailUint() const { return m_strCommodityRetailUint; }

	CString GetCommodityStoreUint() const { return m_strCommodityStoreUint; }

	UINT GetUnitMutiple() const { return m_nUnitMutiple; }
};
