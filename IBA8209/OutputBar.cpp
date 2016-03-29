
#include "stdafx.h"
#include "IBA.h"
#include "IBAHelpper.h"
#include "CreditDlg.h"
#include "ServletHellper.h"
#include "Dal\IBADAL.h"
#include ".\outputbar.h"
#include "MainFrm.h"
#include "Process.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static const INT LIST_COUNT = 5;

static const INT LIST_OPERATION = 0;  // ������־
static const INT LIST_CARD = 1;       // ˢ����¼
static const INT LIST_CHECKOUT = 2;   // ���˼�¼
static const INT LIST_CONSUME = 3;    // ���Ѽ�¼
static const INT LIST_MSG = 4;        // ��Ϣ��¼

static const TCHAR *lpFileNameSuffix[LIST_COUNT] = 
{ 
	_T("_operation.dat"),
	_T("_card.dat"), 
	_T("_checkOut.dat"),
	_T("_consume.dat"), 
	_T("_msg.dat")
};
static COutputListCtrl *lpListCtrl[LIST_COUNT] = {NULL};
/////////////////////////////////////////////////////////////////////////////
// CIBAOutputBar

BEGIN_MESSAGE_MAP(CIBAOutputBar, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(WM_OPERATIONLOG, OnOperationLog)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDM_EXPORTEXCEL, OnExportExcel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIBAOutputBar construction/destruction

CIBAOutputBar::CIBAOutputBar()
{
	// TODO: add one-time construction code here
}

CIBAOutputBar::~CIBAOutputBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// CIBAOutputBar message handlers
//����
int CIBAOutputBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	EnableGripper(FALSE);//����ʾ����CAPTION

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create tabs window:
	if (!m_wndTabs.Create(CBCGPTabWnd::STYLE_3D_ONENOTE, rectDummy, this, 1))
	{
		TRACE0("Failed to create output tab window\n");
		return -1;      // fail to create
	}

	m_wndTabs.EnableTabSwap(FALSE);//��ֹTAB������϶�����

	lpListCtrl[0] = &m_wndListInfo;
	lpListCtrl[1] = &m_wndListActive;
	lpListCtrl[2] = &m_listCheckOut;
	lpListCtrl[3] = &m_wndConsumeInfo;
	lpListCtrl[4] = &m_wndMsgInfo;

	if (!CreateOperationList())
	{
		return -1;
	}

 	if (!CreateActiveList())
 	{
 		return -1;
 	}

	// 2011/10/17-8206-gxx: 
	if (!CreateCheckOutList())
	{
		return -1;
	}

	if (!CreateMsgList())
	{
		return -1;
	}

	if (!CreateConsumeList())
	{
		return -1;
	}
	
	m_wndTabs.AddTab(&m_wndListInfo, LOAD_STRING(IDS_OPERATELOG));//��Ӳ�����Ϣҳ
	m_wndTabs.AddTab(&m_wndListActive, LOAD_STRING(IDS_ACTIVELOG));//��Ӽ����¼ҳ
	m_wndTabs.AddTab(&m_listCheckOut, LOAD_STRING(IDS_CHECKOUT_RECORD)); // ���˼�¼
	m_wndTabs.AddTab(&m_wndConsumeInfo, LOAD_STRING(IDS_RETURNLOG));//�˿�
	m_wndTabs.AddTab(&m_wndMsgInfo, LOAD_STRING(IDS_CLIENTINFO));//����û���Ϣ
	
	m_wndTabs.RecalcLayout();
	m_wndTabs.SetActiveTab(0);
	
	


	return 0;
}

