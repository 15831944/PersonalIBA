#pragma once


#include "IBAFormView.h"
// CNewUserView ������ͼ

class CNewUserView : public CIBAFormView
{
	DECLARE_DYNCREATE(CNewUserView)

public:
	CNewUserView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CNewUserView();

	enum {IDD = IDD_FORMVIEW_NEWUSER};

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

};


