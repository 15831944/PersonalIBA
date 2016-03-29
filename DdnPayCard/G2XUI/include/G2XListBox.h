#pragma once

G2XUI_BEGIN

class G2X_CLASSES CG2XListBox : public CG2XControl
{
	DECLARE_DYNCREATE(CG2XListBox)
	DECLARE_GDI_MESSAGE_MAP()
public:
	CG2XListBox(void);
	~CG2XListBox(void);

	struct ListItem
	{
		INT iImage;
		CString strText;
		BOOL bEnable;   // ���Ƿ�Ϊ����״̬
		BOOL bSelected; // ���Ƿ�ѡ��
		BOOL bHover;    // �Ƿ�������Ϸ�����
		CRect rcItem;   //
		CG2XWnd* pG2XWnd; // ���G2XUI����ָ��
		INT nItemHeight; // ��ĸ߶�
		DWORD dwData;    // ������
	};

	BOOL Create(CG2XWnd* pParent, LPCRECT lpRect, UINT nID, G2XSortStyle sortStyle=G2XSortStyleNormal);

	// �����Ƿ������ѡ
	void SetEnableMulSelect(BOOL bEnable);

	// �����Ƿ���������ƶ����λ��
	void SetEnableMoveItem(BOOL bEnable);

	// �����ַ���
	void InsertString(INT index, LPCTSTR, INT iImage=-1, BOOL bUpdate=TRUE);

	void SetItemWnd(INT iIndex, CG2XWnd* pWnd);

	void AddString(LPCTSTR, INT iImage=-1, BOOL bUpdate=FALSE);
	
	CString GetString(INT nAt);

	void SetItemData(INT iIndex, DWORD dwData);
	DWORD GetItemData(INT iIndex);

	// �����ַ���
	void EraseString(LPCTSTR);
	void EraseString(INT index);

	// ѡ���ַ���
	void SelectString(LPCTSTR);

	// ѡ��ĳ��
	void SetCurSel(INT index);

	void SetImage(INT index, INT iImage);

	
	
	// ��ȡ������ѡ������listbox�����ѡ����ô���ص�һ����ѡ�����λ��
	INT GetSingleSel();
	
	// ��ȡ�����ѡ�����
	// return: ��ѡ����еĸ���
	INT GetMulSel(IN INT* pResArray, IN const INT nSize);

	INT GetCount();

	// ����������
	void SetSortStyle(G2XSortStyle style);

	G2XSortStyle GetSortStyle();

	void Resort();
	void AdjustListBox();

	void RemoveAll(BOOL bRepaint=TRUE);

	// ����ListBox�ı߿����ָ��Ϊ0��ListBoxû�б߿�
	void SetBorder(INT nBorderBreadth=1);

	void SetItemHeight(INT index, INT nHeight);
	
	ListItem* GetItem(INT index);

protected:
	virtual BOOL CompareItem(const ListItem* item1, const ListItem* item2);

	ListItem* CreateEmptyItem();

protected:
	virtual void DrawControl(Graphics* pGraphics,G2XControlState ctrlState);
	virtual void DrawItem(Graphics* pGraphics, ListItem* pItem);
	INT TestPointItem(const CPoint& point);

	g_msg void OnCreate();
	g_msg void OnSize();
	g_msg void OnDestroy();
	g_msg void OnLButtonDown(UINT nFlags, CPoint point);
	g_msg void OnMouseHover(UINT nFlags, CPoint point);
	g_msg void OnMouseLeave();
	g_msg void OnVScrollNotify(NMHDR* pHdr, LRESULT* lpResult);
	g_msg void OnMouseWheel(UINT nFlags, short zDelta, CPoint point);
	g_msg void OnDragging(UINT nFlag, CPoint ptFrom, CPoint point);
	g_msg void OnDragged(UINT nFlag, CPoint ptFrom, CPoint point);
	g_msg void OnLButtonUp(UINT nFlags, CPoint point);

	virtual void OnItemClicking(INT nItem, UINT nFlags);
	virtual void OnItemClicked(INT nItem, UINT nFlags);

protected:
	std::vector<ListItem*> m_items;
	G2XSortStyle m_sortStyle;
	BOOL m_bEnableMulSelect;
	BOOL m_bEnableMoveItem;
	CG2XScrollBar* m_pVScrollBar;
	ListItem* m_pHoverItem;
	INT m_indexFirstVisual;  // ���ļ��ĵ�һ��
	INT m_indexLastVisual;   // ���ļ������һ��
	G2XUtility::CG2XStatusArray<CString> m_selectStatus;
	INT m_iClickingItem;
	INT m_iOldSel;
};

G2X_END
