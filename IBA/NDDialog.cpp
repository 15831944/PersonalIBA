#include "stdafx.h"
#include "IBAHelpper.h"
#include "Cashier.h"
#include "IBA.h"
#include "MainFrm.h"
#include "DialogManager.h"
#include "RealName\IBARealName.h"
#include "ActiveMemberView.h"
#include "Socket\LocalServer.h"
#include ".\NDDialog.h"
#include "Dal\IBADAL.h"

HWND CNDDialog::hWndPre = NULL;

CNDDialog::CNDDialog(UINT nIDTemplate, CWnd *pParent/*= NULL*/)
: CIBADialog(nIDTemplate, pParent)
, m_bFromScan(FALSE), m_nMemberId(0), m_nCardId(0)
, m_bIsMember(FALSE), m_bOnLine(FALSE), m_nUserClassID(0)
, m_bAutoQuery(FALSE)/*, m_bNDHasLocalQuery(FALSE)*/
{
	m_nMoneyOnlineAmount = 0;
	m_nMoneyBalanceW = 0;
	m_nMoneyBalanceY = 0;
	m_nMoneyBalanceZ = 0;
	m_nMoneyAllInAccount=0;
	m_nMoneyGuaranty=0;

	EnableVisualManagerStyle();

	if (m_strNetId.IsEmpty())
	{
		m_strNetId = CCurrentNetBarUser::GetInstance()->GetNetId();
	}

	m_nDlgTemplateID = nIDTemplate;
	//m_bCenterDisconnect = FALSE;

}

CNDDialog::~CNDDialog()
{
}

void CNDDialog::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_IDCARDSNO, m_strNetId);
}

BEGIN_MESSAGE_MAP(CNDDialog, CIBADialog)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, OnBnClickedButtonQuery)
	ON_BN_CLICKED(IDC_BUTTON_SCANRECO, OnBnClickedButtonScanreco)
	ON_COMMAND(IDM_SCAN1, OnScan1)
	ON_COMMAND(IDM_SCAN2, OnScan2)
	ON_COMMAND(IDM_READ2, OnRead2)
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CNDDialog::OnOK()
{
	//CIBADialog::OnOK();
}

void CNDDialog::OnCancel()
{
	CIBADialog::OnCancel();
}

void CNDDialog::SetNetId(CString newVal, BOOL bAutoQuery)
{
	if (newVal.IsEmpty())
	{
		//m_strNetId = theApp.GetLastNetID();
		m_strNetId = CCurrentNetBarUser::GetInstance()->GetNetId();
	}
	else
	{
		m_strNetId = newVal;
	}

	m_bAutoQuery = bAutoQuery;
}

/*************************************************
Function:       // CNDDialog::OnInitDialog
Description:    // CIBADialog::OnInitDialog���г�ʼ���á�
				// ���ݵ�ǰʵ�����ã���ʾ/����ScanReco(ɨ��)��ť
Calls:          // None
Table Accessed: // None
Table Updated:  // None
Input:          // None
Output:         // None
Return:         // None
Others:         // None
*************************************************/
BOOL CNDDialog::OnInitDialog()
{
	CIBADialog::OnInitDialog();

	CBitmap bmp;
	bmp.LoadBitmap(IDB_NETBARUSER);

	m_Images.Create(16, 16, ILC_COLOR24 | ILC_MASK, 0, 0);
	m_Images.Add(&bmp, RGB(250, 50, 250));

	CWnd* pScanBtn = GetDlgItem(IDC_BUTTON_SCANRECO);

	if (pScanBtn != NULL)
	{ 
		pScanBtn->EnableWindow(theApp.GetRealName()->IsScanEnabled());

		if (CNetBarConfig::GetInstance()->GetMustScan() == 2)
		{
			pScanBtn->ShowWindow(SW_HIDE);
		}
	}

	return TRUE;  
}

