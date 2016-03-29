// StoreDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include ".\storedlg.h"
#include "Cashier.h"
#include "IBAHelpper.h"
#include "Dal\DALJXC.h"
#include "Dal\IBADAL.h"
#include "Socket\LocalServer.h"
#include "ComputerListView.h"

// CStoreDlg �Ի���

IMPLEMENT_DYNAMIC(CStoreDlg, CIBADialog)
CStoreDlg::CStoreDlg(CWnd* pParent /*=NULL*/)
: CIBADialog(CStoreDlg::IDD, pParent)
{
	m_lpCommodityGrid = NULL;
}

CStoreDlg::~CStoreDlg()
{
}

void CStoreDlg::DoDataExchange(CDataExchange* pDX)
{
	CIBADialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SORT, m_cboSortType);
}

BEGIN_MESSAGE_MAP(CStoreDlg, CIBADialog)
	ON_CBN_SELCHANGE(IDC_COMBO_SORT, OnCbnSelchangeComboSort)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_PRINTCOMMODITY, OnBnClickedButtonPrintcommodity)
END_MESSAGE_MAP()


// CStoreDlg ��Ϣ�������

BOOL CStoreDlg::OnInitDialog()
{
	CIBADialog::OnInitDialog();

	InitCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

// ��ʼ���ؼ�
BOOL CStoreDlg::InitCtrl(void)
{
	//ͬ�����ݿ��
	CStoreQuantityMethod StoreQuantity;//����̵�ͳ�Ʒ���

	theApp.GetCurCashier()->DoStoreQuantity(StoreQuantity);

	if (StoreQuantity.GetStatusCode() == 0)
	{	
		// 2011/04/27-gxx: ע�͵����д���://������н��ױ���������
		//CIBADAL::GetInstance()->ZeroAllCommodityRemainQuantity();

		for (UINT i = 0; i < StoreQuantity.GetRecordCount(); i++)
		{//���¿���¼����Ʒ���+ ���������
			CIBADAL::GetInstance()->UpdateCommodityRemainQuantity(StoreQuantity.GetCommodityCode(i), StoreQuantity.GetRemainQuantity(i));	
		}
	}
	else
	{
		ShowToolTip(StoreQuantity.GetStatusMessage(), TRUE);
	}

	int index = m_cboSortType.SetCurSel(0);

	if (!SetGrid(index))
	{
		return FALSE;
	}

	return TRUE;
}

// ��ʼ��GRID
BOOL CStoreDlg::SetGrid(UINT nType)
{
	CRect rectGrid;
	GetDlgItem(IDC_STATIC_COMMODITY)->GetClientRect(&rectGrid);
	GetDlgItem(IDC_STATIC_COMMODITY)->MapWindowPoints(this, &rectGrid);
	
	m_CommodityGrid.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rectGrid, this, (UINT)-1);
	
	m_lpCommodityGrid = &m_CommodityGrid;

	switch(nType)
	{
	case 0:
		{
			InitSortGrid();
			FillSortGird();
			break;
		}
	case 1:
		{
			InitPriceGrid();
			FillPriceGird();
			break;
		}
	case 2:
		{
			InitSortPriceGrid();
			FillSortPriceGird();
			break;
		}
	}
	return TRUE;
}

// ��ʼ��GRID
BOOL CStoreDlg::InitSortPriceGrid(void)
{
	m_lpCommodityGrid->RemoveAll();
	
	while(m_lpCommodityGrid->GetColumnCount() > 0)
	{
		m_lpCommodityGrid->DeleteColumn(0);
	}

	CIBAString strHeader;
	strHeader.LoadString(IDS_JXC_STORE_HEAD);
	//strHeader = L"��Ʒ���+�۸�|��Ʒ����|��Ʒ����|���|�ֽ���|���|�˻�����|����|�һ�����";

	CStringArray strArray;
	CIBAHelpper::SplitLine(strHeader, strArray);

	if (theApp.GetCurCashier()->IsOnDuty())
	{
		const UINT Width[] = {120, 180, 90, 40, 60, 40, 60, 40, 60};

		for (INT i = 0; i < strArray.GetCount(); i++)
		{
			m_lpCommodityGrid->InsertColumn(i, strArray.GetAt(i), Width[i]);
			m_lpCommodityGrid->SetColumnAlign(i, HDF_LEFT);
		}
	}
	else
	{
		m_lpCommodityGrid->SetFont(&m_Font);

		const UINT Width[] = {120, 250, 140, 90};

		for (INT i = 0; i < 4; i++)
		{
			m_lpCommodityGrid->InsertColumn(i, strArray.GetAt(i), Width[i]);
			m_lpCommodityGrid->SetColumnAlign(i, HDF_LEFT);
		}
	}

	return TRUE;
}

