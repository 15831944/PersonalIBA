#include "stdafx.h"
#include "IBA.h"
#include "MainFrm.h"
#include "IBAHelpper.h"
#include "DAL\IBADAL.h"
#include "TTS\IBATTS.h"
#include "Socket\LocalServer.h"
#include "RealName\IBARealName.h"
#include "Servlet\WinHttpRequest.h"
#include ".\ServletHellper.h"
#include "CashierHelper.h"
#include "CurrentNetBarUser.h"
#include "RealName\AuditInterface.h"
#include "RealName\RZXRealName.h"
#include "GlobalFunctions.h"
#include "ComputerListView.h"

/*************************************************
Function:       // GetMemberId
Description:    // ʹ��NetId������CheckCard���ɹ�����MemberId�����򷵻�0
*************************************************/

UINT CServletHellper::GetMemberId(LPCTSTR lpszNetId)
{
	UINT nCardId = 0;

	return GetMemberId(lpszNetId, nCardId);
}

/*************************************************
Function:       // GetMemberId
Description:    // ʹ��NetId������CheckCard
Calls:          // DoCheckCard
Input:          // lpszNetId - ����У����˺�
Output:         // nCardId - ���صĿ���
Return:         // �ɹ�����MemberId�����򷵻�0
Others:         // None
*************************************************/

UINT CServletHellper::GetMemberId(LPCTSTR lpszNetId, UINT& nCardId)
{
	CIDCheckCardMethod checkCard;
	checkCard.SetSerialNum(lpszNetId);

	theApp.GetCurCashier()->DoCheckCard(checkCard);

	if (checkCard.GetStatusCode() == 0)
	{
		nCardId = checkCard.GetCardId();

		return checkCard.GetMemberId();
	}

	return 0;
}

/*************************************************
Function:       // ActivationMember
Description:    // �����Ľ��м�� ����Ҫ�����������Լ�ʵ�����������
Calls:          // CCashier::DoActivation
Input:          // nMemberId��Ա�ţ�nActivationType��������(Ĭ��Ϊ0)
Return:         // ���ļ���ɹ�����TRUE����������FALSE
*************************************************/
BOOL CServletHellper::ActivationMember(UINT nMemberID, UINT nActivationType /*=0*/)
{
	IBA_LOG(_T("CServletHellper::ActivationMember Start - MemberId = %d, ActivationType = %d"),nMemberID,nActivationType);
	//�����Ƿ�����������
	if (CNetBarConfig::GetInstance()->GetNeedActivation() == 0)
	{
		IBA_LOG(_T("CServletHellper::ActivationMember Result - FALSE - NeedActivation was set to FALSE"));
		return FALSE;
	}

	//ʵ���Ƿ������ϻ�����
	if (!theApp.GetRealName()->IsCheckInEnabled())
	{
		IBA_LOG(_T("CServletHellper::ActivationMember Result - FALSE - CheckIn is Disabled for this RealName"));
		return FALSE;
	}

	CActivationMethod Activation;

	Activation.SetMemberId(nMemberID);
	Activation.SetMode( CActivationMethod::ACTIVATION_YES );
	Activation.SetDotType(nActivationType);

	IBA_LOG(_T("Temp.CServletHellper::ActivationMember DoActivation - for MemberId = %d"),nMemberID);
	theApp.GetCurCashier()->DoActivation(Activation);

	if (Activation.GetStatusCode() == 0)
	{	
		IBA_LOG(_T("CServletHellper::ActivationMember Result - TRUE - MemberId = %d"),nMemberID);
		//����ǽ���ʵ�����ϴ��û���Ϣ
		if(CNetBarConfig::GetInstance()->GetAuditType() == CIBAGlobal::emRealnameJQ)
		{
			try
			{
			SendRegDataToProxy(nMemberID);
			}
			catch (...)
			{
				IBA_LOG(_T("��ʵ���ӿڷ��ͼ�����Ϣ����! %d"), GetLastError());
			}

		}
		return TRUE;
	}
	else
	{
		theApp.IBAMsgBox(Activation.GetStatusMessage());
	}

	IBA_LOG(_T("CServletHellper::ActivationMember Result - FALSE - Execution error with Center!"));
	return FALSE;
}

