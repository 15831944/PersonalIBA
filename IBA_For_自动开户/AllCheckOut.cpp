// AllCheckOut.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "IBAHelpper.h"
#include "ComputerInfo.h"
#include "Socket\LocalServer.h"
#include "ComputerListView.h"
#include "Servlet\UserClassInfo.h"
#include "Dal\IBADAL.h"
#include "Cashier.h"
#include "RealName\IBARealName.h"
#include "MainFrm.h"
#include "..\L2Markup\Markup.h"
#include ".\AllCheckOut.h"
#include "ReturnDlg.h"
#include "CashierHelper.h"
////////////////////////////////////////////////////////////////////////////////
// CBCGPGridButtonItem Class

CBCGPGridButtonItem::CBCGPGridButtonItem (LPCTSTR lpszText, UINT id) :
CBCGPGridItem (lpszText)
{
	m_id = id;

}
//*****************************************************************************************
void CBCGPGridButtonItem::OnDrawValue (CDC* pDC, CRect rect)
{
	ASSERT_VALID(pDC);

	rect.DeflateRect(1, 1);
	pDC->FillRect(rect, &globalData.brBarFace);
	pDC->Draw3dRect(rect, globalData.clrBarLight, globalData.clrBarDkShadow);
	rect.DeflateRect(1, 1);
	pDC->Draw3dRect(rect, globalData.clrBarHilite, globalData.clrBarShadow);

	COLORREF clrText = pDC->SetTextColor(globalData.clrBarText);

	CString strText = (LPCTSTR)(_bstr_t)m_varValue;

	pDC->DrawText(strText, rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
	pDC->SetTextColor(clrText);
}
//*****************************************************************************************
BOOL CBCGPGridButtonItem::OnClickValue(UINT uiMsg, CPoint point)
{
	if (uiMsg != WM_LBUTTONUP)
	{
		return CBCGPGridItem::OnClickValue(uiMsg, point);
	}

	CBCGPGridCtrl* pGridCtrl = GetOwnerList();
	ASSERT_VALID (pGridCtrl);

	pGridCtrl->GetOwner()->PostMessage(WM_COMMAND, MAKEWPARAM(m_id, BN_CLICKED));

	return TRUE;
}


IMPLEMENT_DYNAMIC(CDlgAllCheckOut, CIBADialog)
CDlgAllCheckOut::CDlgAllCheckOut(CWnd* pParent /*=NULL*/)
: CIBADialog(CDlgAllCheckOut::IDD, pParent)
{
	m_bHasAllout = FALSE;
}

CDlgAllCheckOut::~CDlgAllCheckOut()
{
}

void CDlgAllCheckOut::DoDataExchange(CDataExchange* pDX)
{
	CIBADialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_FIND, m_strFind);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
}

BEGIN_MESSAGE_MAP(CDlgAllCheckOut, CIBADialog)
	ON_REGISTERED_MESSAGE(BCGM_GRID_ITEM_CHANGED, OnItemChanged)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, OnBnClickedButtonQuery)
	ON_WM_CLOSE()
	ON_BN_CLICKED(GirdButtonId, OnBnClickedGirdButton)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnBnClickedButtonPrint)
	ON_NOTIFY(GLCN_CHILD_CONTROL, IDC_LIST1, OnGLCNotify)
END_MESSAGE_MAP()

// CDlgAllCheckOut ��Ϣ�������

