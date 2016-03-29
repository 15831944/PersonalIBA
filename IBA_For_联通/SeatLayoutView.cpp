// CSeatLayoutView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "AddTerminalDlg.h"
#include "Socket\LocalServer.h"
#include "Cashier.h"
#include ".\seatlayoutview.h"
#include "ComputerListView.h"

// CSeatLayoutView

IMPLEMENT_DYNCREATE(CSeatLayoutView, CScrollView)

CSeatLayoutView::CSeatLayoutView()
: m_bTotalView(false)
,m_pSeatBitmap(NULL)
,m_pSeatList(NULL)
,m_sizeTotal(0, 0)
,m_ptPrePoint(0, 0)
,m_bMoveRect(FALSE)
,m_nIndex(-1)
{
	m_CommandMode = M_SEAT_LOCK;
	m_pSeatBitmap = new CBitmap;

	m_pSeatList = new CSeatList;
	m_pSeatList->ReadList();

	m_sizeTotal.cx = 0;
	m_sizeTotal.cy = 0;
	m_bTotalView = FALSE;//Ĭ�Ϲ���

}

CSeatLayoutView::~CSeatLayoutView()
{
	if (m_pSeatBitmap)
	{
		m_pSeatBitmap->Detach();
		delete m_pSeatBitmap;
		m_pSeatBitmap = NULL;
	}

	if (m_pSeatList)
	{
		delete m_pSeatList;
		m_pSeatList = NULL;
	}
}

BEGIN_MESSAGE_MAP(CSeatLayoutView, CScrollView)
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_MENU_SCROLLZOOM, OnMenuScrollzoom)
	ON_UPDATE_COMMAND_UI(ID_MENU_SCROLLZOOM, OnUpdateMenuScrollzoom)
	ON_COMMAND(ID_MENU_FULLZOOM, OnMenuFullzoom)
	ON_UPDATE_COMMAND_UI(ID_MENU_FULLZOOM, OnUpdateMenuFullzoom)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_SEAT_LOCKSEAT, OnSeatLockseat)
	ON_UPDATE_COMMAND_UI(ID_SEAT_LOCKSEAT, OnUpdateSeatLockseat)
	ON_COMMAND(ID_SEAT_ADDSEAT, OnSeatAddseat)
	ON_UPDATE_COMMAND_UI(ID_SEAT_ADDSEAT, OnUpdateSeatAddseat)
	ON_COMMAND(ID_SEAT_LOADSEAT, OnSeatLoadseat)
	ON_UPDATE_COMMAND_UI(ID_SEAT_LOADSEAT, OnUpdateSeatLoadseat)
	ON_COMMAND(IDM_SEAT_DEL, OnSeatDel)
	ON_COMMAND(IDM_SEAT_REFRESH, OnSeatRefresh)
	ON_UPDATE_COMMAND_UI(IDM_SEAT_DEL, OnUpdateSeatDel)
	ON_UPDATE_COMMAND_UI(IDM_SEAT_REFRESH, OnUpdateSeatRefresh)
	ON_COMMAND(IDM_CLOCKWISE, OnClockwise)
	ON_UPDATE_COMMAND_UI(IDM_CLOCKWISE, OnUpdateClockwise)
	ON_WM_CLOSE()
	ON_WM_KILLFOCUS()
	ON_COMMAND(ID_SEAT_COMPUTER_MENU, OnSeatComputerMenu)
END_MESSAGE_MAP()


// CSeatLayoutView ��ͼ

void CSeatLayoutView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: �������ͼ�ĺϼƴ�С
	LoadSeatBitmap(m_pSeatBitmap);

	m_ToolTip.Create(this);
	m_ToolTip.SetColorBk(RGB(200, 200, 200), RGB(220, 220, 220), RGB(200, 200, 200));
	m_ToolTip.SetEffectBk(12);
	m_ToolTip.SetBorder(RGB(200, 200, 200));

}

