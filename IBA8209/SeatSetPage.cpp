// SEATSETDLG.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "SeatSetPage.h"
#include ".\seatsetpage.h"


// CSetPageOfSeat �Ի���

IMPLEMENT_DYNAMIC(CSetPageOfSeat, CIBAPage)
CSetPageOfSeat::CSetPageOfSeat()
: CIBAPage(CSetPageOfSeat::IDD)
, m_strBitmapPath(_T(""))
{
}

CSetPageOfSeat::~CSetPageOfSeat()
{
}

void CSetPageOfSeat::DoDataExchange(CDataExchange* pDX)
{
	CIBAPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BIYMAPPATH, m_strBitmapPath);
}


BEGIN_MESSAGE_MAP(CSetPageOfSeat, CIBAPage)
	ON_BN_CLICKED(IDC_BUTTON_BROWSER, OnBnClickedButtonBrowser)
END_MESSAGE_MAP()


// CSetPageOfSeat ��Ϣ�������

void CSetPageOfSeat::OnBnClickedButtonBrowser()
{
	CFileDialog dlg(TRUE, _T("bmp"), NULL, 0, _T("BMP Files (*.bmp)|*.bmp|JPG Files(*.jpg)|*.jpg|PNG Files(*.png)|*.png|"));
	if (dlg.DoModal() == IDOK)
	{
		SetDlgItemText(IDC_EDIT_BIYMAPPATH, dlg.GetPathName());
		CIBAConfig::GetInstance()->SetSeatBmpPath(dlg.GetPathName());
	}
}

BOOL CSetPageOfSeat::OnInitDialog()
{
	CIBAConfig::GetInstance()->LoadConfig();
	m_strBitmapPath = CIBAConfig::GetInstance()->GetSeatBmpPath();

	CIBAPage::OnInitDialog();//�Ѿ����������ݵ�����ı���

	GetDlgItem(IDC_EDIT_BIYMAPPATH)->SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CSetPageOfSeat::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	UpdateData(TRUE);
	CIBAConfig::GetInstance()->SetSeatBmpPath(m_strBitmapPath);
	CIBAConfig::GetInstance()->SaveConfig();

	CIBAPage::OnOK();
}
