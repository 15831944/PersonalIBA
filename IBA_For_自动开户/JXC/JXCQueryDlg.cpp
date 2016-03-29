// JXC\JXCQueryDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "..\IBA.h"
#include "..\Cashier.h"
#include "..\IBAHelpper.h"
#include "..\dal\IBADAL.h"
#include ".\JXCQueryDlg.h"


// CJXCQueryDlg �Ի���

IMPLEMENT_DYNAMIC(CJXCQueryDlg, CIBADialog)
CJXCQueryDlg::CJXCQueryDlg(CWnd* pParent /*=NULL*/)
: CIBADialog(CJXCQueryDlg::IDD, pParent)
{
}

CJXCQueryDlg::~CJXCQueryDlg()
{
}

void CJXCQueryDlg::DoDataExchange(CDataExchange* pDX)
{
	CIBADialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BUTTON_STARTTIME, m_StartTime);
	DDX_Control(pDX, IDC_BUTTON_ENDTIME, m_EndTime);
	
	DDX_Control(pDX, IDC_COMBO_CASHIER, m_cboCashier);
	DDX_Control(pDX, IDC_COMBO_WAY, m_cboRetailType);
	DDX_Control(pDX, IDC_COMBO_PAYTYPE, m_cboPayType);

	DDX_Text(pDX, IDC_EDIT_COMMODITYID, m_strCommodityId);
	DDX_Text(pDX, IDC_STATIC_INFO, m_strInfo);
}


BEGIN_MESSAGE_MAP(CJXCQueryDlg, CIBADialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CJXCQueryDlg::InitTimeControl()
{
	const UINT stateMask = 
		CBCGPDateTimeCtrl::DTM_SPIN |
		CBCGPDateTimeCtrl::DTM_DROPCALENDAR | 
		CBCGPDateTimeCtrl::DTM_DATE |
		CBCGPDateTimeCtrl::DTM_TIME24H |
		CBCGPDateTimeCtrl::DTM_CHECKBOX |
		CBCGPDateTimeCtrl::DTM_TIME | 
		CBCGPDateTimeCtrl::DTM_TIME24HBYLOCALE;

	UINT stateFlags = 
		CBCGPDateTimeCtrl::DTM_SPIN |
		CBCGPDateTimeCtrl::DTM_DROPCALENDAR | 
		CBCGPDateTimeCtrl::DTM_DATE |
		CBCGPDateTimeCtrl::DTM_TIME24H |
		CBCGPDateTimeCtrl::DTM_TIME;  

	m_StartTime.m_monthFormat = 2;
	m_StartTime.SetState(stateFlags, stateMask);
	m_StartTime.SetFont(&m_Font);
	m_StartTime.SizeToContent();

	m_EndTime.m_monthFormat = 2;
	m_EndTime.SetState(stateFlags, stateMask);
	m_EndTime.SetFont(&m_Font);
	m_EndTime.SizeToContent();

	COleDateTime dtTmp;
	dtTmp.ParseDateTime(theApp.GetCurCashier()->GetOnDutyTime());
	m_StartTime.SetDate(dtTmp);

	dtTmp = COleDateTime::GetCurrentTime();
	dtTmp += COleDateTimeSpan(0, 0, 2, 0);
	m_EndTime.SetDate(dtTmp);

}

void CJXCQueryDlg::InitGridTree()
{
	CRect rectGrid;
	GetDlgItem(IDC_LIST_COMMODITY)->GetClientRect(&rectGrid);
	GetDlgItem(IDC_LIST_COMMODITY)->MapWindowPoints(this, &rectGrid);

	m_wndGridTree.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rectGrid, this, (UINT)-1);

	CIBAString strHeader;
	strHeader.LoadString(IDS_JXCQUERYHEADER);
	//strHeader = L"���۵���|��Ʒ����|��Ʒ����|����|�۸�|�ܽ��|����ʽ|�һ�����";

	CStringArray strArray;
	CIBAHelpper::SplitLine(strHeader, strArray);

	const UINT Width[] = {150, 205, 120, 45, 55, 60, 70, 70};

	for (INT i = 0; i < strArray.GetCount(); i++)
	{
		m_wndGridTree.InsertColumn(i, strArray.GetAt(i), Width[i]);
		m_wndGridTree.SetColumnAlign(i, HDF_LEFT);
	}

	m_wndGridTree.SetFont(&m_Font);
}

