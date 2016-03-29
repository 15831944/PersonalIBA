#include "stdafx.h"
#include "Proxy.h"
#include "..\IBAHelpper.h"
#include "..\IBA.h"
#include ".\zdrealname.h"
#include "..\RealName\IZd.h"
#include "..\ServletHellper.h"
#include "..\CurrentNetBarUser.h"
#include "..\DAL\IBADAL.h"
#include "..\Socket\LocalServer.h"
#include "..\ComputerListView.h"
#include "..\IBADialog.h"
#include "..\DlgRNZdCrc.h"

using namespace NS_REALNAME;

CZDRealName::CZDRealName(void)
{
	CIBALog::GetInstance()->Write(_T("CZDRealName"));
}

CZDRealName::~CZDRealName(void)
{

}

BOOL CZDRealName::InitRealName()
{
	CRealNameBase::InitRealName();

	// ����ɾ��һЩ��������
	CIBADAL::GetInstance()->DelZdRnOutDateData();

	BOOL bRet = FALSE;
	
	if(CIZd::GetInstance()->IsLoad())
	{
		CIZd::IC_CRC_RE icCRCRe;
		DWORD dwCRC = CIBAConfig::GetInstance()->GetZdCrc();
		
		if(0 == dwCRC)
		{
			CDlgRNZdCrc dlg;
			dlg.DoModal();
			dwCRC = dlg.GetCRC();
		}
		
		icCRCRe = CIZd::GetInstance()->m_pfn_V2_SetCRC(dwCRC, CT2A(CIBALog::GetInstance()->GetAppVer()));

		if(0 == icCRCRe.iRe)
		{
			IBA_LOG(_T("CRCУ��ɹ�"));	// 2014-8-28 - qsc
			CIZd::GetInstance()->SetInit(TRUE);
			bRet = TRUE;

			CIBAConfig::GetInstance()->SetZdCrc(dwCRC);
			CIBAConfig::GetInstance()->SaveConfig();	// �ȱ���һ��
		}
		else
		{
			CIBAConfig::GetInstance()->SetZdCrc(0);	// ʧ�����
			CIBAConfig::GetInstance()->SaveConfig();	

			CIZd::GetInstance()->SetInit(FALSE);
			bRet = FALSE;

			CString strShow;
			if(-1 == icCRCRe.iRe)
			{
				strShow = _T("CRCУ�����");
			}
			else if(-2 == icCRCRe.iRe)
			{
				strShow = _T("�汾�Ŵ���");
			}
			CIBAHelpper::MsgBox(AfxGetMainWnd(), strShow, MB_OK);
		}
	}

	return bRet;
}

BOOL CZDRealName::UnInitRealName()
{
	CRealNameBase::UnInitRealName();


	return TRUE;
}

INT CZDRealName::OnRealNameMsg(UINT nMsgType, LPVOID lpData /*= NULL*/, DWORD dwLen /*= 0*/)
{
#ifdef _DEBUG
	CRealNameBase::OnRealNameMsg(nMsgType, lpData, dwLen);
#endif
	IBA_LOG0(_T("CZDRealName::OnRealNameMsg"));		// 2014-8-28 - qsc
	// 2014-8-28 - qsc
	if(!CIZd::GetInstance()->IsInit())
	{
		IBA_LOG0(_T("δ��ʼ��"));
		return 0;
	}
	UNIT_AUTO_TRACE("CZDRealName::OnRealNameMsg(UINT nMsgType=%d, LPVOID lpData=%x, DWORD dwLen=%d)"
		,nMsgType,lpData,dwLen);

	COPYDATASTRUCT *pCopyDataStruct = (COPYDATASTRUCT *)lpData;

	DWORD dwOpera = pCopyDataStruct->dwData;

	switch (dwOpera)
	{
	case IK_ID_OPENUSER:	// ʵ��������
	case IK_ID_CLOSEUSER:	//ʵ�����ػ�
		{
			CIZd::IC_USER iUser;
			ZeroMemory(&iUser, sizeof(CIZd::IC_USER));
			memcpy(&iUser, pCopyDataStruct->lpData, pCopyDataStruct->cbData);

			CIZd::T_IC_USER tiUser;
			CIZd::Conver_IC_USER(&iUser, &tiUser);

			CRealNameBase::OnRealNameMsg(nMsgType, tiUser.szikIDNo, sizeof(tiUser.szikIDNo));
			OnOpenCloseUser(dwOpera, &tiUser);
		}
		break;
	case IK_ID_LOGINOUT:	//ʵ�������� Ҳ���ÿͻ����ϻ�
	case IK_ID_LOGININ:	//ʵ�������� Ҳ���ÿͻ�������
		{
			CIZd::IC_LOGIN iLogin;
			ZeroMemory(&iLogin, sizeof(CIZd::IC_LOGIN));
			memcpy(&iLogin, pCopyDataStruct->lpData, pCopyDataStruct->cbData);

			CIZd::T_IC_LOGIN tiLogin;
			CIZd::Conver_IC_LOGIN(&iLogin, &tiLogin);	
			OnLoginOut(dwOpera, &tiLogin);
		}
		break;
	
	case IK_ID_GETIDCARDDATA:
		/*CIZd::IC_CARDDATA icCardData;
		CIZd::ID_RETURN idReturn = CIZd::GetInstance()->m_pfn_V2_SendICCardData();
		if(0 == idReturn.iErrCode)
		{
			IBA_LOG0(_T("m_pfn_V2_SendICCardData �ɹ�"));
		}
		else
		{
			IBA_LOG(_T("m_pfn_V2_SendICCardData ʧ��:%s"), idReturn.szErrMsg);
		}*/

		break;
	default:
		break;
	}
	

	return 0;
}

