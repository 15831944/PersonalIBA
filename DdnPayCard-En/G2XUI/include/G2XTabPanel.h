/**************************************************************************
CG2XTabPanel: tab��弯��
	CG2XTabPanel�а�����������, ��ͬһʱ��ֻ����ʾһ����������ϲ�
**************************************************************************/

#pragma once

G2XUI_BEGIN

class G2X_CLASSES CG2XTabPanel : public CG2XPanel
{
	DECLARE_DYNCREATE(CG2XTabPanel)
public:
	CG2XTabPanel();
	virtual ~CG2XTabPanel();

protected:
	DECLARE_GDI_MESSAGE_MAP()

	g_msg void OnCreate();
	g_msg void OnSize();

public:

	//---------------------------------------------------------------------
	//- �������
	//- iIndex: �����λ��, -1��ʾ���뵽β��
	//- pNewPanel: Ҫ��������
	//---------------------------------------------------------------------
	BOOL InsertPanel(INT iIndex, CG2XPanel* pNewPanel);

	//- ��ȡ������
	INT GetCount();

	//- �������
	void ActivePanel(INT iIndex);

	//- ͨ�����Ŀؼ�ID���������
	BOOL ActivePanelByID(UINT nPanelID);

	//- ��ȡ���
	CG2XPanel* GetPanel(INT iIndex);

	//- ͨ�����Ŀؼ�ID����ȡ���
	CG2XPanel* GetPanelByID(UINT nPanelID);

	//- ����������CG2XTabPanel��Ե�ļ�϶
	void SetSpacing(UINT nPixel=2);

	//- ��ȡ����
	CG2XPanel* GetActivePanel();

	//- �Ƴ����, ���ռ�õ���ԴҲ�ᱻɾ��
	BOOL RemovePanel(INT iIndex);

	//- �Ƴ��������
	BOOL RemoveAll();

	//- �������λ��
	BOOL ExchangePanel(INT nPanel1, INT nPanel2);

protected:
	typedef std::vector<CG2XPanel*> PanelList;
	PanelList m_panelList;

	// ������CG2XTabPanel��Ե�Ŀ�϶��С
	INT m_nSpacing;

	INT m_nActivePanel;
};

G2X_END