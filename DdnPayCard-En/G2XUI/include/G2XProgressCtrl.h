#pragma once

G2XUI_BEGIN

//========================================
// CG2XProgressCtrl
//    ���ȿؼ�, �������Ľ��ȷ�Χ0~100
//    ��Ϊ������ָ���ܲ�����
// ע��
//    
//    ���д�CG2XWnd��̳е�����������Ӷ��з��䣬
//    ��Ҫ��CG2XWnd����ʹ��delete �ؼ��֣�
//    CG2XWnd�����������Ҫ���� DestroyWnd����
//========================================
class G2X_CLASSES CG2XProgressCtrl : public CG2XWnd
{
	DECLARE_DYNCREATE(CG2XProgressCtrl)
	DECLARE_GDI_MESSAGE_MAP()
public:
	CG2XProgressCtrl();
	virtual ~CG2XProgressCtrl();

	BOOL Create(CG2XWnd* pParent,G2XProgressStyle style,LPCRECT lpRect, UINT nID);

protected:
	virtual void DrawSelf(Gdiplus::Graphics *pGraphics);
	virtual void DrawBallProgress(
		Gdiplus::Graphics *pGraphics, 
		LPRECT lpWhere, 
		BYTE nBallCount,
		BYTE nBallPassed,
		BYTE nBallPassing);

	void DrawBall(Gdiplus::Graphics *pGraphics, CPoint ptCenter, INT nDistance, INT nRadius, float fAngle, Color& clr1, Color& clr2,INT nStatus);

	g_msg void OnCreate();
	g_msg void OnTimer(UINT nEvent);

public:
	// �����ܲ���
	void SetStepCount(INT nCount);

	// ��ȡ�ܲ���
	INT GetStepCount();

	// �õ���ǰ�Ľ��Ȱ�ֱ�
	BYTE GetCurrentProgress();

	// ����ǰ��һ��
	void StepIt();

	// ��ȡ��ǰ����
	INT GetStep();

	// ���õ�ǰ����
	void SetStep(INT nStep);

	void SetShowPercentage(BOOL bShow, BOOL bRepaint=TRUE);

protected:
	G2XProgressStyle m_style;
	BYTE m_nProgress; // ��ǰ����, ��ʼΪ0
	INT m_nStepCount; // �ܲ���
	INT m_nCurrentStep;   // ��ǰ����
	UINT m_nTimerID;
	BYTE m_nBallCount; // ��ĸ�����G2XProgressStyleBalls���
	INT m_nHoverBall;
	BOOL m_bShowPercentage; // �Ƿ���ʾ�ٷֱ�
};

G2X_END