BOOL CZDRealName::OnOpenCloseUser(int nType, CIZd::T_IC_USER* pICUser)
{
	IBA_LOG(_T("�Ƕഫ�ݿ��ػ���Ϣ:ʵ������� %s, ���� %s, ֤������ %d, ֤������ %s, ��������ȡ���� %d, �ֻ��� %s , ��������Ƿ�ɹ� %d"),
		pICUser->szikIDNo, pICUser->szikName, pICUser->iCertType, pICUser->szCertNumber, pICUser->iCardType, pICUser->szMobile, pICUser->iValidate);

	CNetBarUser NetBarUser;
	NetBarUser.SetNetId(pICUser->szikIDNo);
	NetBarUser.SetCertificateID(pICUser->szCertNumber);
	NetBarUser.SetCertificateType(CIZd::SwitchZd2Ddn(pICUser->iCertType));
	NetBarUser.SetUserName(pICUser->szikName);
	NetBarUser.SetSex(0);
	NetBarUser.SetTel(pICUser->szMobile);

	if (!CheckCard(NetBarUser.GetNetId()))
	{
		return 0;
	}

	//ReplyMessage(0);

	if (m_nMemberId > 0) //�Ͽ����ȼ���
	{
		NetBarUser.SetMemberId(m_nMemberId);
		if(IK_ID_OPENUSER == nType)
		{
			if (QueryBalance())
			{	
				if (CDeregisterHelpper::AllowAutoRefund(CCurrentNetBarUser::GetInstance()->Balance)) //�����Զ��˿�����
				{
					if (CDeregisterHelpper::DeregisterUser(CCurrentNetBarUser::GetInstance()->Balance))//�Զ��˿���
					{
						//��������û�����ݴ������ǣ����������û�����


						OnNewUser(NetBarUser);

						return 0; 
					}
				}
			}

			OnOldUser(NetBarUser);
			AutoChangeRealNamePwd(NetBarUser);
		}
		else if(IK_ID_CLOSEUSER == nType)
		{
			OnCloseUser(NetBarUser);
		}
	}
	else //�¿����Ʒ�δע��
	{
		if(IK_ID_OPENUSER == nType)
		{
			OnNewUser(NetBarUser);
		}
		else if(IK_ID_CLOSEUSER == nType)
		{
			// �ÿ������ڣ������κβ���
		}
		
	}
	return 0;
}

