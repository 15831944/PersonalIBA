
#include "stdafx.h"
#include ".\realnamebase.h"
#include "..\IBA.h"
#include "..\Cashier.h"
#include "..\MainFrm.h"
#include "..\NDReturnDlg.h"
#include "..\NDCreditDlg.h"
#include "..\CreditDlg.h"
#include "..\RegisterDlg.h"
#include "..\RegImageDlg.h"
#include "..\CheckMemberDlg.h"
#include "..\DialogManager.h"
#include "..\ScanReco.h"
#include "..\IBAHelpper.h"
#include "..\TTS\IBATTS.h"
#include "..\Socket\LocalServer.h"
#include "..\ServletHellper.h"
#include "..\..\Lib\icDllChain.h"
#include "..\ActiveMemberView.h"
#include "..\IdentityNumber.h"
#include "..\NetBarConfig.h"
#include "..\RealName\IBARealName.h"
#include "..\RealName\WDRealName.h"
#include "RZXRealName.h"
#include "..\Dal\IBADAL.h"
#include "..\ModifyMemberDlg.h"
#include "AuditInterface.h"
#include "..\LocalCenter\G2XCommonTool.h"
#include "..\GlobalFunctions.h"
#include "..\ReturnDlg.h"


// 2014-5-23 - qsc
static std::list<CAuditInterface::RegUserInfo> g_listRegUserInfo;
static CCriticalSection g_csRegUserInfo;
static HANDLE g_hRegUserInfo = NULL;

using namespace NS_REALNAME;
using namespace IBAGlobal;
//ʵ������
CRealNameBase::CRealNameBase(void)
: m_bOpenReader(FALSE), m_nCardId(0), m_nMemberId(0)
, m_DataXReceiver(this, OnDataArrival)
{	
	CIBALog::GetInstance()->Write(_T("CRealNameBase"));
	
	//��֤·������
	CIBAHelpper::MakeSurePathExists(theApp.GetWorkPath() + _T("\\IBATemp"));
	//CIBAHelpper::MakeSurePathExists(theApp.GetWorkPath() + _T("\\IBARealName"));
}

CRealNameBase::~CRealNameBase(void)
{
}

/*************************************************
Function:       // ActivationMember
Description:    // ��ʾˢ����¼�����ݺ�̨�����ϴ���Ƭ���޸��û����ϣ��������ļ���û���ͼ��ɾ����������������ʾ�ڵ�һ��
Calls:          // CServletHellper::ActivationMember
Input:          // netBarUser - ����MemberId, Department, Address, Nation���û�����
Return:         // ����ɹ�����TRUE�����෵��FALSE
Others:         // �����ʵ��ˢ����Balanceδͨ��QueryBalance�Ƿ���ֵ����
*************************************************/
BOOL CRealNameBase::ActivationMember(const CNetBarUser& netBarUser)
{
	IBA_LOG(_T("CRealNameBase::ActivationMember Start - for MemberID = %d"),netBarUser.GetMemberId());

	UINT nMemberID = netBarUser.GetMemberId();
	
	SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 1, NULL); //��ʾˢ����¼

	CBalanceMethod & Balance = CCurrentNetBarUser::GetInstance()->Balance;
	
	//��ܼ��������ϴ���Ƭ���ϴ���Ƭ
	if(0 != CNetBarConfig::GetInstance()->GetSoftSfreg())
	{
		CString strDest = CIBAHelpper::GetUserZjImgFilePath(Balance.GetIdType(), Balance.GetPersonalId());
		// 2014-8-13 - qsc ÿ�ζ����� if(!PathFileExists(strDest))
		{
			const CString strSource = theApp.GetWorkPath() + _T("\\IBATemp\\ID.jpg");
			CopyFile(strSource, strDest, FALSE);
			// 2014-7-4 - qsc ���������±���,��ɾ��ԭ�ļ�
			CString strBak = theApp.GetWorkPath() + _T("\\IBATemp\\ID_bak.jpg");
			CopyFile(strSource, strBak, FALSE);
			DeleteFile(strSource);
		}		
	}

	if(2 == CNetBarConfig::GetInstance()->GetSoftSfreg())
	{
		CString strDest = CIBAHelpper::GetUserZcImgFilePath(Balance.GetIdType(), Balance.GetPersonalId());
		if(!PathFileExists(strDest))
		{
			if(!CIBAHelpper::TakePhoto(CIBAHelpper::GetUserZcImgPath(Balance.GetIdType(), Balance.GetPersonalId()), Balance.GetPersonalId()))
			{
				IBA_LOG(_T("CRealNameBase::ActivationMember Result is FALSE caused by taking photo failed- for MemberID = %d"),netBarUser.GetMemberId());
				return FALSE; 
			}
		}		
	}

	BOOL bUpdateDataBase = FALSE;

	//2014-1-2-qsc �����޸��û�������     ����ܼ�����������or��֤�绰����
	if(0 != CNetBarConfig::GetInstance()->GetShenzhen() ||	(0 != CNetBarConfig::GetInstance()->GetTelVerify()))
	{
		CString strTel(Balance.GetTel());
		strTel.Trim();
		CString strDepart(Balance.GetIdDepart());
		strDepart.Trim();
		CString strAddress(Balance.GetAddress());
		strAddress.Trim();
		CString strNation(Balance.GetJob());
		strNation.Trim();

		//����绰����Ϊ�գ����޸��û����ϣ�����绰����
		if(0 != CNetBarConfig::GetInstance()->GetTelVerify() &&	strTel.IsEmpty())
		{
			IBA_LOG0(_T("�ֻ�����Ϊ��"));
			if (AfxGetMainWnd()->IsIconic())//�жϵ�ǰ����С����
			{
				AfxGetMainWnd()->ShowWindow(SW_MAXIMIZE);//���
			}

			CModifyMemberDlg dlg(NULL, TRUE, FALSE);
			dlg.SetUserInfo(Balance);
			if(IDOK == dlg.DoModal())
			{
				strTel = dlg.GetTelNum();
				bUpdateDataBase = TRUE;
			}
			else
			{
				IBA_LOG(_T("CRealNameBase::ActivationMember Result is FALSE caused by CModifyMemberDlg canceled- for MemberID = %d"),netBarUser.GetMemberId());
				return FALSE;
			}
		}

		CString strBDeparet = Balance.GetIdDepart();
		CString strBAddr = Balance.GetAddress();
		CString strBNation = Balance.GetJob();

		CString strNDeparet = netBarUser.GetDepartment();
		CString strNAddr = netBarUser.GetAddr();
		CString strNNation = netBarUser.GetNation();

		IBA_LOG(_T("��������:%s,%s,%s, ��������:%s,%s,%s"), strNDeparet, strNAddr, strNNation,
			strBDeparet, strBAddr, strBNation);

		// 2014-5-8 ����������Ż����ŵ�����ȥ(��CModifyMemberDlg�����ظ���)���������ȶ������Ż���
		if(strBDeparet != strNDeparet ||
			strBAddr != strNAddr ||
			strBNation != strNNation)
		{
			// 2014-7-16 - qsc ���ķ����������ݶ���Ϊ�յĻ��Ͳ���������
			bool bCenterThreeNoEmpty = false;
			if(!strBDeparet.IsEmpty() &&
				!strBAddr.IsEmpty() &&
				!strBNation.IsEmpty())
			{
				bCenterThreeNoEmpty = true;
			}

			bool bLocalThreeNoEmpty = false;
			if(!strNDeparet.IsEmpty() &&
				!strNAddr.IsEmpty() &&
				!strNNation.IsEmpty())
			{
				bLocalThreeNoEmpty = true;
			}

			// 2014-2-27 - qsc
			if(!strNAddr.IsEmpty())
			{
				strAddress = strNAddr;
			}
			if(!strNDeparet.IsEmpty())
			{
				strDepart = strNDeparet;
			}
			if(!strNNation.IsEmpty())
			{
				strNation = strNNation;
			}
			bUpdateDataBase = TRUE;


			// 2014-7-16 - qsc ���ĵ�����������һ��Ϊ�գ����ұ��ص��������ݶ���Ϊ�ղ�ȥ�޸�
			if(bLocalThreeNoEmpty && !bCenterThreeNoEmpty)
			{
				CModifyMemberMethod ModifyMember;

				ModifyMember.SetSex(Balance.GetSex());
				ModifyMember.SetUsername(Balance.GetUserName());

				ModifyMember.SetIdType(Balance.GetIdType());
				ModifyMember.SetPersonalId(Balance.GetPersonalId());
				ModifyMember.SetOldPersonalId(Balance.GetPersonalId());
				ModifyMember.SetOpenNetBarId(Balance.GetNetBarId());
				ModifyMember.SetPassword(Balance.GetPassword());
				ModifyMember.SetMemberName(Balance.GetMemberName());
				ModifyMember.SetCountryId(_T("CN"));
				ModifyMember.SetAddress(strAddress);
				ModifyMember.SetIdDepart(strDepart);
				ModifyMember.SetJob(strNation);

				ModifyMember.SetTel(strTel);

				theApp.GetCurCashier()->DoModifyMember(ModifyMember);

				if (ModifyMember.GetStatusCode() != 0)
				{
					IBA_LOG(_T("�޸��û�����ʧ��:%s"), ModifyMember.GetStatusMessage());
				}
			}
		}
		if(bUpdateDataBase)
		{
			CIBADAL::GetInstance()->UpdateMemberInfo(nMemberID,strNation, strDepart, strAddress, strTel);
			// 2014-5-8 - qsc
			//ExeSfreg(strNation, strDepart, strAddress, strTel);
			
		}	
		// 2014-5-23 - qsc
		SendRegDataToProxy(strNation, strDepart, strAddress, strTel);

	}

	//TotalBalacne��BalanceMethod�м��� - ��������˻�= �ֽ�� + �û����ͽ�� + ��ǰ��ֵ�Ľ�� + ����Ԥ����
	INT nAllMoney = Balance.GetTotalBalacne();

	if (Balance.GetNeedActivation() == 1)
	{
		UINT nType = netBarUser.GetActivationType();

		if (CServletHellper::ActivationMember(nMemberID, nType)) //����ɹ�
		{
			//��ɾ���������½��棬�����ɱ�֤��ʾ����ǰ��
			//CLocalServer::GetInstance()->ActiveMemberList.RemoveActiveMember(Balance.GetMemberId());
			CLocalServer::GetInstance()->ActiveMemberList.RemoveActiveMemberDBPostActivation(Balance.GetMemberId());
			CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(FALSE);
			//CLocalServer::GetInstance()->ActiveMemberList.AddActiveMember(Balance);
			CLocalServer::GetInstance()->ActiveMemberList.AddActiveMemberPostActivation(Balance);
			CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(TRUE);

			CIBATTS::GetInstance()->SpeakActivation(nAllMoney, SpeakBirthday(m_NetBarUser));

            IBA_LOG(_T("CRealNameBase::ActivationMember Result is TRUE - for MemberID = %d"),netBarUser.GetMemberId());
			return TRUE;
		}
	}
	else
	{
		IBA_LOG(_T("CRealNameBase::ActivationMember Result is FALSE - Balance.GetNeedActivation is FALSE"));
	}

	CIBATTS::GetInstance()->SpeakBalance(nAllMoney);   //�������е��˴���

	IBA_LOG(_T("CRealNameBase::ActivationMember Result is FALSE - Balance.GetNeedActivation is FALSE or ActivationMember failed for MemberID = %d"),netBarUser.GetMemberId());
	return FALSE;
}