BOOL CNDDialog::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message && VK_RETURN == pMsg->wParam) //���س�
	{
		CWnd* pWnd = GetFocus(); //����δ�

		if (pWnd != NULL)
		{
			for (INT i = 0; i < m_FocusEnterMap.GetCount(); i++)
			{
				if (pWnd->GetDlgCtrlID() == m_FocusEnterMap.GetAt(i).nFocusID)
				{
					if (GetDlgItem(m_FocusEnterMap.GetAt(i).nEnterID)->IsWindowEnabled())
					{
						return PostMessage(WM_COMMAND, MAKEWPARAM(m_FocusEnterMap.GetAt(i).nEnterID, BN_CLICKED)); //ģ�ⰴť�����¼�
					}
					else
					{
						return TRUE;
					}
				}			
			}
		}
	}

	return CIBADialog::PreTranslateMessage(pMsg);
}

/*************************************************
Function:       // CNDDialog::CheckCard
Description:    // ��NetId��ΪSerialNumִ��DoCheckCard.
				//  ��ע�� - OnOldUser��ӵ���CNDDialog::QueryBalance ִ��DoBalance��ѯ��ΪDialog����Ա��ֵ. ����m_ActiveMember. ��ִ���������д���º���.
				//  δע�� - OnNewUser()�����ж���.Ӧ����ִ���������д���º���.
				//  ���粻ͨ - DoLocalModeQuery().��m_ActiveMember���Dialog����Ա.��NetId��ʾ�ڹ������С����m_bNDHasLocalQuery=TRUE��
Calls:          // DoCheckCard, OnOldUser/OnNewUser/DoLocalModeQuery
Table Accessed: // None
Table Updated:  // None
Input:          // strNetId - ����SerialNum��CheckCard
Output:         // None
Return:         // None
Others:         // OnNewUser��OnOldUser��Ϊvirtual����
*************************************************/
void CNDDialog::CheckCard(LPCTSTR strNetId)
{
	CIDCheckCardMethod checkCard;      //��NetId��ΪSerialNum����CheckCard
	checkCard.SetSerialNum(strNetId);

	theApp.GetCurCashier()->DoCheckCard(checkCard);

	const INT nStatus = checkCard.GetStatusCode();

	if (nStatus == 0 || nStatus == 75) //У��ɹ������û��Ѿ�����
	{
		if (checkCard.GetMemberId() != 0) //����û��Ѿ�ע�ᣬDoBalance���÷��ؽ����Dialog����Ա��ֵ��
		{
			m_nMemberId = checkCard.GetMemberId();
			m_nCardId = checkCard.GetCardId();

			OnOldCard(m_nCardId, m_nMemberId);	//��ӵ���CNDDialog::QueryBalance ִ��DoBalance��ѯ��ΪDialog����Ա��ֵ. ����m_ActiveMember.			
		}
		else // �û�δע�ᣬ�¿�ע��(�����������Ա����,�����Dialog��Ա��ֵ������m_ActiveMember?)
		{
			OnNewCard(m_nCardId);
		}
	}
	//else if (nStatus == -1)
	//{
	//	// �����������Dialog����Աֵ����ԴΪm_localActiveMember����NetId��ʾ�ڹ������С����m_bNDHasLocalQuery=TRUE��
	//	DoLocalModeQuery();  
	//}
	else
	{
		SetToolTipPos(IDC_BUTTON_QUERY);
		ShowToolTip(checkCard.GetStatusMessage(), TRUE);
	}

	m_bFromScan = FALSE;
}	

//virtual function
void CNDDialog::OnNewCard(UINT nCardId)
{
	SetToolTipPos(IDC_EDIT_IDCARDSNO);
	ShowToolTip(IDS_HASNOTUSED);
	GetDlgItem(IDC_EDIT_IDCARDSNO)->SetFocus();
}

//virtual function.��ӵ���CNDDialog::QueryBalance ִ��DoBalance��ѯ��ΪDialog����Ա��ֵ. ����m_ActiveMember.
void CNDDialog::OnOldCard(UINT nCardId, UINT nMemberId)
{
	ReQuery();
}