/*************************************************
Function:       // Synchronize
Description:    // �뱾��Serverͬ��
Calls:          // 
Table Accessed: // 
Table Updated: // 
Input:          // 
                  // 
Output:         // 
Return:         // 
Others:         // 
*************************************************/
BOOL CServletHellper::Synchronize()
{
	CString strNetBarKey = CNetBarConfig::GetInstance()->GetNetBarKey();
	CString strCenterIP = CNetBarConfig::GetInstance()->GetMainCenterIP();
	CString strPath = CNetBarConfig::GetInstance()->GetMainCenterPath();
	
	UINT nNetBarID = CNetBarConfig::GetInstance()->GetNetBarId();
	UINT nCenterPort = CNetBarConfig::GetInstance()->GetMainCenterPort();

	CString strURL;
	strURL.Format(_T("http://%s:8080/netbar/synchronize/cashregister.jsp?mode=%d&psk=%s&netBarId=%d&path=http://%s:%d/%s"),
		CNetBarConfig::GetInstance()->GetLocalServerIP(), 1, strNetBarKey,
		nNetBarID, strCenterIP, nCenterPort, strPath);

	CIBALog::GetInstance()->WriteFormat(_T("Synchronize URL = %s"), strURL);

	CWinHttpRequest WinHttpRequest; 

	if (WinHttpRequest.GetReadyState() == 0)
	{
		return FALSE;
	}

	WinHttpRequest.SetTimeouts(5000, 5000, 5000, 60000);
	WinHttpRequest.Open(strURL);
	WinHttpRequest.Send();

	if (WinHttpRequest.GetReadyState() == 4) //ִ�гɹ�
	{
		CString strTmp = WinHttpRequest.GetResponseText().Trim();

		CIBALog::GetInstance()->Write(strTmp);

		INT nIdx = strTmp.Find(_T("ʧ��"));

		if (nIdx >= 0 && nIdx < 20)
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}	
	}
	else
	{
		return FALSE;
	}
}

//******************************************************************************


CCreditHelpper::CCreditHelpper(void)
: m_nCreditMoney(0), m_nMemberId(0)
{
}

CCreditHelpper::~CCreditHelpper(void)
{
}

void CCreditHelpper::SetNetId(CString newVal)
{
	m_strNetId = newVal;
}

void CCreditHelpper::SetCreditMoney(INT newVal)
{
	m_nCreditMoney = newVal;
}

/*************************************************
Function:       // Execute
Description:    // CheckCard������ȡ��MemberId�������û������ĳ�ֵ
Calls:          // CServletHellper::GetMemberId,ActivationMember,theApp.GetCurCashier()->DoCredit
Return:         // δע����˺ţ��������ֵ�������󣬼���ʧ�ܣ���ֵʧ�ܾ�����FALSE.��������TRUE
*************************************************/
BOOL CCreditHelpper::Execute()
{
	m_nMemberId = CServletHellper::GetMemberId(m_strNetId); //������ȡ��MemberId

	//δע����˺ţ�����FALSE
	if (m_nMemberId == 0)
	{
		CIBAString strTmp;
		strTmp.LoadString(IDS_HASNOTUSED); //���˺�δע�ᣡ
		strTmp.Insert(0, m_strNetId);
		theApp.IBAMsgBox(strTmp);

		return FALSE;
	}

	// 2011/06/29-gxx: �Ƿ���Գ�ֵ, �һ��в�������
	if (!CLocalServer::GetInstance()->ActiveMemberList.IsAllowCredit(m_nMemberId))
	{
		return FALSE;
	}

	//Ǯ����
	if (m_nCreditMoney <= 0)
	{
		theApp.IBAMsgBox(IDS_MONEYERROR); //�����뿪����
		return FALSE;
	}

	//����
	CServletHellper::ActivationMember(m_nMemberId);

	CCreditMethod Credit;

	Credit.SetMemberId(m_nMemberId);
	Credit.SetTermTime(CIBAHelpper::GetCurrentTimeAsString());
	Credit.SetActivityId(0);
	Credit.SetNoPresent(FALSE);
	Credit.SetAmount(m_nCreditMoney);

	theApp.GetCurCashier()->DoCredit(Credit);

	if (Credit.GetStatusCode() == 0)
	{	
		CIBATTS::GetInstance()->SpeakCreditResult(m_nCreditMoney, Credit.GetCreditPresent(), Credit.GetAvailableBalance());

		OnCreditOK(Credit);

		CString strTmp;
		strTmp.Format(IDS_CREDITRESULT2, m_strNetId, m_nCreditMoney / 100.0, Credit.GetCreditPresent() / 100.0); //%s�����γ�ֵ��%.2f Ԫ�� ���ͣ�%.2f Ԫ��
		theApp.IBAMsgBox(strTmp);
		return TRUE;
	}
	else
	{
		theApp.IBAMsgBox(Credit.GetStatusMessage());
	}

	return FALSE;
}

