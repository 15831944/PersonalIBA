// ShortMsgView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "ShortMsgView.h"
#include ".\shortmsgview.h"
#include "IBAHelpper.h"
#include "Cashier.h"
#include "Dal\IBADAL.h"
#include "ShortMsgDlg.h"
#include "Socket\LocalServer.h"

// CShortMsgView

IMPLEMENT_DYNCREATE(CShortMsgView, CIBAFormView)

CShortMsgView::CShortMsgView()
: CIBAFormView(CShortMsgView::IDD)
, m_strResult(_T(""))
{
}

CShortMsgView::~CShortMsgView()
{
}

void CShortMsgView::DoDataExchange(CDataExchange* pDX)
{
	CIBAFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MSG, m_MsgList);
	DDX_Control(pDX, IDC_BUTTON_STARTTIME, m_btnStartTime);
	DDX_Control(pDX, IDC_BUTTON_ENDTIME, m_btnEndTime);
	DDX_Text(pDX, IDC_STATIC_RESULT, m_strResult);
}

BEGIN_MESSAGE_MAP(CShortMsgView, CIBAFormView)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, OnBnClickedButtonQuery)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	//ON_WM_CLOSE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MSG, OnNMDblclkListMsg)
END_MESSAGE_MAP()

// CShortMsgView ��Ϣ�������

void CShortMsgView::OnBnClickedButtonQuery()
{
	QueryShortMsgList();
}

void CShortMsgView::OnInitialUpdate()
{
	CIBAFormView::OnInitialUpdate();

	InitListView();
	InitTimeCtrl();

	GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC_RESULT)->SetFont(&m_Font);
}

HBRUSH CShortMsgView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CIBAFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	if (CTLCOLOR_STATIC == nCtlColor)
	{
		if (pWnd->GetDlgCtrlID() == IDC_STATIC_RESULT)
		{
			pDC->SetTextColor(RGB(0, 0, 255));
		}
	}
	
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void CShortMsgView::InitListView()
{
	CIBAString strHeader;
	strHeader.LoadString(IDS_SHORTMSG_HEAD);
	//strHeader = L" ʱ��|�ն˺�|IP��ַ|�û���|��Ϣ";

	CStringArray strArray;
	CIBAHelpper::SplitLine(strHeader, strArray);

	const UINT Width[] = {180, 80, 120, 80, 400};

	for (INT i = 0; i < strArray.GetCount(); i++)
	{
		m_MsgList.InsertColumn(i, strArray.GetAt(i), LVCFMT_LEFT, Width[i]);
	}

	m_MsgList.SetFont(&m_Font);
	m_MsgList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}

// ��ʼ��ʱ��ؼ�
BOOL CShortMsgView::InitTimeCtrl(void)
{
	const UINT stateMask = 
		CBCGPDateTimeCtrl::DTM_SPIN |
		CBCGPDateTimeCtrl::DTM_DROPCALENDAR | 
		CBCGPDateTimeCtrl::DTM_DATE |
		CBCGPDateTimeCtrl::DTM_TIME24H |
		CBCGPDateTimeCtrl::DTM_CHECKBOX |
		CBCGPDateTimeCtrl::DTM_TIME | 
		CBCGPDateTimeCtrl::DTM_TIME24HBYLOCALE;

	UINT stateFlags = 
		CBCGPDateTimeCtrl::DTM_SPIN |
		CBCGPDateTimeCtrl::DTM_DROPCALENDAR | 
		CBCGPDateTimeCtrl::DTM_DATE |
		CBCGPDateTimeCtrl::DTM_TIME24H |
		CBCGPDateTimeCtrl::DTM_TIME;  

	m_btnStartTime.m_monthFormat = 2;
	m_btnStartTime.SetState(stateFlags, stateMask);
	m_btnStartTime.SetFont(&m_Font);
	m_btnStartTime.SizeToContent();

	m_btnEndTime.m_monthFormat = 2;
	m_btnEndTime.SetState(stateFlags, stateMask);
	m_btnEndTime.SetFont(&m_Font);
	m_btnEndTime.SizeToContent();
	
	return TRUE;
}

void CShortMsgView::OnSize(UINT nType, int cx, int cy)
{
	MoveWindow(0, 0, cx, cy, FALSE);

	if (m_MsgList.GetSafeHwnd() != NULL)
	{
		m_MsgList.MoveWindow(10, 100, cx - 20, cy - 105, TRUE);
		//����ʱ������
		InitTimeDate();
	}
}

// ��ʼ��ʱ��ؼ�������
BOOL CShortMsgView::InitTimeDate(void)
{
	//�趨ʱ��
	COleDateTime tTime(0, 0, 0, 0, 0, 0);
	//��ʼʱ��
	if (theApp.GetCurCashier()->IsOnDuty())
	{
		tTime.ParseDateTime(theApp.GetCurCashier()->GetOnDutyTime());
	}
	else
	{
		tTime = COleDateTime::GetCurrentTime();
		tTime += COleDateTimeSpan(-1, 0, 0, 0);
	}
	m_btnStartTime.SetDate(tTime);
	//����ʱ��
	tTime = COleDateTime::GetCurrentTime();
	tTime += COleDateTimeSpan(1, 0, 0, 0);
	m_btnEndTime.SetDate(tTime);
	
	return TRUE;
}

// �����Ϣ�б�
INT CShortMsgView::FillMsgList(void)
{
	m_MsgList.DeleteAllItems();

	////////////////////////��ѯ����ε���Ϣ////////////////////
	CClientShortMsgArray  MsgArray;

	CIBADAL::GetInstance()->GetClientShortMsg(MsgArray, 
		m_btnStartTime.ToString(), m_btnEndTime.ToString());

	for (INT ii = 0; ii < MsgArray.GetCount(); ii++)
	{
		CClientShortMessage strMsgInfo = MsgArray.GetAt(ii);

		m_MsgList.InsertItem(0, strMsgInfo.GetRevTime());
		CString strTmp;
		strTmp.Format(_T("%d"), strMsgInfo.GetMemberId());
		m_MsgList.SetItemText(0, 3, strTmp);
		m_MsgList.SetItemText(0, 2, strMsgInfo.GetTermIP());
		m_MsgList.SetItemText(0, 1, strMsgInfo.GetTermID());
		m_MsgList.SetItemText(0, 4, strMsgInfo.GetMsgBody());
	}

	return MsgArray.GetCount();
}

void CShortMsgView::OnClose()
{
	INT nIdx = theApp.GetIBAView()->FindTab(GetSafeHwnd());
	theApp.GetIBAView()->GetTabControl().ShowTab(nIdx, FALSE);

	m_MsgList.DeleteAllItems();
	m_strResult.Empty();

	UpdateData(FALSE);

	//CIBAFormView::OnClose();
}

// ��ѯ��Ϣ
void CShortMsgView::QueryShortMsgList(void)
{
	CIBAString strTmp, strNum;
	strTmp.LoadString(IDS_SHORTMSGRESULT);

	strNum.Format(strTmp, FillMsgList());

	GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(strNum);
}

void CShortMsgView::OnNMDblclkListMsg(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	INT nIdx = m_MsgList.GetSelectionMark();

	if (nIdx >= 0)
	{
		CString strTermId, strTermIP;
		strTermId = m_MsgList.GetItemText(nIdx, 1);
		strTermIP = m_MsgList.GetItemText(nIdx, 2);

		CShortMsgDlg MsgDlg;
		MsgDlg.AddReceiver((UINT)CLocalServer::GetInstance()->ComputerList.Find(strTermId, strTermIP));
		MsgDlg.DoModal();
	}

	*pResult = 0;
}
