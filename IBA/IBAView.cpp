
#include "stdafx.h"
#include "IBA.h"
#include "ComputerListView.h"
#include "ActiveMemberView.h"
#include "NDTermConsumeView.h"
#include "OperationView.h"
#include "ShiftCashView.h"
#include "ComsumeListView.h"
#include "SeatLayoutView.h"
#include "Cashier.h"
#include "shortmsgview.h"
#include "cardopenconsumedlg.h"
#include ".\IBAView.h"

#include "DodonewView.h"
#include "CurrentNetBarUser.h"
#include "NDCreditDlg.h"
#include "NDReturnDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CIBAView

IMPLEMENT_DYNCREATE(CIBAView, CBCGPTabView)

BEGIN_MESSAGE_MAP(CIBAView, CBCGPTabView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_CHILDCLOSE, OnChildClose)
	ON_MESSAGE(WM_UPDATESEAT, OnUpdateSeat)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIBAView construction/destruction

CIBAView::CIBAView()
: m_hTermConsumeView(NULL)
, m_hSeatView(NULL)
, m_hShortMsgView(NULL)
, m_hShiftCashView(NULL)
, m_hCardOpenConsumeView(NULL)
, m_hComsumeListView(NULL)
, m_hOperationView(NULL)
, m_hDodonewView(NULL) // 2011/05/06-gxx: 
{

}

CIBAView::~CIBAView()
{
}


CIBADoc* CIBAView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIBADoc)));
	return (CIBADoc*)m_pDocument;
}

void CIBAView::OnInitialUpdate()
{
	CBCGPTabView::OnInitialUpdate();
}

int CIBAView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create tabs window:
	if (!m_wndTabs.Create(CBCGPTabWnd::STYLE_3D_ONENOTE, rectDummy, this, 1))
	{
		TRACE0("Failed to create tab window\n");
		return -1;      // fail to create
	}

	theApp.SetIBAView(this);

	m_wndTabs.SetFlatFrame();
	m_wndTabs.SetTabBorderSize(0);
	m_wndTabs.AutoDestroyWindow(FALSE);
	m_wndTabs.EnableTabSwap(FALSE);
	
	CIBAString strTmp;
	strTmp.LoadString(IDS_COMPUTERLIST);
	INT nIdx = AddView(RUNTIME_CLASS(CComputerListView), strTmp);

	strTmp.LoadString(IDS_ACTIVEMEMBERVIEW);
	nIdx = AddView(RUNTIME_CLASS(CActiveMemberView), strTmp);

	strTmp.LoadString(IDS_NETBARSEAT);
	nIdx = AddView(RUNTIME_CLASS(CSeatLayoutView),strTmp);
	//ASSERT(nIdx == NETBARSEAT_VIEW);
	m_hSeatView = GetTabControl().GetTabWnd(nIdx)->GetSafeHwnd();
	if(CIBAConfig::GetInstance()->GetDisplayType())
	{
		GetTabControl().ShowTab((nIdx,TRUE));
	}
	else
	{
		GetTabControl().ShowTab(nIdx,FALSE);
	}



	//strTmp.LoadString(IDS_OPERATIONQUERY);
	//nIdx = AddView(RUNTIME_CLASS(COperationView), strTmp);
	//GetTabControl().ShowTab(nIdx, FALSE);
	//m_hOperationView = GetTabControl().GetTabWnd(nIdx)->GetSafeHwnd();

	//strTmp.LoadString(IDS_SHIFICASHQUERY);
	//nIdx = AddView(RUNTIME_CLASS(CShiftCashView), strTmp);
	//GetTabControl().ShowTab(nIdx, FALSE);
	//m_hShiftCashView = GetTabControl().GetTabWnd(nIdx)->GetSafeHwnd();

	//strTmp.LoadString(IDS_CONSUMELISTQUERY);
	//nIdx = AddView(RUNTIME_CLASS(CComsumeListView), strTmp);
	//GetTabControl().ShowTab(nIdx, FALSE);
	//m_hComsumeListView = GetTabControl().GetTabWnd(nIdx)->GetSafeHwnd();

	//strTmp.LoadString(IDS_TERMCONSUME);
	//nIdx = AddView(RUNTIME_CLASS(CNDTermConsumeView), strTmp);
	//GetTabControl().ShowTab(nIdx, FALSE);
	//m_hTermConsumeView = GetTabControl().GetTabWnd(nIdx)->GetSafeHwnd();

	//strTmp.LoadString(IDM_SHORTMSGQUERY);
	//nIdx = AddView(RUNTIME_CLASS(CShortMsgView), strTmp);
	//GetTabControl().ShowTab(nIdx, FALSE);
	//m_hShortMsgView = GetTabControl().GetTabWnd(nIdx)->GetSafeHwnd();

	//strTmp.LoadString(IDS_CARDOPENCONSUME);
	//nIdx = AddView(RUNTIME_CLASS(CCardopenConsumeView), strTmp);
	//GetTabControl().ShowTab(nIdx, FALSE);
	//m_hCardOpenConsumeView = GetTabControl().GetTabWnd(nIdx)->GetSafeHwnd();



	//if (FALSE /*CIBAConfig::GetInstance()->GetDisplayType()*/)
	//{
	//	//this->ShowSeatLayoutView();
	//
	//	strTmp.LoadString(IDS_NETBARSEAT);
	//	nIdx = AddView(RUNTIME_CLASS(CSeatLayoutView), strTmp);
	//	m_hSeatView = GetTabControl().GetTabWnd(nIdx)->GetSafeHwnd();
	//	//GetTabControl().ShowTab(nIdx, TRUE);//��ʾ��λ��ͼ
	//} 



	INT nTmp = theApp.GetInt(_T("CurrentView"), 1);
	GetTabControl().EnsureVisible(nTmp);
	SetActiveView(nTmp);

	return 0;
}

