#include "stdafx.h"
#include ".\cashierhelper.h"
#include "Dal\IBADAL.h"
#include "OperationInfo.h"
#include "Dal\IDTypeInfo.h"
#include "ActiveMember.h"
#include "IBA.h"
#include "MainFrm.h"
#include "Cashier.h"
#include "Socket\LocalServer.h"
#include "ActiveMemberList.h"
#include "IBAHelpper.h"
#include "ComputerListView.h"
#include "Tts\IBATTS.h"
#include "DdnDataX.h"
#include "Encrypt.h"
#include "DdnPcpInc.h"
#include "LocalCenter\gxx_base.h"
#include "GlobalFunctions.h"
#include "ServletHellper.h"
#include "RealName\IBARealName.h"
#include "RealName\AuditInterface.h"
#include "RealName\RZXRealName.h"
#include <afxmt.h>
#include "BrokenNetwork.h"
#include "LocalCenter\GxxString.h"
#include "LocalCenter\gxx_base.h"
#include "LocalCenter\G2XCommonTool.h"
#include "LocalMember.h"
#include "NetBarConfig.h"

extern CCriticalSection g_csLocalCreditLock;

using namespace IBAGlobal;

CCashierHelper::CCashierHelper(void)
{
}

CCashierHelper::~CCashierHelper(void)
{
}

/*************************************************
Function:       //MakeLocalConsumeInfo
Description:    //���ݲ���ActiveMember�����ݼ��㱾�����ѽ����˿�����»�ʱ�䣬MD5���ܺ���䵽CLocalConsumeInfo��Ҫע��LocalExpense���㷨��
Calls:          //GetLocalExpense,
Table Accessed: //None
Table Updated:  //None
Input:          //ActiveMember - �������consumeInfo
Output:         //consumeInfo - ���õ�localsonsumeInfo
Return:         //None
Others:         //Note:GetLocalExpenseʱ��1.Ĭ�Ͻ���0.5Ԫ����ȡ�� 2.���ѽ�����ǰСʱ,�ͼ���Ϊ��ǰСʱ
*************************************************/
void CCashierHelper::MakeLocalConsumeInfo(CActiveMember& ActiveMember, NS_DAL::CLocalConsumeInfo& consumeInfo)
{
	// ��ȡ�������ѽ��, �����˿���
	//��ȡ�������ѽ�� - bFilter�����Ƿ����0.5Ԫ������ȡ�����ˣ�Ĭ��Ϊ�ǡ������������or��½ʱ������ֱ�ӷ���ActiveMember.GetAmout������ͨ�����ʽ��м��㡣
	//����ʱ��������ѽ�����ǰСʱ���ͼ���Ϊ��ǰСʱʣ������ʱ�䡣
	UINT nExpense = ActiveMember.GetLocalExpense();   
	UINT nReturn = 0; 
	if (nExpense > (UINT)ActiveMember.GetAdvancePayment())
	{
		nReturn = 0;
		nExpense = (UINT)ActiveMember.GetAdvancePayment();
	}
	else
	{
		nReturn = ActiveMember.GetAdvancePayment() - nExpense;
	}

	//����SubmitTimeΪ��ǰʱ�䣬RefNoΪMemberID
	consumeInfo.SetSubmitTime( CIBAHelpper::FormatTime(COleDateTime::GetCurrentTime()) );
	consumeInfo.SetRefNo( ActiveMember.GetMemberID() );

	//����CheckinTime��CheckoutTime����ActiveMember��checkinʱ����Ч��������Ϊ��ǰʱ�䣬����ֵ���ã����»�ʱ����Ϊ��ǰ��û���Ϲ��������»�ʱ�䶼����Ϊ��ǰʱ�䡣
	if (!ActiveMember.GetTerminalID().IsEmpty())   
	{
		if (!CIBAHelpper::IsValidTime(ActiveMember.GetCheckInTime()))
		{
			// �ϻ�ʱ�䲻������ʹ�õ�ǰʱ��
			consumeInfo.SetCheckinTime( CIBAHelpper::FormatTime( COleDateTime::GetCurrentTime() ));
			consumeInfo.SetCheckoutTime( consumeInfo.GetCheckinTime() );
		}
		else
		{
			consumeInfo.SetCheckinTime( CIBAHelpper::FormatTime( ActiveMember.GetCheckInTime()) );
			consumeInfo.SetCheckoutTime( CIBAHelpper::FormatTime( COleDateTime::GetCurrentTime() ) );
		}	
	}
	else
	{
		// û���Ϲ������ϻ�ʱ����»��ͱ���һ��
		consumeInfo.SetCheckinTime( CIBAHelpper::FormatTime( ActiveMember.GetActivationTime()) );
		consumeInfo.SetCheckoutTime( CIBAHelpper::FormatTime( ActiveMember.GetActivationTime()) );
	}

	//��䲢ͨ��MD5����
	consumeInfo.SetConsumeAmount( nExpense );
	consumeInfo.SetCreditAmount( ActiveMember.GetAdvancePayment() );
	consumeInfo.SetReturnAmount( nReturn );
	consumeInfo.SetIdNumber( ActiveMember.GetPersonalID() );
	consumeInfo.SetMemberId( ActiveMember.GetMemberID() );
	consumeInfo.SetName( ActiveMember.GetUserName(TRUE) );
	consumeInfo.SetOperator( theApp.GetCurCashier()->GetName() );		
	consumeInfo.SetSerialNum( ActiveMember.GetNetId() );	
	consumeInfo.SetTermId( ActiveMember.GetTerminalID() );
	consumeInfo.SetClassId( ActiveMember.GetClassId() );
	consumeInfo.SetPCClass( ActiveMember.GetPCClassID() );
	consumeInfo.SetPayType( ActiveMember.GetPayType() );
	consumeInfo.MakeMD5();
}

/*************************************************
Function:       //ReturnLocalUser
Description:    //���û�д���CLocalConsumeInfo�������ɱ������Ѽ�¼�������������ѽ��ȡ�
				//���������Ѽ�¼���浽LocalConsume����
				//֪ͨ�ͻ����»�����������ActiveMemberMap�������û���ͼ --- �������ն���ͼ����
Calls:          //MakeLocalConsumeInfo,
Table Accessed: //None
Table Updated:  //LocalConsume
Input:          //ActiveMember - ��������LocalConsumeInfo
                //lpLocalInfo - ���ڽ���Ϣ���ݸ��նˣ����������»�
Output:         //None
Return:         //���˳ɹ�����TRUE.���򷵻�FALSE
Others:         //ֻ���ڱ���ע���û�ʹ�ã�����
*************************************************/
//BOOL CCashierHelper::ReturnLocalUser(CActiveMember& ActiveMember,NS_DAL::CLocalConsumeInfo* lpLocalInfo)
//{
//	IBA_ASSERT2(ActiveMember.GetIsLocalUser(), "����Ϊ�����û�����ִ�д˲���"); //ֻ�б���ע���û�����ʹ�øú�����IBA_ASSERT2��������release����ֹ���У�����
//	if(!ActiveMember.GetIsLocalUser())
//	{
//		AfxMessageBox(_T("���Ǳ���ע���û�������ʧ�ܣ�"));
//		return FALSE;
//	}
//
//	CLocalConsumeInfo consumeInfoTmp;
//	if (lpLocalInfo == NULL)
//	{
//		//���ݲ���ActiveMember�����ݼ��㱾�����ѽ����˿�����»�ʱ�䣬MD5���ܺ���䵽CLocalConsumeInfo��Ҫע��LocalExpense���㷨��
//		MakeLocalConsumeInfo(ActiveMember, consumeInfoTmp);
//		lpLocalInfo = &consumeInfoTmp;
//	}
//	CLocalConsumeInfo& consumeInfo = *lpLocalInfo;
//
//	//��ӵ�localConsume��
//	if (CIBADAL::GetInstance()->AddLocalConsume( consumeInfo ))  //liyajun note: ���LocalConsume�д��ڶ�Ӧ��¼��ֱ�ӹر����ݿ�Recordset������
//	{
//		COperationInfo OperationInfo;
//		OperationInfo.SetOperationType(CIBAGlobal::emOpTypeBrokenReturn);
//
//		OperationInfo.SetIsBundTime( FALSE );
//		OperationInfo.SetUserName( consumeInfo.GetName() );
//		OperationInfo.SetNetId( consumeInfo.GetSerialNum() );
//		OperationInfo.SetMemberId( consumeInfo.GetMemberId() );
//		OperationInfo.SetOperationAmount( consumeInfo.GetReturnAmount() );
//		OperationInfo.SetAllAmount( consumeInfo.GetCreditAmount() );
//		OperationInfo.SetOperationTime( COleDateTime::GetCurrentTime() );
//		OperationInfo.SetDescription(_T("�����û�����(����ģʽ)"));
//		::SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM)&OperationInfo);
//
//		//ɾ����ӦLastUserInfo��¼
//		CIBADAL::GetInstance()->DeleteLastUserInfo(consumeInfo.GetTermId());
//
//		// ֪ͨ�ͻ��˽�������
//		INT_PTR nIndex = CLocalServer::GetInstance()->ComputerList.Find(consumeInfo.GetMemberId());
//		if (nIndex >= 0)
//		{	
//			CLocalServer::GetInstance()->LocalCheckOutUser(nIndex, consumeInfo.GetMemberId()); //��ͻ��˷����»���Ϣ
//			Sleep(2000);
//		}
//
//		CLocalServer::GetInstance()->ActiveMemberList.RemoveActiveMember(consumeInfo.GetMemberId()); //��ActiveMemberMap��ɾ�����󣬴�ʵ��ϵͳ���»�
//		CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(FALSE);
//
//		IBA_LOG(_T("�����û��˿�: netId=%s, memberId=%d"), consumeInfo.GetSerialNum(), consumeInfo.GetMemberId());
//
//		return TRUE;
//	}
//
//	return FALSE;
//}

