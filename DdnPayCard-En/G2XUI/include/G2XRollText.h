#pragma once
#include "g2xcontrol.h"

G2XUI_BEGIN

class G2X_CLASSES CG2XRollText :
	public CG2XControl
{
	DECLARE_DYNCREATE(CG2XRollText)
	DECLARE_GDI_MESSAGE_MAP()
public:
	CG2XRollText(void);
	virtual ~CG2XRollText(void);

	static const INT DEFAULT_LINE_SPACE=5;       // Ĭ���м������
	static const INT DEFAULT_COLUMN_SPACE = 50;  // Ĭ���м������

	void Create(
		CG2XWnd* pParent,
		CRect rcInParent, 
		UINT nID,
		G2XRollTextStyle style=G2XRollTextStyleTop);

	// �����м��, ֻ�Թ������G2XRollTextStyleTop,G2XRollTextStyleBottom��Ч
	void SetLineSpace(INT nLineSpace);

	// �����м��, ֻ�Թ������G2XRollTextStyleLeft,G2XRollTextStyleRight��Ч
	void SetColumnSpace(INT nColumnSpace);

	// ��껮��ʱ���Ƿ�ֹͣ����
	void SetHoverStop(BOOL bHoverStop);

	G2XRollTextStyle GetStyle();

	// �����ּ���ĩβ��Ӹ����ַ����������ַ�����tchSp�ָ�ɶ���ַ���
	void AppendComplexString(LPCTSTR,TCHAR tchSp=_T('\n'));

	// �����ּ���ĩβ����ַ���
	void AppendString(LPCTSTR);

	//
	void InsertString(LPCTSTR,INT nAt=0);

	void SetStartPosition(INT nPos);

	// 
	void Update();

protected:
	g_msg void OnCreate();
	g_msg void OnMouseHover(UINT nFlags, CPoint point);
	g_msg void OnTimer(UINT nEvent);
	g_msg void OnMouseLeave();
	g_msg void OnSize();
	virtual void DrawControl(Graphics* pGraphics,G2XControlState ctrlState);

protected:
	G2XRollTextStyle m_style;
	INT m_nLineSpance;   // �м��
	INT m_nColumnSpance; // �м��
	BOOL m_bHoverStop;   // �Ƿ�������껮��ʱֹͣ����
	BOOL m_bIsStop;      // �Ƿ�Ϊֹͣ״̬ 
	INT m_nOffSet;
	INT m_nStartPos;
	Bitmap *m_pBmp;
	std::vector<CString> m_strArray;

};

G2X_END
