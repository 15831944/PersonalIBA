// JXCPresent.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "..\IBA.h"
#include "JXCPresentDlg.h"
#include "..\NDDialog.h"
#include "..\Servlet\RetailMethod.h"
#include "..\Cashier.h"
#include "..\IBAHelpper.h"
#include ".\jxcpresentdlg.h"
#include "..\Dal\RightsManagement.h"


// CJXCPresentDlg �Ի���

IMPLEMENT_DYNAMIC(CJXCPresentDlg, CNDDialog)
CJXCPresentDlg::CJXCPresentDlg(CWnd* pParent /*=NULL*/)
: CNDDialog(CJXCPresentDlg::IDD, pParent)
, m_bAllowPretranslate(TRUE)
, m_CommodityDlg(this, 2)
, m_nCommodityCount(1)
{
}

CJXCPresentDlg::~CJXCPresentDlg()
{
}

void CJXCPresentDlg::DoDataExchange(CDataExchange* pDX)
{
	CNDDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_COMMODITYID, m_strCommodityId);
	DDX_Text(pDX, IDC_STATIC_NAME, m_strCommodityName);
	DDX_Text(pDX, IDC_STATIC_TYPE, m_strCommodityType);
	DDX_Text(pDX, IDC_STATIC_PRICE, m_strCommodityPrice);
	DDX_Text(pDX, IDC_STATIC_STORE, m_strCommodityRemainQuantity);

	DDX_Control(pDX, IDC_EDIT_COUNT, m_edtCount);

	DDX_Text(pDX, IDC_STATIC_USERNAME, m_strName);
	DDX_Text(pDX, IDC_STATIC_CLASS, m_strUserClass);
	DDX_Text(pDX, IDC_STATIC_ALL, m_strAllBalance);
	DDX_Text(pDX, IDC_STATIC_POINT, m_strPoint);

	DDX_Control(pDX, IDC_LIST_COMMODITY, m_CommodityList);
}


BEGIN_MESSAGE_MAP(CJXCPresentDlg, CNDDialog)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnBnClickedButtonClear)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_COMMODITY, OnBnClickedButtonCommodity)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnBnClickedButtonAdd)
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CJXCPresentDlg ��Ϣ�������

// CJXCExchangeDlg ��Ϣ�������

BOOL CJXCPresentDlg::OnInitDialog()
{
	CNDDialog::OnInitDialog();

	InitListView();

	SetEditFont();

	CIBAHelpper::CreateFont(m_BigFont, 23, 700);
	//GetDlgItem(IDC_STATIC_MONEY)->SetFont(&m_BigFont);
	//�༭��ĸ�������
	m_edtCount.DisableMask();
	m_edtCount.SetValidChars(_T("1234567890"));
	m_edtCount.SetLimitText(3);
	m_edtCount.SetWindowText(_T("1"));

	GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DEL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(FALSE);

	AddFocusEnter(IDC_EDIT_IDCARDSNO, IDC_BUTTON_QUERY);
	AddFocusEnter(IDC_EDIT_COUNT, IDC_BUTTON_ADD);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CJXCPresentDlg::InitListView()
{
	CIBAString strHeader;
	strHeader.LoadString(IDS_JXC_PRESENT_HEAD);
	//strHeader = L"��Ʒ����|��Ʒ����|����|����|�ܽ��";

	CStringArray strArray;
	CIBAHelpper::SplitLine(strHeader, strArray);

	const UINT Width[] = {250, 140, 90, 70, 70};

	for (INT i = 0; i < strArray.GetCount(); i++)
	{
		m_CommodityList.InsertColumn(i, strArray.GetAt(i), LVCFMT_LEFT, Width[i]);
	}

	m_CommodityList.SetFont(&m_Font);
	m_CommodityList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);//����ѡ�з��+�����з��
}

void CJXCPresentDlg::OnOldCard(UINT nCardId, UINT nMemberId)
{
	m_nCardId = nCardId;
	m_nMemberId = nMemberId;

	QueryBalance(m_nMemberId, nCardId);

	UpdateData(FALSE);

	GetDlgItem(IDC_EDIT_COMMODITYID)->SetFocus();

	GetDlgItem(IDOK)->EnableWindow(m_SellRecordArray.GetCount() > 0 && m_nMemberId > 0);
}

