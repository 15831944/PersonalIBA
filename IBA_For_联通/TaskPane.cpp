
#include "stdafx.h"
#include "IBA.h"
#include "MainFrm.h"
#include "IBAHelpper.h"
#include "Cashier.h"
#include "Socket\LocalServer.h"
#include ".\taskpane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CIBATaskPane

BEGIN_MESSAGE_MAP(CIBATaskPane, CBCGPTasksPane)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIBATaskPane construction/destruction

CIBATaskPane::CIBATaskPane()
{
	m_nUserInfoGroup = -1;
	m_nUserInfoTask = -1;
	m_nUserInfoList = -1;
	m_nUserInfoButton = -1;
	m_bShowUseList = FALSE;

	m_strQucikNoteFile = theApp.GetWorkPath() + _T("\\IBAConfig\\QuickNote.dat");
}

CIBATaskPane::~CIBATaskPane()
{
}

/////////////////////////////////////////////////////////////////////////////
// CIBATaskPane message handlers

int CIBATaskPane::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPTasksPane::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CIBAString strTmp;
	strTmp.LoadString(IDS_STANDARD);
	SetCaption(strTmp);

	EnableNavigationToolbar(FALSE);
	EnableWrapLabels(TRUE);
	EnableOffsetCustomControls(FALSE);

	CIBAHelpper::CreateFont(m_Font, 13);

	if (!CreateBroser())
	{
		TRACE0("Failed to create the custom window\n");
		return -1;      // fail to create
	}

	strTmp.LoadString(IDS_SYSMSG);
	int nPage1Gr2 = AddGroup(strTmp);
	AddWindow(nPage1Gr2, m_WebBrowser.GetSafeHwnd(), 150);

	strTmp.LoadString(IDS_USERINFO);
	m_nUserInfoGroup = AddGroup(strTmp, FALSE, TRUE);

	//ShowCashierInfo();

	if (!CreateEditControl())
	{
		TRACE0("Failed to create the custom window\n");
		return -1;      // fail to create
	}

	INT nControlHeight = 135;

	INT nHeight = GetSystemMetrics(SM_CYSCREEN);

	//nControlHeight += (nHeight - 768);

	nControlHeight = 150;

	strTmp.LoadString(IDS_QUICKNOTES);
	int nPage1Gr3 = AddGroup(strTmp, TRUE);
	AddWindow(nPage1Gr3, m_wndEdit.GetSafeHwnd(), nControlHeight);

	return 0;
}

BOOL CIBATaskPane::CreateEditControl()
{
	CRect rectDummy(0, 0, 0, 0);
	DWORD dwEditStyle = WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL;

	if (!m_wndEdit.Create(dwEditStyle, rectDummy, this, (UINT)-1))
	{
		TRACE0("Failed to create the edit window\n");
		return FALSE;
	}

	//m_wndEdit.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	m_wndEdit.SetFont(GetFont());
	
	CString strMsg;

	if (CIBAHelpper::ReadFile(m_strQucikNoteFile, strMsg))
	{
		m_wndEdit.SetWindowText(strMsg);
	}

	return TRUE;
}

void CIBATaskPane::OnDestroy()
{
	CString strMsg;

	m_wndEdit.GetWindowText(strMsg);
	
	CIBAHelpper::WriteFile(m_strQucikNoteFile, strMsg);

	CBCGPTasksPane::OnDestroy();
}

BOOL CIBATaskPane::CreateBroser()
{
	CRect rect;

	if (!m_WebBrowser.Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, this, NULL, NULL))
	{
		TRACE0("Failed to create the WebBrowser window\n");
		return FALSE;
	}

	CString strURL;
	strURL.Format(_T("http://%s:%d/%s/advertiseplay/advertise.jsp?netBarId=%d&posId=4&colCount=2"),
		CNetBarConfig::GetInstance()->GetMainCenterIP(), 
		CNetBarConfig::GetInstance()->GetMainCenterPort(),
		CNetBarConfig::GetInstance()->GetMainCenterPath(),
		CNetBarConfig::GetInstance()->GetNetBarId());

	CIBALog::GetInstance()->Write(strURL);
	
	m_WebBrowser.Navigate(strURL);

	return TRUE;
}