DWORD RegUserThread(void* pParam)
{
	while(1)
	{
		while(g_listRegUserInfo.empty())
		{
			Sleep(100);
		}


		CAuditInterface auditInterface;
		while(!g_listRegUserInfo.empty())
		{

			CSingleLock lock(&g_csRegUserInfo, TRUE);
			CAuditInterface::RegUserInfo regUserInfo = g_listRegUserInfo.back();
			g_listRegUserInfo.pop_back();
			lock.Unlock();

			//try
			//{
			auditInterface.CustomRegUser(regUserInfo);	
			//}
			//catch (...)
			//{
			//	IBA_LOG(_T("%s ע���쳣"), CA2T(regUserInfo.strSerialNum));
			//}
		}


	}
	return 0;
}

// 2014-5-23 - qsc 
void CRealNameBase::SendRegDataToProxy(LPCTSTR szNation, LPCTSTR szDepart, LPCTSTR szAddress, LPCTSTR szTel)
{
	CBalanceMethod & Balance = CCurrentNetBarUser::GetInstance()->Balance;
	CAuditInterface::RegUserInfo regUserInfo;

	COleDateTime dt;
	dt = COleDateTime::GetCurrentTime();
	CString strDatetime;
	strDatetime = dt.Format(_T("%Y-%m-%d"));

	regUserInfo.netbarID = CNetBarConfig::GetInstance()->GetNetBarId();
	regUserInfo.IDType = Balance.GetIdType();
	regUserInfo.strIDNumber = CT2A(Balance.GetPersonalId());
	regUserInfo.strUserName = CT2A(Balance.GetUserName());
	regUserInfo.sex = Balance.GetSex();
	regUserInfo.strCountryCode = "CN";
	regUserInfo.strAddress = CT2A(szAddress);
	regUserInfo.strTelphone = CT2A(szTel);
	regUserInfo.memberID = Balance.GetMemberId();
	regUserInfo.strBirthDay = CT2A(NS_REALNAME::CRZXRealName::GetBirthdayFromSFZ(Balance.GetPersonalId()));
	regUserInfo.strOpenDay = CT2A(strDatetime);
	regUserInfo.strValidDate = CStringA("2050-01-01");
	regUserInfo.national = NS_REALNAME::CRZXRealName::GetNationCode(szNation);
	regUserInfo.strCertificateOffice = CT2A(szDepart);
	regUserInfo.CardType = Balance.GetUserClass();
	regUserInfo.strSerialNum = CT2A(Balance.GetSerailNum());

	CSingleLock lock(&g_csRegUserInfo);
	g_listRegUserInfo.push_back(regUserInfo);
	lock.Unlock();
	if(WAIT_OBJECT_0 == WaitForSingleObject(g_hRegUserInfo, 0) || NULL == g_hRegUserInfo)
	{	
		// �߳�û������
		g_hRegUserInfo = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RegUserThread, NULL, 0, 0);
		CloseHandle(g_hRegUserInfo);
	}

}

void CRealNameBase::ExeSfreg(LPCTSTR szNation, LPCTSTR szDepart, LPCTSTR szAddress, LPCTSTR szTel)
{
	// -A֤������	-B֤�����(����)   -C����		-D�Ա�
	// -E��ַ		-F�绰����		-G����		-H��Ա��
	// -I֤�������� -J����			-K��֤��λ	-L������ 
	// -Mӡˢ�� -R�Ƿ�ֻ��
	// 2014-5-8 - qsc
	CBalanceMethod & Balance = CCurrentNetBarUser::GetInstance()->Balance;
	CString strParam, strIDType, strSex;

	// �Ա�
	if(!Balance.GetSex())
	{
		strSex.LoadString(IDS_MAN);
	}
	else
	{
		strSex.LoadString(IDS_WOMAN);
	}

	for (INT i = 0; i < CNetBarConfig::GetInstance()->m_IDTypeInfoArray.GetCount(); i++)
	{
		if(Balance.GetIdType() == CNetBarConfig::GetInstance()->m_IDTypeInfoArray.GetAt(i).GetIDTypeID())
		{
			strIDType = CNetBarConfig::GetInstance()->m_IDTypeInfoArray.GetAt(i).GetIDTypeName();
			break;
		}
	}
	
	strParam.Format(_T("-A%s -B%s -C%s -D%s -E%s -F%s -G%s -H%d -I%d -J%s -K%s -L%d -M%s -R%d"), 
		Balance.GetPersonalId(), strIDType, Balance.GetUserName(), strSex,
		szAddress, szTel, _T("CN"), Balance.GetMemberId(), 
		Balance.GetIdType(), szNation, szDepart, Balance.GetUserClass(),
		Balance.GetSerailNum(), FALSE);

	IBA_LOG(_T("���ݸ�SFREG����Ϣ:%s"),strParam);
	const CString strSource = theApp.GetWorkPath() + _T("\\IBATemp\\ID.jpg");
	const CString strDest	  = theApp.GetWorkPath() + _T("\\IBABin\\sfreg\\imgID.jpg");
	
	if(PathFileExists(strSource))		// ����һ�������ݣ�����鿴
	{
		if(!CopyFile(strSource, theApp.GetWorkPath() + _T("\\IBATemp\\ID1.jpg"), FALSE))
		{
			IBA_LOG(_T("����%sʧ��1!��%d"), strSource, GetLastError());
		}
	}

	if(PathFileExists(strDest))
	{
		if(!DeleteFile(strDest))
		{
			IBA_LOG(_T("ɾ��%sʧ��!,%d"), strDest,GetLastError());
		}
	}

	if(PathFileExists(strSource))
	{
		if(!CopyFile(strSource, strDest, FALSE))
		{
			IBA_LOG(_T("����%sʧ��!,%d"), strSource,GetLastError());
		}
	}
	else
	{
		IBA_LOG(_T("%s�����ڣ�"), strSource);
	}

	if(PathFileExists(strSource))		// ������ɾ��
	{
		if(!DeleteFile(strSource))
		{
			IBA_LOG(_T("ɾ��%sʧ��!,"), strSource, GetLastError());
		}
	}
	
	CString strSFREGPath = theApp.GetWorkPath() + _T("\\IBABin\\sfreg\\sfreg.exe");
	if(CIBAHelpper::ExePath(strSFREGPath, strParam))
	{
		IBA_LOG0(_T("����sfreg�ɹ�"));
	}
	else
	{
		IBA_LOG0(_T("����sfregʧ��"));
	}
	//}	
}

//
INT CRealNameBase::OnOldUser(CNetBarUser & NetBarUser)
{
	INT nRes = 0;
	IBA_LOG(_T("CRealNameBase::OnOldUser"));

	// QSCADD 2013-7-25 11:10:54
	// ������ʵ���Ӹ�����Ƭ����
	if(1 == CNetBarConfig::GetInstance()->GetIsShowRzxPhoto())
	{
		CString strPhotoPath;
		if(CRZXRealName::GetPhotoPath(strPhotoPath, NetBarUser.GetCertificateID()))
		{	
			CString strPath, strNewFile;
			strPath.Format(_T("%s\\%s"), theApp.GetWorkPath(), _T("UserImages"));
			CIBAHelpper::MakeSurePathExists(strPath);	// ȷ��·������
			strNewFile.Format(_T("%s\\%s.jpg"), strPath, CIBAHelpper::FormatNetId(NetBarUser.GetNetId()));
			IBA_LOG(_T("��ȡ��������Ƭ·��%s, ����·��%s"), strPhotoPath, strNewFile);
			if(!CopyFile(strPhotoPath, strNewFile,FALSE))
			{
				IBA_LOG(_T("������������Ƭʧ��"));
			}
		}	
		else
		{
			IBA_LOG(_T("��ȡ��������Ƭʧ��"));
		}
	}
	
	CActiveMember ActiveMember;

	BOOL bExists =  CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(NetBarUser.GetMemberId(), ActiveMember);

	//�����û�
	if (ActivationMember(NetBarUser))
	{
		nRes = 1;
	}

	// ��������ף����
	BOOL bBirthday = SpeakBirthday(NetBarUser);


	//// ����Ҫ��ģ����Ҳ�����û�ˢ����ҲҪ�ϴ���Ƭ-2013-6-8 16:24:00
	//if (CNetBarConfig::GetInstance()->GetAuditType() == CIBAGlobal::emRealnameWD)
	//{
	//	NS_REALNAME::CWDRealName* pWd = (NS_REALNAME::CWDRealName*)theApp.GetRealName()->m_lpRealName;
	//	if (pWd)
	//	{
	//		IBA_LOG0(_T("�ϴ�����֤����"));
	//		if (pWd->m_bNeedUpload)
	//		{
	//			if (pWd->m_NetBarUser.GetCertificateType() == 99)
	//			{
	//				if (pWd->m_NetBarUser.GetCertificateID().IsEmpty())
	//				{
	//					pWd->m_NetBarUser.SetCertificateID(NetBarUser.GetCertificateID());
	//				}
	//				if (pWd->m_NetBarUser.GetNetId().IsEmpty())
	//				{
	//					pWd->m_NetBarUser.SetNetId(NetBarUser.GetNetId());
	//				}
	//				if (pWd->m_NetBarUser.GetUserName().IsEmpty())
	//				{
	//					pWd->m_NetBarUser.SetUserName(NetBarUser.GetUserName());
	//				}
	//			}
	//			pWd->Start();
	//			pWd->m_bNeedUpload = FALSE;
	//		}
	//	}
	//	else
	//	{
	//		IBA_LOG0(_T("pointer is NULL"));
	//	}
	//}
	if (CIBAConfig::GetInstance()->GetOperationAfterSwipingCard() == 0) //�������κζԻ���
	{
		return nRes;
	}

	if (AfxGetMainWnd()->IsIconic())//�жϵ�ǰ����С����
	{
		AfxGetMainWnd()->ShowWindow(SW_MAXIMIZE);//���
	}

	AfxGetMainWnd()->BringWindowToTop();
	AfxGetMainWnd()->SetForegroundWindow();

	CCurrentNetBarUser::GetInstance()->SetNetId( NetBarUser.GetNetId());

	//������ֵ�˿�
	if (CIBAConfig::GetInstance()->GetOperationAfterSwipingCard() == 1)
	{
		if (CCreditDlg::m_StaticPrehWnd != NULL) //�����Ѿ���������
		{
			BOOL bRet = SendMessage(CCreditDlg::m_StaticPrehWnd, WM_RECEIVECARD, NULL, (LPARAM)(LPCTSTR)NetBarUser.GetNetId());

			if (!bRet) //��ʾ���ڳ�ֵ
			{
		
			}
			else
			{
				SetForegroundWindow(CCreditDlg::m_StaticPrehWnd);
				SetFocus(CCreditDlg::m_StaticPrehWnd);
			}
		}
		else
		{
			CCreditDlg dlg;
			dlg.SetNetId(NetBarUser.GetNetId(), TRUE);
			dlg.SetFormCard();
			dlg.DoModal();	
		}
	}
	else if (CIBAConfig::GetInstance()->GetOperationAfterSwipingCard() == 2) //ʹ���µĳ�ֵ/���˶Ի���
	{
		if (AfxGetMainWnd()->IsWindowEnabled())
		{
			//CString strNetId = NetBarUser.GetNetId();

			if (CCurrentNetBarUser::GetInstance()->Balance.IsNetbarMember() == 1 
				&& !bExists) //��Ա�Ҳ����û���ͼ
			{
				CNDCreditDlg dlg;
				if (bBirthday)
				{
					dlg.m_strBirthday = LOAD_STRING(IDS_BIRTHDAY);
				}
				dlg.SetNetId(NetBarUser.GetNetId());
				dlg.DoModal();
			}
			else
			{
				CNDReturnDlg dlg;
				dlg.SetNetId(NetBarUser.GetNetId());
				dlg.DoModal();
			}
		}
	}
	else
	{
		//...����ʾ�κζԻ���
	}

	return nRes;
}

