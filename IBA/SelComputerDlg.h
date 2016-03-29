#pragma once

#include "IBADialog.h"
#include "afxcmn.h"
#include <wtypes.h>
// CSelComputerDlg �Ի���

class CSelComputerDlg : public CIBADialog
{
	DECLARE_DYNAMIC(CSelComputerDlg)
	DECLARE_MESSAGE_MAP()

public:
	CSelComputerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSelComputerDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SELTERMID };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnBnClickedButtonQuery();
	afx_msg void OnNMDblclkListInfo(NMHDR *pNMHDR, LRESULT *pResult);

private:
	// ��ʼ���ؼ�
	BOOL InitCtrl(void);
	// �������
	BOOL FillList(void);
	//ѡ������
	void SelComputer();
	//�ж�ռ��״̬
	BOOL CheckTakeUp(CString strTermId);
	
private:
	// �ն˺�
	CString m_strTermId;
	//�û���
	UINT m_nMemberId;
	//���ص��ն�
	CString m_strSelTermId;
	// �ն��б���Ϣ
	CBCGPListCtrl m_ListInfo;

public:
	UINT GetMemberId() const { return m_nMemberId; }
	void SetMemberId(UINT val) { m_nMemberId = val; }

	CString GetSelTermId() const { return m_strSelTermId; }
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
