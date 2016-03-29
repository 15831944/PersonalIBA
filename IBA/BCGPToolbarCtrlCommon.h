/**************************************************************************
file:  BCGPToolbarCtrlCommon.h
class1: CBCGPToolbarCtrlCommon,ʵ����ʹ��BCGP������ʱ���滻��ťΪͨ�õĻ����Զ����
	�ؼ�.
  warning: ��ʹ���Զ���ؼ����ʱ����Ҫ����һ����CBCGPToolbarCtrlCommon�̳е���,����
	CreateCtrlWnd����. ��CBCGPToolbarCtrlCommon�����������඼��������CopyFrom����

class2: CVCenterStatic,ʵ�־�̬�ı��Ĵ�ֱ������ʾ.

writer: guoxuxing
date:   2011/03/02
**************************************************************************/

#ifndef __BCGPTOOLBARCTRLCOMMON__H__
#define __BCGPTOOLBARCTRLCOMMON__H__ 

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//common controls enum define

enum emBCGPToolbarCtrlCommon
{
	emBCGP_static,
	emBCGP_edit,
	emBCGP_button,
	emBCGP_combobox,

	/* ���ָ��Ϊ�Զ�������,��Ҫ����CBCGPToolbarCtrlCommon */
	emBCGP_customize     
};

class CVCenterStatic;     // inherit from static,for text vertical center

class CBCGPToolbarCtrlCommon :
	public CBCGPToolbarButton
{

	DECLARE_SERIAL(CBCGPToolbarCtrlCommon)

public:

	CBCGPToolbarCtrlCommon();
	CBCGPToolbarCtrlCommon(UINT uiID, int iImage,
		emBCGPToolbarCtrlCommon  ctrlKind,
		CWnd  *pParent = NULL,
		DWORD dwStyle = 0,
		int iWidth = 0);

	virtual ~CBCGPToolbarCtrlCommon(void);

public:// inherit virtual
	virtual void OnDraw(CDC* pDC, const CRect& rect,
						 CBCGPToolBarImages* pImages, 
		                 BOOL bHorz /* = TRUE */, 
						 BOOL bCustomizeMode /* = FALSE */,
						 BOOL bHighlight /* = FALSE */, 
						 BOOL bDrawBorder /* = TRUE */, 
						 BOOL bGrayDisabledButtons /* = TRUE */);

	virtual void CopyFrom(const CBCGPToolbarButton& src);

	virtual SIZE OnCalculateSize(CDC* pDC, const CSize& sizeDefault, 
		BOOL bHorz);

	virtual void OnMove();

	virtual void OnChangeParentWnd (CWnd* pWndParent);

	virtual void Serialize (CArchive& ar);
	
	virtual void OnShow (BOOL bShow);

protected:// self virtual
	virtual BOOL CreateCtrlWnd();

protected:
	void AdjustCtrlRect();

public:

	void SetCtrlWindowText(CString str);

	CWnd* GetCtrlWnd() const{ return m_pWndCtrl; }
	
protected:
	static const int iDefaultSize = 100;
	static const int iHorzMargin = 3;
	static const int iVertMargin = 3;

	CWnd *					m_pWndParent;    // ������ָ��
	CWnd *					m_pWndCtrl;		 // �ؼ�ָ�� 
	emBCGPToolbarCtrlCommon m_cltrKind;
	DWORD					m_dwCtrlStyle;   // �ؼ����
	int						m_iWidth;        // �ؼ��Ŀ��

	CString m_strCtrlText;
};

class CVCenterStatic : public CStatic
{
	DECLARE_MESSAGE_MAP()

public :
	CVCenterStatic();
	~CVCenterStatic(){}

	afx_msg void OnPaint();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	void SetFont(CFont* pFont);

	COLORREF GetBkColor() const { return m_clrBk; }
	COLORREF GetTextColor() const { return m_clrTxt; }
	COLORREF SetBkColor(const COLORREF clrBk,BOOL bRepaint = FALSE);
	COLORREF SetTextColor(const COLORREF clrTxt,BOOL bRepaint = FALSE);

public:
	virtual void DrawBkAndText(CDC& dc);

public:
	COLORREF m_clrBk;
	COLORREF m_clrTxt;
	CFont *m_pFont;
	
	
};


#endif