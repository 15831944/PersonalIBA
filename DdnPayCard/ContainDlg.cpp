// ContainDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DdnPayCard.h"
#include "ContainDlg.h"
#include ".\containdlg.h"


// CContainDlg �Ի���

IMPLEMENT_DYNAMIC(CContainDlg, CG2XDialog)
CContainDlg::CContainDlg(CWnd* pParent /*=NULL*/)
	: CG2XDialog(CContainDlg::IDD,_T(" ��ţ�㿨ƽ̨(����̨)"), pParent)
{
}

CContainDlg::~CContainDlg()
{
}

void CContainDlg::DoDataExchange(CDataExchange* pDX)
{
	CG2XDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CContainDlg, CG2XDialog)
END_MESSAGE_MAP()


// CContainDlg ��Ϣ�������

BOOL CContainDlg::OnInitDialog()
{
	CG2XDialog::OnInitDialog();
	GetCaptionBar()->GetSysBtnBar()->EraseID(SC_MAXIMIZE);
	GetCaptionBar()->GetSysBtnBar()->EraseID(SC_MINIMIZE);

	SetWindowPos(NULL, 0, 0, 580 + 2, 430 + 25, SWP_NOMOVE);

	CenterWindow();

	CRect rcClient;
	GetClientRect(rcClient);
	rcClient.top += 25;
	m_dlg.Create(CPayVerifyDlg::IDD, this);
	m_dlg.MoveWindow(rcClient);
	m_dlg.ShowWindow(SW_SHOW);
	GetPanel()->AttachWindow(&m_dlg,FALSE);
	return TRUE;  
}