void CIBAOutputBar::OnSize(UINT nType, int cx, int cy) 
{	
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	
	m_wndTabs.SetWindowPos(NULL, 0, 0, cx, cy , /*SWP_NOMOVE |*/ SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndTabs.RecalcLayout();
}

void CIBAOutputBar::SetCreditInfo(COperationInfo &OperationInfo)
{
	m_wndListInfo.SetItemText(0, 2, OperationInfo.GetUserName());

	m_wndListInfo.SetItemText(0, 3, FormatNetId(OperationInfo.GetNetId()));

	CString strTmp;
	strTmp.Format(_T("%.2f"), OperationInfo.GetOperationAmount() / 100.0);
	m_wndListInfo.SetItemText(0, 4, strTmp);
	
	CString strDescription;

	if (OperationInfo.GetPresentAmount() > 0)
	{
		strDescription.AppendFormat(LOAD_STRING(IDS_PRESENT_FORMAT), OperationInfo.GetPresentAmount() / 100.0); // ����:%.2f
	}
	
	if (OperationInfo.GetDeposit() > 0)
	{
		if (!strDescription.IsEmpty())
		{
			strDescription.Append(_T(","));
		}

		strDescription.AppendFormat(LOAD_STRING(IDS_DEPOSIT_FORMAT), OperationInfo.GetDeposit() / 100.0); // Ѻ��:%.2f
	}

	if (!OperationInfo.GetPassWord().IsEmpty())
	{
		if (CNetBarConfig::GetInstance()->GetRealNamePassword() < 2)
		{
			if (!strDescription.IsEmpty())
			{
				strDescription.Append(_T(","));
			}

			strDescription.AppendFormat(LOAD_STRING(IDS_PASSWORD_FORMAT), OperationInfo.GetPassWord()); // ����:%s
		}
	}

	if (OperationInfo.GetIsBundTime())
	{
		if (!strDescription.IsEmpty())
		{
			strDescription.Append(_T(","));
		}

		strDescription.AppendFormat(_T(""), OperationInfo.GetBundTimeAmount() / 100.0); // ��ʱ���:%.2f
	}

	if (!OperationInfo.GetDescription().IsEmpty())
	{
		if (!strDescription.IsEmpty())
		{
			strDescription.Append(_T(","));
		}

		strDescription.Append(OperationInfo.GetDescription());
	}
	
	m_wndListInfo.SetItemText(0, 5, strDescription);
}

BOOL CIBAOutputBar::AddCheckOutLog(COperationInfo& OperationInfo)
{
	CIBAString strTmp;

	BOOL bOK = FALSE;
	switch(OperationInfo.GetOperationType())
	{//����ʱ��|�û�����|�ն˺�|�ϻ�ʱ��|�����˺�|����|֤������|���|��ע
	case CIBAGlobal::emOpTypeAutoReturn:
		{
			m_listCheckOut.InsertItem(0, COleDateTime::GetCurrentTime().Format(_T("%H:%M:%S")));
			CString strTmp;
			strTmp = CNetBarConfig::GetInstance()->GetUserClassName(OperationInfo.GetClassId());
			m_listCheckOut.SetItemText(0, 1, strTmp); // �û�����
			strTmp = OperationInfo.GetNetId();
			m_listCheckOut.SetItemText(0,4, strTmp);

			strTmp = OperationInfo.GetUserName();
			m_listCheckOut.SetItemText(0,5, strTmp);

			strTmp = OperationInfo.GetIdNum();
			m_listCheckOut.SetItemText(0,6, strTmp);

			// ��ע��ʾΪ�Զ��˿�
			m_listCheckOut.SetItemText(0, 8,  CIBAGlobal::GetOperationName(CIBAGlobal::emOpTypeAutoReturn));
		}
		bOK = TRUE;
		break;
	case CIBAGlobal::emOpTypeVipReturn:
		{
			m_listCheckOut.InsertItem(0, COleDateTime::GetCurrentTime().Format(_T("%H:%M:%S")));
			CString strTmp;
			strTmp = CNetBarConfig::GetInstance()->GetUserClassName(OperationInfo.GetClassId());
			m_listCheckOut.SetItemText(0, 1, strTmp); // �û�����

			strTmp = OperationInfo.GetTermId();
			m_listCheckOut.SetItemText(0, 2, strTmp); // �ն˺�

			if (CIBAHelpper::IsValidTime(OperationInfo.GetCheckInTime()))
			{
				strTmp = OperationInfo.GetCheckInTime().Format(_T("%H:%M:%S"));
				m_listCheckOut.SetItemText(0, 3, strTmp);
			}

			strTmp = OperationInfo.GetNetId();
			m_listCheckOut.SetItemText(0,4, strTmp);

			strTmp = OperationInfo.GetUserName();
			m_listCheckOut.SetItemText(0,5, strTmp);

			strTmp = OperationInfo.GetIdNum();
			m_listCheckOut.SetItemText(0,6, strTmp);

			m_listCheckOut.SetItemText(0,8, OperationInfo.GetDescription());
			
			bOK = TRUE;
			break;
		}
		
	case CIBAGlobal::emOpTypeRoomReturn:
		{
			m_listCheckOut.InsertItem(0, COleDateTime::GetCurrentTime().Format(_T("%H:%M:%S")));
			CString strTmp;
			strTmp = CNetBarConfig::GetInstance()->GetUserClassName(OperationInfo.GetClassId());
			m_listCheckOut.SetItemText(0, 1, strTmp); // �û�����

			strTmp = OperationInfo.GetTermId();
			m_listCheckOut.SetItemText(0, 2, strTmp); // �ն˺�

			if (CIBAHelpper::IsValidTime(OperationInfo.GetCheckInTime()))
			{
				strTmp = LOAD_STRING(IDS_ROOM_OPEN_TIME);
				strTmp += OperationInfo.GetCheckInTime().Format(_T("%H:%M:%S"));
				m_listCheckOut.SetItemText(0, 3, strTmp);
			}

			strTmp = OperationInfo.GetNetId();
			m_listCheckOut.SetItemText(0,4, strTmp);

			strTmp = OperationInfo.GetUserName();
			m_listCheckOut.SetItemText(0,5, strTmp);

			strTmp = OperationInfo.GetIdNum();
			m_listCheckOut.SetItemText(0,6, strTmp);

			m_listCheckOut.SetItemText(0,8, OperationInfo.GetDescription());
			
			bOK = TRUE;
			break;
		}
		
	case CIBAGlobal::emOpTypeReturn:
		{
			m_listCheckOut.InsertItem(0, COleDateTime::GetCurrentTime().Format(_T("%H:%M:%S")));
			CString strTmp;
			strTmp = CNetBarConfig::GetInstance()->GetUserClassName(OperationInfo.GetClassId());
			m_listCheckOut.SetItemText(0, 1, strTmp); // �û�����

			strTmp = OperationInfo.GetTermId();
			m_listCheckOut.SetItemText(0, 2, strTmp); // �ն˺�

			if (CIBAHelpper::IsValidTime(OperationInfo.GetCheckInTime()))
			{
				strTmp = OperationInfo.GetCheckInTime().Format(_T("%H:%M:%S"));
				m_listCheckOut.SetItemText(0, 3, strTmp);
			}

			strTmp = OperationInfo.GetNetId();
			m_listCheckOut.SetItemText(0,4, strTmp);

			strTmp = OperationInfo.GetUserName();
			m_listCheckOut.SetItemText(0,5, strTmp);

			strTmp = OperationInfo.GetIdNum();
			m_listCheckOut.SetItemText(0,6, strTmp);


			strTmp.Format(_T("%.2f"), OperationInfo.GetConsumeMoney() / 100.0);
			m_listCheckOut.SetItemText(0,7, strTmp);

			CString strDescription;

			// IDS_OPTYPE_BALANCE: "���:%.2f"
			// IDS_OPTYPE_BALANCE_EX: "���:%.2f + Ѻ��:%.2f"
			if (OperationInfo.GetDeposit() > 0)
			{
				strDescription.Format(LOAD_STRING(IDS_OPTYPE_BALANCE_EX),
					OperationInfo.GetOperationAmount() / 100.0,
					OperationInfo.GetDeposit() / 100.0);
			}
			else
			{
				strDescription.Format(LOAD_STRING(IDS_OPTYPE_BALANCE),
					OperationInfo.GetOperationAmount() / 100.0);
			}

			if (!OperationInfo.GetDescription().IsEmpty())
			{
				if (!strDescription.IsEmpty())
				{
					strDescription.Append(_T(","));
				}

				strDescription.Append(OperationInfo.GetDescription());
			}

			m_listCheckOut.SetItemText(0, 8, strDescription);
		}
		bOK = TRUE;
		break;
	}

	if (bOK)
		SaveToFile( lpFileNameSuffix[LIST_CHECKOUT], *lpListCtrl[LIST_CHECKOUT]);

	// 2014-6-13 - qsc
	// ������Ϣ����������̨�������˲���
	{
		static int nExist = -1;
		if(-1 == nExist)
		{
			CProcess process;
			process.Attach("NetBar.exe");		// ��������̨����
			if(process.IsRunning())
			{
				nExist = 1;
			}
			else
			{
				nExist = 0;
			}
		}

		if(1 == nExist)
		{
			CString str = OperationInfo.GetTermId();
			for(int i = 0; i < str.GetLength(); ++i)
			{
				char ch = str.GetAt(i);
				if(ch < '0' || ch > '9')
				{
					str.Remove(ch);	//	���ŵ��ն˺��Ǵ�����
				}
			}	
			CIBAHelpper::AutoHXPayout(str);
		}
	}
	return FALSE;
}

void CIBAOutputBar::AddOperationLog(COperationInfo& OperationInfo)
{
	CIBAString strTmp;

	m_wndListInfo.InsertItem(0, COleDateTime::GetCurrentTime().Format(_T("%H:%M:%S")));

	
	switch (OperationInfo.GetOperationType())
	{
	case 0://��
		{
			if (OperationInfo.GetIsBundTime())
			{
				m_wndListInfo.SetItemText(0, 1, CIBAGlobal::GetOperationName(CIBAGlobal::emOpTypeBundleTime));
				m_wndListInfo.SetItemText(0, 2, OperationInfo.GetUserName());
				m_wndListInfo.SetItemText(0, 3, FormatNetId(OperationInfo.GetNetId()));
				
				strTmp.Format(_T("%.2f"), OperationInfo.GetOperationAmount() / 100.0);
				m_wndListInfo.SetItemText(0, 4, strTmp);

			}
		}

	case CIBAGlobal::emOpTypeOpen://����
	case CIBAGlobal::emOpTypeCredit://��ֵ
	case CIBAGlobal::emOpTypeCreditDelete://����
	case CIBAGlobal::emOpTypeLocalCredit: // 2011/08/01-8201-gxx: ���س�ֵ
	case CIBAGlobal::emOpTypeCreditModify: // �޸ĳ�ֵ
	case CIBAGlobal::emOpTypeChangeMachine://����
		{
			m_wndListInfo.SetItemText(0,1,CIBAGlobal::GetOperationName(OperationInfo.GetOperationType()));
			SetCreditInfo(OperationInfo);
			break;
		}
	case CIBAGlobal::emOpTypeVipReturn: //��Ա����
	case CIBAGlobal::emOpTypeBrokenReturnVip: // ��Ա��������
	case CIBAGlobal::emOpTypeRoomReturn:
		{
			m_wndListInfo.SetItemText(0, 1,CIBAGlobal::GetOperationName(OperationInfo.GetOperationType()));
			
			m_wndListInfo.SetItemText(0, 2, OperationInfo.GetUserName());
			m_wndListInfo.SetItemText(0, 3, OperationInfo.GetNetId());

			CString strDescription;

			if (!OperationInfo.GetDescription().IsEmpty())
			{
				if (!strDescription.IsEmpty())
				{
					strDescription.Append(_T(","));
				}

				strDescription.Append(OperationInfo.GetDescription());
			}

			m_wndListInfo.SetItemText(0, 5, strDescription);

			break;
		}
	
	case CIBAGlobal::emOpTypeReturn: //�˿�
	case CIBAGlobal::emOpTypeAutoReturn: //�Զ��˿�
	case CIBAGlobal::emOpTypeReturnBalance: //�����
	case CIBAGlobal::emOpTypeLocalReturn: // �����û��˿�
	case CIBAGlobal::emOpTypeBrokenReturn: // �����˿�
		{
			m_wndListInfo.SetItemText(0, 1, CIBAGlobal::GetOperationName(OperationInfo.GetOperationType()));

			m_wndListInfo.SetItemText(0, 2, OperationInfo.GetUserName());
			m_wndListInfo.SetItemText(0, 3, FormatNetId(OperationInfo.GetNetId()));

			strTmp.Format(_T("%.2f"), OperationInfo.GetAllAmount() / 100.0);
			m_wndListInfo.SetItemText(0, 4, strTmp);

			CString strDescription;

			// IDS_OPTYPE_BALANCE: "���:%.2f"
			// IDS_OPTYPE_BALANCE_EX: "���:%.2f + Ѻ��:%.2f"
			if (OperationInfo.GetDeposit() > 0)
			{
				strDescription.Format(LOAD_STRING(IDS_OPTYPE_BALANCE_EX),
					OperationInfo.GetOperationAmount() / 100.0,
					OperationInfo.GetDeposit() / 100.0);
			}
			else
			{
				strDescription.Format(LOAD_STRING(IDS_OPTYPE_BALANCE),
					OperationInfo.GetOperationAmount() / 100.0);
			}

			if (!OperationInfo.GetDescription().IsEmpty())
			{
				if (!strDescription.IsEmpty())
				{
					strDescription.Append(_T(","));
				}

				strDescription.Append(OperationInfo.GetDescription());
			}

			m_wndListInfo.SetItemText(0, 5, strDescription);

			break;
		}
	case CIBAGlobal::emOpTypeOpenLocal:// ���ؿ���
		{
			m_wndListInfo.SetItemText(0, 1, CIBAGlobal::GetOperationName(CIBAGlobal::emOpTypeOpenLocal)); 

			strTmp.LoadString(IDS_LOCALUSER);
			OperationInfo.SetDescription(strTmp);  // ��ʾ�����û���ע
			SetCreditInfo(OperationInfo);
			break;
		}

	case CIBAGlobal::emOpTypeBundleTime://��ʱ
		{
			m_wndListInfo.SetItemText(0, 1, CIBAGlobal::GetOperationName(CIBAGlobal::emOpTypeBundleTime));
			m_wndListInfo.SetItemText(0, 2, OperationInfo.GetUserName());
			m_wndListInfo.SetItemText(0, 3, OperationInfo.GetNetId());

			strTmp.Format(_T("%.2f"), OperationInfo.GetOperationAmount() / 100.0);
			m_wndListInfo.SetItemText(0, 4, strTmp);

			m_wndListInfo.SetItemText(0, 5, OperationInfo.GetDescription());

			break;
		}
	default:
		IBA_ASSERT2(FALSE,"δ֪�Ĳ�������");
		break;
	}

	INT nType = OperationInfo.GetOperationType();

	if (OperationInfo.GetOperationType() == 2 && 
		CIBAGlobal::NetworkStatus != CIBAGlobal::emNetworkBroken &&
		OperationInfo.GetIsResumeData() == FALSE)
	{
		OperationInfo.SetOperationType(0);
		OperationInfo.SetIsMember(FALSE); //�˿�Ŀ϶��Ƿǻ�Ա
		QueryConsume(OperationInfo, TRUE);
	}
	else
	{
		m_wndTabs.SetActiveTab(0);
	}

	// ���浽�ļ�
	SaveToFile( lpFileNameSuffix[LIST_OPERATION], *lpListCtrl[LIST_OPERATION]);
}

void CIBAOutputBar::AddActiveLog()
{
	NS_SERVLET::CBalanceMethod & Balance = CCurrentNetBarUser::GetInstance()->Balance;

	m_wndListActive.InsertItem(0, COleDateTime::GetCurrentTime().Format(_T("%H:%M:%S")));
	
	m_wndListActive.SetItemText(0, 1, Balance.GetUserName() + _T("(") 
		+ CIBAHelpper::GetSexAsString(Balance.GetSex())  + _T(")"));

	m_wndListActive.SetItemText(0, 2, Balance.GetSerailNum());

	CString strTmp;//�û�����
	strTmp = CNetBarConfig::GetInstance()->GetUserClassName(Balance.GetUserClass());
	m_wndListActive.SetItemText(0, 3, strTmp);

	//��������˻�= �ֽ�� + �û����ͽ�� + ��ǰ��ֵ�Ľ�� + ����Ԥ����
	INT nAllMoney = Balance.GetCashBalance() + Balance.GetUsePresent() + Balance.GetCreditPresent() + Balance.GetOnlineAmount();

	strTmp.Format(_T("%.2f"), nAllMoney / 100.0);
	m_wndListActive.SetItemText(0, 4, strTmp);

	strTmp.Format(_T("%d"), Balance.GetCurrentPoint());

	m_wndListActive.SetItemText(0, 5, strTmp);

	m_wndListActive.SetItemText(0, 6, Balance.GetPersonalId());

	if (m_wndListActive.GetItemCount() > 50)
	{
		m_wndListActive.DeleteItem(50);
	}
	
	m_wndTabs.SetActiveTab(1);

	SaveToFile( lpFileNameSuffix[LIST_CARD], *(lpListCtrl[LIST_CARD]));
}

BOOL CIBAOutputBar::CreateOperationList()
{
	const DWORD dwViewStyle = LVS_REPORT | LVS_NOSORTHEADER | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	CIBAHelpper::CreateFont(m_Font, 14);

	if (!m_wndListInfo.Create(dwViewStyle, CRect(1, 1, 1, 1), &m_wndTabs, 2))
	{
		TRACE0("Failed to create output view\n");
		return FALSE;      // fail to create
	}

	CIBAString strHeader;
	strHeader.LoadString(IDS_LOGINFOLISTGHEADER);
	//strHeader = L"ʱ��|��������|����|�����˺�|���|��ע";

	CStringArray strArray;
	CIBAHelpper::SplitLine(strHeader, strArray);

	const UINT Width[] = {70, 120, 100, 150, 60, 200};

	int nColCount = sizeof(Width)/sizeof(UINT);
	if( nColCount != strArray.GetCount() )
	{
		IBA_ASSERT2(false,"�ַ����ָ�����鳤�Ȳ����,�����ַ������е��ַ����Ƿ��иĶ�");

		nColCount = nColCount < strArray.GetCount() ?
			nColCount : strArray.GetCount();  // ��ֹ��������
	}

	for (INT i = 0; i < nColCount; i++)
	{
		m_wndListInfo.InsertColumn(i, strArray.GetAt(i), LVCFMT_LEFT, Width[i]);
	}

	

	return TRUE;
}

BOOL CIBAOutputBar::CreateActiveList()
{
	const DWORD dwViewStyle = LVS_REPORT | LVS_NOSORTHEADER | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if (!m_wndListActive.Create(dwViewStyle, CRect(1, 1, 1, 1), &m_wndTabs, 3))
	{
		TRACE0("Failed to create Activeoutput view\n");
		return FALSE;      // fail to create
	}

	CIBAString strHeader;
	strHeader.LoadString(IDS_LOGACTIVELISTGHEADER);

	CStringArray strArray;
	CIBAHelpper::SplitLine(strHeader, strArray);

	const UINT Width[] = {70, 90, 150, 80, 100, 80, 150};

	int nColCount = sizeof(Width)/sizeof(UINT);
	if( nColCount != strArray.GetCount() )
	{
		IBA_ASSERT2(false,"�ַ����ָ�����鳤�Ȳ����,�����ַ������е��ַ����Ƿ��иĶ�");

		nColCount = nColCount < strArray.GetCount() ?
			nColCount : strArray.GetCount();  // ��ֹ��������
	}

	for (INT i = 0; i < nColCount; i++)
	{
		m_wndListActive.InsertColumn(i, strArray.GetAt(i), LVCFMT_LEFT, Width[i]);
	}

	return TRUE;
}

BOOL CIBAOutputBar::CreateMsgList()
{
	const DWORD dwViewStyle = LVS_REPORT | LVS_NOSORTHEADER | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if (!m_wndMsgInfo.Create(dwViewStyle, CRect(1, 1, 1, 1), &m_wndTabs, 4))
	{
		TRACE0("Failed to create Activeoutput view\n");
		return FALSE;      // fail to create
	}

	CIBAString strHeader;
	strHeader.LoadString(IDS_LOGMSGLISTGHEADER);
	//strHeader = L"ʱ��|�ն˺�|��Ϣ";

	CStringArray strArray;
	CIBAHelpper::SplitLine(strHeader, strArray);

	const UINT Width[] = {130, 70, 450};

	int nColCount = sizeof(Width)/sizeof(UINT);
	if( nColCount != strArray.GetCount() )
	{
		IBA_ASSERT2(false,"�ַ����ָ�����鳤�Ȳ����,�����ַ������е��ַ����Ƿ��иĶ�");

		nColCount = nColCount < strArray.GetCount() ?
			nColCount : strArray.GetCount();  // ��ֹ��������
	}

	for (INT i = 0; i < nColCount; i++)
	{
		m_wndMsgInfo.InsertColumn(i, strArray.GetAt(i), LVCFMT_LEFT, Width[i]);
	}

	//��ѯ����ε���Ϣ
	//UpdateMsgLog();

	return TRUE;
}

BOOL CIBAOutputBar::CreateConsumeList()
{
	const DWORD dwViewStyle = LVS_REPORT | LVS_NOSORTHEADER | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if (!m_wndConsumeInfo.Create(dwViewStyle, CRect(1, 1, 1, 1), &m_wndTabs, 5))
	{
		TRACE0("Failed to create Consumeoutput view\n");
		return FALSE;      // fail to create
	}

	CIBAString strHeader;
	//strHeader.LoadString(IDS_RETURNLOG);
	strHeader.LoadString(IDS_CONSUME_LIST_HEAD);//L"��������|���|��ϸ��Ϣ|����|�����˺�|����";

	CStringArray strArray;
	CIBAHelpper::SplitLine(strHeader, strArray);

	const UINT Width[] = {80, 80, 300, 80, 130, 100};

	int nColCount = sizeof(Width)/sizeof(UINT);
	if( nColCount != strArray.GetCount() )
	{
		IBA_ASSERT2(false,"�ַ����ָ�����鳤�Ȳ����,�����ַ������е��ַ����Ƿ��иĶ�");

		nColCount = nColCount < strArray.GetCount() ?
			nColCount : strArray.GetCount();  // ��ֹ��������
	}

	for (INT i = 0; i < nColCount; i++)
	{
		m_wndConsumeInfo.InsertColumn(i, strArray.GetAt(i), LVCFMT_LEFT, Width[i]);
	}

	return TRUE;
}

BOOL CIBAOutputBar::CreateCheckOutList()
{
	const DWORD dwViewStyle = LVS_REPORT | LVS_NOSORTHEADER | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if (!m_listCheckOut.Create(dwViewStyle, CRect(1, 1, 1, 1), &m_wndTabs, 6))
	{
		TRACE0("Failed to create Consumeoutput view\n");
		return FALSE;      // fail to create
	}

	CIBAString strHeader;
	strHeader.LoadString(IDS_REFUND_RECORD_HEAD); // _T("����ʱ��|�û�����|�ն˺�|�ϻ�ʱ��|�����˺�|����|֤������|���|��ע");

	CStringArray strArray;
	CIBAHelpper::SplitLine(strHeader, strArray);

	const UINT Width[] = {90, 80, 80, 140, 140, 80,140,100,170};

	int nColCount = sizeof(Width)/sizeof(UINT);
	if( nColCount != strArray.GetCount() )
	{
		strHeader = _T("����ʱ��|�û�����|�ն˺�|�ϻ�ʱ��|�����˺�|����|֤������|���|��ע");
		strArray.RemoveAll();
		CIBAHelpper::SplitLine(strHeader, strArray);
		nColCount = strArray.GetCount();
	}

	for (INT i = 0; i < nColCount; i++)
	{
		m_listCheckOut.InsertColumn(i, strArray.GetAt(i), LVCFMT_LEFT, Width[i]);
	}

	return TRUE;
}

void CIBAOutputBar::AddMsgLog(CClientShortMessage & strMsgInfo)
{
	m_wndMsgInfo.InsertItem(0, strMsgInfo.GetRevTime());
	m_wndMsgInfo.SetItemText(0, 1, strMsgInfo.GetTermID());
	m_wndMsgInfo.SetItemText(0, 2, strMsgInfo.GetMsgBody());

	m_wndTabs.SetActiveTab(3);

	SaveToFile( lpFileNameSuffix[LIST_MSG], *lpListCtrl[LIST_MSG] );
}

void CIBAOutputBar::UpdateMsgLog()
{
	////////////////////////��ѯ����ε���Ϣ////////////////////
	CClientShortMsgArray  MsgArray;
	COleDateTime Start, End;
	if (theApp.GetCurCashier()->IsOnDuty())
	{
		Start.ParseDateTime(theApp.GetCurCashier()->GetOnDutyTime());//�ϰ�ʱ��
	}
	else
	{	//һ��ǰ
		Start = COleDateTime::GetCurrentTime();
		Start -= COleDateTimeSpan(1, 0, 0, 0);
	}
	CString strStart = Start.Format(_T("%Y-%m-%d %H:%M"));

	End = COleDateTime::GetCurrentTime();
	End += COleDateTimeSpan(1, 0, 0, 0);
	CString strEnd = End.Format(_T("%Y-%m-%d %H:%M"));

	CIBADAL::GetInstance()->GetClientShortMsg(MsgArray, strStart, strEnd);

	for (INT i = 0; i < MsgArray.GetCount(); i++)
	{
		AddMsgLog(MsgArray.GetAt(i));
	}
	////////////////////////////////////////////////
}

void CIBAOutputBar::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//���أ���ֹ˫��ʱCContrl����Ӧ
	CWnd::OnLButtonDblClk(nFlags, point);
}