BOOL CIBAView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CIBAView::OnActivateView(CView* pView)
{
	if (GetTabControl().GetActiveTab() < 2)
	{
		theApp.WriteInt(_T("CurrentView"), GetTabControl().GetActiveTab());

		m_wndTabs.EnableActiveTabCloseButton(FALSE);

		//�ر�������ʾ
		::PostMessage(m_hComsumeListView, WM_KILLFOCUS, NULL, NULL);
		::PostMessage(m_hTermConsumeView, WM_KILLFOCUS, NULL, NULL);
		::PostMessage(m_hOperationView, WM_KILLFOCUS, NULL, NULL);
		::PostMessage(m_hShiftCashView, WM_KILLFOCUS, NULL, NULL);
		::PostMessage(m_hSeatView, WM_KILLFOCUS, NULL, NULL);
	}
	else
	{
		m_wndTabs.EnableActiveTabCloseButton();
	}
}

void CIBAView::ShowTermConsumeView()
{
	INT nTabId = GetTabControl().GetTabFromHwnd(m_hTermConsumeView);

	if (nTabId < 0)
	{
		CIBAString strTmp;
		strTmp.LoadString(IDS_TERMCONSUME);
		nTabId = AddView(RUNTIME_CLASS(CNDTermConsumeView), strTmp);
		m_hTermConsumeView = GetTabControl().GetTabWnd(nTabId)->GetSafeHwnd();
		((CView*)GetTabControl().GetTabWnd(nTabId))->OnInitialUpdate();
	}

	GetTabControl().ShowTab(nTabId, TRUE);
	GetTabControl().EnsureVisible(nTabId);
	SetActiveView(nTabId);
}

void CIBAView::QueryTermConsumeList(LPCTSTR lpszTermId)
{
	ShowTermConsumeView();
	((CNDTermConsumeView*)GetActiveView())->QueryTermId(lpszTermId);
}

void CIBAView::ShowComsumeListView()
{
	INT nTabId = GetTabControl().GetTabFromHwnd(m_hComsumeListView);

	if (nTabId < 0)
	{
		CIBAString strTmp;
		strTmp.LoadString(IDS_CONSUMELISTQUERY);
		nTabId = AddView(RUNTIME_CLASS(CComsumeListView), strTmp);
		m_hComsumeListView = GetTabControl().GetTabWnd(nTabId)->GetSafeHwnd();
		((CView*)GetTabControl().GetTabWnd(nTabId))->OnInitialUpdate();
	}

	GetTabControl().ShowTab(nTabId, TRUE);
	GetTabControl().EnsureVisible(nTabId);
	SetActiveView(nTabId);
}

void CIBAView::QueryUserComsumeList(LPCTSTR lpszNetId, UINT nMemberId /*= 0*/)
{
	ShowComsumeListView();
	((CComsumeListView*)GetActiveView())->QueryUserConsumeList(lpszNetId, nMemberId);
}

