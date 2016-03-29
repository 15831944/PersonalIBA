#include "stdafx.h"
#include "..\IBA.h"
#include "..\Cashier.h"
#include "..\..\GCardID_RZX\RealName.h"
#include "..\ServletHellper.h"
#include ".\rzxrealname.h"
#include "..\CurrentNetBarUser.h"

using namespace NS_REALNAME;

CRZXRealName::CRZXRealName(void)
{
	CIBALog::GetInstance()->Write(_T("CRZXRealName"));

	m_hIdLogUpload = NULL;
	m_pfnRzxCheckOut = NULL;
	
	//m_hRzxCard = NULL;
	//m_pfnCheckCardActive = NULL;

	Load();
}

CRZXRealName::~CRZXRealName(void)
{
	UnLoad();
}

BOOL CRZXRealName::InitRealName()
{
	CRealNameBase::InitRealName();

	ExecuteProxy();

	return TRUE;
}

BOOL CRZXRealName::UnInitRealName()
{
	CRealNameBase::UnInitRealName();

	//{ 2011/05/13-gxx: �˳�ʱ�������� ������ ʵ���Ľ���
	CL2Process rxzProcess;	rxzProcess.Attach(_T("rzxsurename.exe"));	rxzProcess.Terminate();
	//}
	
	
	return TRUE;
}

void CRZXRealName::Load()
{
	//CIBALog::GetInstance()->Write(_T("Begin loading rzx library!"));

	//m_hIdLogUpload = LoadLibrary(_T("idlogupload.dll"));

	//if (m_hIdLogUpload == NULL)
	//{	
	//	CIBALog::GetInstance()->WriteFormat(_T("LoadLibrary idlogupload.dll Error = %d"), GetLastError());
	//	AfxMessageBox(_T("���������ж�̬��(idlogupload.dll)ʧ�ܣ�"));
	//}
	//else
	//{
	//	m_pfnRzxCheckOut = (pfnRzx2008RzxCheckOut)::GetProcAddress(m_hIdLogUpload, "RzxCheckOut");
	//}

	//CIBALog::GetInstance()->Write(_T("Load idlogupload.dll completed!"));

}

void CRZXRealName::UnLoad()
{
	if (m_hIdLogUpload != NULL)
	{	
		FreeLibrary(m_hIdLogUpload);
		m_hIdLogUpload = NULL;
	}
}