void CJXCPresentDlg::OnGetBalanceOK(NS_SERVLET::CBalanceMethod & Balance)
{
	m_strNetId = Balance.GetSerailNum();
	m_strName = Balance.GetUserName();
	m_strPoint.Format(_T("%d"), Balance.GetCurrentPoint());

	m_nUserClassID = Balance.GetUserClass();
	m_strUserClass = CNetBarConfig::GetInstance()->GetUserClassName(Balance.GetUserClass());

	//�ϼ� = �����˻�+�����˻�+�����˻�+Ԥ��������
	INT nAllMoney = Balance.GetCashBalance() + Balance.GetUsePresent() + Balance.GetCreditPresent() + Balance.GetOnlineAmount();
	m_strAllBalance.Format(_T("%.2f"), nAllMoney / 100.0);
}

void CJXCPresentDlg::OnBnClickedButtonDel()
{
	INT nIdx = m_CommodityList.GetSelectionMark();

	if (nIdx < 0)
	{
		return;
	}

	m_SellRecordArray.RemoveAt(nIdx);

	ShowSellRecordInfo();
}

void CJXCPresentDlg::OnBnClickedButtonClear()
{
	if (IDOK == MsgBox(L"ȡ������������", MB_OKCANCEL | MB_ICONQUESTION))
	{
		ClearData2();
		UpdateData(FALSE);
	}
}
//ִ�жһ�
void CJXCPresentDlg::OnBnClickedOk()
{
	if (!CRightsManagement::GetInstance()->Take_IBAP_RETAILPRESENT(TRUE))
	{
		return;
	}
	CWaitCursor Wait;

	CRetailMethod Retail;

	Retail.SetPayType(1);//�ֽ�ʽ
	Retail.SetMemberId(m_nMemberId);
	Retail.SetRetailType(3);
	Retail.SetSellRecords(m_SellRecordArray);
	theApp.GetCurCashier()->DoRetail(Retail);

	SetToolTipPos(IDOK);

	if (Retail.GetStatusCode() == 0)
	{
		SaveRetailData(Retail);

		ClearData2();

		OnBnClickedButtonQuery();

		UpdateData(FALSE);

		ShowToolTip(L"���ͳɹ���");
	}
	else
	{
		CString strTmp;
		strTmp.Format(L"����ʧ�ܣ�ԭ��%s - %s", Retail.GetStatusMessage(), Retail.GetExtInfo());

		ShowToolTip(strTmp, TRUE);
	}

	GetDlgItem(IDC_EDIT_COMMODITYID)->SetFocus();
}

void CJXCPresentDlg::OnBnClickedButtonCommodity()
{
	m_CommodityDlg.m_CommodityArray.RemoveAll();
	m_CommodityDlg.DoModal();
	m_CommodityList.SetFocus();
}

void CJXCPresentDlg::OnBnClickedButtonAdd()
{
	CString strTmp;
	m_edtCount.GetWindowText(strTmp);
	m_nCommodityCount = _ttoi(strTmp);

	if (m_nCommodityCount <= 0)
	{
		SetToolTipPos(IDC_EDIT_COUNT);
		ShowToolTip(IDS_INPUTERROR);
		return;
	}

	NS_SERVLET::CSellRecord SellRecord;//������Ʒ�б�

	SellRecord.SetQuantity(m_nCommodityCount);
	SellRecord.SetCommodityCode(m_CommodityDlg.m_CommodityArray.GetAt(0).GetCommodityCode());
	SellRecord.SetPrice(m_CommodityDlg.m_CommodityArray.GetAt(0).GetAdvicePrice());
	SellRecord.SetAmount(m_nCommodityCount * m_CommodityDlg.m_CommodityArray.GetAt(0).GetAdvicePrice());
	SellRecord.SetCommodityBarCode(m_CommodityDlg.m_CommodityArray.GetAt(0).GetBarCode());
	SellRecord.SetCommodityName(m_CommodityDlg.m_CommodityArray.GetAt(0).GetCommodityName());
	SellRecord.SetPoint(m_nCommodityCount * m_CommodityDlg.m_CommodityArray.GetAt(0).GetPoint());

	AddSellRecord(SellRecord);

	//���������
	m_strCommodityId.Empty();
	m_strCommodityName.Empty();
	m_strCommodityPrice.Empty();
	m_strCommodityType.Empty();
	m_strCommodityRemainQuantity.Empty();

	UpdateData(FALSE);

	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_COMMODITYID)->SetFocus();
}