void CIBAView::ShowOperationView()
{
	INT nTabId = GetTabControl().GetTabFromHwnd(m_hOperationView);

	if (nTabId < 0)
	{
		CIBAString strTmp;
		strTmp.LoadString(IDS_OPERATIONQUERY);
		nTabId = AddView(RUNTIME_CLASS(COperationView), strTmp);
		m_hOperationView = GetTabControl().GetTabWnd(nTabId)->GetSafeHwnd();
		((CView*)GetTabControl().GetTabWnd(nTabId))->OnInitialUpdate();
	}

	GetTabControl().ShowTab(nTabId, TRUE);
	GetTabControl().EnsureVisible(nTabId);
	SetActiveView(nTabId);
}

void CIBAView::ShowShiftCashView()
{	
	//ȡ�ö�Ӧ��������
	INT nTabId = GetTabControl().GetTabFromHwnd(m_hShiftCashView);

	if (nTabId < 0)
	{
		CIBAString strTmp;
		strTmp.LoadString(IDS_SHIFICASHQUERY);
		nTabId = AddView(RUNTIME_CLASS(CShiftCashView), strTmp);
		m_hShiftCashView = GetTabControl().GetTabWnd(nTabId)->GetSafeHwnd();
		((CView*)GetTabControl().GetTabWnd(nTabId))->OnInitialUpdate();
	}

	GetTabControl().ShowTab(nTabId, TRUE);
	GetTabControl().EnsureVisible(nTabId);
	SetActiveView(nTabId);
}

void CIBAView::ShowSeatLayoutView()
{
	INT nTabId = GetTabControl().GetTabFromHwnd(m_hSeatView);

	if (nTabId < 0)//�������
	{	
		CIBAString strTmp;
		strTmp.LoadString(IDS_NETBARSEAT);
		nTabId = AddView(RUNTIME_CLASS(CSeatLayoutView), strTmp);
		m_hShiftCashView = GetTabControl().GetTabWnd(nTabId)->GetSafeHwnd();
		((CView*)GetTabControl().GetTabWnd(nTabId))->OnInitialUpdate();
		//yajun-temp-0819
		CLocalServer::GetInstance()->LocalGetComputerList();
	}
	if(1 == CIBAConfig::GetInstance()->GetDisplayType())
	{
		GetTabControl().ShowTab(nTabId, FALSE);
		CIBAConfig::GetInstance()->SetDisplayType(0);
	}
	else
	{
		GetTabControl().ShowTab(nTabId, TRUE);
		GetTabControl().EnsureVisible(nTabId);
		SetActiveView(nTabId);

		CIBAConfig::GetInstance()->SetDisplayType(1);
	}
}

BOOL CIBAView::FindComputer(UINT nKeyType, LPCTSTR lpszKeyValue)
{
	return ((CComputerListView*)(GetTabControl().GetTabWnd(0)))->FindComputer(nKeyType, lpszKeyValue);
}

// ��ʾ�ͻ�����Ϣ
void CIBAView::ShowShortMsgView(void)
{
	//ȡ�ö�Ӧ��������
	INT nTabId = GetTabControl().GetTabFromHwnd(m_hShortMsgView);

	if (nTabId < 0)
	{
		CIBAString strTmp;
		strTmp.LoadString(IDS_CLIENTINFO);
		nTabId = AddView(RUNTIME_CLASS(CShortMsgView), strTmp);
		m_hShiftCashView = GetTabControl().GetTabWnd(nTabId)->GetSafeHwnd();
		((CView*)GetTabControl().GetTabWnd(nTabId))->OnInitialUpdate();
	}

	GetTabControl().ShowTab(nTabId, TRUE);
	GetTabControl().EnsureVisible(nTabId);
	SetActiveView(nTabId);
}

void CIBAView::QueryShortMsgList()
{
	ShowShortMsgView();
	((CShortMsgView*)GetActiveView())->QueryShortMsgList();
}
// ����δ���Ѳ�ѯ��ʾ
void CIBAView::ShowCardOpenConsumeView(void)
{
	INT nTabId = GetTabControl().GetTabFromHwnd(m_hCardOpenConsumeView);

	if (nTabId < 0)
	{
		CIBAString strTmp;
		strTmp.LoadString(IDS_CARDOPENCONSUME);
		nTabId = AddView(RUNTIME_CLASS(CCardopenConsumeView), strTmp);
		m_hCardOpenConsumeView= GetTabControl().GetTabWnd(nTabId)->GetSafeHwnd();
		((CView*)GetTabControl().GetTabWnd(nTabId))->OnInitialUpdate();
	}

	GetTabControl().ShowTab(nTabId, TRUE);
	GetTabControl().EnsureVisible(nTabId);
	SetActiveView(nTabId);
}

