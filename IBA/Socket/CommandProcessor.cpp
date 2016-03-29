
#include "StdAfx.h"
#include "CommandProcessor.h"
#include "LocalServer.h"
#include "..\IBA.h"
#include "..\ComputerListView.h"
#include "..\ActiveMemberView.h"
#include "..\IBAHelpper.h"
#include "..\Cashier.h"
#include "..\tts\IBATTS.h"
#include "..\dal\IBADAL.h"
#include "..\MainFrm.h"
#include "..\OperationInfo.h"
#include "..\ActiveMember.h"
#include "..\SeatLayoutView.h"
#include "..\RealName\IZd.h"
#include "..\RealName\RealNameBase.h"
#include "..\RealName\IBARealName.h"
#include "..\CashierHelper.h"

//////////////////////////////////////////////////////////////////////////
//CCommandProcessor
//////////////////////////////////////////////////////////////////////////

CCommandProcessor::CCommandProcessor(CLocalServer* lpLocalServer)
: m_lpLocalServer(lpLocalServer)
, m_bGetLog(FALSE), m_bCaptureScreen(FALSE)
{
	AddHandles();
}

CCommandProcessor::~CCommandProcessor()
{
}

void CCommandProcessor::AddHandle(unsigned char command, pfnCommandHandle commandHandle)
{
	handleList_.insert(std::make_pair(command, commandHandle));
}

void CCommandProcessor::AddHandles()
{
	AddHandle(S_RESPOND_COMPUT_LIST,        &CCommandProcessor::OnGetComputerList);
	AddHandle(S_RESPOND_LOGIN_SUCCEED,      &CCommandProcessor::OnLogin);
	AddHandle(S_RESPOND_CLIENT_LOGIN,       &CCommandProcessor::OnClientLogin);
	AddHandle(S_RESPOND_CLIENT_DISCONNECT,  &CCommandProcessor::OnClientDisconnect);
	AddHandle(S_RESPOND_CLIENT_UPDATEDATA,  &CCommandProcessor::OnClientUpdateData);
	AddHandle(S_RESPOND_CLIENT_CHECKOUT,    &CCommandProcessor::OnClientCheckout);
	AddHandle(S_RESPOND_MAN_CHECKIN,        &CCommandProcessor::OnCheckinResp);
	AddHandle(S_RESPOND_QUESTION_COMPUTER,  &CCommandProcessor::OnQuestionComputer);
	AddHandle(C_MANAGER_GET_COMPUTER_COUNT, &CCommandProcessor::OnGetComputerCount);
	AddHandle(C_MANAGER_GET_PCCLASSMAP,     &CCommandProcessor::OnTermIDPCClassMap);
	AddHandle(C_MANGER_MANUALUNLOCAL,       &CCommandProcessor::OnTermUnLock);
	AddHandle(S_RESPOND_CLIENT_SHORTMESSAGE,&CCommandProcessor::OnClientShortMessage);
	AddHandle(S_RESPOND_MAN_SHORTMSG,       &CCommandProcessor::OnSendShortMsgResp);
	AddHandle(S_RESPOND_CAPTURE_SCREEN,     &CCommandProcessor::OnRespondCaptureScreen);
	AddHandle(S_RESPOND_GET_LOG,            &CCommandProcessor::OnRespondGetLog);
	AddHandle(S_RESPOND_PING_CLIENT,        &CCommandProcessor::OnPingClientResp);
	AddHandle(S_NEWNOTICE_MSG,				&CCommandProcessor::OnGetNewNotice);
	//AddHandle(S_RESPOND_PONG, &CCommandProcessor::OnPing);
	//AddHandle(C_MANAGER_GET_PCCLASSNAME, &CCommandProcessor::OnGetPCClassName);
	//AddHandle(S_RESPOND_CHECK_MANAGER_PASSWORD, &CCommandProcessor::OnCheckManagerPwdResp);
	//AddHandle(S_RESPOND_SET_MANAGER_PASSWORD, &CCommandProcessor::OnSetManagerPwdResp);
	//AddHandle(S_RESPOND_GET_MEMBER_CLASSES, &CCommandProcessor::OnGetMemberClassesResp);
	
	//AddHandle(C_MANAGER_QUESTIIOIN_AMOUNT, &CCommandProcessor::OnQuestionAmount);
	//AddHandle(C_MANAGER_GET_IP2TERMID, &CCommandProcessor::OnGetIP2TermID);
	AddHandle(S_AUTO_RETURN_MSG,            &CCommandProcessor::OnAutoReturnResp);
	AddHandle(C_LOGIN_ZD,					&CCommandProcessor::OnLoingZd);
}

void CCommandProcessor::DefaultProcess(BYTE* /*pBuffer*/, size_t /*bufferLen*/)
{
	//
}

unsigned char CCommandProcessor::GetCommand(BYTE *pBuffer, size_t /*bufferLen*/)
{
	NET_HEAD_MAN *headp_ = reinterpret_cast<NET_HEAD_MAN*>(pBuffer);

	return headp_->Cmd;
}

bool CCommandProcessor::Process(BYTE *pBuffer, size_t bufferLen)
{
	if (!pBuffer || bufferLen <= 0)
		return false;

	//try
	//{
	unsigned char command = GetCommand(pBuffer, bufferLen);

	if (handleList_.count(command) > 0) 
	{
		pfnCommandHandle pfnHandle = handleList_[command];
		(this->*pfnHandle)(pBuffer, bufferLen);
		return true;
	}
	else
	{
		DefaultProcess(pBuffer, bufferLen);	
		return false;
	}
	//}
	//catch (...) 

	//{
	//	DWORD dwErr = GetLastError();
	//	LPVOID lpMsgBuf; 
	//	FormatMessage( 
	//		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
	//		FORMAT_MESSAGE_FROM_SYSTEM | 
	//		FORMAT_MESSAGE_IGNORE_INSERTS, 
	//		NULL, 
	//		dwErr, 
	//		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	//		(LPTSTR) &lpMsgBuf, 
	//		0, 
	//		NULL 
	//		); 
	//	IBA_LOG(_T("CommandProcessor::Process - Unknown Exception.ErrorCode:%d,ErrMsg:%s"),dwErr,(LPCTSTR)lpMsgBuf);
	//}

	return false;
}

void CCommandProcessor::GetHead(BYTE *pBuffer, NET_HEAD_MAN *pHead)
{
	memcpy(pHead, pBuffer, sizeof(NET_HEAD_MAN));
}

