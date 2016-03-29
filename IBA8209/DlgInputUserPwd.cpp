// DlgInputUserPwd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "DlgInputUserPwd.h"


// CDlgInputUserPwd �Ի���

IMPLEMENT_DYNAMIC(CDlgInputUserPwd, CIBADialog)
CDlgInputUserPwd::CDlgInputUserPwd(CWnd* pParent /*=NULL*/)
	: CIBADialog(CDlgInputUserPwd::IDD, pParent)
{
}

CDlgInputUserPwd::~CDlgInputUserPwd()
{
}

void CDlgInputUserPwd::DoDataExchange(CDataExchange* pDX)
{
	CIBADialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PWD, m_strPwd);
}


BEGIN_MESSAGE_MAP(CDlgInputUserPwd, CIBADialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgInputUserPwd ��Ϣ�������

void CDlgInputUserPwd::OnBnClickedOk()
{
	UpdateData();
	CDialog::OnOK();
}