// ��������ʾģʽ�µ�����ת���ɹ���ģʽ�µ�����
BOOL CSeatLayoutView::ToScrollRect(CRect& srcFullRect)
{
	if (!m_pSeatBitmap)
	{
		return FALSE;
	}

	BITMAP bmInfo;
	m_pSeatBitmap->GetObject(sizeof(bmInfo), &bmInfo);
	CRect WinRect;
	GetWindowRect(WinRect);
	double Xradios = (bmInfo.bmWidth) * 1.0 / WinRect.Width();
	double Yradios = (bmInfo.bmHeight) * 1.0 / WinRect.Height();

	srcFullRect.top *= Yradios;
	srcFullRect.bottom *= Yradios;
	srcFullRect.left *= Xradios;
	srcFullRect.right *= Xradios;

	return TRUE;
}

// ת��������ģʽ�µĵ�����
BOOL CSeatLayoutView::ToScrollPoint(CPoint& srcFullPoint)
{
	if (!m_pSeatBitmap)
	{
		return FALSE;
	}
	//////////////////////////�Ƚ���ƽ�Ʋ������ڽ��в��///////////////////////
	//srcFullPoint += GetScrollPosition();//���ۺ�����ʾģʽ���п��ܳ��ֹ���Ч��
	BITMAP bmInfo;
	m_pSeatBitmap->GetObject(sizeof(bmInfo), &bmInfo);
	CRect WinRect;
	GetWindowRect(WinRect);

	double Xradios = (bmInfo.bmWidth) * 1.000 / WinRect.Width();
	double Yradios = bmInfo.bmHeight * 1.000 / WinRect.Height();

	srcFullPoint.y *= Yradios;
	srcFullPoint.x *= Xradios;

	return TRUE;
}

BOOL CSeatLayoutView::ToTotalRect(CRect& srcTotalRect)
{
	if (!m_pSeatBitmap)
	{
		return FALSE;
	}
	
	BITMAP bmInfo;
	m_pSeatBitmap->GetObject(sizeof(bmInfo), &bmInfo);
	CRect WinRect;
	GetWindowRect(WinRect);
	double Xradios = WinRect.Width() * 1.0 / bmInfo.bmWidth;
	double Yradios = WinRect.Height() * 1.0 / bmInfo.bmHeight;

	srcTotalRect.top *= Yradios;
	srcTotalRect.bottom *= Yradios;
	srcTotalRect.left *= Xradios;
	srcTotalRect.right *= Xradios;

	return TRUE;
}

// ת��������ģʽ�µĵ�����
BOOL CSeatLayoutView::ToTotalPoint(CPoint& srcTotalPoint)
{
	if (!m_pSeatBitmap)
	{
		return FALSE;
	}

	BITMAP bmInfo;
	m_pSeatBitmap->GetObject(sizeof(bmInfo), &bmInfo);
	CRect WinRect;
	GetWindowRect(WinRect);

	double Xradios = WinRect.Width() * 1.0 / bmInfo.bmWidth;
	double Yradios = WinRect.Height() * 1.0 / bmInfo.bmHeight;

	srcTotalPoint.y *= Yradios;
	srcTotalPoint.x *= Xradios;

	return TRUE;
}

void CSeatLayoutView::OnDraw(CDC* pDC)
{
	//ͼƬ��������
	if ( !m_pSeatBitmap || !m_pSeatBitmap->m_hObject )
	{
		return;
	}
	//�ڴ�DC��ͼ
	CDC dcMemory;
	dcMemory.CreateCompatibleDC(pDC);
	CBitmap * pOldBitmap = dcMemory.SelectObject(m_pSeatBitmap);

	BITMAP bmInfo;
	m_pSeatBitmap->GetObject(sizeof(bmInfo), &bmInfo);
	
	if (m_bTotalView)//������������
	{
		CRect rect;
		GetClientRect(rect);

		pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &dcMemory, 0, 0, bmInfo.bmWidth, bmInfo.bmHeight, SRCCOPY);
		//*******�ֿ����������з��ּ���DC�޷�ˢ�������Σ�ԭ��δ��********/
		//����������
		double Xratio = rect.Width() / (double)(bmInfo.bmWidth);
		double Yratio = rect.Height() / (double)(bmInfo.bmHeight);
		DrawSeat(pDC, Xratio, Yratio);
	} 
	else//ԭ����������
	{
		pDC->BitBlt(0, 0, bmInfo.bmWidth, bmInfo.bmHeight, &dcMemory, 0, 0, SRCCOPY);
		//*******�ֿ����������з��ּ���DC�޷�ˢ�������Σ�ԭ��δ��********/
		//����������
		DrawSeat(pDC);
	}

	pDC->SelectObject(pOldBitmap);
	
}