void CCommandProcessor::GetBody(BYTE *pBuffer, void *pBody, size_t bodyLen)
{
	memcpy(pBody, pBuffer + sizeof(NET_HEAD_MAN), bodyLen);
}

void CCommandProcessor::OnConnectionEstablished()
{
	m_QuestionComputerList.RemoveAll();
	m_lpLocalServer->LocalCheckin();
}

void CCommandProcessor::OnClose()
{
	m_lpLocalServer->GetComputerListView()->Refresh(FALSE);
}

void CCommandProcessor::OnCheckinResp(BYTE *pBuffer, size_t bufLen)
{
	//NET_HEAD_MAN head = {0};
	MANAGER_CHECKIN_RESP resp = {0};

	//GetHead(pBuffer, &head);
	GetBody(pBuffer, (BYTE*)&resp, sizeof(resp));
	
	if (resp.isOk)
	{
		m_lpLocalServer->SetCheckInStatus(1);
		m_lpLocalServer->LocalLogin();

		m_lpLocalServer->LocalAskNotice();
	}
	else
	{
		m_lpLocalServer->SetCheckInStatus(-1);
	}
}

void CCommandProcessor::OnLogin(BYTE* pBuffer, size_t bufLen)
{
	const CString strMinVer(_T("3.1.1.4"));

	NET_HEAD_MAN head = {0};
	GetHead(pBuffer, &head);

	if (head.Length > 0)
	{
		MANAGER_LOGIN_RESP Login_Resp = {0};
		GetBody(pBuffer, &Login_Resp, sizeof Login_Resp);

		CString strVer = CA2W(Login_Resp.szServerVer);

		if (strVer >= strMinVer)
		{
			m_lpLocalServer->LocalGetPCClassMap();
			m_lpLocalServer->LocalGetComputerList();

			return;
		}
	}

	CString strTmp;

	strTmp.Format(IDS_ISERVERLOWVER, strMinVer);

	AfxMessageBox(strTmp, MB_ICONINFORMATION);
	
	TerminateProcess(GetCurrentProcess(), 0);
}