// ��ʼ��GRID
BOOL CStoreDlg::InitPriceGrid(void)
{
	m_lpCommodityGrid->RemoveAll();
	while(m_lpCommodityGrid->GetColumnCount() > 0)
	{
		m_lpCommodityGrid->DeleteColumn(0);
	}

	CIBAString strHeader;
	strHeader.LoadString(IDS_JXC_STORE_HEAD2);
	//strHeader = L"��Ʒ�۸�|��Ʒ����|��Ʒ����|��Ʒ���|���|�ֽ���|���|�˻�����|����|�һ�����";

	CStringArray strArray;
	CIBAHelpper::SplitLine(strHeader, strArray);

	if (theApp.GetCurCashier()->IsOnDuty())
	{
		const UINT Width[] = {60, 180, 90, 90, 40, 60, 40, 60, 40, 60};

		for (INT i = 0; i < strArray.GetCount(); i++)
		{
			m_lpCommodityGrid->InsertColumn(i, strArray.GetAt(i), Width[i]);
			m_lpCommodityGrid->SetColumnAlign(i, HDF_LEFT);
		}
	}
	else
	{
		m_lpCommodityGrid->SetFont(&m_Font);

		const UINT Width[] = {70, 250, 140, 90, 90};

		for (INT i = 0; i < 5; i++)
		{
			m_lpCommodityGrid->InsertColumn(i, strArray.GetAt(i), Width[i]);
			m_lpCommodityGrid->SetColumnAlign(i, HDF_LEFT);
		}
	}

	return TRUE;
}

// ��ʼ��GRID
BOOL CStoreDlg::InitSortGrid(void)
{
	m_lpCommodityGrid->RemoveAll();
	while(m_lpCommodityGrid->GetColumnCount() > 0)
	{
		m_lpCommodityGrid->DeleteColumn(0);
	}

	CIBAString strHeader;
	strHeader.LoadString(IDS_JXC_STORE_HEAD2);
	//strHeader = L"��Ʒ���|��Ʒ����|��Ʒ����|�۸�|���|�ֽ���|���|�˻�����|����|�һ�����";

	CStringArray strArray;
	CIBAHelpper::SplitLine(strHeader, strArray);

	if (theApp.GetCurCashier()->IsOnDuty())
	{
		const UINT Width[] = {60, 180, 90, 40, 40, 60, 40, 60, 40, 60};

		for (INT i = 0; i < strArray.GetCount(); i++)
		{
			m_lpCommodityGrid->InsertColumn(i, strArray.GetAt(i), Width[i]);
			m_lpCommodityGrid->SetColumnAlign(i, HDF_LEFT);
		}
	}
	else
	{
		m_lpCommodityGrid->SetFont(&m_Font);

		const UINT Width[] = {70, 250, 140, 90, 90};

		for (INT i = 0; i < 5; i++)
		{
			m_lpCommodityGrid->InsertColumn(i, strArray.GetAt(i), Width[i]);
			m_lpCommodityGrid->SetColumnAlign(i, HDF_LEFT);
		}
	}

	return TRUE;
}