//�����ǳ���Ϣ�ṹ
void CCashierHelper::MakeCheckOutInfo(CActiveMember& ActiveMember, NS_DAL::CCheckOutinfo& CheckOutinfo)
{
	CheckOutinfo.SetCheckOutTime(CIBAHelpper::GetCurrentTimeAsString(TRUE)); //�ǳ�ʱ��

	//����˳���ܱ仯�������д���ʱ��һ��

	CheckOutinfo.SetTerminalId(ActiveMember.GetTerminalID());  //�ն˺�
	CheckOutinfo.SetSerialNum(ActiveMember.GetNetId());		   //�˺�
	CheckOutinfo.SetMemberId(ActiveMember.GetMemberID());      //��Ա��
	CheckOutinfo.SetUserName(ActiveMember.GetUserName());      //�û���
	CheckOutinfo.SetPersonalId(ActiveMember.GetPersonalID());  //֤������
	CheckOutinfo.SetClassName(ActiveMember.GetUserClassName());//�û�������
	CheckOutinfo.SetClassId(ActiveMember.GetClassId());        //�û�����
	CheckOutinfo.SetClassState(ActiveMember.IsMember());       //�Ƿ��Ա
	CheckOutinfo.SetBalanceMoney(ActiveMember.GetAvailavleBalance()); //���
}

/***************************************************************************************************
Function:       //CreditLocalUser
Description:    //���б��س�ֵ. ���̾�����localMember��localCredit��������¼�¼.
Table Updated:  //LocalMember,LocalCredit
Input:          //ActiveMember - �Ѽ����û�����
                //nCreditMoeny - ��ֵ���
Return:         //���ش�����Ϣ.�ɹ��Ĵ�����ϢΪ��.
********************************************************************************************************/
CString CCashierHelper::DoCredit_InLocal(CActiveMember& ActiveMember, UINT nCreditMoeny)
{
	IBA_ASSERT2(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken, "�����������µ���");

	CString strRet;
	strRet.Empty();

	UINT nMemberId = ActiveMember.GetMemberID();
	if(nMemberId == 0 || nCreditMoeny == 0) { return strRet;}
	if(nMemberId == 0)
	{
		strRet.Empty();
		strRet.Format(_T("�û��˺Ŵ���!"));
		return strRet;
	}
	if(nCreditMoeny == 0)
	{
		strRet.Empty();
		strRet.Format(_T("��ֵ������!"));
		return strRet;
	}	

	//��ѯlocalMember���Ƿ������û���¼,û�м�¼���ȴ�����¼������
	CLocalMember lm;
	if(FALSE == CIBADAL::GetInstance()->GetLocalMemberByMemberID(nMemberId,lm))
	{
		GxxDictionaryPtr memberPtr = CIBADAL::GetInstance()->CreateNewLocalMemberWithMemberID(nMemberId);
		if(memberPtr == NULL)
		{
			CActiveMember Am;
			if(FALSE == CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(nMemberId,Am))
			{
				strRet.Empty();
				strRet.Format(_T("��ȡ�û���Ϣʧ��!"));
				IBA_LOG(_T("Abnormal! CCashierHelper::CreditLocalUser MemberId %d Amount %d ����LocalMember��¼ʧ�ܣ�activemember��û���û���Ϣ����Ϣ"),nMemberId,nCreditMoeny);
			}			
			return strRet;
		}
		if (FALSE == CIBADAL::GetInstance()->InsertNewLocalMember(memberPtr))
		{
			strRet.Empty();
			strRet.Format(_T("���ݿ����ʧ��!"));
			IBA_LOG(_T("Abnormal! CCashierHelper::CreditLocalUser MemberId %d Amount %d ����LocalMember��¼ʧ��"),nMemberId,nCreditMoeny);
			return strRet;
		}
	}

	/*����Ҫ�Ȳ���localCredit,�ɹ��Ŵ����س�ֵ�ɹ�,Ȼ���ٸ���localMember,�������localMemberʧ�ܵĻ�,��ɾ��localCredit�ļ�¼,�����ֵʧ��*/
	// ���뱾�س�ֵ��¼
	CString strCheckCode;
	if(FALSE == CIBADAL::GetInstance()->InsertNewLocalCredit(nMemberId,nCreditMoeny,strCheckCode))
	{
		strRet.Empty();
		strRet.Format(_T("���ݿ����ʧ��!"));
		IBA_LOG(_T("Abnormal! CCashierHelper::CreditLocalUser MemberId %d Amount %d ���س�ֵ���±��ؿ�(localCredit)ʧ��"),nMemberId,nCreditMoeny);
		return strRet;
	}
	//���ݳ�ֵ�����¼�¼,ʧ����ɾ����ֵ��¼
	if(FALSE == CIBADAL::GetInstance()->UpdateLocalMember_CreditInfo(nMemberId,nCreditMoeny))
	{
		strRet.Empty();
		strRet.Format(_T("���ݿ����ʧ��!"));
		IBA_LOG(_T("Abnormal! CCashierHelper::CreditLocalUser MemberId %d Amount %d ���س�ֵ���±��ؿ�(localMember)ʧ��"),nMemberId,nCreditMoeny);
		CIBADAL::GetInstance()->DeleteLocalCredit(strCheckCode);
		return strRet;
	}

	IBA_LOG(_T("���س�ֵ�ɹ�: MemberId %d Amount %d"),nMemberId,nCreditMoeny);
	strRet.Empty();
	return strRet;
}

BOOL CCashierHelper::CashSellDianka(LPCTSTR lpURL)
{
	CDataXMSender xmSender;
	if (xmSender.Create(0,_T("DdnPcp-CA591950-AA6A-495a-9ABE-C02374F479FE")))
	{
		CStringA strURL = CT2W(lpURL);
		INT nRes = xmSender.SendData(1,0,0,(LPVOID)strURL.GetString(),strURL.GetLength());
		IBA_LOG0(_T("�����ͻ��������ֽ𹺵㿨��tip"));
		return nRes >= 0;
	}	
	else
	{
		IBA_LOG(_T("����ͻ��˷��͹������ֽ���㿨������ʧ��"));
		return FALSE;
	}
}