/*************************************************
Function:       //OnClientCheckout
Description:    //��ӦUDO����.����ΪUDO��Ա����,UDO����,UDO�û������Զ��»�
				//������Ӧʵ���»�,�����ն���Ϣ���û���Ϣ
Table Updated:  //LocalMember,LastUser,ShortMessage
Input:          //pBuffer - UDO��Ϣ,����:�ն˺�,�ն�IP,�ն�״̬,(1�ǻ�����0����ͨ,2�����»�(���߿ͻ���ֻ����������������)��3֪ͨ�ͻ������µ�¼)
                //bufLen - ���ݳ���
*************************************************/
void CCommandProcessor::OnClientCheckout(BYTE *pBuffer, size_t bufLen)
{
	MANAGER_CLIENT_DISCONNECT mcd = {0};
	GetBody(pBuffer, &mcd, sizeof(MANAGER_CLIENT_DISCONNECT)); 

	CString strTermID = CA2W(mcd.termID); //TermID
	CString strIP = CA2W(mcd.termIP); //IP
	int nCheckoutType = mcd.nCheckoutType; //1�ǻ�����0����ͨ,2�����»�(���߿ͻ���ֻ����������������)��3֪ͨ�ͻ������µ�¼

	//�ն��б��в����ն�
	INT_PTR nIndex = m_lpLocalServer->ComputerList.Find(strTermID, strIP);
	if(nIndex < 0){return;}
	CComputerInfo& computer = m_lpLocalServer->ComputerList.GetComputer(nIndex);
	UINT nMemberId = computer.GetMemberID();
	CString sIP = computer.GetComputerIP();
	CString sTermId = computer.GetTerminalID();

	//�����б��в����ն��ϵ��û�
	CActiveMember ActiveMember;
	BOOL bUserFound = m_lpLocalServer->ActiveMemberList.GetActiveMember(computer.GetMemberID(),ActiveMember);

	//�����ն���Ϣ-����û���Ϣ,״̬��Ϊ����
	computer.ClearUserInfo();
	computer.SetComputerStatus(CComputerInfo::ECS_LOCKED);
	m_lpLocalServer->ComputerList.UpdateComputer(nIndex,computer);
	m_lpLocalServer->GetComputerListView()->Refresh(FALSE);

	//�ն������û����û�δ����.��Ҫ�����û���Ϣ�ͽ���ʵ���»�.ֱ�ӷ���.
	if(FALSE == bUserFound){return;}

	//ʵ���»�
	theApp.GetRealName()->AuditUserCheckOut(sTermId, sIP, nMemberId);
	//ZDʵ���»� -  �ͻ���ֱ�����߻�����Ҳ��A��������������ֱ����A�������˺���B�����ϻ�����ʱ�����Ҳ�����Ϣ
	if(CIBAGlobal::emRealnameZD == CNetBarConfig::GetInstance()->GetAuditType())
	{
		CString strSN, strName;
		CIBADAL::GetInstance()->GetZdRnInfoByTermId(strTermID, strSN, strName);
		NS_REALNAME::CheckoutInfo coi;
		ZeroMemory(&coi, sizeof(coi));
		_tcsncpy(coi.szName, strName, 100);
		_tcsncpy(coi.szNetId, strSN, 100);
		_tcsncpy(coi.szTermId, strTermID, 50);
		_tcsncpy(coi.szIP, strIP, 50);
		coi.bClientCheckOut = true;
		coi.nCheckoutType = mcd.nCheckoutType;

		theApp.GetRealName()->CheckOut(&coi);
	}

	//���������־-��Ա����
	COperationInfo OperationInfo;
	if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		OperationInfo.SetOperationType(CIBAGlobal::emOpTypeBrokenReturnVip);
	}
	else
	{
		OperationInfo.SetOperationType(CIBAGlobal::emOpTypeVipReturn);
	}	
	OperationInfo.SetNetId(ActiveMember.GetNetId());
	OperationInfo.SetUserName(ActiveMember.GetUserName());
	//OperationInfo.SetDescription(ActiveMember.GetRemark());
	OperationInfo.SetClassId(ActiveMember.GetClassId());
	OperationInfo.SetTermId(ActiveMember.GetTerminalID());
	OperationInfo.SetCheckInTime(ActiveMember.GetCheckInTime());
	OperationInfo.SetIdNum(ActiveMember.GetPersonalID());
	CString strRemark = ActiveMember.GetRemark();

	//2016-0201
	//ɾ��LastUser���¼ -- ��������ʱ�Ų�ɾ����¼ 
	if(0 == nCheckoutType && 0 < ActiveMember.GetAvailavleBalance())
	{
		IBA_LOG(_T("OnClientCheckOUt - checkouttype = 0 and balance >0"));
	}	
	else
	{
		CIBADAL::GetInstance()->DeleteLastUser(nMemberId);
	}

	//�»�����=0,�����Ϊ0,�����ǻ����л��������㱻�߳�������״̬Ϊ��������,ʹIBA���ٲ�ѯ.ɾ���ն˺�.������������û���Ϣ
	if(0 == nCheckoutType)
	{
		if(0 == ActiveMember.GetAvailavleBalance())
		{
			//����״̬
			ActiveMember.SetTerminalID(_T(""));
			ActiveMember.SetStatus(CActiveMember::EStatus_CenterOnLine);
			m_lpLocalServer->ActiveMemberList.UpdateActiveMember(ActiveMember, TRUE);
			//���Ӻ���ʾ�ͻ�����Ϣ - ûǮ�ˣ�����
			CClientShortMessage csm;
			CIBAString strBody;
			strBody.LoadString(IDS_NOMONEYLOCK);
			csm.SetMsgBody(strBody);
			csm.SetTermID(sTermId);
			csm.SetTermIP(sIP);
			csm.SetRevTime(CIBAHelpper::GetCurrentTimeAsString(TRUE));
			csm.SetMemberId(nMemberId);

			CIBADAL::GetInstance()->AddClientShortMsg(csm);

			//���Ӳ�����Ϣ,˵�������Ϊ0�����
			CString strMsg(strRemark);
			strRemark.Format(_T("����.�»�����"));
		}
		else
		{
			//���Ӳ�����Ϣ,˵���ǻ��������
			CString strMsg(strRemark);
			strRemark.Format(_T("����.�»�����"));
		}
	}
	else if(2 == nCheckoutType || 202 == nCheckoutType)//��ʾ��Ա�»�
	{
		BOOL bSuccessDBOperation(TRUE);//���ݿ�����Ƿ�ɹ�

		//����ģʽ,�򱾵ؿ����ӽ��˼�¼
		if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
		{
			//��ѯLocalMember�Ƿ��м�¼
			CLocalMember lMember;
			BOOL bHasRecord = CIBADAL::GetInstance()->GetLocalMemberByMemberID(ActiveMember.GetMemberID(),lMember);
			if(bHasRecord)//�м�¼,�����˿���Ϣ
			{
				COleDateTime CheckoutTime = COleDateTime::GetCurrentTime();
				if(0 > CIBADAL::GetInstance()->UpdateLocalMember_CheckOutInfo(ActiveMember.GetMemberID(),CheckoutTime,TRUE))
				{
					bSuccessDBOperation = FALSE;
					IBA_LOG(_T("Error.���±��ؽ��˼�¼ʧ��."));
				}
			}
			else//�޼�¼,������¼,�����¼,���¼�¼
			{
				GxxDictionaryPtr lmPrt = CIBADAL::GetInstance()->CreateNewLocalMemberWithMemberID(ActiveMember.GetMemberID());  //������¼
				if(lmPrt)
				{
					if(CIBADAL::GetInstance()->InsertNewLocalMember(lmPrt))//�����¼
					{
						COleDateTime CheckoutTime = COleDateTime::GetCurrentTime();
						if(0 > CIBADAL::GetInstance()->UpdateLocalMember_CheckOutInfo(ActiveMember.GetMemberID(),CheckoutTime,TRUE))//���¼�¼
						{
							bSuccessDBOperation = FALSE;
							IBA_LOG(_T("Error.���±��ؽ��˼�¼ʧ��."));
						}	
					}
					else
					{
						bSuccessDBOperation = FALSE;
						IBA_LOG(_T("Error.Ϊ���˲��뱾�ؼ�¼ʧ��."));
					}
				}
				else
				{
					bSuccessDBOperation = FALSE;
					IBA_LOG(_T("Error.Ϊ���˴������ؼ�¼ʧ��."));
				}
			}
			//����ʧ��,����ʧ����Ϣ��������־
			if(FALSE == bSuccessDBOperation)
			{
				CString strErrMsg(strRemark);
				strRemark.Format(_T("%s,%s"),LOAD_STRING(IDS_ERROR_DBOPERATION),strErrMsg);
			}
		}
		//���Ӻ���ʾ�ͻ�����Ϣ - ��Ա�»�
		CClientShortMessage csm;
		CIBAString strBody;
		strBody.LoadString(IDS_PAYOUTMEMBER);
		csm.SetMsgBody(strBody);
		csm.SetTermID(sTermId);
		csm.SetTermIP(sIP);
		csm.SetRevTime(CIBAHelpper::GetCurrentTimeAsString(TRUE));
		csm.SetMemberId(nMemberId);

		CIBADAL::GetInstance()->AddClientShortMsg(csm);
		//�����û�,���Ӽ����б�ɾ��
		if(0 != ActiveMember.GetRoomMemberId())
		{
			ActiveMember.SetRoomMemberId(0);
			ActiveMember.SetTerminalID(_T("")); 
			ActiveMember.SetPCClassID(0);
			ActiveMember.SetRemark(LOAD_STRING(IDS_ROOM_HAS_REFUND));
			m_lpLocalServer->ActiveMemberList.UpdateActiveMember(ActiveMember, TRUE);
		}
		else//�ǰ����û�,�Ӽ����б�ɾ��
		{
			m_lpLocalServer->ActiveMemberList.RemoveActiveMember(nMemberId);
		}
	}
	else//��ʱ�û�.
	{
		//����������������ʱ�û�
		return;
	}

	//��ʾ������־
	OperationInfo.SetDescription(strRemark);
	::SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM)(&OperationInfo));

	//ˢ���û���ͼ���ն���ͼ
	m_lpLocalServer->GetActiveMemberView()->Refresh(TRUE);
	m_lpLocalServer->GetComputerListView()->Refresh(FALSE);


			//����ģʽ�£����»򴴽�����LocalMember����isReturned��CheckOutTime
		//if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
		//{
		//	if(!bIsRemoved)
		//	{
		//			//	���Ϊ0,������UDO�����߳������,����Ŀǰ����,����Ҫ��checkout��deregister����,���ֱ���˳�.����Ҳ��CheckActiveMember��ȡ������.
		//		if(ActiveMember.GetAvailavleBalance() == 0)
		//		{
		//			IBA_LOG(_T("Temp.CCommandProcessor::OnClientCheckOut - ���Ϊ0,������UDO�����߳������,Do Nothing!"));
		//			return;
		//		}
		//		if(!CCashierHelper::UpdateLocalMemberForLocalReturn(ActiveMember.GetMemberID()))
		//		{
		//			IBA_LOG(_T("Abnormal.CCommandProcessor::OnClientCheckOut - UpdateLocalMemberForLocalReturn failed!"));
		//			return;
		//		}
		//			//���>0�ı�ʾ�������˵ģ��Ӽ����б���ɾ��
		//		m_lpLocalServer->ActiveMemberList.RemoveActiveMember(ActiveMember.GetMemberID());
		//	}
		//}
}