void CIBATaskPane::ShowUserInfo(CComputerInfo & ComputerInfo)
{
	CString strContent;

	if (ComputerInfo.HasUser())
	{
		CActiveMember ActiveMember;

		if (CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(
			ComputerInfo.GetMemberID(), ActiveMember))
		{
			ShowUserInfo(ActiveMember);
		}
	}
	else
	{
		m_nUserInfoTask = -1;
		m_nUserInfoButton = -1;
		m_nUserInfoList = -1;
		m_bShowUseList = FALSE;

		RemoveAllTasks(m_nUserInfoGroup);

		ShowCashierInfo();
	}
}

void CIBATaskPane::ShowUserInfo(CActiveMember & ActiveMember)
{
	CString strContent;
	
	CString strUseTime = ActiveMember.GetCheckInTimeAsString();
	if (!strUseTime.IsEmpty())
	{
		strUseTime.AppendFormat(_T("(%s)"), ActiveMember.GetPayTypeAsString());
	}

		/*L"����:%s\r\n"
		L"�˺�:%s\r\n"
		L"�û�����:%s\r\n"
		L"֤������:%s\r\n"
		L"\r\n"
		L"\r\n"
		L"��������:%s\r\n"
		L"��������:%s\r\n"

		L"ʹ��ʱ��:%s\r\n"
		L"ʹ��ʱ��:%s\r\n"
		L"\r\n"
		L"\r\n"
		L"���ý��:%s\r\n"
		L"���ѽ��:%s\r\n",*/

	strContent.Format(LOAD_STRING(IDS_TASK_INFO),
		ActiveMember.GetUserName(),
		ActiveMember.GetNetId(),
		ActiveMember.GetUserClassName(),
		ActiveMember.GetPersonalID(),
		ActiveMember.GetTerminalID(),
		ActiveMember.GetPCClass(),
		strUseTime,
		ActiveMember.GetUseTimeAsString(),
		ActiveMember.GetAvailavleBalanceAsString(),
		ActiveMember.GetAmountAsString());

	m_ActiveMember = ActiveMember;

	ShowUserInfo(strContent);
}

void CIBATaskPane::ShowUserInfo(CString strContent)
{
	if (m_nUserInfoTask == -1)
	{
		RemoveAllTasks(m_nUserInfoGroup);

		m_nUserInfoTask = AddLabel(m_nUserInfoGroup, strContent);

		AddSeparator(m_nUserInfoGroup);

		m_nUserInfoButton = AddTask(m_nUserInfoGroup, _T("�ϻ���¼>>"), -1, IDM_AGENT);

		AddSeparator(m_nUserInfoGroup);

		m_nUserInfoList = AddLabel(m_nUserInfoGroup, _T(""));
	}
	else
	{
		SetTaskName(m_nUserInfoGroup, m_nUserInfoTask, strContent);		
		SetTaskName(m_nUserInfoGroup, m_nUserInfoList, _T(""));
		
		SetTaskName(m_nUserInfoGroup, m_nUserInfoButton, _T("�ϻ���¼>>"));
		m_bShowUseList = FALSE;
	}

	RecalcLayout();
}

void CIBATaskPane::OnClickTask(int nGroupNumber, int nTaskNumber, UINT uiCommandID, DWORD dwUserData)
{
	if (IDM_AGENT == uiCommandID)
	{
		if (m_bShowUseList)
		{
			SetTaskName(m_nUserInfoGroup, m_nUserInfoList, _T(""));
			SetTaskName(m_nUserInfoGroup, m_nUserInfoButton, _T("�ϻ���¼>>"));
		}
		else
		{
			CWaitCursor wait;

			CString strTmp(QueryConsume());

			SetTaskName(m_nUserInfoGroup, m_nUserInfoList, strTmp);

			SetTaskName(m_nUserInfoGroup, m_nUserInfoButton, _T("<<�ϻ���¼"));
		}

		RecalcLayout();

		m_bShowUseList = !m_bShowUseList;
	}
}

