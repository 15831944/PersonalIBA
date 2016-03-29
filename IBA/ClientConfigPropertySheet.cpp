// ClientConfigPropertySheet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include ".\clientconfigpropertysheet.h"


// CClientConfigPropertySheet

IMPLEMENT_DYNAMIC(CClientConfigPropertySheet, CIBASheet)
CClientConfigPropertySheet::CClientConfigPropertySheet(CWnd* pParentWnd)
: CIBASheet(IDS_CLIENTSET, pParentWnd)
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	SetLook(CIBASheet::PropSheetLook_List);

	AddPage(&m_CommonConfig);
	AddPage(&m_BundleTimePage);
	AddPage(&m_UnlockPage);

	EnableVisualManagerStyle();
}


CClientConfigPropertySheet::~CClientConfigPropertySheet()
{
}


BEGIN_MESSAGE_MAP(CClientConfigPropertySheet, CIBASheet)
END_MESSAGE_MAP()


// CClientConfigPropertySheet ��Ϣ�������

BOOL CClientConfigPropertySheet::OnInitDialog()
{
	BOOL bResult = CIBASheet::OnInitDialog();

	// TODO:  �ڴ��������ר�ô���

	return bResult;
}