void CDlgAllCheckOut::OnBnClickedOk() //ִ��ȫ������
{
	if (IDOK != MsgBox(_T("ȷ��Ҫִ��ȫ��ֹͣ�Ʒ���"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
	{
		return;
	}
	m_bHasAllout = TRUE;

	m_ReturnInfoArray.RemoveAll();

	GetDlgItem(IDOK)->EnableWindow(FALSE);

	UINT nMemberCount = 0, 
		 nTmpUserCount = 0,
		 nLocalUserCount = 0;

	arrMembers.RemoveAll();

	if( GetActiveMembers(arrMembers) >= 0)
	{	
		for(int i = 0 ; i < arrMembers.GetCount() ; i++ )
		{
			ReturnInfo rinfo;
			CActiveMember& ActiveMember = arrMembers[i];
			if (ActiveMember.GetIsLocalUser())
			{
				rinfo.bLocal = TRUE;
				rinfo.bLocalReturned = FALSE;
				CCashierHelper::MakeLocalConsumeInfo(ActiveMember, rinfo.LocalInfo);

				nLocalUserCount++;
			}
			else
			{
				rinfo.bLocal = FALSE;
				CCashierHelper::MakeCheckOutInfo(ActiveMember, rinfo.CheckOutInfo);

				if ( rinfo.CheckOutInfo.GetClassState() == 0) //��ʱ�û�
				{
					nTmpUserCount++;
				}
				else
				{
					nMemberCount++;
					rinfo.CheckOutInfo.SetReturnState(1);		
				}
				CCashierHelper::ReturnLocalCenterUser(ActiveMember, &rinfo.CheckOutInfo);

				if ( rinfo.CheckOutInfo.GetClassState() == 0) //��ʱ�û�
				{
					CIBADAL::GetInstance()->UpdateCheckOutInfo(ActiveMember.GetNetId(), 0);
					rinfo.CheckOutInfo.SetReturnState(0);
				}
			}
			
			m_ReturnInfoArray.Add(rinfo);
		}	
	}
	
	//����
	CLocalServer::GetInstance()->LocalCheckOutAllUser();

	//ˢ��
	CLocalServer::GetInstance()->GetComputerListView()->Refresh(TRUE);

	FillList();

	SelFirstOne();

	CString strResult;
	
	strResult.Format(_T("ȫ��ֹͣ�Ʒѣ����гɹ����˻�Ա�û� %d λ, ��ʱ�û� %d λ, �����û� %d λ��"), 
		nMemberCount, nTmpUserCount, nLocalUserCount);
	
	GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(strResult);

	// 2011/07/21-8201-gxx: ���ӱ����û����ж�
	GetDlgItem(IDC_BUTTON_PRINT)->EnableWindow(nTmpUserCount > 0 || nLocalUserCount > 0);

	
}

CBCGPGridRow* CDlgAllCheckOut::CreateNewRow()
{
	CBCGPGridRow* pRow = m_GridCtrl.CreateRow(m_GridCtrl.GetColumnCount());

	for (int i = 0; i < m_GridCtrl.GetColumnCount();i++)
	{
		pRow->GetItem(i)->AllowEdit(FALSE);
	}

	return pRow;
}


void CDlgAllCheckOut::AddRow(INT_PTR nIndex, ReturnInfo &rInfo)
{
	CBCGPGridRow* pRow = CreateNewRow();

	pRow->SetData(nIndex);

	if (rInfo.bLocal)
	{
		pRow->GetItem(0)->SetValue(rInfo.LocalInfo.GetSerialNum().GetString());
		pRow->GetItem(1)->SetValue(rInfo.LocalInfo.GetName().GetString());
		pRow->GetItem(2)->SetValue(CNetBarConfig::GetInstance()->GetUserClassName(rInfo.LocalInfo.GetClassId()).GetString());
		pRow->GetItem(3)->SetValue(rInfo.LocalInfo.GetTermId().GetString());
	
		CString strTmp;

		UINT nTmp = CIBAHelpper::RoundMoney(rInfo.LocalInfo.GetReturnAmount(), CNetBarConfig::GetInstance()->GetRoundType());
		strTmp.Format(_T("%.2f"), nTmp / 100.0);
		pRow->GetItem(4)->SetValue(strTmp.GetString());

		if ( rInfo.bLocalReturned )
		{
			pRow->GetItem(5)->SetValue(_T("���˿�"));
		}
		else
		{
			CBCGPGridItem* pItem = new CBCGPGridButtonItem(_T("�˿�"), GirdButtonId);
			pRow->ReplaceItem(6, pItem);
		}
		
		m_listCtrl.InsertItem(nIndex,_T(""));
		m_listCtrl.SetItemText(nIndex,0,rInfo.LocalInfo.GetSerialNum().GetString());
		m_listCtrl.SetItemText(nIndex,1,rInfo.LocalInfo.GetName().GetString());
		m_listCtrl.SetItemText(nIndex,2,CNetBarConfig::GetInstance()->GetUserClassName(rInfo.LocalInfo.GetClassId()).GetString());
		m_listCtrl.SetItemText(nIndex,3,rInfo.LocalInfo.GetTermId().GetString());
		m_listCtrl.SetItemText(nIndex,4,strTmp.GetString());
		if ( rInfo.bLocalReturned )
		{
			m_listCtrl.SetItemText(nIndex,5,_T("���˿�"));
		}
		else
		{
			m_listCtrl.SetItem(nIndex,6,_T("�˿�"),NS_COMMON::CGxxListCtrl::GLC_Button);
		}
	}
	else
	{
		CCheckOutinfo& CheckOutinfo = rInfo.CheckOutInfo;

		pRow->GetItem(0)->SetValue(CheckOutinfo.GetSerialNum().GetString());
		pRow->GetItem(1)->SetValue(CheckOutinfo.GetUserName().GetString());
		pRow->GetItem(2)->SetValue(CheckOutinfo.GetClassName().GetString());
		pRow->GetItem(3)->SetValue(CheckOutinfo.GetTerminalId().GetString());

		CString strTmp;

		UINT nTmp = CIBAHelpper::RoundMoney(CheckOutinfo.GetBalanceMoney(), CNetBarConfig::GetInstance()->GetRoundType());
		strTmp.Format(_T("%.2f"), nTmp / 100.0);
		pRow->GetItem(4)->SetValue(strTmp.GetString());

		if (CheckOutinfo.GetReturnState() == 1)
		{
			pRow->GetItem(5)->SetValue(_T("���˿�"));
		}
		else
		{
			CBCGPGridItem* pItem = new CBCGPGridButtonItem(_T("�˿�"), GirdButtonId);
			pRow->ReplaceItem(6, pItem);
		}

		m_listCtrl.InsertItem(nIndex,_T(""));
		m_listCtrl.SetItemText(nIndex,0,CheckOutinfo.GetSerialNum().GetString());
		m_listCtrl.SetItemText(nIndex,1,CheckOutinfo.GetUserName().GetString());
		m_listCtrl.SetItemText(nIndex,2,CheckOutinfo.GetClassName().GetString());
		m_listCtrl.SetItemText(nIndex,3,CheckOutinfo.GetTerminalId().GetString());

		m_listCtrl.SetItemText(nIndex,4,strTmp.GetString());
		if ( CheckOutinfo.GetReturnState() == 1 )
		{
			m_listCtrl.SetItemText(nIndex,5,_T("���˿�"));
		}
		else
		{
			m_listCtrl.SetItem(nIndex,6,_T("�˿�"),NS_COMMON::CGxxListCtrl::GLC_Button);
		}
	}

	

	//CBCGPGridCheckItem* pItem = new CBCGPGridCheckItem(CheckOutinfo.GetReturnState() > 0 ? true : false);
	//pRow->ReplaceItem(7, pItem);

	m_GridCtrl.AddRow(pRow, FALSE);
}

BOOL CDlgAllCheckOut::InitList(void)
{
	CRect rectGrid;
	GetDlgItem(IDC_STATIC_CHECKOUT)->GetClientRect(&rectGrid);
	GetDlgItem(IDC_STATIC_CHECKOUT)->MapWindowPoints(this, &rectGrid);
	m_GridCtrl.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rectGrid, this, (UINT)-1);
	
	m_GridCtrl.RemoveAll();
	
	while(m_GridCtrl.GetColumnCount() > 0)
	{
		m_GridCtrl.DeleteColumn(0);
	}

	CString strHeader;
	//strHeader.LoadString(IDS_DUTYJXC);
	strHeader = L"�����˺�|����|�û�����|��������|���˽��(Ԫ)|�˿�״̬";

	CStringArray strArray;
	CIBAHelpper::SplitLine(strHeader, strArray);

	const UINT Width[] = {125, 80, 90, 70, 90, 80, 80};

	for (INT i = 0; i < strArray.GetCount(); i++)
	{
		m_GridCtrl.InsertColumn(i, strArray.GetAt(i), Width[i]);
		m_GridCtrl.SetColumnAlign(i, HDF_LEFT);
	}

	m_GridCtrl.InsertColumn(i, _T(""), 80);

	m_GridCtrl.SetWholeRowSel();
	m_GridCtrl.SetSingleSel(FALSE);
	m_GridCtrl.EnableHeader(TRUE, BCGP_GRID_HEADER_SORT);

	// Set grid colors
	CBCGPGridColors colors;
	colors.m_LeftOffsetColors.m_clrBackground = globalData.clrWindow;
	colors.m_OddColors.m_clrBackground = RGB(238, 246, 255);

	m_GridCtrl.SetColorTheme(colors);//��ɫ������

	//m_GridCtrl.SetFont(&m_Font);

	for(INT i=0; i < strArray.GetCount(); i++)
	{
		m_listCtrl.InsertColumn(i, strArray.GetAt(i),LVCFMT_LEFT,Width[i]);
	}
	m_listCtrl.InsertColumn(strArray.GetCount(), _T(""),LVCFMT_LEFT,80,TRUE,FALSE,TRUE,NULL,NS_COMMON::CGxxListCtrl::GLC_Button);

	return TRUE;
}

BOOL CDlgAllCheckOut::FillList(void)
{
	m_GridCtrl.RemoveAll();

	m_listCtrl.DeleteAllItems();

	if (m_bHasAllout == FALSE)
	{
		CCheckOutInfoArray arrCheckOut;
		CIBADAL::GetInstance()->GetCheckOutInfo(arrCheckOut);//��ѯ���ݿ�
		for (int i = 0; i < arrCheckOut.GetCount(); i++ )
		{
			ReturnInfo rInfo;
			rInfo.bLocal = FALSE;
			rInfo.CheckOutInfo = arrCheckOut.GetAt(i);
			m_ReturnInfoArray.Add(rInfo);
		}
	}

	for (INT i = 0; i < m_ReturnInfoArray.GetCount(); i++)
	{
		ReturnInfo & rInfo = m_ReturnInfoArray.GetAt(i);

		if (!rInfo.bLocal && rInfo.CheckOutInfo.GetClassState() != 0)
		{
			continue; // ֻ���������ʱ�û�
		}
		
		AddRow(i, rInfo);
	}

	m_listCtrl.ShowAllSubWindows();

	SelFirstOne();

	//GetDlgItem(IDOK)->EnableWindow(m_ReturnInfoArray.GetCount() <= 0);

	return TRUE;
}

void CDlgAllCheckOut::OnOK()
{
	CBCGPDialog::OnOK();
}

void CDlgAllCheckOut::OnCancel()
{
	CBCGPDialog::OnCancel();
}

BOOL CDlgAllCheckOut::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		CWnd * pWnd = GetFocus();
		
		if (pWnd != NULL)
		{
			if (pWnd->GetDlgCtrlID() == IDC_EDIT_FIND)
			{
				OnBnClickedButtonQuery();
				return TRUE;
			}
		}
	}

	//���ؽ���

	if (WM_CHAR == pMsg->message && pMsg->wParam != VK_RETURN)
	{
		CWnd* pWnd = GetFocus();
		
		if (pWnd != NULL && pWnd->GetDlgCtrlID() != IDC_EDIT_FIND)
		{
			GetDlgItem(IDC_EDIT_FIND)->SetFocus();
			GetDlgItem(IDC_EDIT_FIND)->PostMessage(WM_CHAR, pMsg->wParam, pMsg->lParam);
			return TRUE;
		}
	}

	return CIBADialog::PreTranslateMessage(pMsg);
}