/*************************************************
Function:       //OnClientLogin
Description:    //����Iserver��ϢS_RESPOND_CLIENT_LOGIN
				//ONLINE�Ĳ�����,֮ǰû��ONLINE��,��������б����ҵ���Ӧ�ն˵��û�,attach��ComputerInfo,�����ӵ�ComoputerList.
				//���ECS_SUSPEND,�Ҽ����б��д����û���Ϣ,�����������һ�.�������û���ͼ״̬ΪActive.
Input:          //pBuffer - ����
				//bufLen - ���ݳ���
*************************************************/
void CCommandProcessor::OnClientLogin(BYTE *pBuffer, size_t bufLen)
{
	MANAGER_COMPUT_INFO ci = {0};
	GetBody(pBuffer, &ci, sizeof(ci));

	CComputerInfo computer(ci);

	//״̬��ΪECS_ONLINE���ն�,ʹ���ն�ID�������б���Ѱ���û�,�ҵ���attach
	// 2011/06/10-gxx: 
	if (computer.GetComputerStatus() != CComputerInfo::ECS_ONLINE)
	{
		CActiveMember ActiveMember;
		if (m_lpLocalServer->ActiveMemberList.GetActiveMember( computer.GetTerminalID(), ActiveMember, TRUE))
		{
			computer.AttachActiveMember(ActiveMember);
		}	
	}

	m_lpLocalServer->ComputerList.AddComputer(computer);

	IBA_TRACE(_T("OnClientLogin = %s\r\n"), computer.GetTerminalID());

	//�Ƴ����������ն���ʾ - QuestionComputerList����IServer��ϢS_RESPOND_QUESTION_COMPUTER
	for (INT i = 0; i < m_QuestionComputerList.GetCount(); i++)
	{
		if (m_QuestionComputerList.GetAt(i).Find(computer.GetTerminalID()) >= 0)
		{
			m_QuestionComputerList.RemoveAt(i);
		}
	}

	m_lpLocalServer->GetComputerListView()->Refresh(FALSE);

	//���Ϊ�����һ�,�����û���ͼΪActive
	if (computer.GetComputerStatus() == CComputerInfo::ECS_SUSPEND)
	{
		CActiveMember ActiveMember;

		if (m_lpLocalServer->ActiveMemberList.GetActiveMember(computer.GetTerminalID(), ActiveMember))
		{
			ActiveMember.SetActive(TRUE);
			m_lpLocalServer->ActiveMemberList.UpdateActiveMember(ActiveMember, TRUE);
			m_lpLocalServer->GetActiveMemberView()->Refresh(FALSE);
		}
	}

}

void CCommandProcessor::OnClientDisconnect(BYTE *pBuffer, size_t bufLen)
{
	MANAGER_CLIENT_DISCONNECT mcd = {0};

	GetBody(pBuffer, &mcd, sizeof(mcd));

	CString strTermID = CA2W(mcd.termID);
	CString strIP = CA2W(mcd.termIP);

	INT_PTR nIndex = m_lpLocalServer->ComputerList.Find(strTermID, strIP);

	if (nIndex >= 0)
	{
		CComputerInfo& computer = m_lpLocalServer->ComputerList.GetComputer(nIndex);
	
		if (computer.GetMemberID() > 0) //����ʧȥ������
		{
			CActiveMember ActiveMember;

			if (m_lpLocalServer->ActiveMemberList.GetActiveMember(computer.GetMemberID(), ActiveMember))
			{
				ActiveMember.SetActive(FALSE);
				m_lpLocalServer->ActiveMemberList.UpdateActiveMember(ActiveMember, TRUE);
			}

			m_lpLocalServer->GetActiveMemberView()->Refresh(FALSE);
		}
		
		computer.SetComputerStatus(CComputerInfo::ECS_LOCKED);
		computer.SetComputerStatus(CComputerInfo::ECS_OFFLINE);
		IBA_LOG(_T("CCommandProcessor::OnClientDisconnect - SetComputerStatus(CComputerInfo::ECS_OFFLINE) for TermId:%s IP:%s"),strTermID,strIP);
		
		if (computer.GetAvailavleBalance() == 0) //���Ϊ0������û���Ϣ
		{
			//computer.ClearUserInfo();
		}

		m_lpLocalServer->ComputerList.UpdateComputer(nIndex, computer);
		m_lpLocalServer->GetComputerListView()->Refresh(FALSE);
	}

	TRACE("OnClientDisconnect\r\n");
}