/*************************************************
Function:       // OnOldUserInLocalMode
Description:    // ����ģʽ�£�Ϊ�Ѽ����û���δ�����Ա�û����е����򼤻������.
Calls:          // DoModal
Table Updated:  // Member
Input:          // NetBaruUser - �ṩNetId��CertificateID
Return:         // m_NetBarUser.GetCertificateIDΪ��,����0
				// ��Member�����Ҳ�����Ӧ���û�,����0
				// ��������1
Others:         // ֻ�б���ģʽ���¼���Ļ�Ա���Żᱻ�������Ϊ0�����´���localActiveMember��ԭ���ͼ���ģ�����Ҫ���´�����ֻ��Ҫ����ΪlocalActiveMember
*************************************************/
INT CRealNameBase::OnOldUserInLocalMode(CNetBarUser & NetBarUser)
{
	CIBALog::GetInstance()->Write(_T("CRealNameBase::OnOldUserInLocalMode"));

	// QSCADD 2013-7-25 11:10:54
	// ������ʵ���Ӹ�����Ƭ����
	if(1 == CNetBarConfig::GetInstance()->GetIsShowRzxPhoto())
	{
		CString strPhotoPath;
		if(CRZXRealName::GetPhotoPath(strPhotoPath, NetBarUser.GetCertificateID()))
		{	
			CString strPath, strNewFile;
			strPath.Format(_T("%s\\%s"), theApp.GetWorkPath(), _T("UserImages"));
			CIBAHelpper::MakeSurePathExists(strPath);	// ȷ��·������
			strNewFile.Format(_T("%s\\%s.jpg"), strPath, CIBAHelpper::FormatNetId(NetBarUser.GetNetId()));
			IBA_LOG(_T("��ȡ��������Ƭ·��%s, ����·��%s"), strPhotoPath, strNewFile);
			if(!CopyFile(strPhotoPath, strNewFile,FALSE))
			{
				IBA_LOG(_T("������������Ƭʧ��"));
			}
		}	
		else
		{
			IBA_LOG(_T("��ȡ��������Ƭʧ��"));
		}
	}

	CActiveMember ActiveMemberActivated;

	//�Ѿ�����ģ���������
	BOOL bExists =  CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(NetBarUser.GetMemberId(), ActiveMemberActivated);

	if(bExists)
	{
		if (CIBAConfig::GetInstance()->GetOperationAfterSwipingCard() == 0) //�������κζԻ���
		{
			return 1;
		}

		if (AfxGetMainWnd()->IsIconic())//�жϵ�ǰ����С����
		{
			AfxGetMainWnd()->ShowWindow(SW_MAXIMIZE);//���
		}

		AfxGetMainWnd()->BringWindowToTop();
		AfxGetMainWnd()->SetForegroundWindow();

		CCurrentNetBarUser::GetInstance()->SetNetId(NetBarUser.GetNetId());

		//������ֵ�˿�
		if (CIBAConfig::GetInstance()->GetOperationAfterSwipingCard() == 1)
		{
			if (CCreditDlg::m_StaticPrehWnd != NULL) //�����Ѿ���������
			{
				BOOL bRet = SendMessage(CCreditDlg::m_StaticPrehWnd, WM_RECEIVECARD, NULL, (LPARAM)(LPCTSTR)NetBarUser.GetNetId());
				if (!bRet) //��ʾ���ڳ�ֵ
				{
					//DoNothing
				}
				else
				{
					SetForegroundWindow(CCreditDlg::m_StaticPrehWnd);
					SetFocus(CCreditDlg::m_StaticPrehWnd);
				}
			}
			else
			{
				CCreditDlg dlg;
				//dlg.SetLocalActiveMember(ActiveMemberActivated);
				dlg.SetNetId(NetBarUser.GetNetId());
				dlg.DoModal();	
			}
		}
		else if (CIBAConfig::GetInstance()->GetOperationAfterSwipingCard() == 2) //ʹ���µĳ�ֵ/���˶Ի���
		{
			if (AfxGetMainWnd()->IsWindowEnabled())
			{
				//���жϳ�ֵ���ֵ�˿��δ�������������˵�����ڳ�ֵ��������������
				BOOL bIsNDCredit(FALSE);
				LPTSTR lpszWndCaption = new TCHAR[30];
				memset(lpszWndCaption,0,30);
				HWND hwnd = AfxGetApp()->GetMainWnd()->GetForegroundWindow()->GetSafeHwnd();
				if(::GetWindowText(hwnd,lpszWndCaption,30) > 0)
				{
					CString strWndCation(lpszWndCaption);
					strWndCation.Trim();
					bIsNDCredit = !strWndCation.CompareNoCase(_T("��ֵ"));
				}
				if(bIsNDCredit)  //��ֵ�����Ѿ�����
				{
					//DoNothing
				}
				else
				{
					CNDReturnDlg dlg;
					//dlg.SetLocalActiveMember(ActiveMemberActivated);
					dlg.SetNetId(NetBarUser.GetNetId());
					dlg.DoModal();
				}
			}
		}
		else
		{
			//...����ʾ�κζԻ�����DoNothing
		}
		return 1;
	}

	//�����û� - �����m_localActiveMember
	
	//�����û�
	IBA_LOG0 (_T ("Temp.CRealNameBase::OnLocalRealName: This is a VIP user. Will Create temp ActiveMember for him."));

	if (m_NetBarUser.GetNetId(). IsEmpty ())
	{
		IBA_LOG0 (_T ("Abnormal.CRealNameBase::OnLocalRealName:m_NetBarUser.NetId is empty! Will return TRUE directly!"));
		return 0 ;
	}

	// ʹ��CertificateID��member����ȡ������������ʼ���Ϊ0��д��localMember, ��Ҫ������ֵ����
	CString sql ;
	UINT uNetBarId = CNetBarConfig::GetInstance()->GetNetBarId();

	sql.Format ( _T("select * from member where  serialNum='%s'and netBarId = %d and (available = 0 or available = 1)"), m_NetBarUser.GetNetId(), uNetBarId); 

	GxxDictionaryPtr memberPtr = CIBADAL:: GetInstance ()->ReadRecordFromDB (sql);

	//�����ټ��һ�飬ȷ������δ���۸�
	if (memberPtr == NULL)
	{
		IBA_LOG (_T ("Abnormal.CRealNameBase::OnLocalRealName: No record found in member table.Data has been changed in Member table! Will return TRUE directly!"));
		return 0 ;
	}
	
	CActiveMember ActiveMember;
	//Ϊ��Ա�û�������ʱ������������ڱ���ģʽ���ϻ�����ʼ�������Ϊ0.
	IBA_TRACE ("%s", memberPtr -> describe_d(0, false ).c_str ());
	ActiveMember .SetNetbarID ( memberPtr-> valueForKey ("netBarId")->intValue ());                       //NetbarID
	ActiveMember .SetMemberID ( memberPtr-> valueForKey ("memberId")->intValue ());                       //MemberID
	ActiveMember .SetClassId ( memberPtr-> valueForKey ("classId")->intValue ());                         //ClassID
/*	std ::wstring name = G2XUtility ::CG2XCommonTool :: Utf8_to_WideChar( std ::string ( memberPtr-> valueForKey ("name")->stringValue ()));
	ActiveMember .SetUserName ( CString( name .c_str ()));        */                                        //UserName
	CString strUserName;
	if(CIBADAL::GetInstance()->GetUserNameFromMember(m_NetBarUser.GetNetId(),strUserName))
	{
		ActiveMember.SetUserName(strUserName);
	}
	else
	{
		std ::wstring name = G2XUtility ::CG2XCommonTool :: Utf8_to_WideChar( std ::string ( memberPtr-> valueForKey ("name")->stringValue ()));
		ActiveMember .SetUserName ( CString( name .c_str ()));                                       //UserName
	}
	ActiveMember .SetNetId ( CString( CA2T (memberPtr -> valueForKey("serialNum")-> stringValue ())));       //NetId
	ActiveMember .SetIdType ( memberPtr-> valueForKey ("idType")->intValue ());                           //idType
	ActiveMember .SetPersonalID ( CString( CA2T (memberPtr -> valueForKey("idNumber")-> stringValue ())));   //PersonalID
	ActiveMember .SetSex ( memberPtr-> valueForKey ("sex")->intValue ());                                  //Sex

	ActiveMember .SetAvailavleBalance (0);                                                            //AvailavleBalance
	ActiveMember .SetAdvancePayment (0);                                                              //AdvancePayment
	ActiveMember .SetAmount (0);                                                                       //Amount
	ActiveMember .SetCreditProcess ( ActiveMember. GetAdvancePaymentAsString ());                        //CreditProcess start with AdvancePayment
	ActiveMember .SetCasherID ( CIBAConfig:: GetInstance ()->GetCashRegisterID ());                       //CashierID
	CString strNetId = ActiveMember. GetNetId ();
	strNetId = strNetId . Right(8);
	strNetId .TrimLeft ( _T('0')); // ���˵���� ��0
	ActiveMember .SetNetIdAlias ( strNetId);                                                           //AliasNetId
	ActiveMember .SetActivationTime ( COleDateTime:: GetCurrentTime ());                                 //ActivationTime = now
	ActiveMember .SetActive ( TRUE);                                                                   //Active=TRUE
	ActiveMember .SetStatus ( CActiveMember:: EStatus_Waiting );                                         //Status = Waiting���ϻ�
	ActiveMember .SetIsOnLine(FALSE);                                                                          //IsOnline
	//ActiveMember.SetRemark(_T("LocalActivated"));                                                                 //Remark:����Ǳ��ؼ����
	//ActiveMember.SetTerminalID(_T("''"));                                                                          //TerminalId��CheckoutInfo�������������ΪNULL(activeMember��Ĭ��ΪNULL����������Ҫ��ֵ)

	SendMessage (CMainFrame :: m_hOperationLog, WM_OPERATIONLOG , 1, (LPARAM)&ActiveMember ); //��ʾˢ����¼

	//��ӵ������б�
	CLocalServer::GetInstance()->ActiveMemberList.AddActiveMember(ActiveMember);

	//δ��ֵ��Ա
	//ActiveMember.SetRefNo();         //���ú�
	//ActiveMember.SetRemark();              //Remark
	//ActiveMember.SetIsRegisterBT();  //�Ƿ񿪻���ʱ
	//ActiveMember.SetPayType();       //PayType
	//ActiveMember.SetPCClassID();     //PCClassID
	//ActiveMember.SetCheckInTime();   //CheckInTime - COleDateTime
	//ActiveMember.SetUpdateDataTime(); //UpdateDataTime - COleDateTime
	//ActiveMember.SetNextCheckinTime(); //NextCheckinTime - UINT
	//ActiveMember.SetDynamicMaxAmount(); //�ۼƷⶥ��
	//ActiveMember.SetIsLocalUser();      //IsLocalUser
	//ActiveMember.SetPassword();         //password
	//ActiveMember.SetPolicyInfo();       //costRate
	//ActiveMember.SetUserClassName();    //UserClassName
	//ActiveMember.SetRoomMemberId();     //�����������û���
	//ActiveMember.SetScanType();         //ɨ������

	if (CIBAConfig::GetInstance()->GetOperationAfterSwipingCard() == 0) //�������κζԻ���
	{
		return 1;
	}

	if (AfxGetMainWnd()->IsIconic())//�жϵ�ǰ����С����
	{
		AfxGetMainWnd()->ShowWindow(SW_MAXIMIZE);//���
	}

	AfxGetMainWnd()->BringWindowToTop();
	AfxGetMainWnd()->SetForegroundWindow();

	CCurrentNetBarUser::GetInstance()->SetNetId( NetBarUser.GetNetId());

	if (AfxGetMainWnd()->IsWindowEnabled())
	{
		//���жϳ�ֵ���ֵ�˿��δ�������������˵�����ڳ�ֵ
		BOOL bIsNDCredit(FALSE);
		LPTSTR lpszWndCaption = new TCHAR[30];
		memset(lpszWndCaption,0,30);
		HWND hwnd = AfxGetApp()->GetMainWnd()->GetForegroundWindow()->GetSafeHwnd();
		if(::GetWindowText(hwnd,lpszWndCaption,30) > 0)
		{
			CString strWndCation(lpszWndCaption);
			strWndCation.Trim();
			bIsNDCredit = !strWndCation.CompareNoCase(_T("��ֵ"));
		}
		if(bIsNDCredit)
		{
			//DoNothing
		}
		else
		{
			CNDCreditDlg dlg;
			//if (bBirthday)
			//{
			//	dlg.m_strBirthday = LOAD_STRING(IDS_BIRTHDAY);
			//}
			//dlg.SetLocalActiveMember(ActiveMember); //����ΪLocalActiveMember
			dlg.SetNetId(NetBarUser.GetNetId());
			dlg.DoModal();
		}
	}
	return 1;

	////������ֵ�˿�
	//if (CIBAConfig::GetInstance()->GetOperationAfterSwipingCard() == 1)  
	//{
	//	if (CCreditDlg::m_StaticPrehWnd != NULL) //�����Ѿ���������
	//	{
	//		BOOL bRet = SendMessage(CCreditDlg::m_StaticPrehWnd, WM_RECEIVECARD, NULL, (LPARAM)(LPCTSTR)NetBarUser.GetNetId());
	//		if (!bRet) //��ʾ���ڳ�ֵ
	//		{
	//			//DoNothing
	//		}
	//		else
	//		{
	//			SetForegroundWindow(CCreditDlg::m_StaticPrehWnd);
	//			SetFocus(CCreditDlg::m_StaticPrehWnd);
	//		}
	//	}
	//	else
	//	{
	//		CCreditDlg dlg;
	//		dlg.SetLocalActiveMember(ActiveMember);
	//		dlg.DoModal();	
	//	}
	//}
	//else if (CIBAConfig::GetInstance()->GetOperationAfterSwipingCard() == 2) //ʹ���µĳ�ֵ/���˶Ի��� 
	//{
	//	if (AfxGetMainWnd()->IsWindowEnabled())
	//	{
	//		//���жϳ�ֵ���ֵ�˿��δ�������������˵�����ڳ�ֵ
	//		BOOL bIsNDCredit(FALSE);
	//		LPTSTR lpszWndCaption = new TCHAR[30];
	//		memset(lpszWndCaption,0,30);
	//		HWND hwnd = AfxGetApp()->GetMainWnd()->GetForegroundWindow()->GetSafeHwnd();
	//		if(::GetWindowText(hwnd,lpszWndCaption,30) > 0)
	//		{
	//			CString strWndCation(lpszWndCaption);
	//			strWndCation.Trim();
	//			bIsNDCredit = !strWndCation.CompareNoCase(_T("��ֵ"));
	//		}
	//		if(bIsNDCredit)
	//		{
	//			//DoNothing
	//		}
	//		else
	//		{
	//			CNDCreditDlg dlg;
	//			//if (bBirthday)
	//			//{
	//			//	dlg.m_strBirthday = LOAD_STRING(IDS_BIRTHDAY);
	//			//}
	//			dlg.SetLocalActiveMember(ActiveMember); //����ΪLocalActiveMember
	//			dlg.DoModal();
	//		}
	//	}
	//}
	//else
	//{
	//	//...����ʾ�κζԻ���
	//}

	//return 1;
}