BOOL CDlgAllCheckOut::OnInitDialog()
{
	CIBADialog::OnInitDialog();

	

	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_NOSORTHEADER);
	m_listCtrl.SetItemsHeight(20);


	InitList();
	FillList();

	GetDlgItem(IDC_EDIT_FIND)->SetFont(&m_Font);
	GetDlgItem(IDC_STATIC_INFO)->SetFont(&m_Font);
	GetDlgItem(IDC_STATIC_RESULT)->SetFont(&m_Font);

	m_GridCtrl.ShowWindow(FALSE);

	return FALSE;  // return TRUE unless you set the focus to a control
}

HBRUSH CDlgAllCheckOut::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CIBADialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_INFO)
	{
		pDC->SetTextColor(RGB(0, 0, 255));
	}
	else if (pWnd->GetDlgCtrlID() == IDC_STATIC_RESULT)
	{
		pDC->SetTextColor(RGB(255, 0, 0));
	}

	return hbr;
}

void CDlgAllCheckOut::OnBnClickedButtonQuery()
{
	UpdateData();
	
	//��������
	if (m_strFind.IsEmpty())
	{
		ShowToolTip(IDS_INPUTERROR, IDC_EDIT_FIND);
		return;
	}

	if (m_strFind.GetAt(0) >= 65 || m_strFind.GetLength() < 4) //��ĸ��ͷ���߳���С��4
	{
		for (INT i = 0; i < m_GridCtrl.GetRowCount(); i++)
		{
			CBCGPGridRow* pRow = m_GridCtrl.GetRow(i);
			
			CString RowValue = pRow->GetItem(0)->GetValue();

			if (RowValue.GetLength() > m_strFind.GetLength())
			{
				RowValue = RowValue.Right(m_strFind.GetLength());
			}

			if (m_strFind.CompareNoCase(RowValue) == 0)
			{
				break;
			}
		}

		if (i < m_GridCtrl.GetRowCount())
		{
			m_GridCtrl.SetFocus();
			m_GridCtrl.EnsureVisible(m_GridCtrl.GetRow(i));
			m_GridCtrl.SetCurSel(m_GridCtrl.GetRow(i));

			m_listCtrl.SetSelectionMark(i);
			m_listCtrl.SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED,  LVIS_SELECTED | LVIS_FOCUSED);
			m_listCtrl.EnsureVisible(i,TRUE);
		}

		m_strFind.Empty();
		UpdateData(FALSE);

		return;
	}

	for (INT ii = 0; ii < m_GridCtrl.GetRowCount(); ii++)
	{
		CBCGPGridRow * pRow = m_GridCtrl.GetRow(ii);
		CString RowValue = pRow->GetItem(0)->GetValue();
		
		if (RowValue.GetLength() > m_strFind.GetLength())
		{
			RowValue = RowValue.Right(m_strFind.GetLength());
		}

		if (m_strFind.CompareNoCase(RowValue) == 0)
		{
			break;
		}
	}

	if (ii < m_GridCtrl.GetRowCount())
	{
		m_GridCtrl.SetFocus();
		m_GridCtrl.EnsureVisible(m_GridCtrl.GetRow(ii));
		m_GridCtrl.SetCurSel(m_GridCtrl.GetRow(ii));

		m_listCtrl.SetSelectionMark(ii);
		m_listCtrl.SetItemState(ii, LVIS_SELECTED | LVIS_FOCUSED,  LVIS_SELECTED | LVIS_FOCUSED);
		m_listCtrl.EnsureVisible(ii,TRUE);
	}

	m_strFind.Empty();

	UpdateData(FALSE);
}

