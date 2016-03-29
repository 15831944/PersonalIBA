// AddTerminalDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "AddTerminalDlg.h"
#include ".\addterminaldlg.h"
#include "ComputerInfo.h"
#include "Socket/LocalServer.h"


// CDlgAddTerm �Ի���

IMPLEMENT_DYNAMIC(CDlgAddTerm, CIBADialog)
CDlgAddTerm::CDlgAddTerm(CWnd* pParent /*=NULL*/)
: CIBADialog(CDlgAddTerm::IDD, pParent)
,m_SeatList(NULL)
{
}

CDlgAddTerm::~CDlgAddTerm()
{
}

void CDlgAddTerm::DoDataExchange(CDataExchange* pDX)
{
	CIBADialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TREMINALIP, m_cboTerminalIP);
	DDX_Control(pDX, IDC_COMBO_TERMINALID, m_cboTerminalID);
	DDX_CBString(pDX, IDC_COMBO_TERMINALID, m_strTerminalID);
	DDX_CBString(pDX, IDC_COMBO_TREMINALIP, m_strTerminalIP);
}


BEGIN_MESSAGE_MAP(CDlgAddTerm, CIBADialog)
	ON_CBN_SELCHANGE(IDC_COMBO_TERMINALID, OnCbnSelchangeComboTerminalid)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CDlgAddTerm ��Ϣ�������

BOOL CDlgAddTerm::OnInitDialog()
{
	CIBADialog::OnInitDialog();

	LoadComputeInfo();

	m_cboTerminalID.SetCurSel(0);
	OnCbnSelchangeComboTerminalid();
	m_cboTerminalID.SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

// ͨ�������������������е��ն���Ϣ
void CDlgAddTerm::LoadComputeInfo(void)
{
	for (INT_PTR ii = 0; ii < CLocalServer::GetInstance()->ComputerList.GetCount(); ii++)
	{
		CComputerInfo info = CLocalServer::GetInstance()->ComputerList.GetComputer(ii);

		CString str  = info.GetTerminalID();
		if (str.IsEmpty())
		{
			continue;
		}

		INT_PTR jj = 0;
		for (jj = 0; jj < m_SeatList->GetSize(); jj++)
		{
			CSeat * tSeat = m_SeatList->GetAt(jj);
			if (str == tSeat->GetTerminalID())
			{
				break;
			}
		}

		if (jj >= m_SeatList->GetSize())
		{
			m_cboTerminalID.AddString(str);

			str = info.GetComputerIP();
			if (!str.IsEmpty())
			{
				m_cboTerminalIP.AddString(str);
			}
		}
	}

}

void CDlgAddTerm::OnCbnSelchangeComboTerminalid()
{
	UpdateData(TRUE);

	CString strTerminalID;
	int CurSelIndex = m_cboTerminalID.GetCurSel();
	m_cboTerminalID.GetLBText(CurSelIndex, strTerminalID);
	INT_PTR index = CLocalServer::GetInstance()->ComputerList.LookFor(0, strTerminalID);

	if (index >= 0)
	{
		CComputerInfo info = CLocalServer::GetInstance()->ComputerList.GetComputer(index);

		CString str = info.GetComputerIP();

		if (!str.IsEmpty())
		{
			m_cboTerminalIP.SelectString(-1, str);
		}
	}

}

void CDlgAddTerm::OnBnClickedOk()
{
	CBCGPDialog::OnOK();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CDlgAddTerm::OnOK()
{
	CBCGPDialog::OnOK();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CDlgAddTerm::OnBnClickedCancel()
{
	CBCGPDialog::OnCancel();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CDlgAddTerm::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CBCGPDialog::OnClose();
}
