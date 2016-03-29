
#include "stdafx.h"
#include "IBA.h"
#include "IBAConfig.h"
#include "IBAHelpper.h"
#include "Cashier.h"
#include "..\L2Markup\Markup.h"
#include ".\colorgridctrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//******************************************************************************

CIBAGrid::CIBAGrid()
{

}

CIBAGrid::~CIBAGrid()
{

}

BEGIN_MESSAGE_MAP(CIBAGrid, CBCGPGridCtrl)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

void CIBAGrid::OnLButtonUp(UINT nFlags, CPoint point)
{
	CBCGPGridCtrl::OnLButtonUp(nFlags, point);

	GetParent()->PostMessage(WM_LBUTTONUP);
}

void CIBAGrid::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CBCGPGridCtrl::OnLButtonDblClk(nFlags, point);

	GetParent()->PostMessage(WM_LBUTTONDBLCLK);
}

void CIBAGrid::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBCGPGridCtrl::OnKeyDown(nChar, nRepCnt, nFlags);

	if (VK_RETURN == nChar)
	{
		GetParent()->PostMessage(WM_LBUTTONDBLCLK);
	}
}

int CIBAGrid::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetReadOnly();//ֻ��
	SetWholeRowSel();//����ѡ��
	SetSingleSel(FALSE);//��ѡ
	EnableHeader(TRUE, 0);//�б�ͷʹ��

	// Set grid colors
	CBCGPGridColors colors;
	colors.m_LeftOffsetColors.m_clrBackground = globalData.clrWindow;
	colors.m_OddColors.m_clrBackground = RGB(238, 246, 255);// ���������еı���ɫ
	//colors.m_SelColors.m_clrBackground = RGB(50, 50, 180);

	SetColorTheme(colors);//��ɫ������
	
	SetScrollBarsStyle(CBCGPScrollBar::BCGP_SBSTYLE_VISUAL_MANAGER);//��������ʽ

	return 0;
}

int CIBAGrid::CompareMoneyString(CString left, CString right)const
{
	int nLeft = left.Find(_T('��'),0);
	int nRight = right.Find(_T('��'),0);
	if( nLeft < 0 || nRight <0 )
	{
		return left.Compare(right);
	}
	else
	{
		int nLI = 0;
		int nRI = 0;
		
		// ȥ������ҷ���
		left = left.Mid(nLeft+1,left.GetLength()-nLeft-1);
		right = right.Mid(nRight+1,right.GetLength()-nRight-1);

		// ����'.'֮ǰ�����ָ���
		for(int i = 0 ; i < left.GetLength() ; i ++ )
		{
			if( left[i] == _T('.') )
				break;
			nLI++;
		}
		for(int i = 0 ; i < right.GetLength() ; i ++ )
		{
			if( right[i] == _T('.') )
				break;
			nRI++;
		}

		if( nLI < nRI )
		{
			return -1;
		}
		else if( nLI > nRI )
		{
			return 1;
		}
		else
		{
			return left.Compare(right);
		}
	}
}

int CIBAGrid::CompareItems(const CBCGPGridItem* pItem1, const CBCGPGridItem* pItem2)const
{
	/*ASSERT_VALID (this);
	ASSERT_VALID (pItem1);
	ASSERT_VALID (pItem2);*/

	const _variant_t& var1 = pItem1->GetValue();
	const _variant_t& var2 = pItem2->GetValue();

	if (var1.vt == var2.vt)
	{
		switch (var1.vt)
		{
		case VT_BSTR:
			{
				return CompareMoneyString((const TCHAR*)var1.bstrVal,(const TCHAR*)var2.bstrVal);
			}
		}
	}

	return CBCGPGridCtrl::CompareItems(pItem1,pItem2);
}
/////////////////////////////////////////////////////////////////////////////
// CComputerGrid

CComputerGrid::CComputerGrid(CComputerList* lpComputerList)
: m_lpComputerList(lpComputerList)
{
	
}

CComputerGrid::~CComputerGrid()
{
}

BEGIN_MESSAGE_MAP(CComputerGrid, CIBAGrid)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CComputerGrid message handlers

int CComputerGrid::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CIBAGrid::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetRowMarker(FALSE);
	EnableHeader(TRUE, BCGP_GRID_HEADER_MOVE_ITEMS | BCGP_GRID_HEADER_SORT/* | BCGP_GRID_HEADER_HIDE_ITEMS*/);
	EnableMarkSortedColumn(FALSE, FALSE);//�б�ͷ��ʽ���Զ������ֹ
	
	if (CIBAConfig::GetInstance()->GetFontType() > 0)
	{
		CIBAHelpper::CreateFont(m_TextFont);
		SetFont(&m_TextFont);
	}

	InitColumnHead();

	SetTimer(TimeIdOfUpdateTime, IntervalOfUpdateTime, NULL);

	LoadState(_T("IBA"));//��ע�������
	AdjustLayout();

	return 0;
}

