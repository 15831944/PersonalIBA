#include "StdAfx.h"
#include "resource.h"
#include ".\ToolbarLabel.h"

#include ".\Cashier.h"
#include "Socket\LocalServer.h"
#include "IBA.h"
//******************************************************************************

IMPLEMENT_SERIAL(CIBAFastlaneButton,CBCGPToolbarCtrlCommon,1)

CIBAFastlaneButton::CIBAFastlaneButton(void)
{
}

CIBAFastlaneButton::CIBAFastlaneButton(CBCGPToolBar *pToolbar)
:CBCGPToolbarCtrlCommon( ID_FASTLANEDLG,
						CImageHash::GetImageOfCommand(ID_FASTLANEDLG),emBCGP_customize,pToolbar)
{

}

CIBAFastlaneButton::~CIBAFastlaneButton(void)
{
}

void CIBAFastlaneButton::CopyFrom(const CBCGPToolbarButton& src)
{
	CBCGPToolbarCtrlCommon::CopyFrom(src);
}

BOOL CIBAFastlaneButton::CreateCtrlWnd()
{
	if( m_pWndCtrl )
	{
		m_pWndCtrl->DestroyWindow();
		delete m_pWndCtrl;
		m_pWndCtrl = NULL;
	}

	CDlgIBAFastlane *pDlg = new CDlgIBAFastlane(m_pWndParent);
	BOOL bRes = pDlg->Create(CDlgIBAFastlane::IDD,m_pWndParent);
	m_pWndCtrl = pDlg;
	return bRes;
}

SIZE CIBAFastlaneButton::OnCalculateSize(CDC* pDC, const CSize& sizeDefault, BOOL bHorz)
{
	CRect rc;
	m_pWndParent->GetClientRect(rc);
	CSize sz;
	sz.cy = sizeDefault.cy;
	sz.cx = rc.Width() - 1;
	return CBCGPToolbarButton::OnCalculateSize(pDC,sz,bHorz);
}

//***************************************************************************


CIBAFastLane::CIBAFastLane()
{

}

CIBAFastLane::~CIBAFastLane()
{

}

BOOL CIBAFastLane::SaveState(LPCTSTR lpszProfileName /*= NULL*/, int nIndex /*= -1*/, UINT uiID /*= (UINT) -1*/)
{
	return TRUE;
}

void CIBAFastLane::ReplaceButtons()
{
	CIBAFastlaneButton fbtn(this);
	ReplaceButton(ID_FASTLANEDLG, fbtn);
}

BOOL CIBAFastLane::CreateFastLane(CWnd* pParentWnd)
{
	EnableLargeIcons(FALSE); //�Ƿ�ǿ��ΪСͼ�깤����
	
	m_bTextLabels = FALSE;

	if (!CreateEx(pParentWnd, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		/*| CBRS_GRIPPER*/ | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1, 1, 1, 1),
		AFX_IDW_TOOLBAR + 5) || !LoadToolBar(IDR_FASTDLG/*IDR_FASTLANE*/))
	{
		TRACE0("Failed to create toolbar\n");
		return FALSE;      // fail to create
	}
	//ID_FASTLANEDLG


	CIBAString strTmp;
	strTmp.LoadString(IDS_FASTLANE);
	SetWindowText(strTmp);

	return TRUE;
}

void CIBAFastLane::AdjustLayout()
{
	m_bTextLabels = FALSE;

	CBCGPToolBar::AdjustLayout();
}

void CIBAFastLane::ComboSetFocus()
{
	HWND hComb = GetComboHwnd();

	if (hComb)
	{
		::SendMessage(hComb, WM_SETFOCUS, NULL, NULL);
	}	
}

CString CIBAFastLane::GetComboText()
{
	CString strTmp;

	CBCGPToolbarButton* pBtn = GetButton(CommandToIndex(ID_FASTLANECOMBO));

	if (pBtn != NULL)
	{
		CBCGPToolbarComboBoxButton *pComboBox = (CBCGPToolbarComboBoxButton*)pBtn;

		strTmp = pComboBox->GetText();
		pComboBox->SetText(_T(""));
	}

	return strTmp;
}

BOOL CIBAFastLane::ComboHasFocus()
{
	BOOL bRet = FALSE;
	
	CWnd *pWnd = GetFocus();
	if( pWnd )
	{
		pWnd = pWnd->GetParent();
		if (pWnd&&pWnd->GetSafeHwnd() == GetComboHwnd())
		{
			bRet = TRUE;
		}
	}

	return bRet;
}

