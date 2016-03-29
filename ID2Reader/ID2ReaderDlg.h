// ID2ReaderDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "SShowPic.h"
#include <afxmt.h>

#include "G2XThreadTemplate.h"
#include "DelFilePastDayThread.h"
using namespace G2XUtility;

#define WM_UPDATE_DATA (WM_USER+100)

// CID2ReaderDlg �Ի���
class CID2ReaderDlg : public CDialog, public CG2XThreadDelegate
{
// ����
public:
	CID2ReaderDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CID2ReaderDlg();

// �Ի�������
	enum { IDD = IDD_ID2READER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnUIUpdateData(WPARAM wParam, LPARAM lParam);
	CString m_strName;
	CString m_strSex;
	CString m_strNation;
	CString m_strBirthday;
	CString m_strAddress;
	CString m_strIdNumber;
	CString m_strOrg;
	CString m_strOutOfDate;

	CString m_strImgPath;

	// ɾ����־�ļ��߳�
	CDelFilePastDayThread m_threadDelFile;
protected:
	virtual void didInitInstance(CG2XThread* pThread);
	virtual int didExitInstance(CG2XThread* pThread);
	virtual void didRun(CG2XThread* pThread);
	virtual bool didCanExitThread(CG2XThread* pThread);

public:
	// ���������Ϣ
	void Clear();

	BOOL m_bReadOK;
	CString m_strDate;
	CSShowPic m_staticPic;
	afx_msg void OnTimer(UINT nIDEvent);

	DWORD m_dwReserveTime; // ����ʱ��

	CCriticalSection m_cs;
	CG2XThread m_thread;
	BOOL m_bDeviceOpened;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
};