BOOL CCashierHelper::InitDiankaPlatform()
{
	if (!theApp.GetCurCashier()->IsOnDuty())
	{
		IBA_LOG0(_T("error: ��ʼ���㿨ƽ̨ʧ��, ����Ա����δ�ϰ�״̬"));
		return FALSE;
	}


	// 2012/02/29-8237-gxx: 
	{
		CFileVersionInfo fvi;
		CString strPath = theApp.GetWorkPath() +  + _T("\\IBABin\\");
		strPath.TrimRight(_T('\\'));
		strPath += _T("\\DdnPcp.dll");
		if (!fvi.Create(strPath))
		{
			IBA_LOG0(_T("error: ��ȡDdnPcp.dll�İ汾��ʧ��"));
		}
		else
		{
			CString strVer = fvi.GetProductVersion();

			strVer.Replace(_T(", "), _T("."));

			IBA_LOG(_T("DdnPcp.dll (version: %s)"), strVer);

			if (strVer.CompareNoCase(_T("2.0.0.3")) < 0)
			{
				CString strInfo;
				strInfo.Format(_T("IBA.exe %s ֻ��֧��DdnPcp.dll %s ���ϰ汾"), 
					CIBALog::GetInstance()->GetAppVer(), strVer);
				IBA_LOG0(strInfo);
				theApp.IBAMsgBox(strInfo,MB_OK|MB_ICONWARNING);
				return FALSE;
			}
		}
	}
	
	IBA_LOG0(_T("��ʼ���㿨ƽ̨����"));
	


	//ShowClientDianKa(m_hWnd,_T("http://www.baidu.com/"),WM_CLIENT_DIANKA,0,0);
	//	1.��ַ��ʽ��
	//http://�Ʒ�����:8080/netbar/common/pointcard.jsp?
	//	userId=zhoufeng&netBarId=4&dutyId=12348709&cashRegisterId=1
	//		&serial=1311144479&check=150c5c46996ca7726fe4e73c28cbae0a 
	//		2.�������壺
	//userId: ��ǰ����Ա�ĵ�¼�˺�
	//netBarId: ��ǰ����id
	//dutyId: ��ǰ���id�����ֶ�Ϊ�Ժ���չ
	//cashRegisterId: ����̨id�����ֶ�Ϊ�Ժ���չ
	//serial: ��������ʾ��ʱ�������time()�ķ���ֵ����������10���ӵ�ʱ��
	//check: У�鴮��check=md5(userId+netBarId+dutyId+cashRegisterId+serial+md5(userPwd))������userPwdΪ����Ա������

	UINT nNetbarID = CNetBarConfig::GetInstance()->GetNetBarId();
	CString strCasherID = theApp.GetCurCashier()->GetName();
	CString strPwd = theApp.GetCurCashier()->GetPassword();
	CString strTime;
	CTime tm = CTime::GetCurrentTime();
	strTime.Format(_T("%lld"), tm.GetTime());
	UINT nDutyId = theApp.GetCurCashier()->GetDutyID();
	UINT nCashRegisterId = CIBAConfig::GetInstance()->GetCashRegisterID();

	CString strOrgCode;
	strOrgCode.AppendFormat(_T("%s"),strCasherID);
	strOrgCode.AppendFormat(_T("%d"), nNetbarID);
	strOrgCode.AppendFormat(_T("%d"), nDutyId);
	strOrgCode.AppendFormat(_T("%d"), nCashRegisterId);
	strOrgCode.AppendFormat(_T("%lld"), tm.GetTime());
	CString strPwdMd5;
	CEncrypt::CalcMD5(theApp.GetCurCashier()->GetPassword(),strPwdMd5);
	strOrgCode.AppendFormat(_T("%s"), strPwdMd5);
	CString strNewCode;
	CEncrypt::CalcMD5(strOrgCode,strNewCode);

	CString strParam;
	strParam.AppendFormat(_T("userId=%s"),strCasherID);
	strParam += _T("&amp;");
	strParam.AppendFormat(_T("netBarId=%d"), nNetbarID);
	strParam += _T("&amp;");
	strParam.AppendFormat(_T("dutyId=%d"), nDutyId);
	strParam += _T("&amp;");
	strParam.AppendFormat(_T("cashRegisterId=%d"), nCashRegisterId);
	strParam += _T("&amp;");
	strParam.AppendFormat(_T("serial=%lld"), tm.GetTime());
	strParam += _T("&amp;");
	strParam.AppendFormat(_T("check=%s"), strNewCode);


	CString strUrlMain = _T("http://");
	strUrlMain += CNetBarConfig::GetInstance()->GetMainCenterIP();
	strUrlMain.AppendFormat(_T(":%d"), CNetBarConfig::GetInstance()->GetMainCenterPort());
	strUrlMain += _T("/netbar/common/pointcard.jsp");

	CString strUrlBakup = _T("http://");
	strUrlBakup += CNetBarConfig::GetInstance()->GetBackupCenterIP();
	strUrlBakup.AppendFormat(_T(":%d"), CNetBarConfig::GetInstance()->GetBackupCenterPort());
	strUrlBakup += _T("/netbar/common/pointcard.jsp");

	CString strUrls;
	strUrls += strUrlMain;
	strUrls += _T("|");
	strUrls += strUrlBakup;

	//strUrl += strParam;

	//strUrl = _T("http://msdn.microsoft.com/library");

	/*<DdnPayCard Version= "1" DomainId="99" 
		NetbarId="4" Cashier="pm" DdnServer="192.168.10.253:8080">URL</DdnPayCard>*/

	

	CString strDDNPro;
	strDDNPro = _T("<DdnPayCard Version=\"1\" ");   
	strDDNPro.AppendFormat(_T("CashId=\"%d\" "), nCashRegisterId); // ����̨ID
	strDDNPro.AppendFormat(_T("DomainId=\"%d\" "), CNetBarConfig::GetInstance()->GetDomainId());
	strDDNPro.AppendFormat(_T("NetbarId=\"%d\" "), nNetbarID);
	strDDNPro.AppendFormat(_T("Cashier=\"%s\" "), strCasherID);// ����Ա����
	strDDNPro.AppendFormat(_T("DutyId=\"%d\" "), nDutyId);
	strDDNPro.AppendFormat(_T("CasherPwd=\"%s\" "), theApp.GetCurCashier()->GetPassword());

	strDDNPro.AppendFormat(_T("DdnServer=\"%s\">"), CNetBarConfig::GetInstance()->GetDodoNewHost());
	strDDNPro.AppendFormat(_T("%s</DdnPayCard>"),strUrls);


	if (!DdnInitializePcp(strDDNPro))
	{
		IBA_LOG0(_T("error: ����  DdnPcp.dll �ĵ�������DdnInitializePcpʧ��"));
		return FALSE;
	}

	
	return TRUE;
}

BOOL CCashierHelper::UninitDiankaPlatform()
{
	DdnTerminatePcp();
	IBA_LOG0(_T("ж�ص㿨ƽ̨"));
	return TRUE;
}

BOOL CCashierHelper::SellDianka()
{
	IBA_LOG0(_T("����㿨����"));
	DdnShowPcp();
	return TRUE;
}