//��䰴����ͼ۸��������
//strHeader = L"��Ʒ���|��Ʒ����|��Ʒ����|�۸�|���|�ֽ���|���|�˻�����|����|�һ�����";
void CStoreDlg::FillSortPriceGird()
{
	m_lpCommodityGrid->RemoveAll();

	CCommodityArray CommodityArray;//������Ʒ����
	CCommodityQueryCondition CommodityQueryCondition;//��Ʒ��ѯ����������ָ���ض���Ʒ��ѯ
	//�����ݿ�ȡ�ÿ������
	if (!CIBADAL::GetInstance()->GetSortPriceCommodity(CommodityArray, CommodityQueryCondition)) return;

	CCommoditySoldInfoArray CommoditySoldInfoArray;
	if (!CIBADAL::GetInstance()->GetCommoditySoldInfo(CommoditySoldInfoArray)) return;

	//ȡ������
	const INT nColumns = m_lpCommodityGrid->GetColumnCount();

	INT nStoreNum = 0; //�������

	CString strLastGroupId;//��һ�ε���Ʒ
	UINT nLastPrice = 0;
	CBCGPGridRow* pLastGroup = NULL;
	//��Ʒ��¼
	for (INT i = 0; i < CommodityArray.GetCount(); i++)
	{	//��Ʒ��Ϣ
		CCommodity & Commodity = CommodityArray.GetAt(i);

		//��Ʒ���ID
		CString strTmp = Commodity.GetGroupId();
		UINT nTmpPrice = Commodity.GetAdvicePrice();
		//����Ʒ����ͬһ��
		if (strLastGroupId != strTmp || nLastPrice != nTmpPrice)
		{	
			if (nStoreNum > 0)//��������0
			{
				pLastGroup->GetItem(3)->SetValue(nStoreNum);
				nStoreNum = 0;
			}

			strLastGroupId = strTmp;
			nLastPrice = nTmpPrice;
			//������¼
			pLastGroup = m_lpCommodityGrid->CreateRow(nColumns);
			pLastGroup->AllowSubItems();// ����ʽ�Ӳ˵�
			strTmp = Commodity.GetGroupName();
			strTmp.AppendFormat(_T(" %.2f"), nTmpPrice / 100.0);
			pLastGroup->GetItem(0)->SetValue(strTmp.GetString());
			//��Ӽ�¼
			m_lpCommodityGrid->AddRow(pLastGroup, FALSE);
		}

		//����Ʒ���ֹ�
		CBCGPGridRow* pCommodity = m_lpCommodityGrid->CreateRow(nColumns);
		ASSERT_VALID(pCommodity);
		//��������
		pCommodity->GetItem(1)->SetValue(Commodity.GetCommodityName().GetString());
		pCommodity->GetItem(2)->SetValue(Commodity.GetBarCode().GetString());
		pCommodity->GetItem(3)->SetValue(Commodity.GetRemainQuantity());

		if (theApp.GetCurCashier()->IsOnDuty())
		{
			for (INT j = 0; j < CommoditySoldInfoArray.GetCount(); j++)
			{
				CCommoditySoldInfo CommoditySoldInfo = CommoditySoldInfoArray.GetAt(j);

				if (CommoditySoldInfo.GetCommoditycode().CompareNoCase(Commodity.GetBarCode()) == 0)
				{
					//�ֽ�ϼ�����
					INT nCount = CommoditySoldInfo.GetSoldCount1() + CommoditySoldInfo.GetReturnCount1();

					if (nCount > 0)
					{
						pCommodity->GetItem(4)->SetValue(nCount);
						strTmp.Format(_T("%.2f"), (CommoditySoldInfo.GetSoldAmout1() + CommoditySoldInfo.GetReturnAmout1()) / 100.0);
						pCommodity->GetItem(5)->SetValue(strTmp.GetString());
					}
					//�˻��ϼ�����
					nCount = CommoditySoldInfo.GetSoldCount2() + CommoditySoldInfo.GetReturnCount2();

					if (nCount > 0)
					{
						pCommodity->GetItem(6)->SetValue(nCount);
					}
					//���ͺϼ�����
					if (CommoditySoldInfo.GetPresentCount() > 0)
					{
						pCommodity->GetItem(7)->SetValue(CommoditySoldInfo.GetPresentCount());
					}

					if (CommoditySoldInfo.GetExchangePoint())
					{
						pCommodity->GetItem(8)->SetValue(CommoditySoldInfo.GetExchangePoint());
					}
				}
			}
		}

		nStoreNum += Commodity.GetRemainQuantity();
		pLastGroup->AddSubItem(pCommodity, FALSE);//������б�

	}

	if (pLastGroup != NULL) //����Ʒ����
	{
		pLastGroup->GetItem(3)->SetValue(nStoreNum);
	}

	m_lpCommodityGrid->AdjustLayout();
}