void CDlgAllCheckOut::SelFirstOne()
{
	//////////////ѡ�е�һ��,���������˳�����Ҫ//////////
	m_GridCtrl.AdjustLayout();
	m_GridCtrl.SetFocus();

	CBCGPGridRow* pRow = m_GridCtrl.GetRow(0);

	if (pRow != NULL)
	{
		m_GridCtrl.EnsureVisible(pRow);
		m_GridCtrl.SetCurSel(pRow);
	}
}

HRESULT CDlgAllCheckOut::OnItemChanged(WPARAM wParam, LPARAM lParam)
{
	BCGPGRID_ITEM_INFO* pItemInfo = (BCGPGRID_ITEM_INFO*)lParam;

	/*COleVariant vTmp = pItemInfo->pItem->GetValue();
	
	CBCGPGridRow * pRow = m_GridCtrl.GetCurSel();

	UINT nReturnState = vTmp.boolVal == 0 ? 0 : 1;

	if (pRow != NULL)
	{
		CString strNetId = pRow->GetItem(0)->GetValue();

		for (INT i = 0; i < m_ReturnInfoArray.GetCount(); i++)
		{
			ReturnInfo & rInfo = m_ReturnInfoArray.GetAt(i);

			if (rInfo.bLocal)
			{
				if (rInfo.LocalInfo.GetSerialNum().CompareNoCase(strNetId) == 0)
				{
					CActiveMember ActiveMember;
					rInfo.bLocalReturned = CCashierHelper::ReturnLocalUser(ActiveMember,
						&rInfo.LocalInfo);
				}
			}
			else
			{
				if (rInfo.CheckOutInfo.GetSerialNum().CompareNoCase(strNetId) == 0)
				{
					rInfo.CheckOutInfo.SetReturnState(1);
					CActiveMember ActiveMember;
					if (!CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(rInfo.CheckOutInfo.GetMemberId(), ActiveMember) )
					{
						continue;
					}

					rInfo.bLocalReturned = CCashierHelper::ReturnLocalCenterUser(
						ActiveMember, &rInfo.CheckOutInfo);
				}
			}
			
		}
	}*/

	return 0;
}