/*************************************************
Function:       // ReportLocalUser-��ReportLocalCenterUser
Description:    // �����Ĳ��������û��ڱ���ģʽ�µĶ������ָ����ɾ��localMember�м�¼��
Table Updated:  // localMember������Ѿ��ϱ������ɾ����¼
Input:          // pLocalMember - ��localMember����ȡ���ļ�¼
Return:         // �ɹ�����TRUE�����򷵻�FALSE
Others:         // ���Ǿɵ����ڱ���ע���û����ϱ����������ڸ�ΪΪ �����û��ϱ�����
FlowChart Redefine:
���ļ�����ĵ�½�Ļ�Ա -  + CheckOut + Activate + CheckCard&DaBalance
���ļ�����ĵ�½����ʱ�û� -  + CheckCard&DaBalance
���ļ�����ص�½�Ļ�Ա - BuckleAmount + CheckCard&DaBalance
���ؼ�����ص�½�Ļ�Ա - BuckleAmount + CheckCard&DaBalance
���ļ�����ص�½����ʱ�û� - BuckleAmount + Activate + CheckCard&DaBalance
*************************************************/
//BOOL CCashierHelper::ReportLocalCenterUser( GxxDictionary* pLocalMember )
//{	
//	if (pLocalMember == NULL) 
//	{
//		IBA_LOG(_T("Abnormal .CCashierHelper::ReportLocalCenterUser: pLocalMember is Empty!"));
//		GXX_ASSERT(0, "pLocalMemberһ������Ϊ��");
//		return FALSE;
//	}
//	if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
//	{
//		IBA_LOG(_T("Abnormal .CCashierHelper::ReportLocalCenterUser: Network is broken!"));
//		GXX_ASSERT(0, "�������ڶ�������µ��� CheckoutLocalUser");
//		return FALSE;
//	}
//
//	UINT nMemberID = pLocalMember->valueForKey("memberId")->intValue();
//
//	CActiveMember ActiveMember;
//	if(!CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(nMemberID,ActiveMember))
//	{
//		IBA_LOG(_T("Error!CCashierHelper::ReportLocalUser �޷��Ӽ����б��л�ȡ��MemberId=%d�Ķ���,������localMember�����в���.��߶�ע��."),nMemberID);
//		return FALSE;	
//	}
//
//	//��ȡcheckinTime��checkoutTime��checkoutTime�����Ч�򽫵�ǰʱ������ΪcheckoutTime
//	CString strCheckinTime = CIBAHelpper::TranformTime(StringValueForKey("checkinTime", pLocalMember));
//	CString strCheckoutTime = StringValueForKey("checkoutTime", pLocalMember);
//	COleDateTime dtCheckoutTime = CIBAHelpper::StringToOleDateTime(strCheckoutTime);
//	if (CIBAHelpper::IsValidTime(dtCheckoutTime)) 
//	{
//		strCheckoutTime = CIBAHelpper::TranformTime(strCheckoutTime);
//	}
//	else 
//	{
//		strCheckoutTime = CIBAHelpper::GetCurrentTimeAsString(FALSE);
//	}
//
//	UINT nClassID = pLocalMember->valueForKey("classId")->intValue();
//	UINT nPCClass = pLocalMember->valueForKey("pcClass")->intValue();
//	CString strTermID = StringValueForKey("termId", pLocalMember);
//	UINT nPayType = pLocalMember->valueForKey("payType")->intValue();
//
//	BOOL isLocalCheckin = (pLocalMember->valueForKey("isLocalCheckin")->intValue() == 1);
//
//	//ֻ�б���ģʽ��¼���û���ִ��DoBuckleAmount
//	if(pLocalMember->boolValueForKey("isLocalCheckin"))
//	{
//		//�����Ĳ��ϱ��۷�
//		CBuckleAmountMethod buckleMethod;
//		buckleMethod.SetMemberId(nMemberID);
//		buckleMethod.SetCheckinTime(strCheckinTime);
//		buckleMethod.SetCheckoutTime(strCheckoutTime);
//		buckleMethod.SetPcClass(nPCClass);
//		buckleMethod.SetPayType(nPayType);
//		buckleMethod.SetTermId(strTermID);
//
//		//���ѽ�� = �ܽ�� - ���
//		int nAmount = 0;
//		nAmount = pLocalMember->valueForKey("totalMoney")->intValue() - pLocalMember->valueForKey("balance")->intValue();
//
//		//�����ѽ���������ȡ��
//		if (nAmount < 0) 
//		{
//			nAmount = 0;
//		}
//		if(nAmount % 50 >0)
//		{
//			nAmount = 50 * (nAmount / 50 + 1);
//		}
//
//		buckleMethod.SetBuckleAmount(nAmount);
//
//		theApp.GetCurCashier()->DoBuckleAmount(buckleMethod);
//
//		if (buckleMethod.GetStatusCode() != 0) 
//		{
//			IBA_LOG(_T("Abnormal!CCashierHelper::ReportLocalUser Desp:Report center error! Proc:when doing BuckleAmount! StatusCode is %d"),buckleMethod.GetStatusCode());
//			return FALSE;	
//		}
//	}
//
//	//���ļ���,���ĵ�½�Ļ�Ա��ҪCheckOut + Activate
//	if(CNetBarConfig::GetInstance()->GetIsMember(pLocalMember->intValueForKey("classId")) &&    //��Ա
//		ActiveMember.GetRemark().CompareNoCase(_T("LocalActivated")) != 0 && //���ļ���
//		pLocalMember->boolValueForKey("isLocalCheckin") == 0) //���ĵ�½
//	{
//		IBA_LOG(_T("Temp.CCashierHelper::ReportLocalCenterUser :This User was activated in Center Mode , MemberId = %d"),nMemberID);
//		//CheckOut
//		CString strCheckoutTime = CIBAHelpper::GetCurrentTimeAsString(FALSE);
//
//		CCheckout2Method checkout2Method;
//		checkout2Method.SetMemberId(nMemberID);
//		checkout2Method.SetEndUsingTime(strCheckoutTime);
//		theApp.GetCurCashier()->DoCheckout2(checkout2Method, TRUE);
//		if (checkout2Method.GetStatusCode() == 0 || checkout2Method.GetStatusCode() == 31)
//		{
//			IBA_TRACE(_T("���ϱ�checkout2�ɹ�, memberId=%d,  ����ʱ��=%s"),nMemberID, strCheckoutTime);
//			IBA_LOG(_T("Temp.CCashierHelper::ReportLocalUser.��Ա�ϱ����ݺ�checkout�ɹ�, memberId=%d,����ʱ��=%s"), nMemberID,strCheckoutTime);
//		}
//		else
//		{
//			IBA_LOG(_T("Abnormal.CCashierHelper::ReportLocalCenterUser.��Ա�ϱ����ݺ�checkoutʧ�ܣ�for memberId=%d"), nMemberID);
//			return FALSE;
//		}
//
//		//Activate
//		if (CServletHellper::ActivationMember(nMemberID))
//		{
//			IBA_TRACE(_T("��;����ɹ�"));
//			IBA_LOG(_T("Temp.CCashierHelper::ReportLocalCenterUser.��Ա�ϱ����ݲ�checkout�󼤻�ɹ���for memberId=%d"), nMemberID);
//		} 
//		else 
//		{
//			IBA_TRACE(_T("��;����ʧ��"));
//			IBA_LOG(_T("Abnormal.CCashierHelper::ReportLocalUser.��Ա�ϱ����ݲ�checkout�󼤻�ʧ�ܣ�for memberId=%d"), nMemberID);
//			return FALSE;
//		}
//	}
//
//	//���ļ���,���ص�½����ʱ�û���ҪActivate
//	if( !CNetBarConfig::GetInstance()->GetIsMember(pLocalMember->intValueForKey("classId")) &&    //��ʱ�û�
//		ActiveMember.GetRemark().CompareNoCase(_T("LocalActivated")) != 0 && //���ļ���
//		pLocalMember->boolValueForKey("isLocalCheckin") == 1) //���ص�½		
//	{
//		IBA_LOG(_T("Temp.CCashierHelper::ReportLocalCenterUser :This User was activated in Center Mode , MemberId = %d"),nMemberID);
//		//Activate
//		if (CServletHellper::ActivationMember(nMemberID))
//		{
//			IBA_TRACE(_T("��;����ɹ�"));
//			IBA_LOG(_T("Temp.CCashierHelper::ReportLocalUser.��Ա�ϱ����ݲ�checkout�󼤻�ɹ���for memberId=%d"), nMemberID);
//		} 
//		else 
//		{
//			IBA_TRACE(_T("��;����ʧ��"));
//			IBA_LOG(_T("Abnormal.CCashierHelper::ReportLocalUser.��Ա�ϱ����ݲ�checkout�󼤻�ʧ�ܣ�for memberId=%d"), nMemberID);
//			return FALSE;
//		}
//	}
//
//	//�������Ͷ���Ҫ�����û���Ϣ��checkCard�Լ�DoBalance
//	CIDCheckCardMethod CheckCard;
//	CString strTmp = ActiveMember.GetNetId();
//
//	if (strTmp.GetLength() < 10)
//	{
//		strTmp.Insert(0, CString(_T("0000000000")).Left(10 - strTmp.GetLength()));
//	}
//	CheckCard.SetSerialNum(strTmp);
//	theApp.GetCurCashier()->DoCheckCard(CheckCard);
//	if(CheckCard.GetStatusCode() != 0)
//	{
//		IBA_LOG(_T("Abnormal.CCashierHelper::ReportLocalUser.IDcheckCardʧ�ܣ�for memberId=%d"), nMemberID);
//	}
//	else
//	{
//		int nCardId = CheckCard.GetCardId();
//
//		CBalanceMethod Balance;
//		Balance.SetMemberId(nMemberID);
//		Balance.SetCardId(nCardId);
//		//Balance.SetRound(0);
//
//		theApp.GetCurCashier()->DoBalance(Balance);
//
//		if(Balance.GetStatusCode()!= 0)
//		{
//			IBA_LOG(_T("Abnormal.CCashierHelper::ReportLocalUser.DoBalanceʧ�ܣ�for memberId=%d"), nMemberID);
//		}
//	}
//
//	// �ӱ������ݿ���ɾ�����ؼ�¼
//	if(!CBrokenNetwork::RemoveLocalMember(nMemberID))
//	{
//		IBA_LOG(_T("Abnormal!CCashierHelper::ReportLocalUser Desp:Access DB error! Proc:when delect from localMember!"));
//		return FALSE;
//	}
//	return TRUE;
//}

