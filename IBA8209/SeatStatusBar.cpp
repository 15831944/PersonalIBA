
#include "stdafx.h"
#include "IBA.h"
#include "IBAHelpper.h"
#include "CreditDlg.h"
#include "ServletHellper.h"
#include "Dal\IBADAL.h"
#include "SeatStatusBar.h"
#include "IBACommDefine.h"
#include ".\seatstatusbar.h"
#include "..\IBARes\resource.h"

using namespace NS_IBACommDef;
//#ifdef _DEBUG
//#define new DEBUG_NEW		// ��GDI+�� new Gdiplus::Bitmap��ͻ
//#endif

/////////////////////////////////////////////////////////////////////////////
// CSeatStatusBar

BEGIN_MESSAGE_MAP(CSeatStatusBar, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeatStatusBar construction/destruction

CSeatStatusBar::CSeatStatusBar()
{
	// TODO: add one-time construction code here
	//m_ptrSeatBitmap->Load(theApp.GetWorkPath() + _T("\\IBAConfig\\STATUSTIPS.bmp"));
	HBITMAP bmp = LoadBitmap(theApp.GetResHandle(), MAKEINTRESOURCE(IDB_STATUSTIPS));
	m_pSeatBitmap = new Gdiplus::Bitmap(bmp, NULL);
	m_pSeatBitmap->FromHBITMAP(bmp, NULL);
}

CSeatStatusBar::~CSeatStatusBar()
{
	if(m_pSeatBitmap)
	{
		delete m_pSeatBitmap;
		m_pSeatBitmap = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSeatStatusBar message handlers
//����
int CSeatStatusBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	EnableGripper(FALSE);//����ʾ����CAPTION
	return 0;
}

void CSeatStatusBar::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: �ڴ˴������Ϣ����������
	// 2014-2-14 - qsc �����κδ���Ŀ���ǲ���Ĭ�ϵ��Ҽ��˵�����
}

void CSeatStatusBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CBCGPDockingControlBar::OnPaint()
	
	CDC dcMemory;
	dcMemory.CreateCompatibleDC(&dc);
	HBITMAP hBitNew = CreateCompatibleBitmap(dc.m_hDC, m_pSeatBitmap->GetWidth(),
		m_pSeatBitmap->GetHeight());
	HGDIOBJ hOldBitmap = dcMemory.SelectObject(hBitNew);
	Gdiplus::Graphics graphics(dcMemory.m_hDC);

	Gdiplus::RectF grect; grect.X=0, grect.Y=0; grect.Width = m_pSeatBitmap->GetWidth(); grect.Height = m_pSeatBitmap->GetHeight();
	graphics.DrawImage(m_pSeatBitmap, grect, 0, 0, m_pSeatBitmap->GetWidth(), m_pSeatBitmap->GetHeight(), Gdiplus::UnitPixel);
	dc.BitBlt(0, 0, m_pSeatBitmap->GetWidth(), 
		m_pSeatBitmap->GetHeight(), &dcMemory, 0, 0, SRCCOPY);

	dcMemory.SelectObject(hOldBitmap);
	DeleteObject(hBitNew);
}

void CSeatStatusBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CBCGPDockingControlBar::OnLButtonDown(nFlags, point);
}

BOOL CSeatStatusBar::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//pDC->SetBkColor(RGB(255, 0, 0));
	//return CBCGPWnd::OnEraseBkgnd(pDC);
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(rect, GetSysColor(COLOR_WINDOW));
	return TRUE;
	//return CBCGPDockingControlBar::OnEraseBkgnd(pDC);
}