//�黯�ɱ�׼10λ���֣�����9λ��ԭ������
CString CIBAOutputBar::FormatNetId(CString strNetId)
{
	INT nLength = strNetId.GetLength();
	
	if (nLength > 9)
	{
		return strNetId;
	}
	else
	{
		CString strRet = _T("0000000000");
		strRet = strRet.Mid(0, 10 - nLength);
		strRet.Append(strNetId);

		return strRet;
	}
}

void CIBAOutputBar::QueryConsume(COperationInfo &OperationInfo, BOOL bNeedReturnRecord,UINT nConsumeType)
{	
	//strHeader = L"ʱ��|�����˺�|����|��������|���ѽ��|����Ա|��ע|�û���";
	
	CWaitCursor wait;

	m_wndConsumeInfo.DeleteAllItems();
	m_wndTabs.SetActiveTab(3);

	if (OperationInfo.GetMemberId() == 0)
	{
		UINT nCardId = 0;

		UINT nMemberId = CServletHellper::GetMemberId(OperationInfo.GetNetId(), nCardId);

		CCurrentNetBarUser::GetInstance()->Balance.SetMemberId(nMemberId);
		CCurrentNetBarUser::GetInstance()->Balance.SetCardId(nCardId);
		CCurrentNetBarUser::GetInstance()->Balance.SetRound(0);//ȡ��

		theApp.GetCurCashier()->DoBalance(CCurrentNetBarUser::GetInstance()->Balance);

		if (CCurrentNetBarUser::GetInstance()->Balance.GetStatusCode() == 0)
		{
			OperationInfo.SetMemberId(nMemberId);
			OperationInfo.SetUserName(CCurrentNetBarUser::GetInstance()->Balance.GetUserName());
		}
		else
		{
			return;
		}
	}

	CConsumeListMethod ConsumeListMethod;
	ConsumeListMethod.SetConsumeType(nConsumeType);//ȫ�����Ѳ�ѯ
	ConsumeListMethod.SetNeedBusinessName(2);

	if (OperationInfo.GetIsMember()) //��ʱ�û�������ʼʱ��
	{
		COleDateTime NowTime = COleDateTime::GetCurrentTime(), StartTime;

		StartTime = NowTime - COleDateTimeSpan(1, 0, 0, 0);

		if (OperationInfo.GetOperationType() > 0)
		{
			StartTime = NowTime - COleDateTimeSpan(1, 0, 0, 0);
		}
		else
		{
			if (StartTime < OperationInfo.GetOperationTime())
			{
				StartTime = OperationInfo.GetOperationTime();
			}
		}

		ConsumeListMethod.SetBeginTime(StartTime.Format(_T("%Y%m%d%H%M%S")));
	}

 	ConsumeListMethod.SetMemberId(OperationInfo.GetMemberId());
 
 	theApp.GetCurCashier()->DoConsumeList(ConsumeListMethod);
 
 	if (ConsumeListMethod.GetStatusCode() == 0)
	{
		//���ѷ�ʽ
		CIBAString strConsumeName; //= L"����|��ֵ|�˻�����|���ձ�����|������(��)|�˿�|��ֵ��|���ձ���ֵ";
		strConsumeName.LoadString(IDS_CONSUMETYPE);

		CStringArray saConsumeName;
		CIBAHelpper::SplitLine(strConsumeName, saConsumeName);

		for (INT i = 0; i < (INT)ConsumeListMethod.GetRecordCount(); i++)
		{
			//��������|���|��ϸ��Ϣ|����Ա|�����˺�|����

			m_wndConsumeInfo.InsertItem(i, saConsumeName.GetAt(ConsumeListMethod.GetConsumeType(i) - 1));

			CString strTmp;
			//ʱ��
			strTmp = ConsumeListMethod.GetBeginTime(i);
			strTmp.Insert(2, _T("-"));
			strTmp.Insert(5, _T(" "));
			strTmp.Insert(8, _T(":"));
			//strTmp.Insert(11, _T(":"));
			strTmp = strTmp.Left(strTmp.GetLength() - 2);
			m_wndConsumeInfo.SetItemText(i, 2, strTmp);

			/**
			*��������0: ȫ�� 1:���� 2:��ֵ 3:������ 4����ţ���� 5:������(��) 6:�˿� 7����ֵ�� 8����ţ��ֵ
			*/
			switch (ConsumeListMethod.GetConsumeType(i))
			{
			case 1://����
				{
					if (ConsumeListMethod.GetBeginTime(i) != ConsumeListMethod.GetEndTime(i))
					{					
						CString strTmp2(ConsumeListMethod.GetEndTime(i));
						strTmp2.Insert(2, _T("-"));
						strTmp2.Insert(5, _T(" "));
						strTmp2.Insert(8, _T(":"));
						//strTmp2.Insert(11, _T(":"));

						strTmp2 = strTmp2.Left(strTmp2.GetLength() - 2);

						strTmp.Append(_T(" - "));
						strTmp.Append(strTmp2);

						m_wndConsumeInfo.SetItemText(i, 2, strTmp);
					}
				}
				//���ﲻҪbreak
			case 3:
			case 4:
			case 5://3������ 4����ţ���� 5:������(��)4��������֧����ʽ
				strTmp.Format(_T("%.2f"), -ConsumeListMethod.GetAmount(i) / 100.0);
				m_wndConsumeInfo.SetItemText(i, 1, strTmp);
				break;

			case 2:
			case 7:
			case 8://��ֵ����ֵ������ţ��ֵ��Ϊ���뷽ʽ
				strTmp.Format(_T("%.2f"), ConsumeListMethod.GetAmount(i) / 100.0);
				m_wndConsumeInfo.SetItemText(i, 1, strTmp);
				break;

			case 6://�˿�
				strTmp.Format(_T("%.2f"), ConsumeListMethod.GetAmount(i) / 100.0);
				m_wndConsumeInfo.SetItemText(i, 1, strTmp);
				bNeedReturnRecord = FALSE;
				break;
			}

			strTmp = m_wndConsumeInfo.GetItemText(i, 2);
			strTmp.Append(_T(" "));
			strTmp.Append(ConsumeListMethod.GetDescription(i));
			m_wndConsumeInfo.SetItemText(i, 2, strTmp);

			strTmp = ConsumeListMethod.GetBusinessName(i);
			m_wndConsumeInfo.SetItemText(i, 3, strTmp);

			//�����˺�
			m_wndConsumeInfo.SetItemText(i, 4, OperationInfo.GetNetId());
			//����
			m_wndConsumeInfo.SetItemText(i, 5, OperationInfo.GetUserName());
		}

		if (bNeedReturnRecord) //�������ûͬ�����������Լ�����һ���˿��¼
		{
			m_wndConsumeInfo.InsertItem(0, saConsumeName.GetAt(5));

			CString strTmp;

			strTmp.Format(_T("%.2f"), OperationInfo.GetOperationAmount() / 100.0);
			
			m_wndConsumeInfo.SetItemText(0, 1, strTmp);

			m_wndConsumeInfo.SetItemText(0, 2, COleDateTime::GetCurrentTime().Format(_T("%m-%d %H:%M")));

			m_wndConsumeInfo.SetItemText(0, 3, theApp.GetCurCashier()->GetName());
			//�����˺�
			m_wndConsumeInfo.SetItemText(0, 4, OperationInfo.GetNetId());
			//����
			m_wndConsumeInfo.SetItemText(0, 5, OperationInfo.GetUserName());
		}
	}
}

