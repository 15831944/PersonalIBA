#pragma once

//========================================
// CG2XFoundationWnd
//   G2XLib��Ļ���������, CWnd�н�����G2XLib UI����Ķ���G2XUI����Ӧ��ʹ��
//   CG2XFoundationWnd�����
// CG2XFoundationWnd����������ɣ� �������Ϳͻ���
//   ������: CG2XCaptionBar����
//   �ͻ���: CG2XPanel����
//========================================

G2XUI_BEGIN
G2X_FRAME

class G2X_CLASSES CG2XFoundationWnd : public CG2XWnd
{
	DECLARE_DYNCREATE(CG2XFoundationWnd)
	DECLARE_GDI_MESSAGE_MAP()
public:
	CG2XFoundationWnd();
	virtual ~CG2XFoundationWnd();

protected:
	CG2XCaptionBar* m_pCaptionBar; // ������
	CG2XPanel* m_pClientArea;      // �ͻ��������
};
G2X_END
G2X_END