/**************************************************************************
G2XUI������ؼ�����������

CG2XGridCtrl: ����ؼ�
	���:	�ھ�CG2XHeadCtrl,��ͷ�ؼ�
			�ھ�CG2XGridPanel,�����������
			�ھ�CG2XScrollBar,��ֱ or ˮƽ������

**************************************************************************/

#pragma once

G2XUI_BEGIN

// alignment
#define GVGCS_LEFT   DT_LEFT
#define GVGCS_CENTER DT_CENTER
#define GVGCS_RIGHT  DT_RIGHT

#define GVGCFMT_NORMAL    0x00    // ����״̬
#define GVGCFMT_HOVER    0x01    // ��꾭��
#define GVGCFMT_CLICKING 0x02    // �����
#define GVGCFMT_SELECTED 0x04   //  ��ѡ��


#define GDI_M_HEADCTRL_RELAYOUT 0x0101
#define GDI_M_HEADCTRL_SORT 0x0102 // wParam: ��Ҫ�����������, lParam: �µ�������

class G2X_CLASSES CG2XHeadCtrl;
class G2X_CLASSES CG2XGridPanel;
class G2X_CLASSES CG2XScrollBar;
class G2X_CLASSES CG2XHeadItem;
class G2X_CLASSES CG2XGridItem;
class G2X_CLASSES CG2XGridRow;


//-------------------------------------------------------------------------
//- CG2XGridCtrl
//- ����ؼ�
//- ���:
//      CG2XHeadCtrl ��ͷ�ؼ�
//      CG2XGridPanel ������������
//      CG2XScrollBar ��ֱ��ˮƽ������
//-------------------------------------------------------------------------

class G2X_CLASSES CG2XGridCtrl : public CG2XWnd
{
	DECLARE_DYNAMIC(CG2XGridCtrl)
	DECLARE_GDI_MESSAGE_MAP()
public:
	friend class G2X_CLASSES CG2XGridPanel;
	friend class G2X_CLASSES CG2XGridRow;
	
	CG2XGridCtrl(void);
	virtual ~CG2XGridCtrl(void);
public:
	typedef std::vector<CG2XGridRow*> ArrayRows;

	static const INT HSCROLLBAR_ID = 0x0FFFFFF1;
	static const INT VSCROLLBAR_ID = 0x0FFFFFF2;
	static const INT DEFAULT_ROW_HEIGHT  = 18; // Ĭ���и�
	static const INT DEFAULT_HEAD_HEIGHT = 20; // Ĭ�ϱ�ͷ�߶�
	static const INT DEFAULT_SCROLL_SIZE = 18; // Ĭ�Ϲ������Ŀ��

protected:
	g_msg void OnCreate();

	// ����ˮƽ����������Ϣ
	g_msg void OnHScrollNotify(NMHDR* pHdr,LONG* lpRes);

	// ���մ�ֱ����������Ϣ
	g_msg void OnVScrollNotify(NMHDR* pHdr,LONG* lpRes);
	
	// ���ձ�ͷ���²��ֵ���Ϣ
	g_msg LRESULT OnHeadRelayOut(WPARAM,LPARAM);

	g_msg LRESULT OnHeadSort(WPARAM,LPARAM);

	g_msg void OnSize();

	g_msg void OnDestroy();
	
public:
	//---------------------------------------------------------------------
	//								��ͷ���
	//---------------------------------------------------------------------

	// ��ȡ��ͷ�ؼ�
	CG2XHeadCtrl* GetHeadCtrl();

	// �滻��ͷ�ؼ�
	BOOL ReplaceHeadCtrl(CG2XHeadCtrl* pNewHeadCtrl);

	// ������
	BOOL InsertColumn(INT iCol, LPCTSTR lpTxt, INT nAlign=GVGCS_LEFT, INT nWidth=-1, INT iImage=-1);

	void EnableColumnSort(INT iCol, BOOL bEnable);

	//---------------------------------------------------------------------
	//								 �����
	//---------------------------------------------------------------------

	// ����һ���հ��ж���
	CG2XGridRow* CreateRow();

	// ��ȡ����
	INT GetColumnCount();

	// ɾ��������
	void DeleteAllRows(BOOL bRedraw=TRUE);

	// ���ñ�ͷ�ĸ߶�
	void SetHeadHeight(INT nHeight=DEFAULT_HEAD_HEIGHT);
	
