#pragma once
#include <vector>

G2XUI_BEGIN

#define G2X_INVALID_COLOR (COLORREF(-1))

struct G2X_CLASSES CG2XTextNode
{
	CG2XTextNode();

	CString strFontName; // ��������,���Ϊ�ձ���ΪĬ������
	BYTE nFontSize;      // �����С
	CString strFontStyle;// ������
	COLORREF clrText;    // ������ɫ
	
	COLORREF clrBk;      // ������ɫ,G2X_INVALID_COLOR��ʾ͸��


	INT nSelectFrom;	 // �ı���ѡ�����
	INT nSelectTo;		 // �ı���ѡ���յ�
	BYTE nReturnChar;	 // �������еĵ�һ���ڵ��Ƿ��лس���
	Font * pFont;		 // ����
	INT nCX;			 // �ڵ�ռ�õĿ��
	INT nCY;			 // �ڵ�ռ�õĸ߶�
	INT nCharCount;      // �ַ��ĸ���
	
protected:
	CString strText;     // �ı�

public:
	void RefreshSize();
	void ClearSelect();

	CString& GetText() { return strText; }
	void SetText(LPCTSTR lpszText);

	BOOL IsEmptyNode() { return strText.IsEmpty(); }

	INT GetLengthByFrom();
	INT GetLengthByTo();

	CString GetSelectString();
};

class G2X_CLASSES CAnalyseTextAttribute;

//-------------------------------------------------------------------------
// CAnalyseTextAttribute
//   ����ʽ�ı��ķ����ͻ���
//-------------------------------------------------------------------------
class G2X_CLASSES CAnalyseTextAttribute
{
public:
	CAnalyseTextAttribute(void);
	~CAnalyseTextAttribute(void);

	//- ��ȡ�ַ�������Ӧ���ַ�������
	static INT GetLegthByCharCount(LPCTSTR lpszText, INT nCharCount);

	//- ��ȡ�ַ����е��ַ�����, ���ۺ��ֻ�����ĸ������Ϊ�ǵ����ַ�
	static INT GetCharCount(LPCTSTR lpszText);

	static SIZE MessageString(LPCTSTR, Font* pFont);

	//---------------------------------------------------------------------
	//- lpszText: �ı���ʽ
	//    ֧�ֱ�ǩ��: 
	//       <font>...</font>
	//          ����:
	//             name: "��������", ����: name="����"
	//             size: �����С, ����: size="16"
	//             style: ���,����: style="bold,underline,italic"
	//          ע: �����ָ������,�ı�ʹ�ÿؼ�������ʹ�С
	//
	//       <text>..</text>
	//          ����:
	//             color: ������ɫ, ����: color="RGB(0,0,255"
	//             bk-color: "���ֱ�����ɫ, ����: bk-color="RGB(255,255,255)"
	//          ע: �����ָ������,�ı���ɫĬ��Ϊ��ɫ, ������ɫĬ��Ϊ͸��
	//
	//- pG2XWnd: �������CG2XWnd
	//- bIgnoreReturn: �Ƿ���Իس���
	//
	// ע: ����ı��к��лس���,����ת���� &#13;, ���з���Ҫת���� &#10;
	// <font> �� <text> ֧��Ƕ��
	//---------------------------------------------------------------------
	BOOL Analyse(LPCTSTR lpszText, CG2XWnd* pG2XWnd, BOOL bIgnoreReturn = FALSE);

	//- �Զ����з���
	void AnalyseAutoReturn();

	//- ������ڵ������,(��ԭʼ�Ľڵ�����)
	int GetCount();

	//---------------------------------------------------------------------
	// �����ı�
	//- G2XTextHAlign: ˮƽ���뷽ʽ
	//- G2XTextVAlign: ��ֱ���뷽ʽ
	//- G2XControlState: �ؼ�״̬
	//---------------------------------------------------------------------
	virtual void DrawText(Graphics* pGraphics, LPRECT lpRect, G2XTextHAlign, G2XTextVAlign, G2XControlState);

