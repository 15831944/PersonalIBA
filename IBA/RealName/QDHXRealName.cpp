#include "stdafx.h"
#include "..\IBA.h"
#include "..\ServletHellper.h"
#include ".\qdhxrealname.h"

using namespace NS_REALNAME;
//�ൺ����

CQDHXRealName::CQDHXRealName(void)
{//д��־
	CIBALog::GetInstance()->Write(_T("CQDHXRealName"));
}

CQDHXRealName::~CQDHXRealName(void)
{
}

INT CQDHXRealName::OnRealNameMsg(UINT nMsgType, LPVOID lpData /*= NULL*/, DWORD dwLen /*= 0*/)
{
	UNIT_AUTO_TRACE("CQDHXRealName::OnRealNameMsg(UINT nMsgType=%d, LPVOID lpData=%x, DWORD dwLen=%d)"
		,nMsgType,lpData,dwLen);

	CRealNameBase::OnRealNameMsg(nMsgType, lpData, dwLen);

	if (0 == nMsgType) //ˢ����Ϣ
	{
		//2015-1027
		if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
		{
			return 0;
		}

		CString strKey(_T("RealName\\HX\\OpenUser"));//�ൺ����

		CString strIdCardNumber = theApp.GetProfileString(strKey, _T("IDNo"));

		if (strIdCardNumber.IsEmpty())
		{
			return 0;
		}

		if (!CheckCard(strIdCardNumber))
		{
			return 0;
		}

		CNetBarUser NetBarUser;
		NetBarUser.SetNetId(strIdCardNumber);
		NetBarUser.SetDataFrom(1);

		if (m_nMemberId > 0) //�Ͽ����ȼ���
		{
			NetBarUser.SetMemberId(m_nMemberId);

			QueryBalance();

			OnOldUser(NetBarUser);
		}
		else //�¿����Ʒ�δע��
		{
			NetBarUser.SetCertificateType(11);
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

BOOL CQDHXRealName::UploadImage(UINT nType, CNetBarUser& NetBarUser, UINT nOption)
{
	return FALSE;
}
