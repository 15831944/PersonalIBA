#pragma once

// ��ͼƬ���󱣴浽bmp�ļ�
BOOL	SaveBitmapToFile(HBITMAP hBitmap, LPCTSTR lpFileName );
// ��ȡͼƬ�����ʽ��Ӧ��clsid
int		GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
// ��bmp��ʽͼƬ�ļ�ת����jpg��ʽͼƬ�ļ�
void	BMP2JPG(CString strBMPFile, CString strJPGFile);
// ����ָ����С����ͼƬ
void	BMPScale(CStringW strDstBmpFile, CStringW strSrcBmpFile, int nWidth, int nHeight);