void CComputerGrid::OnDestroy()
{
	SaveState(_T("IBA"));

	CIBAGrid::OnDestroy();
}

void CComputerGrid::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case TimeIdOfUpdateTime : 
		{

		UpdateAllComputersUseTime();

		//�ϰ��������û���Ϊ��
		if (theApp.GetCurCashier()->IsOnDuty())
		{		
			CComputerList::CStatInfo StatInfo;
			m_lpComputerList->GetStatInfo(StatInfo);

			if (StatInfo.GetOnline() > 0)
			{
				SaveToFile();
			}
		}

		break;

		}

	}

	CIBAGrid::OnTimer(nIDEvent);
}

void CComputerGrid::Refresh(BOOL bRedrawAll)
{
	ASSERT(m_lpComputerList);

	if (bRedrawAll)
	{
		RedrawAllComputers();
	}
	else
	{
		RedrawUpdateComputers();
	}
	
	AdjustLayout();

	//Ҫ��Ҫ�Զ������أ�

	//AutoSort();
}

void CComputerGrid::InitColumnHead()
{
	//��������|�ն�״̬|�����˺�|�û�����|�ϻ�ʱ��|����ģʽ|�ۼ�ʱ��|�ۼ�����|�������|�û�����|֤������|����|IP��ַ|�ն˰汾|����
	
	CIBAString strTmp;
	strTmp.LoadString(IDS_COMPUTERLISTHEADER);

	CStringArray strArray;
	CIBAHelpper::SplitLine(strTmp, strArray);

	for (INT i = 0; i < strArray.GetCount(); i++)
	{
		InsertColumn(i, strArray.GetAt(i), 80);
	}

}

void CComputerGrid::UpdateRowData(CBCGPGridRow* pRow, CComputerInfo& computer)
{
	//��������|�ն�״̬|�����˺�|�û�����|
	pRow->GetItem(0)->SetValue(computer.GetTerminalID().GetString(), FALSE);
	pRow->GetItem(1)->SetValue(computer.GetStatusAsString().GetString(), FALSE);
	pRow->GetItem(2)->SetValue(computer.GetNetId().GetString(), FALSE);
	pRow->GetItem(3)->SetValue(computer.GetMemberType().GetString(), FALSE);

	//�ϻ�ʱ��|����ģʽ|�ۼ�ʱ��|�ۼ�����|
	pRow->GetItem(4)->SetValue(computer.GetCheckInTimeAsString().GetString(), FALSE);
	pRow->GetItem(5)->SetValue(computer.GetPayTypeAsString().GetString(), FALSE);
	pRow->GetItem(6)->SetValue(computer.GetUseTimeAsString().GetString(), FALSE);
	pRow->GetItem(7)->SetValue(computer.GetAmountAsString().GetString(), FALSE);
	
	//�������|�û�����|֤������|����|
	pRow->GetItem(8)->SetValue(computer.GetAvailavleBalanceAsString().GetString(), FALSE);

	CString strUserName = computer.GetUserName(TRUE);
	pRow->GetItem(9)->SetValue(CString(strUserName).GetString(), FALSE);

	pRow->GetItem(10)->SetValue(computer.GetPersonalID().GetString(), FALSE);
	pRow->GetItem(11)->SetValue(computer.GetPCClass().GetString(), FALSE);

	//IP��ַ|�ն˰汾|����
	pRow->GetItem(12)->SetValue(computer.GetComputerIP().GetString(), FALSE);
	pRow->GetItem(13)->SetValue(computer.GetClientVersion().GetString(), FALSE);
	pRow->GetItem(14)->SetValue(computer.GetLcation().GetString(), FALSE);

	//����״̬������ɫ
	COLORREF color = GetTextColor(computer);

	for (int i = 0; i < GetColumnCount(); i++)
	{
		pRow->GetItem(i)->SetTextColor(color, FALSE);
	}

	//��Ӹ����û��������ɫ
	//����������ߣ���ִ���������
	if (computer.GetComputerStatus() == CComputerInfo::ECS_ONLINE)  
	{
		UpdateRowUserType(pRow, computer);
	}
	//��ʱ����
	if (computer.GetPayType() > 0)//��ʱ��0Ϊ��ͨ��1Ϊ�ⶥ��2��Ԥ��
	{
		//���ݰ�ʱ״̬������ɫ
		COLORREF color = CIBAConfig::GetInstance()->GetBundTimeColor();

		for (int i = 0; i < GetColumnCount(); i++)
		{
			pRow->GetItem(i)->SetTextColor(color, FALSE);
		}
	}
}