	// ����Ĭ���и�,���е��н���������߶ȣ�������Ӧ�õ����и߶ȷ��Ϊ�Զ������
	void SetDefaultRowHeight(INT nHeight); 
	
	// ��ȡĬ���и�
	INT GetRowHeight(){ return m_nRowHeight; }
	
	// ���µ�������
	void RelayOut();

	// ����һ��
	void InsertRow(CG2XGridRow* pRow,INT nAt=0, BOOL bInsertBefore=TRUE, BOOL bRepaint=FALSE);

	// ɾ��һ��
	void DeleteRow(CG2XGridRow* pRow);

	// ɾ��һ��
	void DeleteRow(INT nAt);

	// ��ȡ��
	CG2XGridRow* GetRow(INT nAt);

	// �õ������б������λ��
	INT RowToIndex(CG2XGridRow* pRow);

	// ��ȡ�������
	CG2XGridPanel* GetPanel(){ return m_pGridPanel; }

	// ��ȡ����
	INT GetRowsCount();

	// ��ĳ����Ϊѡ��״̬������ȡ��ѡ��״̬
	void SelectRow(INT iRow, BOOL bSel = TRUE);

	// ��ĳһ����Ϊѡ��״̬������ȡ��ѡ��״̬
	void SelectItem(INT iRow,INT iItem, BOOL bSel = TRUE);

	// ���������ߵ���ɫ
	void SetGridColor(const CColor& clrGrid);

	// ��ȡѡ�����
	CG2XGridRow* GetSelectedRow();

	// ��ȡѡ�����
	void GetSelectedRows(ArrayRows& arrRows);

	//- �����Ƿ����ø��л�ɫ, �����ɫֵָ��Ϊ��,CG2XGridCtrlʹ��Ĭ��ֵ
	void SetInterlaceColor(BOOL bAllow, CColor *pClr1=NULL, CColor *pClr2=NULL);

public:
	// �Ƿ�ȫ��ѡ��
	BOOL IsFullRowSelected() { return m_bFullRowSelected; }

	// ����Ϊȫ��ѡ��
	void SetFullRowSelected(BOOL bFull) { m_bFullRowSelected = bFull; }

	// �Ƿ�ֻ֧�ֵ�ѡ
	BOOL IsSingleSel() { return m_bSingleSel; }

	// ����Ϊ��ѡ������Ϊ��ѡ
	void SetSingleSel(BOOL bSingle);

	// �Ƿ�������
	BOOL HasGrid() { return m_bHasGrid; }

	// ���������񣬻���û��
	void SetHasGrid(BOOL bHasGrid);

	// �Ƿ���ƿհ����������
	BOOL IsDrawSpaceGrid() { return m_bDrawSpaceGrid; }

	// ���ÿհ������Ƿ���Ҫ��������
	void SetDrawSpaceGrid(BOOL bDraw);

	//- ����ͼ���б�
	BOOL SetImageList(const CG2XImageList& imgSrc);

	//- ��ȡͼ���б�
	CG2XImageList* GetImageList() const;

	//- ����
	void Sort(INT iColumn, G2XSortStyle sortStyle);
	
protected:
	void UpdateScrollBar();

private:
	struct SortParam
	{
		G2XSortStyle sortStyle;
		INT iColumn;
	};
	BOOL CompareRow(SortParam sp, const CG2XGridRow* pRow1, const CG2XGridRow* pRow2);

protected:
	virtual BOOL CompareItem(INT iColumn, G2XSortStyle sortStyle, const CG2XGridItem* pItem1, const CG2XGridItem* pItem2);

protected:
	CG2XHeadCtrl *m_pHeadCtrl;
	CG2XGridPanel* m_pGridPanel;
	CG2XScrollBar* m_pHScrollBar;
	CG2XScrollBar* m_pVScrollBar;

	CG2XImageList* m_pImageList; // ͼ���б�
	ArrayRows m_arrRows;      // �����еļ���
	ArrayRows m_selectedRows; // ��ѡ����еļ���

	INT m_nRowHeight; // Ĭ���и�
	INT m_nRowsTotoalHeight;
	BOOL m_bFullRowSelected;
	BOOL m_bSingleSel;
	BOOL m_bHasGrid;
	BOOL m_bDrawSpaceGrid;		// �Ƿ���ƿհ����������
	INT m_nHeadHeight; // ��ͷ�ĸ߶�
	BOOL m_bAllowInterlaceColor; // �Ƿ�������л�ɫ
	CColor m_colorInterlace1;
	CColor m_colorInterlace2;
};

