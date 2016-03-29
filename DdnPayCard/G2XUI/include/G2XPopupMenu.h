#pragma  once

G2XUI_BEGIN

class G2X_CLASSES CG2XPopupMenu;
struct G2XPopupMenuItem
{
	UINT nID;			// �˵���ID, 0��ʾ�ָ���
	CString strText;	// �˵����ı�
	INT iImage;			// ͼ������
	CG2XPopupMenu *pChildMenu; // �Ӳ˵�
	BOOL bEnable;
};

#define ID_G2XUI_CHILD_MENU (UINT(-1))

class G2X_CLASSES CG2XPopupMenuListBox : public CG2XListBox
{
	DECLARE_GDI_MESSAGE_MAP()
public:
	CG2XPopupMenuListBox();
	virtual void DrawItem(Graphics* pGraphics, ListItem* pItem);

	g_msg void OnMouseHover(UINT nFlag, CPoint point);
	g_msg void OnDestroy();

	INT m_nInsdention; // ��������������
	INT m_nInsdentionRight; // �����ұ�����������

	//- ��ȡlistbox��ȫ���������������ʵĴ�С
	CSize GetBestSize();
};

class G2X_CLASSES CG2XPopupMenu : public G2XUIFrame::CG2XFoundationWindow
{
	DECLARE_MESSAGE_MAP()
public:
	CG2XPopupMenu();

	//---------------------------------------------------------------------
	//- ����
	//- bAutoWidth: �˵��Ŀ���Ƿ��Զ���Ӧ
	//-    [FALSE]: nWidth��Ч
	//- nWidth: bAutoWidthΪFALSEʱ,�˵��Ĺ̶����
	//- pParentWnd: �˵��ĸ�����, ���ղ˵�������
	//- pOwnerWnd: �˵��������ߵ�G2XUI����, �˵��������¼��ᷢ�͸���
	//- pParentMenu: ���˵�
	//---------------------------------------------------------------------
	BOOL Create(BOOL bAutoWidth, INT nWidth,CWnd* pParentWnd, CG2XWnd* pOwnerWnd, CG2XPopupMenu* pParentMenu=NULL);

	//- �˵��ĳ�ʼ������
	virtual void OnInitialUpdate();

public:

	//- ����˵���, ��Ч��nIndexЧ�������AppendMenuItemЧ��һ��
	void InsertMenuItem(INT nIndex, const G2XPopupMenuItem& item);

	//---------------------------------------------------------------------
	//- �����Ӳ˵�
	//- nIndex: �����λ��, ��Ч��λ�ñ�ʾ���뵽ĩβ
	//- lpText: �Ӳ˵��ı���
	//- pChildPopup: �Ӳ˵���ָ��, �Ӳ˵��������ʹ��CreateChildPopupMenu������
	//- iImage: �Ӳ˵����ͼ������, �˵���Ҫ�ѵ���SetImageList
	//---------------------------------------------------------------------
	void InsertPopupMenu(INT nIndex, LPCTSTR lpText, CG2XPopupMenu* pChildPopup, INT iImage=-1);

	//- ����ָ���
	void InsertSeparator(INT nIndex);

	//- ��Ӳ˵���
	void AppendMenuItem(const G2XPopupMenuItem& item);
	
	//---------------------------------------------------------------------
	//- ��Ӳ˵���
	//- nID:
	//   [ID_SEPARATOR]: ��ʾ�ָ���
	//   [ID_G2XUI_CHILD_MENU]: �Ӳ˵�
	//   [����]: �˵���ID
	//- iImage: ͼ������, �˵���Ҫ�ѵ���SetImageList
	//---------------------------------------------------------------------
	void AppendMenuItem(LPCTSTR lpText, UINT nID, INT iImage=-1); 

	//---------------------------------------------------------------------
	//- ����Ӳ˵�
	//- lpText: �Ӳ˵��ı���
	//- pChildPopup: �Ӳ˵���ָ��, �Ӳ˵��������ʹ��CreateChildPopupMenu������
	//- iImage: �Ӳ˵����ͼ������, �˵���Ҫ�ѵ���SetImageList
	//---------------------------------------------------------------------
	void AppendPopupMenu(LPCTSTR lpText, CG2XPopupMenu* pChildPopup, INT iImage=-1);

	//- ���һ���ָ���
	void AppendSeparator();

	//- ��ʾ�˵�,������Ļ������λ��
	void ShowPopupMenu(INT nPosX, INT nPosY);

	//- ����ͼ���б�
	void SetImageList(const CG2XImageList& imgList);
	
	//- ����һ������ʽ���Ӳ˵�
	CG2XPopupMenu* CreateChildPopupMenu();

	//- ��ȡ����Ĳ˵�
	CG2XPopupMenu* GetTopMenu();

	//- ���ز˵�
	//- pSelf: ����pSelf�˵��µ������Ӳ˵�
	//- bSelfHide: �Ƿ�Ҳ��������
	//- pExcept: ����pExceptָ��Ĳ˵������أ��������Ӳ˵�ȫ������
	void HideChildMenu(CG2XPopupMenu* pSelf, BOOL bSelfHide, CG2XPopupMenu* pExcept=NULL);

	//- ͨ���˵���ID��ȡ�˵����ڲ˵����λ��, �����������Ӳ˵�, ��Ч�Ĳ˵����-1
	INT CommandToIndex(UINT nID);

	G2XPopupMenuItem* GetMenuItem(INT nIndex);

	//- ��ȡ�˵���
	INT GetMenuItemCount();

	//- �����˵���
	BOOL EraseMenuItem(INT iIndex);
	
	//- �����˵���
	BOOL EraseMenuItemByID(UINT nID);

	//- ���û����ò˵���
	BOOL EnableMenuItem(INT iIndex, BOOL bEnable);
	
protected:
	void DoLoseFocus();
	CG2XPopupMenu* FindMenu(CG2XPopupMenu* pTop, HWND hWnd);

protected:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	g_msg void OnListBoxClick();

protected:
	CG2XPopupMenuListBox* m_pListBox;

public:
	BOOL m_bAutoWidth;
	INT m_nMenuWidth;
	CG2XWnd* m_pG2XOwner;
	CG2XPopupMenu* m_pParentMenu;
	BOOL m_bMenuShown;
};

G2X_END