INT CRealNameBase::OnNewUser(CNetBarUser & NetBarUser)
{
	//if (CRegisterDlg::m_StaticPrehWnd != NULL) //ע�ᴰ���Ѿ�����
	//{
	//	SendMessage(CRegisterDlg::m_StaticPrehWnd, WM_RECEIVECARD, NULL, (LPARAM)&NetBarUser);
	//	return;
	//}

	//2015-0910 liyajun ��������½�ֹ����
	IBA_LOG(_T("Temp.CRealNameBase::OnNewUser - Start"));
	if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{	
		MessageBox(NULL,_T("��������½�ֹ������"),NULL,MB_OK);
		return 0;
	}


	if (AfxGetMainWnd()->IsIconic())
	{
		AfxGetMainWnd()->ShowWindow(SW_MAXIMIZE);
		AfxGetMainWnd()->BringWindowToTop();
	}
	
	CRegisterDlg dlg;
	dlg.SetUserInfo(NetBarUser);
	if (dlg.DoModal() == IDOK)
	{
		IBA_LOG(_T("Temp.CRealNameBase::OnNewUser - Result = 1"));
		return 1;
	}
	IBA_LOG(_T("Temp.CRealNameBase::OnNewUser - Result = 0"));
	return 0;
}

/*************************************************
Function:       // ReadUserInfo
Description:    // ��ע��� HKEY_CURRENT_USER��RealName\\Base\\NewUser �ж�ȡ�û���Ϣ���浽NetBarUser�С�(ע����е�ֵ��ID2Reader.exeˢ��ʱ������)
Calls:          // GetProfileString,GetProfileInt
Table Accessed: // None
Table Updated: //  None
Input:          // None
Output:         // NetBarUser���󣬰�����ǰ�û���������Ϣ
Return:         // ����TRUE,����ע����� HKEY_CURRENT_USER��RealName\\Base\\NewUser ��IDNoΪ�գ�����FALSE
Others:         // �����̨�����������Զ���ȡ�˺ţ�NetBarUser.NetIdΪ��ȡ�����˺ŵĺ�12λ��
*************************************************/
BOOL CRealNameBase::ReadUserInfo(CNetBarUser & NetBarUser)
{
	IBA_LOG(_T("CRealNameBase::ReadUserInfo Start.Key = HKEY_CURRENT_USER��RealName\\Base\\NewUser"));
	CString strKey(_T("RealName\\Base\\NewUser"));

	CString strTmp = theApp.GetProfileString(strKey, _T("IDNo")); //��ע���HKEY_CURRENT_USER·���¶�ȡIDNo
	IBA_LOG(_T("Read IDNo:%s"),strTmp);

	strTmp.Trim();

	if (strTmp.IsEmpty())
	{
		IBA_LOG(_T("Abnormal. CRealNameBase::ReadUserInfo: HKEY_CURRENT_USER��RealName\\Base\\NewUser ��IDNoΪ��"));
		return FALSE;
	}

	NetBarUser.SetNetId(strTmp);                                         //NetId����ΪIDNo

	strTmp = theApp.GetProfileString(strKey, _T("Name"));
	strTmp.Trim();
	strTmp.Remove(' ');
	NetBarUser.SetUserName(strTmp);                                     //UserName����ΪName

	strTmp = theApp.GetProfileString(strKey, _T("CertificateID"));      //CertificateID
	NetBarUser.SetCertificateID(strTmp);

	strTmp = theApp.GetProfileString(strKey, _T("Password"));           //Password (һ��Ϊ�գ�)
	NetBarUser.SetPassword(strTmp);

	INT nTmp = theApp.GetProfileInt(strKey, _T("CertificateType"), 11); //CertificateType
	NetBarUser.SetCertificateType(nTmp);
	
	// 2013-12-18 18:18:55-qsc
	strTmp = theApp.GetProfileString(strKey, _T("Nation"));//����       //Nation
	NetBarUser.SetNation(strTmp);

	CString strMan;
	strMan.LoadString(IDS_MAN);
	strTmp = theApp.GetProfileString(strKey, _T("Sex"));                //Sex
	if (strTmp.CompareNoCase(strMan) != 0)
	{
		NetBarUser.SetSex(1);	 //Ů��
	}
	else
	{
		NetBarUser.SetSex(0);	// �е�
	}
	
	strTmp = theApp.GetProfileString(strKey, _T("Address"));            //��ַ
	NetBarUser.SetAddr(strTmp);
	strTmp = theApp.GetProfileString(strKey, _T("Department"));         //��֤����
	NetBarUser.SetDepartment(strTmp);

	//strTmp = theApp.GetProfileString(strKey, _T("Signdate"));//ǩ������
	//strTmp = theApp.GetProfileString(strKey, _T("OutOfDate"));	 //��Ч��
	//strTmp = theApp.GetProfileString(strKey, _T("Birthday"));	//��������

	NetBarUser.SetDataFrom(1);                                  //�������� 1 - ˢ��    (���� 2 - ������ 0 - ����)                

	//�Ƿ�����ɾ������ע�����������ɾ����������SetDeleteKey(TRUE)
	if (NetBarUser.GetDeleteKey())
	{
		theApp.DelRegTree(HKEY_CURRENT_USER, _T("Software\\Sunlike\\IBA\\RealName\\Base\\NewUser"));
	}
	else
	{
		NetBarUser.SetDeleteKey(TRUE);
	}
	
	// 8257 (guoxuxing 2013-1-28): ����֤����ͼƬ
	if (CNetBarConfig::GetInstance()->GetEnableID2Reader())
	{
		CString strPath = theApp.GetWorkPath();
		CString strImgPath = strPath + _T("\\IDImages\\");
		if (!CIBAHelpper::MakeSurePathExists(strImgPath))
			IBA_LOG(_T("����֤����·��ʧ�� %s"), strPath);
		CString strID2ReaderPath = strPath + _T("\\IBABin\\");
		CString strIDPic = strID2ReaderPath + _T("temp.jpg");
		CString strNewPicPath = strImgPath;
		strNewPicPath.Append(NetBarUser.GetCertificateID());
		strNewPicPath.Append(_T(".jpg"));
		if (!PathFileExists(strIDPic))
		{
			IBA_LOG(_T("ID2Reader.exeδ����֤����ͼƬ %s"), strIDPic);
		}
		else
		{
			CopyFile(strIDPic, strNewPicPath, TRUE);

			// 2013-12-18 14:52:37-qsc
			const CString strDest	= theApp.GetWorkPath() + _T("\\IBATemp\\ID.jpg");
			if(!CopyFile(strIDPic, strDest, FALSE))
			{
				IBA_LOG(_T("����%sʧ��!,%d"), strIDPic, GetLastError());
			}
		}
	}
	
	//���ĺ�̨���ã��Ƿ��Զ���ȡ�˺ţ�0-�����ã�1-��ȡ֤����(һ�����֤��������֤)�ĺ�12λΪ�����˺�
	if(1 == CNetBarConfig::GetInstance()->GetEnableAccount12()) 
	{
		CString strCID = NetBarUser.GetCertificateID();//ȡ֤����

		if(12 < strCID.GetLength() &&                  //��֤���ų���>12���Լ�֤������Ϊ11��12(һ�����֤��������֤)����ȡ��12λ
			(NetBarUser.GetCertificateType() == 11 ||
			NetBarUser.GetCertificateType() == 12))
		{
			NetBarUser.SetNetId(strCID.Right(12));
		}
		else
		{
			//NetBarUser��NetIdֱ��ʹ��ע����л�ȡ��IDN
		}

	}
	return TRUE;
}

