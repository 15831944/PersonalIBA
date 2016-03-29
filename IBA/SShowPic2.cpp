// SShowPic.cpp : ʵ���ļ�
//

#include "stdafx.h"

#include ".\sshowpic2.h"


// CSShowPic2
//static Gdiplus::GdiplusStartupInput gdiplusStartupInput;
//static ULONG_PTR gdiplusToken;
//
//using namespace Gdiplus;
//
//#pragma comment(lib,"gdiplus.lib")

IMPLEMENT_DYNAMIC(CSShowPic2, CStatic)
CSShowPic2::CSShowPic2()
{
	m_clr = RGB(255,255,255);
}

CSShowPic2::~CSShowPic2()
{
}


BEGIN_MESSAGE_MAP(CSShowPic2, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CSShowPic2 ��Ϣ�������


void CSShowPic2::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CStatic::OnPaint()
	if(!m_strFullPath.IsEmpty() &&
		PathFileExists(m_strFullPath))
	{
		CRect rect;
		GetClientRect(rect);
		ShowJpgGif(&dc, m_strFullPath, 0, 0, rect.Width(), rect.Height());
	}
	else
	{
		CRect rect;
		GetClientRect(rect);
		dc.FillSolidRect(rect, m_clr);	
	}
}

void CSShowPic2::ShowPic(LPCTSTR szFullPath)
{
	m_strFullPath.Empty();
	m_strFullPath = szFullPath;
	// �Ǵ�����Ч�����������ػ溯����ͼ
	Invalidate();
}

bool ShowJpgGif(CDC* pDC,CString strPath, int x, int y, int width, int height)  
{  
	IStream *pStm;   
	CFileStatus fstatus;   
	CFile file;   
	LONG cb;   
	if (file.Open(strPath,CFile::modeRead)&&file.GetStatus(strPath,fstatus)&&((cb = fstatus.m_size) != -1))   
	{   
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, cb);   
		LPVOID pvData = NULL;   
		if (hGlobal != NULL)   
		{   
			pvData = GlobalLock(hGlobal);  
			if (pvData != NULL)   
			{   
				file.Read(pvData, cb);   
				GlobalUnlock(hGlobal);   
				CreateStreamOnHGlobal(hGlobal, TRUE, &pStm);   
			}  
		}  
	}  
	else  
		return false;  
	//��ʾJPEG��GIF��ʽ��ͼƬ��GIFֻ����ʾһ֡����������ʾ������  
	//Ҫ��ʾ����GIF��ʹ��ACTIVE��//����  
	IPicture *pPic;  
	//load image from file stream  
	if(SUCCEEDED(OleLoadPicture(pStm,fstatus.m_size,TRUE,IID_IPicture,(LPVOID*)&pPic)))  
	{  
		OLE_XSIZE_HIMETRIC hmWidth;   
		OLE_YSIZE_HIMETRIC hmHeight;   
		pPic->get_Width(&hmWidth);   
		pPic->get_Height(&hmHeight);   
		double fX,fY;   
		//get image height and width  
		/*fX = (double)pDC->GetDeviceCaps(HORZRES)*(double)hmWidth/((double)pDC->GetDeviceCaps(HORZSIZE)*100.0);   
		fY = (double)pDC->GetDeviceCaps(VERTRES)*(double)hmHeight/((double)pDC->GetDeviceCaps(VERTSIZE)*100.0); */  
		//use render function display image  
		if(FAILED(pPic->Render(*pDC,x,y,(DWORD)width,(DWORD)height,0,hmHeight,hmWidth,-hmHeight,NULL)))   
		{  
			pPic->Release();  
			return false;  
		}  
		pPic->Release();  
	}   
	else   
		return false;   
	return true;  
}

