#pragma once

#include <gdiplus.h>

class CSShowPic : public CStatic
{
	DECLARE_DYNAMIC(CSShowPic)

public:
	CSShowPic();
	virtual ~CSShowPic();
	// ����·����ʾһ��ͼƬ
	void ShowPic(LPCTSTR szFullPath);
	void SetPicBk(COLORREF clr) { m_clr = clr; }		// ������Ƭ��ɫ
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

private:
	TCHAR m_szFullPath[MAX_PATH];
	Gdiplus::Image* m_pImage;
	COLORREF m_clr;
};