/*************************************************
Function:       // OnLocalRealName
Description:    // ����ģʽУ��NetId,��Member����Ҷ�ӦNetId��NebBarId,�ҿ��õ��˻�,������m_nCardId��m_nMemberId,����m_NetBarUser
				// ��У��ɹ�Ϊ�Ѽ����û���δ�����Ա�û����е����򱾵ؼ���͵�������..
Calls:          // CheckCardInLocalMode,OnOldUserInLocalMode
Table Accessed: // Member�� - ��ѯδ����VIP�û�����Ϣ���Ը�ֵ��Ϊ����ʱ������ActiveMember����
Input:          // nMsgType - δʹ��
                // strNetID - ����ѯNetID
Return:         // Member���в��Ҳ�����Ӧ�û�,����FALSE
				// Member���в��Ҳ�����Ӧ�û�,�������Ȼ�ָ�����,����FALSE
Others:         // ���δ���ֻΪVIP��Ա�û�������ʱ����
*************************************************/
BOOL NS_REALNAME::CRealNameBase::OnLocalRealName(UINT nMsgType, CString strNetID)
{
	if(CheckCardInLocalMode(strNetID)) //����ģʽ��У��NetId.��Member����Ҷ�ӦNetId��NebBarId,�ҿ��õ��˻�,������m_nCardId��m_nMemberId,����m_NetBarUser
	{
		if(!m_NetBarUser.GetNetId().IsEmpty())
		{
			OnOldUserInLocalMode(m_NetBarUser);//����ģʽ�£�Ϊ�Ѽ����û���δ�����Ա�û����е����򼤻������.
			return TRUE;
		}
	}
	return FALSE;
}

/*************************************************
Function:       // DoHasLocalRealName
Description:    // ������Ϣ��Դ(�������õķ�ʽ)����m_NebBarUser����.����ģʽʱ,���б���ģʽ�ļ���򵯴�����.
				// ����ģʽ:���ͨ�� - ��
							ˢ�� - ��
					����ģʽ: ���ͨ�� - ȡ����
							  ˢ�� - ��ע���ȡ�û����ݵ�m_NetBarUser
Calls:          // OnLocalRealName
Table Accessed: // None
Table Updated:  // None
Input:          // nMsgType - ��ʾ��Ϣ��Դ��999-ˢ����0-???��CIBAGlobal::emRealnameZD-�Ƕ�ʵ��������-δ֪
                // lpData - ���ͨ��ʱ��������Ϣ��dwLen - ���ͨ�������볤��
Output:         // None. 
Return:         // ����ģʽ���б��ز����󷵻�TRUE,ˢ����ȡע���ʧ��ʱ����TRUE.�����������FALSE.
Others:         // ����TRUEʱ,��ֱ���˳�������Ϣ��Ӧ,�൱��ʲô������
*************************************************/
BOOL NS_REALNAME::CRealNameBase::DoHasLocalRealName(UINT nMsgType,LPVOID lpData /*= NULL*/, DWORD dwLen /*= 0*/)
{
	CString strMsgType = _T("nMsgType=0");
	if(nMsgType != 0)
	{
		strMsgType = (nMsgType == 999)?_T("Sweep card"):_T("Convinient channel");
	}
	IBA_LOG(_T("CRealNameBase::DoHasLocalRealName, Start - %s"),strMsgType);

	m_NetBarUser.SetDeleteKey(FALSE);//��ɾ��HKEY_LOCAL_USER��Software\\Sunlike\\IBA\\RealName\\Base\\NewUser�ļ��������ּ�

	CString strNetId;

	switch(nMsgType)
	{
	//case 99: //���ͨ����-ȡ����
	//	strNetId = *((CString*)lpData);
	//	break;

	case 999: //ʵ��ˢ��-ȡm_NetBarUser.NetId
		if (!ReadUserInfo(m_NetBarUser))  //��ע��� HKEY_CURRENT_USER��RealName\\Base\\NewUser �ж�ȡ�û���Ϣ���浽NetBarUser��.��IDNoΪ��,����FALSE
		{
			IBA_LOG0(_T("Abnormal.CRealNameBase::DoHasLocalRealName:ReadUserInfo failed after sweeping card. IDNo is empty in registry.Will return directly!"));
			return TRUE; //�˴�Ҫ����TRUE,�Ȳ�����뱾�ؼ�������,�ֲ������������������.�൱��ʲô������.
		}
		strNetId = m_NetBarUser.GetNetId();
		break;

	case 0: //Ϊ0 -ȡm_NetBarUser.NetId
		if (!ReadUserInfo(m_NetBarUser))
		{
			IBA_LOG0(_T("Abnormal.CRealNameBase::DoHasLocalRealName:ReadUserInfo failed when  nMsgType=0. IDNo is empty in registry.Will return directly!"));
			return TRUE;
		}
		strNetId = m_NetBarUser.GetNetId();
		break;

	case CIBAGlobal::emRealnameZD:		// 2014-8-28 - qsc ΪCIBAGlobal::emRealnameZD��ȡ����
		//strNetId = (LPCTSTR)lpData;
		strNetId = *((CString*)lpData);
		break;

	default:                            //������ֱ���˳�
		IBA_ASSERT2(0,"δ֪λ��ʵ����Ϣ����");
		IBA_LOG(_T("Abnormal.CRealNameBase::DoHasLocalRealName: nMsgType = %d , Will return FALSE directly!"), nMsgType);  //�˴���û������������״̬������Ϣ��Ч���ֿ�����
		return TRUE;
		break;
	}

	//ֻ�ж����Ž��б���ģʽ�Ĳ���
	if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		if(OnLocalRealName(nMsgType, strNetId))    //����ģʽУ��NetId,��Member����Ҷ�ӦNetId��NebBarId,�ҿ��õ��˻�,������m_nCardId��m_nMemberId,����m_NetBarUser
												// ��У��ɹ�Ϊ�Ѽ����û���δ�����Ա�û����е����򱾵ؼ���͵�������..		
		{
			return TRUE;
		}
	}
	return FALSE;
}

