#pragma once

#include "IBADialog.h"
#include "Servlet\BundleTimeListMethod.h"

//
//class:CDlgBundleTimeList ��ʱ��Ϣ�б�Ի���
//
class CDlgBundleTimeList : public CIBADialog
{
	DECLARE_DYNAMIC(CDlgBundleTimeList)
	DECLARE_MESSAGE_MAP()
public:
	CDlgBundleTimeList(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgBundleTimeList();

// �Ի�������
	enum { IDD = IDD_BUNDLETINE_LISTDLG };

protected:
	
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnOK();
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMDblclkListBundletime(NMHDR *pNMHDR, LRESULT *pResult);
	
	void InitListView();

	void FillBundleTimeList();

	void InsertItem(INT nArrayOrder, INT nItem);

	CBCGPListCtrl m_BundleTimeList;

public:	

	UINT GetBundleTimeId() const { return m_nBundleTimeId; }
	UINT GetBundleTimeType() const { return m_nBundleTimeType; }
	UINT GetBundleTimeClass() const { return m_nBundleTimeClassId; }
	UINT GetBundleTimeMoney() const { return m_nBundleTimeMoney; }

	CString GetBundleTimeMoneyString() const;
	CString GetStartTime() const { return m_strStartTime; }
	CString GetEndTime() const { return m_strEndTime; }
	CString GetRemainTime() const { return m_strRemainTime; }// ��ʱ��ʣ��ʱ��
	
	//��ʱ���Ե�ѡ��ʽ
	void SetSelectStyle(UINT val) { m_nSelectStyle = val; }

	void SetUserClassId(UINT newVal) { m_nUserClassId = newVal; }

private:

	CString m_strStartTime;
	CString m_strEndTime;
	CString m_strRemainTime;// ��ʱ��ʣ��ʱ��

	UINT m_nBundleTimeId;
	UINT m_nBundleTimeType;  //��ʱ��������
	UINT m_nBundleTimeClassId; //��ʱ��������
	UINT m_nBundleTimeMoney;

	//��ʱ���Ե�ѡ��ʽ��0Ϊ��ͨ��ʽ��ȫ������ 1Ϊ���߷�ʽ����ʱ������
	UINT m_nSelectStyle;

	//�û�����
	UINT m_nUserClassId;
};
