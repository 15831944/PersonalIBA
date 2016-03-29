#pragma once

#include "G2XWnd.h"
#include "G2XImageList.h"

G2XUI_BEGIN

struct G2XCommandBarButtonInfo
{
	UINT nID;
	CString strTitle;
	INT nImgIndex;
	BOOL bEnable;
	BOOL bSelected;
	COLORREF clrText;
	BOOL bHover;
};

class G2X_CLASSES CG2XCommandBar;

class G2X_CLASSES CG2XCommandBarButton 
{
public:
	CG2XCommandBarButton(G2XCommandBarButtonInfo& btnInfo, CG2XCommandBar* pBar);
	virtual void DrawButton(Graphics* pGraphics, CRect& rt);

	CG2XCommandBar* m_pBar;
	G2XCommandBarButtonInfo m_btnInfo;
};

//-------------------------------------------------------------------------
// CG2XCommandBar
//		����QQ���Թܼҡ�360һ���Ĺ�������ͼƬ+���ֵİ�ť��
//      CG2XCommandBar����Ĭ��͸����
// ����ʹ��: 1) ����Create
//          2) ����SetImageList,���û��ͼƬ����ʹ�ò�����
//          3) ����InsertButtons,����һ�鰴ť
//-------------------------------------------------------------------------
class G2X_CLASSES CG2XCommandBar : public CG2XControl
{
	DECLARE_DYNCREATE(CG2XCommandBar)
	DECLARE_GDI_MESSAGE_MAP()
	friend class CG2XCommandBarButton;
public:
	CG2XCommandBar();
	virtual ~CG2XCommandBar();

protected:
	virtual void DrawControl(Graphics* pGraphics,G2XControlState ctrlState);

	g_msg void OnMouseHover(UINT nFlags, CPoint point);
	g_msg void OnMouseLeave();
	g_msg void OnLButtonDown(UINT nFlags, CPoint point);

	g_msg void OnCreate();
public:
	//---------------------------------------------------------------------
	//- ����
	//- bHasTitle: ��ť�Ƿ����ı�����
	//- lpBtnSize:
	//      [NULL]: CG2XCommandBar�����Զ�������ť�Ĵ�С
	//     [!NULL]: ÿ����ť��������̶���С
	//---------------------------------------------------------------------
	BOOL Create(
		CG2XWnd* pParent, 
		CRect rcInParent,        
		BOOL bHasTitle,          
		UINT nID, 
		LPSIZE lpBtnSize=NULL    // if lpBtnSize=NULL buttons auto size, else fixed size
		);

	// ����ͼ���б�
	BOOL SetImageList(const CG2XImageList& imgList);

	// �������ť
	void InsertButtons(G2XCommandBarButtonInfo* pBtns, int nCount);

	// ��������
	void Relayout();

	// ����ĳ����ťΪ��ѡ��״̬
	void SetSelBtn(INT iIndex);

	// ����ĳ����ťΪ��ѡ��״̬
	void SetSelBtnByID(UINT nID);

	// ʹ��ť���û򲻿���
	BOOL EnableButton(UINT nID, BOOL bEnable);

	// ����ӳ�䵽��ť������
	INT CommandToIndex(UINT nID);

	// �Ƴ���ť
	BOOL RemoveButton(UINT nID);
	
	// ���밴ť,���nIndexָ������һ����Чλ�ã���ť������뵽ĩβ
	void InsertButton(INT nIndex, G2XCommandBarButtonInfo& info);

	// ��ť����
	INT GetCount();

protected:
	CG2XImageList* m_pImgList;

	std::vector<CG2XCommandBarButton*> m_arrBtns;
	std::vector<RECT> m_arrBtnRect;

	SIZE m_szImg;
	INT m_iSelBtn;
	INT m_iSelTemp;
	BOOL m_bHasTitle;

	SIZE *m_pFixedSize;
};

G2X_END