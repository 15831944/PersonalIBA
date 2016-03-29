#pragma once

#include "BCGPToolbarCtrlCommon.h"
#include "DlgIBAFastlane.h"
#include ".\common\ThreadTemplate.h"


class CIBAFastlaneButton :
	public CBCGPToolbarCtrlCommon
{
	DECLARE_SERIAL(CIBAFastlaneButton)
public:
	CIBAFastlaneButton(void);
	CIBAFastlaneButton(CBCGPToolBar *pToolbar);

	virtual ~CIBAFastlaneButton(void);

	virtual void CopyFrom(const CBCGPToolbarButton& src);

protected:
	virtual BOOL CreateCtrlWnd();

	virtual SIZE OnCalculateSize(CDC* pDC, const CSize& sizeDefault, BOOL bHorz);

	CFont m_hFont;
public:
	static const int  DEFAULT_WIDTH = 280;
};

//******************************************************************************

class CIBAFastLane : public CBCGPToolBar
{
public:
	CIBAFastLane();
	virtual ~CIBAFastLane();

	virtual BOOL SaveState(LPCTSTR lpszProfileName = NULL, int nIndex = -1, UINT uiID = (UINT) -1);

	BOOL CreateFastLane(CWnd* pParentWnd);

	void ReplaceButtons();

	virtual BOOL AllowChangeTextLabels () const
	{
		return FALSE;
	}

	virtual void AdjustLayout();

	BOOL ComboHasFocus();

	void ComboSetFocus();

	CString GetComboText();

	BOOL RobFocus(MSG* pMsg);

	HWND GetComboHwnd();

	// ��ȡ����ͨ���ӶԻ���ָ��
	CDlgIBAFastlane* GetDlgFastLane();

	// �����߳�
	void StartThread();

	// �����������ȡ���ձ��㲥���Ѽ�¼���̺߳���
	void ThreadRun();

protected:
	static const UINT CYCLE_TIME = 10*60*1000;  // 10����

	// �̸߳����������ȡ���ձ����Ѽ�¼
	NS_COMMON::CThreadTemplate<CIBAFastLane> m_Thread; 
};