// TextResouseTool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������

#include "StrProviderLoader.h"

// CTextResouseToolApp:
// �йش����ʵ�֣������ TextResouseTool.cpp
//

class CGlobalParam
{
protected:
	CGlobalParam();
	void LoadParam();
public:
	static CGlobalParam* GetInstance()
	{
		static CGlobalParam param;
		return &param;
	}

	BOOL GetIsNextHit()const;    // �Ƿ���һ������
	void SetIsNextHit(BOOL bVal);

	BOOL GetReplace()const;
	void SetReplace(BOOL bVal);
	BOOL GetMenuBefore()const;
	void SetMenuBefore(BOOL bVal);
	BOOL GetCheckChange();
	void SetCheckChange(BOOL bVal);
	CString GetFormatChars();
	void SetFormatChars(CString strVal);
	BOOL GetForbidEditID();
	void SetForbidEditID(BOOL bVal);

private:
	BOOL m_bIsNextHit;  // ��һ���Ƿ�������
	BOOL m_bReplace;    // ID�ظ�ʱ���Ƿ��滻����
	BOOL m_bMenuBefore; // ����RCʱ���Ƿ��Ƚ����˵�
	BOOL m_bCheckChange; // ���༭����ʱ�������Ƿ����ʽ�����ı�
	CString m_strFormatChars; // ����ʽ���ַ�����
	BOOL m_bForbidEditID; // �Ƿ����ID�ɱ༭
};
class CTextResouseToolApp : public CWinApp
{
public:
	CTextResouseToolApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	HACCEL m_hAccelTable;
	virtual int ExitInstance();
};

extern CTextResouseToolApp theApp;
