// CrashShowDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "CrashShowDlg.h"


// CCrashShowDlg �Ի���

IMPLEMENT_DYNAMIC(CCrashShowDlg, CDialog)
CCrashShowDlg::CCrashShowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCrashShowDlg::IDD, pParent)
{
}

CCrashShowDlg::~CCrashShowDlg()
{
}

void CCrashShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCrashShowDlg, CDialog)
END_MESSAGE_MAP()


// CCrashShowDlg ��Ϣ�������
BOOL CCrashShowDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString strMsg;
	strMsg = _T("��⣡���������...\n\n�ǳ���Ǹ����������������źͲ��㣬���ǵĿ�����Ա�������ռ���Ϣ���ӽ��޸�...");
	GetDlgItem(IDC_CRASH_MESSAGE)->SetWindowText(strMsg);

	return TRUE;
}
