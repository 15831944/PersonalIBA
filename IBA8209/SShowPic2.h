#pragma once

// ���CSShowPic��ЩJPGͼƬ�޷���ʾ������(GDI+������)
class CSShowPic2 : public CStatic
{
	DECLARE_DYNAMIC(CSShowPic2)

public:
	CSShowPic2();
	virtual ~CSShowPic2();
	// ����·����ʾһ��ͼƬ
	void ShowPic(LPCTSTR szFullPath);
	void SetPicBk(COLORREF clr) { m_clr = clr; }		// ������Ƭ��ɫ
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

private:
	CString m_strFullPath;
	COLORREF m_clr;
};
bool ShowJpgGif(CDC* pDC,CString strPath, int x, int y, int width, int height);
