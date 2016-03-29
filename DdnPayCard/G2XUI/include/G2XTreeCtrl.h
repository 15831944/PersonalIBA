//==============================================
//= CG2XTreeCtrl ���οؼ�
//=  ���οؼ�ֻ��һ�����ڵ㣬���ڵ�����������������ڵ�û�и��ڵ�
//=  ���ӽڵ�Ľڵ����չ�����۵��ӽڵ��б�, ǰ���ǽڵ��bEnable����ָ��true
////============================================
#pragma once

G2XUI_BEGIN

struct CG2XTreeNode;
typedef std::vector<CG2XTreeNode*> CG2XTreeNodeList;

// ���ڵ�
struct CG2XTreeNode
{
	CG2XTreeNode* pParentNode;				// ���ڵ�, NULL��ʾ�˽ڵ�Ϊ���ڵ�
	CG2XTreeNodeList children;	// �ӽڵ��б�
	INT iIndex;                         // �ڵ��ڸ��ڵ�ĺ��ӽڵ����е�λ��(�ӿ�����ٶ�)
	G2XNodeState state;					// �ڵ��״̬
	INT iImage;							// ͼ���б��е�����
	CString strText;					// �ڵ����������
	DWORD dataPtr;                      // �ڵ������
	CG2XWnd* pNodeWnd;					// �ڵ㴰�ڣ�һ��ΪNULL
	BOOL bEnable;						// �ڵ��Ƿ�������״̬
	BOOL bSelected;
	CRect rc;                           // �ڵ���������
	INT nNodeHeight;                    // �ڵ�����λ�õĸ߶�
};



// ��·��, �հ�·����Ӧ�Ľڵ�Ϊ���ڵ�
// ����: ·��(0->1)��ʾ���ڵ��µĵ�1���ڵ�ĵ�2���ӽڵ�,��������
struct G2X_CLASSES CG2XTreePath : public std::vector<INT>
{

};

class G2X_CLASSES CG2XTreeCtrl : public CG2XControl
{
	DECLARE_DYNCREATE(CG2XTreeCtrl)
	DECLARE_GDI_MESSAGE_MAP()
public:
	CG2XTreeCtrl();
	virtual ~CG2XTreeCtrl();

public:
	// �������οؼ�
	BOOL Create(CG2XWnd* pParent, BOOL bShowRootNode, LPCRECT lpRect, UINT nID);

	// �������οؼ�����ʾ
	void AdjustTree();

public:
	// ��ȡ���ڵ�
	CG2XTreeNode* GetRoot();

	// ����һ���հ׽ڵ�
	CG2XTreeNode* CreateNode();

	// �õ�·����Ӧ�Ľڵ�
	CG2XTreeNode* GetTreeNode(const CG2XTreePath& path);

	// �õ��ڵ��Ӧ��·��
	CG2XTreePath GetPath(CG2XTreeNode* pNode);

	// ɾ��·����Ӧ�Ľڵ�
	void DeleteTreeNode(const CG2XTreePath& path, BOOL bUpdate=TRUE);

	//----------------------------------------
	// ��ӽڵ�
	//- path: ��ӵĽڵ�����·��λ��
	//- bAsChild:TRUE, �½ڵ���Ϊpath��Ӧ�ڵ���ӽڵ㣬���½ڵ㱻��ӵ�ĩβ
	//- bAsChild:FALSE,��pathָ����λ�ô�����ڵ�
	//----------------------------------------
	void AddNode(
		const CG2XTreePath& path, 
		CG2XTreeNode* pNewNode, 
		BOOL bAsChild,
		BOOL bUpdate=TRUE);

	//----------------------------------------
	// ����ڵ�
	//- path: ��pathָ����λ�ô�����ڵ�
	//----------------------------------------
	void InsertNode(
		const CG2XTreePath& path,
		CG2XTreeNode* pNewNode,
		BOOL bUpdate=TRUE);

	//----------------------------------------
	// չ��/�۵��ڵ�
	//- bExpanding:TRUE, չ���ڵ�
	//- bExpanding:FALSE, ����ڵ�
	//----------------------------------------
	void ExpandingNode(const CG2XTreePath& path, BOOL bExpanding, BOOL bUpdate=TRUE);


	//----------------------------------------
	// ����ĳ���ڵ����һ���ڵ�,
	// ע: ���Ա��۵��Ľڵ�(���pPreNode�к��Ӳ��������۵��ģ���ô���ĺ��Ӳ��ᱻ������)
	//- return NULL: �����������
	//----------------------------------------
	CG2XTreeNode* NextNode(CG2XTreeNode* pPreNode);

