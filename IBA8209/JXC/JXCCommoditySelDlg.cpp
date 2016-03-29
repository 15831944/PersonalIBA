// JXCCommoditySelDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "..\IBA.h"
#include ".\JXCCommoditySelDlg.h"
#include "..\Dal\IBADAL.h"
#include "..\IBAHelpper.h"
#include "..\ServletHellper.h"
// CJXCCommoditySelDlg �Ի���

IMPLEMENT_DYNAMIC(CJXCCommoditySelDlg, CIBADialog)
CJXCCommoditySelDlg::CJXCCommoditySelDlg(CWnd* pParent /*=NULL*/)
: CIBADialog(CJXCCommoditySelDlg::IDD, pParent)
, m_nCurSel(INVALID_COMMODITYID), m_nUnitMutiple(1)
, m_nDepositCount(0), m_nPrice(0)
{
}

CJXCCommoditySelDlg::~CJXCCommoditySelDlg()
{
}

void CJXCCommoditySelDlg::DoDataExchange(CDataExchange* pDX)
{
	CIBADialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CJXCCommoditySelDlg, CIBADialog)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_BUTTON_SYNCSERVER, OnBnClickedButtonSyncserver)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CJXCCommoditySelDlg ��Ϣ�������

BOOL CJXCCommoditySelDlg::OnInitDialog()
{
	CIBADialog::OnInitDialog();

	SetEditFont();

	InitGridTree();
	
	QueryCommodity();
	
	CIBALog::GetInstance()->Write(_T("QueryCommodity()..."));
	
	if (m_CommodityArray.GetCount() <= 0) //�յ���ͬ��һ��
	{
		SetTimer(TimerIdOfSync, 200, NULL);
	}
	else
	{
		FillGird();
	}

	CIBALog::GetInstance()->Write(_T("CJXCCommoditySelDlg::OnInitDialog()---End......"));
	
	return FALSE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CJXCCommoditySelDlg::InitGridTree()
{
	CRect rectGrid;
	GetDlgItem(IDC_LIST_COMMODITY)->GetClientRect(&rectGrid);
	GetDlgItem(IDC_LIST_COMMODITY)->MapWindowPoints(this, &rectGrid);

	m_wndGridTree.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rectGrid, this, (UINT)-1);
	m_wndGridTree.SetFont(&m_Font);

	CIBAString strHeader;
	strHeader.LoadString(IDS_JXC_COMM_SEL_HEAD);
	//strHeader = L"��Ʒ���|��Ʒ����|��Ʒ����|�۸�|��λ|�������";

	CStringArray strArray;
	CIBAHelpper::SplitLine(strHeader, strArray);

	const UINT Width[] = {80, 240, 120, 70, 50, 80};

	for (INT i = 0; i < strArray.GetCount(); i++)
	{
		m_wndGridTree.InsertColumn(i, strArray.GetAt(i), Width[i]);
		m_wndGridTree.SetColumnAlign(i, HDF_LEFT);
	}
}

void CJXCCommoditySelDlg::FillGird()
{	
	CString strLastGroupId;
	CBCGPGridRow* pLastGroup = NULL;

	const INT nColumns = m_wndGridTree.GetColumnCount();

	for (INT i = 0; i < m_CommodityArray.GetCount(); i++)
	{	
		CCommodity & Commodity = m_CommodityArray.GetAt(i);

		INT nTmp = 0; 

		CString strTmp = Commodity.GetGroupId();

		if (strLastGroupId != strTmp)
		{	
			strLastGroupId = strTmp;
			pLastGroup = m_wndGridTree.CreateRow(nColumns);
			pLastGroup->AllowSubItems();
			pLastGroup->GetItem(0)->SetValue(Commodity.GetGroupName().GetString());
			pLastGroup->GetItem(0)->SetData(INVALID_COMMODITYID);
			m_wndGridTree.AddRow(pLastGroup, FALSE);
		}

		CBCGPGridRow* pCommodity = m_wndGridTree.CreateRow(nColumns);
		ASSERT_VALID(pCommodity);

		pCommodity->GetItem(0)->SetData(i);
		pCommodity->GetItem(1)->SetValue(Commodity.GetCommodityName().GetString());
		pCommodity->GetItem(2)->SetValue(Commodity.GetBarCode().GetString());

		UINT nMul = Commodity.GetUnitMultiple();
		double dblTmp = 0;
		if( nMul == 0 )
		{
			CIBALog::GetInstance()->Write(_T("��Ʒ��λ����Ϊ0,�����Զ�ת��Ϊ1"));
			Commodity.SetUnitMultiple(1);
		}
		dblTmp = Commodity.GetContractPrice() / Commodity.GetUnitMultiple() / 100.0;
		strTmp.Format(_T("%.2f"), dblTmp);
		
		pCommodity->GetItem(3)->SetValue(strTmp.GetString());

		CIBADAL::GetInstance()->GetCommodityUint(Commodity.GetRetailUnit(), m_strCommodityRetailUint);
		pCommodity->GetItem(4)->SetValue(m_strCommodityRetailUint.GetString());

		pCommodity->GetItem(5)->SetValue(Commodity.GetRemainQuantity());

		pLastGroup->AddSubItem(pCommodity, FALSE);
	}

	m_wndGridTree.AdjustLayout();

	m_wndGridTree.SetFocus();

	CBCGPGridRow* pRow = m_wndGridTree.GetRow(1);

	if (pRow != NULL)
	{
		m_wndGridTree.EnsureVisible(pRow);
		m_wndGridTree.SetCurSel(pRow);
	}

}