BOOL CDlgAllCheckOut::SaveToDB(CCheckOutInfoArray& CheckOutArray)
{
	if (CheckOutArray.GetCount() <= 0)
	{
		return TRUE;
	}

	//2011-4-7-gxx
	CString strSQL;

	strSQL = _T("select * from checkoutinfo where 1=0");

	IBA_TRACE(strSQL);

	CADODBConnInfo* pDb = NULL;

	if (!CIBADAL::GetInstance()->GetDBConnInfo(pDb)) 
	{
		return FALSE;
	}

	CADORecordset Rs(pDb->GetConn());

	if (!Rs.Open(strSQL)) 
	{
		pDb->Release();
		return FALSE;
	}
	
	for (int i = 0 ; i < CheckOutArray.GetCount() ; i++ )
	{
		CCheckOutinfo& CheckOutinfo = CheckOutArray.GetAt(i);

		Rs.AddNew();

		Rs.SetFieldValue(_T("terminalId"),CheckOutinfo.GetTerminalId());
		Rs.SetFieldValue(_T("checkOutTime"),CheckOutinfo.GetCheckOutTime());
		Rs.SetFieldValue(_T("serialNum"), CheckOutinfo.GetSerialNum());
		Rs.SetFieldValue(_T("className"), CheckOutinfo.GetClassName());
		Rs.SetFieldValue(_T("memberId"), CheckOutinfo.GetMemberId());
		Rs.SetFieldValue(_T("classState"), CheckOutinfo.GetClassState());
		Rs.SetFieldValue(_T("guarantyMoney"), CheckOutinfo.GetGuarantyMoney());
		Rs.SetFieldValue(_T("balanceMoney"), CheckOutinfo.GetBalanceMoney());
		Rs.SetFieldValue(_T("userName"), CheckOutinfo.GetUserName());
		Rs.SetFieldValue(_T("personalId"), CheckOutinfo.GetPersonalId());
		Rs.SetFieldValue(_T("returnState"), CheckOutinfo.GetReturnState());

		Rs.Update();
	}
	
	Rs.Close();

	pDb->Release();

	return TRUE;
	//
}

