#include "stdafx.h"

#include <afxtempl.h>

#include <GdiPlus.h>
using namespace Gdiplus;

#include "CaptureVideo.h"
#include "IBALog.h"

// ��������������ͷ����ƵԴ,���õ���ƵԴ��^^�ָ�,����
// ��ʶɨ����
const CString g_strUnuseCapture("IDR USB Device");

BOOL SaveBitmapToFile(HBITMAP hBitmap, LPCTSTR lpFileName )
{
	HDC hDC;					//�豸������
	int iBits;					//��ǰ��ʾ�ֱ�����ÿ��������ռ�ֽ���
	WORD wBitCount;				//λͼ��ÿ��������ռ�ֽ���
	DWORD dwPaletteSize=0,		//�����ɫ���С�� λͼ�������ֽڴ�С ��λͼ�ļ���С �� д���ļ��ֽ���
		dwBmBitsSize,
		dwDIBSize, dwWritten;
	BITMAP Bitmap;				//λͼ���Խṹ
	BITMAPFILEHEADER bmfHdr;	//λͼ�ļ�ͷ�ṹ
	BITMAPINFOHEADER bi;		//λͼ��Ϣͷ�ṹ
	LPBITMAPINFOHEADER lpbi;	//ָ��λͼ��Ϣͷ�ṹ

	HANDLE fh, hDib, hPal,hOldPal=NULL; //�����ļ��������ڴ�������ɫ����

	//����λͼ�ļ�ÿ��������ռ�ֽ���
	HDC hWndDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	hDC = ::CreateCompatibleDC( hWndDC ) ;
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);

	if (iBits <= 1)
		wBitCount = 1;
	else if (iBits <= 4)
		wBitCount = 4;
	else if (iBits <= 8)
		wBitCount = 8;
	else if (iBits <= 24)
		wBitCount = 24;
	else
		wBitCount = 24 ;

	//�����ɫ���С
	if (wBitCount <= 8)
		dwPaletteSize = (1 << wBitCount) * sizeof(RGBQUAD);

	//����λͼ��Ϣͷ�ṹ
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount+31)/32) * 4 * Bitmap.bmHeight ;

	//Ϊλͼ���ݷ����ڴ�
	hDib = GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;

	// �����ɫ��
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}

	// ��ȡ�õ�ɫ�����µ�����ֵ
	GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight,
		(LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
		+dwPaletteSize,
		(LPBITMAPINFO )
		lpbi, DIB_RGB_COLORS);

	//�ָ���ɫ��
	if (hOldPal)
	{
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}

	//����λͼ�ļ�
	fh = CreateFile(lpFileName, GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (fh == INVALID_HANDLE_VALUE)
		return FALSE;

	// ����λͼ�ļ�ͷ
	bmfHdr.bfType = 0x4D42; // "BM"
	dwDIBSize = sizeof(BITMAPFILEHEADER)
		+ sizeof(BITMAPINFOHEADER)
		+ dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER)
		+ (DWORD)sizeof(BITMAPINFOHEADER)
		+ dwPaletteSize;

	// д��λͼ�ļ�ͷ
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

	// д��λͼ�ļ���������
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize,
		&dwWritten, NULL);

	//���
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);

	return TRUE;
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT num = 0;			// number of image encoders
	UINT size = 0;			// size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;			// Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;			// Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;   // Success
		}    
	}

	free(pImageCodecInfo);
	return -1;   // Failure
}

void BMP2JPG(CString strBMPFile, CString strJPGFile)
{
	CLSID   encoderClsid;
	WCHAR   strGuid[39];
	if(GetEncoderClsid(L"image/jpeg", &encoderClsid) >= 0)
		StringFromGUID2(encoderClsid, strGuid, 39);

	Image image(strBMPFile.AllocSysString());
	image.Save(strJPGFile.AllocSysString(),&encoderClsid,NULL);
}

void BMPScale(CStringW strDstBmpFile, CStringW strSrcBmpFile, int nWidth, int nHeight)
{
	//LPWSTR pwcsSrcPic = (LPWSTR)malloc(sizeof(WCHAR) * (strSrcBmpFile.GetLength() + 1));
	//MultiByteToWideChar (CP_ACP, 0, strSrcBmpFile, -1, pwcsSrcPic, strSrcBmpFile.GetLength());
	////MultiByteToWideChar (CP_ACP, 0, strSrcBmpFile, -1, NULL, 0);
	//*(pwcsSrcPic + strSrcBmpFile.GetLength()) = 0;
	Bitmap image(strSrcBmpFile);

	Image* pScaledImage = NULL;

	pScaledImage = image.GetThumbnailImage(nWidth, nHeight, NULL, NULL);

	CLSID   encoderClsid;
	WCHAR   strGuid[39];
	if(GetEncoderClsid(L"image/bmp", &encoderClsid) >= 0)
		StringFromGUID2(encoderClsid, strGuid, 39);

	pScaledImage->Save(strDstBmpFile.AllocSysString(),&encoderClsid,NULL);

}