void CComputerGrid::UpdateRowUserType(CBCGPGridRow* pRow, CComputerInfo& computer)
{
	CString strTmp;
	strTmp = computer.GetMemberType();

	for (INT i = 0; i < CNetBarConfig::GetInstance()->m_UserClassArray.GetCount(); i++ )
	{
		CString strClassName = CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(i).GetClassName();
		
		//���ü���������ʶ��ͬ�ļ���
		if (strTmp == strClassName)
		{	//���ü���ID��ʶ�������ļ�����ɫ
			UINT nClassID = CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(i).GetClassID();
			COLORREF color = CIBAConfig::GetInstance()->GetUserTypeColor(nClassID);
			//��������������
			for (INT j = 0; j < GetColumnCount(); j++)
			{
				pRow->GetItem(j)->SetTextColor(color, FALSE);
			}

			break;
		}
	}

}

COLORREF CComputerGrid::GetTextColor(CComputerInfo& computer)
{
	switch (computer.GetComputerStatus())
	{
	case CComputerInfo::ECS_OFFLINE : 
		return CIBAConfig::GetInstance()->GetOffLineColor();

	case CComputerInfo::ECS_ONLINE:
		return CIBAConfig::GetInstance()->GetOnLineColor();

	case CComputerInfo::ECS_LOCKED:
		return CIBAConfig::GetInstance()->GetLockScreenColor();

	case CComputerInfo::ECS_LITTERMONEY:
		return CIBAConfig::GetInstance()->GetLittleMoneyColor();

	case CComputerInfo::ECS_THIEF:
		return CIBAConfig::GetInstance()->GetThiefColor();
	
	case CComputerInfo::ECS_BIRTHDAY:
		return CIBAConfig::GetInstance()->GetBirthDayColor();
	
	case CComputerInfo::ECS_SUSPEND:
		return CIBAConfig::GetInstance()->GetSuspendColor();

	case CComputerInfo::ECS_BLACK:
		return CIBAConfig::GetInstance()->GetBlackColor();

	case CComputerInfo::ECS_VIP:
		return CIBAConfig::GetInstance()->GetVIPColor();

	case CComputerInfo::ECS_UNLOCK:
		return CIBAConfig::GetInstance()->GetUnLockColor();
	
	case CComputerInfo::ECS_CONFLICT:
		return CIBAConfig::GetInstance()->GetConflictColor();

	case CComputerInfo::ECS_NORECORD:
		return CIBAConfig::GetInstance()->GetNoRecordColor();

	default:
		return RGB(0, 0, 0);
	}
}

void CComputerGrid::UpdateComputerInfoToRow(DWORD_PTR dwData, CComputerInfo& computer)
{	
	computer.SetUpdateState(CComputerInfo::EUS_NORMAL);

	CBCGPGridRow* pRow = FindRowByData(dwData);

	if (pRow != NULL) //�Ѿ����ڵ���
	{
		if (CheckShowCondition(computer))
		{
			UpdateRowData(pRow, computer);
		}
		else
		{
			RemoveRow(pRow->GetRowId()); //�Ƴ�����
		}		
	}
	else //�������
	{
		if (CheckShowCondition(computer))
		{
			pRow = CreateRow(GetColumnCount());
			pRow->SetData(dwData);
			UpdateRowData(pRow, computer);
			AddRow(pRow, FALSE);
		}
	}
}

void CComputerGrid::UpdateAllComputersUseTime()
{
	for (INT_PTR i = 0; i < m_lpComputerList->GetCount(); i++)
	{
		CComputerInfo& computer = m_lpComputerList->GetComputer(i);

		CBCGPGridRow* pRow = FindRowByData(i);

		if (pRow != NULL)
		{
			pRow->GetItem(6)->SetValue(computer.GetUseTimeAsString().GetString());
		}
	}
}

void CComputerGrid::AutoSort()
{
	if (CIBAConfig::GetInstance()->GetEnmergencyVisible() && IsSortingMode())
	{	
		INT nCol = GetSortColumn();

		BOOL bAscending = (GetColumnsInfo().GetColumnState(nCol) == 1); //��������

		Sort(nCol, bAscending);
	}
}

