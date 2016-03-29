// DlgChoseForwardBundle.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "DlgChoseForwardBundle.h"
#include ".\dlgchoseforwardbundle.h"
#include "HelpFunc.h"

// CDlgChoseForwardBundle �Ի���

IMPLEMENT_DYNAMIC(CDlgChoseForwardBundle, CIBADialog)
CDlgChoseForwardBundle::CDlgChoseForwardBundle(CWnd* pParent /*=NULL*/)
: CIBADialog(CDlgChoseForwardBundle::IDD, pParent),
	m_dAllBalance(0.0), m_nUserClassID(0), m_nSelBundleTimeID(0),
	m_nForwardBundleMoney(0), m_bIsMember(FALSE)
{

}

CDlgChoseForwardBundle::~CDlgChoseForwardBundle()
{
}

void CDlgChoseForwardBundle::DoDataExchange(CDataExchange* pDX)
{
	CIBADialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_PCCLASS, m_ListPCClass);
}


BEGIN_MESSAGE_MAP(CDlgChoseForwardBundle, CIBADialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgChoseForwardBundle ��Ϣ�������

BOOL CDlgChoseForwardBundle::OnInitDialog()
{
	CIBADialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ListPCClassInit();
	InsertPCClassItem();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgChoseForwardBundle::ListPCClassInit()
{
	m_ListPCClass.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ListPCClass.InsertColumn(0, _T("���"), LVCFMT_LEFT, 70);
	m_ListPCClass.InsertColumn(1, _T("��������"), LVCFMT_LEFT, 100);

	CRect rect;
	m_ListPCClass.GetClientRect(&rect);
	m_ListPCClass.InsertColumn(2, _T("Ӧ���ֽ�"), LVCFMT_LEFT, rect.Width() - 170);

}

void CDlgChoseForwardBundle::InsertPCClassItem()
{
	// ɸѡWK������ʹ�õ���������
	// ��BundleTimeInfo�е���������ĳ������Ϣ����˵��������ĳ����ʹ��
	// ���������κ�������Ϣ����˵����������ʹ��
	CArray<CIDNameRecord,CIDNameRecord&> AvaiablePCClassInfoArray;
	CFullBundleTimeInfo BundleTimeInfo;
	if(!CNetBarConfig::GetInstance()->m_NetbarBundleTime.GetBundleTimeInfo(m_nUserClassID, m_nSelBundleTimeID,BundleTimeInfo))
	{
		return;
	}
	bool bFindOneofPCClassName(FALSE);
	for (INT i = 0; i < CNetBarConfig::GetInstance()->m_PCClassInfoArray.GetCount(); i++)
	{
		CIDNameRecord IDNameRecord = CNetBarConfig::GetInstance()->m_PCClassInfoArray.GetAt(i);
		if( BundleTimeInfo.GetDescription().Find(IDNameRecord.GetName(),0) >= 0)
		{
			AvaiablePCClassInfoArray.Add(IDNameRecord);
			bFindOneofPCClassName = TRUE;
		}
	}
	if(!bFindOneofPCClassName)
	{
		AvaiablePCClassInfoArray.Copy(CNetBarConfig::GetInstance()->m_PCClassInfoArray);
	}

	// ���ñ�ź���������
	for (INT i = 0; i < AvaiablePCClassInfoArray.GetCount(); i++)
	{
		const DWORD dwPCClassID = AvaiablePCClassInfoArray.GetAt(i).GetID();
		CString strTmp;
		strTmp.Format(_T("%d"), dwPCClassID);
		m_ListPCClass.InsertItem(i, strTmp);
		m_ListPCClass.SetItemText(i, 1, AvaiablePCClassInfoArray.GetAt(i).GetName());
	}
	
	BOOL bBundleTimeWithCash = CNetBarConfig::GetInstance()->GetBundleTimeWithCash(m_bIsMember);
	int nMoney = 0;
	DWORD dwPCClassID  = 0;
	CString strTmp("");

	// WK������"��ֹ�˻���ʱ"
	if (bBundleTimeWithCash)
	{
		for (INT i = 0; i < AvaiablePCClassInfoArray.GetCount(); i++)
		{
			dwPCClassID = AvaiablePCClassInfoArray.GetAt(i).GetID();
			nMoney = HELPFUNC::GetCashForwardBundleNoBalance(m_dAllBalance, m_nUserClassID, m_nSelBundleTimeID, dwPCClassID);
			strTmp.Format(_T("%d.00"), nMoney / 100);
			
			m_ListPCClass.SetItemText(i, 2, strTmp);
			m_ListPCClass.SetItemData(i, nMoney);
		}
	}
	else
	{
		for (INT i = 0; i < AvaiablePCClassInfoArray.GetCount(); i++)
		{
			dwPCClassID = AvaiablePCClassInfoArray.GetAt(i).GetID();
			nMoney = HELPFUNC::GetMoneyToForwardBundle(m_dAllBalance, m_nUserClassID, m_nSelBundleTimeID, dwPCClassID);
			strTmp.Format(_T("%d.00"), nMoney / 100);

			m_ListPCClass.SetItemText(i, 2, strTmp);
			m_ListPCClass.SetItemData(i, nMoney);
		}	
	}
	m_ListPCClass.SetColTextColor(2, RGB(255,0,0));
	m_ListPCClass.SetRowHeigt(20);
	m_ListPCClass.SetFontHW(15, 0);

	CFont font;
	font.CreateFont(15,0, 0, 0,FW_HEAVY,FALSE,FALSE,0,0,0,0,0,0,_TEXT("����"));

	LOGFONT lf;
	font.GetLogFont(&lf);
	m_ListPCClass.SetColFont(2, &lf);
}

void CDlgChoseForwardBundle::SetForwarBundleInfo(double dAllBalance, int nUserClassID , int nSelBundleTimeId, BOOL bIsMember)
{
	m_dAllBalance = dAllBalance;
	m_nUserClassID =nUserClassID;
	m_nSelBundleTimeID = nSelBundleTimeId;
	m_bIsMember = bIsMember;
}

int CDlgChoseForwardBundle::GetForwardBundleMoney()
{
	return m_nForwardBundleMoney;
}


void CDlgChoseForwardBundle::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	INT nIndex = m_ListPCClass.GetSelectionMark();

	if (nIndex >= 0)
	{
		m_nForwardBundleMoney = m_ListPCClass.GetItemData(nIndex);
	}
	else
	{
		m_nSelBundleTimeID = 0;
	}
	EndDialog(IDOK);
}
