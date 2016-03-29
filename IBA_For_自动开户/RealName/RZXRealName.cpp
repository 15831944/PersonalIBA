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