	//----------------------------------------
	// ����ĳ���ڵ����һ���ڵ�
	// ע: �������κνڵ�(�������нڵ㱻չ��ʱ����NextNodeһ��)
	//     ���ڱ����������нڵ�
	//- return NULL: �����������
	//----------------------------------------
	CG2XTreeNode* NextNodeEx(CG2XTreeNode* pPreNode);

	//----------------------------------------
	// ��ȡ�ڵ������������Ĳ��
	// ע: ���ڵ�Ĳ��=0, �����ڵ�Ĳ� > 0
	//----------------------------------------
	INT GetLayer(CG2XTreeNode* pNode);
	
	//----------------------------------------
	// ��ȡ���б�ѡ��Ľڵ㣬���ؽڵ����
	//- pResults: 
	//   [NULL]: ����ȡ�ڵ�
	//   [!NULL]:���汻ѡ��Ľڵ�
	//- return: ��ѡ��Ľڵ����
	//----------------------------------------
	INT GetSelectedNode(std::vector<CG2XTreeNode*> *pResults=NULL);
	
	//----------------------------------------
	//- ��pathָ����·���µ�ֱϵ�ӽڵ���������
	//----------------------------------------
	void Resort(G2XSortStyle sortStyle,const CG2XTreePath& path);

	//----------------------------------------
	//- �����ڵ�
	//- lptszNodeText:Ҫ�����Ľڵ��ı�
	//- bSearchAll:
	//    [TRUE]: ����ָ��·���µ����нڵ�
	//    [FALSE]: ����ָ��·���µĵ�һ�������к��ӽڵ�
	//- pIn: ��ָ���ڵ�������
	//    [NULL]: �ڸ��ڵ�������
	//----------------------------------------
	CG2XTreeNode* SearchNode(LPCTSTR lptszNodeText, BOOL bSearchAll, CG2XTreeNode* pIn);

	//----------------------------------------
	//- ѡ��ڵ�
	//- bCenterShown:
	//-   [TRUE]: ���й�����������£����ڵ������ʾ�ڽ�����
	//----------------------------------------
	void SelectNode(CG2XTreeNode* pNode, BOOL bSelected=TRUE, BOOL bCenterShown = FALSE);

public:
	//----------------------------------------
	// ���Ե����ڵĽڵ�
	//- return:
	//   [NULL]: �õ㲻λ���κνڵ�
	//----------------------------------------
	virtual CG2XTreeNode* TestPtInNode(LPPOINT);

	//----------------------------------------
	// ���Ե�����ͼ��Ľڵ�,ͼ����ָ�۵�ͼ��
	//- return:
	//   [NULL]: �õ㲻λ���κνڵ��ͼ��λ��
	//----------------------------------------
	virtual CG2XTreeNode* TestPtInIcon(LPPOINT);

	
protected:
	virtual BOOL CompareItem(G2XSortStyle sortStyle,const CG2XTreeNode* item1, const CG2XTreeNode* item2);
	virtual void DrawControl(Graphics* pGraphics,G2XControlState ctrlState);
	virtual void DrawNode(Graphics* pGraphics,CG2XTreeNode* pNode, G2XControlState ctrlState);

	g_msg void OnCreate();
	g_msg void OnDestroy();
	g_msg void OnSize();
	g_msg void OnVScrollNotify(NMHDR* pHdr, LRESULT* lpResult);
	g_msg void OnLButtonDown(UINT nFlags, CPoint point);
	g_msg void OnMouseHover(UINT nFlags, CPoint point);
	g_msg void OnMouseLeave();
	g_msg void OnMouseWheel(UINT nFlags, short zDelta, CPoint point);

private:
	void DeleteNode(CG2XTreeNode*& pNode, BOOL bUpdate=TRUE);
	
public:
	CG2XImageList m_imgList;
protected:
	CG2XTreeNode *m_pRoot;
	CG2XTreeNode *m_pFirstVisualNode;
	CG2XTreeNode *m_pLastVisualNode;
	BOOL m_bShowRootNode;
	
	CG2XScrollBar *m_pVScrollBar;
	INT m_nIndentation;      // �����֮���������
	BOOL m_bNeedIndentation; // ��Ͳ�֮���Ƿ���Ҫ����
	SIZE m_szIcon;           // �۵�/չ��ͼ��Ĵ�С
	BOOL m_bEnableFoldingByClickAnywhere; // �������ڵ���κεط������۵���չ�������ΪFALSE����ôֻ�ܵ��ͼ������۵���չ��
	CG2XTreeNode *m_pHoverNode;
	BOOL m_bMulSelected;     // �����ѡ
};

G2X_END