#pragma once

#include "IBADialog.h"
//
//class:CDlgBossPwd û��Ȩ��ʱ�������˶Ի�������Ȩ������
//
class CDlgBossPwd : public CIBADialog
{
	DECLARE_DYNAMIC(CDlgBossPwd)
	DECLARE_MESSAGE_MAP()

public:
	CDlgBossPwd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgBossPwd();

// �Ի�������
	enum { IDD = IDD_DIALOG_BOSSPWD };

protected:
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	
	virtual void OnOK();

	CString m_strPwd;

};
//
//class:CDlgBossPwd û��Ȩ��ʱ�������˶Ի�������Ȩ������
//
class CDlgBossPwdEx : public CIBADialog
{
	DECLARE_DYNAMIC(CDlgBossPwdEx)
	DECLARE_MESSAGE_MAP()

public:
	CDlgBossPwdEx(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgBossPwdEx();

	// �Ի�������
	enum { IDD = IDD_DIALOG_BOSSPWD_EX };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	virtual void OnOK();

	CString m_strPwd;

public:
	CString m_strCaption;
	CString m_strRemark;
};