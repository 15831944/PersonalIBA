// TextResouseToolDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"

#include "StringListCtrl.h"
#include "FileAnalyzer.h"

#include "StrProviderLoader.h"

// CTextResouseToolDlg �Ի���
class CTextResouseToolDlg : public CStringLoaderDlg<CDialog>
{
// ����
public:
	CTextResouseToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TEXTRESOUSETOOL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;
	int m_nDefaultTop;
	CString m_strOpenedPath;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAbout();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CStringListCtrl m_listStrings;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedLoadrc();

	BOOL AddItem(StringItem& it);
	afx_msg void OnBnClickedBtnSaveAs();
	afx_msg void OnBnClickedOpen();
	afx_msg LRESULT OnItemChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
	afx_msg void OnSearch();
	afx_msg void OnSearchNext();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnUpdateSave(CCmdUI* pCmdUI);
	afx_msg void OnDelete();
	afx_msg void OnInsert();
	afx_msg void OnAdd();
	afx_msg void OnSetting();
	afx_msg void OnUpdateDelete(CCmdUI* pCmdUI);

	void OnExit();

	CMenu m_menuMain;
protected:
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedSave();
	BOOL Search( CString strKeyword, BOOL bCase, BOOL bWholeMatch,int nLastResult, int& nNewResult);
	afx_msg void OnDropFiles(HDROP hDropInfo);

	void OpenGrcFile(CMyString strFile);
	void OpenRCFile(CString strFile);
	afx_msg void OnNMRclickListString(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	void UpdateMenu(CMenu* pMenu);
};
