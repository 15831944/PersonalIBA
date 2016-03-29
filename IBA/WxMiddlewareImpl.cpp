#include "StdAfx.h"
#include "WxMiddlewareImpl.h"
#include "Dal\IBADAL.h"
#include "Dal\ConfigInfo.h"
#include <atlutil.h>
#include "NetBarConfig.h"
#include "IBAConfig.h"
#include "IBALog.h"
#include "ComputerInfo.h"
#include "Socket\LocalServer.h"
#include "resource.h"
#include "IBAHelpper.h"
#include "ComputerListView.h"
#include "ActiveMemberView.h"
#include "WxMsgTips.h"

CWxMiddlewareImpl* CWxMiddlewareImpl::GetInstance()
{
	static CWxMiddlewareImpl* instance = NULL;
	if (instance == NULL) {
		instance = new CWxMiddlewareImpl();
	}

	return instance;
}

CWxMiddlewareImpl::CWxMiddlewareImpl()
{
	
}

CWxMiddlewareImpl::~CWxMiddlewareImpl()
{

}

void CWxMiddlewareImpl::getNetbarIdAndDomainId( UINT * netbarId, UINT * domainId )
{
	*domainId = CNetBarConfig::GetInstance()->GetDomainId();
	*netbarId = CNetBarConfig::GetInstance()->GetNetBarId();
}

BOOL CWxMiddlewareImpl::getMiddlewareCenterIPPort( TCHAR* ip, UINT * port )
{
	//char szIP[64];
	//GetPrivateProfileStringA("WXMiddleWare","IP","192.168.10.193",szIP,sizeof(szIP),".\\IBAConfig\\IBA.ini");

	//*port = GetPrivateProfileInt(_T("WXMiddleWare"),_T("port"),8080,_T(".\\IBAConfig\\IBA.ini"));

	//in_addr addr;
	//addr.S_un.S_addr = inet_addr(szIP);
	//char* pszIp = inet_ntoa(addr);
	//_tcscpy(ip,CA2T(pszIp));

	//return TRUE;
	IBA_TRACE(_T("��ȡIP΢���м��IP�Ͷ˿�"));
	
	hostent* pHost = gethostbyname("notice.dodonew.com");
	if (pHost)
	{
		in_addr addr;
		char* pList = pHost->h_addr_list[0];
		if (!pList)
		{
			IBA_LOG0(_T("��ȡ�б�ʧ��"));
			return FALSE;
		}

		memcpy(&addr.S_un.S_addr, pList, pHost->h_length);
		char* pszIp = inet_ntoa(addr);
		_tcscpy(ip, CA2T(pszIp));

		CConfigInfo configInfo;
		configInfo.SetKeyString(_T("noticeServerAddress"));
		if (CIBADAL::GetInstance()->GetSystemConfig(configInfo))
		{
			CUrl url;
			url.CrackUrl(configInfo.GetKeyValue3());
			*port = (UINT)url.GetPortNumber();

			return TRUE;
		}
	}
	else
	{
		IBA_LOG0(_T("��ȡ�������ʧ�ܡ�"));
	}

	return FALSE;
}

