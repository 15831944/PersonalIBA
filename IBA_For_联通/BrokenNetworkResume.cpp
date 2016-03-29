#include "stdafx.h"
#include "IBA.h"
#include "Cashier.h"
#include "dal\IBADAL.h"
#include "RealName\IBARealName.h"
#include "MainFrm.h"
#include "IBAHelpper.h"
#include ".\brokennetworkresume.h"


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
	m_bShouldResume = bResume;
}

BOOL CBrokenNetworkResume::GetShouldResume()
{
	return m_bShouldResume;
}

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


void CBrokenNetworkResume::StartResume(INT nConnectionState)
{
	if (nConnectionState == -1)
		return;

	if (!m_bShouldResume)
		return;

	CIBALog::GetInstance()->Write(_T("CBrokenNetworkResume::StartResume"));

	m_bShouldResume = FALSE;

	Start();
}


INT CBrokenNetworkResume::Run()
{
	CoInitialize(NULL);

	CSingleLock lock(&m_TestUploadLock, TRUE);
	//
	// �������ύ��������
	//

	//��ѯ���ݿ�
	CCheckOutInfoArray CheckOutArray;

	CIBADAL::GetInstance()->GetCheckOutInfo(CheckOutArray);

	INT i = 0;

	for (i = 0; i < CheckOutArray.GetCount(); i++)
	{
		if (!theApp.GetCurCashier()->IsOnDuty())
		{
			theApp.IBAMsgBox(IDS_STRING326);//ֻ�����ϰ�״̬���ж����ָ�
			CoUninitialize();
			return 0;
		}

		CCheckOutinfo & CheckOutinfo = CheckOutArray.GetAt(i);
		CIBALog::GetInstance()->WriteFormat(_T("�����������˵��û� %s "),CheckOutinfo.GetSerialNum());
		//ͳһִ�еǳ�
		CCheckout2Method Checkout2;
		Checkout2.SetSafeLogout(0); //�ǳ���Ϊ1���ǳ�
		Checkout2.SetMemberId(CheckOutinfo.GetMemberId());

		COleDateTime OleCheckOutTime;
		OleCheckOutTime.ParseDateTime(CheckOutinfo.GetCheckOutTime());
		Checkout2.SetEndUsingTime(OleCheckOutTime.Format(_T("%Y%m%d%H%M%S")));
		
		theApp.GetCurCashier()->DoCheckout2(Checkout2);		

		int nStatus = Checkout2.GetStatusCode();

		if (CheckOutinfo.GetClassState() == 0 && CheckOutinfo.GetReturnState() == 1)//��ͨ�û�
		{
			SetShouldResume(FALSE); // ��ֹ�ݹ�ʽ�Ļָ�
			DeregisterUser(CheckOutinfo);
		}
		else
		{
			//д�˿���Ϣ��־
			COperationInfo OperationInfo;
			OperationInfo.SetOperationType(CIBAGlobal::emOpTypeVipReturn);
			OperationInfo.SetIsBundTime(FALSE);
			OperationInfo.SetUserName(CheckOutinfo.GetUserName());
			OperationInfo.SetNetId(CheckOutinfo.GetSerialNum());
			OperationInfo.SetMemberId(CheckOutinfo.GetMemberId());
			OperationInfo.SetDescription(_T("����������"));
			OperationInfo.SetIsResumeData(TRUE);
			OperationInfo.SetClassId(CheckOutinfo.GetClassId());

			//����ȴ�ֱ�����أ�������������̲߳���ȫ
			SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM)(&OperationInfo));
		}

		

		Sleep(50);
	}
	
	IBA_TRACE("============================i=%d",i);

	if (i >= CheckOutArray.GetCount())
	{
		//������ݿ�
		CIBADAL::GetInstance()->DeleteAllCheckOutInfo();
	}

	CoUninitialize();

	return 0;
}


BOOL CBrokenNetworkResume::DeregisterUser(CCheckOutinfo& CheckOutinfo)
{
	//���鿨��
	CIDCheckCardMethod IDCheckCard;
	IDCheckCard.SetSerialNum(CheckOutinfo.GetSerialNum());
	theApp.GetCurCashier()->DoCheckCard(IDCheckCard);

	if (IDCheckCard.GetStatusCode() != 0)
	{
		return FALSE;
	}

	//ע������
	CDeregisterMethod Deregister;

	Deregister.SetMemberId(IDCheckCard.GetMemberId());
	Deregister.SetPersonalId(CheckOutinfo.GetPersonalId());
	Deregister.SetMemberName(CheckOutinfo.GetClassName());
	Deregister.SetCardId(IDCheckCard.GetCardId());
	Deregister.SetPassword(_T("PASSWORDPASSWORD"));
	Deregister.SetReturnMode(0);

	theApp.GetCurCashier()->DoDeregister(Deregister);

	if (Deregister.GetStatusCode() == 0)
	{	
		UINT nAll = Deregister.GetBalance() + Deregister.GetGuarantyMoney();//�ϼ�

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
		//����ȴ�ֱ�����أ�������������̲߳���ȫ
		SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM)(&OperationInfo));

		//������Ϣд�����ݿ�
		{//д���ݿ�
			CReturnedInfo ReturnedInfo;

			ReturnedInfo.SetCardId(IDCheckCard.GetCardId());
			ReturnedInfo.SetReturnDate(CIBAHelpper::FormatCenterTime(Deregister.GetTranTime()));
			ReturnedInfo.SetRefNo(Deregister.GetRefNo());
			ReturnedInfo.SetDeposit(Deregister.GetGuarantyMoney());
			ReturnedInfo.SetOperator(theApp.GetCurCashier()->GetName());
			ReturnedInfo.SetNetBarId(CNetBarConfig::GetInstance()->GetNetBarId());
			ReturnedInfo.SetCashRegisterID(CIBAConfig::GetInstance()->GetCashRegisterID());
			ReturnedInfo.SetMemberId(CheckOutinfo.GetMemberId());
			ReturnedInfo.SetSerialNo(Deregister.GetLocalSerialNo());
			ReturnedInfo.SetReturnAmount(nAll);
			ReturnedInfo.SetClassId(1);
			CIBADAL::GetInstance()->AddReturnedRecord(ReturnedInfo);
		}

		//ʵ������ʱ��
		if (CNetBarConfig::GetInstance()->GetReturnOnlyClassId() != CheckOutinfo.GetClassId())
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