BOOL CZDRealName::OnLoginOut(int nType, CIZd::T_IC_LOGIN* ptLogin)
{
	IBA_LOG(_T("�Ƕഫ�ݿ��ػ���Ϣ:�û��İ󶨿���� %s, ʵ������� %s, �ͻ���IP��ַ %s, �ͻ������� %s"),
		ptLogin->szikICNo, ptLogin->szikIDNo, ptLogin->szikIP, ptLogin->szikHost);
	
	if (!CheckCard(ptLogin->szikICNo))
	{
		return FALSE;
	}
	
	if(m_nMemberId <= 0)
	{
		return FALSE;
	}

	if(IK_ID_LOGININ == nType)
	{
		CheckIn(ptLogin->szikHost, ptLogin->szikIP, ptLogin->szikICNo, ptLogin->szicPassword, m_nMemberId);
	}
	else if(IK_ID_LOGINOUT == nType)
	{
		INT_PTR nIndex = CLocalServer::GetInstance()->ComputerList.Find(m_nMemberId);

		if (nIndex >= 0)
		{	
			CLocalServer::GetInstance()->LocalCheckOutUser(nIndex, m_nMemberId, 2);

			Sleep(500);
		}

		CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(FALSE);
	}
	

	return TRUE;
}

BOOL CZDRealName::CheckRealName()
{
	CRealNameBase::CheckRealName();


	return TRUE;
}

BOOL CZDRealName::IsRegisterEnabled()
{
	return TRUE;
}

BOOL CZDRealName::IsCheckInEnabled()
{
	if(CIZd::GetInstance()->IsInit())
	{
		return TRUE;
	}
	else 
	{
		return FALSE;
	}
}

INT CZDRealName::OnCloseUser(CNetBarUser & NetBarUser)
{
	CIBALog::GetInstance()->Write(_T("CZDRealName::OnCloseUser"));

	if (NetBarUser.GetMemberId() > 0)//�Ѿ�����
	{	
		if (QueryBalance())
		{	//�����Զ��˿�����
			if (CDeregisterHelpper::AllowAutoRefund(CCurrentNetBarUser::GetInstance()->Balance))
			{
				if (CDeregisterHelpper::DeregisterUser(CCurrentNetBarUser::GetInstance()->Balance))
				{
					return 0; //�Զ��˿���
				}
			}

			if (CCurrentNetBarUser::GetInstance()->Balance.GetNeedActivation() == 0) 
			{
				CIBALog::GetInstance()->Write(_T("�������������"));

				AfxGetMainWnd()->PostMessage(WM_EXECUTEREALNAME);

				return 1;//����Ҫ�����ʾ�Ѿ��������,ֱ�ӷ���һ�Ϳ���
			}
		}
	}
	else//δע��
	{
		//���ݶ���֤��������Ӧ��ʵ����������ֱ�Ӹ��������˺� 
		
		if (AutoMatchIDCard(NetBarUser))
		{
			if (CCurrentNetBarUser::GetInstance()->Balance.GetNeedActivation() == 0)
			{
				AfxGetMainWnd()->PostMessage(WM_EXECUTEREALNAME);

				return 1;//����Ҫ�����ʾ�Ѿ��������,ֱ�ӷ���һ�Ϳ���
			}
		}
	}

	return 0; //ֻ�����߷���1������ȫ������0
}

INT CZDRealName::OnOpenUser(CNetBarUser & NetBarUser)
{
	CIBALog::GetInstance()->Write(_T("CZDRealName::OnOpenUser"));

	if (NetBarUser.GetMemberId() > 0)
	{
		// 2011/09/05-8201-gxx: ��OnOpenUser���жϵ�ǰˢ���û����û����Ƿ��ǰһ�β����ʱ���û���һ�£������һ�£���ô�������²�һ�����
		if (NetBarUser.GetMemberId() != CCurrentNetBarUser::GetInstance()->Balance.GetMemberId())
		{
			QueryBalance();
		}

		//�����Ϊʵ������
		//AutoChangeRealNamePwd(NetBarUser);

		if (AutoModifyMemberInfo()) //��������
		{
			QueryBalance();
		}

		// 2011/11/29-8211-gxx: 
		int nRes = OnOldUser(NetBarUser);//���û�
		AutoChangeRealNamePwd(NetBarUser);
		IBA_LOG(_T("OnOpenUser Active return %d"),nRes);
		return nRes;

	}
	else 
	{
		if (AutoMatchIDCard(NetBarUser)) //�ҵ�ƥ�������
		{
			// 2011/11/29-8211-gxx: 
			int nRes = OnOldUser(NetBarUser);//���û�
			AutoChangeRealNamePwd(NetBarUser);
			IBA_LOG(_T("OnOpenUser Active return %d"),nRes);
			return nRes;
		}
		else
		{
			// 2011/11/25-8211-gxx: �ĳ��з���ֵ
			INT nRes = OnNewUser(NetBarUser);
			IBA_LOG(_T("OnOpenUser Register return %d"),nRes);
			return nRes;
		}
	}

	// 2011/11/25-8211-gxx: ����1�ĳɷ���0
	IBA_LOG0(_T("OnOpenUser return 0"));

	return 0;

}