// ������λͼ���ó�Ա�������ն���
void CSeatLayoutView::LoadSeatBitmap(CBitmap * pSeatBitmap)
{
	CString strPath;
	strPath = CIBAConfig::GetInstance()->GetSeatBmpPath();
	//����ͼ��
	HBITMAP hBitmap = (HBITMAP)::LoadImage(NULL, strPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (hBitmap != NULL)
	{
		m_pSeatBitmap->Detach();
		m_pSeatBitmap->Attach(hBitmap);

		SetViewSize(m_bTotalView);
	}
	else
	{
		m_pSeatBitmap->Detach();
		SetViewSize(m_bTotalView, TRUE);
	}
}

void CSeatLayoutView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	switch (m_CommandMode)
	{
	case M_SEAT_LOCK:
		{
			CMenu menu;
			menu.LoadMenu(IDR_POPUP_SEATZOOM);

			CMenu * pPopupMenu = menu.GetSubMenu(0);
			//����ʹ�õ����˵�����this��ǰ�������Ӧ��Ӧ����Ϣ
			theApp.GetContextMenuManager()->ShowPopupMenu(pPopupMenu->Detach(), point.x, point.y, this, TRUE);
			break;
		}
	case M_SEAT_ADD:
		{
			CPoint pt = point;
			ScreenToClient(&pt);
			pt = pt + GetScrollPosition();
			if (m_bTotalView)
			{
				ToScrollPoint(pt);
			}
			m_nIndex = m_pSeatList->InRect(pt);
			

			CMenu menu;
			menu.LoadMenu(IDR_POPUP_SEATEDIT);
			CMenu * pPopupMenu = menu.GetSubMenu(0);
			//����ʹ�õ����˵�����this��ǰ�������Ӧ��Ӧ����Ϣ
			theApp.GetContextMenuManager()->ShowPopupMenu(pPopupMenu->Detach(), point.x, point.y, this, TRUE);
			
			break;
		}
	case M_SEAT_SAVE:
		{
			break;
		}
	case M_SEAT_LOAD:
		{
			break;
		}
	case M_SEAT_COMPUTER_MENU:
		{
			CPoint pt = point;
			ScreenToClient(&pt);
			pt = pt + GetScrollPosition();
			if (m_bTotalView)
			{
				ToScrollPoint(pt);
			}
			m_nIndex = m_pSeatList->InRect(pt);

			if (m_nIndex >= 0)
			{//����Ԫ�����򣬼��ƶ�����
				CSeat * tSeat = m_pSeatList->GetAt(m_nIndex);
				CComputerInfo info = tSeat->GetComputerInfo();

				CString strTermID = tSeat->GetTerminalID();
				CComputerListView* pComputerView = CLocalServer::GetInstance()->GetComputerListView();
				pComputerView->FindComputer(0,strTermID);
			} 

			CMenu menu;
			menu.LoadMenu(IDR_COMPUTER_MENU);
			CMenu* pPopupMenu = menu.GetSubMenu(0);

			theApp.GetContextMenuManager()->ShowPopupMenu(
				pPopupMenu->Detach(),
				point.x, 
				point.y, 
				CLocalServer::GetInstance()->GetComputerListView(), 
				TRUE);

		}
		break;
	}
}

// ��ʾ����
void CSeatLayoutView::SetViewSize(BOOL bViewType, BOOL bDefault)
{
	if (!bDefault)
	{
		if (bViewType)//������ʾģʽ
		{
			CRect rect;
			GetClientRect(rect);
			//������������
			m_sizeTotal.cx = rect.Width();
			m_sizeTotal.cy = rect.Height();
		} 
		else//������ʾģʽ
		{
			//������������
			BITMAP bmInfo;
			m_pSeatBitmap->GetObject(sizeof(bmInfo), &bmInfo);
			m_sizeTotal.cx = bmInfo.bmWidth;
			m_sizeTotal.cy = bmInfo.bmHeight;
		}
	} 
	else
	{//Ĭ�ϴ���
		m_sizeTotal.cx = DefaultBitmapSize;
		m_sizeTotal.cy = DefaultBitmapSize;
	}
	//��ʾ��������
	SetScrollSizes(MM_TEXT, m_sizeTotal);
}

