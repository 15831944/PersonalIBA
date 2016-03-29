// DlgInputMobile.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "DlgInputMobile.h"
#include ".\dlginputmobile.h"


// CDlgInputMobile �Ի���

IMPLEMENT_DYNAMIC(CDlgInputMobile, CIBADialog)
CDlgInputMobile::CDlgInputMobile(CWnd* pParent /*=NULL*/)
	: CIBADialog(CDlgInputMobile::IDD, pParent)
{
}

CDlgInputMobile::~CDlgInputMobile()
{
}

void CDlgInputMobile::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	CIBADialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgInputMobile, CIBADialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgInputMobile ��Ϣ�������

void CDlgInputMobile::OnBnClickedOk()
{
	// �ж��Ƿ�Ϊ��Ч���ֻ�����
	CString strNumbers;
	m_edit.GetWindowText(strNumbers);
	if (strNumbers.IsEmpty())
	{
		MessageBox(_T("�������ֻ�����!"),_T("��ʾ"),MB_OK|MB_ICONWARNING);
		m_edit.SetFocus();
		return;
	}
	if (strNumbers.GetLength() != 11)
	{
		MessageBox(_T("��������Ч���ֻ�����!"),_T("��ʾ"),MB_OK|MB_ICONWARNING);
		m_edit.SetSel(0, strNumbers.GetLength());
		m_edit.SetFocus();
		return;
	}
	if (strNumbers.GetAt(0) != _T('1'))
	{
		MessageBox(_T("��������Ч���ֻ�����!"),_T("��ʾ"),MB_OK|MB_ICONWARNING);
		m_edit.SetSel(0, strNumbers.GetLength());
		m_edit.SetFocus();
		return;
	}
	m_strPhoneNumber = strNumbers;
	CDialog::OnOK();
}

BOOL CDlgInputMobile::OnInitDialog()
{
	CIBADialog::OnInitDialog();

	m_edit.SetLimitText(11);

	m_edit.SetFocus();

	return FALSE;  
}