/*************************************************
Function:       // ReportLocalCredit
Description:    // �������ϱ�LocalCredit����ĳ���û��ĳ�ֵ��¼
Table Updated: //  localCredit
Input:          // pLocalMember - �������û���localMember��¼
Otherss:		// ���۳ɰܶ���ɾ��localCredit��¼
*************************************************/
void CCashierHelper::ReportLocalCredit( GxxDictionary* pLocalMember )
{
	if (CIBAGlobal::NetworkStatus != CIBAGlobal::emNetworkNormal) 
	{
		IBA_LOG(_T("Abnormal.CCashierHelper::ReportLocalCredit:Network is broken!"));
		GXX_ASSERT(0, "�������ڶ�������µ��� ReportLocalCredit");
		return;
	}

	//ȡ��MemberID��strNetId
	UINT nMemberID = pLocalMember->valueForKey("memberId")->intValue();
	CString strNetId = StringValueForKey("netId", pLocalMember);
	CString strUserName = StringValueForKey("memberName", pLocalMember);

	// ��localCredit��ȡ��memberId��Ӧ�ĳ�ֵ��¼
	CString sql;
	sql.Format(_T("select * from localCredit where memberId=%d"), nMemberID);
	GxxArrayPtr localCreditArrayPtr = CIBADAL::GetInstance()->ReadRecordsFromDB(sql.GetBuffer());
	sql.ReleaseBuffer();

	//����localCredit���ϱ���ֵ
	for (int i = 0; i < localCreditArrayPtr->count(); i++)
	{
		CString strOperationLogMsg;//������־��ע��Ϣ

		GxxDictionaryPtr localCreditPtr = localCreditArrayPtr->valueAtIndex(i)->getDictionary();
		
		UINT nAmount = localCreditPtr->valueForKey("amount")->intValue();                       //��ֵ���

		CString strRawTime = CA2T(localCreditPtr->valueForKey("creditTime")->stringValue());    //��ֵʱ��
		CString strTime = strRawTime;

		//У���ֵ��¼,��ƥ����ɾ��
		CString oldCheckcode = CA2T(localCreditPtr->valueForKey("checkcode")->stringValue());   //У���룬Ҫ��ת����TCHAR
		CString nowCheckcode = CIBAHelpper::CalcLocalCreditCheckCode(nMemberID, strTime, nAmount);  //����ȡ����ֵ���¼���У���룬���Ƿ�ƥ��
		if (nowCheckcode.CompareNoCase(oldCheckcode) != 0)   
		{
			IBA_LOG(_T("Abnormal. CCashierHelper::ReportLocalCredit: ��Ч���س�ֵ��¼,���ݱ��۸�.memberId:%d, �����˺�:%s, ��ֵ���:%.2fԪ"), nMemberID, strNetId, nAmount/100.f);
			strOperationLogMsg.Format(_T("�ϱ�ʧ�ܣ� Error: ��Ч��ֵ��¼."));
			RemoveLocalCredit(oldCheckcode);
			continue;
		}

		//��ֵ���Ϊ0��ɾ��
		if(nAmount <= 0) 
		{	
			strOperationLogMsg.Format(_T("�ϱ�ʧ�ܣ� Error: ��ֵ���Ϊ0."));
			RemoveLocalCredit(oldCheckcode);
			continue;
		}

		/*****��ʼ�ϱ�****/

		//���ֵʱ������ڳ���8Сʱ�����Ļ���Ϊ�����ѹ��ڡ��뽫����ʱ���Ϊ��ǰʱ�䡣
		COleDateTime oleT = CIBAHelpper::StringToOleDateTime(strTime);
		COleDateTimeSpan span = COleDateTime::GetCurrentTime() - oleT;
		if (span.GetTotalSeconds() > (8 * 3600 - 5))      
		{
			strTime = CIBAHelpper::GetCurrentTimeAsString(TRUE);
		}

		CCreditMethod Credit;
		Credit.SetMemberId(nMemberID);                             //��Ա��
		Credit.SetTermTime(CIBAHelpper::TranformTime(strTime));    //��ֵʱ��
		Credit.SetActivityId(0);                                   
		Credit.SetNoPresent(0);                                    //���ͳ�ֵ
		Credit.SetAmount(nAmount);                                 //��ֵ���

		theApp.GetCurCashier()->MakeupCredit(Credit);

		if (Credit.GetStatusCode() == 0) 
		{
			strOperationLogMsg.Format(_T("�ϱ��ɹ�"));
			IBA_LOG(_T("�������س�ֵ�ɹ�,memberId=%d, �����˺�:%s, ��ֵ���:%.2fԪ, ���س�ֵʱ��/��ǰʱ��:%s"), 
				nMemberID, strNetId ,nAmount/100.f, strTime);

			IBA_TRACE(_T("�������س�ֵ�ɹ�,memberId=%d, �����˺�:%s, ��ֵ���:%.2fԪ, ��ֵʱ��=%s"), 
				nMemberID, strNetId ,nAmount/100.f, strTime);
		}
		else                                          
		{
			strOperationLogMsg.Format(_T("�ϱ�ʧ�ܣ� Error: %s"),Credit.GetStatusMessage());
			IBA_LOG(_T("Error.�������س�ֵʧ��,memberId=%d, �����˺�:%s, ��ֵ���:%.2fԪ, ���س�ֵʱ��/��ǰʱ��%s.LocalCredit��¼��ɾ��"), 
				nMemberID, strNetId ,nAmount/100.f, strTime);

			IBA_TRACE(_T("�������س�ֵʧ��,memberId=%d, �����˺�:%s, ��ֵ���:%.2fԪ, ��ֵʱ��=%s"), 
				nMemberID, strNetId ,nAmount/100.f, strTime);
		}

		RemoveLocalCredit(oldCheckcode);//���۳ɹ�ʧ�ܶ�ɾ����¼

		//д������־
		COperationInfo OperationInfo;
		OperationInfo.SetOperationType(CIBAGlobal::emOpTypeMakeupLocalCredit);
		OperationInfo.SetUserName(strUserName);
		OperationInfo.SetNetId(strNetId);
		OperationInfo.SetOperationAmount(nAmount);
		OperationInfo.SetDescription(strOperationLogMsg);
		::SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM)(&OperationInfo));
	}
	return;
}

/*************************************************
Function:       // ReportLocalReturn
Description:    // �������ϱ�LocalMember������ʱ�û����˿��¼
Table Updated: //  localCredit
Input:          // pLocalMember - �������û���localMember��¼
*************************************************/
void CCashierHelper::ReportLocalReturn(CLocalMember& localMember)
{
	//����ģʽ�²������ϱ�.��Ա�����˿�.
	if(CIBAGlobal::NetworkStatus != CIBAGlobal::emNetworkNormal 
		|| CNetBarConfig::GetInstance()->GetIsMember(localMember.nClassId)
		|| 0 == localMember.nIsReturned)
	{
		return;
	}
	CString strOperationLogMsg;

	//������Balance��ò���
	CBalanceMethod balance;
	balance.SetMemberId(localMember.nMemberId);
	BOOL bRes = balance.ExecuteBalance();
	if(FALSE == bRes)
	{
		strOperationLogMsg.Format(_T("�ϱ�ʧ�ܣ� Error: %s"),balance.GetStatusMessage());
	}
	else//�����˿�
	{
		CDeregisterMethod Deregister;

		Deregister.SetMemberId(localMember.nMemberId);
		Deregister.SetPersonalId(balance.GetPersonalId());
		Deregister.SetMemberName(balance.GetMemberName());
		Deregister.SetCardId(balance.GetCardId());
		Deregister.SetPassword(balance.GetPassword());
		CString strEndTime = localMember.checkoutTime.Format(_T("%Y%m%d%H%M%S"));
		Deregister.SetEndUsingTime(strEndTime);

		Deregister.SetDutyId(theApp.GetCurCashier()->GetDutyID());
		Deregister.SetCashRegisterId(CIBAConfig::GetInstance()->GetCashRegisterID());
		Deregister.SetOperator(theApp.GetCurCashier()->GetName());
		Deregister.SetNetBarId(CNetBarConfig::GetInstance()->GetNetBarId());

		BOOL bRes1 = Deregister.ExecuteDeregister();
		if(FALSE == bRes1)
		{
			strOperationLogMsg.Format(_T("�ϱ�ʧ�ܣ� Error: %s"),Deregister.GetStatusMessage());
		}
		else//�ɹ�
		{
			strOperationLogMsg.Format(_T("�ϱ��ɹ�"));

			//���潻����Ϣ��ע���
			Deregister.SaveTranInfo();

			//�����¼�¼�����ؿ�
			CReturnedInfo ReturnedInfo;
			ReturnedInfo.SetCardId(_ttoi(localMember.strNetId));
			ReturnedInfo.SetReturnDate(CIBAHelpper::FormatCenterTime(Deregister.GetTranTime()));
			ReturnedInfo.SetRefNo(Deregister.GetRefNo());
			ReturnedInfo.SetDeposit(Deregister.GetGuarantyMoney());
			ReturnedInfo.SetOperator(theApp.GetCurCashier()->GetName());
			ReturnedInfo.SetNetBarId(CNetBarConfig::GetInstance()->GetNetBarId());
			ReturnedInfo.SetCashRegisterID(CIBAConfig::GetInstance()->GetCashRegisterID());
			ReturnedInfo.SetMemberId(Deregister.GetMemberId());
			ReturnedInfo.SetSerialNo(Deregister.GetLocalSerialNo());
			ReturnedInfo.SetReturnAmount(Deregister.GetBalance() + Deregister.GetGuarantyMoney());
			ReturnedInfo.SetClassId(localMember.nClassId);

			CIBADAL::GetInstance()->AddReturnedRecord(ReturnedInfo);
		}		
	}
	//д������־
	COperationInfo OperationInfo;
	OperationInfo.SetOperationType(CIBAGlobal::emOpTypeMakeupLocalReturn);
	OperationInfo.SetUserName(localMember.strMemberName);
	OperationInfo.SetNetId(localMember.strNetId);
	OperationInfo.SetOperationAmount(localMember.nBalance);
	OperationInfo.SetDescription(strOperationLogMsg);
	::SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM)(&OperationInfo));

}

