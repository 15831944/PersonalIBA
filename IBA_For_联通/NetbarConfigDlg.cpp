// NetbarConfigDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include ".\NetbarConfigDlg.h"


// CNetbarConfigDlg �Ի���

IMPLEMENT_DYNAMIC(CNetbarConfigDlg, CIBADialog)
CNetbarConfigDlg::CNetbarConfigDlg(CWnd* pParent /*=NULL*/)
: CIBADialog(CNetbarConfigDlg::IDD, pParent)
{
}

CNetbarConfigDlg::~CNetbarConfigDlg()
{
}

void CNetbarConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CIBADialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNetbarConfigDlg, CIBADialog)
END_MESSAGE_MAP()


// CNetbarConfigDlg ��Ϣ�������

void CNetbarConfigDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	CIBADialog::OnOK();
}

BOOL CNetbarConfigDlg::OnInitDialog()
{
	CIBADialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