VOID CALLBACK CWxMiddlewareImpl::TimerProc(        //ʱ��ص�����
							   HWND hwnd,         // handle to window
							   UINT uMsg,         // WM_TIMER message
							   UINT_PTR idEvent,  // timer identifier
							   DWORD dwTime       // current system time
							   )
{
	//std::vector<CheckinInfo>::iterator it = CWxMiddlewareImpl::GetInstance()->m_termsArray.begin();
	std::vector<OperationInfo>::iterator it = CWxMiddlewareImpl::GetInstance()->m_termsArray.begin();
	for (; it != CWxMiddlewareImpl::GetInstance()->m_termsArray.end(); ++it)
	{
		if (it->nTimerId == idEvent) 
		{
			KillTimer(NULL, idEvent);

			if (0 == it->nOperate)			// checkin
			{
				//�Ӽ����б�����û�,�ҵ����Ҳ�����֪ͨ�м�����. 
				CString strWhere;
				//strWhere.Format(_T("netId like '%%%s%%' and TermId='%s'"), it->strNetId.GetBuffer(), it->strTermId.GetBuffer());
				strWhere.Format(_T("netId like '%%%s%%' and TermId='%s'"), it->strNetId, it->strTermId);		

				if (CIBADAL::GetInstance()->GetTableRecordCount(_T("ActiveMember"), strWhere) > 0)
				{
					// ��֪�м��
					CWxMiddlewareImpl::GetInstance()->CallLoginStatus(it->strTermId, 0);
				}
				else
				{
					// ��֪�м��
					CWxMiddlewareImpl::GetInstance()->CallLoginStatus(it->strTermId, 1);
				}
			}
			else if (1 == it->nOperate)		// checkout  // 0:checkin  1:checkout 2:message
			{
				CString strWhere;
				//strWhere.Format(_T("netId like '%%%s%%' and TermId='%s'"), it->strNetId.GetBuffer(), it->strTermId.GetBuffer());
				strWhere.Format(_T("netId like '%%%s%%' and TermId='%s'"), it->strNetId, it->strTermId);

				if (CIBADAL::GetInstance()->GetTableRecordCount(_T("ActiveMember"), strWhere) > 0)
				{
					// ��֪�м��
					CWxMiddlewareImpl::GetInstance()->CallLogoutStatus(it->strTermId, 1);
				}
				else
				{
					// ��֪�м��
					CWxMiddlewareImpl::GetInstance()->CallLogoutStatus(it->strTermId, 0);
				}
				
			}
			else if (2 == it->nOperate)
			{
				CWxMiddlewareImpl::GetInstance()->CallMessageStatus(1);
			}
			else
			{
				continue;
			}

			GetInstance()->m_termsArray.erase(it);
		}
	}

	
}

BOOL CWxMiddlewareImpl::checkin( LPCTSTR lpszNetId, LPCTSTR lpszTermId, LPCTSTR lpszPassword )
{
	IBA_LOG(_T("����΢���м���ϻ���Ϣ"));

	CComputerInfo computer ;
	if (!CLocalServer::GetInstance()->ComputerList.GetComputerInfo(lpszTermId, computer))
	{
		IBA_LOG(_T("�ϻ�ʱ���ӻ�����ͼ��ȡ�˺ţ�%s��Ӧ�Ļ�����%sʧ��"), lpszNetId, lpszTermId);
		this->CallLoginStatus(lpszTermId, 1);
		return FALSE;
	}
		
	//{ 2011/07/27-8201-gxx: 
	CActiveMember ActiveMember;
	if (!CLocalServer::GetInstance()->ActiveMemberList.GetActiveMemberNetID(lpszNetId, ActiveMember, FALSE))
	{
		IBA_LOG(_T("�ϻ�ʱ�����û���ͼ��ȡ�˺ţ�%s��Ӧ���û�ʧ��"), lpszNetId);
		this->CallLoginStatus(lpszTermId, 1);
		return FALSE;
	}

	// 2011/11/03-8210-gxx: �������˺��ϻ�
	if (ActiveMember.GetMemberID() == ActiveMember.GetRoomMemberId())
	{
		if (ActiveMember.GetPCClassID() != computer.GetPCClassID())
		{
			IBA_LOG(LOAD_STRING(IDS_ROOM_CHECKIN_ERROR1));
			this->CallLoginStatus(lpszTermId, 1);
			return FALSE;
		}
	}
		
	//��ӵ����ݿ�
	CLastUserInfo LastUserInfo;
	LastUserInfo.SetNetId(lpszNetId);
	LastUserInfo.SetMemberId(ActiveMember.GetMemberID());
	LastUserInfo.SetTermId(computer.GetTerminalID());
	LastUserInfo.SetLoginStyle(1/*GetCheckIn() ? 1 : 0*/); //�Զ���¼Ҫ��ǿ�Ƶ�¼����
	LastUserInfo.SetUpdateTime(CIBAHelpper::GetCurrentTimeAsString(TRUE));
	LastUserInfo.SetPassWord(lpszPassword);
	
	CIBADAL::GetInstance()->DeleteLastUserInfo(LastUserInfo.GetTermId());
	CIBADAL::GetInstance()->AddLastUserInfo(LastUserInfo);
	IBA_LOG(_T("Temp.CWxMiddlewareImpl::checkin AddLastUserInfo"));
			
	if (computer.GetComputerStatus() == CComputerInfo::ECS_LOCKED)
	{
		INT_PTR nIdx = CLocalServer::GetInstance()->ComputerList.Find(computer.GetTerminalID(), computer.GetComputerIP());
		CLocalServer::GetInstance()->LocalAutoCheckIn(nIdx);
	}

	// ��UDO�����ϻ���Ϣ�ɹ�
	//CheckinInfo info;
	OperationInfo info;
	info.strNetId = lpszNetId;
	info.strTermId = lpszTermId;
	info.nOperate = 0;
	info.nTimerId = SetTimer(NULL, 0, 1000, TimerProc);
	m_termsArray.push_back(info);

	return TRUE;
}