LRESULT CIBAView::OnChildClose(WPARAM wp, LPARAM lp)
{
	INT nTabId = GetTabControl().GetTabFromHwnd((HWND)lp);

	if (nTabId >= 0)
	{
		GetTabControl().ShowTab(nTabId, FALSE);
		RemoveView(nTabId);
	}

	return 0;
}

void CIBAView::ShowDodonewView()
{
	INT nTabId = GetTabControl().GetTabFromHwnd(m_hDodonewView);

	if (nTabId < 0)
	{
		CIBAString strLabel;
		strLabel.LoadString(IDS_ZENGSHOUBAO);
		nTabId = AddView(RUNTIME_CLASS(CDodonewView), strLabel);
		m_wndTabs.AutoDestroyWindow(TRUE);
		m_hDodonewView = GetTabControl().GetTabWnd(nTabId)->GetSafeHwnd();
		((CView*)GetTabControl().GetTabWnd(nTabId))->OnInitialUpdate();
	}
	else
	{
		((CDodonewView*)GetTabControl().GetTabWnd(nTabId))->Reflash();
	}

	GetTabControl().ShowTab(nTabId, TRUE);
	GetTabControl().EnsureVisible(nTabId);
	SetActiveView(nTabId);
}

CActiveMemberView* CIBAView::GetActiveMemberView()
{
	ASSERT( IsWindow(m_hWnd) );

	CActiveMemberView *pMemberView = (CActiveMemberView*)GetTabControl().
		GetTabWnd(ACTIVEMEMBER_VIEW);

	ASSERT( pMemberView );
	ASSERT( pMemberView->IsKindOf( RUNTIME_CLASS(CActiveMemberView) ) );

	return pMemberView;
}

CComputerListView* CIBAView::GetComputerListView()
{
	ASSERT( IsWindow(m_hWnd) );

	CComputerListView *pComputerView = (CComputerListView*)GetTabControl().
		GetTabWnd(COMPUTERLIST_VIEW);

	ASSERT( pComputerView );
	ASSERT( pComputerView->IsKindOf( RUNTIME_CLASS(CComputerListView) ) );

	return pComputerView;
}

//�жϵ�ǰ������Ƿ������ͼ
BOOL CIBAView::IsCurrentComputerView()
{
	CView * pView = GetActiveView();
	ASSERT(pView);

	if (pView->IsKindOf(RUNTIME_CLASS(CComputerListView)))
	{
		return TRUE;
	}

	return FALSE;
}

/*************************************************
Function:       // GotoActiveMember
Description:    // ʹ�ò���strNetIdƥ���û���ͼ�б��Ѽ����б������λ��ƥ�䵽���û����ڼ���Map������������ʾ�û���Ϣ
				// ����ģʽ�£������˿��
				// ����ģʽ�£����Ĳ�ѯ����ѯʧ�ܷ���FALSE����ѯ�ɹ������˿�ڡ�
Calls:          // FindUserAt,SetSelUser,QueryBalance
Table Accessed: // None
Table Updated:  // None
Input:          // strNetId - �����ҵ��ն�Id(�����λ�ַ�)
                // bActiveView - �Ƿ񼤻��û���ͼ
Output:         // None
Return:         // ����ģʽ�£���ѯ���ʧ�ܷ���FALSE����������TRUE
Others:         // ����δ������û������κζ������������б��в����û��൱�ڲ���ȫƥ�䡣
*************************************************/
BOOL CIBAView::GotoActiveMember(CString strNetId,BOOL bActiveView)
{
	if (bActiveView)
	{
		GetTabControl().SetActiveTab(ACTIVEMEMBER_VIEW);
	}
	
	CActiveMemberView *pMemberView = GetActiveMemberView();

	//���û���ͼ�б���ʹ�����λ����ƥ����û���index
	//����Ϊ�û���ͼ�ĵ�ǰѡ���У���Map�в��Ҷ�Ӧ�����ҵ�������������ʾ���û���Ϣ��������CCurrentNetBarUser��NetId,����return false
	int nIndex = pMemberView->FindUserAt(0,strNetId);//0��ʾֻƥ�����λ,��ֻ���û�NetId�а���strNetId����
	if(nIndex < 0)
	{
		return FALSE;
	}
	pMemberView->SetSelUser(nIndex, TRUE);

	CActiveMember ActiveMember;

	//���û�δ���ֱ�ӷ���FALSE
	//����ģʽ��,�����˿��
	//����ģʽ��,�������˿��

	if (!CLocalServer::GetInstance()->ActiveMemberList.GetActiveMemberNetID(strNetId, ActiveMember, FALSE))
	{
		theApp.IBAMsgBox(IDS_HASNOTUSED);
		return FALSE;
	}
	else
	{
		//NormalMode
		if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkNormal)
		{
			if(!CCurrentNetBarUser::GetInstance()->QueryBalance(ActiveMember.GetNetId()))
			{
				return FALSE;
			}
			else
			{
				CNDReturnDlg dlg;
				dlg.SetNetId(ActiveMember.GetNetId());
				dlg.DoModal();
				//if(CCurrentNetBarUser::GetInstance()->Balance.IsNetbarMember())  //��Ա������ֵ����
				//{	
    //                CNDCreditDlg dlg;
				//	dlg.SetNetId(ActiveMember.GetNetId());
				//	dlg.DoModal();
				//}
				//else                                                              //��ʱ�û������˿����
				//{
				//	CNDReturnDlg dlg;
				//	dlg.SetNetId(ActiveMember.GetNetId());
				//	dlg.DoModal();
				//}
			}
		}
		//LocalMode
		else
		{
			CNDReturnDlg dlgReturn;
			dlgReturn.SetNetId(ActiveMember.GetNetId());
			dlgReturn.DoModal();
		}
	}
	return TRUE;
}

