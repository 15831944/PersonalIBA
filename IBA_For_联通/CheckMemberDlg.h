#pragma once

#include "Servlet\BalanceMethod.h"
#include "IBADialog.h"
#include "NetBarUser.h"

//
//class:CCheckMemberDlg ����Ա�ĶԻ���
//
class CCheckMemberDlg : public CIBADialog
{
	DECLARE_DYNAMIC(CCheckMemberDlg)
	DECLARE_MESSAGE_MAP()

public:
	CCheckMemberDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCheckMemberDlg();

// �Ի�������
	enum { IDD = IDD_RN_CHECKMEMBER };

protected:

	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnBnClickedOk();
	virtual void OnCancel();

	void DoModify();

public:
	
	void SetRealName(CNetBarUser& NetBarUser);
	void SetServlet(NS_SERVLET::CBalanceMethod& BalanceMethod);
	void SetCardID(CString strCardID);

private:
	//�������¼���ƣ����û�������
	CString m_strMemberName;
	//�����￪������ID
	UINT m_nNetbarID;
	//���ĵ�����
	CString m_strPwd;

	// ʵ���������˺�
	CString m_strRNCardID;
	// ���ı���������˺�
	CString m_strSLCardID;
	// ʵ��������
	CString m_strRNName;
	// ��������
	CString m_strSLName;
	// ʵ����֤������
	CString m_strRNCertificateType;
	UINT m_nRNCertificateType;
	// ����֤������
	CString m_strSLCertificateType;
	UINT m_nSLCertificateType;
	// ʵ����֤������
	CString m_strRNCertificateID;
	// ����֤������
	CString m_strSLCertificateID;
 
	// ��ʾ�ı�
	CStatic m_STTip;
};