/*************************************************
Function:       //OnGetComputerList
Description:    //ȡ��Iserver���������ն���Ϣ,���������ն���Ϣ,״̬ΪECS_ONLINE��attach��Ӧ�����û���Ϣ��ComputerInfo
				//�������ӵ�ComputerList,����ComputerListView,�ػ���λͼ
Calls:          //ComputerListView()->Refresh(TRUE),SeatLayoutView()->RedrawWindow()
Input:          //BYTE *pBuffer  - ���յ�������
                //size_t bufLen  - ���ݳ���
*************************************************/
void CCommandProcessor::OnGetComputerList(BYTE *pBuffer, size_t bufLen)
{
	//ȡ�õ�һ��ComputerInfoָ��,���㹲�ж��ٸ�ComputerInfo
	MANAGER_COMPUT_INFO* pComputerInfo = 
		reinterpret_cast<MANAGER_COMPUT_INFO*>(pBuffer + sizeof(NET_HEAD_MAN));

	size_t nComputerCount = (bufLen - sizeof(NET_HEAD_MAN)) / sizeof(MANAGER_COMPUT_INFO);

	m_lpLocalServer->ComputerList.Clear();

	//��������ComputerInfo
	for (size_t i = 0; i < nComputerCount; i++)
	{
		CComputerInfo computer(*(pComputerInfo + i));

		//��m_PCClassMap��ȡ�ö�Ӧ������Ϣ(ClassID)
		std::map<CString, UINT>::const_iterator pos = m_PCClassMap.find(computer.GetTerminalID());
		
		if (pos != m_PCClassMap.end())
		{
			computer.SetPCClassID(pos->second);
		}

		//�ж�Iserver���������ն�״̬
		CComputerInfo::ECOMPUTERSTATUS nStatus = computer.GetComputerStatus();
			//��ΪECS_ONLINE��Ӽ����б���ȡ���û���Ϣ��attach��computer.  ONLINE��Iserver�ᴫ��UpdateData��OnClientUpdateDate�н������ӻ����
		if (nStatus!=CComputerInfo::ECS_ONLINE)
		{
			CActiveMember ActiveMember;
			if (m_lpLocalServer->ActiveMemberList.GetActiveMember(computer.GetTerminalID(),ActiveMember, TRUE))
			{
				computer.AttachActiveMember(ActiveMember);
			}	
		}
		//else
		//{
		//	CString strTermId = computer.GetComputerName();
		//	IBA_LOG(_T("CCommadProcessor::OnGetComputerList - offline client Id = %s, status code = %d"),strTermId,computer.GetComputerStatus());
		//}
		// 2014-3-28 - qsc QSCTAG ��ʱע��
		//else		// 2014-1-17 - qsc �����ʱ����,��Ϊ����״̬,�ֲ��ڼ����б���,������Ϊ0,����Ϊ����	
		//{
		//	CActiveMember ActiveMember;
		//	if (!m_lpLocalServer->ActiveMemberList.GetActiveMember(computer.GetTerminalID(),ActiveMember, TRUE))
		//	{
		//		if (computer.GetAvailavleBalance() == 0)
		//		{
		//			computer.ClearUserInfo();
		//			computer.SetComputerStatus(CComputerInfo::ECS_LOCKED);
		//		}
		//	}
		//}

		//���ն���Ϣ���ӵ��ն��б�
		m_lpLocalServer->ComputerList.AddComputer(computer);
	}
	//����m_ComputerKeyMap(����ΪstrTermID+strComputerIP��ɵ�)
	m_lpLocalServer->ComputerList.MakeComputerKeyMap();

	DecodeQuestionComputer();

	//2015-0919 liyajun ��ʱȡ���ϱ�PC��Ϣ
	// 2015-3-5-tmelody �ϱ�PC��Ϣ
	//if (m_lpLocalServer->GetNeedReportPCInfo())
	//{
	//	m_lpLocalServer->ComputerList.StartReportPCInfo();
	//	m_lpLocalServer->SetNeedReportPCInfo(FALSE);
	//}

	//�����ն���ͼ
	//try{
	m_lpLocalServer->GetComputerListView()->Refresh(TRUE);
	//}catch (...)
	//{}
	// 2014-1-16-qsc �ն���Ϣ��ȡ���˲��������ų�һЩ�����ڵĵ���
	m_lpLocalServer->GetSeatLayoutView()->Loadseat();
	// 2014-1-15-qsc ���������ݣ��ն���Ϣ���ػ�
	m_lpLocalServer->GetSeatLayoutView()->RedrawWindow();
}

/*************************************************
Function:       //OnTermIDPCClassMap
Description:    //�����յ����ն�ID�б�,��ȡ������Ϣ.���浽m_PCClassMap,ͬʱ���浽NetBarConfig::m_TermIDPCClassMap
Input:          //BYTE *pBuffer - ���յ�����
                //size_t bufLen - ���ݳ���
*************************************************/
void CCommandProcessor::OnTermIDPCClassMap(BYTE *pBuffer, size_t bufLen)
{
	CStringA mapStr;
	mapStr = (char*)(pBuffer + sizeof(NET_HEAD_MAN));

	CString strPCClassMap = CA2T(mapStr);

	CStringArray PCClassAarry;

	CIBAHelpper::SplitLine(strPCClassMap, PCClassAarry, '\n');

	for (INT i = 0; i < PCClassAarry.GetCount(); i++)
	{
		CString strTmp = PCClassAarry.GetAt(i);
		
		INT nIdx = strTmp.Find('=');

		CString strKey = strTmp.Left(nIdx);

		UINT nValue = _ttoi(strTmp.Mid(nIdx + 1));

		m_PCClassMap.insert(std::make_pair(strKey, nValue));

		//2015-0824 liyajun �����ն˻���������Ϣ��NetBarConfig
		CNetBarConfig::GetInstance()->m_TermIDPCClassMap = m_PCClassMap;
	}
}