// �滭��λ������
void CSeatLayoutView::DrawSeat(CDC * pDC, double XRatio, double YRatio)
{
	BOOL bIsCreateDC = FALSE;
	if (!pDC)
	{
		pDC = GetDC();
		bIsCreateDC = TRUE;
	}
	/////////////////////////////////////////
	if (!m_pSeatList)
	{
		if (bIsCreateDC)
		{
			pDC->DeleteDC();
		}
		return;
	}
	m_pSeatList->DrawAllRect(pDC, XRatio, YRatio);
	///////////////////////////////////////////
	if (bIsCreateDC)
	{
		pDC->DeleteDC();
	}
}

void CSeatLayoutView::OnLButtonDown(UINT nFlags, CPoint point)
{
	switch (m_CommandMode)
	{
	case M_SEAT_LOCK:
		{
			break;
		}
	case M_SEAT_ADD:
		{
			//�����ƶ����Ӵ�����,������������Ϊ���ѹ�����������ͼ�ƶ�Ҳѹ��
			m_bMoveRect = FALSE;

			CPoint pt = point;
			pt = pt + GetScrollPosition();
			if (m_bTotalView)
			{
				ToScrollPoint(pt);
			}
			m_nIndex = m_pSeatList->InRect(pt);
			if (m_nIndex >= 0)
			{//����Ԫ�����򣬼��ƶ�����
				//SetCapture();
				m_ptPrePoint = m_pSeatList->GetAt(m_nIndex)->GetPosition();
			} 

			break;
		}
	case M_SEAT_SAVE:
		{
			break;
		}
	case M_SEAT_LOAD:
		{
			break;
		}
	}

	CScrollView::OnLButtonDown(nFlags, point);
}

void CSeatLayoutView::OnLButtonUp(UINT nFlags, CPoint point)
{
	switch (m_CommandMode)
	{
	case M_SEAT_LOCK:
		{
			//�����ƶ����Ӵ�����,������������Ϊ���ѹ�����������ͼ�ƶ�Ҳѹ��
			CPoint pt = point;
			pt = pt + GetScrollPosition();
			if (m_bTotalView)
			{
				ToScrollPoint(pt);
			}
			m_nIndex = m_pSeatList->InRect(pt);
			if (m_nIndex >= 0)
			{//����Ԫ�����򣬼��ƶ�����
				CSeat * tSeat = m_pSeatList->GetAt(m_nIndex);
				CComputerInfo info = tSeat->GetComputerInfo();

				CIBAString strToolTip, strTmp;
				strTmp.LoadString(ID_SEAT_TERMINALINFO);
				strToolTip.Format(strTmp, tSeat->GetTerminalID(), \
					tSeat->GetTerminalIP(),tSeat->GetTerminalIP(), \
					info.GetUserName(), info.GetSexAsString(), \
					info.GetMemberType(), info.GetCheckInTimeAsString(),\
					info.GetUseTimeAsString(), info.GetStatusAsString());
				
				CPoint pt = point;
				ClientToScreen(&pt);
				m_ToolTip.ShowHelpTooltip(&pt, strToolTip);
			} 
			break;
		}
	case M_SEAT_ADD:
		{
			if (!m_pSeatList)
			{
				break;
			} 
			
			CPoint pt = point;
			pt = pt + GetScrollPosition();
			if (m_bTotalView)
			{
				ToScrollPoint(pt);
			}

			if (m_nIndex >=0)
			{	
				if (m_bMoveRect)
				{
					//�ͷŽ���
// 					if (GetCapture() == this)
// 					{
// 						ReleaseCapture();
// 					}
					CSeat * tSeat = m_pSeatList->GetAt(m_nIndex);
					CString strTerminalID = tSeat->GetTerminalID();
					CString strTerminalIP = tSeat->GetTerminalIP();
					INT_PTR nDirection = tSeat->GetDirection();

					m_pSeatList->DeleteAt(m_nIndex);
					AddSeat(point, strTerminalID, strTerminalIP, nDirection);

					RedrawWindow();
					m_bMoveRect = FALSE;
					m_nIndex = -1;//ע��˴�������˫��ʱ�ᷢ��һ�ΰ��£����ε�����Ϣ�������
				}
			}
			else
			{
				if(!m_bMoveRect)
				{
					if (m_pSeatList->GetSize() < CLocalServer::GetInstance()->ComputerList.GetCount())
					{
						CDlgAddTerm addDlg;
						addDlg.SetExistSeatList(m_pSeatList);
						if (addDlg.DoModal() == IDOK)
						{
							CString strTerminalID, strTerminalIP;
							strTerminalID = addDlg.GetTerminalID();
							strTerminalIP = addDlg.GetTerminalIP();
							AddSeat(point, strTerminalID, strTerminalIP);
						}
					} 
					else
					{
						AfxMessageBox(_T("û���ն˿�����ӣ�"));
					}
				}
				m_nIndex = -1;//ע��˴�������˫��ʱ�ᷢ��һ�ΰ��£����ε�����Ϣ�������
			}

			m_ptPrePoint = CPoint(0, 0);//�������ݣ�Ϊ�ƶ��̻���
			
			break;
		}
	case M_SEAT_SAVE:
		{
			break;
		}
	case M_SEAT_LOAD:
		{
			break;
		}
	}
//////////////////////////////
	CScrollView::OnLButtonUp(nFlags, point);
}

