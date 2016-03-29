#pragma once


G2XUI_BEGIN

enum G2XBorderStyle
{
	G2XBorderNone,				// û�б߿�
	G2XBorderInner,				// �ڱ߿�, (�߿��ڿؼ�������)
	G2XBorderOuter				// ��߿�, (�߿��ڿؼ�������)
};
// �ؼ�����ʾ״̬, �ؼ���ͼʱ��Ҫ������4��״̬
enum G2XControlState
{
	G2XControlStateNormal = 0,  // �ؼ�������ʾ״̬
	G2XControlStateHover = 1,   // �ؼ����λ���Ϸ�������״̬
	G2XControlStateSinkage = 2, // �ؼ��³�״̬,����갴�����û���ͷ�ʱ��״̬
	G2XControlStateDisable = 3  // �ؼ�������״̬
};

#define G2XControlStateCount 4  // �ؼ���״̬����Ϊ4��

// �ı�ˮƽ���뷽ʽ
enum G2XTextHAlign
{
	G2XTextHAlignLeft = 0,
	G2XTextHAlignCenter = 1,
	G2XTextHAlignRight = 2
};

// �ı���ֱ������뷽ʽ
enum G2XTextVAlign
{
	G2XTextVAlignTop = 0,
	G2XTextVAlignMiddle = 1,
	G2XTextVAlignBottom = 2
};

// �������ֿؼ��ķ��
enum G2XRollTextStyle
{
	G2XRollTextStyleTop = 0,    // ���ִ������Ϲ���
	G2XRollTextStyleBottom =1,  // ���ִ������¹���
	G2XRollTextStyleLeft =2,    // ���ִ����������
	G2XRollTextStyleRight = 3   // ���ִ������ҹ���
};

// ��ť���
enum G2XButtonStyle
{
	G2XButtonStylePushButton,         // ͨ�õİ�ť��ʽ,����������
	G2XButtonStyleLink,               // ������ʽ�İ�ť
	G2XButtonStyleAlphaButton,        // ��Alphaͨ����button,���ı�,Ĭ�ϱ���ɫ��͸����
	G2XButtonStyleImageButton,        // ͼ��ť,û������,ͼ��������䵽��ť����������
	G2XButtonStyleImageButtonEx       // ��չ��ͼ��ť����ť���ұ����ı�,��ť������͸����
};

// ���������
enum G2XProgressStyle
{
	G2XProgressStyleGeneral,          // һ����ͨ��ʽ
	G2XProgressStyleXP,               // XP��ʽ�Ľ�����
	G2XProgressStyleWin7,             // WIN7��ʽ�Ľ�����
	G2XProgressStyleBalls,            // ���ζ����״��̬������
	G2XProgressStyleCustum            // �Զ���
};

// ϵͳ��ť�����
enum G2XSysBtnBarStyle
{
	G2XSysBtnBarStyleCustom,          // �Զ�������Ҫ����CG2XSysBtnBar::DrawCustomButton����
	G2XSysBtnBarStyleQQSoftMgr,       // ��QQ���Թܼҵ��������һ���İ�ť���
	G2XSysBtnBarStyleQQ2012           // ��QQ2012һ���İ�ť���
};

// ������
enum G2XGradientStyle
{
	G2XGradientStyleNull,             // û���κν���Ч��
	G2XGradientStyleVLiner,           // ��ֱ�������Խ���,��ָ��2����ɫֵ
	G2XGradientStyleVBlock,           // ��ֱ����ֳ�����������, ������ֱ𽥱�,��ָ��4����ɫֵ
	G2XGradientStyleHLiner,           // ˮƽ�������Խ���,��ָ��2����ɫֵ
	G2XGradientStyleHBlock,           // ˮƽ����ֳ�����������,������ֱ𽥱�,��ָ��4����ɫֵ
	G2XGradientStyleEllipseInner,     // �ڲ���Բ����,��ָ��������ɫֵ,�߽���ɫ��������ɫ
	G2XGradientStyleEllipseOuter      // �ⲿ��Բ����,��ָ��������ɫֵ,�߽���ɫ��������ɫ
};

// ����пؼ��ķ�����ʽ
enum G2XPanelPlace
{
	G2XPanlePalceNormal,       // �ؼ����ֲ���

	G2XPanelPlaceFollowRight,  // �ؼ�λ�ø�������ұ߽�
	G2XPanelPlaceFollowBottom, // �ؼ�λ�ø�������±߽�
	G2XPanelPlaceFollowRB,     // �ؼ�λ�ø���������½�

	G2XPanelPlaceAutoWidth,    // �ؼ�λ�ò��仯������Զ��仯���߶ȱ��ֲ���
	G2XPanelPlaceAutoHeight,   // �ؼ�λ�ò��仯����Ȳ��仯���߶��Զ��仯
	G2XPanelPlaceAutoSize,     // �ؼ�λ�ò��仯���ؼ���С�Զ��仯

	G2XPanelPlaceFollowAutoWidth, // �ؼ�λ�ø�������±߽粢�ҿؼ�����Զ��仯,�߶Ȳ��仯
	G2XPanelPlaceFollowAutoHeight// �ؼ�λ�ø�������ұ߽粢�ҿؼ��߶��Զ��仯����Ȳ��仯
};

// ���������
enum G2XCaptionBarStyle
{
	G2XCaptionBarStyleMinBox = 0x0001,
	G2XCaptionBarStyleMaxBox = 0x0002,
	G2XCaptionBarStyleCloseBox = 0x0004,
	G2XCaptionBarStyleSysButton = (G2XCaptionBarStyleMinBox|G2XCaptionBarStyleMaxBox|G2XCaptionBarStyleCloseBox)
};

// �ɷָ��������ָ���ʽ
enum G2XSplitPanelStyle
{
	G2XSplitPanelStyleHorizontal, // ˮƽ����������
	G2XSplitPanelStyleVertical    // ��ֱ����������
};

enum G2XSplitPanelSizeStyle
{
	G2XSplitPanelSizeStylePercentage,// �ָ����Ĵ�С�ı�ʱ��2�����Ŀ�Ȱ������仯
	G2XSplitPanelSizeStyleFirst,     // �ָ����Ĵ�С�ı�ʱ����һ�����Ŀ�ȸ���仯�����ڶ������Ŀ�Ȳ���
	G2XSplitPanelSizeStyleSecond     // �ָ����Ĵ�С�ı�ʱ���ڶ������Ŀ�ȸ���仯������һ�����Ŀ�Ȳ���
};

// ������
enum G2XSortStyle
{
	G2XSortStyleNormal=0,   // ������
	G2XSortStyleDesc=-1,    // ����
	G2XSortStyleAsce=1      // ����
};

// �ڵ�״̬
enum G2XNodeState
{
	G2XNodeStateNone,     // û���ӽڵ�
	G2XNodeStateExpanded, // �ӽڵ��б���չ����
	G2XNodeStateFolded    // �ӽڵ��б����۵�������
};

G2X_END