/*************************************************
Function:       // CNDDialog::QueryBalance
Description:    // ִ��DoBalance��ѯ��ΪDialog����Ա��ֵ. ����m_ActiveMember.
				// ��ѯʧ�ܣ����������ຯ��
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          // nMemberId - ���ڲ�ѯ�������
                // nCardId - ���ڲ�ѯ�������
Output:         // None
Return:         // ��ѯ�ɹ�����TRUE�����򷵻�FALSE
Others:         // None
*************************************************/
BOOL CNDDialog::QueryBalance(UINT nMemberId, UINT nCardId)
{
	CBalanceMethod& Balance = CCurrentNetBarUser::GetInstance()->Balance;

	Balance.SetMemberId(nMemberId);
	Balance.SetCardId(nCardId);
	Balance.SetRound(0);

	theApp.GetCurCashier()->DoBalance(Balance);

	int nStatus = Balance.GetStatusCode();
	if (nStatus == 0)
	{
		OnGetBalanceOK(Balance);//ΪDialog����Ա��ֵ. ����m_ActiveMember

		//theApp.UpdateToolbarLastID(Balance.GetSerailNum()); //Temporarily comment out

		return TRUE;
	}
	else
	{
		OnGetBalanceFailed(Balance); //��ѯʧ����ʱ��������������virtual����ʵ����ʱ��������
		return FALSE;
	}
}

BOOL CNDDialog::ReQuery()
{
	return QueryBalance(m_nMemberId, m_nCardId);
}

/*************************************************
Function:       // CNDDialog::OnGetBalanceOK
Description:    // ΪDialog����Ա��ֵ. ����m_ActiveMember
Input:          // Balance - �������Dlg�Ĳ���
Others:         // virtual���ɱ���д
*************************************************/
void CNDDialog::OnGetBalanceOK(CBalanceMethod & Balance)
{
	m_bOnLine = (Balance.GetOnline() == 1);     //�û��Ƿ�����
	//m_bIsMember = Balance.GetIsMember();        //�Ƿ��Ա,������ʱ�û��Ķ��ǻ�Ա	
	m_bIsMember = Balance.IsNetbarMember();
	m_strNetId = Balance.GetSerailNum();		//NetId=SerialNum	
	m_strName.Format(_T("%s(%s)"), Balance.GetUserName(), CIBAHelpper::GetSexAsString(Balance.GetSex())); //�û���(�Ա�)
	m_strPersonalID = Balance.GetPersonalId(); //�û�ID	
	m_nUserClassID = Balance.GetUserClass();  //n�û�����
	m_strUserClass = CNetBarConfig::GetInstance()->GetUserClassName(Balance.GetUserClass()); //�û�����
	m_strMemberName = Balance.GetMemberName(); //�û�����
	m_strOpenNetBar = CNetBarConfig::GetInstance()->GetNetBarName(Balance.GetOpenNetbarId()); //����������
	m_strTermId = Balance.GetTermId(); //�ն˺�
	m_nMoneyBalanceW = Balance.GetCashBalance(); //W��� = ���ͳ�ֵ���
	m_nMoneyBalanceY = Balance.GetUsePresent(); //Y��� = ���ͳ�ֵ���
	m_nMoneyBalanceZ = Balance.GetCreditPresent(); //Z��� = �����˻����
	m_nMoneyOnlineAmount = Balance.GetOnlineAmount(); //OnlineAmountԤ�۽��
	m_nCurrentPoint = Balance.GetCurrentPoint(); //�û���ǰ����
	m_nMoneyGuaranty = Balance.GetGuarantyMoney(); //Ѻ��-ֻ����ʱ�û���
	m_nMoneyAllInAccount = Balance.GetTotalBalacne(); //�ܽ�� = ���ͳ�ֵ��� + ���á����ͳ�ֵ����� + ���á������ʻ������ + �û�Ԥ�۽��

	CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(m_nMemberId, m_ActiveMember); //��Map�в����û������浽ActiveMember��

	m_strAllBalance.Format(_T("%.2f"), Balance.GetTotalBalacne() / 100.0);
	//theApp.UpdateToolbarLastID(m_strNetId); //Temporarily comment out
}

void CNDDialog::OnGetBalanceFailed(CBalanceMethod & Balance)
{

}