//������Ʒ����
//strHeader = L"��Ʒ���|��Ʒ����|��Ʒ����|�۸�|���|�ֽ���|���|�˻�����|����|�һ�����";
void CStoreDlg::FillPriceGird()
{
	m_lpCommodityGrid->RemoveAll();

	CCommodityArray CommodityArray;//������Ʒ����
	CCommodityQueryCondition CommodityQueryCondition;//��Ʒ��ѯ����
	//�����ݿ�ȡ�ÿ������
	if (!CIBADAL::GetInstance()->GetPriceCommodity(CommodityArray, CommodityQueryCondition)) return;

	CCommoditySoldInfoArray CommoditySoldInfoArray;
	if (!CIBADAL::GetInstance()->GetCommoditySoldInfo(CommoditySoldInfoArray)) return;

	//ȡ������
	const INT nColumns = m_lpCommodityGrid->GetColumnCount();

	UINT nStoreNum = 0; //�������

	UINT nPrePrice = 0;//��ǰ�ļ۸�

	CString strTmp;//��ʱ����
	CBCGPGridRow* pLastGroup = NULL;
	//��Ʒ��¼
	for (INT i = 0; i < CommodityArray.GetCount(); i++)
	{	//��Ʒ��Ϣ
		CCommodity & Commodity = CommodityArray.GetAt(i);
		//����ͷ����
		UINT nPrice = Commodity.GetAdvicePrice();
		if (nPrice != nPrePrice)
		{
			if (nStoreNum > 0)//��������0
			{
				pLastGroup->GetItem(4)->SetValue(nStoreNum);
				nStoreNum = 0;
			}

			nPrePrice = nPrice;
			//������¼
			pLastGroup = m_lpCommodityGrid->CreateRow(nColumns);
			pLastGroup->AllowSubItems();// ����ʽ�Ӳ˵�
			strTmp.Format(_T("%.2f"), Commodity.GetAdvicePrice() / 100.0);
			pLastGroup->GetItem(0)->SetValue(strTmp.GetString());
			//��Ӽ�¼
			m_lpCommodityGrid->AddRow(pLastGroup, FALSE);
		}

		//����Ʒ���ֹ�
		CBCGPGridRow* pCommodity = m_lpCommodityGrid->CreateRow(nColumns);
		ASSERT_VALID(pCommodity);
		//��������
		pCommodity->GetItem(1)->SetValue(Commodity.GetCommodityName().GetString());
		pCommodity->GetItem(2)->SetValue(Commodity.GetBarCode().GetString());
		strTmp.Format(_T("%.2f"), Commodity.GetAdvicePrice() / 100.0);
		pCommodity->GetItem(3)->SetValue(Commodity.GetGroupName().GetString());
		pCommodity->GetItem(4)->SetValue(Commodity.GetRemainQuantity());

		if (theApp.GetCurCashier()->IsOnDuty())
		{
			for (INT j = 0; j < CommoditySoldInfoArray.GetCount(); j++)
			{
				CCommoditySoldInfo CommoditySoldInfo = CommoditySoldInfoArray.GetAt(j);

				if (CommoditySoldInfo.GetCommoditycode().CompareNoCase(Commodity.GetBarCode()) == 0)
				{
					//�ֽ�ϼ�����
					INT nCount = CommoditySoldInfo.GetSoldCount1() + CommoditySoldInfo.GetReturnCount1();

					if (nCount > 0)
					{
						pCommodity->GetItem(5)->SetValue(nCount);
						strTmp.Format(_T("%.2f"), (CommoditySoldInfo.GetSoldAmout1() + CommoditySoldInfo.GetReturnAmout1()) / 100.0);
						pCommodity->GetItem(6)->SetValue(strTmp.GetString());
					}
					//�˻��ϼ�����
					nCount = CommoditySoldInfo.GetSoldCount2() + CommoditySoldInfo.GetReturnCount2();

					if (nCount > 0)
					{
						pCommodity->GetItem(7)->SetValue(nCount);
					}
					//���ͺϼ�����
					if (CommoditySoldInfo.GetPresentCount() > 0)
					{
						pCommodity->GetItem(8)->SetValue(CommoditySoldInfo.GetPresentCount());
					}

					if (CommoditySoldInfo.GetExchangePoint())
					{
						pCommodity->GetItem(9)->SetValue(CommoditySoldInfo.GetExchangePoint());
					}
				}
			}

		}

		nStoreNum += Commodity.GetRemainQuantity();
		pLastGroup->AddSubItem(pCommodity, FALSE);//������б�
	}

	if (pLastGroup != NULL) //����Ʒ����
	{
		pLastGroup->GetItem(4)->SetValue(nStoreNum);
	}

	m_lpCommodityGrid->AdjustLayout();

}

