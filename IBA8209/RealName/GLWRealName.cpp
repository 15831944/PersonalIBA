#include "stdafx.h"
#include "..\IBA.h"
#include "..\ServletHellper.h"
#include "..\MainFrm.h"
#include "..\Socket\LocalServer.h"
#include "..\CurrentNetBarUser.h"
#include ".\glwrealname.h"
#include "..\Encrypt.h"
#include "..\GlobalDefine.h"
#include "..\GlobalFunctions.h"

using namespace NS_REALNAME;
//������
CGLWRealName::CGLWRealName(void)
{
	CIBALog::GetInstance()->Write(_T("CGLWRealName"));
}

CGLWRealName::~CGLWRealName(void)
{
}

BOOL CGLWRealName::InitRealName()
{
	CRealNameBase::InitRealName();

	ExecuteProxy();

	return TRUE;
}

INT CGLWRealName::OnRealNameMsg(UINT nMsgType, LPVOID lpData /*= NULL*/, DWORD dwLen /*= 0*/)
{
	UNIT_AUTO_TRACE("CGLWRealName::OnRealNameMsg(UINT nMsgType=%d, LPVOID lpData=%x, DWORD dwLen=%d)"
		,nMsgType,lpData,dwLen);

	if ( CRealNameBase::OnRealNameMsg(nMsgType, lpData, dwLen) == -1)
	{
		return 0;
	}

	if (0 == nMsgType)
	{
		CIBALog::GetInstance()->Write(_T("CGLWRealName::OnRealNameMsg"));

		//����0�ͻ��ٷ�OPENUSER

		if (!ReadUserInfo(m_NetBarUser))//��ע����ȡ��Ϣ
		{
			CIBALog::GetInstance()->Write(_T("ReadUserInfo Error!"));
			return 0;
		}

		// 2013-6-9 11:36:55 ��ӡ�������������Ϣ
		IBA_LOG(_T("������������Ϣ, �û�����%s, �����˺ţ�%s, ֤�����룺%s, ֤�����ͣ�%d"), 
			m_NetBarUser.GetUserName(), m_NetBarUser.GetNetId(), 
			m_NetBarUser.GetCertificateID(), m_NetBarUser.GetCertificateType());

		CString strNetId = m_NetBarUser.GetNetId();

		//{ 2011/08/01-8201-gxx: 
		if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
		{
			OnLocalRealName(nMsgType, strNetId);
			return 0;
		}
		else
		{
			CActiveMember ActiveMember;
			BOOL bGetOK = (CLocalServer::GetInstance()->ActiveMemberList.GetActiveMemberNetID(strNetId, ActiveMember));
			if (bGetOK && ActiveMember.GetIsLocalUser())
			{
				OnLocalRealName(nMsgType, strNetId);
				return 0;
			}
		}
		//}


		if (!CheckCard(m_NetBarUser.GetNetId()))
		{
			return 0;
		}

		m_NetBarUser.SetMemberId(m_nMemberId);//ȡ���û���

#ifdef GLW_FUNCTION_FUZHOU
		// ���ݵĹ������߱������Ĺ��ܣ�����Ϊ�յ���֤������Ҳ�ܿ���
		if (m_NetBarUser.GetUserName().GetLength() < 2 && m_NetBarUser.GetCertificateID().GetLength() > 1) {
			m_NetBarUser.SetUserName(IBAGlobal::MakeUserName());
		}
#endif
		
	
		if (m_NetBarUser.GetUserName().IsEmpty()) //�ػ���Ϣ
		{
			return OnCloseUser(m_NetBarUser);
		}
		else //������Ϣ
		{
			return OnOpenUser(m_NetBarUser);
		}

		//theApp.DelRegTree(HKEY_CURRENT_USER, _T("Software\\Sunlike\\IBA\\RealName\\GLW\\OpenUser"));
	}

	return 1;
}

