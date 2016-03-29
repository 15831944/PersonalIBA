#include "stdafx.h"
#include "IBA.h"
#include "Cashier.h"
#include "dal\IBADAL.h"
#include "RealName\IBARealName.h"
#include "MainFrm.h"
#include "IBAHelpper.h"
#include ".\brokennetworkresume.h"
#include "Socket\LocalServer.h"
#include "ComputerList.h"
#include "ComputerListView.h"
//#include "CommandProcessor.h"


CBrokenNetworkResume::CBrokenNetworkResume(void)
{
	m_bShouldResume = FALSE;
}

CBrokenNetworkResume::~CBrokenNetworkResume(void)
{
}

BOOL CBrokenNetworkResume::Init()
{
	return (m_bShouldResume = CheckShouldResume());
}

void CBrokenNetworkResume::SetShouldResume(BOOL bResume)
{
	IBA_LOG(_T("m_bShouldResume => %d"),(int)bResume);
	m_bShouldResume = bResume;
}

BOOL CBrokenNetworkResume::GetShouldResume()
{
	return m_bShouldResume;
}

/*************************************************
Function:       // CheckShouldResume
Description:    // ���CheckOutInfo�����Ƿ��м�¼
Calls:          // CIBADAL::GetCheckOutInfo(CCheckOutInfoArray& CheckOutArray)
Table Accessed: // CheckOutInfo
Table Updated: // None
Input:          // None
Output:         // None
Return:         // �����м�¼������TRUE���޼�¼������FALSE
Others:         // ����˵��
*************************************************/
BOOL CBrokenNetworkResume::CheckShouldResume()
{
	BOOL bRet = FALSE;

	CCheckOutInfoArray CheckOutArray;
	
	CIBADAL::GetInstance()->GetCheckOutInfo(CheckOutArray);
	
	if (CheckOutArray.GetCount() > 0)
	{
		bRet = TRUE;
	}
	
	return bRet;
}

/*************************************************
Function:       // StartResume
Description:    // �������ܡ����ܵȵ�����
Calls:          //  CBrokenNetworkResume::Run
*************************************************/
void CBrokenNetworkResume::StartResume(INT nConnectionState)
{
	if (nConnectionState == -1)
		return;

	if (!m_bShouldResume)
		return;	

	m_bShouldResume = FALSE;
	IBA_LOG(_T("m_bShouldResume => FALSE"));

	Start();
}