BOOL CWxMiddlewareImpl::checkout( LPCTSTR lpszNetId, LPCTSTR lpszTermId )
{
	IBA_TRACE(_T("����΢���м���»���Ϣ"));
	IBA_LOG(_T("����΢���м���»���Ϣ"));

	//�ӻ�����ͼ��ȡ�ն�,��ȡ����-return
	CComputerInfo computer;
	if (!CLocalServer::GetInstance()->ComputerList.GetComputerInfo(lpszTermId, computer))
	{
		IBA_LOG(_T("�»�ʱ���ӻ�����ͼ��ȡ�˺ţ�%s��Ӧ�Ļ�����%sʧ��"), lpszNetId, lpszTermId);
		this->CallLogoutStatus(lpszTermId, 1);
		return FALSE;
	}

	//�Ӽ����б��ȡ�û�,��ȡ����-return
	CActiveMember ActiveMember; //�û�ûǮ�Զ�����������������
	if (!CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(computer.GetMemberID(), ActiveMember))
	{
		IBA_LOG(_T("�»�ʱ�����û���ͼ��ȡ�˺ţ�%s��Ӧ���û�ʧ��"), lpszNetId);
		this->CallLogoutStatus(lpszTermId, 1);
		return FALSE;
	}

	//��CCheckOutinfo��Ӽ�¼,����ʱ��Ϊ��ǰʱ��
	CCheckOutinfo CheckOutInfo;
	CheckOutInfo.SetCheckOutTime(CIBAHelpper::GetCurrentTimeAsString(TRUE));
	CheckOutInfo.SetTerminalId(ActiveMember.GetTerminalID());
	CheckOutInfo.SetSerialNum(ActiveMember.GetNetId());
	CheckOutInfo.SetMemberId(ActiveMember.GetMemberID());
	CheckOutInfo.SetUserName(ActiveMember.GetUserName());
	CheckOutInfo.SetPersonalId(ActiveMember.GetPersonalID());
	CheckOutInfo.SetClassName(ActiveMember.GetUserClassName());
	CheckOutInfo.SetClassId(ActiveMember.GetClassId());
	CheckOutInfo.SetClassState(ActiveMember.IsMember());
	CheckOutInfo.SetBalanceMoney(ActiveMember.GetAvailavleBalance());
	CheckOutInfo.SetReturnState(1); // ����״̬Ϊ���˿�

	BOOL bRes = CIBADAL::GetInstance()->AddCheckOutInfo(CheckOutInfo);
	if (!bRes)
	{
		IBA_LOG(_T("�»�ʱ�����ݿ����ʧ�ܣ��˺ţ�%s��Ӧ�Ļ�����%s"), lpszNetId, lpszTermId);
		this->CallLogoutStatus(lpszTermId, 1);
		return FALSE;
	}

	// ��ѯ����
	INT_PTR nIdx = CLocalServer::GetInstance()->ComputerList.Find(computer.GetTerminalID(), computer.GetComputerIP());
	if (nIdx < 0)
	{
		IBA_LOG(_T("�»�ʱ����ѯ��������ʧ�ܣ��ն˺ţ�%s�ն˵�ַ��%s"), computer.GetTerminalID(), computer.GetComputerIP());
		this->CallLogoutStatus(lpszTermId, 1);
		return FALSE;
	}

	// ֪ͨ�ͻ��˽�������
	if (computer.GetComputerStatus() != CComputerInfo::ECS_LOCKED)
	{
		CLocalServer::GetInstance()->LocalCheckOutUser(nIdx, computer.GetMemberID(), 2);
		Sleep(2000);
		CIBADAL::GetInstance()->DeleteLastUserInfo(CheckOutInfo.GetTerminalId());
	}

	// ��UDO�����»���Ϣ�ɹ�
	OperationInfo info;
	info.strNetId = lpszNetId;
	info.strTermId = lpszTermId;
	info.nOperate = 1;
	info.nTimerId = SetTimer(NULL, 0, 1000, TimerProc);
	m_termsArray.push_back(info);

	return TRUE;
}