void CJXCPresentDlg::ShowSellRecordInfo()
{//��ʾ���б�
	UINT nTmp = 0; CString strTmp;

	m_CommodityList.DeleteAllItems();

	for (INT i = 0; i < m_SellRecordArray.GetCount(); i++)
	{
		CSellRecord & SellRecord = m_SellRecordArray.GetAt(i);

		m_CommodityList.InsertItem(i, SellRecord.GetCommodityName());

		m_CommodityList.SetItemText(i, 1, SellRecord.GetCommodityBarCode());

		strTmp.Format(_T("%.2f"), SellRecord.GetPrice() / 100.0);
		m_CommodityList.SetItemText(i, 2, strTmp);

		strTmp.Format(_T("%d"), SellRecord.GetQuantity());
		m_CommodityList.SetItemText(i, 3, strTmp);

		strTmp.Format(_T("%.2f"), SellRecord.GetAmount() / 100.0);
		m_CommodityList.SetItemText(i, 4, strTmp);

		m_CommodityList.SetItemData(i, i);

		nTmp += m_SellRecordArray.GetAt(i).GetAmount();
	}

	if (nTmp > 0)
	{
		m_strAllMoney.Format(_T("%d"), nTmp);	
	}
	else
	{
		m_strAllMoney.Empty();
	}

	UpdateData(FALSE);

	GetDlgItem(IDOK)->EnableWindow(m_SellRecordArray.GetCount() > 0 && m_nMemberId > 0);
	GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(m_SellRecordArray.GetCount() > 0);
	GetDlgItem(IDC_BUTTON_DEL)->EnableWindow(m_SellRecordArray.GetCount() > 0);
}

void CJXCPresentDlg::SaveRetailData(CRetailMethod &Retail)
{//���ݿ����
	CRetailVoucher RetailVoucher;//����ƾ֤

	RetailVoucher.SetVoucherNo(Retail.GetVoucherNo());
	RetailVoucher.SetCashRegisterID(CIBAConfig::GetInstance()->GetCashRegisterID());
	RetailVoucher.SetVoucherTime(CIBAHelpper::FormatCenterTime(Retail.GetTranTime()));
	RetailVoucher.SetStoreId(CIBAConfig::GetInstance()->GetStoreID());
	RetailVoucher.SetMemberId(m_nMemberId);
	RetailVoucher.SetNetBarId(CNetBarConfig::GetInstance()->GetNetBarId());
	RetailVoucher.SetPayType(0); //��������Ϊ0��Ϊ��ͳ�Ʒ���
	RetailVoucher.SetUserId(theApp.GetCurCashier()->GetName());
	RetailVoucher.SetAccountYear(COleDateTime::GetCurrentTime().Format(_T("%Y")));
	RetailVoucher.SetAccountMonth(COleDateTime::GetCurrentTime().Format(_T("%m")));
	RetailVoucher.SetRefNo(Retail.GetRefNo());
	RetailVoucher.SetRetailType(3);

	CIBADAL::GetInstance()->AddRetailVoucher(RetailVoucher);

	for (INT i = 0; i < m_SellRecordArray.GetCount(); i++)
	{
		CRetailDetail RetailDetail;//

		RetailDetail.SetVoucherNo(Retail.GetVoucherNo());
		RetailDetail.SetQuantity(m_SellRecordArray.GetAt(i).GetQuantity());
		RetailDetail.SetPrice(m_SellRecordArray.GetAt(i).GetPrice());
		RetailDetail.SetCommodityCode(m_SellRecordArray.GetAt(i).GetCommodityCode());
		//RetailDetail.SetPoint(m_SellRecordArray.GetAt(i).GetPoint());

		CIBADAL::GetInstance()->AddRetailDetail(RetailDetail);
	}
}
//��ѯ��Ʒ
BOOL CJXCPresentDlg::QueryCommodity()
{
	UpdateData();

	if (m_strCommodityId.IsEmpty())
	{
		return FALSE;
	}

	//���������
	m_strCommodityName.Empty();
	m_strCommodityPrice.Empty();
	m_strCommodityType.Empty();
	m_strCommodityRemainQuantity.Empty();

	UpdateData(FALSE);
	//�ֿ���Ʒ��ѯ
	CCommodityQueryCondition CommodityQueryCondition;
	CommodityQueryCondition.m_strCommodityId = m_strCommodityId;
	CommodityQueryCondition.m_bHasPoint = TRUE;

	m_CommodityDlg.m_CommodityArray.RemoveAll();
	m_CommodityDlg.QueryCommodity(CommodityQueryCondition);//��ѯ�Ի���

	if (m_CommodityDlg.m_CommodityArray.GetCount() > 1)
	{
		m_CommodityDlg.DoModal();
	}
	else
	{
		if (m_CommodityDlg.m_CommodityArray.GetCount() > 0)
		{
			m_strCommodityId = m_CommodityDlg.m_CommodityArray.GetAt(0).GetBarCode();
			m_strCommodityName = m_CommodityDlg.m_CommodityArray.GetAt(0).GetCommodityName();
			m_strCommodityPrice.Format(_T("%d"), m_CommodityDlg.m_CommodityArray.GetAt(0).GetPoint());
			m_strCommodityType = m_CommodityDlg.m_CommodityArray.GetAt(0).GetGroupName();
			m_strCommodityRemainQuantity.Format(_T("%d"), m_CommodityDlg.m_CommodityArray.GetAt(0).GetRemainQuantity());

			UpdateData(FALSE);

			GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_COUNT)->SetFocus();
		}
		else
		{
			SetToolTipPos(IDC_EDIT_COMMODITYID);
			ShowToolTip(IDS_NOCOMMODITY);
		}
	}

	//ѡ��״̬����ֹ���������
	GetDlgItem(IDC_EDIT_COUNT)->SetFocus();
	CString txt;
	GetDlgItem(IDC_EDIT_COMMODITYID)->GetWindowText(txt);
	((CEdit *)GetDlgItem(IDC_EDIT_COMMODITYID))->SetSel(0, txt.GetLength());

	return TRUE;
}

