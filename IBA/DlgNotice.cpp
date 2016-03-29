// DlgNotice.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "DlgNotice.h"
#include "Socket\LocalServer.h"
#include "IBAHelpper.h"
#include ".\dlgnotice.h"


// CDlgNotice �Ի���

IMPLEMENT_DYNAMIC(CDlgNotice, CIBADialog)

CDlgNotice::CDlgNotice(CWnd* pParent /*= NULL*/)
	:CIBADialog(CDlgNotice::IDD, pParent)
{
	

	m_bMouseAt = FALSE;
}

void CDlgNotice::SetParams(LPCTSTR lpTitle,LPCTSTR lpInfo,LPCTSTR lpLink,BOOL bShouldSign, BOOL bUpdate)
{
	m_strTitle = lpTitle;
	m_strLink = lpLink;
	m_strInfo = lpInfo;
	m_bShouldSign = bShouldSign;

	if( bUpdate )
	{
		SetWindowText(m_strTitle);

		CIBAString strNames;
		strNames.LoadString(IDS_NOTICE_BTN_NAMES);
		CStringArray strArr;
		CIBAHelpper::SplitLine(strNames, strArr);

		if (m_bShouldSign)
		{
			// ��Ҫǩ�գ���ʾ ǩ��
			m_btn.SetWindowText(strArr.GetAt(0));

			CMenu* pMenu = this->GetSystemMenu(FALSE); 
			pMenu->EnableMenuItem(SC_CLOSE,MF_DISABLED ); 

		}
		else
		{
			CMenu* pMenu = this->GetSystemMenu(FALSE); 
			//pMenu->ModifyMenu(SC_CLOSE,MF_BYCOMMAND | MF_ENABLED ); 
			pMenu->EnableMenuItem(SC_CLOSE, MF_ENABLED);

			if (!m_strLink.IsEmpty())
			{
				// �����ӣ���ʾ�鿴
				m_btn.SetWindowText(strArr.GetAt(1));
			}
			else
			{
				// �����ӣ���ʾ�ر�
				m_btn.SetWindowText(strArr.GetAt(2));
			}
		}

		GetDlgItem(IDC_STATIC_INFO)->SetWindowText(m_strInfo);
	}
}

CDlgNotice::~CDlgNotice()
{
}

void CDlgNotice::DoDataExchange(CDataExchange* pDX)
{
	CIBADialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDOK, m_btn);
}


BEGIN_MESSAGE_MAP(CDlgNotice, CIBADialog)
	ON_STN_CLICKED(IDC_STATIC_INFO, OnStnClickedStaticInfo)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_SETCURSOR()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgNotice ��Ϣ�������

BOOL CDlgNotice::OnInitDialog()
{
	CIBADialog::OnInitDialog();

	SetParams(m_strTitle,m_strInfo,m_strLink,m_bShouldSign,TRUE);

	return TRUE; 
}

void CDlgNotice::OnStnClickedStaticInfo()
{
	if (!m_strLink.IsEmpty())
	{
		ShellExecute(NULL,_T("open"),m_strLink,NULL,NULL,SW_SHOW);
	}
}

void CDlgNotice::OnBnClickedOk()
{
	if (m_bShouldSign)
	{
				// ����IServer��ǩ��
		CLocalServer::GetInstance()->LocalAskNotice(TRUE);
		CBCGPDialog::OnOK();
	}
	else
	{
		if (!m_strLink.IsEmpty())
		{
			OnStnClickedStaticInfo();
		}
		else
		{
			CBCGPDialog::OnCancel();
		}
	}
}

BOOL CDlgNotice::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_INFO && !m_strLink.IsEmpty())
	{
		HCURSOR hc = 0;
		
		hc = ::LoadCursor(NULL,IDC_HAND);
		::SetCursor(hc);
		if (m_bMouseAt == FALSE)
		{
			Invalidate();
		}
		m_bMouseAt = TRUE;
		return TRUE;
	}
	else
	{
		if (m_bMouseAt == TRUE)
		{
			Invalidate();
		}
		m_bMouseAt = FALSE;
	}

	return CIBADialog::OnSetCursor(pWnd, nHitTest, message);
}

HBRUSH CDlgNotice::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CIBADialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_INFO)
	{
		if (m_bMouseAt)
		{
			pDC->SetTextColor(RGB(0,0,255));
		}
		else
		{
			pDC->SetTextColor(RGB(0,0,0));
		}
	}
	return hbr;
}