/*************************************************
Function:       // OnRealNameMsg
Description:    // ��Ӧˢ������ͨ������(���뿪ͷ��Ϊ\\,?,+,�ҳ���>=6,�Һ�̨�����������뼤��)��
				// ������Ϣ��Դ(�������õķ�ʽ)����m_NebBarUser����.����ģʽʱ,���б���ģʽ�ļ���򵯴�����.
				// �������� - 
				// ���ͨ����У��NetId. ������m_nCardId��m_nMemberId(��NetBarUser) -������� -����ʾSeriaNum�ڹ����� -����������Ի���(���ݺ�̨����)  -- û��ע�����û���һ����
				// ʵ��ˢ����ʵ��ˢ�� - ע�����û���Ϣ��NetBarUser��У��NetId.������m_nCardId��m_nMemberId -��m_nMemberId=0���û��Ϳ������� -��������û���������Ի���(���ݺ�̨����) 
Calls:          // ...
Input:          // nMstType = ˢ��ʱΪ��999
                // lpData = ���ͨ������ʱΪ����������֣�ˢ��ʱΪ��NULL
				// dwLen = ���ͨ������ʱΪ����������ֳ��ȣ�ˢ��ʱΪ��NULL
Return:         // ����ģʽ���û�״̬ΪemUserStateLocal����-1����������0
*************************************************/
INT CRealNameBase::OnRealNameMsg(UINT nMsgType, LPVOID lpData /*= NULL*/, DWORD dwLen /*= 0*/)
{
	UNIT_TRACE("CRealNameBase::OnRealNameMsg(...)");
	IBA_LOG(_T("CRealNameBase::OnRealNameMsg  Start -ˢ��"));

	//������Ϣ��Դ(�������õķ�ʽ)����m_NebBarUser����.����ģʽʱ,���б���ģʽ�ļ���򵯴�����.
		// ����ģʽ:���ͨ�� - ��
	//	ˢ�� - ��
	//	����ģʽ: ���ͨ�� - ȡ����
	//	ˢ�� - ��ע���ȡ�û����ݵ�m_NetBarUser
	if (DoHasLocalRealName(nMsgType, lpData, dwLen))
	{
		IBA_LOG(_T("CRealNameBase::OnRealNameMsg  Result - TRUE , LocalMode Operate or Register key is empty for sweep card."));
		return 0;
	}

	//����ģʽ��� - ��������ģʽ��CheckCard,Dobalance,����m_NetBarUser��OnOldUser�������漤���Ա
	//ʵ��ˢ�� - ��ע�����û���Ϣ��NetBarUser��У��NetId. ������m_nCardId��m_nMemberId -��m_nMemberId=0���û��Ϳ������� -��������û���������Ի���(���ݺ�̨����) 
	if (999 == nMsgType)  //ͨ��ģ��ˢ����Ϣ
	{
		ReadUserInfo(m_NetBarUser); 
		m_NetBarUser.GetNetId();
		if (!CheckCard(m_NetBarUser.GetNetId()))   //IDCheckCardʧ����ֱ�ӷ���
		{
			IBA_LOG(_T("Temp.CRealNameBase::OnRealNameMsg: CheckCard failed. will return without doing anything"));
			return 0;
		}
		//���CheckCard�ɹ���m_nMemberId !=0 ����Ϊ��ע���û�
		if (m_nMemberId > 0)                       
		{
			m_NetBarUser.SetMemberId(m_nMemberId);
			QueryBalance();   //�����
			OnOldUser(m_NetBarUser);   //������ݺ�̨���õ�����Ӧ�Ի���(��ֵ�˿������)
		}
		//���CheckCard�ɹ���m_nMemberId =0 �� m_nCardId!=0 ����Ϊδע���û�
		else                                      
		{
			// 2013-12-6 15:31:50 qsc
			if (AutoMatchIDCard(m_NetBarUser))    //����ҵ���ƥ������֤�ţ��Ǿ����뻻���������û��㣿
			{
				AfxGetMainWnd()->BringWindowToTop();
				AfxGetMainWnd()->SetForegroundWindow();
				OnOldUser(m_NetBarUser);
			}
			else
			{
				OnNewUser(m_NetBarUser);          //ע��Ϊ���û�
			}
		}
	}

	return 0;
}

//������ˢ��
void CRealNameBase::OnCardIn()
{
	IBA_LOG(_T("����OnCardInˢ����ˢ����"));
	char szCardID[20] = {0};

	wReadIdCard(szCardID, TRUE);

	CString strCardID = CA2T(szCardID);//

	if (!CheckCard(strCardID))
	{
		return;
	}

	CNetBarUser NetBarUser;
	NetBarUser.SetNetId(strCardID);
	NetBarUser.SetDataFrom(1);

	if (m_nMemberId > 0) //�Ͽ����ȼ���
	{
		NetBarUser.SetMemberId(m_nMemberId);
		QueryBalance();
		OnOldUser(NetBarUser);
	}
	else //�¿����Ʒ�δע��
	{
		NetBarUser.SetCertificateID(strCardID);
		NetBarUser.SetCertificateType(99);
		NetBarUser.SetUserName(_T("��ʱ�û�"));
		NetBarUser.SetSex(0);
	
		// 2013-12-6 15:31:50 qsc
		if (AutoMatchIDCard(NetBarUser))
		{
			AfxGetMainWnd()->BringWindowToTop();
			AfxGetMainWnd()->SetForegroundWindow();

			OnOldUser(NetBarUser);
		}
		else
		{
			OnNewUser(NetBarUser);
		}
	}
}

INT CRealNameBase::InvokeRealName(UINT nMsgType, LPVOID lpData /*= NULL*/, DWORD dwLen /*= 0*/)
{
	return 0;
}

BOOL CRealNameBase::InitRealName()
{
	if (CIBAConfig::GetInstance()->GetUseOurReader())
	{
		ULONG lRet = wInit(0);//�򿪴��ڲ���ʼ��

		if (MSG_OK == lRet)
		{
			//���ý�����Ϣ���壬
			//ע�⣬����������һ���߳��ڲ��ϵĽ���ɨ�裬���������Զ�����Ϣ���Ӧ
			wSetRecvWnd(AfxGetMainWnd()->GetSafeHwnd());
			
			m_bOpenReader = TRUE;			
		}
		else
		{
			IBA_LOG(_T("Can't open the reader! wInit Return = %d"), lRet);
		}
	}

	if (!m_DataXReceiver.Create(_T("FC96DB2B-5188-4419-883F-DAB7D412555B")))
	{
		IBA_LOG(_T("Create DataXReceiver failed"));
	}

	m_DataXSender.Create(1, _T("3EA34B24-A23C-4c78-9B68-F66545913299"));

	ExecuteRNS(TRUE);

	return TRUE;
}

BOOL CRealNameBase::UnInitRealName()
{
	if (m_bOpenReader)
	{
		wClose();

		m_bOpenReader = FALSE;
	}
	ExecuteRNS(FALSE);

	
	return TRUE;
}

BOOL CRealNameBase::CheckRealName()
{
	ExecuteProxy();

	return TRUE;
}

BOOL CRealNameBase::IsScanEnabled()
{
	if (theApp.GetCurCashier()->IsOnDuty())
	{
		return (CIBAConfig::GetInstance()->GetScanId() != 0) &&
			CNetBarConfig::GetInstance()->GetScanEnabled() == 1;
	}

	return FALSE;
}

BOOL CRealNameBase::ScanUserInfo(UINT nType, CNetBarUser& NetBarUser, UINT nOption)
{
	IBA_LOG(_T("��ȡ����%d"), nType);
	CString strTmpPath = theApp.GetWorkPath() + _T("\\IBATemp\\");
	CIBAHelpper::MakeSurePathExists(strTmpPath);
	
	if(CIBAGlobal::SRT_SCAN2_BACK != nType)
	{
		strTmpPath.Append(_T("ID.jpg"));
	}
	else
	{
		strTmpPath.Append(_T("ID2_BACK.jpg"));
	}
	//IBA_LOG(_T("���õ�ַ%s"), strTmpPath);

	if(PathFileExists(strTmpPath))
	{
		if(!DeleteFile(strTmpPath))
		{
			IBA_LOG(_T("ɾ��%sʧ�ܣ�,%d"), strTmpPath, GetLastError());
		}
	}
	
	//static CIBAScanReco ScanReco;
	CIBAScanReco ScanReco;

	if (ScanReco.Open() == 1)
	{
		INT nRet = 0, nIdType = 11;

		if (CIBAGlobal::SRT_SCAN1 == nType)
		{
			if (ScanReco.ScanImage(IDENTITY_ID1, strTmpPath) == 1)
			{
				nRet = ScanReco.Recognise(IDENTITY_ID1, _T(""));
			}
		}
		else if (CIBAGlobal::SRT_SCAN2 == nType)
		{
			if (ScanReco.ScanImage(IDENTITY_ID2, strTmpPath) == 1)
			{
				nRet = ScanReco.Recognise(IDENTITY_ID2, _T(""));
			}

			nIdType = 12;
		}
		else if(CIBAGlobal::SRT_SCAN2_BACK == nType)
		{
			if (ScanReco.ScanImage(IDENTITY_ID2_BACK, strTmpPath) == 1)
			{
				nRet = ScanReco.Recognise(IDENTITY_ID2_BACK, _T(""));
			}

			nIdType = 12;
		}
		else if (CIBAGlobal::SRT_READ2 == nType)
		{
			nRet = ScanReco.ReadCard(strTmpPath);

			nIdType = 12;
		}
		else if(CIBAGlobal::SRT_SCAN_PASSPORT == nType)
		{
			if (ScanReco.ScanImage(IDENTITY_PASSPORT, strTmpPath) == 1)
			{
				nRet = ScanReco.Recognise(IDENTITY_PASSPORT, _T(""));
			}

			nIdType = 94;
		}
		else
		{
			//������ʧ��
			nRet= 0; 
		}

		if (nRet == 1)
		{	
			NetBarUser.SetCertificateID(ScanReco.GetIDNumber());
			NetBarUser.SetCertificateType(nIdType);
			NetBarUser.SetUserName(ScanReco.GetName().Trim());
			NetBarUser.SetNation(ScanReco.GetNation());
			NetBarUser.SetDepartment(ScanReco.GetDepartment());
			NetBarUser.SetSex(ScanReco.GetSex());
			NetBarUser.SetNetId(NetBarUser.GetCertificateID());
			NetBarUser.SetDataFrom(2);
			NetBarUser.SetCanEditUserInfo(TRUE);
			NetBarUser.SetAddr(ScanReco.GetAddress());
			// 2014-3-25 - qsc
			NetBarUser.SetSignDate(ScanReco.GetSignDate());
			// qscadd 2013-10-25
			if(1 == CNetBarConfig::GetInstance()->GetEnableAccount12())
			{
				CString strCID = NetBarUser.GetCertificateID();
				if(12 < strCID.GetLength() && 
					(NetBarUser.GetCertificateType() == 11 ||
					NetBarUser.GetCertificateType() == 12))
				{
					NetBarUser.SetNetId(strCID.Right(12));
				}
			}

			return TRUE;
		}

		ScanReco.Close();
	}
	else
	{
		IBA_LOG0(_T("ɨ���Ǵ�ʧ�ܣ�"));
	}
	return FALSE;
}