/*************************************************
Function:       // Execute
Description:    // �ӱ��ͨ����ȡ����(spaceǰΪ�˺�,��Ϊ���)��
				// �������� - ������ֵ(CheckCard������ȡ��MemberId�������û������ĳ�ֵ)
				// ����ָ�ģʽ�Ҳ�Ϊ�����û���������ֵ
				// ����Ͽ���Ϊ�����û��ģ����б��س�ֵ
Calls:          // CCreditHelpper::Execute, CCashierHelper::CreditLocalUser
Input:          // strCmdLine - ���ͨ�����룺��ʽΪNetId CreditAmount
Return:         // ��ֵ�ɹ�����TRUE,���򷵻�FALSE
*************************************************/

BOOL CCreditHelpper::Execute(CString strCmdLine)
{
	IBA_LOG(_T("���ͨ�����ٳ�ֵ"));
	CStringArray sa;
	
	CIBAHelpper::SplitLine(strCmdLine, sa, ' ');

	if (sa.GetCount() < 2)
	{
		theApp.IBAMsgBox(IDS_PARAMERROR);  //�Բ�����������������������룡
		return FALSE;
	}

	//�ո�ǰΪNetId����Ϊ����ֵ���
	CString strNetId = sa.GetAt(0);
	INT nCreditMoney = _tstof(sa.GetAt(1)) * 100;

	//2012/03/05-8238-gxx: 
	if (CIBAConfig::GetInstance()->GetMaxCreditMoney() != 0) //ÿ�ʳ�ֵ������Ϊ0��ʾû�����ƣ���IBA.ini������
	{
		INT nMax = CIBAConfig::GetInstance()->GetMaxCreditMoney()*100;
		if (nCreditMoney > nMax)
		{
			theApp.IBAMsgBox(IDS_OVERFLOW_MAXMONEY); //��ǰ�ĳ�ֵ��������̨���õ�����ֵ���
			return FALSE;
		}
	}
	// --end

	//�������������г�ֵ
	if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkNormal)
	{
		SetNetId(strNetId);
		SetCreditMoney(nCreditMoney);
		return Execute();
	}
	else 
	{
		CActiveMember ActivemMember;
		if (!CLocalServer::GetInstance()->ActiveMemberList.GetActiveMemberNetID(strNetId,ActivemMember))
		{
			return FALSE;
		}
		//����ָ�ģʽ�Ҳ�Ϊ�����û���������ֵ
		if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkRestore)
		{
			if (!ActivemMember.GetIsLocalUser())
			{
				SetNetId(strNetId);
				SetCreditMoney(nCreditMoney);
				return Execute();
			}
		}
		//����Ͽ���Ϊ�����û��ģ��Ž��б��س�ֵ.�˴����޸ģ���
		//else
		//{
		//	if (!ActivemMember.GetIsLocalUser())
		//	{
		//		return FALSE;
		//	}
		//}
		//����Ͽ������
		else
		{
			if (nCreditMoney <= 0)
			{
				return FALSE;
			}

			CString strErrMsg = CCashierHelper::DoCredit_InLocal(ActivemMember, nCreditMoney);
			if(strErrMsg.IsEmpty())
			{
				return TRUE;
			}
			return FALSE;
		}
	}
	
	return FALSE;	
}