LRESULT CIBAOutputBar::OnOperationLog(WPARAM wp, LPARAM lp)
{
	WORD wKind = LOWORD(wp);
	switch (wKind)
	{
	case 0:
		{
			COperationInfo* pOperationInfo = (COperationInfo*)lp;
			AddCheckOutLog(*pOperationInfo);
			AddOperationLog(*pOperationInfo);
			m_wndListInfo.ActiveRemind();
		}

		break;

	case 1:

		AddActiveLog();
		break;

	case 2:
		{
			COperationInfo* pOperationInfo = (COperationInfo*)lp;
			QueryConsume(*pOperationInfo,FALSE,HIWORD(wp));
			m_wndConsumeInfo.ActiveRemind();
		}

		break;
	}

	

	return 0;
}

void CIBAOutputBar::RefreshAllInfoList()
{
	for (int i =0; i < LIST_COUNT; i++)
	{
		RefreshInfoList(lpFileNameSuffix[i], *lpListCtrl[i]);
	}
}

void CIBAOutputBar::RefreshInfoList(CString strFileSuffix, COutputListCtrl& listCtrl)
{
	listCtrl.DeleteAllItems();

	CString strFileName = theApp.GetWorkPath();
	strFileName += _T("\\IBAConfig\\operateInfo\\");
	strFileName += theApp.GetCurCashier()->GetName();
	strFileName += strFileSuffix;

	if( !::PathFileExists(strFileName) )
	{
		return ;
	}

	CString strInfo;
	CIBAHelpper::ReadFile(strFileName,strInfo);
	CStringArray arrStrOperations;
	CIBAHelpper::SplitLine(strInfo,arrStrOperations,_T('\n'));

	int nLines = arrStrOperations.GetCount();

	for(int i = 0 ; i < nLines ; i ++)
	{
		CString& strLine = arrStrOperations[i];
		CStringArray strArray;
		CIBAHelpper::SplitLine(strLine,strArray,_T('|'));

		listCtrl.InsertItem(0,_T(""));

		IBA_ASSERT2(strArray.GetCount() == listCtrl.GetHeaderCtrl().GetItemCount(),"�����¼�ļ����ֶεĸ���Ҫ���б���������");
		
		for(int j = 0; j < strArray.GetCount(); j++ )
		{
			listCtrl.SetItemText(0, j, strArray[j]);
		}
	}
	//
}