INT CRZXRealName::OnRealNameMsg(UINT nMsgType, LPVOID lpData /*= NULL*/, DWORD dwLen /*= 0*/)
{
	UNIT_AUTO_TRACE("CRZXRealName::OnRealNameMsg(UINT nMsgType=%d, LPVOID lpData=%x , DWORD dwLen=%d)"
		,nMsgType,lpData,dwLen);

	CRealNameBase::OnRealNameMsg(nMsgType, lpData, dwLen);

	if (0 == nMsgType)
	{
		ReplyMessage(0);

		CString strKey(_T("RealName\\RZX\\IDUser"));

		CString strIdCardNumber = theApp.GetProfileString(strKey, _T("IDNo"));

		if (strIdCardNumber.IsEmpty())
		{
			return 0;
		}

		UINT nValidate = theApp.GetProfileInt(strKey, _T("Validate"), 0);

		if (nValidate == 0 || nValidate == 1)
		{
			//
		}
		else
		{
			CIBALog::GetInstance()->WriteFormat(_T("RZX IDCard Validate = %d"), nValidate);

			return 0;
		}

		m_NetBarUser.SetNetId(strIdCardNumber);
		CString strTmp = theApp.GetProfileString(strKey, _T("Name"));
		m_NetBarUser.SetUserName(strTmp);
		strTmp = theApp.GetProfileString(strKey, _T("CertificateID"));
		m_NetBarUser.SetCertificateID(strTmp);
		strTmp = theApp.GetProfileString(strKey, _T("CertificateType"));
		m_NetBarUser.SetCertificateType(_ttoi(strTmp));
		strTmp = theApp.GetProfileString(strKey, _T("Password"));
		m_NetBarUser.SetPassword(strTmp);
		m_NetBarUser.SetDataFrom(1);

		//�����ͣ�1 - ��ͨ�����Ʒѣ� 3 - ��ʱ�����Ʒ�
		m_NetBarUser.SetIsMember(1 == theApp.GetProfileInt(strKey, _T("IDCardType"), 3));

		// 2013/5/3-guoxuxing
		CNetBarConfig::GetInstance()->m_bRZXCardMember = m_NetBarUser.GetIsMember();

		if (!CheckCard(m_NetBarUser.GetNetId()))
		{
			return 0;
		}

		if (m_nMemberId > 0) //�Ͽ����ȼ���
		{
			m_NetBarUser.SetMemberId(m_nMemberId);

			QueryBalance();

			//�����Ϊʵ������
			AutoChangeRealNamePwd(m_NetBarUser);

			if (AutoModifyMemberInfo())
			{
				QueryBalance();
			}


			//AfxGetMainWnd()->BringWindowToTop();
			//AfxGetMainWnd()->SetForegroundWindow();

			OnOldUser(m_NetBarUser);
		}
		else //�¿����Ʒ�δע��
		{
			IBA_LOG (_T("�ж��Ƿ��б�Ҫ�������˺�ת����֤������"));
			if (AutoMatchIDCard(m_NetBarUser))
			{
				IBA_LOG (_T("��Ҫת��"));
				AfxGetMainWnd()->BringWindowToTop();
				AfxGetMainWnd()->SetForegroundWindow();

				OnOldUser(m_NetBarUser);
			}
			else
			{
				IBA_LOG (_T("����Ҫת��"));
				OnNewUser(m_NetBarUser);
			}
		}
	}

	//theApp.DelRegTree(HKEY_CURRENT_USER, _T("Software\\Sunlike\\IBA\\RealName\\RZX\\IDUser"));

	return 0;
}

//ʵ��������Ϣ����
INT CRZXRealName::InvokeRealName(UINT nMsgType, LPVOID lpData /*= NULL*/, DWORD dwLen /*= 0*/)
{
	if (!theApp.GetCurCashier()->IsOnDuty())//����Ա�����ߣ����°࣬����
	{
		return FALSE;
	}

	CIBALog::GetInstance()->WriteFormat(_T("InvokeRealName: nMsgType = %d"), nMsgType);//��Ϣд��־

	switch (nMsgType)
	{
	case 1:
		return SendMsgToRZX(IDCARD_MANAGE_ADD);//����

	case 2:
		return SendMsgToRZX(IDCARD_MANAGE_LOSD);//

	case 3:
		return SendMsgToRZX(IDCARD_MANAGE_MODIFY);//�޸�

	case 4:
		return SendMsgToRZX(IDCARD_MANAGE_MEMBER);//

	case 100:
		return CheckOutTempUser((LPCTSTR)lpData);//ʵ������ʱ��

	}

	return FALSE;	
}

BOOL CRZXRealName::UploadImage(UINT nType, CNetBarUser& NetBarUser, UINT nOption)
{
	return FALSE;
}

BOOL CRZXRealName::IsMenuEnabled()
{
	return TRUE;
}

BOOL CRZXRealName::IsRealNameExtendMenuEnabled()
{
	return TRUE;
}

//ʵ�����˿�ǳ�
//INT CRZXRealName::CheckOutTempUser(LPCTSTR lpCardNum)
//{
//	INT nRet = 0;
//
//	if (m_pfnRzxCheckOut != NULL)
//	{	
//		CString strTmp(lpCardNum);
//
//		if (strTmp.GetLength() > 10) //������ֻ֧��10λ
//		{
//			strTmp = strTmp.Right(10);
//		}
//
//		CStringA strCardNum = CT2CA(strTmp);
//		nRet = m_pfnRzxCheckOut(strCardNum);
//
//		CIBALog::GetInstance()->WriteFormat(_T("m_pfnRzxCheckOut: CardNum = %s, Ret = %d"), strTmp, nRet);
//
//		//����0�ɹ���1ʧ��
//		if (1 == nRet)
//		{
//			//AfxMessageBox(_T("������ע��������ʧ�ܣ�"));
//			CIBALog::GetInstance()->Write(_T("������ע��������ʧ�ܣ�"));
//		}
//	}
//	else
//	{		
//		CIBALog::GetInstance()->WriteFormat(_T("Can't get address 0f RzxCheckOut!"));
//	}
//	
//	return nRet;
//} 