void CJXCCommoditySelDlg::OnDestroy()
{
	CIBADialog::OnDestroy();

	m_wndGridTree.DeleteAllColumns();
	m_wndGridTree.DestroyWindow();
}


void CJXCCommoditySelDlg::OnCancel()
{
	CBCGPDialog::OnCancel();
}

void CJXCCommoditySelDlg::OnOK()
{
	CBCGPGridRow* pRow = m_wndGridTree.GetCurSel();

	if (pRow != NULL)
	{
		m_nCurSel = pRow->GetItem(0)->GetData();
	}
	else
	{
		m_nCurSel = INVALID_COMMODITYID;
	}

	if (GetCurSelData(m_nCurSel))
	{
		CBCGPDialog::OnOK();
	}
	else
	{
		CBCGPDialog::OnCancel();
	}
}

void CJXCCommoditySelDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (0 == nFlags && point == CPoint(0, 0)) 
	{
		CBCGPGridRow* pCommodity = m_wndGridTree.GetCurSel();

		if (pCommodity != NULL)
		{
			m_nCurSel = pCommodity->GetItem(0)->GetData();
			
			if (m_nCurSel != INVALID_COMMODITYID)
			{
				OnOK();
			}
		}
		else
		{
			m_nCurSel = INVALID_COMMODITYID;
		}
	}
	else
	{
		CIBADialog::OnLButtonUp(nFlags, point);
	}
}

BOOL CJXCCommoditySelDlg::GetCurSelData(INT nCurSelIndex /*= INVALID_COMMODITYID*/)
{
	//��Чѡ��
	if (nCurSelIndex == INVALID_COMMODITYID)
	{
		return FALSE;
	}
	//Խ��
	if (nCurSelIndex < 0 || nCurSelIndex >= m_CommodityArray.GetCount())
	{
		return FALSE;
	}
	//��¼����
	CCommodity & TmpCommodity = m_CommodityArray.GetAt(nCurSelIndex);
	//��Ʒ����
	m_strCommodityName = TmpCommodity.GetCommodityName();
	//��Ʒ����
	m_strCommodityType = TmpCommodity.GetGroupName();
	//��Ʒ����
	//m_strCommodityCode = TmpCommodity.GetBarCode();
	m_strCommodityCode = TmpCommodity.GetCommodityCode();
	//��Ʒ����
	m_nPrice = TmpCommodity.GetContractPrice() / TmpCommodity.GetUnitMultiple();
	//��Ʒ�������
	m_nDepositCount = TmpCommodity.GetRemainQuantity();
	//��Ʒ��λ
	CIBADAL::GetInstance()->GetCommodityUint(TmpCommodity.GetRetailUnit(), m_strCommodityRetailUint);
	//������
	m_nUnitMutiple = TmpCommodity.GetUnitMultiple();
	//��λ
	CIBADAL::GetInstance()->GetCommodityUint(TmpCommodity.GetStoreUnit(), m_strCommodityStoreUint);

	return TRUE;
}

void CJXCCommoditySelDlg::OnBnClickedButtonSyncserver()
{
	CWaitCursor WaitCursor;

	GetDlgItem(IDC_BUTTON_SYNCSERVER)->EnableWindow(FALSE);

	SetToolTipPos(IDC_BUTTON_SYNCSERVER);

	if (CServletHellper::Synchronize())
	{
		ShowToolTip(_T("����ͬ���ɹ���"));

		m_CommodityArray.RemoveAll();
		m_wndGridTree.RemoveAll();

		QueryCommodity();
		FillGird();
		
		m_wndGridTree.SetFocus();
	}
	else
	{
		ShowToolTip(_T("����ͬ��ʧ�ܣ�"), TRUE);
	}

	GetDlgItem(IDC_BUTTON_SYNCSERVER)->EnableWindow(TRUE);
}

void CJXCCommoditySelDlg::QueryCommodity()
{
	CCommodityQueryCondition CommodityQueryCondition;

	if (!m_strCommodityCode.IsEmpty())
	{
		CommodityQueryCondition.m_strCommodityId = m_strCommodityCode;
	}

	try
	{
		CIBADAL::GetInstance()->GetCommodity(m_CommodityArray, CommodityQueryCondition);
	}
	catch (...)
	{
		CIBALog::GetInstance()->Write(_T("CJXCCommoditySelDlg::QueryCommodity() Error!"));
	}
}

void CJXCCommoditySelDlg::OnTimer(UINT nIDEvent)
{
	CIBADialog::OnTimer(nIDEvent);

	if (TimerIdOfSync == nIDEvent)
	{
		KillTimer(TimerIdOfSync);
		OnBnClickedButtonSyncserver();
	}
}