BOOL CJXCPresentDlg::PreTranslateMessage(MSG* pMsg)
{
	//�趨ʱ���ڲ���������Ϣ
	if (!m_bAllowPretranslate)
	{
		return CBCGPDialog::PreTranslateMessage(pMsg);
	}

	if (WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam) //���س�
	{
		CWnd* pWnd = GetFocus(); //����δ�

		if (pWnd != NULL)
		{
			if (pWnd->GetDlgCtrlID() == IDC_EDIT_COMMODITYID)
			{
				if (QueryCommodity())//��Ʒ��ѯ
				{
					m_bAllowPretranslate = FALSE;
					SetTimer(1001, 500, NULL);
					return TRUE;
				}				
			}	
		}
	}

	if (WM_KEYDOWN == pMsg->message && VK_ESCAPE == pMsg->wParam )
	{
		GetParent()->GetParent()->PostMessage(WM_CLOSE);
	}

	return CNDDialog::PreTranslateMessage(pMsg);
}

HBRUSH CJXCPresentDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CNDDialog::OnCtlColor(pDC, pWnd, nCtlColor);

// 	if (CTLCOLOR_STATIC == nCtlColor)
// 	{
// 		if (pWnd->GetDlgCtrlID() == IDC_STATIC_MONEY)
// 		{
// 			pDC->SetTextColor(RGB(0, 0, 255));
// 		}	
// 	}

	return hbr;
}

void CJXCPresentDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if (!bShow)
	{
		m_ToolTip.HideTooltip();
	}

	CNDDialog::OnShowWindow(bShow, nStatus);
}

void CJXCPresentDlg::ClearData()
{
	m_strName.Empty();
	m_strPoint.Empty();
	m_strAllBalance.Empty();
	m_strUserClass.Empty();

	m_nMemberId = 0;
	m_nCardId = 0;
}

void CJXCPresentDlg::ClearData2()
{
	ClearData();

	m_CommodityList.DeleteAllItems();
	m_SellRecordArray.RemoveAll();

	m_strAllMoney.Empty();

	m_strCommodityName.Empty();
	m_strCommodityPrice.Empty();
	m_strCommodityId.Empty();
	m_strCommodityType.Empty();
	m_strCommodityRemainQuantity.Empty();

	m_nCommodityCount = 1;
	m_edtCount.SetWindowText(_T("1"));

	GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DEL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(FALSE);
}

void CJXCPresentDlg::AddSellRecord(NS_SERVLET::CSellRecord& SellRecord)
{//���۶���
	BOOL bFind = FALSE;

	for (INT i = 0; i < m_SellRecordArray.GetCount(); i++)
	{
		if (0 == m_SellRecordArray.GetAt(i).GetCommodityCode().CompareNoCase(SellRecord.GetCommodityCode()))
		{
			m_SellRecordArray.GetAt(i).SetQuantity(m_SellRecordArray.GetAt(i).GetQuantity() + SellRecord.GetQuantity());

			m_SellRecordArray.GetAt(i).SetAmount(SellRecord.GetPrice() * m_SellRecordArray.GetAt(i).GetQuantity());

			bFind = TRUE;

			break;
		}
	}

	if (!bFind)
	{
		m_SellRecordArray.Add(SellRecord);
	}

	ShowSellRecordInfo();
}

void CJXCPresentDlg::OnTimer(UINT nIDEvent)
{
	//��ʱ����
	if (nIDEvent == 1001)
	{
		m_bAllowPretranslate = TRUE;
	}

	CNDDialog::OnTimer(nIDEvent);
}
