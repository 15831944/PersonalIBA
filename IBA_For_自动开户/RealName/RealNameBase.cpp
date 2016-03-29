
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

using namespace NS_REALNAME;
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

//�����û�������ʵ����ˢ֮����еĲ���
BOOL CRealNameBase::ActivationMember(const CNetBarUser& netBarUser)
{
	UINT nMemberID = netBarUser.GetMemberId();
	//��ʾˢ����¼
	SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 1, NULL);

	CBalanceMethod & Balance = CCurrentNetBarUser::GetInstance()->Balance;
	
	BOOL bUpdateDataBase = FALSE;
	//2014-1-2-qsc �����޸��û�������
	if(0 != CNetBarConfig::GetInstance()->GetShenzhen() ||
		(0 != CNetBarConfig::GetInstance()->GetTelVerify()))
	{
		CMemberInfoMethod MemberInfo;

		MemberInfo.SetMemberId(nMemberID);
		theApp.GetCurCashier()->DoMemberInfo(MemberInfo);
		CString strTel(MemberInfo.GetTel(0));
		strTel.Trim();
		CString strDepart(MemberInfo.GetIdDepart(0));
		strDepart.Trim();
		CString strAddress(MemberInfo.GetAddress(0));
		strAddress.Trim();
		CString strNation(MemberInfo.GetJob(0));
		strNation.Trim();

		if(0 != CNetBarConfig::GetInstance()->GetTelVerify() &&
			strTel.IsEmpty())
		{
			if (AfxGetMainWnd()->IsIconic())//�жϵ�ǰ����С����
			{
				AfxGetMainWnd()->ShowWindow(SW_MAXIMIZE);//���
			}

			CString str = MemberInfo.GetSerailNum(0);
			CModifyMemberDlg dlg(NULL, TRUE, FALSE, MemberInfo.GetSerailNum(0));
			if(IDOK == dlg.DoModal())
			{
				strTel = dlg.GetTelNum();
				bUpdateDataBase = TRUE;
			}
			else
			{
				return FALSE;
			}
		}

		if(netBarUser.GetAddr() != MemberInfo.GetAddress(0) ||
			netBarUser.GetDepartment() != MemberInfo.GetIdDepart(0) ||
			netBarUser.GetNation() != MemberInfo.GetJob(0))
		{
			CModifyMemberMethod ModifyMember;

			ModifyMember.SetSex(MemberInfo.GetSex(0));
			ModifyMember.SetUsername(MemberInfo.GetUserName(0));

			ModifyMember.SetIdType(MemberInfo.GetIdType(0));
			ModifyMember.SetPersonalId(MemberInfo.GetPersonalId(0));
			ModifyMember.SetOldPersonalId(MemberInfo.GetPersonalId(0));
			ModifyMember.SetOpenNetBarId(MemberInfo.GetNetBarId(0));
			ModifyMember.SetPassword(Balance.GetPassword());
			ModifyMember.SetMemberName(MemberInfo.GetMemberName(0));
			ModifyMember.SetCountryId(_T("CN"));
			ModifyMember.SetAddress(netBarUser.GetAddr());
			ModifyMember.SetIdDepart(netBarUser.GetDepartment());
			ModifyMember.SetJob(netBarUser.GetNation());

			ModifyMember.SetTel(strTel);

			theApp.GetCurCashier()->DoModifyMember(ModifyMember);

			bUpdateDataBase = TRUE;
			if (ModifyMember.GetStatusCode() != 0)
			{
				IBA_LOG(_T("�޸��û�����ʧ��:%s"), ModifyMember.GetStatusMessage());
			}
		}
		if(bUpdateDataBase)
		{
			CIBADAL::GetInstance()->UpdateMemberInfo(nMemberID,
				strNation, strDepart, strAddress, strTel);
		}	
	}

	//��������˻�= �ֽ�� + �û����ͽ�� + ��ǰ��ֵ�Ľ�� + ����Ԥ����
	INT nAllMoney = Balance.GetTotalBalacne();

	if (Balance.GetNeedActivation() == 1)
	{
		if (CServletHellper::ActivationMember(nMemberID)) //����ɹ�
		{
			//��ɾ���������½��棬�����Ǳ�֤������ʾ����ǰ��
			CLocalServer::GetInstance()->ActiveMemberList.RemoveActiveMember(Balance.GetMemberId());
			CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(FALSE);

			CLocalServer::GetInstance()->ActiveMemberList.AddActiveMember(Balance);
			CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(FALSE);

			CIBATTS::GetInstance()->SpeakActivation(nAllMoney, SpeakBirthday(m_NetBarUser));

			return TRUE;
		}
	}

	CIBATTS::GetInstance()->SpeakBalance(nAllMoney);

	return FALSE;
}
//
INT CRealNameBase::OnOldUser(CNetBarUser & NetBarUser)
{
	INT nRes = 0;
	CIBALog::GetInstance()->Write(_T("CRealNameBase::OnOldUser"));

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

	BOOL bExists =  CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(
		NetBarUser.GetMemberId(), ActiveMember);

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
				dlg.DoModal();
			}
			else
			{
				CNDReturnDlg dlg;
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

INT CRealNameBase::OnNewUser(CNetBarUser & NetBarUser)
{
	//if (CRegisterDlg::m_StaticPrehWnd != NULL) //ע�ᴰ���Ѿ�����
	//{
	//	SendMessage(CRegisterDlg::m_StaticPrehWnd, WM_RECEIVECARD, NULL, (LPARAM)&NetBarUser);
	//	return;
	//}
	
	if (AfxGetMainWnd()->IsIconic())
	{
		AfxGetMainWnd()->ShowWindow(SW_MAXIMIZE);
		AfxGetMainWnd()->BringWindowToTop();
	}
	
	CRegisterDlg dlg;
	dlg.SetUserInfo(NetBarUser);
	if (dlg.DoModal() == IDOK)
	{
		return 1;
	}
	return 0;
}

BOOL CRealNameBase::ReadUserInfo(CNetBarUser & NetBarUser)
{
	CString strKey(_T("RealName\\Base\\NewUser"));

	CString strTmp = theApp.GetProfileString(strKey, _T("IDNo"));

	strTmp.Trim();

	if (strTmp.IsEmpty())
	{
		return FALSE;
	}

	NetBarUser.SetNetId(strTmp);

	strTmp = theApp.GetProfileString(strKey, _T("Name"));
	strTmp.Trim();
	strTmp.Remove(' ');
	NetBarUser.SetUserName(strTmp);

	strTmp = theApp.GetProfileString(strKey, _T("CertificateID"));
	NetBarUser.SetCertificateID(strTmp);

	strTmp = theApp.GetProfileString(strKey, _T("Password"));
	NetBarUser.SetPassword(strTmp);

	INT nTmp = theApp.GetProfileInt(strKey, _T("CertificateType"), 11);
	NetBarUser.SetCertificateType(nTmp);
	
	// 2013-12-18 18:18:55-qsc
	strTmp = theApp.GetProfileString(strKey, _T("Nation"));//����
	NetBarUser.SetNation(strTmp);

	CString strMan;
	strMan.LoadString(IDS_MAN);
	strTmp = theApp.GetProfileString(strKey, _T("Sex"));
	if (strTmp.CompareNoCase(strMan) != 0)
	{
		NetBarUser.SetSex(1);	 //Ů��
	}
	else
	{
		NetBarUser.SetSex(0);	// �е�
	}
	
	strTmp = theApp.GetProfileString(strKey, _T("Address"));//��ַ
	NetBarUser.SetAddr(strTmp);
	strTmp = theApp.GetProfileString(strKey, _T("Department"));//��֤����
	NetBarUser.SetDepartment(strTmp);

	//strTmp = theApp.GetProfileString(strKey, _T("Signdate"));//ǩ������
	//strTmp = theApp.GetProfileString(strKey, _T("OutOfDate"));	 //��Ч��
	//strTmp = theApp.GetProfileString(strKey, _T("Birthday"));	//��������

	NetBarUser.SetDataFrom(1);

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

BOOL NS_REALNAME::CRealNameBase::OnLocalRealName(UINT nMsgType, CString strNetID)
{
	IBA_LOG0(_T("OnLocalRealName..."));
	CActiveMember ActiveMember;
	if (!CLocalServer::GetInstance()->ActiveMemberList.GetActiveMemberNetID(strNetID, ActiveMember, FALSE))
	{
		if (m_NetBarUser.GetCertificateID().IsEmpty())
		{
			IBA_LOG0(_T("New user! ʧ�ܣ�֤������Ϊ��!"));
			return TRUE;
		}
		IBA_LOG0(_T("Net user!"));
		OnNewUser(m_NetBarUser);
		
		return TRUE;
	}
	IBA_LOG0(_T("Old user!"));
	CNDReturnDlg dlgRetun;
	dlgRetun.SetNetId(ActiveMember.GetNetId());
	dlgRetun.DoModal();
	return TRUE;
}

BOOL NS_REALNAME::CRealNameBase::DoHasLocalRealName(UINT nMsgType,LPVOID lpData /*= NULL*/, DWORD dwLen /*= 0*/)
{
	IBA_LOG(_T("CRealNameBase::DoHasLocalRealName, ������ʵ��ˢ��, msg=%d"), nMsgType);

	m_NetBarUser.SetDeleteKey(FALSE);
	CString strNetId;
	switch(nMsgType)
	{
	case 99: // �Զ���
		strNetId = *((CString*)lpData);
		break;
	case 999:
		if (!ReadUserInfo(m_NetBarUser))
		{
			IBA_LOG0(_T("ReadUserInfo ʧ��"));
			return FALSE;
		}
		strNetId = m_NetBarUser.GetNetId();
		break;
	case 0:
		if (!ReadUserInfo(m_NetBarUser))
		{
			IBA_LOG0(_T("ReadUserInfo ʧ��"));
			return FALSE;
		}
		strNetId = m_NetBarUser.GetNetId();
		break;
	default:
		IBA_ASSERT2(0,"δ֪λ��ʵ����Ϣ����");
		IBA_LOG(_T("δ֪ʵ������%d"), nMsgType);
		return FALSE;
		break;
	}
	
	if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		OnLocalRealName(nMsgType, strNetId);
		return TRUE;
	}
	else
	{
		if (CLocalServer::GetInstance()->ActiveMemberList.GetUserStateByNetId(strNetId) == CIBAGlobal::emUserStateLocal)
		{
			OnLocalRealName(nMsgType, strNetId);
			return TRUE;
		}
	}

	return FALSE;
}
INT CRealNameBase::OnRealNameMsg(UINT nMsgType, LPVOID lpData /*= NULL*/, DWORD dwLen /*= 0*/)
{
	UNIT_TRACE("CRealNameBase::OnRealNameMsg(...)");
	IBA_LOG0(_T("CRealNameBase::OnRealNameMsg(...)"));

	if (DoHasLocalRealName(nMsgType, lpData, dwLen))
	{
		return -1;
	}

	if (99 == nMsgType) //�Զ���
	{
		if (lpData != NULL)
		{
			CString* pStr = (CString*)lpData;

			CheckCard(pStr->GetString());

			if (m_nMemberId > 0)
			{
				m_NetBarUser.SetMemberId(m_nMemberId);

				// 2011/05/17-gxx: 
				m_NetBarUser.SetNetId(*pStr);

				QueryBalance();
				OnOldUser(m_NetBarUser);
			}
		}
	}
	else if (999 == nMsgType)
	{
		ReadUserInfo(m_NetBarUser);

		m_NetBarUser.GetNetId();

		if (!CheckCard(m_NetBarUser.GetNetId()))
		{
			return 0;
		}

		if (m_nMemberId > 0) //�Ͽ����ȼ���
		{
			m_NetBarUser.SetMemberId(m_nMemberId);

			QueryBalance();

			OnOldUser(m_NetBarUser);
		}
		else //�¿����Ʒ�δע��
		{
			// 2013-12-6 15:31:50 qsc
			if (AutoMatchIDCard(m_NetBarUser))
			{
				AfxGetMainWnd()->BringWindowToTop();
				AfxGetMainWnd()->SetForegroundWindow();

				OnOldUser(m_NetBarUser);
			}
			else
			{
				OnNewUser(m_NetBarUser);
			}
		}
	}

	return 0;
}

//������ˢ��
void CRealNameBase::OnCardIn()
{
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
		ULONG lRet = wInit(0);

		if (MSG_OK == lRet)
		{
			//���ý�����Ϣ���壬
			//ע�⣬����������һ���߳��ڲ��ϵĽ���ɨ�裬���������Զ�����Ϣ���Ӧ
			wSetRecvWnd(AfxGetMainWnd()->GetSafeHwnd());
			
			m_bOpenReader = TRUE;			
		}
		else
		{
			CIBALog::GetInstance()->WriteFormat(_T("Can't open the reader! wInit Return = %d"), lRet);
		}
	}

	if (!m_DataXReceiver.Create(_T("FC96DB2B-5188-4419-883F-DAB7D412555B")))
	{
		CIBALog::GetInstance()->WriteFormat(_T("Create DataXReceiver failed"));
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
	
	strTmpPath.Append(_T("ID.jpg"));
	//IBA_LOG(_T("���õ�ַ%s"), strTmpPath);

	if(PathFileExists(strTmpPath))
	{
		if(!DeleteFile(strTmpPath))
		{
			IBA_LOG(_T("ɾ��%sʧ�ܣ�,%d"), strTmpPath, GetLastError());
		}
	}

	CIBAScanReco ScanReco;
	//static CIBAScanReco ScanReco;

	if (ScanReco.Open() == 1)
	{
		INT nRet = 0, nIdType = 11;

		if (1 == nType)
		{
			if (ScanReco.ScanImage(10, strTmpPath) == 1)
			{
				nRet = ScanReco.Recognise(10, _T(""));
			}
		}
		else if (2 == nType)
		{
			//qsctt
			//int nTick1 = GetTickCount();
			if (ScanReco.ScanImage(11, strTmpPath) == 1)
			{
				//int nTick2 = GetTickCount();
				//TRACE("\nScanImage:%d\n", nTick2 - nTick1);
				nRet = ScanReco.Recognise(11, _T(""));
				//TRACE("\nRecognise:%d\n", GetTickCount() - nTick2);
			}

			nIdType = 12;
		}
		else if (3 == nType)
		{
			nRet = ScanReco.ReadCard(strTmpPath);

			nIdType = 12;
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


BOOL CRealNameBase::CheckCard(LPCTSTR lpszNetId)
{
	if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		return TRUE;
	}

	// qscadd 2013-10-25
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
	else if (IDCheckCard.GetStatusCode() == 41) //���Ų�����
	{
	}
	else
	{
		if (IDCheckCard.GetStatusCode() != -1)
		{
			//ע��,���Ͽ�û���û���
			theApp.IBAMsgBox(IDCheckCard.GetStatusMessage());
		}	
	}

	m_nCardId = 0;
	m_nMemberId = 0;

	return FALSE;
}

BOOL CRealNameBase::QueryBalance()
{
	UNIT_AUTO_TRACE("CRealNameBase::QueryBalance()--");

	CCurrentNetBarUser::GetInstance()->Balance.SetMemberId(m_nMemberId);
	CCurrentNetBarUser::GetInstance()->Balance.SetCardId(m_nCardId);
	CCurrentNetBarUser::GetInstance()->Balance.SetRound(0);//ȡ��

	theApp.GetCurCashier()->DoBalance(CCurrentNetBarUser::GetInstance()->Balance);

	if (CCurrentNetBarUser::GetInstance()->Balance.GetStatusCode() == 0)
	{
		theApp.UpdateToolbarLastID( 
			CCurrentNetBarUser::GetInstance()->Balance.GetSerailNum());

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

BOOL CRealNameBase::AutoMatchIDCard(CNetBarUser & NetBarUser)
{   
	//��ͨģʽ
	if (CNetBarConfig::GetInstance()->GetAutoMatchIDCard() == 0) return FALSE;

	//�����֤
	if (NetBarUser.GetNetId().GetLength() < 15) return FALSE;

	//�������֤��Ӧ���˺�
	CMemberInfoMethod MemberInfo;

	MemberInfo.SetPersonalId(NetBarUser.GetNetId());
	MemberInfo.SetAvailable(1);

	theApp.GetCurCashier()->DoMemberInfo(MemberInfo);

	if (MemberInfo.GetStatusCode() != 0) return FALSE;

	if (MemberInfo.GetMemberCount() == 0) return FALSE;

	CheckCard(MemberInfo.GetSerailNum(0));

	if (m_nMemberId > 0)
	{
		CString strNewNetId = NetBarUser.GetNetId();//����������
		NetBarUser.SetMemberId(m_nMemberId);
		NetBarUser.SetNetId(MemberInfo.GetSerailNum(0));

		if (QueryBalance())
		{
			//�����֤ת�˺�
			if (ChangeNetIdToPersonalId(strNewNetId))
			{
				NetBarUser.SetNetId(strNewNetId);

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
BOOL CRealNameBase::ChangeNetIdToPersonalId(CString strNewNetId)
{	
	//����û����
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

	//����CardId
	CIDCheckCardMethod checkCard;
	checkCard.SetSerialNum(strNewNetId);
	theApp.GetCurCashier()->DoCheckCard(checkCard);

	if (checkCard.GetStatusCode() == 0)
	{
		//�޸��˺�
		CCardReplaceMethod CardReplace;
		CardReplace.SetPersonalId(CCurrentNetBarUser::GetInstance()->Balance.GetPersonalId());//֤������
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

