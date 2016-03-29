#pragma once

G2XUI_BEGIN
G2X_FRAME

class G2X_CLASSES CG2XCaptionBar :	public CG2XWnd
{
	DECLARE_DYNCREATE(CG2XCaptionBar)
	DECLARE_GDI_MESSAGE_MAP()
public:
	CG2XCaptionBar(void);
	virtual ~CG2XCaptionBar(void);

	BOOL Create(CG2XWnd* pParent, LPCTSTR lpCaption);

	void SetIcon(Image* pImage, COLORREF clrTransparent=RGB(255,255,255));

	void SetIcon(HICON hIcon);

	// �滻���е�ϵͳ��ť��
	void ReplaceSysBtnBar(CG2XSysBtnBar* pNewSysBar);

protected:
	g_msg void OnSize();
	g_msg void OnLButtonDown(UINT nFlags, CPoint point);
	g_msg void OnLButtonDClick(UINT nFlags, CPoint point);
	
	g_msg void OnMinimize();
	g_msg void OnMaximize();
	g_msg void OnRestore();
	g_msg void OnClose();

	virtual void DrawSelf(Gdiplus::Graphics *pGraphics);

	// �ж������Ӧ��λ���Ƿ�Ϊ�, ���Ϊ���,��겻�����϶�����
	virtual BOOL TestMouseActive(CPoint pt);

	virtual void DrawBackground(Graphics *pGraphics);

public:
	
	CG2XSysBtnBar* GetSysBtnBar();
	void SetTitleColor(const Color& clr);

	void SetCaptionHeight(INT nHeight);

	// �������ĸ߶ȣ����Ϊ-1,��ô���ĸ߶Ⱥͱ������ĸ߶�һ��
	void SetSysBtnBarHeight(INT nHeight=-1);

protected:
	CG2XSysBtnBar* m_pSysBtnBar; // ��ť������,Ĭ������С������󻯣��ر�
	INT m_nSysBtnBarHeight;      
	Image* m_pImage;        // ������ͼ��ͼ��
	INT m_nCaptionHeight;   // �������ĸ߶�
	INT m_nSysBtnCount;     
	CColor m_clrTitle;      // �����ı�����ɫ

	G2XTextHAlign m_hTitleAlign;
	G2XTextVAlign m_vTitleAlign;
};

G2X_END
G2X_END