class G2X_CLASSES CG2XGridPanel : public CG2XWnd
{
	DECLARE_DYNCREATE(CG2XGridPanel)
	DECLARE_GDI_MESSAGE_MAP()
public:
	friend class G2X_CLASSES CG2XGridCtrl;
	CG2XGridPanel(CG2XGridCtrl* pGridCtrl);

	void Update(BOOL bUpdateRows=FALSE);

protected:
	virtual void DrawSelf(Gdiplus::Graphics *pGraphics);

	// ��������
	void DrawGrid(
		Graphics *pGraphics, 
		const INT nOffX, 
		const INT nOffY,
		const INT* pHeights, const INT nCountY, 
		const INT* pWidths, const INT nCountX,
		CColor& clrGrid,
		INT nPenWidth=1);
	
	void SetHScrollPos(INT nPos, BOOL bRedraw=FALSE);
	void SetVScrollPos(INT nPos, BOOL bRedraw=FALSE);

	g_msg void OnCreate();
	g_msg void OnSize();
	g_msg void OnMouseHover(UINT nFlags, CPoint point);
	g_msg void OnMouseLeave();
	g_msg void OnLButtonDown(UINT nFlags, CPoint point);
	g_msg void OnDestroy();

	virtual BOOL OnGdiNotify(UINT nID, WPARAM wParam, LPARAM lParam, LONG* lpResult);

public:
	CG2XGridCtrl* m_pGridCtrl;

	INT m_nHScrollPos;					// ˮƽ��������λ��
	INT m_nVScrollPos;					// ��ֱ��������λ��
	std::vector<CRect> m_arrRects;		// �����ڵ�����
	
	//std::vector<INT> m_arrHeigths;	// ��ռ�õĸ߶�
	INT m_iFirstRow;					// �����������ĵ�һ��       
	INT m_iLastRow;						// ���������������һ��
	
	CG2XGridCtrl::ArrayRows* m_pRows;
	INT m_nTopRowOffset;

	INT m_iHoverRow;
	INT m_iHoverItem;

	CColor m_clrGrid;
	INT    m_nGridBreadth;

	int * m_pCXs;
	int   m_nCXCount;
	
};

class G2X_CLASSES CG2XHeadCtrl : public CG2XControl
{
	DECLARE_DYNCREATE(CG2XHeadCtrl)
	DECLARE_GDI_MESSAGE_MAP()
public:
	friend class G2X_CLASSES CG2XGridCtrl;
	CG2XHeadCtrl();
	virtual ~CG2XHeadCtrl();

	BOOL InsertItem(
		INT iItem,
		LPCTSTR lpTxt,
		INT nAlign=GVGCS_LEFT, 
		INT nWidth=-1,
		INT iImage=-1);

	void SetImageList(UINT nIDRes, INT nImgWidth);
	void SetImageList(LPCTSTR lpFileName, INT nImgWidth);

	BOOL DeleteItem(INT iItem);
	BOOL DeleteAllItems();

	CG2XHeadItem* GetItem(INT iItem);
	BOOL ReplaceItem(INT iItem,CG2XHeadItem* pNewItem);
	RECT GetItemRect(INT iItem);

	

	void RelayOut(BOOL bNotify=TRUE);
	INT GetItemCount() { return (INT)m_arrayItems.size(); }

	INT GetTotalWidth();
	INT GetHeadHeight();

	void GetWidths(INT *pWidths,INT nCount);

	void SetHScrollOffset(INT nOff);

	void SetDrawEndSplitline(BOOL bDraw) { m_bDrawEndSplitLine = bDraw; }
protected:
	virtual void DrawControl(Graphics* pGraphics, G2XControlState ctrlState);
	virtual void DrawItem(Graphics* pGraphics,LPRECT lpRect,CG2XHeadItem* pItem, INT nFlags=GVGCFMT_NORMAL);
	virtual void DrawSplitLine(Graphics* pGraphics, LPRECT lpRect);

	// ������ı���
	virtual void DrawItemBk(Graphics* pGraphics, LPRECT lpRect, CG2XHeadItem* pItem, INT nFlags=GVGCFMT_NORMAL){}
	g_msg void OnCreate();
	g_msg void OnDragging(UINT nFlag, CPoint ptFrom, CPoint point);
	g_msg void OnDragged(UINT nFlag, CPoint ptFrom, CPoint point);
	g_msg void OnMouseHover(UINT nFlags, CPoint point);
	g_msg void OnMouseLeave();
	g_msg void OnSize();
	g_msg void OnLButtonDown(UINT nFlags, CPoint point);