/*************************************************
Function:       // OnClientUpdateData
Description:    // �ӿͻ��˽���ĳһ�ն˵ĸ��±��ģ������ն��б������¶�Ӧ�ļ����û��б�(�ӱ����л�ȡ���µ��ն�״̬���û���Ϣ�����ѵȵ�..)
Calls:          // UpdateComputer, UpdateActiveMember
Table Updated:  // ActiveMember
Input:          // pBuffer �ն���Ϣ����
                // bufLen ���ĳ���
*************************************************/
void CCommandProcessor::OnClientUpdateData(BYTE *pBuffer, size_t bufLen)
{
	IBA_LOG(_T("OnClientUpdata - Start"));
	MANAGER_COMPUT_INFO ci = {0};
	GetBody(pBuffer, &ci, sizeof(ci));
	
	CComputerInfo computer(ci);
	IBA_LOG(_T("UDO��������PersonalID=%d"),computer.GetPersonalID());
	//��MemberID��m_ComputerArray�в����ն�.��MemberId = 0???
	INT_PTR nIndex = m_lpLocalServer->ComputerList.Find(computer.GetMemberID());

	//�������ͬMemberID���û���Ϣ���ն���Ϣ(����ʱ,��һ̨�ն˵��û���Ϣ���ܻ�δ��ɾ��)
	if (nIndex >= 0)
	{
		CComputerInfo& computer2 = m_lpLocalServer->ComputerList.GetComputer(nIndex);
		computer2.ClearUserInfo();//ֻʣ�´󲿷��ն���Ϣ���ն���Ϣ�ϱ�����ʱ��(time_t m_lCheckTime)
		m_lpLocalServer->ComputerList.UpdateComputer(nIndex, computer2);//����nIndexλ�õ�ComputerInfo,��m_UpdateStateΪEUS_CHANGE,PCClassID���ֲ���.����nIndex��m_UpdateIndexArray.
	}

	//���ӵ��ն��б���ˢ���ն���ʾ
	m_lpLocalServer->ComputerList.AddComputer(computer);  //(���ն˺�+IP����)����computerInfo.���Ѵ�����ͬTermId+IP����,�������,��������.����m_UpdateIndexArray��m_ComputerKeyMap
	m_lpLocalServer->GetComputerListView()->Refresh(FALSE); //ˢ�¸��¹���computerInfo���ն���ͼ
	
	IBA_LOG(_T("CCommandProcessor::OnClientUpdateData - TermId = %s IP = %s IDOStauts = %d Status = %d ComputerStatus = %s PersonalID = %s"),computer.GetTerminalID(),computer.GetComputerIP(),computer.GetIDOStatus(),computer.GetStatus(),computer.GetStatusAsString(),computer.GetPersonalID());
	//TermId = 053 IP = 192.168.1.53 IDOStauts = 1 Status = 2 ComputerStatus = �û����� PersonalID = 362226199907042110

	// �����б������ҵ��û�,���ն�attach��activeMember��
	// δ�ҵ����򽫸�ActiveMember���ӵ�Map�У����ü���ʱ��Ϊ��ǰ��Ԥ����Ϊ���(0?)����ϸΪ���(0?)
	//������Map��DB��������ʾ
	BOOL bExists = FALSE;
	CActiveMember ActiveMember;
	if (m_lpLocalServer->ActiveMemberList.GetActiveMember(computer.GetMemberID(), ActiveMember))
	{
		bExists = TRUE;
	}

	ActiveMember.Attach(ci);

	IBA_LOG(_T("Temp.OnClientUpdateData.SetAvailableBalance = %d, for MemberId = %d"),ActiveMember.GetAvailavleBalance(),ActiveMember.GetMemberID());

	//2015-0911 liyajun ��CMapClassID�л�ȡPCClassID
	CString strTermId = computer.GetTerminalID();
	std::map<CString, UINT>::const_iterator pos = m_PCClassMap.find(strTermId);

	if (pos != m_PCClassMap.end())
	{
		computer.SetPCClassID(pos->second);
	}
	ActiveMember.SetPCClassID(computer.GetPCClassID());
	ActiveMember.SetActive(TRUE);

	// 2011/10/27-8210-gxx: 
	if (bExists)
	{
		if (ActiveMember.GetRoomMemberId() == 0)
		{
			UINT nRoomMemberID = 0;
			CLocalServer::GetInstance()->ActiveMemberList.IsRoomArea(computer.GetPCClassID(), nRoomMemberID);
			if (nRoomMemberID > 0) //�������������û���RoomMemberID>0������Ϊ�������ǰ������������ø��û���RoomMemberId��
			{
				ActiveMember.SetRoomMemberId(nRoomMemberID);
				IBA_LOG(_T("Set activeMember.RoomMember = %d"),nRoomMemberID);
				bExists = FALSE;
			}
		}
	}	

	//������Ҫ�������ݿ��Ԥ����ͱ�ע��
	if (!bExists)
	{
		CIBALog::GetInstance()->WriteFormat(_T("OnClientUpdateDate - %d�û�������,�������ӵ���ͼ."), ActiveMember.GetMemberID());

		ActiveMember.SetActivationTime( ActiveMember.GetCheckInTime() );
		ActiveMember.SetAdvancePayment(ActiveMember.GetAvailavleBalance());
		ActiveMember.SetCreditProcess(ActiveMember.GetAvailavleBalanceAsString());
	}

	m_lpLocalServer->ActiveMemberList.UpdateActiveMember(ActiveMember, TRUE);//!bExists);
	m_lpLocalServer->GetActiveMemberView()->Refresh(FALSE);
	IBA_LOG(_T("OnClientUpdata - End"));
}

void CCommandProcessor::OnQuestionComputer(BYTE *pBuffer, size_t bufLen)
{
	m_QuestionComputerList.RemoveAll();

	CStringA strQuestionComputer;

	CString strTmp;

	static const size_t nPerLen = LEN_TERMID + 1 + LEN_IPADDR + 1 + MAX_COMPUTERNAME_LENGTH;

	size_t nCount = (bufLen - sizeof(NET_HEAD_MAN)) / nPerLen;

	for (size_t i = 0; i < nCount; i++)
	{
		strQuestionComputer = (char*)(pBuffer + sizeof(NET_HEAD_MAN) + nPerLen * i);

		strTmp = CA2WEX<512>(strQuestionComputer);
		
		m_QuestionComputerList.Add(strTmp);
	}

	DecodeQuestionComputer();

	m_lpLocalServer->GetComputerListView()->Refresh(FALSE);
}

void CCommandProcessor::DecodeQuestionComputer()
{
	CString strTermID, strIP, strTmp;

	for (INT i = 0; i < m_QuestionComputerList.GetCount(); i++)
	{
		strTmp = m_QuestionComputerList.GetAt(i);

		UINT nPos1 = strTmp.Find(_T('/'));
		UINT nPos2 = strTmp.Find(_T('~'));

		strTermID = strTmp.Left(nPos1);
		strIP = strTmp.Mid(nPos1 + 1, nPos2 - nPos1 - 1);

		INT_PTR nIndex = m_lpLocalServer->ComputerList.Find(strTermID, strIP);

		if (nIndex >= 0)
		{
			CComputerInfo& computer = m_lpLocalServer->ComputerList.GetComputer(nIndex);
			computer.SetComputerStatus(CComputerInfo::ECS_THIEF);
			m_lpLocalServer->ComputerList.UpdateComputer(nIndex, computer);			
		}
	}
}

void CCommandProcessor::OnGetComputerCount(BYTE *pBuffer, size_t bufLen)
{

}

void CCommandProcessor::OnClientShortMessage(BYTE *pBuffer, size_t bufLen)
{
	if (CIBAConfig::GetInstance()->GetPauseRevMsg()) //��ͣ������Ϣ
	{
		return;
	}

	if (!theApp.GetCurCashier()->IsLogin())
	{
		return;
	}

	SHORT_MESSAGE_INFO shortMsgInfo  = {0};

	GetBody(pBuffer, &shortMsgInfo, sizeof(shortMsgInfo));	

	if (shortMsgInfo.shortMsg.side == 1 &&
		CIBAConfig::GetInstance()->GetCashRegisterID() == (UINT)shortMsgInfo.shortMsg.counterID) //�Լ����͵�
	{
		return;
	}

	m_lpLocalServer->GetComputerListView()->OnClientShortMessage(shortMsgInfo);
}