BOOL CRealNameBase::UploadImage(UINT nType, CNetBarUser& NetBarUser, UINT nOption)
{
	return FALSE;
}

BOOL CRealNameBase::IsRegisterEnabled()
{
	return TRUE;
}

BOOL CRealNameBase::IsCheckInEnabled()
{
	return TRUE;
}

BOOL CRealNameBase::IsMenuEnabled()
{
	return FALSE;
}

BOOL CRealNameBase::IsRealNameExtendMenuEnabled()
{
	return FALSE;
}

INT CRealNameBase::OnExecuteMsg(UINT nExecuteType, LPVOID lpData /*= NULL*/, DWORD dwLen /*= 0*/)
{
	return 0;
}

BOOL CRealNameBase::NeedToCheckRegisterData()
{
	return FALSE; //Ĭ�ϲ���Ҫ
}

BOOL CRealNameBase::DoScan(UINT nType)
{
	CNetBarUser NetBarUser;

	if (ScanUserInfo(nType, NetBarUser, 0))
	{
		CheckCard(NetBarUser.GetNetId());

		if (m_nMemberId > 0)
		{
			NetBarUser.SetMemberId(m_nMemberId);
			QueryBalance();
			OnOldUser(NetBarUser);
		}
		else
		{
			// 2013-12-6 14:56:54 - qsc
			if (AutoMatchIDCard(NetBarUser))
			{
				AfxGetMainWnd()->BringWindowToTop();
				AfxGetMainWnd()->SetForegroundWindow();

				OnOldUser(NetBarUser);
			}
			else
			{
				OnNewUser(NetBarUser);
			}
			//OnNewUser(NetBarUser);
		}

		return TRUE;
	}

	return FALSE;
}

/*************************************************
Function:       // CheckCard
Description:    // У��NetId. ������m_nCardId��m_nMemberId.
Calls:          // theApp.GetCurCashier()->DoCheckCard(IDCheckCard);
Table Accessed: // None
Table Updated:  // None
Input:          // lpszNetId - ��������������DoCheckCard
Output:         // Implicitly save result of IDCheckCard to m_nCardId and m_nMemberId.
Return:         // ����״̬ΪBroken,ֱ�ӷ���FALSE. IDCheckCard�ɹ�������TRUE.���򷵻�FALSE.
Others:         // 
*************************************************/
BOOL CRealNameBase::CheckCard(LPCTSTR lpszNetId)
{
	//����ģʽ����ҪCheckCard?
	if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		IBA_LOG(_T("Abnormal.CRealNameBase::CheckCard:NetworkStatus suddenly changed to broken"));
		return FALSE;
	}

	// qscadd 2013-10-25 �����̨�������Զ���ȡ�˺ţ���֤������Ϊ11��12ʱ(һ�������֤)����ȡ֤�������12λΪstrNetId������ͬ�����Ա���
	CString strNetId(lpszNetId);
	if(1 == CNetBarConfig::GetInstance()->GetEnableAccount12())
	{
		CString strCID = m_NetBarUser.GetCertificateID();
		if(12 < strCID.GetLength() && 
			(m_NetBarUser.GetCertificateType() == 11 ||
			m_NetBarUser.GetCertificateType() == 12))
		{
			m_NetBarUser.SetNetId(strCID.Right(12));
			strNetId = strCID.Right(12);
		}
	}

	//������CheckCard�������ص�CardId��MemberId���浽m_nCardId��m_nMemberId��
	CIDCheckCardMethod IDCheckCard;
	
	IDCheckCard.SetSerialNum(strNetId);
	IDCheckCard.SetSerialNumType(1);

	theApp.GetCurCashier()->DoCheckCard(IDCheckCard);

	if (IDCheckCard.GetStatusCode() == 0)
	{
		m_nCardId = IDCheckCard.GetCardId();		
		m_nMemberId = IDCheckCard.GetMemberId();
		return TRUE;
	}
	//�翨�Ų����ڻ����Ͽ�����m_nCardId��m_nMemberId����Ϊ0.����FALSE
	else if (IDCheckCard.GetStatusCode() == 41) //���Ų�����
	{
		IBA_LOG(_T("Abnormal.CRealNameBase::CheckCard: IDCheckCard - CardId not exist"));
	}
	else
	{
		if (IDCheckCard.GetStatusCode() != -1)
		{
			//ע��,���Ͽ�û���û���
			theApp.IBAMsgBox(IDCheckCard.GetStatusMessage());
		}	
		else
		{
			IBA_LOG(_T("Abnormal.CRealNameBase::CheckCard: IDCheckCard - Failed, return value(-1)"));
		}
	}

	m_nCardId = 0;
	m_nMemberId = 0;

	return FALSE;
}

/*************************************************
Function:       // CheckCardInLocalMode
Description:    // ����ģʽ��У��NetId.��Member����Ҷ�ӦNetId��NebBarId,�ҿ��õ��˻�,������m_nCardId��m_nMemberId,����m_NetBarUser
Calls:          // None
Table Accessed: // Member
Table Updated:  // None
Input:          // lpszNetId - ���ڴ�Member���в�ѯ��¼
Output:         // Implicitly save result to m_nCardId and m_nMemberId.
Return:         // ����ģʽ����FALSE. Member���в�ѯ������Ӧ��¼,����FALSE.
Others:         // 
*************************************************/
BOOL CRealNameBase::CheckCardInLocalMode(LPCTSTR lpszNetId)
{
	if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkNormal)
	{
		return FALSE;
	}

	//��Member����У��NetId,δע���˻�����ʱ�˺ž�������ע��
	CString sql ;
	UINT uNetBarId = CNetBarConfig::GetInstance()->GetNetBarId();
	sql .Format ( _T("select * from member where serialNum='%s' and netBarId = %d and (available = 0 or available = 1)"), lpszNetId,uNetBarId);

	IBA_LOG(_T("Temp.CRealNameBase::CheckCardInLocalMode: Find userinfomation with NebBarId = %d and serialNum = %s and (available = 0 or available = 1)"),uNetBarId,lpszNetId);

	GxxDictionaryPtr memberPtr = CIBADAL:: GetInstance ()->ReadRecordFromDB ( sql);
	if (memberPtr == NULL)
	{
		m_nCardId = 0;
		m_nMemberId = 0;
		return FALSE ;
	}

	if (!CNetBarConfig :: GetInstance()-> GetIsMember ( memberPtr ->intValueForKey ("classId")) )
	{
		m_nCardId = 0;
		m_nMemberId = 0;
		return FALSE ;
	}

	m_nCardId = memberPtr->intValueForKey("cardId");
	m_nMemberId = memberPtr->intValueForKey("memberId");

	m_NetBarUser.SetMemberId(m_nMemberId);

	CString strTmp;
	m_NetBarUser.SetNetId(StringValueForKey("serialNum",memberPtr));               //NetId����ΪidNumber
	m_NetBarUser.SetCertificateID(StringValueForKey("idNumber",memberPtr));     //CertificatedID
	
	strTmp = StringValueForKey("name",memberPtr);                                  //UserName����ΪName
	strTmp.Trim();
	strTmp.Remove(' ');
	m_NetBarUser.SetUserName(strTmp);

	m_NetBarUser.SetPassword(_T(""));                                             //Password (һ��Ϊ�գ�)
	m_NetBarUser.SetCertificateType(memberPtr->intValueForKey("idType"));         //CertificateType
	m_NetBarUser.SetNation(StringValueForKey("job",memberPtr));                  //����       //Nation

	strTmp.Format(_T("%s"),StringValueForKey("sex",memberPtr));                  //Sex
	m_NetBarUser.SetSex(_ttoi(strTmp));
	m_NetBarUser.SetAddr(StringValueForKey("address",memberPtr));               //��ַ
	m_NetBarUser.SetDepartment(StringValueForKey("department",memberPtr));       //��֤����
	m_NetBarUser.SetDataFrom(0);                                  //�������� 1 - ˢ��    (���� 2 - ������ 0 - ����) 

	////strTmp = theApp.GetProfileString(strKey, _T("Signdate"));//ǩ������
	////strTmp = theApp.GetProfileString(strKey, _T("OutOfDate"));	 //��Ч��
	////strTmp = theApp.GetProfileString(strKey, _T("Birthday"));	//��������             

	return TRUE;
}
/*************************************************
Function:       // QueryBalance
Description:    // ʹ��m_nMemberId��m_nCardId�����Ĳ���ʹ�÷��ص�SerialNum��ʾ�ڹ������С�
Calls:          // theApp.GetCurCashier()->DoBalance
Input:          // Implicitly use m_nMemberId,m_nCardId to DoBalance
Return:         // DoBalance�ɹ�������TRUE. ���򷵻�FALSE.
*************************************************/
BOOL CRealNameBase::QueryBalance()
{
	UNIT_AUTO_TRACE("CRealNameBase::QueryBalance()--");

	CCurrentNetBarUser::GetInstance()->Balance.SetMemberId(m_nMemberId);
	CCurrentNetBarUser::GetInstance()->Balance.SetCardId(m_nCardId);
	CCurrentNetBarUser::GetInstance()->Balance.SetRound(0);//ȡ��

	theApp.GetCurCashier()->DoBalance(CCurrentNetBarUser::GetInstance()->Balance);

	if (CCurrentNetBarUser::GetInstance()->Balance.GetStatusCode() == 0)
	{
		//theApp.UpdateToolbarLastID( CCurrentNetBarUser::GetInstance()->Balance.GetSerailNum()); //Temporarily comment out

		return TRUE;
	}

	return FALSE;
}