/*************************************************
Function:       //Run
Description:    //Ϊ΢���»�ָ��ϱ���Ա�»�(ʵ���»�,ˢ���û���Ϣ,�û���ͼ,�ն���Ϣ,������ͼ)
Table Updated:  //CheckOutInfo
Return:         //0
Others:         //����ģʽ��,����ԱCheckin,DoOnduty,DoRegister,DoCredit���緢��CheckOutInfo�����м�¼�������
*************************************************/
INT CBrokenNetworkResume::Run()
{
	IBA_LOG(_T("Start thread:CBrokenNetworkResume"));

	CoInitialize(NULL);

	CSingleLock lock(&m_TestUploadLock, TRUE);   //�����ٽ���Դm_TestUploadLock

	//δ�ϰ�or������ģʽ,�˳�
	if (!theApp.GetCurCashier()->IsOnDuty() || CIBAGlobal::NetworkStatus != CIBAGlobal::emNetworkNormal)
	{
		theApp.IBAMsgBox(IDS_STRING326);
		CoUninitialize();
		return 0;
	}

	//��ȡCheckOutInfo���м�¼
	CCheckOutInfoArray CheckOutArray;

	if(FALSE==CIBADAL::GetInstance()->GetCheckOutInfo(CheckOutArray))
	{
		IBA_LOG(_T("Abnormal.��ѯCCheckoutInfo�����"));
		GXX_ASSERT(0,"��ѯCCheckoutInfo�����");
		CoUninitialize();
		return 0;
	}

	//����CheckOutInfo���м�¼
	INT i = 0;
	for (i = 0; i < CheckOutArray.GetCount(); i++)
	{
		CCheckOutinfo & CheckOutinfo = CheckOutArray.GetAt(i);

		//����ʱ�û�������,ȡ������
		if(CheckOutinfo.GetTerminalId().IsEmpty())
		{
			CActivationMethod Activation;
			Activation.SetMemberId(CheckOutinfo.GetMemberId());
			Activation.SetMode(CActivationMethod::ACTIVATION_NO);
			theApp.GetCurCashier()->DoActivation(Activation);
		}
		else//����,ִ��Checkout2
		{
			CCheckout2Method Checkout2;
			Checkout2.SetNetBarId(CNetBarConfig::GetInstance()->GetNetBarId());
			Checkout2.SetMemberId(CheckOutinfo.GetMemberId());
			Checkout2.SetSafeLogout(0); 
			Checkout2.SetEndUsingTime(CheckOutinfo.GetCheckOutTime());
			Checkout2.SetOperator(theApp.GetCurCashier()->GetName());

			BOOL bRes = Checkout2.ExecuteCheckout2();
			CString strOperationLogMsg;
			if(bRes)
			{
				strOperationLogMsg.Format(_T("�ϱ��ɹ�"));
			}
			else
			{
				strOperationLogMsg.Format(_T("�ϱ�ʧ�ܣ� Error: %s"),Checkout2.GetStatusMessage());
				//continue;
			}

			//д������־
			COperationInfo OperationInfo;
			OperationInfo.SetOperationType(CIBAGlobal::emOpTypeMakeupLocalReturnVip);
			OperationInfo.SetUserName(CheckOutinfo.GetUserName());
			OperationInfo.SetNetId(CheckOutinfo.GetSerialNum());
			OperationInfo.SetOperationAmount(CheckOutinfo.GetBalanceMoney());
			OperationInfo.SetDescription(strOperationLogMsg);
			::SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM)(&OperationInfo));	

			INT_PTR nIndex = CLocalServer::GetInstance()->ComputerList.Find(Checkout2.GetMemberId());
			if (nIndex >= 0)
			{
				//ʵ���»�
				CActiveMember ActiveMember;
				CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(CheckOutinfo.GetMemberId(),ActiveMember);
				CComputerInfo& computer = CLocalServer::GetInstance()->ComputerList.GetComputer(nIndex);
				theApp.GetRealName()->AuditUserCheckOut(computer.GetTerminalID(), computer.GetComputerIP(), Checkout2.GetMemberId());
				//�Ƕ�ʵ��
				if(CIBAGlobal::emRealnameZD == CNetBarConfig::GetInstance()->GetAuditType())
				{
					NS_REALNAME::CheckoutInfo coi;
					ZeroMemory(&coi, sizeof(coi));
					CString strName,strSex;
					strSex = ActiveMember.GetSex()?_T("Ů"):_T("��");
					strName.Format(_T("%s(%s)"),ActiveMember.GetUserName(),strSex);
					_tcsncpy(coi.szName, strName, 100);
					_tcsncpy(coi.szNetId, ActiveMember.GetNetId(), 100);
					_tcsncpy(coi.szTermId, ActiveMember.GetTerminalID(), 50);

					CString strIP;
					CIBADAL::GetInstance()->GetIPFromTermId(coi.szTermId, strIP);
					_tcsncpy(coi.szIP, strIP, 50);
					coi.nCheckoutType = CIBAGlobal::cot_MemLogout;

					theApp.GetRealName()->CheckOut(&coi);
				}

				//�����ն�״̬Ϊ����,ˢ���ն���ͼ
			/*	CComputerInfo& computer = CLocalServer::GetInstance()->ComputerList.GetComputer(nIndex);*/
				computer.SetComputerStatus(CComputerInfo::ECS_LOCKED);
				computer.ClearUserInfo();
				CLocalServer::GetInstance()->ComputerList.UpdateComputer(nIndex, computer);
				CLocalServer::GetInstance()->GetComputerListView()->Refresh(FALSE);
			}
		}

		//�����б�ɾ���û�,�����û���ͼ
		CLocalServer::GetInstance()->ActiveMemberList.RemoveActiveMember(CheckOutinfo.GetMemberId());
		CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(TRUE);
	}
	Sleep(50);

	//ɾ���������м�¼
	CIBADAL::GetInstance()->DeleteAllCheckOutInfo();
	
	CoUninitialize();
	return 0;
}

