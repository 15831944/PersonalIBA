#pragma once
#include "AnalyseTextAttribute.h"

G2XUI_BEGIN

/*-------------------------------------------------------------------------
CG2XStaticEx : public CG2XStatic

- ��չ�ľ�̬�ı��ؼ�, ֧����ʽ�ı���ֻ���ڵ���SetWindowTextʱ��ָ���ı���ʽ��
  ��ο�SetWindowText��˵��������ֱ�ӵ���CG2XStaticEx::SetWindowText�Ż�������
-------------------------------------------------------------------------*/
class G2X_CLASSES CG2XStaticEx : public CG2XStatic
{
	DECLARE_DYNCREATE(CG2XStaticEx)
	DECLARE_GDI_MESSAGE_MAP()
public:
	CG2XStaticEx(void);
	~CG2XStaticEx(void);

	//---------------------------------------------------------------------
	//- bMulLines: �Ƿ�֧�ֶ��У������֧���ı��еĻس���������
	//- hAlign: �ı���ˮƽ���뷽ʽ
	//- vAlign: �ı��Ĵ�ֱ���뷽ʽ
	//---------------------------------------------------------------------
	BOOL Create(
		CG2XWnd* pParent, 
		LPCTSTR lpWndText, 
		LPCRECT lpRect, 
		UINT nID, 
		BOOL bMulLines=FALSE, 
		G2XTextHAlign hAlign=G2XTextHAlignLeft, 
		G2XTextVAlign vAlign=G2XTextVAlignMiddle);

	//---------------------------------------------------------------------
	//- strText: �ı���ʽ
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
	// ע: ����ı��к��лس���,����ת���� &#13;, ���з���Ҫת���� &#10;
	// <font> �� <text> ֧��Ƕ��
	//---------------------------------------------------------------------
	void SetWindowText(LPCTSTR strText, BOOL bRepaint=TRUE);

	virtual void DrawControl(Graphics* pGraphics,G2XControlState ctrlState);

public:
	CAnalyseTextAttribute m_textAttrs;
	BOOL m_bMulLines;
};

G2X_END
