#pragma once



// CDodonewView Html ��ͼ

class CDodonewView : public CHtmlView
{
	DECLARE_DYNCREATE(CDodonewView)

protected:
	CDodonewView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CDodonewView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
protected:
	virtual void PostNcDestroy();
public:
	afx_msg void OnNcDestroy();
	void Reflash();
};