void CCreditHelpper::OnCreditOK(CCreditMethod & Credit)
{
	CBalanceMethod Balance;
	Balance.SetMemberId(m_nMemberId);

	theApp.GetCurCashier()->DoBalance(Balance);

	if (Balance.GetStatusCode() == 0)
	{
		CString strTmp;

		if (Balance.GetOnline() == 1) //���ͳ�ֵ��Ϣ
		{			
			strTmp.Format(IDS_ONLINECREDIT, m_nCreditMoney / 100.0, Credit.GetCreditPresent() / 100.0);
			CLocalServer::GetInstance()->LocalSendShortMessageByMemberID(m_nMemberId, strTmp);
		}

		COperationInfo OperationInfo;
		OperationInfo.SetOperationType(1);
		OperationInfo.SetIsBundTime(FALSE);
		OperationInfo.SetUserName(Balance.GetUserName());
		OperationInfo.SetNetId(Balance.GetSerailNum());
		OperationInfo.SetMemberId(m_nMemberId);
		OperationInfo.SetOperationAmount(m_nCreditMoney);
		OperationInfo.SetPresentAmount(Credit.GetCreditPresent());
		SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM)(&OperationInfo));

		//д�����ݿ�
		CCreditInfoDB CreditInfo;

		CreditInfo.SetCardSerial(Balance.GetSerailNum());
		CreditInfo.SetCreditDate(CIBAHelpper::FormatCenterTime(Credit.GetTrantime()));
		CreditInfo.SetRefNo(Credit.GetRefNo());
		CreditInfo.SetCreditPresent(Credit.GetCreditPresent());
		CreditInfo.SetCreditAmount(m_nCreditMoney);
		CreditInfo.SetOperator(theApp.GetCurCashier()->GetName());
		CreditInfo.SetNetBarId(CNetBarConfig::GetInstance()->GetNetBarId());
		CreditInfo.SetCashRegisterID(CIBAConfig::GetInstance()->GetCashRegisterID());
		CreditInfo.SetMemberId(m_nMemberId);
		CreditInfo.SetSerialNo(Credit.GetLocalSerialNo());
		CreditInfo.SetCassId(Balance.GetUserClass());

		CIBADAL::GetInstance()->AddCreditRecord(CreditInfo);

	}
}