void CSeatLayoutView::OnMouseMove(UINT nFlags, CPoint point)
{
	switch (m_CommandMode)
	{
	case M_SEAT_LOCK:
		{
			break;
		}
	case M_SEAT_ADD:
		{
			if (nFlags != MK_LBUTTON || m_ptPrePoint == CPoint(0, 0))
			{
				break;
			}

			if (m_pSeatList->GetSize() <= 0)
			{
				break;
			}

			m_bMoveRect = TRUE;

			CDC * pDC = GetDC();
			if (!pDC)
			{
				break;
			}

			if (m_nIndex >=0 )
			{
				//�����ƶ����Ӵ�����,������������Ϊ���ѹ�����������ͼ�ƶ�Ҳѹ��
				CPoint Curpt = point;
				Curpt = Curpt + GetScrollPosition();

				INT OldMode = pDC->SetROP2(R2_NOTXORPEN);

				CSeat * tSeat = m_pSeatList->GetAt(m_nIndex);
				if (tSeat)
				{
					if (m_bTotalView)
					{
						ToScrollPoint(Curpt);

						CRect rect(0, 0, 0, 0);
						GetWindowRect(rect);

						BITMAP bmInfo;
						m_pSeatBitmap->GetObject(sizeof(bmInfo), &bmInfo);

						double Xratio = rect.Width() / (double)(bmInfo.bmWidth);
						double Yratio = rect.Height() / (double)(bmInfo.bmHeight);

						tSeat->DrawRect(pDC, m_ptPrePoint - GetScrollPosition(), Xratio, Yratio);
						tSeat->DrawRect(pDC, Curpt - GetScrollPosition(), Xratio, Yratio);
					}
					else
					{
						tSeat->DrawRect(pDC, m_ptPrePoint - GetScrollPosition());
						tSeat->DrawRect(pDC, Curpt - GetScrollPosition());
					}
				}
				
				pDC->SetROP2(OldMode);

				m_ptPrePoint = Curpt;
			}

			ReleaseDC(pDC);
			break;
		}
	case M_SEAT_SAVE:
		{
			break;
		}
	case M_SEAT_LOAD:
		{
			break;
		}
	}

	CScrollView::OnMouseMove(nFlags, point);
}

void CSeatLayoutView::OnSeatLockseat()
{
	m_pSeatList->SaveList();
	m_CommandMode = M_SEAT_LOCK;
}

