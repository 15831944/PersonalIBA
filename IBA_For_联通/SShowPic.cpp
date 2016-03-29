// SShowPic.cpp : ʵ���ļ�
//

#include "stdafx.h"

#include ".\sshowpic.h"


// CSShowPic
static Gdiplus::GdiplusStartupInput gdiplusStartupInput;
static ULONG_PTR gdiplusToken;

using namespace Gdiplus;

#pragma comment(lib,"gdiplus.lib")

IMPLEMENT_DYNAMIC(CSShowPic, CStatic)
CSShowPic::CSShowPic()
{
	ZeroMemory(m_szFullPath, sizeof(m_szFullPath));

	BOOL bOK = Gdiplus::GdiplusStartup(&gdiplusToken, 
		&gdiplusStartupInput, NULL) == 0;

	m_pImage = NULL;
}

CSShowPic::~CSShowPic()
{
	if (m_pImage)
	{
		delete m_pImage;
	}
	m_pImage = NULL;
	Gdiplus::GdiplusShutdown(gdiplusToken);
}


BEGIN_MESSAGE_MAP(CSShowPic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CSShowPic ��Ϣ�������


void CSShowPic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CStatic::OnPaint()
	if(m_pImage)
	{
		CRect rect;
		GetWindowRect(&rect);
		ScreenToClient(&rect);
		Graphics gr(dc.GetSafeHdc());
		gr.DrawImage(m_pImage, Rect(rect.top, rect.left, rect.Width(),rect.Height()), 0, 0, m_pImage->GetWidth(), m_pImage->GetHeight(), UnitPixel);
	}
	else
	{
			//dc.TextOut(0,0, _T("����Ƭ��Ϣ"));
	}
}

void CSShowPic::ShowPic(LPCTSTR szFullPath)
{
	ZeroMemory(m_szFullPath, sizeof(m_szFullPath));
	_tcsncpy(m_szFullPath, szFullPath, MAX_PATH);

	if('\0' != m_szFullPath[0])
	{
		Image * pImge = Image::FromFile(CT2W(m_szFullPath));
		if (pImge == NULL || pImge->GetLastStatus() != Ok)
		{
			if (m_pImage)
			{
				delete m_pImage;
			}
			m_pImage = NULL;
		}
		else
		{
			m_pImage = new Bitmap(pImge->GetWidth(), pImge->GetHeight());
			Graphics gr(m_pImage);
			gr.DrawImage(pImge, 0, 0, 0, 0, pImge->GetWidth(), pImge->GetHeight(), UnitPixel);
		}
		if (pImge)
		{
			delete pImge;
		}
	}
	else
	{
		if (m_pImage)
		{
			delete m_pImage;
		}
		m_pImage = NULL;
	}
	// �Ǵ�����Ч�����������ػ溯����ͼ
	Invalidate();
}