/*************************************************
Function:       //DeregisterUser
Description:    //��ʱ�û��˿�.
Return:			//�ɹ�����TRUE,ʧ�ܷ���FALSE
*************************************************/
BOOL CDeregisterHelpper::DeregisterUser(UINT nMemberId,CString& ErrMsg)
{
	ErrMsg.Empty();
	CActiveMember ActiveMember;
	if(FALSE == CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(nMemberId,ActiveMember))
	{
		ErrMsg.Format(_T("�����б��ȡ�û�ʧ��"));
		return FALSE;
	}
	//�û�����,֪ͨUDO�»�
	if(ActiveMember.GetIsOnLine())
	{
		INT_PTR nIndex = CLocalServer::GetInstance()->ComputerList.Find(nMemberId);
		if(nIndex >= 0)
		{
			if (nMemberId == ActiveMember.GetRoomMemberId()) //�����û�,nCheckoutType = 2
			{
				CLocalServer::GetInstance()->LocalCheckOutUser(nIndex, nMemberId, 2);
			}
			else												//�ǰ����û�,nCheckoutType = 0
			{
				CLocalServer::GetInstance()->LocalCheckOutUser(nIndex, nMemberId, 0);
			}
			Sleep(500);
		}
	}

	//��ȡCardId
	UINT nCardId;
	CIBADAL::GetInstance()->GetCardIdFromMember(ActiveMember.GetNetId(),nCardId);
	//��ʱ�û��˿�
	CDeregisterMethod Deregister;

	Deregister.SetMemberId(nMemberId);
	Deregister.SetPersonalId(ActiveMember.GetPersonalID());
	Deregister.SetMemberName(ActiveMember.GetUserName(FALSE));
	Deregister.SetCardId(nCardId);
	Deregister.SetPassword(ActiveMember.GetPassword());
	//Deregister.SetEndUsingTime(m_strLastLogoutTime);

	BOOL bRes = theApp.GetCurCashier()->DoDeregister(Deregister); 

	//�˿�ʧ��,���ش�����Ϣ
	if(FALSE == bRes)
	{
		if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
		{
			ErrMsg = LOAD_STRING(IDS_ERROR_DBOPERATION);//�����¾������ݿ����ʧ��
		}
		else
		{
			ErrMsg = Deregister.GetStatusMessage();
		}		
		return FALSE;
	}
	else//�ɹ�-���������־
	{
		UINT nAll = 0;
		//����ģʽ,�����˿�
		if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
		{
			UINT nDeposit = 0;
			CIBADAL::GetInstance()->GetDepositFromMember(ActiveMember.GetNetId(),nDeposit);
			nAll = ActiveMember.GetAvailavleBalance() + nDeposit;

			COperationInfo OperationInfo;
			OperationInfo.SetOperationType(CIBAGlobal::emOpTypeBrokenReturn);//�����˿�
			OperationInfo.SetUserName(ActiveMember.GetUserName(TRUE));
			OperationInfo.SetNetId(ActiveMember.GetNetId());
			OperationInfo.SetAllAmount(nAll);
			OperationInfo.SetOperationAmount(ActiveMember.GetAvailavleBalance());
			OperationInfo.SetDeposit(nDeposit);
			OperationInfo.SetDescription(ActiveMember.GetTerminalID());
			::SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM)&OperationInfo);
		}
		else//����ģʽ,�˿�
		{
			nAll = Deregister.GetBalance() + Deregister.GetGuarantyMoney();//�ϼ�
			COperationInfo OperationInfo;
			OperationInfo.SetOperationType(CIBAGlobal::emOpTypeReturn);//�˿�
			OperationInfo.SetIsBundTime(FALSE);
			OperationInfo.SetUserName(ActiveMember.GetUserName(TRUE));
			OperationInfo.SetNetId(ActiveMember.GetNetId());
			OperationInfo.SetMemberId(nMemberId);
			OperationInfo.SetOperationAmount(Deregister.GetBalance());
			OperationInfo.SetAllAmount(nAll);
			OperationInfo.SetDeposit(Deregister.GetGuarantyMoney());
			OperationInfo.SetOperationTime(ActiveMember.GetActivationTime());
			OperationInfo.SetDescription(ActiveMember.GetTerminalID());
			OperationInfo.SetClassId(ActiveMember.GetClassId());
			OperationInfo.SetTermId(ActiveMember.GetTerminalID());
			OperationInfo.SetIdNum(ActiveMember.GetPersonalID());
			OperationInfo.SetCheckInTime(ActiveMember.GetCheckInTime());
			OperationInfo.SetConsumeMoney(ActiveMember.GetAdvancePayment()-nAll);

			::SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM)&OperationInfo);
		}

		//����
		//CIBATTS::GetInstance()->SpeakReturnResult(nAll);                                  

		//�����ն���Ϣ(����û���Ϣ,ˢ����ͼ) 
		INT_PTR nIndex = CLocalServer::GetInstance()->ComputerList.Find(nMemberId);
		if (nIndex >= 0)                                                                      
		{
			CComputerInfo& computer = CLocalServer::GetInstance()->ComputerList.GetComputer(nIndex);
			computer.ClearUserInfo();
			CLocalServer::GetInstance()->ComputerList.UpdateComputer(nIndex, computer);
			CLocalServer::GetInstance()->GetComputerListView()->Refresh(FALSE);
		}
	}
	return TRUE;
}

