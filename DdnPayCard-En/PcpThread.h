#pragma once

class CMainWnd;
// CPcpThread

class CPcpThread : public CWinThread
{
	DECLARE_DYNCREATE(CPcpThread)
	DECLARE_MESSAGE_MAP()

protected:
	CPcpThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CPcpThread();

	virtual BOOL InitInstance();
	virtual int ExitInstance();

	CString m_strParams;

	CMainWnd* m_lpMainWnd;

	static CPcpThread* m_lpThis;

public:
	static HWND m_hMainWnd;

public:

	static BOOL CreatePcp(LPCTSTR lpszParams);

	static BOOL DestoryPcp();

	static BOOL ShowPcp();

	static BOOL HidePcp();
};