void CWxMiddlewareImpl::showmsg(LPCTSTR lpszTermId, LPCTSTR lpszUserID, LPCTSTR lpszPayAmount, LPCTSTR lpszGiveAmount, LPCTSTR lpszTime)
{
	IBA_LOG0(_T("����΢���м����ʾ��Ϣ"));

	static CWxMsgTips* pTips = NULL;
	if (pTips)
	{
		if (!IsWindow(pTips->GetSafeHwnd()))
			pTips = NULL;
	}

	if (!pTips)
	{
		pTips = new CWxMsgTips();
		if (!pTips->Create())
			pTips = NULL;
	}

	if (pTips)
	{
		pTips->SetUserID(lpszUserID);
		pTips->SetPayAmount(lpszPayAmount);
		pTips->SetGiveAmount(lpszGiveAmount);
		pTips->SetTermID(lpszTermId);
		pTips->SetTime(lpszTime);

		pTips->ShowRBCorner();
	}

	OperationInfo info;
	info.nOperate = 2;
	info.nTimerId = SetTimer(NULL, 0, 1000, TimerProc);
	m_termsArray.push_back(info);
}

void CWxMiddlewareImpl::LoadWxLibray()
{
	CString strWorkPath = CIBAConfig::GetInstance()->GetAppPath();
	strWorkPath.Append(_T("\\WXM.dll"));
	m_hMod = LoadLibrary(strWorkPath);

	if (m_hMod != NULL) {
		//AfxMessageBox(_T("load dll!"));
		m_procSetDelegate = (fnSetDelegateProc)::GetProcAddress(m_hMod, "SetDelegate");
		m_procLoginStatus = (fnLoginStatusProc)::GetProcAddress(m_hMod, "LoginStatus");
		m_procLogoutStatus = (fnLogoutStatusProc)::GetProcAddress(m_hMod, "LogoutStatus");
		m_procMessageStatus = (fnMessageStatusProc)::GetProcAddress(m_hMod, "MessageStatus");

		this->CallSetDelegate();

		m_thread.SetCallBack(this, Run);
		//m_thread.Start();
		//m_thread.Suspend();
	}
}

void CWxMiddlewareImpl::CallSetDelegate()
{
	m_procSetDelegate(this);
}

void CWxMiddlewareImpl::CallLoginStatus( LPCTSTR lpszTermId, int nStatus )
{
	m_procLoginStatus(lpszTermId, nStatus);
}

void CWxMiddlewareImpl::CallLogoutStatus(LPCTSTR lpszTermId, int nStatus)
{
	m_procLogoutStatus(lpszTermId, nStatus);
}

void CWxMiddlewareImpl::CallMessageStatus(int nStatus)
{
	m_procMessageStatus(nStatus);
}

void CWxMiddlewareImpl::Run()
{
	
}