void CCommandProcessor::OnSendShortMsgResp(BYTE* /*pBuffer*/, size_t /*bufLen*/)
{
	
}

void CCommandProcessor::OnRespondCaptureScreen(BYTE *pBuffer, size_t /*bufLen*/)
{
	if (!m_bCaptureScreen)
	{
		return;
	}

	m_bCaptureScreen = FALSE;

	try
	{
		NET_HEAD_MAN head = {0};
		SCREEN_TERMINFO stTermInfo = {0};

		GetHead(pBuffer, &head);
		GetBody(pBuffer, &stTermInfo, sizeof(stTermInfo));

		CByteArray buf;
		buf.SetSize(head.Length - sizeof(stTermInfo));
		CopyMemory(buf.GetData(), pBuffer + sizeof(head) + sizeof(stTermInfo), buf.GetSize());

		CString strPicFile(theApp.GetWorkPath() + _T("\\IBAScreen\\"));

		CIBAHelpper::MakeSurePathExists(strPicFile);

		strPicFile.Append(CA2W(stTermInfo.termID));
		strPicFile.Trim();
		strPicFile.AppendFormat(_T("[%s].jpg"), CIBAHelpper::GetCurrentTimeAsString());

		SaveFile(strPicFile, buf);
	}
	catch (CFileException* e)
	{		
		e->Delete();
		CIBALog::GetInstance()->Write(_T("Capture Screen: Operation file fails."));
	}
	//catch (...)
	//{
	//	CIBALog::GetInstance()->Write(_T("Capture Screen failed."));
	//}
}

void CCommandProcessor::OnRespondGetLog(BYTE *pBuffer, size_t /*bufLen*/)
{
	if (!m_bGetLog)
	{
		return;
	}
	
	m_bGetLog = FALSE;

	try
	{
		NET_HEAD_MAN head = {0};
		SCREEN_TERMINFO stTermInfo = {0};

		GetHead(pBuffer, &head);
		GetBody(pBuffer, &stTermInfo, sizeof(stTermInfo));

		CByteArray buf;
		buf.SetSize(head.Length - sizeof(stTermInfo));
		CopyMemory(buf.GetData(), pBuffer + sizeof(head) + sizeof(stTermInfo), buf.GetSize());

		CString strPicFile(theApp.GetWorkPath() + _T("\\IBATermLog\\"));
		CIBAHelpper::MakeSurePathExists(strPicFile);

		strPicFile.Append(CA2W(stTermInfo.termID));
		strPicFile.Trim();

		if (head.Version == 1) //zip
		{
			strPicFile.AppendFormat(_T("[%s].zip"), CIBAHelpper::GetCurrentTimeAsString());
		}
		else
		{
			strPicFile.AppendFormat(_T("[%s].log"), CIBAHelpper::GetCurrentTimeAsString());
		}
		
		SaveFile(strPicFile, buf);

	}
	catch (CFileException* e)
	{		
		e->Delete();
		CIBALog::GetInstance()->Write(_T("Get Log: Operation file failed."));
	}
	//catch (...)
	//{
	//	CIBALog::GetInstance()->Write(_T("Get Log failed."));
	//}
}

void CCommandProcessor::SaveFile(CString strFile, CByteArray &buf)
{
	CFile File;

	if (File.Open(strFile, CFile::modeCreate | CFile::modeWrite))
	{
		File.Write(buf.GetData(), (UINT)buf.GetSize());
		File.Flush();
		File.Close();
		
		if (CIBAConfig::GetInstance()->GetOpenAfterSave())
		{
			ShellExecute(NULL, _T("open"), strFile, NULL, NULL, SW_SHOWNORMAL);
		}
	}
	else
	{
		CIBALog::GetInstance()->Write(_T("SaveFile: Create file failed."));
	}
}

void CCommandProcessor::OnTermUnLock(BYTE *pBuffer, size_t bufLen)
{
	MANAGER_COMPUT_INFO ci = {0};

	GetBody(pBuffer, &ci, sizeof(ci));

	CString strTermID = CA2W(ci.termID);
	CString strIP = CA2W(ci.IPAddr);

	INT_PTR nIndex = m_lpLocalServer->ComputerList.Find(strTermID, strIP);

	if (nIndex >= 0)
	{
		CComputerInfo& computer = m_lpLocalServer->ComputerList.GetComputer(nIndex);
		computer.Attach(ci);

		if (5 == ci.dwIDOStatus) //�������
		{
			computer.ClearUserInfo();
		}

		computer.SetComputerStatus(7 == ci.dwIDOStatus ? CComputerInfo::ECS_UNLOCK : CComputerInfo::ECS_LOCKED);

		m_lpLocalServer->ComputerList.UpdateComputer(nIndex, computer);
		m_lpLocalServer->GetComputerListView()->Refresh(FALSE);
	}

	if (7 == ci.dwIDOStatus) //����
	{
		NS_TTS::CIBATTS::GetInstance()->SpeakUnlock();
	}
}

void CCommandProcessor::OnPingClientResp(BYTE *pBuffer, size_t bufLen)
{
	PINGCLIENTRESP pcr = {0};
	GetBody(pBuffer, &pcr, sizeof PINGCLIENTRESP);

	CString strTermID = CA2W(pcr.termId);
	CString strIP = CA2W(pcr.termIp);

	INT_PTR nIndex = m_lpLocalServer->ComputerList.Find(strTermID, strIP);

	if (nIndex >= 0)
	{
		CComputerInfo& computer = m_lpLocalServer->ComputerList.GetComputer(nIndex);

		if (computer.GetComputerStatus() == CComputerInfo::ECS_THIEF)
		{
			if (0 == pcr.cCode) //��ͨ
			{
				computer.SetComputerStatus(CComputerInfo::ECS_OFFLINE);
				IBA_LOG(_T("CCommandProcessor::OnPingClientResp - SetComputerStatus(CComputerInfo::ECS_OFFLINE) for TermId:%s IP:%s"),strTermID,strIP);
			}
		}
		else if (computer.GetComputerStatus() == CComputerInfo::ECS_OFFLINE)
		{
			if (1 == pcr.cCode) //ͨ
			{
				computer.SetComputerStatus(CComputerInfo::ECS_THIEF);
			}
		}

		m_lpLocalServer->ComputerList.UpdateComputer(nIndex, computer);
		m_lpLocalServer->GetComputerListView()->Refresh(FALSE);
	}
}

