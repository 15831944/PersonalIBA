// DlgCheckPwd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgCheckPwd.h"
#include ".\dlgcheckpwd.h"


// CDlgCheckPwd �Ի���

IMPLEMENT_DYNAMIC(CDlgCheckPwd, CG2XDialog)
CDlgCheckPwd::CDlgCheckPwd(CWnd* pParent /*=NULL*/)
	: CG2XDialog(CDlgCheckPwd::IDD, _T("����������"), pParent)
{
}

CDlgCheckPwd::~CDlgCheckPwd()
{
}

void CDlgCheckPwd::DoDataExchange(CDataExchange* pDX)
{
	CG2XDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgCheckPwd, CG2XDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_GVWN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgCheckPwd ��Ϣ�������

void CDlgCheckPwd::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strPwd;
	GetDlgItemText(IDC_EDIT_PWD, strPwd);
	if(_T("delldodonew") == strPwd)
	{
		OnOK();
	}
	else
	{
		AfxMessageBox(_T("�������!"));
	}
}

BOOL CDlgCheckPwd::OnInitDialog()
{
	CG2XDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	GetCaptionBar()->GetSysBtnBar()->EraseID(SC_MAXIMIZE);
	GetCaptionBar()->GetSysBtnBar()->EraseID(SC_MINIMIZE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