void CDlgAllCheckOut::OnClose()
{
	//SaveToFile(m_CheckOutArray);

	CIBADialog::OnClose();
}
void CDlgAllCheckOut::OnGLCNotify(LPNMHDR lpHdr, LRESULT *pResult)
{
	NS_COMMON::LPNMHDR_GLC_CONTROL lpGLC = (NS_COMMON::LPNMHDR_GLC_CONTROL)lpHdr;
	*pResult = 0;

	if (lpGLC->nRowID != -1 && lpGLC->pCtrlHdr->code == BN_CLICKED)
	{
		CString strNetId = m_listCtrl.GetItemText(lpGLC->nRowID, 0);

		for (INT i = 0; i < m_ReturnInfoArray.GetCount(); i++)
		{
			ReturnInfo & rInfo = m_ReturnInfoArray.GetAt(i);

			if (rInfo.bLocal)
			{
				if (rInfo.LocalInfo.GetSerialNum().CompareNoCase(strNetId) == 0)
				{
					CActiveMember ActiveMember;
					ActiveMember.SetIsLocalUser(TRUE);
					rInfo.bLocalReturned = CCashierHelper::ReturnLocalUser(ActiveMember,
						&rInfo.LocalInfo);

					if (rInfo.bLocalReturned)
					{
						m_listCtrl.SetItemText(lpGLC->nRowID, 5, _T("���˿�"));
						m_listCtrl.SetItem(lpGLC->nRowID, 6, _T(""));
					}
				}
			}
			else
			{
				if (rInfo.CheckOutInfo.GetSerialNum().CompareNoCase(strNetId) == 0)
				{
					CReturnDlg dlg;

					dlg.SetCheckOutInfo(rInfo.CheckOutInfo);

					if (IDOK == dlg.DoModal())
					{
						rInfo.CheckOutInfo.SetReturnState(1);
						BOOL bResult = CIBADAL::GetInstance()->UpdateCheckOutInfo(strNetId, 1);
						if (bResult)
						{
							m_listCtrl.SetItemText(lpGLC->nRowID, 5, _T("���˿�"));
							m_listCtrl.SetItem(lpGLC->nRowID, 6, _T(""));
						}
					}
				}
			}
		}
	}
}
void CDlgAllCheckOut::OnBnClickedGirdButton()
{
	CBCGPGridRow * pRow = m_GridCtrl.GetCurSel();

	if (pRow != NULL)
	{
		CString strNetId = pRow->GetItem(0)->GetValue();

		for (INT i = 0; i < m_ReturnInfoArray.GetCount(); i++)
		{
			ReturnInfo & rInfo = m_ReturnInfoArray.GetAt(i);

			if (rInfo.bLocal)
			{
				if (rInfo.LocalInfo.GetSerialNum().CompareNoCase(strNetId) == 0)
				{
					CActiveMember ActiveMember;
					ActiveMember.SetIsLocalUser(TRUE);
					rInfo.bLocalReturned = CCashierHelper::ReturnLocalUser(ActiveMember,
						&rInfo.LocalInfo);

					if (rInfo.bLocalReturned)
					{
						try
						{
							pRow->GetItem(5)->SetValue(_T("���˿�"));
							CBCGPGridItem* pItem = new CBCGPGridItem(_T(""));
							pRow->ReplaceItem(6, pItem);
							m_GridCtrl.RedrawWindow();
							m_GridCtrl.UpdateWindow();
						}
						catch(...){}
					}
				}
			}
			else
			{
				if (rInfo.CheckOutInfo.GetSerialNum().CompareNoCase(strNetId) == 0)
				{
					CActiveMember *pActiveMember = NULL;
					for(int ar = 0 ; ar < arrMembers.GetCount(); ar++)
					{
						if (arrMembers.GetAt(ar).GetNetId().CompareNoCase(strNetId) == 0)
						{
							pActiveMember = &arrMembers.GetAt(ar);
						}
					}
					
					if (!pActiveMember)
					{
						continue;
					}
					
					rInfo.CheckOutInfo.SetReturnState(1);
					//BOOL bResult = CCashierHelper::ReturnLocalCenterUser(
					//	*pActiveMember, &rInfo.CheckOutInfo);
					BOOL bResult = CIBADAL::GetInstance()->UpdateCheckOutInfo(strNetId, 1);


					if (bResult)
					{
						try
						{
							pRow->GetItem(5)->SetValue(_T("���˿�"));
							CBCGPGridItem* pItem = new CBCGPGridItem(_T(""));
							pRow->ReplaceItem(6, pItem);
							m_GridCtrl.RedrawWindow();
							m_GridCtrl.UpdateWindow();
						}
						catch(...){}
					}
				}
			}
		}

	}
}