/*************************************************
Function:       // CNDDialog::OnBnClickedButtonQuery
Description:    // ��ѯ�û���Ϣ����m_localActiveMember��DoBalance����ֵΪDialog����Ա��ֵ����������ʱ��������m_ActiveMember,Note:����ģʽʱ,m_localActiveMemberӦ���ⲿ�����.
// 1. ����ģʽ�£�����ģʽ��ѯ - ��m_localActiveMember���Dialog����Աֵ������NetId��ʾ�ڹ������С�
	Note:����������д����ο��������ʵ�֣�
// 2. ����״̬�£�����ģʽ��ѯ - DoCheckCard,��ע���û�����OnOldUser(���ද��ΪDoBalance�����Dialog��Աֵ,����m_ActiveMeber)��δע���û�ΪOnNewUser(�����޶���)
Calls:          // DoLocalModeQuery,CheckCard
Others:         //����һ������Ӧ��ť��Ϣ����ĳЩ�����Ի���������OnTimer��Ϣ�л����.
*************************************************/
void CNDDialog::OnBnClickedButtonQuery()
{
	IBA_TRACE("OnBnClickedButtonQuery");
	CWaitCursor Wait;
	UpdateData();//�ӶԻ���������ݵ����б���

	//����˺�Ϊ�գ���ʾ��ʾ��
	if (m_strNetId.IsEmpty())
	{
		SetToolTipPos(IDC_EDIT_IDCARDSNO);
		ShowToolTip(IDS_INPUTNETID);
		GetDlgItem(IDC_EDIT_IDCARDSNO)->SetFocus();
		return;
	}

	ClearData(); 

	if (CIBAGlobal::NetworkStatus != CIBAGlobal::emNetworkNormal)//Restore/Broken - �Ӽ����б��в����û�����ֵ����Ա����
	{
		DoLocalModeQuery();
	}
	else//Normal - �����Ľ��в�ѯ,����Balance�ӿڵķ���ֵ��ֵ����Ա����
	{
		CheckCard(m_strNetId);
	}
	
	UpdateData(FALSE);//���¶Ի���������ʾ
}

void CNDDialog::ClearData()
{

}

void CNDDialog::AddFocusEnter(UINT nFocusID, UINT nEnterID)
{
	TFocusEnter FocusEnter = {nFocusID, nEnterID};

	m_FocusEnterMap.Add(FocusEnter);
}

void CNDDialog::OnBnClickedButtonScanreco()
{
	CMenu menu;
	menu.LoadMenu(IDR_MENU_SCAN);
	CMenu* pPopup = menu.GetSubMenu(0);
	pPopup->RemoveMenu(4, MF_BYPOSITION);

	CRect rect;
	GetDlgItem(IDC_BUTTON_SCANRECO)->GetWindowRect(rect);
	theApp.GetContextMenuManager()->ShowPopupMenu(pPopup->Detach(), rect.CenterPoint().x, rect.CenterPoint().y, this, TRUE);
}

void CNDDialog::OnScan1()
{
	ScanReco(1);
}

void CNDDialog::OnScan2()
{
	ScanReco(2);
}

void CNDDialog::OnRead2()
{
	ScanReco(3);
}

void CNDDialog::ScanReco(UINT nType)
{
	CWaitCursor Wait;

	CNetBarUser NetBarUser;

	if (theApp.GetRealName()->ScanUserInfo(nType, NetBarUser))
	{
		m_bFromScan = TRUE;
		m_strNetId = NetBarUser.GetCertificateID();
		UpdateData(FALSE);
		OnBnClickedButtonQuery();
	}
	else
	{
		SetToolTipPos(IDC_BUTTON_SCANRECO);
		ShowToolTip(IDS_SCANFAILED);
	}
}