HWND CIBAFastLane::GetComboHwnd()
{
	CDlgIBAFastlane *pDlg = GetDlgFastLane();
	if (pDlg)
	{
		return pDlg->m_combFast.GetSafeHwnd();
	}

	return NULL;
}


BOOL CIBAFastLane::RobFocus(MSG* pMsg)
{	
	//������س�����ַ����̲���ʱ
	
	if (WM_CHAR == pMsg->message && VK_RETURN != pMsg->wParam)
	{
		if (!ComboHasFocus() && AfxGetMainWnd()->IsWindowEnabled())
		{
			TCHAR szTmp[50] = {0};
			
			GetClassName(pMsg->hwnd, szTmp, 49);
			
			//��Ӧ��Ϣ�Ĵ��岻��Edit����Button��ʱ������
			if (CString(_T("Edit")).CompareNoCase(szTmp) != 0 
				&& CString(_T("Button")).CompareNoCase(szTmp) != 0
				&& CString(_T("Internet Explorer_Server")).CompareNoCase(szTmp) != 0)
			{
				ComboSetFocus();//ȡ�ý���
				//���ͽ�����Ϣ
				::PostMessage(GetComboHwnd(), WM_CHAR, pMsg->wParam, pMsg->lParam);

				return TRUE;
			}
		}
	}

	return FALSE;
}

CDlgIBAFastlane* CIBAFastLane::GetDlgFastLane()
{
	CIBAFastlaneButton *pBtn = (CIBAFastlaneButton*)GetButton(0);
	if(!pBtn)
	{
		ASSERT(0);
		return NULL;
	}

	CDlgIBAFastlane *pDlg = (CDlgIBAFastlane*)pBtn->GetCtrlWnd();
	if (!pDlg)
	{
		ASSERT(0);
		return NULL;
	}
	if (!IsWindow(pDlg->GetSafeHwnd()))
	{
		ASSERT(0);
		return NULL;
	}

	return pDlg;
}

void CIBAFastLane::ThreadRun()
{
	while(TRUE)
	{
		{
		CString strs;

		CDlgIBAFastlane *pDlg = GetDlgFastLane();
		if (!pDlg)
		{
			ASSERT(0);
			return;
		}

//#define MY_DEBUG
#ifdef MY_DEBUG
		strs += CString(_T("��Ա�û� 0001 12:00 ����12.00")) + _T("|");
		strs += CString(_T("��Ա�û� 0002 12:05 ����8.00")) + _T("|");
		strs += CString(_T("��Ա�û� 0003 12:10 ����5.00")) + _T("|");
		strs += CString(_T("��Ա�û� 0004 12:11 ����9.00")) + _T("|");
		strs += CString(_T("��Ա�û� 0005 12:12 ����7.00")) + _T("|");
#else
		
		CConsumeListMethodEx clMethod;
		if (!theApp.GetCurCashier()->DoConsumeListEx(clMethod))
		{
			Sleep(CYCLE_TIME);
			continue;
		}
		if (clMethod.GetStatusCode()!=0)
		{
			Sleep(CYCLE_TIME);
			continue;
		}

		for(int i = 0; i < (int)clMethod.GetRecordCount() ; i++)
		{
			CString strTxt;
			UINT nMemberID = clMethod.GetMemberID(i);

			CActiveMember AM;
			CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(nMemberID,
				AM);

			strTxt += AM.GetUserClassName() + _T(" ");
			strTxt += AM.GetUserName() + _T(" ");

			CString strTime = clMethod.GetCostTime(i);
			strTime.Insert(2, _T("-"));
			strTime.Insert(5, _T(" "));
			strTime.Insert(8, _T(":"));
			strTime.Insert(11, _T(":"));

			strTxt += strTime + _T(" ");

			strTxt += clMethod.GetBusinessName(i) + _T(" ");

			strTxt.AppendFormat(_T("���:%.2f"),clMethod.GetAmount(i)/100.0);

			strs += strTxt+_T("|");
		}
#endif
		pDlg->SendMessage(WM_UPDATEROLLTEXT, WPARAM((LPCTSTR)strs.GetString()),0);

		} // ��}��Ŀ��: �ֲ������Զ��ͷ�; ���ӵĽ����:�ر�IBA��,��������ڴ�й¶.
		Sleep(CYCLE_TIME);
	}
}

void CIBAFastLane::StartThread()
{
	// 2011/07/18-8201-gxx: ��֤Ϊ��Ч�Ĵ���
	if (m_hWnd != NULL)
	{
		m_Thread.SetCallBack(this,ThreadRun);
		m_Thread.Start();
	}	
}

//---------------------------------------------------------------------