INT CGLWRealName::OnCloseUser(CNetBarUser & NetBarUser)
{
	CIBALog::GetInstance()->Write(_T("CGLWRealName::OnCloseUser"));

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


INT CGLWRealName::OnOpenUser(CNetBarUser & NetBarUser)
{
	CIBALog::GetInstance()->Write(_T("CGLWRealName::OnOpenUser"));

	if (NetBarUser.GetMemberId() > 0)
	{
		// 2011/09/05-8201-gxx: ��OnOpenUser���жϵ�ǰˢ���û����û����Ƿ��ǰһ�β����ʱ���û���һ�£������һ�£���ô�������²�һ�����
		if (NetBarUser.GetMemberId() != CCurrentNetBarUser::GetInstance()->Balance.GetMemberId())
		{
			QueryBalance();
		}

		//�����Ϊʵ������
		AutoChangeRealNamePwd(NetBarUser);

		if (AutoModifyMemberInfo()) //��������
		{
			QueryBalance();
		}

		// 2011/11/29-8211-gxx: 
		int nRes = OnOldUser(NetBarUser);//���û�
		IBA_LOG(_T("OnOpenUser Active return %d"),nRes);
		return nRes;

	}
	else 
	{
		if (AutoMatchIDCard(NetBarUser)) //�ҵ�ƥ�������
		{
			// 2011/11/29-8211-gxx: 
			int nRes = OnOldUser(NetBarUser);//���û�
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

//������ĺ�ʵ���������Ƿ���ͬ
BOOL CGLWRealName::AutoChangeRealNamePwd(CNetBarUser& NetBarUser)
{	
	if (CNetBarConfig::GetInstance()->GetRealNamePassword() < 2)	//С��2��������
	{
		return TRUE;
	}

	//����������Ϊ��,������
	if (NetBarUser.GetPassword().IsEmpty())
	{
		CIBALog::GetInstance()->Write(_T("����������Ϊ�գ�����ʵ�����������Ƿ���ȷ��"));
		return TRUE;
	}

	//��������
	if (NetBarUser.GetPassword().Compare(CCurrentNetBarUser::GetInstance()->Balance.GetPassword()) != 0)
	{//���벻һ��
		CResetPwdMethod ResetPwd;

		ResetPwd.SetMemberId(NetBarUser.GetMemberId());
		ResetPwd.SetPassword(NetBarUser.GetPassword());
		ResetPwd.SetPersonalId(CCurrentNetBarUser::GetInstance()->Balance.GetPersonalId());

		theApp.GetCurCashier()->DoResetPwd(ResetPwd);

		return FALSE;
	}

	return TRUE;
}

//������ʵ�������û������Ƿ���ȫ��ͬ


INT CGLWRealName::ChangeCertificateType(INT nTmp)
{
	switch (nTmp)
	{
	case 111: nTmp = 11; break; //���֤

	case 113: nTmp = 93; break; //���ڱ�

	case 414: nTmp = 94; break; //����

	case 511: nTmp = 95; break; //̨��֤

	case 516: nTmp = 96; break; //����֤

	case 233: nTmp = 92; break; //ʿ��֤

	case 114: nTmp = 90; break; //����֤

	case 123: nTmp = 91; break; //����֤

  case 335: nTmp = 20; break; //��ʻ֤

	default:
		nTmp = 99; //����֤��
	}	
	
	return nTmp;
}

BOOL CGLWRealName::ReadUserInfo(CNetBarUser & NetBarUser)
{
	//�趨HKEY_CURRENT_USER\\software\\sunlike\\IBA\\�µķ��ʼ���֧RealName\\GLW\\OpenUser
	
	CString strKey(_T("RealName\\GLW\\OpenUser"));

	CString strTmp = theApp.GetProfileString(strKey, _T("IDNo"));

	strTmp.Trim();

	if (strTmp.IsEmpty())
	{
		return FALSE;
	}
	else
	{
		//// ��֤ˢ����Ϣ
		//CString strIDNo = theApp.GetProfileString(strKey, _T("IDNo"));
		//CString strTime = theApp.GetProfileString(strKey, _T("tValue1"));
		//CString strCode = theApp.GetProfileString(strKey, _T("tValue2"));
		//BYTE byCheck[8] = {0x03,0xF1,0x00,0x0A,0x0B,0xED,0x02,0xC1};
		//time_t tm = time(NULL);
		//int nTime = _ttol(strTime.GetString());

		//if (tm - nTime > 5* 60)
		//{
		//	AfxMessageBox(_T("��Ч��ˢ����Ϣ��Դ����ˢ����ʱ!"), MB_OK|MB_ICONERROR);
		//	IBA_LOG0(_T("ˢ����ʱ!"));
		//	return FALSE;
		//}

		//CStringA strOrgCode;
		//strOrgCode += CT2A(strIDNo);
		//strOrgCode += CT2A(strTime);
		//INT nLen = strOrgCode.GetLength();
		//BYTE *pBuff = new BYTE[nLen+8];
		//memcpy(pBuff,strOrgCode.GetString(), nLen);
		//memcpy(pBuff+nLen, byCheck, 8);
		//char pCode[33] = {0};

		//CEncrypt::CalcMD5Bytes(pBuff, nLen+8, pCode);

		//CStringA strCodeA = CT2A(strCode);
		//if (strCodeA.CompareNoCase(pCode) != 0)
		//{
		//	AfxMessageBox(_T("��Ч��ˢ����Ϣ��Դ!"), MB_OK|MB_ICONERROR);
		//	IBA_LOG0(_T("��Ч��ˢ����Ϣ��Դ!"));
		//	delete []pBuff;
		//	return FALSE;
		//}

		//delete []pBuff;
	}

	// �ж��Ƿ�Ϊָ��ʶ���ˢ��
	if (strTmp.GetLength() > 2) {
		if (strTmp.Left(2).CompareNoCase(_T("FN")) == 0) {
			// ��ָ��ʶ��ȥ��FN���������˺Ż�֤����
			NetBarUser.SetActivationType(1);
			strTmp = strTmp.Mid(2);
		}
	}
	NetBarUser.SetNetId(strTmp);

	strTmp = theApp.GetProfileString(strKey, _T("Name"));
	strTmp.Trim();
	strTmp.Remove(' ');
	NetBarUser.SetUserName(strTmp);

	strTmp = theApp.GetProfileString(strKey, _T("CertificateID"));
	// �ж��Ƿ�Ϊָ��ʶ���ˢ��
	if (strTmp.GetLength() > 2) {
		if (strTmp.Left(2).CompareNoCase(_T("FN")) == 0) {
			// ��ָ��ʶ��ȥ��FN���������˺Ż�֤����
			NetBarUser.SetActivationType(1);
			strTmp = strTmp.Mid(2);
		}
	}
	NetBarUser.SetCertificateID(strTmp);

	strTmp = theApp.GetProfileString(strKey, _T("Password"));
	NetBarUser.SetPassword(strTmp);

	INT nTmp = theApp.GetProfileInt(strKey, _T("CertificateType"), 111);
	NetBarUser.SetCertificateType(ChangeCertificateType(nTmp));
	
	NetBarUser.SetDataFrom(1);

	return TRUE;
}


INT CGLWRealName::OnExecuteMsg(UINT nExecuteType, LPVOID lpData /*= NULL*/, DWORD dwLen /*= 0*/)
{
	if (m_NetBarUser.GetMemberId() > 0)
	{
		OnOldUser(m_NetBarUser);
	}

	return 0;
}

BOOL CGLWRealName::IsMenuEnabled()
{
	return TRUE;
}


INT CGLWRealName::InvokeRealName(UINT nMsgType, LPVOID lpData /*= NULL*/, DWORD dwLen /*= 0*/)
{
	if (!theApp.GetCurCashier()->IsOnDuty())//����Ա�����ߣ����°࣬����
	{
		return FALSE;
	}

	CIBALog::GetInstance()->WriteFormat(_T("InvokeRealName: nMsgType = %d"), nMsgType);//��Ϣд��־

	switch (nMsgType) //1���п� 2��ʧ 3�޸����� 4�����Ա
	{
	case 1:
		return SendMsgToGLW(1);//����

	case 2:
		return SendMsgToGLW(2);//��ʧ

	case 3:
		return SendMsgToGLW(3);//�޸�����

	case 4:
		return SendMsgToGLW(4);//�����Ա
	}

	return FALSE;	
}

BOOL CGLWRealName::SendMsgToGLW(UINT nMsgType)
{
	HWND hSureName = ::FindWindow(NULL, _T("MessageForm"));

	if (hSureName != NULL)
	{
		//����̨6

		HWND hTmp = ::FindWindow(NULL, _T("����̨6"));

		::ShowWindow(hTmp, SW_MAXIMIZE);

		::BringWindowToTop(hTmp); 
	}
	else
	{
		hSureName = ::FindWindow(_T("TFrmPrepareOpenUser"), _T("Ԥ�����б�"));

		if (!IsWindow(hSureName))
		{
			AfxMessageBox(_T("�Ҳ�������������̨��"));

			return FALSE;
		}
	}

	#define UO_BASE WM_USER + 2000

	BOOL bSend = ::PostMessage(hSureName, UO_BASE, nMsgType, NULL);

	return bSend;
} 


BOOL CGLWRealName::NeedToCheckRegisterData()
{
	return TRUE;
}

BOOL CGLWRealName::OnCardInfo(DataX_UserInfo& dxUserInfo)
{
	CString strTmp = CA2T(dxUserInfo.cardNo);
	m_NetBarUser.SetNetId(strTmp);

	strTmp = CA2T(dxUserInfo.name);
	m_NetBarUser.SetUserName(strTmp);

	strTmp = CA2T(dxUserInfo.idNo);
	m_NetBarUser.SetCertificateID(strTmp);

	m_NetBarUser.SetCertificateType(ChangeCertificateType(dxUserInfo.idType));

	strTmp = CA2T(dxUserInfo.password);
	m_NetBarUser.SetPassword(strTmp);

	m_NetBarUser.SetDataFrom(1);

	if (!CheckCard(m_NetBarUser.GetNetId()))
	{
		return 0;
	}

	m_NetBarUser.SetMemberId(m_nMemberId);//ȡ���û���

	if (m_nMemberId > 0)
	{
		if (QueryBalance()) //�����Զ��˿�����
		{	
			if (CCurrentNetBarUser::GetInstance()->Balance.GetOnline() == 1)
			{
				ReplyMessage(2);
			}
			else if (CCurrentNetBarUser::GetInstance()->Balance.GetNeedActivation() == 0) 
			{				
				ReplyMessage(1); //����1��GLW
			}

			AfxGetMainWnd()->PostMessage(WM_EXECUTEREALNAME);
		}

		ReplyMessage(0);
	}
	else
	{
		//{ 2012/02/01-8237-gxx: 
		INT nRes = OnNewUser(m_NetBarUser);

		IBA_LOG(_T("CGLWRealName::OnCardInfo  send result:%d"), nRes);
		ReplyMessage(nRes);
		//}
		
	}

	return TRUE;
}

BOOL CGLWRealName::OnForceCheckOut(DataX_UserInfo& dxUserInfo)
{
	CString strCardNo = CA2T(dxUserInfo.cardNo);

	CString strTermId = CA2T(dxUserInfo.termId);

	CString strTermIp = CA2T(dxUserInfo.termIp);

	CIBALog::GetInstance()->WriteFormat(_T("CRealNameBase::OnForceCheckOut %s,%s,%s,%d"), 
		strCardNo, strTermId, strTermIp, dxUserInfo.dwReserve1);

	m_NetBarUser.SetNetId(strCardNo);

	if (!CheckCard(m_NetBarUser.GetNetId()))
	{
		return FALSE;
	}

	m_NetBarUser.SetMemberId(m_nMemberId);

	if (1 == dxUserInfo.dwReserve1) //�Ƿ��û���ǿ���߿�
	{
		INT_PTR nIndex = CLocalServer::GetInstance()->ComputerList.Find(m_nMemberId);

		if (nIndex >= 0) //Զ���»�
		{
			CComputerInfo& ComputerInfo = CLocalServer::GetInstance()->ComputerList.GetComputer(nIndex);

			if (ComputerInfo.GetTerminalID().CompareNoCase(strTermId) == 0 ||
				ComputerInfo.GetComputerIP().CompareNoCase(strTermIp) == 0)
			{
				CLocalServer::GetInstance()->LocalForceCheckOut(nIndex, _T("ϵͳ��������ϵ����֧����Ա"));
			}
		}
	
		CCheckout2Method Checkout2;
		Checkout2.SetSafeLogout(0); //�۵���ǰʱ��
		Checkout2.SetMemberId(m_nMemberId);
		theApp.GetCurCashier()->DoCheckout2(Checkout2);	

		return 1;
	}
	else
	{
		if (m_nMemberId > 0)
		{
			QueryBalance();

			if (CCurrentNetBarUser::GetInstance()->Balance.GetNeedActivation() == 0) 
			{
				CIBALog::GetInstance()->Write(_T("�������������"));

				AfxGetMainWnd()->PostMessage(WM_EXECUTEREALNAME);

				return 1;
			}
		}
	}

	return 2;
}