CString CIBATaskPane::QueryConsume()
{	
	CString strTmp, strRet;

	CConsumeListMethod ConsumeListMethod;
	
	ConsumeListMethod.SetConsumeType(1);

	if (m_ActiveMember.IsMember())
	{
		ConsumeListMethod.SetBeginTime(m_ActiveMember.GetActivationTime().Format(_T("%Y%m%d%H%M%S")));
	}

	ConsumeListMethod.SetMemberId(m_ActiveMember.GetMemberID());

	theApp.GetCurCashier()->DoConsumeList(ConsumeListMethod);

	UINT nCount = 0;

	if (ConsumeListMethod.GetStatusCode() == 0)
	{
		for (INT i = 0; i < (INT)ConsumeListMethod.GetRecordCount(); i++)//���3��
		{
			if (ConsumeListMethod.GetBeginTime(i) == ConsumeListMethod.GetEndTime(i))
			{
				continue; //ʱ��һ����������̨��ʱ
			}

			//ʱ��
			strTmp = ConsumeListMethod.GetBeginTime(i);
			strTmp.Insert(2, _T("-"));
			strTmp.Insert(5, _T(" "));
			strTmp.Insert(8, _T(":"));
			strTmp = strTmp.Left(strTmp.GetLength() - 2);

			strRet.AppendFormat(_T("��ʼʱ��:%s\r\n"), strTmp);

			strTmp = ConsumeListMethod.GetEndTime(i);
			strTmp.Insert(2, _T("-"));
			strTmp.Insert(5, _T(" "));
			strTmp.Insert(8, _T(":"));
			strTmp = strTmp.Left(strTmp.GetLength() - 2);

			strRet.AppendFormat(_T("����ʱ��:%s\r\n"), strTmp);

			strRet.AppendFormat(_T("ʹ���ն�:%s\r\n"), ConsumeListMethod.GetBusinessName(i));

			nCount++;

			if (nCount < 2)
			{
				strRet.Append(_T("\r\n"));
			}
			else
			{
				break;
			}
		}
	}

	if (strRet.IsEmpty())
	{
		strRet.AppendFormat(_T("û���ϻ���¼��\r\n\r\n"));
	}

	return strRet;
}

void CIBATaskPane::ShowCashierInfo()
{
	CString strTmp;

	//\r\n����Ա:%s\r\n\r\n�ϰ�ʱ��:%s\r\n\r\n
	//\r\n����Ա:%s\r\n\r\n����û�ϰ࣬�����ϰ࣡\r\n\r\n

	if (theApp.GetCurCashier()->IsOnDuty())
	{
		strTmp.Format(LOAD_STRING(IDS_CASHER_ON_FMT),
			theApp.GetCurCashier()->GetName(),
			theApp.GetCurCashier()->GetOnDutyTime());
	}
	else
	{
		strTmp.Format(LOAD_STRING(IDS_CASHER_OFF_FMT),
			theApp.GetCurCashier()->GetName());
	}

	AddLabel(m_nUserInfoGroup, strTmp);

	RecalcLayout();
}

//BOOL CIBATaskPane::SaveState(LPCTSTR lpszProfileName /*= NULL*/, int nIndex /*= -1*/, UINT uiID /*= (UINT) -1*/)
//{
//	//return CBCGPTasksPane::SaveState(lpszProfileName, nIndex, uiID);
//
//	return TRUE;
//}
//
//BOOL CIBATaskPane::LoadState(LPCTSTR lpszProfileName /*= NULL*/, int nIndex /*= -1*/, UINT uiID /*= (UINT) -1*/)
//{
//	return FALSE;
//}
void CIBATaskPane::OnLButtonDown(UINT nFlags, CPoint point)
{
	CBCGPTasksPane::OnLButtonDown(nFlags, point);
}
