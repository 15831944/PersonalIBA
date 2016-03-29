#include "StdAfx.h"
#include "imageex.h"

CImageEx::CImageEx(void)
{
}

CImageEx::~CImageEx(void)
{
}

void CImageEx::DoForPNG()
{
	if (!IsNull())
	{
		for (int i = 0; i < GetWidth(); ++i)
		{
			for (int j = 0; j < GetHeight(); ++j)
			{
				BYTE* pucColor = (BYTE*)GetPixelAddress(i, j);
				pucColor[0] = pucColor[0] * pucColor[3] / 255;
				pucColor[1] = pucColor[1] * pucColor[3] / 255;
				pucColor[2] = pucColor[2] * pucColor[3] / 255;
			}
		} 
	}
}

BOOL CImageEx::LoadFromOtherResource(HINSTANCE hInstance, UINT nIDResource, LPCTSTR lpszResType)
{
	BOOL bRet = FALSE;

	// ������Դ
	HRSRC hRsrc = FindResource(hInstance, MAKEINTRESOURCE(nIDResource), lpszResType);

	if (hRsrc == NULL) return FALSE;

	// ������Դ
	HGLOBAL hImgData = LoadResource(hInstance, hRsrc);
	
	if (hImgData == NULL) return FALSE;

	// �����ڴ��е�ָ����Դ
	LPVOID  lpVoid = LockResource(hImgData);
	DWORD   dwSize = SizeofResource(hInstance, hRsrc);
	HGLOBAL hNew = GlobalAlloc(GHND, dwSize);
	LPBYTE  lpByte = (LPBYTE)GlobalLock(hNew);

	memcpy(lpByte, lpVoid, dwSize);

	// ����ڴ��е�ָ����Դ
	GlobalUnlock(hNew);

	// ��ָ���ڴ洴��������
	LPSTREAM pStream = NULL;

	if (S_OK == CreateStreamOnHGlobal(hNew, TRUE, &pStream))
	{
		bRet = (S_OK == Load(pStream));
	}

	GlobalFree(hNew);
	// �ͷ���Դ
	FreeResource(hImgData);

	return bRet;
}

HRESULT CImageEx::SaveAsJpeg(LPCTSTR lpszFileName, ULONG Quality)
{
	Save(lpszFileName); //ֻΪ���ó�ʼ��GDI+

	DeleteFile(lpszFileName);

	CLSID encoderClsid = {0};

	UINT num = 0;			// number of image encoders
	UINT size = 0;			// size of the image encoder array in bytes
	Gdiplus::GetImageEncodersSize(&num, &size);
	
	if (size == 0) return E_FAIL;			// Failure

	Gdiplus::ImageCodecInfo* pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL) return E_FAIL;			// Failure

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

	BOOL bFind = FALSE;

	for (UINT i = 0; i < num; i++)
	{
		if (wcscmp(pImageCodecInfo[i].MimeType, L"image/jpeg") == 0)
		{
			encoderClsid = pImageCodecInfo[i].Clsid;
			bFind = TRUE;
			break;
		}
	}

	free(pImageCodecInfo);

	if (!bFind) return E_FAIL;

	Gdiplus::EncoderParameters encoderParameters;
	encoderParameters.Count = 1;
	encoderParameters.Parameter[0].Guid = Gdiplus::EncoderQuality;
	encoderParameters.Parameter[0].Type = Gdiplus::EncoderParameterValueTypeLong;
	encoderParameters.Parameter[0].NumberOfValues = 1;
	encoderParameters.Parameter[0].Value = &Quality;

	Gdiplus::Bitmap bm(operator HBITMAP(), NULL);

	if (Gdiplus::Ok != bm.Save(CT2CW(lpszFileName), &encoderClsid, &encoderParameters))
	{
		return E_FAIL;
	}
	
	return S_OK;
}
