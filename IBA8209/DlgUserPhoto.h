#pragma once
#include "afxwin.h"

#include "SShowPic.h"
// CDlgUserPhoto �Ի���

class CDlgUserPhoto : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgUserPhoto)

public:
	CDlgUserPhoto(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgUserPhoto();

// �Ի�������
	enum { IDD = IDD_DLG_USER_PHOTO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
public:
	CSShowPic m_staticPhoto;
};