/*************************************************
Function:         // GotoActiveMemberEx
Description:      // ʹ���ն˺Ų���ȫƥ���ڼ����б��в����û���ѡ�и��û�������������ʾ�û���Ϣ��������ֵ���˿
Calls:            // GetActiveMemberBest,GotoActiveMember
Table Accessed:   // None
Table Updated:    // None
Input:            // strTermID - �������Ҷ�Ӧ�û����ն˺�
Output:           // pBestTernID - ��������ˣ�����������ʵ��TerminalID(strTermID������ƥ�䵽�Ĳ���ȫ��ͬ)
Return:           // δƥ�䵽������ģʽ��������DoBalanceʧ�ܷ���FALSE����������TRUE
Others:           // ʹ��GetActiveMemberBest����ʹ�ò���ȫƥ��,ͬʱ�������ѡ�����ƥ��.
*************************************************/
BOOL CIBAView::GotoActiveMemberEx(CString strTermID, CString* pBestTernID /*=NULL*/)
{
	// �����ն˶�Ӧ���û�
	CActiveMember ActiveMember;
	//��ActiveMemberMap�и���strTermId�������ƥ���Ԫ�أ���ʹ����ȫƥ�䡣
	BOOL bFound = CLocalServer::GetInstance()->ActiveMemberList.GetActiveMemberBest(strTermID,ActiveMember, FALSE);

	if (bFound)
	{
		if (pBestTernID)
		{
			*pBestTernID = ActiveMember.GetTerminalID();
		}
		return GotoActiveMember(ActiveMember.GetNetId(), FALSE);//ѡ�и��û�������������ʾ�û���Ϣ�����˿
	}
	else
	{
		return FALSE;
	}
}

//��ComputerList�в��Ҷ�Ӧ��TermID
void CIBAView::GotoComputer(CString strTermID, BOOL bActiveView)
{
	if (strTermID.IsEmpty())
	{
		return;
	}
	CComputerListView *pComputerView = GetComputerListView();

	if (pComputerView->FindComputer(0,strTermID) && bActiveView)
	{
		GetTabControl().SetActiveTab(COMPUTERLIST_VIEW);
	}
}

void CIBAView::GotoComputerByIP(CString strIP, BOOL bActiveView)
{
	CComputerListView *pComputerView = GetComputerListView();
	if (pComputerView->FindComputer(2,strIP) && bActiveView)
	{
		GetTabControl().SetActiveTab(COMPUTERLIST_VIEW);
	}
}

void CIBAView::GotoComputerByNetID(CString strNetId, BOOL bActiveView)
{
	CComputerListView *pComputerView = GetComputerListView();
	if (pComputerView->FindComputer(1,strNetId) && bActiveView)
	{
		GetTabControl().SetActiveTab(COMPUTERLIST_VIEW);
	}
}

LRESULT CIBAView::OnUpdateSeat(WPARAM wParam, LPARAM lParam)
{
	return ::SendMessage(m_hSeatView, WM_UPDATESEAT, wParam, lParam);
}