void CCommandProcessor::OnAutoReturnResp(BYTE *pBuffer, size_t bufLen)
{
	AUTORETURNMSG prm = {0};
	GetBody(pBuffer, &prm, sizeof AUTORETURNMSG);
	
	IBA_LOG(_T("CCommandProcessor::OnAutoReturnResp - receive IServer return information-----------"));
	CString strCheckInTime;
	CString strCheckOutTime;

}

void CCommandProcessor::OnGetNewNotice(BYTE *pBuffer, size_t bufLen)
{
	CIBALog::GetInstance()->Write(_T("��ȡ����ɹ�"));

	TNewNotice newNotice = {0};
	GetBody(pBuffer, &newNotice, sizeof TNewNotice);

	CString strTitle = CA2W(newNotice.cNoticeTitle);
	CString strInfo = CA2W(newNotice.cNoticeInfo);
	CString strLink = CA2W(newNotice.cNoticeLink);
	BOOL bShouldSign = BOOL(newNotice.nNoticeFlag);

	((CMainFrame*)theApp.GetMainWnd())->ShowNotice(strTitle,strInfo,strLink,bShouldSign);

}

void CCommandProcessor::OnLoingZd( BYTE *pBuffer, size_t bufLen )
{
	IBA_LOG0(_T("OnLoinZd"));
	// ������̨�����������
	if (CIBAConfig::GetInstance()->GetCashRegisterID() == 1)
	{
		if(CIBAGlobal::emRealnameZD == CNetBarConfig::GetInstance()->GetAuditType())
		{
			TLOGIN_ZD login_zd = {0};
			GetBody(pBuffer, &login_zd, sizeof TLOGIN_ZD);
			CActiveMember ActiveMember;

			CString strNetId = CA2T(login_zd.szNetid);

			if(strNetId.GetLength() < 10)
			{
				strNetId.Insert(0, CString(_T("0000000000")).Left(10 - strNetId.GetLength()));
			}
			bool bChange = false;
			CString strDBTermId;
			COleDateTime oleTime;
			if(CLocalServer::GetInstance()->ActiveMemberList.GetActiveMemberNetID(strNetId, ActiveMember))
			{
				bChange = !ActiveMember.GetTerminalID().IsEmpty();		// ��Ϊ��˵���ǻ���

				// ���ݿ���û����ؼ�¼��˵�����ǻ���
				// ����ָ���ϻ��նˣ���ֱ�����û���ͼ�������ݷ�ֹ�����������
				CIBADAL::GetInstance()->GetZdRnInfo(strNetId, strDBTermId, oleTime);		
				if(strDBTermId.IsEmpty())
				{
					bChange = false;
				}
			}

			ID_LOGINRETURN_ZD loginReturn_zd;
			CIZd::ID_LOGINRETURN loginReturn;
			ZeroMemory(&loginReturn_zd, sizeof(loginReturn_zd));
			ZeroMemory(&loginReturn, sizeof(loginReturn));
			// ��һ�ε�¼����OpenUserEx����������LoginEx
			if(bChange)
			{
				// ͬһ̨������˵�����ǻ�����ֻ�������ڻ��������Ĳ���
				COleDateTimeSpan span = COleDateTime::GetCurrentTime() - oleTime;
				if(0 == strDBTermId.CompareNoCase(CA2T(login_zd.szTermId)) && 
					span.GetTotalSeconds() < 20)		// 20������̨ͬ����
				{
					loginReturn.iErrCode = 0;
				}
				else
				{
					if(CIZd::GetInstance()->IsIDPwdCorrect(login_zd.szNetid, login_zd.szPwd))
					{
						IBA_LOG(_T("%s����%s"), strDBTermId, CA2T(login_zd.szTermId));
						// 2014-10-30 - qsc �������Ȱ�ԭ�����»�
						CString strIP;
						CIBADAL::GetInstance()->GetIPFromTermId(strDBTermId, strIP);
						CIZd::GetInstance()->V2_Logout(login_zd.szNetid, login_zd.szNetid, CT2A(strIP));
						
						loginReturn = CIZd::GetInstance()->V2_LoginEx(
							login_zd.szNetid, login_zd.szNetid, login_zd.szPwd, login_zd.szIP);	

						if(0 == loginReturn.iErrCode)
						{
							CIBADAL::GetInstance()->AddZdRnInfo(strNetId, CA2T(login_zd.szTermId), CA2T(loginReturn.szName));
						}
					}
					else
					{
						loginReturn.iErrCode = -9;
						strcpy(loginReturn.szErrMsg, "�����������");
					}
				}
			}
			else
			{
				loginReturn = CIZd::GetInstance()->V2_OpenUserEx(
					login_zd.szNetid, login_zd.szNetid, login_zd.szPwd, login_zd.szIP, "1");

				if(0 == loginReturn.iErrCode)
				{
					CIBADAL::GetInstance()->AddZdRnInfo(strNetId, CA2T(login_zd.szTermId), CA2T(loginReturn.szName));
				}
			}
			//char szIP[20];
			//char szTermId[15];

			// �������Ƕಿ��
			//char szCardId[21];		//��������
			//int  iCardType;			//��������ȡ���� 0������֤ 1���ֻ� 2��ָ�� 3�����ӵǼ�
			//char szName[31]; 		//����
			//int   iCertType;		//֤������
			//char szCertNumber[19];	//֤������
			//int  iErrCode;			//����ţ�������������0��ʾ�ɹ�
			//char szErrMsg[101];		//�������������Error��Ϊ0�Ļ�
			strncpy(loginReturn_zd.szTermId, login_zd.szTermId, 15);
			strncpy(loginReturn_zd.szIP,	 login_zd.szIP, 20);
			strncpy(loginReturn_zd.szCardId, loginReturn.szCardId, 21);
			strncpy(loginReturn_zd.szName,	loginReturn.szName, 31);
			strncpy(loginReturn_zd.szCertNumber, loginReturn.szCertNumber, 19);
			strncpy(loginReturn_zd.szErrMsg, loginReturn.szErrMsg, 101);
			loginReturn_zd.iCardType	= loginReturn.iCertType;
			loginReturn_zd.iErrCode		= loginReturn.iErrCode;

			CLocalServer::GetInstance()->LocalLoginZdReturn(&loginReturn_zd);
		}	
	}
}