void CIBAOutputBar::DeleteDutyFile()
{
	for(int i = 0; i < LIST_COUNT; i++)
	{
		DeleteListFile(lpFileNameSuffix[i]);
	}
}

void CIBAOutputBar::SaveToFile(CString strFileSuffix, COutputListCtrl& listCtrl)
{
	int i = 0;
	int nColCount = listCtrl.GetHeaderCtrl().GetItemCount();
	CString str;
	while( i <  nColCount )
	{
		CString strTmp;
		strTmp = listCtrl.GetItemText(0,i);
		str += strTmp;
		str += _T("|");

		i++;
	}
	str += _T("\n");

	CString strFileName = theApp.GetWorkPath();
	strFileName += _T("\\IBAConfig\\operateInfo\\");

	CIBAHelpper::MakeSurePathExists(strFileName); // ����Ŀ¼

	strFileName += theApp.GetCurCashier()->GetName();
	strFileName += strFileSuffix;

	CIBAHelpper::AppendWriteFile(strFileName,str);
}

void CIBAOutputBar::DeleteListFile(CString strFileSuffix)
{
	CString strFileName = theApp.GetWorkPath();
	strFileName += _T("\\IBAConfig\\operateInfo\\");
	strFileName += theApp.GetCurCashier()->GetName();

	strFileName += strFileSuffix;

	if( !::PathFileExists(strFileName) )
	{
		return ;
	}
	::DeleteFile(strFileName);
}