void CNDDialog::InitWindowPos()
{
	if (IsWindow(hWndPre))
	{
		CRect rect;

		::GetWindowRect(hWndPre, rect);

		SetWindowPos(NULL, rect.left - 10, rect.top - 25, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
	}

	hWndPre = GetSafeHwnd();
}


void CNDDialog::OnTimer(UINT nIDEvent)
{
	CIBADialog::OnTimer(nIDEvent);

	if (TimerIdOfRePosDialogs == nIDEvent)
	{
		KillTimer(nIDEvent);
		//theApp.GetDialogManager()->ReposDialogs();
	}
}

void CNDDialog::RePosDialogs()
{
	SetTimer(TimerIdOfRePosDialogs, 100, NULL);
}

BOOL CNDDialog::IsMeGotFocus()
{
	CWnd* pWnd = GetFocus();

	if (pWnd != NULL)
	{
		if (pWnd->GetDlgCtrlID() == GetDlgCtrlID())
		{
			return TRUE;
		}
		else
		{
			CWnd* pOwner = pWnd->GetParentOwner();

			if (pOwner != NULL)
			{
				if (pOwner->GetDlgCtrlID() == GetDlgCtrlID())
				{
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

void CNDDialog::SimulateClickMe()
{
	CPoint point;
	GetCursorPos(&point);

	CRect rect;
	GetWindowRect(rect);

	DWORD dx = (rect.left + 10) * 65536 / GetSystemMetrics(SM_CXSCREEN);
	DWORD dy = (rect.top + 10) * 65536 / GetSystemMetrics(SM_CYSCREEN);
	
	mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP,
		dx, dy, 0, GetMessageExtraInfo());

	SetCursorPos(point.x, point.y);
}

void CNDDialog::OnCenterDisconnect()
{
	// do nothing
}

int CNDDialog::GetUserState()
{
	return CLocalServer::GetInstance()->ActiveMemberList.GetUserStateByNetId(m_strNetId);
}

/*************************************************
Function:       // CNDDialog::DoLocalModeQuery
Description:    // ���ز�ѯ�û���Ϣ,��䵽����
Return:         // 
*************************************************/
BOOL CNDDialog::DoLocalModeQuery()
{
	//�����û���Ϣ
	UINT nMemberId;
	if(FALSE == CIBADAL::GetInstance()->GetMemberId(m_strNetId,nMemberId)){return FALSE;}
	CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(nMemberId,m_ActiveMember);
	
	//����Ա����
	m_nMemberId = m_ActiveMember.GetMemberID();
	m_bOnLine = m_ActiveMember.GetIsOnLine(); 
	m_bIsMember = m_ActiveMember.IsMember();
	//m_strNetId= m_ActiveMember.GetNetId(); //���δ����,m_strNetId��Ϊ��
	m_strName.Format(_T("%s(%s)"), m_ActiveMember.GetUserName(), m_ActiveMember.GetSexAsString()); 
	m_strPersonalID = m_ActiveMember.GetPersonalID();
	m_nUserClassID = m_ActiveMember.GetClassId();
	m_strUserClass = m_ActiveMember.GetUserClassName();
	m_strMemberName = m_ActiveMember.GetUserName();
	m_strOpenNetBar = CNetBarConfig::GetInstance()->GetNetBarName(CNetBarConfig::GetInstance()->GetNetBarId());
	m_strTermId = m_ActiveMember.GetTerminalID();
	
	m_nMoneyAllInAccount = m_ActiveMember.GetAvailavleBalance();

	//ȡѺ��
	UINT nMoneyGuaranty = 0;
	if(FALSE == CIBADAL::GetInstance()->GetDepositFromMember(m_strNetId,nMoneyGuaranty))
	{
		m_nMoneyGuaranty = 0;
	}
	else
	{
		m_nMoneyGuaranty = nMoneyGuaranty;
	}
	//CString sql;
	//sql.Format(_T("select deposit from member where memberId=%d"),m_ActiveMember.GetMemberID());
	//GxxDictionaryPtr MemberPtr = CIBADAL::GetInstance()->ReadRecordFromDB(sql);

	//if(!MemberPtr)
	//{
	//	IBA_LOG(_T("Error! CNDDialog::DoLocalModeQuery��Member�������ݿ��ȡѺ��ʧ��.MemberId:%d"),ActiveMember.GetMemberID());
	//	m_nMoneyGuaranty = 0;
	//}
	//else
	//{
	//	m_nMoneyGuaranty = MemberPtr->valueForKey("deposit")->intValue();
	//}

	//����ַ���
	m_strAllBalance.Format(_T("%.2f"), (double)m_ActiveMember.GetAvailavleBalance()/ 100.0); //�ܽ��=�������

	//theApp.UpdateToolbarLastID(m_strNetId); //Temporarily comment out

	return TRUE;
}

BOOL CNDDialog::DoLocalModeOperate()
{
	return TRUE;
}

void CNDDialog::SetLocalActiveMember( CActiveMember& activeMember )
{
	m_LocalActiveMember = activeMember;
}