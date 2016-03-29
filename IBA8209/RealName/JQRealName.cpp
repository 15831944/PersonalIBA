#include "stdafx.h"
#include "..\IBA.h"
#include "..\ServletHellper.h"
#include "..\MainFrm.h"
#include "..\Socket\LocalServer.h"
#include "..\CurrentNetBarUser.h"
#include ".\jqrealname.h"
#include "..\Encrypt.h"

using namespace NS_REALNAME;
//������
CJQRealName::CJQRealName(void)
{
	CIBALog::GetInstance()->Write(_T("CJQRealName"));
}

CJQRealName::~CJQRealName(void)
{
}

BOOL CJQRealName::InitRealName()
{
	CRealNameBase::InitRealName();

	ExecuteProxy();

	return TRUE;
}

INT CJQRealName::OnRealNameMsg(UINT nMsgType, LPVOID lpData /*= NULL*/, DWORD dwLen /*= 0*/)
{
	UNIT_AUTO_TRACE("CJQRealName::OnRealNameMsg(UINT nMsgType=%d, LPVOID lpData=%x, DWORD dwLen=%d)"
		,nMsgType,lpData,dwLen);

	if ( CRealNameBase::OnRealNameMsg(nMsgType, lpData, dwLen) == -1)
	{
		return 0;
	}

	if (0 == nMsgType)
	{
		CIBALog::GetInstance()->Write(_T("CJQRealName::OnRealNameMsg"));

		//����0�ͻ��ٷ�OPENUSER

		if (!ReadUserInfo(m_NetBarUser))//��ע����ȡ��Ϣ
		{
			CIBALog::GetInstance()->Write(_T("ReadUserInfo Error!"));
			return 0;
		}

		// 2013-6-9 11:36:55 ��ӡ�������������Ϣ
		IBA_LOG(_T("���Ŵ�����Ϣ, �û�����%s, �����˺ţ�%s, ֤�����룺%s, ֤�����ͣ�%d, ��ַ:%d, �Ա�:%d"), 
			m_NetBarUser.GetUserName(), m_NetBarUser.GetNetId(), 
			m_NetBarUser.GetCertificateID(), m_NetBarUser.GetCertificateType(),
			m_NetBarUser.GetAddr(), m_NetBarUser.GetSex());

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
	
		if (m_NetBarUser.GetUserName().IsEmpty()) //�ػ���Ϣ
		{
			return OnCloseUser(m_NetBarUser);
		}
		else //������Ϣ
		{
			return OnOpenUser(m_NetBarUser);
		}

		theApp.DelRegTree(HKEY_CURRENT_USER, _T("Software\\Sunlike\\IBA\\RealName\\JQ\\OpenUser"));
	}

	return 1;
}

INT CJQRealName::OnCloseUser(CNetBarUser & NetBarUser)
{
	CIBALog::GetInstance()->Write(_T("CJQRealName::OnCloseUser"));

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
				CIBALog::GetInstance()->Write(_T("�������ż��"));

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


INT CJQRealName::OnOpenUser(CNetBarUser & NetBarUser)
{
	CIBALog::GetInstance()->Write(_T("CJQRealName::OnOpenUser"));

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
BOOL CJQRealName::AutoChangeRealNamePwd(CNetBarUser& NetBarUser)
{	
	if (CNetBarConfig::GetInstance()->GetRealNamePassword() < 2)	//С��2��������
	{
		return TRUE;
	}

	//����������Ϊ��,������
	if (NetBarUser.GetPassword().IsEmpty())
	{
		CIBALog::GetInstance()->Write(_T("��������Ϊ�գ�����ʵ�����������Ƿ���ȷ��"));
		return TRUE;
	}

	//��������
	if (NetBarUser.GetPassword().Compare(CCurrentNetBarUser::GetInstance()->Balance.GetPassword()) != 0)
	{
		//���벻һ��
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


INT CJQRealName::ChangeCertificateType(INT nTmp)
{
	return nTmp;
}

BOOL CJQRealName::ReadUserInfo(CNetBarUser & NetBarUser)
{
	//�趨HKEY_CURRENT_USER\\software\\sunlike\\IBA\\�µķ��ʼ���֧RealName\\GLW\\OpenUser
	
	CString strKey(_T("RealName\\JQ\\OpenUser"));

	CString strTmp = theApp.GetProfileString(strKey, _T("IDNo"));

	strTmp.Trim();

	if (strTmp.IsEmpty())
	{
		return FALSE;
	}
	else
	{
		CString strTime, strIDNo, strCode;
		// ��֤ˢ����Ϣ
		strTime = theApp.GetProfileString(strKey, _T("vValue1"));
		strCode = theApp.GetProfileString(strKey, _T("vValue2"));

		time_t tm = time(NULL);
		long nTime = _ttol(strTime.GetString());

		if (tm - nTime > 5* 60)
		{
			AfxMessageBox(_T("��Ч��ˢ����Ϣ��Դ����ˢ����ʱ!"), MB_OK|MB_ICONERROR);
			IBA_LOG0(_T("ˢ����ʱ!"));
			return FALSE;
		}

		CString strOrgCode, strMD5;
		strOrgCode += CT2A(strTmp);
		strOrgCode += CT2A(strTime);
		strOrgCode += "jqdodonew1035";

		// �˺������ֺ���ĸ��ϣ������Ǳ�������
		CEncrypt::CalcMD5(strOrgCode, strMD5);

		if (strCode != strMD5)
		{
			AfxMessageBox(_T("��Ч��ˢ����Ϣ��Դ!"), MB_OK|MB_ICONERROR);
			IBA_LOG0(_T("��Ч��ˢ����Ϣ��Դ!"));
			return FALSE;
		}
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
	NetBarUser.SetCertificateType(ChangeCertificateType(nTmp));
	
	// ���� ûȥ��
	strTmp = theApp.GetProfileString(strKey, _T("Country"));

	strTmp = theApp.GetProfileString(strKey, _T("Address"));	
	NetBarUser.SetAddr(strTmp);

	// jqadd
	int nSex = theApp.GetProfileInt(strKey, _T("Sex"), 0);
	NetBarUser.SetSex(nSex);

	// ������Ϣ ûȥ��
	strTmp = theApp.GetProfileString(strKey, _T("Other"));

	NetBarUser.SetDataFrom(1);

	return TRUE;
}


INT CJQRealName::OnExecuteMsg(UINT nExecuteType, LPVOID lpData /*= NULL*/, DWORD dwLen /*= 0*/)
{
	if (m_NetBarUser.GetMemberId() > 0)
	{
		OnOldUser(m_NetBarUser);
	}

	return 0;
}

BOOL CJQRealName::IsMenuEnabled()
{
	return FALSE;
}