void CComputerGrid::RedrawUpdateComputers()
{
	while (TRUE)
	{	//ȡ��Ҫˢ�µ��У�֮��ɾ����ˢ��Ҫ��
		INT_PTR nIndex = m_lpComputerList->GetUpdateComputer();

		if (nIndex >= 0)
		{
			CComputerInfo& computer = m_lpComputerList->GetComputer(nIndex);

			if (computer.GetUpdateState() != CComputerInfo::EUS_NORMAL)//��Ҫˢ��
			{
				UpdateComputerInfoToRow(nIndex, computer);
			}
		}
		else
		{
			break;
		}
	}
}

void CComputerGrid::RedrawAllComputers()
{
	RemoveAll();

	for (INT_PTR i = 0; i < m_lpComputerList->GetCount(); i++)
	{
		CComputerInfo& computer = m_lpComputerList->GetComputer(i);		
		UpdateComputerInfoToRow(i, computer);
	}
}

BOOL CComputerGrid::CheckShowCondition(CComputerInfo &computer)
{
	BOOL bRet = TRUE;

	if (!CIBAConfig::GetInstance()->GetShowAllComputer() 
		&&computer.GetComputerStatus() == CComputerInfo::ECS_OFFLINE
		|| !CheckShowZone(computer))
	{
		bRet = FALSE;
	}

	return bRet;
}

BOOL CComputerGrid::CheckShowZone(CComputerInfo &computer)
{
	BOOL bRet = TRUE;

	if (!CIBAConfig::GetInstance()->GetShowPCClassType(computer.GetPCClassID()))
	{
		bRet = FALSE;
	}

	return bRet;
}

void CComputerGrid::SaveToFile()
{
	CMarkup ActiveMembers;

	//ActiveMembers.SetDoc(_T("<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>\r\n"));
	
	ActiveMembers.AddElem(_T("ActiveMembers"));

	ActiveMembers.AddChildElem(_T("Operator"), theApp.GetCurCashier()->GetName());
	//ActiveMembers.AddChildElem(_T("Password"), theApp.GetCurCashier()->GetPassword());
	ActiveMembers.AddChildElem(_T("Time"), CIBAHelpper::GetCurrentTimeAsString(TRUE));

	for (INT_PTR i = 0; i < m_lpComputerList->GetCount(); i++)
	{
		CComputerInfo& computer = m_lpComputerList->GetComputer(i);

		if (computer.HasUserInfo())
		{
			ActiveMembers.AddChildElem(_T("MemberInfo"));
			ActiveMembers.IntoElem();

			ActiveMembers.AddChildElem(_T("TermId"), computer.GetTerminalID());
			ActiveMembers.AddChildElem(_T("TermIP"), computer.GetComputerIP());
			ActiveMembers.AddChildElem(_T("Status"), computer.GetStatusAsString());
			ActiveMembers.AddChildElem(_T("NetId"), computer.GetNetId());
			ActiveMembers.AddChildElem(_T("MemberId"), computer.GetMemberID());
			ActiveMembers.AddChildElem(_T("UserName"), computer.GetUserName());
			ActiveMembers.AddChildElem(_T("Sex"), computer.GetSex());
			ActiveMembers.AddChildElem(_T("IdType"), computer.GetIdType());
			ActiveMembers.AddChildElem(_T("PersonalId"), computer.GetPersonalID());
			ActiveMembers.AddChildElem(_T("ClassName"), computer.GetMemberType());
			ActiveMembers.AddChildElem(_T("ClassId"), computer.GetUserClassId());
			ActiveMembers.AddChildElem(_T("IsMember"), computer.IsMember());
			ActiveMembers.AddChildElem(_T("AvailavleBalance"), computer.GetAvailavleBalance());
			ActiveMembers.AddChildElem(_T("Amount"), computer.GetAmount());
			ActiveMembers.AddChildElem(_T("PayType"), computer.GetPayType());
			ActiveMembers.AddChildElem(_T("Lcation"), computer.GetLcation());
			ActiveMembers.AddChildElem(_T("CheckInTime"), computer.GetCheckInTime().Format(_T("%Y-%m-%d %H:%M:%S")));
			ActiveMembers.AddChildElem(_T("UseTime"), computer.GetUseTimeAsString());
			ActiveMembers.OutOfElem();
		}
	}

	ActiveMembers.Save(theApp.GetWorkPath() +  _T("\\IBAConfig\\ActiveMembers.xml"));
}


void CComputerGrid::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBCGPGridCtrl::OnKeyDown(nChar, nRepCnt, nFlags);

	if (VK_RETURN == nChar)
	{
		if (CIBAConfig::GetInstance()->GetOpenAfterEnter())
		{
			GetParent()->PostMessage(WM_LBUTTONDBLCLK);
		}
	}
}