BOOL CZDRealName::CheckTakeUp(CString strTermId)
{
	//��ѯ���ն��Ƿ�ռ��
	BOOL bTakeUp = FALSE;

	CLastUserInfo LastUserInfo;

	if (CIBADAL::GetInstance()->GetLastUserInfo(LastUserInfo, 0, strTermId))
	{
		if (LastUserInfo.GetSuspend())//�һ�
		{
			bTakeUp = TRUE;
		}
		else//�ǹһ�
		{
			COleDateTime updateTime;
			updateTime.ParseDateTime(LastUserInfo.GetUpdateTime());
			COleDateTimeSpan interval = COleDateTime::GetCurrentTime() - updateTime;

			if (interval.GetTotalMinutes() < 10)//��ռ�ã������û�����Ч
			{
				bTakeUp = TRUE;
			} 
		}
	}

	return bTakeUp;
}


void CZDRealName::CheckoutOldComputer(const CString& strTermID, int nMemberId)
{
	if (strTermID.IsEmpty())
	{
		return;
	}

	INT_PTR nIdx = CLocalServer::GetInstance()->ComputerList.LookFor(0, strTermID);

	if (nIdx < 0)
	{
		return;
	}

	IBA_LOG(_T("%s �����»�"), strTermID);
	CLocalServer::GetInstance()->LocalCheckOutUser(nIdx, nMemberId, 1);

	Sleep(1000);

	CLocalServer::GetInstance()->GetComputerListView()->Refresh(TRUE);
}

BOOL CZDRealName::CheckIn(LPCTSTR szTermID, LPCTSTR szComputerIP, LPCTSTR szNetId, LPCTSTR szPwd, int nMemberId)
{
	INT_PTR nIdx = CLocalServer::GetInstance()->ComputerList.Find(szTermID, szComputerIP);
	CComputerInfo & computer = CLocalServer::GetInstance()->ComputerList.GetComputer(nIdx);

	if (CheckTakeUp(computer.GetTerminalID())) //ռ����
	{
		CIBAHelpper::MsgBox(AfxGetMainWnd(), IDS_CHANGECOMPUTERERROR);
		return FALSE;
	}
	
	//��ӵ����ݿ�
	CLastUserInfo LastUserInfo;
	LastUserInfo.SetNetId(szNetId);
	LastUserInfo.SetMemberId(nMemberId);
	LastUserInfo.SetTermId(computer.GetTerminalID());
	LastUserInfo.SetLoginStyle(1/*GetCheckIn() ? 1 : 0*/); //�Զ���¼Ҫ��ǿ�Ƶ�¼����
	LastUserInfo.SetUpdateTime(CIBAHelpper::GetCurrentTimeAsString(TRUE));

	// �����û�
	CString strTmp = CIBAHelpper::EncodePassword(szPwd);
	LastUserInfo.SetPassWord(strTmp);

	CIBADAL::GetInstance()->DeleteLastUserInfo(LastUserInfo.GetTermId());
	CIBADAL::GetInstance()->AddLastUserInfo(LastUserInfo);


	//�����¼
	
	if (computer.GetComputerStatus() == CComputerInfo::ECS_LOCKED)
	{
		CLocalServer::GetInstance()->LocalAutoCheckIn(nIdx);
	}
	else
	{
		IBA_LOG0(_T("�Զ�����"));
		CLocalServer::GetInstance()->LocalStartupComputer(nIdx);
	}

	CIBAHelpper::MsgBoxFormat(AfxGetMainWnd(), MB_OK, IDS_CHANGECOMPUTERSUCCESS, LastUserInfo.GetTermId());
	return TRUE;
}

