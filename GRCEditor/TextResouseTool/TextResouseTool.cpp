// TextResouseTool.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "TextResouseTool.h"
#include "TextResouseToolDlg.h"

#include "GxxString.h"
#include ".\textresousetool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTextResouseToolApp

BEGIN_MESSAGE_MAP(CTextResouseToolApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTextResouseToolApp ����

CTextResouseToolApp::CTextResouseToolApp()
{
	m_hAccelTable = NULL;
}


// Ψһ��һ�� CTextResouseToolApp ����

CTextResouseToolApp theApp;


// CTextResouseToolApp ��ʼ��

BOOL CTextResouseToolApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

	CWinApp::InitInstance();

	CGxxString<TCHAR> str;
	TCHAR ch[1024] = {0};
	GetModuleFileName(m_hInstance, ch, 1024);
	str = ch;
	str.CutTail(_T('\\'));
	CString strDll = str + _T("\\StrProvider.dll");
	CString strGrc = str + _T("\\GrcEditor.grc");

	if (!CStrProviderLoader::GetInstance()->Loading(strDll, strGrc))
	{
		MessageBox(NULL,_T("GrcEditor.grc��ȡʧ�ܣ������޷�����"),_T("GrcEditor"),MB_OK|MB_ICONERROR);
		return FALSE;
	}

	AfxEnableControlContainer();

	m_hAccelTable = LoadAccelerators(m_hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CTextResouseToolDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȷ�������ر�
		//�Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//�Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	// ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

BOOL CTextResouseToolApp::PreTranslateMessage(MSG* pMsg)
{
	if (m_hAccelTable) {
		if (::TranslateAccelerator(m_pMainWnd->GetSafeHwnd(), m_hAccelTable, pMsg)) {
			return(TRUE);
		}
	}

	return CWinApp::PreTranslateMessage(pMsg);
}

int CTextResouseToolApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}


//-------------------------------------------------------------------------

CGlobalParam::CGlobalParam()
{
	LoadParam();
}

void CGlobalParam::LoadParam()
{
	m_bIsNextHit = theApp.GetProfileInt(_T("UI"), _T("IsNextHit"), (int)TRUE); // Ĭ����һ������
	m_bReplace = theApp.GetProfileInt(_T("UI"), _T("Replace"), (int)TRUE); // Ĭ���滻
	m_bMenuBefore = theApp.GetProfileInt(_T("UI"), _T("MenuBefore"), (int)TRUE); // Ĭ���滻
	m_bCheckChange = theApp.GetProfileInt(_T("UI"), _T("CheckChange"), (int)TRUE); // Ĭ���滻
	m_strFormatChars = theApp.GetProfileString(_T("UI"), _T("Formatchars"), _T("|;%;")); // Ĭ���滻
	m_bForbidEditID = theApp.GetProfileInt(_T("UI"), _T("ForbidEditID"), (int)TRUE); // Ĭ���滻
}

BOOL CGlobalParam::GetIsNextHit() const
{
	return m_bIsNextHit;
}

void CGlobalParam::SetIsNextHit(BOOL bVal)
{
	m_bIsNextHit = bVal;
	theApp.WriteProfileInt(_T("UI"), _T("IsNextHit"), bVal);
	
}

BOOL CGlobalParam::GetReplace() const
{	
	return m_bReplace;
}	

void CGlobalParam::SetReplace(BOOL bVal)
{
	m_bReplace = bVal;
	theApp.WriteProfileInt(_T("UI"), _T("Replace"), bVal);
}

void CGlobalParam::SetMenuBefore(BOOL bVal)
{
	m_bMenuBefore = bVal;
	theApp.WriteProfileInt(_T("UI"), _T("MenuBefore"), bVal);
}

BOOL CGlobalParam::GetMenuBefore() const
{
	return m_bMenuBefore;
}

BOOL CGlobalParam::GetCheckChange()
{
	return m_bCheckChange;
}

void CGlobalParam::SetCheckChange(BOOL bVal)
{
	m_bCheckChange = bVal;
	theApp.WriteProfileInt(_T("UI"), _T("CheckChange"), bVal); // Ĭ���滻
}

CString CGlobalParam::GetFormatChars()
{
	return m_strFormatChars;
}

void CGlobalParam::SetFormatChars(CString strVal)
{
	m_strFormatChars = strVal;
	theApp.WriteProfileString(_T("UI"), _T("Formatchars"), strVal); // Ĭ���滻
}

BOOL CGlobalParam::GetForbidEditID()
{
	return m_bForbidEditID;
}

void CGlobalParam::SetForbidEditID(BOOL bVal)
{
	m_bForbidEditID = bVal;
	theApp.GetProfileInt(_T("UI"), _T("ForbidEditID"), bVal); // Ĭ���滻
}