	INT TestItem(CPoint point);

protected:
	typedef std::vector<CG2XHeadItem*> ArrayItems;
	
	ArrayItems m_arrayItems;
	CG2XImageList m_imgList;
	BOOL m_bDraggingSplit;
	CPoint m_ptDragPoint;
	INT m_iDragItemSplit;
	INT m_iHoverItem;

	INT m_nScrollBarOffset; // ��������ˮƽƫ�� >=0
	
	BOOL m_bDrawEndSplitLine; // �Ƿ���Ҫ����β���ķָ���
};
class G2X_CLASSES CG2XHeadItem
{
protected:
	CG2XHeadItem()
	{
		m_nAlign = GVGCS_LEFT;
		m_nWidth = 60;
		m_iImage = -1;
		m_bEnableSort = FALSE;
		m_sortStyle = G2XSortStyleNormal;
		m_pItemWnd = NULL;
		m_clrText = RGB(0,0,0);
	}

public:
	static CG2XHeadItem* CreateObject()
	{
		return new CG2XHeadItem;
	}
	void CopyFrom(const CG2XHeadItem* pItem)
	{
		m_strTxt = pItem->m_strTxt;
		m_nAlign = pItem->m_nAlign;
		m_nWidth = pItem->m_nWidth;
		m_iImage = pItem->m_iImage;
		m_bEnableSort = pItem->m_bEnableSort;
		m_sortStyle = pItem->m_sortStyle;
		m_clrText = pItem->m_clrText;
		m_pItemWnd = pItem->m_pItemWnd;
	}

	CString m_strTxt;
	INT m_nAlign;
	INT m_nWidth;
	INT m_iImage;
	BOOL m_bEnableSort;
	G2XSortStyle m_sortStyle;
	COLORREF m_clrText;
	CG2XWnd* m_pItemWnd;
};
class G2X_CLASSES CGdiVirtualGridItemID
{
public:
	CGdiVirtualGridItemID()
	{
		SetRowCol(-1,-1);
	}
	CGdiVirtualGridItemID(INT nRow, INT nCol)
	{
		SetRowCol(nRow, nCol);
	}
	CGdiVirtualGridItemID(const CGdiVirtualGridItemID& ID)
	{
		SetRowCol(ID.m_nRow, ID.m_nCol);
	}

	BOOL IsValidItem() const
	{
		return m_nRow != -1 && m_nCol != -1;
	}

public:
	void SetRowCol(INT nRow, INT nCol)
	{
		ASSERT(nRow>=-1 && nCol >=-1);
		m_nRow = nRow; m_nCol = nCol;
	}

	INT m_nRow;
	INT m_nCol;
};
class G2X_CLASSES CGdiVirtualGridItemRange
{
public:
	CGdiVirtualGridItemRange()
	{
		SetRange(-1,-1,-1,-1);
	}
	CGdiVirtualGridItemRange(INT nCol1, INT nRow1, INT nCol2, INT nRow2)
	{
		SetRange(nCol1, nRow1, nCol2, nRow2);
	}
	CGdiVirtualGridItemRange(const CGdiVirtualGridItemID& nID1, const CGdiVirtualGridItemID& nID2)
	{
		SetRange(nID1, nID2);
	}
	void SetRange(INT nCol1, INT nRow1, INT nCol2, INT nRow2)
	{
		ASSERT(nCol1 >= -1 && nRow1 >= -1 && nCol2 >= -1 && nRow2 >= -1);
		m_nCol1 = nCol1; m_nCol2 = nCol2; m_nRow1 = nRow1; m_nRow2 = nRow2;
	}
	void SetRange(const CGdiVirtualGridItemID& nID1, const CGdiVirtualGridItemID& nID2)
	{
		SetRange(nID1.m_nCol, nID1.m_nRow, nID2.m_nCol, nID2.m_nRow);
	}
	CGdiVirtualGridItemID GetLeftTop()
	{
		return CGdiVirtualGridItemID(m_nRow1, m_nCol1);
	}
	CGdiVirtualGridItemID GetRightBottom()
	{
		return CGdiVirtualGridItemID(m_nRow2, m_nCol2);
	}
	BOOL IsEmpty()
	{
		return m_nCol1 == -1 && m_nCol2 == -1 && m_nRow1 == -1 && m_nRow2 == -1;
	}