/*************************************************
Function:       // DeregisterUser
Description:    // У�鿨��IDCheckCard��������ע��Deregister���Ƕ�ʵ���»�����LastUser,Returned,Member���и��¼�¼������ʵ���ӿڵĽ��в���
Calls:          // DoCheckCard��DoDeregister��theApp.GetRealName()->CheckOut��AddReturnedRecord��theApp.GetRealName()->InvokeRealName
Table Accessed: // None
Table Updated: // LastUser,Returned,Member
Input:          // CheckOutInfo-��CheckOutInfo���ж�ȡ���ļ�¼
Output:         // None
Return:         // DoDeregister�ɹ����򷵻�TRUE�����򷵻�FALSE
Others:         // None
*************************************************/
BOOL CBrokenNetworkResume::DeregisterUser(CCheckOutinfo& CheckOutinfo)
{
	IBA_LOG(_T("CBrokenNetworkResume::DeregisterUser Start - SerialNum = %d"),CheckOutinfo.GetSerialNum());
	//�����ļ��鿨��
	CIDCheckCardMethod IDCheckCard;
	IDCheckCard.SetSerialNum(CheckOutinfo.GetSerialNum());
	theApp.GetCurCashier()->DoCheckCard(IDCheckCard);

	if (IDCheckCard.GetStatusCode() != 0)
	{
		IBA_LOG(_T("CBrokenNetworkResume::DeregisterUser Result - FALSE , DoCheckCard Failed!"));
		return FALSE;
	}

	//ע������
	CDeregisterMethod Deregister;

	Deregister.SetMemberId(IDCheckCard.GetMemberId());
	Deregister.SetPersonalId(CheckOutinfo.GetPersonalId());
	Deregister.SetMemberName(CheckOutinfo.GetClassName());
	Deregister.SetCardId(IDCheckCard.GetCardId());
	Deregister.SetPassword(_T("PASSWORDPASSWORD"));
	Deregister.SetReturnMode(0);                      //0��ʾ�����˿�

	theApp.GetCurCashier()->DoDeregister(Deregister); //ִ��ע����ʵ���»�����

	if (Deregister.GetStatusCode() == 0)
	{	
		UINT nAll = Deregister.GetBalance() + Deregister.GetGuarantyMoney();//�ϼ� Balance��Ӧ�˳�ֵ�ʻ����,GuarantyMoney��Ӧ��Ѻ��

		CString strNetId;
		strNetId.Format(_T("%s"), CheckOutinfo.GetSerialNum());

		//д�˿���Ϣ��־
		COperationInfo OperationInfo;
		OperationInfo.SetOperationType(CIBAGlobal::emOpTypeReturn);
		OperationInfo.SetIsBundTime(FALSE);
		OperationInfo.SetUserName(CheckOutinfo.GetUserName());
		OperationInfo.SetNetId(CheckOutinfo.GetSerialNum());
		OperationInfo.SetMemberId(CheckOutinfo.GetMemberId());
		OperationInfo.SetOperationAmount(Deregister.GetBalance());
		OperationInfo.SetAllAmount(nAll);
		OperationInfo.SetDeposit(Deregister.GetGuarantyMoney());
		OperationInfo.SetDescription(_T("����������"));
		
		// ������Ƕ�ʵ��������ʵ���»�
		if(CIBAGlobal::emRealnameZD == CNetBarConfig::GetInstance()->GetAuditType())
		{
			NS_REALNAME::CheckoutInfo coi;
			ZeroMemory(&coi, sizeof(coi));
			_tcsncpy(coi.szName, CheckOutinfo.GetUserName(), 100);
			_tcsncpy(coi.szNetId, CheckOutinfo.GetSerialNum(), 100);
			_tcsncpy(coi.szTermId, CheckOutinfo.GetTerminalId(), 50);

			CString strIP;
			CIBADAL::GetInstance()->GetIPFromTermId(coi.szTermId, strIP);
			_tcsncpy(coi.szIP, strIP, 50);
			coi.nCheckoutType = CIBAGlobal::cot_TempLogout;

			theApp.GetRealName()->CheckOut(&coi);
		}

		//����ȴ�ֱ�����أ�������������̲߳���ȫ
		SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM)(&OperationInfo));

		//������Ϣд�����ݿ�
		{//д���ݿ�
			CReturnedInfo ReturnedInfo;

			ReturnedInfo.SetCardId(IDCheckCard.GetCardId());                                     //�˺�
			ReturnedInfo.SetReturnDate(CIBAHelpper::FormatCenterTime(Deregister.GetTranTime())); //�˿�ʱ��
			ReturnedInfo.SetRefNo(Deregister.GetRefNo());                                        
			ReturnedInfo.SetDeposit(Deregister.GetGuarantyMoney());                              //Ѻ��
			ReturnedInfo.SetOperator(theApp.GetCurCashier()->GetName());                         //����Ա����
			ReturnedInfo.SetNetBarId(CNetBarConfig::GetInstance()->GetNetBarId());               //����ID
			ReturnedInfo.SetCashRegisterID(CIBAConfig::GetInstance()->GetCashRegisterID());      //����̨ID
			ReturnedInfo.SetMemberId(CheckOutinfo.GetMemberId());                                //��Ա��
			ReturnedInfo.SetSerialNo(Deregister.GetLocalSerialNo());                             //������ˮ��
			ReturnedInfo.SetReturnAmount(nAll);                                                  //Ӧ���ܽ��
			ReturnedInfo.SetClassId(1);                                                          //�û�����Ϊ1������Ա����
			CIBADAL::GetInstance()->AddReturnedRecord(ReturnedInfo);  //�����˿���˻��������ݱ�ĸ���,����LastUser, Returned, Member��
		}

		//����ʵ���ӿڵĲ��������˿�»�����ֵ��
		if (CNetBarConfig::GetInstance()->GetReturnOnlyClassId() != CheckOutinfo.GetClassId()) //����������ֻ�˿�-��ֵ������̨����ʱͨ��CheckIn�����Ľ��յ�����ֵ
		{
			theApp.GetRealName()->InvokeRealName(100, (LPVOID)strNetId.GetString());
		}

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