INT CRZXRealName::CheckOutTempUser(LPCTSTR lpCardNum)
{
	#define UM_ID_TEMPCARD_RZX  WM_USER + 1010

	CString strTmp(lpCardNum);

	if (strTmp.GetLength() > 10) //������ֻ֧��10λ
	{
		strTmp = strTmp.Right(10);
	}

	CStringA strCardNum = CT2CA(strTmp);

	COPYDATASTRUCT cds = {0};

	cds.dwData = UM_ID_TEMPCARD_RZX;
	cds.cbData = strCardNum.GetLength();
	cds.lpData = (PVOID)strCardNum.GetString();

	HWND hSureName = ::FindWindow(NULL, _T("������ʵ���Ǽǹ���ϵͳ"));

	if (hSureName == NULL)
	{
		//AfxMessageBox(_T("�Ҳ���������ʵ���Ǽǹ���ϵͳ��"));

		CIBALog::GetInstance()->Write(_T("�Ҳ���������ʵ���Ǽǹ���ϵͳ��"));

		return FALSE;
	}

	::SendMessage(hSureName, WM_COPYDATA, (WPARAM)hSureName, (LPARAM)&cds);
	
	return TRUE;
} 


BOOL CRZXRealName::SendMsgToRZX(UINT nMsgType)
{
	HWND hSureName = ::FindWindow(NULL, _T("������ʵ���Ǽǹ���ϵͳ"));

	if (hSureName == NULL)
	{
		AfxMessageBox(_T("�Ҳ���������ʵ���Ǽǹ���ϵͳ��"));

		return FALSE;
	}

	BOOL bSend = ::PostMessage(hSureName, WM_USER_IDCARD_MANAGE, nMsgType, NULL);

	return bSend;
} 


BOOL CRZXRealName::RobFocusAfterSwipingCard()
{
	return TRUE;
}

//������ĺ�ʵ���������Ƿ���ͬ
BOOL CRZXRealName::AutoChangeRealNamePwd(CNetBarUser& NetBarUser)
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

BOOL NS_REALNAME::CRZXRealName::GetPhotoPath( CString& strFile, const CString& strID )
{
	CRegKey RegKey;
	ULONG uLen = 256;

	if(ERROR_SUCCESS == RegKey.Open(HKEY_LOCAL_MACHINE, 
		_T("SOFTWARE\\RzxCorp\\SureName")))
	{
		RegKey.QueryValue( strFile.GetBuffer(256), _T("ExePath"), &uLen);
		strFile.ReleaseBuffer();
		if(!strFile.IsEmpty())
		{
			strFile += _T("\\temp\\");
			strFile += strID;
			strFile += _T(".jpg");
			return TRUE;
		}	
	}
	return FALSE;
}


CString NS_REALNAME::CRZXRealName::GetBirthdayFromSFZ( const CString& sfzh )
{
	CString strBirthday;
	if (sfzh.GetLength() == 15) 
	{
		CString year, month, day;
		year = sfzh.Mid(6, 2);
		month = sfzh.Mid(8, 2);
		day = sfzh.Mid(10, 2);

		if (_ttoi(year) > 50) { 
			year = "19" + year;
		}
		else
		{
			year = "20" + year;
		}

		strBirthday = year+"-" + month+ "-" +day;
	}	
	else if (sfzh.GetLength() == 18) 
	{
		CString year, month, day;
		year = sfzh.Mid(6, 4);
		month = sfzh.Mid(10, 2);
		day = sfzh.Mid(12, 2);
		strBirthday = year+"-" + month+ "-" +day;
	}
	return strBirthday;
}