void NS_REALNAME::CZDRealName::OnCheckOut( const CheckoutInfo* pCheckoutInfo )
{
	IBA_LOG0(_T("OnCheckOut Enter"));
	// ������̨�����������
	if (CIBAConfig::GetInstance()->GetCashRegisterID() == 1)
	{
		if(-1 == pCheckoutInfo->nCheckoutType)
		{
			IBA_LOG0(_T("�쳣����"));
			ASSERT(FALSE);
		}
		else
		{
			// ���ǻ���
			if(CIBAGlobal::cot_Change != pCheckoutInfo->nCheckoutType)
			{
				
				CStringA strId = CT2A(pCheckoutInfo->szNetId);

				CStringA strTermId = pCheckoutInfo->szTermId;
				strTermId.Trim();

				// ����̨�Ľ�����Ϣ��ֻ���������Ϣ���������»���Ϣ
				// �������̨�Ľ�����Ϣ���ն�ID��Ϊ�գ��Ͳ����������ȿͻ��˷���������Ϣ�ٲ���(����ʧ�ܵ������Ҫ�����ն˲�Ϊ�յ�)

				if(pCheckoutInfo->bClientCheckOut)
				{
					IBA_LOG0(_T("�ͻ��˽�����Ϣ"));
					CIZd::GetInstance()->V2_Logout(strId, strId, CT2A(pCheckoutInfo->szIP));
					CIZd::GetInstance()->V2_CloseUser(strId, strId,
						CT2A(pCheckoutInfo->szIP), CT2A(pCheckoutInfo->szTermId));
					CIBADAL::GetInstance()->DelZdRnInfo(CA2T(strId));
				}
				else
				{
					if(strTermId.IsEmpty())
					{
						IBA_LOG0(_T("����̨δ�ϻ�������Ϣ"));
						CIZd::GetInstance()->V2_CloseUser(strId, strId,
							CT2A(pCheckoutInfo->szIP), CT2A(pCheckoutInfo->szTermId));
						CIBADAL::GetInstance()->DelZdRnInfo(CA2T(strId));
					}
					else
					{
						IBA_LOG0(_T("����̨����������Ϣ"));
						// 1. ����̨������Ϣ���ն˺����ݿ��ն˲�һ�£�˵���ǻ���ʧ����
						// �����ݿ��Ϊ׼

						// 2. �ն���ͬ�������п����ǿͻ���ֱ�ӻ�����ֻ����������Ȼ��ֱ������̨���ˣ��ն˺Ŵ���
						CString strDBTermId;
						COleDateTime oleTime;
						CIBADAL::GetInstance()->GetZdRnInfo(CA2T(strId), strDBTermId, oleTime);	

						bool bHaveChangeLogout = false;
						if(0 != strDBTermId.CompareNoCase(pCheckoutInfo->szTermId))
						{
							bHaveChangeLogout = true;
						}
						else		
						{
							int HaveChange = 0;
							CIBADAL::GetInstance()->GetHaveChange(pCheckoutInfo->szNetId, HaveChange);
							if(HaveChange)
							{
								bHaveChangeLogout = true;
							}
						}

						if(bHaveChangeLogout)
						{
							CString strIP;
							CIBADAL::GetInstance()->GetIPFromTermId(strDBTermId, strIP);

							CIZd::GetInstance()->V2_Logout(strId, strId, CT2A(strIP));
							CIZd::GetInstance()->V2_CloseUser(strId, strId,
								CT2A(strIP), CT2A(strDBTermId));
							CIBADAL::GetInstance()->DelZdRnInfo(CA2T(strId));
						}
					}
				}
			}
			else
			{
				CIBADAL::GetInstance()->SetHaveChange(pCheckoutInfo->szNetId, 1);
			}
		}	
	}
			
	IBA_LOG0(_T("OnCheckOut Leave"));
}

//������ĺ�ʵ���������Ƿ���ͬ
BOOL NS_REALNAME::CZDRealName::AutoChangeRealNamePwd(CNetBarUser& NetBarUser)
{	
	//���벻Ϊ�գ�����Ϊ��
	if (!CCurrentNetBarUser::GetInstance()->Balance.GetPassword().IsEmpty())
	{
		CResetPwdMethod ResetPwd;

		ResetPwd.SetMemberId(NetBarUser.GetMemberId());
		ResetPwd.SetPassword(NetBarUser.GetPassword());
		ResetPwd.SetPersonalId(CCurrentNetBarUser::GetInstance()->Balance.GetPersonalId());

		theApp.GetCurCashier()->DoResetPwd(ResetPwd);

		return FALSE;
	}

	return TRUE;
}