//������Ʒ����
//strHeader = L"��Ʒ���|��Ʒ����|��Ʒ����|�۸�|���|�ֽ���|���|�˻�����|����|�һ�����";
void CStoreDlg::FillSortGird()
{	
	m_lpCommodityGrid->RemoveAll();

	CCommodityArray CommodityArray;//��Ʒ����
	CCommodityQueryCondition CommodityQueryCondition;//��ѯ����
	
	//ȡ����Ʒ������ݺ���Ʒ��ѯ����
	if (!CIBADAL::GetInstance()->GetStoreCommodity(CommodityArray, CommodityQueryCondition)) return;

	CCommoditySoldInfoArray CommoditySoldInfoArray;
	if (!CIBADAL::GetInstance()->GetCommoditySoldInfo(CommoditySoldInfoArray)) return;

	const INT nColumns = m_lpCommodityGrid->GetColumnCount();

	INT nTmp = 0; 

	CString strLastGroupId;
	CBCGPGridRow* pLastGroup = NULL;

	for (INT i = 0; i < CommodityArray.GetCount(); i++)
	{	
		CCommodity & CommodityInfo = CommodityArray.GetAt(i);

		CString strTmp = CommodityInfo.GetGroupId();

		if (strLastGroupId != strTmp)
		{	
			if (nTmp > 0)
			{
				pLastGroup->GetItem(4)->SetValue(nTmp);
				nTmp = 0;
			}

			strLastGroupId = strTmp;
			pLastGroup = m_lpCommodityGrid->CreateRow(nColumns);
			pLastGroup->AllowSubItems();
			pLastGroup->GetItem(0)->SetValue(CommodityInfo.GetGroupName().GetString());
			m_lpCommodityGrid->AddRow(pLastGroup, FALSE);
		}

		CBCGPGridRow* pCommodity = m_lpCommodityGrid->CreateRow(nColumns);
		ASSERT_VALID(pCommodity);

		pCommodity->GetItem(1)->SetValue(CommodityInfo.GetCommodityName().GetString());
		pCommodity->GetItem(2)->SetValue(CommodityInfo.GetBarCode().GetString());
		strTmp.Format(_T("%.2f"), CommodityInfo.GetAdvicePrice() / 100.0);
		pCommodity->GetItem(3)->SetValue(strTmp.GetString());
		pCommodity->GetItem(4)->SetValue(CommodityInfo.GetRemainQuantity());

		if (theApp.GetCurCashier()->IsOnDuty())
		{
			for (INT j = 0; j < CommoditySoldInfoArray.GetCount(); j++)
			{
				CCommoditySoldInfo CommoditySoldInfo = CommoditySoldInfoArray.GetAt(j);

				if (CommoditySoldInfo.GetCommoditycode().CompareNoCase(CommodityInfo.GetBarCode()) == 0)
				{
					//�ֽ�ϼ�����
					INT nCount = CommoditySoldInfo.GetSoldCount1() + CommoditySoldInfo.GetReturnCount1();

					if (nCount > 0)
					{
						pCommodity->GetItem(5)->SetValue(nCount);
						strTmp.Format(_T("%.2f"), (CommoditySoldInfo.GetSoldAmout1() + CommoditySoldInfo.GetReturnAmout1()) / 100.0);
						pCommodity->GetItem(6)->SetValue(strTmp.GetString());
					}
					//�˻��ϼ�����
					nCount = CommoditySoldInfo.GetSoldCount2() + CommoditySoldInfo.GetReturnCount2();

					if (nCount > 0)
					{
						pCommodity->GetItem(7)->SetValue(nCount);
					}
					//���ͺϼ�����
					if (CommoditySoldInfo.GetPresentCount() > 0)
					{
						pCommodity->GetItem(8)->SetValue(CommoditySoldInfo.GetPresentCount());
					}

					if (CommoditySoldInfo.GetExchangePoint())
					{
						pCommodity->GetItem(9)->SetValue(CommoditySoldInfo.GetExchangePoint());
					}
				}
			}
		}

		nTmp += CommodityInfo.GetRemainQuantity();

		pLastGroup->AddSubItem(pCommodity, FALSE);
	}

	if (pLastGroup != NULL) //����Ʒ����
	{
		pLastGroup->GetItem(4)->SetValue(nTmp);
	}

	m_lpCommodityGrid->AdjustLayout();

}

void CStoreDlg::OnCbnSelchangeComboSort()
{
	UINT nIndex = m_cboSortType.GetCurSel();

	switch(nIndex)
	{
	case 0:
		{
			InitSortGrid();
			FillSortGird();
			break;
		}
	case 1:
		{
			InitPriceGrid();
			FillPriceGird();
			break;
		}
	case 2:
		{
			InitSortPriceGrid();
			FillSortPriceGird();
			break;
		}
	default:
		break;
	}
}

void CStoreDlg::OnBnClickedOk()
{
	CBCGPDialog::OnOK();
}

void CStoreDlg::OnBnClickedButtonPrintcommodity()
{
	//CStoreDlg::OnBnClickedOk();

	//BCGPPrintPreview(&m_CommodityGridView);

	m_lpCommodityGrid->ExpandAll();
	m_lpCommodityGrid->Print();

}

//void CStoreDlg::OnDestroy()
//{
//	//PostQuitMessage(0);
//
//	CIBADialog::OnDestroy();
//
//	// TODO: �ڴ˴������Ϣ����������
//}