	//---------------------------------------------------------------------
	// �����ı�( �Զ����� )
	//- G2XTextHAlign: ˮƽ���뷽ʽ
	//- G2XTextVAlign: ��ֱ���뷽ʽ
	//- G2XControlState: �ؼ�״̬
	//---------------------------------------------------------------------
	virtual void DrawTextEx(Graphics* pGraphics, LPRECT lpRect, G2XTextHAlign, G2XTextVAlign, G2XControlState);

	//- �����ı���СҪ����Ĵ�С
	CSize CalcSize();

	//- ��ȡ�����ı�,���Ա�ǩ
	CString GetText();

	//- ��ȡ�����ַ����е�ѡ����ı�
	CString GetSelectedString();
	
	//- ��ȡ�ı���ѡ��Χ
	//- pAbsFrom: ȡ�þ��Ե��ַ���ʼλ��
	//- pAbsTo: ȡ�þ��Ե��ַ�����λ��
	void GetSel(INT * pAbsFrom, INT * pAbsTo);

	//- ����ѡ����ı�
	//- nAbsFrom: ���Ե���ʼ�ַ�λ��
	//- nAbsTo: ���ԵĽ����ַ�λ��
	void SetSel(INT nAbsFrom, INT nAbsTo);

	//- �滻ѡ����ı�
	void ReplaceSelected(LPCTSTR lpstr);

	//- ���ѡ��״̬
	BOOL ClearSel();

	//- �Ƴ���ѡ����ı�
	BOOL RemoveSelected();

	//- ��ָ��λ�ô������ַ���
	BOOL InsertString(INT nAbsWhere, LPCTSTR lpszText);

	void SetSelToHome();
	void SetSelToEnd();

	//- �����˸��������
	BOOL DoBackspace();

	//- ����س���������
	BOOL DoEnter();

	//- ����DEL��������
	BOOL DoDelete();

private:
	void AnalyseText(IXMLDOMNode* pNode, IXMLDOMNode* pNodeText, IXMLDOMNode* pNodeFont);
	
	
protected:
	COLORREF GetColor(LPCTSTR lpszColor, LPCTSTR lpszAttrName);
	DOMNodeType GetNodeType(IXMLDOMNode* pNode);
	CString GetNodeName(IXMLDOMNode* pNode);
	CString GetAttributeValue(IXMLDOMNode* pNode, LPCTSTR lpszAttriName);

public:
	typedef std::vector<CG2XTextNode> NodeArray;	// �ڵ�����
	typedef std::vector<INT> IntArray;				// ��������
	typedef NodeArray NodeLine;						// �ڵ���
	typedef std::vector<NodeLine> LineArray;		// ������
	

	NodeArray m_arrayTextNodes; // �����󣬱���������ı��ڵ�

	LineArray m_arrayLines;    // ÿһ�еĽڵ�����
	IntArray m_arrayHeight;     // ÿһ�е��и�
	IntArray m_arrayWidth;      // ÿһ�е��п�

	//-�Զ����������·���ÿһ�е���Ϣ
	LineArray m_arraysNodeEx;
	IntArray m_arrayHeightEx;     // ÿһ�е��и�
	IntArray m_arrayWidthEx;      // ÿһ�е��п�
	

	BOOL m_bIgnoreReturn;       // �Ƿ���Իس���, Ĭ�ϲ�����

	CG2XWnd* m_pG2XWnd;

	BOOL m_bAutoReturn;
	INT m_nAutoReturnWidth;		// �Զ�������ʱ�Ŀ�Ȳο�

	BYTE m_nLineSpacing;		// �м��

	COLORREF m_clrDefaultText;	// Ĭ�ϵ��ı���ɫ

	INT m_nCursorAt;            // �������λ��

	COLORREF m_clrCursor;
	BOOL m_bShowCursor;
	UINT m_nTotalCharCount;		// �ܹ����ַ�����

	INT m_nDefaultFontHeight;

public:
	//- ��ȡ��ѡ���ַ����Ľڵ�
	void GetSelectedNodes(NodeArray& nodes);

	void InitializeBlankNode(CG2XTextNode& tn);

private:
	INT GenerateNodes(INT nOffX, INT nMaxWidth, CG2XTextNode* pNode,LineArray& lines,CG2XTextNode*& pLast);

	BOOL CreateFontForNode(CG2XTextNode& node);

	void GenerateLines();

	void GenerateSize();

	void GenerateWidthAndHeight();
};

G2X_END