BOOL CJXCQueryDlg::OnInitDialog()
{
	CIBADialog::OnInitDialog();

	SetEditFont();
	InitTimeControl();
	InitGridTree();

	// 2011/08/31-8201-gxx: ��Ӧ���Եı仯
#ifndef __USING_INTER_RESOURCE
	{
		CIBAString strCob;
		strCob.LoadString(IDS_PAYTYPE_COMBO_EX);
		CStringArray strArr;
		CIBAHelpper::SplitLine(strCob,strArr);
		if (strArr.GetCount() != 3)
		{
			strArr.RemoveAll();
			strArr.Add(_T("ȫ��"));
			strArr.Add(_T("�ֽ���"));
			strArr.Add(_T("�˻�����"));
		}
		m_cboPayType.ResetContent();
		m_cboPayType.AddString(strArr.GetAt(0));
		m_cboPayType.AddString(strArr.GetAt(1));
	}
	{
		//{ȫ��;����;�˻�;����;
		CIBAString strCob;
		strCob.LoadString(IDS_QUERY_JXC_TYPE);
		CStringArray strArr;
		CIBAHelpper::SplitLine(strCob,strArr);
		if (strArr.GetCount() != 4)
		{
			strArr.RemoveAll();
			strArr.Add(_T("ȫ��"));
			strArr.Add(_T("����"));
			strArr.Add(_T("�˻�"));
			strArr.Add(_T("����"));
		}
		m_cboRetailType.ResetContent();
		m_cboRetailType.AddString(strArr.GetAt(0));
		m_cboRetailType.AddString(strArr.GetAt(1));
		m_cboRetailType.AddString(strArr.GetAt(2));
		m_cboRetailType.AddString(strArr.GetAt(3));
	}
#endif

	m_cboRetailType.SetCurSel(0);
	m_cboRetailType.SetFont(&m_Font);

	m_cboPayType.SetCurSel(0);
	m_cboPayType.SetFont(&m_Font);

	//
	InitOperatorsCombo();

	m_cboCashier.SetFont(&m_Font);

	GetDlgItem(IDC_STATIC_INFO)->SetFont(&m_Font);

	SetTimer(1000, 1000 * 60, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
void CJXCQueryDlg::InitOperatorsCombo()
{
	COperatorArray OperatorArray;
	CIBADAL::GetInstance()->GetOperatorRecord(OperatorArray);
	if (OperatorArray.GetCount() > 0)
	{//�����ݿ����
		for (INT i = 0; i < OperatorArray.GetCount(); i++)
		{
			m_cboCashier.AddString(OperatorArray.GetAt(i));
		}
	}

	m_cboCashier.SetWindowText(theApp.GetCurCashier()->GetName());
}

HBRUSH CJXCQueryDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CIBADialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (CTLCOLOR_STATIC == nCtlColor)
	{
		if (pWnd->GetDlgCtrlID() == IDC_STATIC_INFO)
		{
			pDC->SetTextColor(RGB(0, 0, 255));
		}
	}

	return hbr;
}

void CJXCQueryDlg::OnBnClickedOk()
{
	CWaitCursor Wait;
	
	UpdateData();

	m_wndGridTree.RemoveAll();

	FillGird();

	m_wndGridTree.AdjustLayout();
}

void CJXCQueryDlg::FillGird()
{	
	CString strLastGroupId;
	CBCGPGridRow* pLastGroup = NULL;

	INT nAmount = 0, nReturnAmount = 0, nAllPoint = 0;

	const INT nColumns = m_wndGridTree.GetColumnCount();

	CRetailQueryCondition RetailQueryCondition;

	m_cboCashier.GetWindowText(RetailQueryCondition.m_strCashier);
	RetailQueryCondition.m_strCashier.Trim();

	RetailQueryCondition.m_strStartTime = m_StartTime.ToString();
	RetailQueryCondition.m_strEndTime = m_EndTime.ToString();
	RetailQueryCondition.m_nPayType = m_cboPayType.GetCurSel();
	RetailQueryCondition.m_nRetailType = m_cboRetailType.GetCurSel();
	RetailQueryCondition.m_nStoreId = CIBAConfig::GetInstance()->GetStoreID();
	RetailQueryCondition.m_strCommodityId = m_strCommodityId;

	if (CIBADAL::GetInstance()->GetRetailInfo(m_RetailInfoArray, RetailQueryCondition))
	{
		for (INT i = 0; i < m_RetailInfoArray.GetCount(); i++)
		{	
			CRetailInfo & RetailInfo = m_RetailInfoArray.GetAt(i);

			INT nTmp = 0; 

			CString strTmp = RetailInfo.RetailVoucher.GetVoucherNo();

			if (strLastGroupId != strTmp)
			{	
				strLastGroupId = strTmp;
				pLastGroup = m_wndGridTree.CreateRow(nColumns);
				pLastGroup->AllowSubItems();

				m_cboRetailType.GetLBText(RetailInfo.RetailVoucher.GetRetailType(), strTmp);
				if (RetailInfo.RetailVoucher.GetRetailType() == 3 && RetailInfo.RetailDetail.GetPoint() > 0 )
				{
					strTmp = LOAD_STRING(IDS_POINT);
					//strTmp = _T("���ֶһ�");
				}

				strTmp.Insert(0, '(');
				strTmp.AppendChar(')');
				strTmp.Insert(0, strLastGroupId);

				pLastGroup->GetItem(0)->SetValue(strTmp.GetString());
				pLastGroup->GetItem(1)->SetValue(RetailInfo.RetailVoucher.GetVoucherTime().GetString());

				m_wndGridTree.AddRow(pLastGroup, FALSE);
			}

			CBCGPGridRow* pCommodity = m_wndGridTree.CreateRow(nColumns);

			pCommodity->GetItem(1)->SetValue(RetailInfo.Commodity.GetCommodityName().GetString());
			pCommodity->GetItem(2)->SetValue(RetailInfo.RetailDetail.GetCommodityCode().GetString());
			
			if (RetailInfo.RetailVoucher.GetRetailType() == 2)//�˻�
			{	
				nReturnAmount += RetailInfo.RetailDetail.GetAmount();
			}
			else
			{		
				nAmount += RetailInfo.RetailDetail.GetAmount();			
			}

			pCommodity->GetItem(3)->SetValue(RetailInfo.RetailDetail.GetQuantity());
			
			strTmp.Format(_T("%.2f"), RetailInfo.RetailDetail.GetPrice() / 100.0);
			pCommodity->GetItem(4)->SetValue(strTmp.GetString());

			strTmp.Format(_T("%.2f"), RetailInfo.RetailDetail.GetAmount() / 100.0);
			pCommodity->GetItem(5)->SetValue(strTmp.GetString());

			if (RetailInfo.RetailVoucher.GetRetailType() == 3)//����
			{
				nAllPoint += RetailInfo.RetailDetail.GetPoint();
				strTmp.Format(_T("%d"), RetailInfo.RetailDetail.GetPoint());
				pCommodity->GetItem(7)->SetValue(strTmp.GetString());

				//����ʽ�Ĵ���
				CString strTmp;
				if (RetailInfo.RetailDetail.GetPoint() > 0)
				{
					strTmp = LOAD_STRING(IDS_POINT);
					pCommodity->GetItem(6)->SetValue(strTmp.GetString());
				}
				else
				{
					m_cboRetailType.GetLBText(RetailInfo.RetailVoucher.GetRetailType(), strTmp);
					pCommodity->GetItem(6)->SetValue(strTmp.GetString());
				}
			}
			else
			{
				m_cboPayType.GetLBText(RetailInfo.RetailVoucher.GetPayType(), strTmp);
				pCommodity->GetItem(6)->SetValue(strTmp.GetString());
			}

			pLastGroup->AddSubItem(pCommodity, FALSE);
		}

	}
	
	m_strInfo.Format(IDS_JXCQUERYRESULT,
		m_RetailInfoArray.GetCount(), nAmount / 100.0, nReturnAmount / 100.0, 
		(nAmount + nReturnAmount) / 100.0, nAllPoint);

	//ѡ��״̬����ֹ���������
	//GetDlgItem(IDC_EDIT_COMMODITYID)->SetFocus();;
	CString txt;
	GetDlgItem(IDC_EDIT_COMMODITYID)->GetWindowText(txt);
	((CEdit *)GetDlgItem(IDC_EDIT_COMMODITYID))->SetSel(0, txt.GetLength());
	
	UpdateData(FALSE);
}

void CJXCQueryDlg::OnTimer(UINT nIDEvent)
{
	COleDateTime dtTmp;

	dtTmp = COleDateTime::GetCurrentTime();
	dtTmp += COleDateTimeSpan(0, 0, 2, 0);
	m_EndTime.SetDate(dtTmp);

	CIBADialog::OnTimer(nIDEvent);
}

BOOL CJXCQueryDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message && VK_ESCAPE == pMsg->wParam )
	{
		GetParent()->GetParent()->PostMessage(WM_CLOSE);
	}

	return CIBADialog::PreTranslateMessage(pMsg);
}