BOOL CRealNameBase::AutoModifyMemberInfo()
{
	if (CNetBarConfig::GetInstance()->GetNoModifyMember() == 0)
	{
		return FALSE;
	}

	if (m_NetBarUser.GetUserName() != CCurrentNetBarUser::GetInstance()->Balance.GetUserName() || //����
		m_NetBarUser.GetCertificateID() != CCurrentNetBarUser::GetInstance()->Balance.GetPersonalId() || //֤����
		m_NetBarUser.GetCertificateType() != CCurrentNetBarUser::GetInstance()->Balance.GetIdType()) //֤������
	{
		CCheckMemberDlg CheckMemberDlg;
		CheckMemberDlg.SetCardID(m_NetBarUser.GetNetId());
		CheckMemberDlg.SetRealName(m_NetBarUser);
		CheckMemberDlg.SetServlet(CCurrentNetBarUser::GetInstance()->Balance);
		CheckMemberDlg.DoModal();

		return TRUE;
	}

	return FALSE;
}

/*************************************************
Function:       // AutoMatchIDCard  ������������������������
Description:    // ����̨�����Ƿ�Ϊ�Զ�ƥ�����֤�ţ�����˺ų����Ƿ�������֤���ȣ����������˳�FALSE
				// ����������������֤�Ŷ�Ӧ�Ŀ���(CardId)��NetBarUser.NetId = ���ķ��ص��˺�(NetId)�����Ҳ����򷵻�FALSE
				// CheckCard,У�����淵�ص��˺ţ����nMemberId>0˵�������Ѿ����ڶ�Ӧ�Ļ�Ա����ôҪ���뻻�� 
				// CCardReplaceMethod���������뻻����NetBarUser.NetId = ���ķ��ص��¿���
				// �����˺ű��浽NetBarUser�У�����У�鿨�źͲ����
Calls:          // ...
Table Accessed: // None
Table Updated:  // None
Input:          // NetBarUser - �ṩ�˺���Ϣ
Output:         // NetBarUser - ��������ת���ɵ��˺��ٱ��浽NetBarUser
Return:         // ���ж��ɹ�����TRUE�����򷵻�FASLE
Others:         // ���������߼�δ�õ������幦���Բ�ȷ�ϣ�
*************************************************/
BOOL CRealNameBase::AutoMatchIDCard(CNetBarUser & NetBarUser)
{   
	//��̨����Ϊ���Զ�ƥ�����֤�ţ�ֱ�ӷ���FALSE
	if (CNetBarConfig::GetInstance()->GetAutoMatchIDCard() == 0) return FALSE;

	//���֤���볤��<15��ֱ�ӷ���FALSE
	if (NetBarUser.GetNetId().GetLength() < 15) return FALSE;

	//�û���ѯ�ӿ� - ͨ��PersonalId���Ƿ��ܲ�ѯ����Ӧ�û�
	CMemberInfoMethod MemberInfo;
	MemberInfo.SetPersonalId(NetBarUser.GetNetId());
	MemberInfo.SetAvailable(1);

	theApp.GetCurCashier()->DoMemberInfo(MemberInfo);
	if (MemberInfo.GetStatusCode() != 0) return FALSE;   //���Ĳ���ʧ�ܣ�ֱ�ӷ���FALSE
	if (MemberInfo.GetMemberCount() == 0) return FALSE;  //�����Ҳ�����ֱ�ӷ���FALSE


	CheckCard(MemberInfo.GetSerailNum(0)); //������У�鿨�ţ���NetId�õ�CardId��MemberId
	if (m_nMemberId > 0)
	{
		CString strNewNetId = NetBarUser.GetNetId();//���������ʺ�
		NetBarUser.SetMemberId(m_nMemberId);
		NetBarUser.SetNetId(MemberInfo.GetSerailNum(0));  //��������NetBarUser.NetId = ���ķ��صĿ����к�

		if (QueryBalance())  //�����Ĳ�ѯ���
		{
			//�����ӿ�
			if (ChangeNetIdToPersonalId(strNewNetId)) 
			{
				NetBarUser.SetNetId(strNewNetId);//��������NetBarUser.NetId = ������Ŀ����к�

				//TODO:�ٲ�һ���û���Ϣ
				CheckCard(strNewNetId);
				QueryBalance();
			}
			return TRUE;
		}
	}
	return FALSE;
}

//�Ѷ���֤ת��Ϊ������
/*************************************************
Function:       // ChangeNetIdToPersonalId
Description:    // 
Calls:          // 
Table Accessed: // 
Table Updated: // 
Input:          // 
                // 
Output:         // 
Return:         // 
Others:         // 
*************************************************/
BOOL CRealNameBase::ChangeNetIdToPersonalId(CString strNewNetId)
{	
	//����û���� - gintautomatchidcard =2��������
	if (CNetBarConfig::GetInstance()->GetAutoMatchIDCard() != 2)
	{
		return FALSE;
	}

	//���֤��֤
	CIdentityNumber IdentityNumber(strNewNetId);

	if (!IdentityNumber.IsValid())
	{
		CIBALog::GetInstance()->Write(_T("PersonalId is not valid!"));
		return FALSE;
	}

	//У�鿨��Ϣ��ͨ�����������Ϊ��ӡˢ��������ȡ��CardId
	CIDCheckCardMethod checkCard;
	checkCard.SetSerialNum(strNewNetId);
	theApp.GetCurCashier()->DoCheckCard(checkCard);

	if (checkCard.GetStatusCode() == 0)
	{
		//�޸��˺�
		CCardReplaceMethod CardReplace;
		CardReplace.SetPersonalId(CCurrentNetBarUser::GetInstance()->Balance.GetPersonalId());//֤������  ����Balance��ֵ����
		CardReplace.SetMemberName(CCurrentNetBarUser::GetInstance()->Balance.GetMemberName());
		CardReplace.SetOpenNetbarId(CCurrentNetBarUser::GetInstance()->Balance.GetOpenNetbarId());
		CardReplace.SetCardId(checkCard.GetCardId());	//ʹ�����ɵ�CardId
		CardReplace.SetPassword(_T("PASSWORDPASSWORD"));//����֤����

		theApp.GetCurCashier()->DoCardReplace(CardReplace);

		if (CardReplace.GetStatusCode() == 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CRealNameBase::ExecuteProxy()
{
	BOOL bRes = TRUE;

	HANDLE hMutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, _T("39F59AC6-0F51-4939-B91A-80012AC93AB6"));

	if (hMutex != NULL)
	{
		CloseHandle(hMutex);
	}
	else
	{
		CString strProxy(theApp.GetWorkPath());
		strProxy.Append(_T("\\IBABin\\IReal.exe"));
		
		CL2Process SunlikeAuditProxy(strProxy);
		if (!SunlikeAuditProxy.Execute())
		{
			bRes = FALSE;
		}
	}

	// 2011/11/16-8210-gxx: 

	hMutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, _T("_SUNLIKEAUDITPROXY_MUTEX_"));

	if (hMutex != NULL)
	{
		CloseHandle(hMutex);
	}
	else
	{
		CString strProxy(theApp.GetWorkPath());
		strProxy.Append(_T("\\IBABin\\SunlikeAuditProxy.exe"));

		CL2Process SunlikeAuditProxy(strProxy);
		if (!SunlikeAuditProxy.Execute())
		{
			bRes = FALSE;
		}

		// 2015-4-9
		strProxy = theApp.GetWorkPath();
		strProxy.Append(_T("\\IBABin\\rzxsurename.exe"));

		CL2Process RzxSureName(strProxy);
		if (!RzxSureName.Execute())
		{
			bRes = FALSE;
		}
	}

	//

	return bRes;
}

BOOL CRealNameBase::RobFocusAfterSwipingCard()
{
	return FALSE;
}

void CRealNameBase::OnCashierCheckIn()
{
	m_DataXSender.PostData(DataX_DDN2RN_CASHIERCHECKIN, 0, 0);
}

INT CRealNameBase::OnDataArrival(INT nAppId, INT nMsg, INT nParam1, INT nParam2, 
								 LPVOID lpBuf, size_t cbLen)
{
	DataX_UserInfo dxUserInfo = {0};

	if (DataX_RN2DDN_CARDINFO == nMsg)
	{
		memcpy(&dxUserInfo, lpBuf, sizeof DataX_UserInfo);
		
		return OnCardInfo(dxUserInfo);
	}
	else if (DataX_RN2DDN_FORCECHECKOUT == nMsg)
	{
		memcpy(&dxUserInfo, lpBuf, sizeof DataX_UserInfo);

		return OnForceCheckOut(dxUserInfo);
	}
	else if (DataX_RN2DDN_ADDMEMBER == nMsg)
	{
		memcpy(&dxUserInfo, lpBuf, sizeof DataX_UserInfo);

		return OnAddMember(dxUserInfo);
	}

	return 0;
}

BOOL CRealNameBase::OnCardInfo(DataX_UserInfo& dxUserInfo)
{
	return FALSE;
}

BOOL CRealNameBase::OnForceCheckOut(DataX_UserInfo& dxUserInfo)
{
	return FALSE;
}

BOOL CRealNameBase::OnAddMember(DataX_UserInfo& dxUserInfo)
{
	return FALSE;
}

BOOL CRealNameBase::SpeakBirthday(CNetBarUser & NetBarUser)
{
	CIdentityNumber dn(NetBarUser.GetCertificateID());
	if (dn.IsValid() && dn.IsBirthDay())
	{
		/*CString strSoundFile(theApp.GetWorkPath() + _T("\\IBAConfig\\Shengri.wav"));

		if (PathFileExists(strSoundFile))
		{
			sndPlaySound(strSoundFile, SND_ASYNC);
		}	*/

		return TRUE;
	}
	return FALSE;
}

void CRealNameBase::ExecuteRNS(BOOL bInit) 
{
	CString strRNS, strCMD;
	strRNS = theApp.GetWorkPath() + _T("\\IBABin\\RNS.EXE");

	if (PathFileExists(strRNS))
	{
		strCMD.Format(_T("-DID%d -RNT%d"), CNetBarConfig::GetInstance()->GetDomainId(),
			CNetBarConfig::GetInstance()->GetNetBarId());

		strCMD.AppendChar(' ');
		strCMD.Append(bInit ? _T("-I") : _T("-U"));

		CL2Process RNS(strRNS);
		RNS.Execute(strCMD);
	}
}

//******************************************************************************

CNoneRealName::CNoneRealName(void)
{
	CIBALog::GetInstance()->Write(_T("CNoneRealName"));
}

CNoneRealName::~CNoneRealName(void)
{

}

BOOL CNoneRealName::InitRealName()
{
	CRealNameBase::InitRealName();

	return TRUE;
}

BOOL CNoneRealName::CheckRealName()
{
	return TRUE;
}

BOOL CNoneRealName::IsScanEnabled()
{
	if (theApp.GetCurCashier()->IsOnDuty())
	{
		return (CIBAConfig::GetInstance()->GetScanId() != 0);
	}

	return FALSE;
}

