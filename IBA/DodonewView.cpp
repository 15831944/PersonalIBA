// DodonewView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "DodonewView.h"
#include ".\dodonewview.h"
#include "Cashier.h"
#include "Encrypt.h"


// CDodonewView

IMPLEMENT_DYNCREATE(CDodonewView, CHtmlView)

CDodonewView::CDodonewView()
{
}

CDodonewView::~CDodonewView()
{
}

void CDodonewView::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDodonewView, CHtmlView)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()


// CDodonewView ���

#ifdef _DEBUG
void CDodonewView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CDodonewView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG


// CDodonewView ��Ϣ�������

void CDodonewView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();

	//this->Navigate(_T("http://www.duduniu.cn"));

	UINT nNetbarID = CNetBarConfig::GetInstance()->GetNetBarId();
	CString strCasherID = theApp.GetCurCashier()->GetName();
	CString strPwd = theApp.GetCurCashier()->GetPassword();
	CString strLinkPage = _T("http://www.duduniu.cn/iba.jsp");

	CString strUrl = _T("http://");
	strUrl += CNetBarConfig::GetInstance()->GetQueryHost();

	CString strCodePwd;
	CEncrypt::CalcMD5(strPwd,strCodePwd);
	strCodePwd.MakeLower();
	
	strUrl.AppendFormat( _T("/netbar/common/thirdpartdodonew.jsp?netBarId=%d&userId=%s&password=%s&linkPage=%s"),
		nNetbarID, strCasherID, strCodePwd, strLinkPage);
	strUrl += _T("&flag=duduniu");

	this->Navigate2(strUrl);
	
	CIBALog::GetInstance()->WriteFormat(_T("�����ձ���ַ:%s"),strUrl);
}

void CDodonewView::Reflash()
{
	UINT nNetbarID = CNetBarConfig::GetInstance()->GetNetBarId();
	CString strCasherID = theApp.GetCurCashier()->GetName();
	CString strPwd = theApp.GetCurCashier()->GetPassword();
	CString strLinkPage = _T("http://www.duduniu.cn/iba.jsp");

	CString strUrl = _T("http://");
	strUrl += CNetBarConfig::GetInstance()->GetQueryHost();

	CString strCodePwd;
	CEncrypt::CalcMD5(strPwd,strCodePwd);
	strCodePwd.MakeLower();

	strUrl.AppendFormat( _T("/netbar/common/thirdpartdodonew.jsp?netBarId=%d&userId=%s&password=%s&linkPage=%s"),
		nNetbarID, strCasherID, strCodePwd, strLinkPage);
	strUrl += _T("&flag=duduniu");

	this->Navigate2(strUrl);
}

void CDodonewView::OnDestroy()
{
	//CHtmlView::OnDestroy();

	CView::OnDestroy();
}

void CDodonewView::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//CHtmlView::OnClose();
	theApp.GetIBAView()->PostMessage(WM_CHILDCLOSE, GetDlgCtrlID(), (LPARAM)GetSafeHwnd());
}

void CDodonewView::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���

	CHtmlView::PostNcDestroy();
}

void CDodonewView::OnNcDestroy()
{
	CHtmlView::OnNcDestroy();

	// TODO: �ڴ˴������Ϣ����������
}