	BOOL ItemInRange(const CGdiVirtualGridItemID& ItemID)
	{
		if (!ItemID.IsValidItem())
		{
			return FALSE;
		}
		if (IsEmpty())
		{
			ASSERT(0);
			return FALSE;
		}

		CRect rt(m_nCol1, m_nRow1, m_nCol2, m_nRow2);
		return rt.PtInRect(CPoint(ItemID.m_nCol, ItemID.m_nRow));
	}

	INT m_nCol1;
	INT m_nRow1;
	INT m_nCol2;
	INT m_nRow2;
};

class G2X_CLASSES CG2XGridItem
{
public:
	friend class CG2XGridRow;
	friend class CG2XGridPanel;
	friend class CG2XGridCtrl;

	CG2XGridItem(CG2XGridRow* pParentRow);
	virtual ~CG2XGridItem();
	void CopyFrom(const CG2XGridItem* pItem);
	CG2XGridCtrl* GetGridCtrl() const;
	CG2XGridRow* GetParentRow() const;
	void SetData(DWORD dwData);
	DWORD GetData();
	void SetItemText(LPCTSTR lpStr, BOOL bRedraw=TRUE);
	CString GetItemText();
	void SetColorData(const CG2XControlColorData& colorData);
	CG2XControlColorData& GetColorData() const;
	void SetItemWnd(CG2XWnd* pWnd, LPSIZE lpFixedSize=NULL);
	void SetItemWndSize(const SIZE& sz, BOOL bUpdate=FALSE);
	CG2XWnd* GetItemWnd();
	void SetImage(int iImage, BOOL bRedraw=TRUE);
	INT GetImage();

public:
	virtual void DrawItem(Graphics* pGraphics,LPRECT lpRect, G2XControlState ctrlState);
	
protected:
	CG2XGridRow* m_pParentRow;
	INT m_iImage;						
	INT m_nAlign;						// Alignment
	CG2XWnd *m_pItemWnd;
	DWORD m_dwData; 
	CString m_strText;
	CG2XControlColorData* m_pColorData; // If is NULL,using Row's colorData
	SIZE m_szItemWnd;
};
class G2X_CLASSES CG2XGridRow
{
protected:
	friend class CG2XGridCtrl;
	friend class CG2XGridPanel;
	CG2XGridRow(CG2XGridCtrl* pGridCtrl, BOOL bUsingDefaultRowHeight=TRUE);
public:
	virtual ~CG2XGridRow();
	
	void SetGridCtrl(CG2XGridCtrl* pGridCtrl, BOOL bUsingDefaultRowHeight=TRUE);

	void SetItemCount(INT nCount);

	CG2XGridItem* operator[](INT iItem) const;

	CG2XGridItem* GetItem(INT iItem) const;

	void ReplaceItem(INT iItem,CG2XGridItem* pNewItem);
	
	void SetData(DWORD dwData);
	
	DWORD GetData();

	// �����и�
	void SetRowHeight(INT nHeight, BOOL bUpdate=FALSE);

	// ��ȡ�и�
	INT GetRowHeight();

	// �Ƿ�ʹ��Ĭ���иߣ�TRUE: ʹ��GridCtrlָ����Ĭ���и�,FALSE: ʹ��CG2XGridRow���Զ����и�
	BOOL IsUsingDefaultRowHeight();
	void UsingCustomRowHeight(BOOL bCustom){ m_bUsingDefaultRowHeight= !bCustom; }

	virtual void DrawRow(Graphics* pGraphics, LPRECT lpRect, G2XControlState ctrlState, INT iHoverItem);

	INT GetItemCount() { return (INT)m_arrItems.size(); }

	CG2XGridCtrl* GetGridCtrl(){ return m_pGridCtrl; }
	CG2XGridPanel* GetPanel(){ return m_pGridCtrl->GetPanel(); }
	CG2XControlColorData* GetColorData() { return m_pColorData; }
	INT GetRowID() { return m_nRowID; }
public:
	typedef std::vector<CG2XGridItem*> ArrayItem;
	
	CG2XGridCtrl* m_pGridCtrl; 
	ArrayItem m_arrItems;
	INT   m_nRowID;
	DWORD m_dwData;
	INT   m_nRowHeight;
	BOOL  m_bUsingDefaultRowHeight;
	BOOL  m_bSelected;
	BOOL  m_bFocused;
	INT   m_iSelectedItem;
	
	CG2XControlColorData *m_pColorData;
};

G2X_END