/*************************************************
Function:       // ReportLocalConsume
Description:    // �������ϱ�LocalMember�����û��ı������Ѽ�¼
Input:          // pLocalMember - �������û���localMember��¼
*************************************************/
void CCashierHelper::ReportLocalConsume(CLocalMember& localMember)
{
	//����ģʽ�²������ϱ�.����ģʽ�µ�½�Ĳ���Ҫ�ϱ�.
	if(CIBAGlobal::NetworkStatus != CIBAGlobal::emNetworkNormal 
		|| FALSE == localMember.nIsLocalCheckin)
	{
		return;
	}
	//CString strOperationLogMsg;
	//�����ϱ��۷�
	COleDateTime CheckinTime(localMember.checkinTime);

	CBuckleAmountMethod buckleMethod;
	buckleMethod.SetMemberId(localMember.nMemberId);
	buckleMethod.SetCheckinTime(CheckinTime.Format(_T("%Y-%m-%d %H:%M:%S")));
	buckleMethod.SetCheckoutTime(localMember.checkoutTime.Format(_T("%Y-%m-%d %H:%M:%S")));
	buckleMethod.SetPcClass(localMember.nClassId);
	buckleMethod.SetPayType(localMember.nPayType);
	buckleMethod.SetTermId(localMember.strTermId);

	//�����ѽ�� = totalMoney - balance
	int nAmount = 0;
	nAmount = localMember.ntotalMoney - localMember.nBalance;
	//�����ѽ���������ȡ��
	if (nAmount < 0) 
	{
		nAmount = 0;
	}
	if(nAmount % 50 > 0)
	{
		nAmount = 50 * (nAmount / 50 +1 ); 
	}
	buckleMethod.SetBuckleAmount(nAmount);

	theApp.GetCurCashier()->DoBuckleAmount(buckleMethod);  //������ִ�п۷ѣ������˻������ѽ�������Ҳ�۵�
	if (buckleMethod.GetStatusCode() != 0) 
	{
		IBA_LOG(_T("�ϱ��۷�ʧ��.MemberId:%d,ErrorCode:%d,ErrorMsg:%s "),localMember.nMemberId,buckleMethod.GetStatusCode(),buckleMethod.GetStatusMessage());
	}

	//д������־
	//COperationInfo OperationInfo;
	//OperationInfo.SetOperationType(CIBAGlobal::emOpTypeMakeupLocalReturn);
	//OperationInfo.SetUserName(localMember.strMemberName);
	//OperationInfo.SetNetId(localMember.strNetId);
	//OperationInfo.SetOperationAmount(localMember.nBalance);
	//OperationInfo.SetDescription(strOperationLogMsg);
	//::SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM)(&OperationInfo));

}
/*************************************************
Function:       // ReportLocalReturnVip
Description:    // �������ϱ�LocalMember���л�Ա�Ľ��˼�¼
Table Updated: //  localCredit
Input:          // pLocalMember - �������û���localMember��¼
*************************************************/
void CCashierHelper::ReportLocalReturnVip(CLocalMember& localMember)
{
	//����ģʽ�²������ϱ�.��ʱ�û����ǽ���.
	if(CIBAGlobal::NetworkStatus != CIBAGlobal::emNetworkNormal 
		|| FALSE == CNetBarConfig::GetInstance()->GetIsMember(localMember.nClassId)
		|| 0 == localMember.nIsReturned)
	{
		return;
	}
	CString strOperationLogMsg;

	//������ʱ��͵ǳ�ʱ��
	BOOL bIsOnlineWhileCheckout(FALSE);
	if(CIBAHelpper::IsValidTime(localMember.checkinTime) && localMember.checkinTime > COleDateTime(2015,1,1,0,0,0) //����͵ǳ�ʱ�䶼��Ч,��ʾ�����½���ʱ�û�����
		&& CIBAHelpper::IsValidTime(localMember.checkoutTime) && localMember.checkoutTime > COleDateTime(2015,1,1,0,0,0))
	{
		bIsOnlineWhileCheckout = TRUE;
	}
	else if(!CIBAHelpper::IsValidTime(localMember.checkinTime) || localMember.checkinTime <= COleDateTime(2015,1,1,0,0,0)//�����ʾ�����½���ʱ,�û�������
		&& !CIBAHelpper::IsValidTime(localMember.checkoutTime) || localMember.checkoutTime <= COleDateTime(2015,1,1,0,0,0))
	{
		bIsOnlineWhileCheckout = FALSE;
	}
	else//�������,�϶��ǳ�����
	{
		return;
	}

	//����ʱ�û�����,��ִ��Checkout
	if(bIsOnlineWhileCheckout)
	{
		CCheckout2Method Checkout2;
		Checkout2.SetNetBarId(CNetBarConfig::GetInstance()->GetNetBarId());
		Checkout2.SetMemberId(localMember.nMemberId);
		Checkout2.SetSafeLogout(0); 
		CString strEndTime = localMember.checkoutTime.Format(_T("%Y%m%d%H%M%S"));
		Checkout2.SetEndUsingTime(strEndTime);
		Checkout2.SetOperator(theApp.GetCurCashier()->GetName());

		BOOL bRes = Checkout2.ExecuteCheckout2();
		if(bRes)
		{
			strOperationLogMsg.Format(_T("�ϱ��ɹ�"));
		}
		else
		{
			strOperationLogMsg.Format(_T("�ϱ�ʧ�ܣ� Error: %s"),Checkout2.GetStatusMessage());
		}

		//д������־
		COperationInfo OperationInfo;
		OperationInfo.SetOperationType(CIBAGlobal::emOpTypeMakeupLocalReturnVip);
		OperationInfo.SetUserName(localMember.strMemberName);
		OperationInfo.SetNetId(localMember.strNetId);
		OperationInfo.SetOperationAmount(localMember.nBalance);
		OperationInfo.SetDescription(strOperationLogMsg);
		::SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM)(&OperationInfo));	
	}
	else//����ʱ�û�������,��ִ�з�����.�������־.
	{
		CActivationMethod Activation;
		Activation.SetMemberId(localMember.nMemberId);
		Activation.SetMode(CActivationMethod::ACTIVATION_NO);
		BOOL bRes = theApp.GetCurCashier()->DoActivation(Activation);
		//if(bRes)
		//{
		//	strOperationLogMsg.Format(_T("�ϱ��ɹ�"));
		//}
		//else
		//{
		//	strOperationLogMsg.Format(_T("�ϱ�ʧ�ܣ� Error: %s"),Activation.GetStatusMessage());
		//}
	}
}
/*************************************************
Function:       //WriteOperationLog
Description:    //����������־
Calls:          //SendMessage
Input:          //ActiveMember - ��־������Դ
                //OperationType - ��־����
*************************************************/
//void CCashierHelper::WriteReturnOperationLog(CActiveMember& ActiveMember,UINT OperatinType)
//{
//	COperationInfo OperationInfo ;
//	OperationInfo .SetOperationType (OperatinType);   
//	CString UserName;
//	UserName.Format(_T("%s(%s)"),ActiveMember.GetUserName(),ActiveMember.GetSexAsString());
//	OperationInfo.SetUserName (UserName);
//	OperationInfo.SetNetId ( ActiveMember.GetNetId());
//	OperationInfo.SetDescription(ActiveMember.GetTerminalID());
//	OperationInfo.SetClassId(ActiveMember.GetClassId());
//	OperationInfo.SetCheckInTime(ActiveMember.GetCheckInTime());
//	OperationInfo.SetIdNum(ActiveMember.GetPersonalID());
//
//	//��ʱ�û���Ҫ���������
//	if(OperatinType == CIBAGlobal::emOpTypeBrokenReturn)
//
//	{
//		UINT nDeposit(0);
//		CString sql;
//		sql.Format(_T("select deposit from member where memberId=%d"),ActiveMember.GetMemberID());
//
//		GxxDictionaryPtr MemberPtr = CIBADAL::GetInstance()->ReadRecordFromDB(sql);
//
//		if(!MemberPtr)
//		{
//			IBA_LOG(_T("Error! CCashierHelper::WriteReturnOperationLog Desp:Access DB Error! Proc: Read Deposit from Member for MemberId = %d"),ActiveMember.GetMemberID());
//		}
//		nDeposit = MemberPtr->valueForKey("deposit")->intValue();
//
//		UINT nAll = ActiveMember.GetAvailavleBalance() + nDeposit;
//
//		OperationInfo.SetAllAmount(nAll);
//		OperationInfo.SetDeposit(nDeposit);
//		OperationInfo.SetOperationAmount(ActiveMember.GetAvailavleBalance());
//		OperationInfo.SetTermId(ActiveMember.GetTerminalID());
//		OperationInfo.SetConsumeMoney(ActiveMember.GetAdvancePayment()-nAll);
//	}
//
//	::SendMessage (CMainFrame :: m_hOperationLog, WM_OPERATIONLOG , 0, (LPARAM )(& OperationInfo));
//}