void CIBAOutputBar::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: �ڴ˴������Ϣ����������
	int TabIndex = m_wndTabs.GetActiveTab();
	switch (TabIndex)
	{
	case 2:
		{
			DWORD dwPos = GetMessagePos();
			CPoint point( LOWORD(dwPos), HIWORD(dwPos) );

			CMenu menu;
			VERIFY( menu.LoadMenu( IDR_MENU_CHECKOUT ) );
			CMenu* popup = menu.GetSubMenu(0);
			ASSERT( popup != NULL );
			popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this );
			break;
		}
	default:
		CBCGPDockingControlBar::OnContextMenu(pWnd, point) ;
		break;
	}
	//return ;
}

void CIBAOutputBar::OnExportExcel()
{
	UpdateData();

	//// �򿪴洢�ļ��Ի���
	//CFileDialog dlgSave(FALSE, _T("xls"), _T("���˼�¼��"));
	//dlgSave.m_ofn.lpstrFilter = _T("*.xls");
	//if(IDOK != dlgSave.DoModal())
	//	return;

	//// ��ȡȫ·��
	//CString strFullPath;
	//strFullPath = dlgSave.GetPathName();

	// ��ȡ����������
	int nRowCount = m_listCheckOut.GetItemCount();
	CBCGPHeaderCtrl& Header = m_listCheckOut.GetHeaderCtrl();
	int nColCount = Header.GetItemCount();


	CString strAppend("");
	CString strTmp("");

	for(int nRow = 0; nRow < nRowCount; nRow++)
	{
		for(int nCol = 0; nCol < nColCount; nCol++)
		{
			strTmp.Empty();
			strTmp = m_listCheckOut.GetItemText(nRow, nCol);
			// "\t" ��ʾ������һ����Ԫ��
			strAppend += strTmp + _T("\t");
		}
		// "\n" ��ʾ������һ��
		strAppend += _T("\n");	
	}

	CString strFullPath;
	GetTempPath(MAX_PATH, strFullPath.GetBuffer(MAX_PATH));
	strFullPath.ReleaseBuffer();

	strFullPath += _T("���˼�¼��.xls");

	if(PathFileExists(strFullPath))
	{
		while(!DeleteFile(strFullPath))
		{
			static UINT nCount = 0;
			CString strReplace("");
			strReplace.Format(_T("tmp%d.xls"), nCount++);
			strFullPath.Replace(_T(".xls"), strReplace);		// ����ļ��޷�ɾ�������޸��ļ���

			if(nCount > 5)		// ����5���Ͳ�Ҫ�ٳ�����
				break;
		}
	}

	// nRowCount+1 ������������һ�б�����
	m_ExpThread.SetParam(strAppend, strFullPath, nRowCount, nColCount);
	m_ExpThread.Start();
}
