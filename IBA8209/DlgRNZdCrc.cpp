// DlgRNZdCrc.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "DlgRNZdCrc.h"
#include ".\dlgrnzdcrc.h"


// CDlgRNZdCrc �Ի���

IMPLEMENT_DYNAMIC(CDlgRNZdCrc, CIBADialog)
CDlgRNZdCrc::CDlgRNZdCrc(CWnd* pParent /*=NULL*/)
	: CIBADialog(CDlgRNZdCrc::IDD, pParent)
	, m_dwCRC(0)
{
}

CDlgRNZdCrc::~CDlgRNZdCrc()
{
}

void CDlgRNZdCrc::DoDataExchange(CDataExchange* pDX)
{
	CIBADialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgRNZdCrc, CIBADialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgRNZdCrc ��Ϣ�������

void CDlgRNZdCrc::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// 2014-8-27 - qsc
	m_dwCRC = GetDlgItemInt(IDC_EDIT_CRC);
	CBCGPDialog::OnOK();
}