//�Զ��˿��
/*************************************************
Function:       //AllowAutoRefund
Description:    //�Ƿ������Զ��˿�������Ϊ�������Զ��˿
				//��Ա/��̨���ò������˿�/��̨����ֻ������Ѻ���˻�����Ǯ/���ߵ�/��ʱ��/�¿��˺�δ���ѹ���/���»�ʱ�� < ����̨���õ��Զ��˿�ʱ��
Input:          //Balance - �û�Balance��Ϣ
Return:         //�����Զ��˿��true,���򷵻�false
*************************************************/
BOOL CDeregisterHelpper::AllowAutoRefund(CBalanceMethod & Balance)
{		
	//if (!CIBAConfig::GetInstance()->GetAutoRefundment()) //�Ƿ�������ˢ���Զ��˿�
	//{
	//	return FALSE;
	//}

	if (Balance.IsNetbarMember()) //��Ա�û��Լ���̨���ò������˿�������˿�
	{
		return FALSE;
	}

	if (Balance.GetAllowReturn() == 1) //�����˿� - ��̨����0:������ 1:���� 2:ֻ��Ѻ��
	{
		if (Balance.GetReturnAmount() > 0) //Ӧ�������ķ��ض���0.
		{
			return FALSE;
		}
	}
	else if (Balance.GetAllowReturn() == 2) //ֻ��Ѻ��
	{
		if (Balance.GetTotalBalacne() > 0) //�ϼ��˻�����Ǯ��������
		{
			return FALSE;
		}
	}
	else //�������˿�
	{
		return FALSE;
	}

	//if (Balance.GetGuarantyMoney() > 0) //��Ѻ���
	//{
	//	return FALSE;
	//}

	if (Balance.GetOnline() == 1) //���� 
	{
		return FALSE;
	}
	
	if (Balance.GetPayType() > 0) //��ʱ
	{
		return FALSE;
	}

	CString logoutTime = Balance.GetLogoutTime();
	if (logoutTime.GetLength() > 8 &&  logoutTime.Left(8).CompareNoCase(_T("19700101")) == 0)//�¿����˺Ų���
	{
		return FALSE;
	}
	//if (Balance.GetLogoutTime().CollateNoCase(_T("19700101080000")) == 0 || 
	//	Balance.GetLogoutTime().CollateNoCase(_T("19700101070000")) == 0) //�¿����˺Ų���
	//{
	//	return FALSE;
	//}

	COleDateTime dtLogoutTime = CIBAHelpper::CenterTimeToOleDateTime(Balance.GetLogoutTime());
	
	COleDateTime dtNow = COleDateTime::GetCurrentTime();
	
	COleDateTimeSpan dtSpan = dtNow - dtLogoutTime; //�»������ڶ೤ʱ����

	if (dtSpan.GetTotalMinutes() < CActiveMember::GetRefundmentTime())//���»�ʱ�� < ����̨���õ��Զ��˿�ʱ��
	{
		return FALSE;
	}
	
	return TRUE;
}
/*************************************************
Function:       //AllowAutoRefundInLocalMode
Description:    //�Ƿ������Զ��˿�������Ϊ�������Զ��˿
				//��Ա/��̨���ò������˿�/��̨����ֻ������Ѻ���˻�����Ǯ/���ߵ�/��ʱ��/�¿��˺�δ���ѹ���/���»�ʱ�� < ����̨���õ��Զ��˿�ʱ��
Input:          //Balance - �û�Balance��Ϣ
Return:         //�����Զ��˿��true,���򷵻�false
*************************************************/
BOOL CDeregisterHelpper::AllowAutoRefundInLocalMode(CActiveMember& ActiveMember, int nParaAllowReturn, COleDateTime checkouttime)
{		


	if (nParaAllowReturn == 2) //ֻ��Ѻ��
	{
		if (ActiveMember.GetAvailavleBalance() > 0) //�ϼ��˻�����Ǯ��������
		{
			return FALSE;
		}
	}

	if (ActiveMember.GetIsOnLine() == 1) //���� 
	{
		return FALSE;
	}
	
	if (ActiveMember.GetPayType() > 0) //��ʱ
	{
		return FALSE;
	}

	if(CIBAHelpper::IsValidTime(checkouttime) && checkouttime > COleDateTime(1970,1,1,0,0,0))
	{
		CString logoutTime = checkouttime.Format(_T("%y%m%d%H%M%S"));
		if (logoutTime.GetLength() > 8 &&  logoutTime.Left(8).CompareNoCase(_T("19700101")) == 0)//�¿����˺Ų���
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
	
	COleDateTime dtNow = COleDateTime::GetCurrentTime();
	
	COleDateTimeSpan dtSpan = dtNow - checkouttime; //�»������ڶ೤ʱ����

	if (dtSpan.GetTotalMinutes() < CActiveMember::GetRefundmentTime())//���»�ʱ�� < ����̨���õ��Զ��˿�ʱ��
	{
		return FALSE;
	}
	
	return TRUE;
}
/*************************************************
Function:       //SendRegDataToProxy
Description:    //��ʵ���ϱ��û���Ϣ����RegisterDlg::SendRegDataToProxy
Calls:          //auditInterface.regUser_
Table Accessed: //None
Table Updated:  //None
Input:          //nMemberId - �û���
Output:         //None
Return:         //None
Others:         //None
*************************************************/
void CServletHellper::SendRegDataToProxy(UINT nMemberId)
{
	if(!PathFileExists(theApp.GetWorkPath() + _T("\\IBABin\\AuditInterface.dll")))
	{
		return ;
	}
	//�ӱ��ط������в����û���Ϣ
	CString sql;
	sql.Format(_T("select * from member where memberId= %d"), nMemberId);

	CString strSex;
	CString strNation; 
	int nIdType;
	int nUserClassID;
	CString strPersonalID; 
	CString strName;
	CString strAddr;
	CString strTelNum;
	CString strOfficer;
	CString strNetId;

	GxxDictionaryPtr memberPtr = CIBADAL:: GetInstance ()->ReadRecordFromDB (sql);
	if(memberPtr != NULL)
	{
		strSex = CString((memberPtr->valueForKey("sex")->stringValue()));
		strNation = IBAGlobal::StringValueForKey("job",memberPtr);
		nIdType = memberPtr->valueForKey("idType")->intValue();
		nUserClassID = memberPtr->valueForKey("classId")->intValue();
		strPersonalID = IBAGlobal::StringValueForKey("idNumber",memberPtr); 
		strName = IBAGlobal::StringValueForKey("name",memberPtr);
		strAddr = IBAGlobal::StringValueForKey("address",memberPtr);
		strTelNum = IBAGlobal::StringValueForKey("tel",memberPtr);
		strOfficer = IBAGlobal::StringValueForKey("department",memberPtr);
		strNetId = IBAGlobal::StringValueForKey("serialNum",memberPtr);
	}
	else
	{
		IBA_LOG(_T("CServletHellper::SendRegDataToProxy - No Record found in member in LocalServer for MemberId = %d, Can not send info to RealName after activation!"),nMemberId);
		return;
	}


	CAuditInterface auditInterface;
	if(!auditInterface.Load())
	{
		IBA_LOG0(_T("����ʵ���ӿڶ�̬��AuditInterface.dllʧ�ܣ�����AuditInterface.dll�İ汾��"));
		return ;
	}
	auditInterface.uninit_();
	CStringA strIpA = CT2A(CNetBarConfig::GetInstance()->GetProxyIP());
	auditInterface.init_(strIpA, 7834);
	// �Ա�
	//m_cboSex.GetLBText(m_cboSex.GetCurSel(), strSex);

	int isex;
	if (strSex == _T("0")) isex = 1;else isex  = 0;
	int nNation = NS_REALNAME::CRZXRealName::GetNationCode(strNation);
	UINT nNetbarId = CNetBarConfig::GetInstance()->GetNetBarId();
	COleDateTime dt;
	dt = COleDateTime::GetCurrentTime();
	CString strDatetime;
	strDatetime = dt.Format(_T("%Y-%m-%d"));

	CString validDate("2050-01-01");
	CString strBrithday = NS_REALNAME::CRZXRealName::GetBirthdayFromSFZ(strPersonalID);

	CStringA strPersonalIDA = CT2A(strPersonalID);
	CStringA strBrithdayA = CT2A(strBrithday);
	CStringA strDatetimeA = CT2A(strDatetime);
	CStringA strValidDateA = CT2A(validDate);

	CStringA strNameA = CT2A(strName);
	CStringA strAddrA = CT2A(strAddr);
	CStringA strTelNumA = CT2A(strTelNum);
	CStringA strOfficerA = CT2A(strOfficer);
	CStringA strNetIdA = CT2A(strNetId);

	int retcode = -1;
	try
	{
	int ret = auditInterface.regUser_(nNetbarId, nIdType, strPersonalIDA, strNameA, 
		isex, "CN", strAddrA, strTelNumA, nMemberId, strBrithdayA,
		strDatetimeA, strValidDateA, nNation, strOfficerA, nUserClassID, strNetIdA, &retcode);

	if (ret == 0)
	{
		if (retcode == 1 || retcode == -6 )
		{
			IBA_LOG0(_T("��ʵ���ӿڴ�������Ϣ�ɹ���������ʵ����!"));
		}
	}
	else if (ret == ERROR_NETWORK_FAILED)
	{
		IBA_LOG0(_T("�������,�ϴ��û���Ϣʧ��!"));
	}
	else
	{
		IBA_LOG0(_T("�����쳣!"));
	}
	}
	catch(...)
	{
		IBA_LOG(_T("��ʵ�������׳��쳣:%d"), GetLastError());
	}

}