/*************************************************
Function:       //UpdateLocalMemberForLocalReturn
Description:    //Ϊ����ģʽ���˸���LocalMember���¼.LocalMember�����ж�Ӧ�û���¼,������isReturned��checkoutTime�ֶ�,�����޼�¼,�򴴽���¼�����¼�ٸ��¼�¼.
Calls:          //..
Table Accessed: //LocalMember
Table Updated:  //LocalMember
Input:          //nMemberID - ��Ҫ���»������û���
Output:         //None
Return:         //���»򴴽��������ʧ�ܶ�����FALSE.
Others:         //None
*************************************************/
//BOOL CCashierHelper::UpdateLocalMemberForLocalReturn(UINT nMemberID)
//{
//	CString sql;
//	sql.Format(_T("select * from localMember where memberId=%d"), nMemberID);
//	GxxDictionaryPtr memberPtr = CIBADAL::GetInstance()->ReadRecordFromDB(sql);
//
//	//�����м�¼
//	if (memberPtr) 
//	{
//		CString strCheckoutTime = CIBAHelpper::GetCurrentTimeAsString(TRUE);
//		sql.Format(_T("update localMember set checkoutTime='%s',isReturned=1 where memberId=%d"), strCheckoutTime, nMemberID);
//		if(CIBADAL::GetInstance()->ExecuteSQLEx(sql) < 0)
//		{
//			IBA_LOG(_T("Error! CCashierHelper::UpdateLocalMemberForLocalReturn: Desp: Access DB fail! Proc: when update localMember for MemberId = %d"),nMemberID);
//			//GXX_ASSERT(0,"����lLocalMember�����");
//			return FALSE;
//		}
//		IBA_LOG(_T("TempUse.CCashierHelper::UpdateLocalMemberForLocalReturn: isReturned is set to 1 in localMember for MemberId = %d"),nMemberID);
//	}
//	//�����޼�¼
//	else 
//	{
//		memberPtr = CIBADAL::GetInstance()->CreateNewLocalMemberWithMemberID(nMemberID);  //������¼
//		if (!memberPtr)
//		{
//			IBA_LOG(_T("Error! CCashierHelper::UpdateLocalMemberForLocalReturn: Desp: Access DB fail! Proc: when create localMember record for MemberId = %d"),nMemberID);
//			IBA_LOG(_T("CCashierHelper::UpdateLocalMemberForLocalReturn Result - FALSE, for MemberId = %d"),nMemberID);
//			return FALSE;
//		}
//		if (!CIBADAL::GetInstance()->InsertNewLocalMember(memberPtr))                    //�����¼
//		{
//			IBA_LOG(_T("Error! CCashierHelper::UpdateLocalMemberForLocalReturn: Desp: Access DB fail! Proc: when insert into localMember for MemberId = %d"),nMemberID);
//			IBA_LOG(_T("CCashierHelper::UpdateLocalMemberForLocalReturn Result - FALSE, for MemberId = %d"),nMemberID);
//			return FALSE;
//		}
//
//		IBA_TRACE(memberPtr->describe().c_str());
//		CString str = IBAGlobal::StringValueForKey("memberName", memberPtr);
//
//		CString strCheckoutTime = CIBAHelpper::GetCurrentTimeAsString(TRUE);
//
//		sql.Empty();
//		sql.Format(_T("update localMember set checkoutTime='%s',isReturned=1 where memberId=%d"),strCheckoutTime, nMemberID);   //���¼�¼
//		if (CIBADAL::GetInstance()->ExecuteSQL(sql))
//		{
//			IBA_LOG(_T("TempUse.CCashierHelper::UpdateLocalMemberForLocalReturn: isReturned is set to 1 in localMember for MemberId = %d"),nMemberID);
//		}
//		else
//		{
//			IBA_LOG(_T("Error! CCashierHelper::UpdateLocalMemberForLocalReturn: Desp: Access DB fail! Proc: when update localMember for MemberId = %d"),nMemberID);
//			IBA_LOG(_T("CCashierHelper::UpdateLocalMemberForLocalReturn Result - FALSE, for MemberId = %d"),nMemberID);
//			return FALSE;
//		}
//	}
//	return TRUE;
//}

/*************************************************
Function:       //RealnameZDCheckOut
Description:    //�Ƕ�ʵ���»�
Calls:          //ZDRealName::CheckOut
Table Accessed: //None
Table Updated:  //None
Input:          //strUserName - �û���
                //strNetId - �˺�
				//strTermId - �ն˺�
Output:         //None
Return:         //None
Others:         //None
*************************************************/
//void CCashierHelper::RealnameZDCheckOut(CString strUserName,CString strNetId, CString strTermId)
//{
//	if(CIBAGlobal::emRealnameZD != CNetBarConfig::GetInstance()->GetAuditType())
//	{
//		return;
//	}
//
//	NS_REALNAME::CheckoutInfo coi;
//	ZeroMemory(&coi, sizeof(coi));
//	_tcsncpy(coi.szName, strUserName, 100);
//	_tcsncpy(coi.szNetId, strNetId, 100);
//	_tcsncpy(coi.szTermId, strTermId, 50);
//
//	CString strIP;
//	CIBADAL::GetInstance()->GetIPFromTermId(coi.szTermId, strIP);
//	_tcsncpy(coi.szIP, strIP, 50);
//	coi.nCheckoutType = CIBAGlobal::cot_MemLogout;
//
//	theApp.GetRealName()->CheckOut(&coi);
//}

/*************************************************
Function:       //UpdateDBForLocalReturn
Description:    //Ϊ���˸��±������ݿ�.
Calls:          //AddReturnedRecord
Table Accessed: //Member
Table Updated:  //LastUser - ɾ����¼, Returned - ������¼, Member - �޸ļ�¼��AvailableΪ2(���˿�)
Input:          //ActiveMember - ���±��������Դ
Output:         //None
Return:         //����ʧ�ܷ���FALSE
Others:         //None
*************************************************/
//BOOL CCashierHelper::UpdateDBForReturn(CActiveMember& ActiveMember)
//{   
//	UINT nMemberID = ActiveMember.GetMemberID();
//
//	CString sql;
//	sql.Format(_T("select deposit from member where memberId=%d"),nMemberID);
//	GxxDictionaryPtr MemberPtr = CIBADAL::GetInstance()->ReadRecordFromDB(sql);
//	if(!MemberPtr)
//	{
//		IBA_LOG(_T("Error! CCashierHelper::UpdateDBForReturn Desp:Access DB Error! Proc: Read Deposit from Member for MemberId = %d"),ActiveMember.GetMemberID());
//		return FALSE;
//	}
//	int nDeposit = MemberPtr->valueForKey("deposit")->intValue();
//
//	UINT nAll = ActiveMember.GetAvailavleBalance() + nDeposit;
//
//	CReturnedInfo ReturnedInfo;
//
//	ReturnedInfo.SetCardId(_ttoi(ActiveMember.GetNetId()));
//	//ReturnedInfo.SetReturnDate(CIBAHelpper::FormatCenterTime(GetCurrentTime));
//	ReturnedInfo.SetReturnDate(CIBAHelpper::GetCurrentTimeAsString(TRUE));
//	ReturnedInfo.SetRefNo(ActiveMember.GetRefNo());
//	ReturnedInfo.SetDeposit(nDeposit);
//	ReturnedInfo.SetOperator(theApp.GetCurCashier()->GetName());
//	ReturnedInfo.SetNetBarId(CNetBarConfig::GetInstance()->GetNetBarId());
//	ReturnedInfo.SetCashRegisterID(CIBAConfig::GetInstance()->GetCashRegisterID());
//	ReturnedInfo.SetMemberId(nMemberID);
//	ReturnedInfo.SetSerialNo(_ttoi(ActiveMember.GetPersonalID()));
//	ReturnedInfo.SetReturnAmount(nAll);
//	ReturnedInfo.SetClassId(ActiveMember.GetClassId());
//
//	return CIBADAL::GetInstance()->AddReturnedRecord(ReturnedInfo);
//}

