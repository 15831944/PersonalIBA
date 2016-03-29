#pragma once

#include "IBADialog.h"
#include "NetBarConfig2.h"
#include "afxwin.h"

//
//class: CDlgNetbarConfig �ŵ꽨�����öԻ���
//
class CDlgNetbarConfig : public CIBADialog
{
	DECLARE_DYNAMIC(CDlgNetbarConfig)
	DECLARE_MESSAGE_MAP()

public:
	CDlgNetbarConfig();
	virtual ~CDlgNetbarConfig();

// �Ի�������
	enum { IDD = IDD_NETBARINFO };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSync();//����ʱ��ͬ��
	afx_msg void OnBnClickedOk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnHotKey(WPARAM wParam, LPARAM lParam);//ϵͳ�ȼ�
	afx_msg void OnBnClickedButtonAllcheckout();

private:

	CString m_strCenterIP;//����IP
	UINT m_nCenterPort;//���Ķ˿�
	CString m_strPath;//δ��
	UINT m_nNetBarID;//����id

	CString m_strNetBarKey;//����ʶ����
	CString m_strNetBarName;//��������
	CString m_strPrincipal;//������
	CString m_strAddr;//���ɵ�ַ
	CString m_strTel;//������ϵ�绰
	CString m_strCashRegisterId;//����̨id
	CString m_strBossPwd;//�ϰ�����
	CString m_IServerIP;//������IP
	CString m_strMAC;//������ַ

	CNetBarConfig2 m_NetBarConfig2;//�����õ�������Ϣ

	INT m_nLocalModeStartKind; // 2011/08/10-8201-gxx: Ӧ��ģʽ�����÷�ʽ

private:

	void InitData();
	void SaveData();

//public:
//	afx_msg void OnBnClickedRadio1();
//	afx_msg void OnBnClickedRadio2();
//	afx_msg void OnBnClickedRadio3();
//	void DoChangeLocalStartKind(INT nNewKind); // 2011/08/10-8201-gxx: 
};