void CDlgAllCheckOut::OnBnClickedButtonPrint()
{
	m_GridCtrl.Print();
}


// ����-1��ʾ��Ч
int CDlgAllCheckOut::GetActiveMembers(CArray<CActiveMember>& ActiveMembers)
{
	BOOL bRet = FALSE;

	CString strSQL;

	strSQL = _T("select activemember.*, member.classId as userClassId from activemember");

	strSQL.Append(_T(" left outer join member on activemember.memberId = member.memberId where activemember.memberId <> 0"));

	IBA_TRACE(strSQL);

	CADODBConnInfo* pDb = NULL;

	if (!CIBADAL::GetInstance()->GetDBConnInfo(pDb)) return FALSE;

	CADORecordset Rs(pDb->GetConn());

	if (Rs.Open(strSQL)) 
	{
		bRet = TRUE;
	}
	else
	{
		pDb->Release();
		return FALSE;
	}

	for (UINT i = 0; i < Rs.GetRecordCount(); i++)
	{
		CActiveMember ActiveMember;

		CString strTmp; INT nTmp = 0;

		if (Rs.GetFieldValue(_T("memberId"), nTmp))
		{
			ActiveMember.SetMemberID(nTmp);
		}

		if (Rs.GetFieldValue(_T("netId"), strTmp))
		{
			ActiveMember.SetNetId(strTmp);
		}

		if (Rs.GetFieldValue(_T("userClassId"), nTmp))
		{
			ActiveMember.SetClassId(nTmp);
		}

		if (Rs.GetFieldValue(_T("name"), strTmp))
		{
			ActiveMember.SetUserName(strTmp);
		}

		if (Rs.GetFieldValue(_T("sex"), nTmp))
		{
			ActiveMember.SetSex(nTmp);
		}

		if (Rs.GetFieldValue(_T("idNumber"), strTmp))
		{
			ActiveMember.SetPersonalID(strTmp);
		}

		if (Rs.GetFieldValue(_T("idType"), nTmp))
		{
			ActiveMember.SetIdType(nTmp);
		}

		if (Rs.GetFieldValue(_T("lastRemainAmount"), nTmp))
		{
			ActiveMember.SetAvailavleBalance(nTmp);
		}

		if (Rs.GetFieldValue(_T("amount"), nTmp))
		{
			ActiveMember.SetAmount(nTmp);
		}

		if (Rs.GetFieldValue(_T("refNo"), nTmp))
		{
			ActiveMember.SetIsOnLine( nTmp > 0 );

			ActiveMember.SetRefNo(nTmp);
		}

		if (Rs.GetFieldValue(_T("status"), nTmp))
		{
			ActiveMember.SetStatus(nTmp);
		}

		Rs.GetFieldValue(_T("activationTime"), ActiveMember.SET(ActivationTime));

		Rs.GetFieldValue(_T("updateTime"), ActiveMember.SET(UpdateDataTime));

		Rs.GetFieldValue(_T("checkinTime"), ActiveMember.SET(CheckInTime));

		if (Rs.GetFieldValue(_T("termId"), strTmp))
		{
			ActiveMember.SetTerminalID(strTmp);
		}

		if (Rs.GetFieldValue(_T("pcClass"), nTmp))
		{
			ActiveMember.SetPCClassID(nTmp);
		}

		if (Rs.GetFieldValue(_T("isActive"), nTmp))
		{
			ActiveMember.SetActive(nTmp);
		}

		if (Rs.GetFieldValue(_T("payType"), nTmp))
		{
			ActiveMember.SetPayType(nTmp);
		}

		if (Rs.GetFieldValue(_T("advancePayment"), nTmp))
		{
			ActiveMember.SetAdvancePayment(nTmp);
		}

		if (Rs.GetFieldValue(_T("creditProcess"), strTmp))
		{
			ActiveMember.SetCreditProcess(strTmp);
		}

		// 2011-3-29-gxx-�������̨ID�Ķ�ȡ
		if (Rs.GetFieldValue(_T("lastNetBarId"), nTmp))
		{
			ActiveMember.SetCasherID(nTmp);
		}

		// 2011/05/13-gxx: ��ӱ�ע
		if (Rs.GetFieldValue(_T("Remark"), strTmp))
		{
			ActiveMember.SetRemark(strTmp);
		}

		// 2011/05/19-gxx: �����Ƿ�Ϊ��ʱ
		if (Rs.GetFieldValue(_T("registerBT"), nTmp))
		{
			ActiveMember.SetIsRegisterBT(nTmp);
		}

		// 2011/07/21-8201-gxx: {
		// �ۼƷⶥ��
		if (Rs.GetFieldValue(_T("dynamicMaxAmount"), nTmp))
		{
			ActiveMember.SetDynamicMaxAmount(nTmp);
		}

		// �Ƿ񱾵��û�
		if (Rs.GetFieldValue(_T("isLocal"), nTmp))
		{
			ActiveMember.SetIsLocalUser(nTmp);
		}

		// �ϻ�����, MD5ֵ
		if (Rs.GetFieldValue(_T("password"), strTmp))
		{
			ActiveMember.SetPassword(strTmp);
		}

		// ����
		if (Rs.GetFieldValue(_T("PolicyInfo"), strTmp))
		{
			ActiveMember.SetPolicyInfo(strTmp);
		}
		//}

		ActiveMembers.Add(ActiveMember);

		Rs.MoveNext();
	}

	IBA_TRACE("�û�����Ϊ:%d",Rs.GetRecordCount());

	Rs.Close();

	pDb->Release();

	//CLocalServer::GetInstance()->ActiveMemberList.Copy(ActiveMembers);

	return ActiveMembers.GetCount();
}