/*************************************************
Function:       //RemoveLocalCredit
Description:    //��LocalCredit��ɾ������(�ȼ���Ƿ����)
Table Updated:  //LocalCredit
Input:          //checkCode - Ҫɾ���ļ�¼��checkCode
Return:         //�����޶�Ӧ��¼��ɹ�������TRUE
*************************************************/
BOOL CCashierHelper::RemoveLocalCredit(CString checkCode)
{
	CSingleLock lock(&g_csLocalCreditLock,TRUE);

	CString sql;

	sql.Format(_T("select * from localcredit WHERE checkcode = '%s'"),checkCode);
	if(CIBADAL::GetInstance()->CountOfRecordsWithSQL(sql) < 1 )
	{
		return TRUE;
	}

	sql.Format(_T("DELETE from localcredit WHERE checkcode = '%s'"),checkCode);

	if(CIBADAL::GetInstance()->ExecuteSQLEx(sql) < 1)
	{
		IBA_LOG(_T("Error!CCashierHelper::RemoveLocalCredit ɾ��localCredit��¼ʧ��.sql=%s"),sql);
		return FALSE;
	}

	return TRUE;
}

/*************************************************
Function:       //RemoveLocalCredit
Description:    //��LocalCredit��ɾ������(�ȼ���Ƿ����)
Calls:          //...
Table Accessed: //LocalCredit
Table Updated:  //LocalCredit
Input:          //MemberId - Ҫɾ���ļ�¼��MemberId
Output:         //None
Return:         //�����޶�Ӧ��¼��ɹ�������TRUE
Others:         //None
*************************************************/
BOOL CCashierHelper::RemoveLocalCredit(UINT MemberId)
{
	IBA_LOG(_T("CCashierHelper::RemoveLocalCredit - Start"));
	CSingleLock lock(&g_csLocalCreditLock,TRUE);

	CString sql;

	sql.Format(_T("select * from localcredit WHERE memberId = %d"),MemberId);
	if(CIBADAL::GetInstance()->CountOfRecordsWithSQL(sql) < 1 )
	{
		lock.Unlock();
		IBA_LOG(_T("CCashierHelper::RemoveLocalCredit - End. No record in table whose MemberId = %d"),MemberId);
		return TRUE;
	}

	sql.Format(_T("DELETE from localcredit WHERE memberId = %d"),MemberId);

	if(CIBADAL::GetInstance()->ExecuteSQLEx(sql) <1)
	{
		IBA_LOG(_T("CCashierHelper::RemoveLocalCredit - Result is FALSE for memberId = %d"),MemberId);
		lock.Unlock();
		GXX_ASSERT(0,"��localCreditɾ������ʧ�ܣ�");
		return FALSE;
	}

	IBA_LOG(_T("CCashierHelper::RemoveLocalCredit - Result is TRUE for memberId = %d"),MemberId);
	lock.Unlock();
	return TRUE;
}

/*************************************************
Function:       //CalcExpense
Description:    //���㱾�ؼƷ�ʱ�ķ���
Input:          //strRate - �����ַ���
				//lastUpdateTime - ��һ�θ���ʱ��
				//nLastBalance - ��һ����ʣ���
Return:         //�µ����
*************************************************/
UINT CCashierHelper::CalcExpense(CString strRate,time_t lastUpdateTime,UINT nLastBalance)
{
	UINT nNewBalance = nLastBalance;

	time_t nowT = time(NULL);
	//��������
	char sRate[128];
	memcpy(sRate,CT2A(strRate.GetBuffer()),sizeof(sRate));
	G2XUtility::CGxxStringA rate = sRate;
	float fRates[24];
	for (int i = 0; i < 24; i++) 
	{
		fRates[i] = rate.Mid(i*4, 4).ParseInt() / 100.0f;
	}

	COleDateTime lastTime(lastUpdateTime);
	COleDateTime nowTime(nowT);

	//��ʼ�������
	int nLastHour, nNowOur;
	nLastHour = lastTime.GetHour();
	nNowOur = nowTime.GetHour();
	int nLastMin = lastTime.GetMinute();
	int nLastSec = lastTime.GetSecond();

	int nTotalSec = nowT - lastUpdateTime;              //������
	int nFrontSec = nLastMin*60 + nLastSec;    //�ϴμƷ�ʱ�������
	IBA_LOG(_T("���ؼƷ�.������%d"),nTotalSec);

	float fSum = 0;
	if (nFrontSec + nTotalSec <= 3600)                 //������ͬһ��Сʱ֮�ڣ�ȡ��ǰСʱ�Ŀ��ʼ������
	{
		fSum += fRates[nLastHour] * nTotalSec / 3600;
	}
	else 
	{
		fSum += fRates[nLastHour] * (3600 - nFrontSec) / 3600;   //��ǰСʱ�İ���ǰСʱ���ʼ���
		nTotalSec -= (3600 - nFrontSec);
		nLastHour++;
		for (; nTotalSec > 0; nTotalSec -= 3600, nLastHour += 1) //ʣ��İ�����Сʱ�Ŀ��ʼ���
		{
			if (nLastHour == 24) //���ϼ��п��ܱ��24
			{
				nLastHour = 0;
			}
			int nSe = 3600;
			if (nTotalSec < 3600) 
			{
				nSe = nTotalSec;
			}
			fSum += fRates[nLastHour] * nSe / 3600;
		}
	}

	
	int nConsume = int(fSum * 100);
	if (nConsume == 0)                                               //ÿ�μƷ��������Ϊ1��Ǯ
	{
		nConsume = 1;
	}
	IBA_LOG(_T("���ؼƷ�,������%d(��Ǯ)"),nConsume);

	nNewBalance = nLastBalance - nConsume;			

	if (nNewBalance < 0) 
	{
		nNewBalance = 0;
	}
	return nNewBalance;
}

/*************************************************
Function:       //CalcRemainTime
Description:    //���㱾�ؼƷ�ʱ��ʣ�������ʱ��
Input:          //strRate - �����ַ���
				//lastUpdateTime - ��һ�θ���ʱ��
				//nBalance - ���
Return:         //�µ����
*************************************************/
UINT CCashierHelper::CalcRemainTime(UINT nBalance,time_t LastUpdateTime,CString strRate)
{
	char sRate[128];
	memcpy(sRate,CT2A(strRate.GetBuffer()),sizeof(sRate));
	G2XUtility::CGxxStringA rate = sRate;
	if(rate.GetLength() != 96)
	{
		IBA_LOG(_T("Abnormal.CLocalCenter::HandleQueryBalance.��Ч�Ŀ��ʴ�"));
		return 0;
	}
	GXX_ASSERT(rate.GetLength() == 96, "��Ч�Ŀ��ʴ�");
	float fRates[24];
	float fMinRate = 1000.0f;
	for (int i = 0; i < 24; i++)
	{
		fRates[i] = rate.Mid(i*4, 4).ParseInt() / 100.0f;
		if (fRates[i] < fMinRate) 
		{
			fMinRate = fRates[i];
		}
	}

	// ����RemainTime
	int nRemainTime = 0;

	float fBalance = (float)nBalance / 100.0f;
	COleDateTime lastTime(LastUpdateTime);
	int nLastHour = lastTime.GetHour();
	int nLastSec = 60 * lastTime.GetMinute() + lastTime.GetSecond();

	float fB = (3600 - nLastSec) * fRates[nLastHour] / 3600;    //�ϴ��ϱ�ʱ�����ڵ��Ǹ�Сʱʣ�������*�����=��Сʱ���Ҫ������Ҫ�Ľ��
	if (fB < fBalance) 
	{
		nRemainTime += 3600 - nLastSec;   //�������nRemainTime = ��Сʱʣ�µ�ʱ��

		nLastHour += 1;
		if (nLastHour == 24) 
		{
			nLastHour = 0;
		}
		fBalance -= fB;

		nLastSec = 0;
		while(1) {

			if (fBalance - fRates[nLastHour] > 0.000001)   //ѭ���������������һ��Сʱ�������-=��һ��Сʱ��Ҫ�Ľ��
			{
				fBalance -= fRates[nLastHour];
				nLastHour++;
				nRemainTime += 3600;
				if (nLastHour == 24)
				{
					nLastHour = 0;
				}
			}
			else                                           //ֱ������Ϊֹ������������������ѵ�ʱ�䡣��ȷ������Ǹ�Сʱ����û�о�ȷ�����ӡ�
			{
				break;
			}
		}
	}

	float fCurPrice = fRates[nLastHour] / 3600; // ÿһ��ļ۸�

	while (fBalance > 0.000001)                   //balanceΪ�������һ��Сʱ�Ľ��
	{                                        

		nLastSec += 1;
		if (nLastSec >= 3600) 
		{
			nLastHour += 1;
			if (nLastHour == 24) 
			{
				nLastHour = 0;
			}
			nLastSec = 0;

			fCurPrice = fRates[nLastHour] / 3600;
		}
		fBalance -= 1 * fCurPrice;
		nRemainTime += 1;
	}                                    //����ʣ��ʱ�侫ȷ������
	
	return nRemainTime;
}