void CSeatLayoutView::OnUpdateSeatLockseat(CCmdUI *pCmdUI)
{
	if (theApp.GetCurCashier()->IsOnDuty())
	{
		pCmdUI->SetCheck(m_CommandMode == M_SEAT_LOCK);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CSeatLayoutView::OnSeatAddseat()
{
	m_CommandMode = M_SEAT_ADD;
}

void CSeatLayoutView::OnUpdateSeatAddseat(CCmdUI *pCmdUI)
{
	if (theApp.GetCurCashier()->IsOnDuty())
	{
		pCmdUI->SetCheck(m_CommandMode == M_SEAT_ADD);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CSeatLayoutView::OnSeatLoadseat()
{
	if (m_pSeatList)
	{
		m_pSeatList->ReadList();
		RedrawWindow();
	}
	m_CommandMode = M_SEAT_LOCK;
}

void CSeatLayoutView::OnUpdateSeatLoadseat(CCmdUI *pCmdUI)
{
	if (theApp.GetCurCashier()->IsOnDuty())
	{
		pCmdUI->SetCheck(m_CommandMode == M_SEAT_LOAD);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CSeatLayoutView::OnMenuScrollzoom()
{
	m_bTotalView = FALSE;
	SetViewSize(m_bTotalView);

	RedrawWindow();
}

void CSeatLayoutView::OnUpdateMenuScrollzoom(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!m_bTotalView);
}

void CSeatLayoutView::OnMenuFullzoom()
{
	m_bTotalView = TRUE;
	SetViewSize(m_bTotalView);

	RedrawWindow();
}

void CSeatLayoutView::OnUpdateMenuFullzoom(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bTotalView);
}
// �������������λ����
void CSeatLayoutView::AddSeat(CPoint ptCurpoint, CString strTerminalID, CString strTerminalIP, INT_PTR nDirection)
{
	//�����ƶ����Ӵ�����,������������Ϊ���ѹ�����������ͼ�ƶ�Ҳѹ��
	CPoint pt = ptCurpoint;
	pt = pt + GetScrollPosition();
	//������ͬģʽ�µ����꣬ȫ��ͳһ������ģʽ������
	if (m_bTotalView)
	{
		ToScrollPoint(pt);
	}

	CSeat * tSeat = new CSeat;
	if (tSeat)
	{
		tSeat->SetPosition(pt);
		tSeat->SetRadius(DefaultRadius);//�������õģ�������ʱ��ֵ
		tSeat->SetTerminalID(strTerminalID);
		tSeat->SetTerminalIP(strTerminalIP);
		tSeat->SetDirection(nDirection);//Ĭ�����
		m_pSeatList->Add(tSeat);
	}
	
	CRect rect(m_ptPrePoint, ptCurpoint);
	rect.NormalizeRect();
	rect.InflateRect(tSeat->GetRadius() * 2, tSeat->GetRadius() * 2);
	rect.NormalizeRect();

	RedrawWindow(&rect);
}

BOOL CSeatLayoutView::PreTranslateMessage(MSG* pMsg)
{
	m_ToolTip.RelayEvent(pMsg);

	return CScrollView::PreTranslateMessage(pMsg);
}

void CSeatLayoutView::OnSeatDel()
{
	if (m_nIndex >= 0)
	{
		m_pSeatList->DeleteAt(m_nIndex);
		m_nIndex = -1;
		RedrawWindow();
	} 
}

void CSeatLayoutView::OnSeatRefresh()
{
	RedrawWindow();
}

void CSeatLayoutView::OnUpdateSeatDel(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_nIndex >=0);
}

void CSeatLayoutView::OnUpdateSeatRefresh(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}

void CSeatLayoutView::OnClockwise()
{
	if (m_nIndex >=0)
	{
		CSeat * tSeat = m_pSeatList->GetAt(m_nIndex);
		if (!tSeat)
		{
			return;
		}

		if (tSeat->GetDirection() >= 7)
		{
			tSeat->SetDirection(0);
		}
		else
		{
			tSeat->SetDirection(tSeat->GetDirection() + 1);
		}
		m_nIndex = -1;
		RedrawWindow();
	}
}

void CSeatLayoutView::OnUpdateClockwise(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_nIndex >=0);
}

void CSeatLayoutView::OnClose()
{
	//m_bTotalView = FALSE;
	//m_ptPrePoint = CPoint(0, 0);
	//m_bMoveRect = FALSE;
	//m_nIndex = -1;
	
	CIBAConfig::GetInstance()->SetDisplayType(0);
	theApp.GetIBAView()->PostMessage(WM_CHILDCLOSE, GetDlgCtrlID(), (LPARAM)GetSafeHwnd());

	//CScrollView::OnClose();
}

void CSeatLayoutView::OnKillFocus(CWnd* pNewWnd)
{
	m_ToolTip.HideTooltip();

	CScrollView::OnKillFocus(pNewWnd);
}

void CSeatLayoutView::OnSeatComputerMenu()
{
	m_CommandMode = M_SEAT_COMPUTER_MENU;
}