int NS_REALNAME::CRZXRealName::GetNationCode(const CString& strNation)
{
	if(strNation == _T("��")) return 1;
	else if(strNation == _T("�ɹ�")) return 2;
	else if(strNation == _T("��")) return 3;
	else if(strNation == _T("��")) return 4;
	else if(strNation == _T("ά���")) return 5;
	else if(strNation == _T("��")) return 6;
	else if(strNation == _T("��")) return 7;
	else if(strNation == _T("׳")) return 8;
	else if(strNation == _T("����")) return 9;
	else if(strNation == _T("����")) return 10;
	else if(strNation == _T("��")) return 11;
	else if(strNation == _T("��")) return 12;
	else if(strNation == _T("��")) return 13;
	else if(strNation == _T("��")) return 14;
	else if(strNation == _T("����")) return 15;
	else if(strNation == _T("����")) return 16;
	else if(strNation == _T("������")) return 17;
	else if(strNation == _T("��")) return 18;
	else if(strNation == _T("��")) return 19;
	else if(strNation == _T("����")) return 20;
	else if(strNation == _T("��")) return 21;
	else if(strNation == _T("�")) return 22;
	else if(strNation == _T("��ɽ")) return 23;
	else if(strNation == _T("����")) return 24;
	else if(strNation == _T("ˮ")) return 25;
	else if(strNation == _T("����")) return 26;
	else if(strNation == _T("����")) return 27;
	else if(strNation == _T("����")) return 28;
	else if(strNation == _T("�¶�����")) return 29;
	else if(strNation == _T("��")) return 30;
	else if(strNation == _T("���Ӷ�")) return 31;
	else if(strNation == _T("����")) return 32;
	else if(strNation == _T("Ǽ")) return 33;
	else if(strNation == _T("����")) return 34;
	else if(strNation == _T("����")) return 35;
	else if(strNation == _T("ë��")) return 36;
	else if(strNation == _T("����")) return 37;
	else if(strNation == _T("����")) return 38;
	else if(strNation == _T("����")) return 39;
	else if(strNation == _T("����")) return 40;
	else if(strNation == _T("������")) return 41;
	else if(strNation == _T("ŭ")) return 42;
	else if(strNation == _T("���α��")) return 43;
	else if(strNation == _T("����˹")) return 44;
	else if(strNation == _T("���¿�")) return 45;
	else if(strNation == _T("�°�")) return 46;
	else if(strNation == _T("����")) return 47;
	else if(strNation == _T("ԣ��")) return 48;
	else if(strNation == _T("��")) return 49;
	else if(strNation == _T("������")) return 50;
	else if(strNation == _T("����")) return 51;
	else if(strNation == _T("���״�")) return 52;
	else if(strNation == _T("����")) return 53;
	else if(strNation == _T("�Ű�")) return 54;
	else if(strNation == _T("���")) return 55;
	else if(strNation == _T("��ŵ")) return 56;

	return 99;
}

BOOL NS_REALNAME::CRZXRealName::AutoMatchIDCard( CNetBarUser & NetBarUser )
{
	//��ͨģʽ
	if (CNetBarConfig::GetInstance()->GetAutoMatchIDCard() == 0) return FALSE;

	CString strIdNumber = NetBarUser.GetCertificateID();
	//�����֤
	if (strIdNumber.GetLength() < 15) return FALSE;

	//�������֤��Ӧ���˺�
	CMemberInfoMethod MemberInfo;

	MemberInfo.SetPersonalId(strIdNumber);
	MemberInfo.SetAvailable(1);

	theApp.GetCurCashier()->DoMemberInfo(MemberInfo);

	if (MemberInfo.GetStatusCode() != 0) return FALSE;

	if (MemberInfo.GetMemberCount() == 0) return FALSE;

	CheckCard(MemberInfo.GetSerailNum(0));

	if (m_nMemberId > 0)
	{
		CString strNewNetId = strIdNumber;
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
