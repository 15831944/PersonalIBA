// DlgCapture.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "DlgCapture.h"
#include ".\dlgcapture.h"


// CDlgCapture �Ի���

IMPLEMENT_DYNAMIC(CDlgCapture, CBCGPDialog)

CDlgCapture::CDlgCapture(LPCTSTR lpszImageName, CWnd* pParent /*= NULL*/)
	: CBCGPDialog(CDlgCapture::IDD, pParent)
{
	m_strImageName = lpszImageName;
	EnableVisualManagerStyle(TRUE);
}
CDlgCapture::~CDlgCapture()
{
}

void CDlgCapture::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);

}


BEGIN_MESSAGE_MAP(CDlgCapture, CBCGPDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgCapture ��Ϣ�������

BOOL CDlgCapture::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	m_capImage.SetImagesPath(CIBAConfig::GetInstance()->GetImagesPath());
	if (!m_capImage.Init(GetDlgItem(IDC_STATIC_VIDEO)->GetSafeHwnd()))
	{
		EndDialog(IDCANCEL);
	}

	return TRUE;  
}

void CDlgCapture::OnBnClickedOk()
{
	if (m_capImage.CaptureImage(m_strImageName))
	{
		CBCGPDialog::OnOK();
	}
	else
	{
		theApp.IBAMsgBox(_T("����ʧ��!"));
	}
}
