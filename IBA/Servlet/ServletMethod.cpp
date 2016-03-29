#include "stdafx.h"
#include "servletmethod.h"
#include "CommonMethod.h"
#include "WinHttpRequest.h"
#include "..\IBA.h"
#include "..\Encrypt.h"
#include "..\IBAHelpper.h"
#include "..\BrokenNetwork.h"
#include "..\MsgBoxDlg.h"
#include "..\MainFrm.h"

using namespace NS_SERVLET;

UINT CServletMethod::m_nFirstCenterIndex = 1; //Ĭ��Ϊ�����Ŀ�ʼ
UINT CServletMethod::m_nFirstQueryCenterIndex = 1; //Ĭ��Ϊ����ѯ���Ŀ�ʼ

CServletMethod::CServletMethod(LPCTSTR lpszMethodName)
: m_strMethodName(lpszMethodName), m_nCardType(ECT_ID)
, m_nLocalSerialNo(0), m_nNetbarId(0)
, m_bUseQueryHost(FALSE), m_nRevTimeout(20)//20��
{
	m_nNetbarId = CNetBarConfig::GetInstance()->GetNetBarId();
	m_strPSK = CNetBarConfig::GetInstance()->GetNetBarKey();
}

CServletMethod::~CServletMethod(void)
{
}

CString CServletMethod::GetMethodName()
{
	return m_strMethodName;
}

CString CServletMethod::GetRequestAddr(UINT nIndex)
{
	static const TCHAR HttpFormatString[] = _T("http://%s:%d/%s");

	CString strUrl;

	switch( nIndex )
	{
	case E_LOCAL_CENTER:
		{
			//��������
			strUrl.Format(HttpFormatString, CNetBarConfig::GetInstance()->GetLocalServerIP(),
				8080, _T("localnetbar"));
		}
		break;
	case E_MAIN_CENTER:
		{
			//������
			strUrl.Format(HttpFormatString, CNetBarConfig::GetInstance()->GetMainCenterIP(),
				CNetBarConfig::GetInstance()->GetMainCenterPort(),
				CNetBarConfig::GetInstance()->GetMainCenterPath());
		}
		break;
	case E_BACKUP_CENTER:
		{
			//������
			strUrl.Format(HttpFormatString, CNetBarConfig::GetInstance()->GetBackupCenterIP(),
				CNetBarConfig::GetInstance()->GetBackupCenterPort(),
				CNetBarConfig::GetInstance()->GetBackupCenterPath());
		}
		break;
	case E_OTHER_CENTER:
		{
			//�������
			strUrl.Format(_T("http://%s/netbar"), CNetBarConfig::GetInstance()->GetStandbyHost());
		}
		break;
	case E_QUERY_CENTER:
		{
			//����ѯ��
			strUrl.Format(_T("http://%s/netbar"), CNetBarConfig::GetInstance()->GetQueryHost());
		}
		break;
	case E_QUERY_CENTER2:
		{
			//����ѯ��
			strUrl.Format(_T("http://%s/netbar"), CNetBarConfig::GetInstance()->GetQueryHost2());
		}
		break;
	default:
		{
			IBA_ASSERT2(false,"δ֪����λ�ã��������");
			WriteLog(_T("δ֪����λ�ã��������"));
		}
		break;
	}

//#ifdef DEBUG
//	strUrl = _T("http://192.168.10.233:8080/netbar");   //2015-0826 liyajun Temp
//	//strUrl = _T("http://127.0.0.1:8080/netbar");   //2015-0826 liyajun Temp
//	//strUrl = _T("http://121.11.92.182/netbar");   //2015-0826 liyajun Temp
//#endif

	strUrl.Append(_T("/servlet/CommonRequest?command="));
	strUrl.Append(GetMethodName());
	strUrl.Append(_T("&version=2&param="));

	//WriteLog(_T("URL:") + strUrl + _T("xxx(Params)"));
	if(CIBAGlobal::NetworkStatus != CIBAGlobal::emNetworkBroken)
	{
		WriteLog(_T("URL:") + strUrl);
	}	
	
	return strUrl;
}

void CServletMethod::AddParams()
{
	m_Params.AddParam(_T("NetbarId"), EPT_NUMBER, 5);
	m_Params.SetValue(_T("NetbarId"), m_nNetbarId);
}

void CServletMethod::AddRespFields()
{
	m_RespFields.AddRespField(_T("MAC"), EPT_STRING, 16);
	m_RespFields.AddRespField(_T("serialNo"), EPT_NUMBER, 6);
	m_RespFields.AddRespField(_T("Status"), EPT_NUMBER, 2);
	m_RespFields.SetValue(_T("Status"), _T("-1"));
}

CString CServletMethod::EncodeParams(CString strParams)
{
	//strParams = CURLEncode(strParams);

	return strParams;
}

UINT CServletMethod::MakeLocalSerialNo()
{
	return CIBAConfig::GetInstance()->GetLocalSerialNo();
}

CString CServletMethod::MakeParamsString()
{
	CString strParams;
	
	m_nLocalSerialNo = MakeLocalSerialNo();

	CIBAHelpper::AppendString(strParams, m_nLocalSerialNo, 6);

	strParams.Append(MergeParams());

	CString strMac(MakeMacString(strParams));
	
	if (strMac.IsEmpty())
	{
		CIBAHelpper::FillString(strMac, 16, '0');
	}

	strParams.Insert(0, strMac);
	strParams = EncodeParams(strParams);

	// 2015-3-27 tmelody
	// �ϱ�������־
	if (0 == GetMethodName().CompareNoCase(_T("NonSysService")))
	{
#if _DEBUG
		WriteLog(_T("Params:") + strParams);
#endif
	}
	else
	{
		if(CIBAGlobal::NetworkStatus != CIBAGlobal::emNetworkBroken)
		{
			WriteLog(_T("Params:") + strParams);
		}		
	}

	return strParams;
}

BOOL CServletMethod::OnBegin()
{
	return TRUE;
}

//����m_RespFields.Status�ʹ�ӡ��־
void CServletMethod::OnEnd(BOOL bRet)
{
	CString strTmp = m_RespFields.GetValueAsString(_T("Status"));

	if(CIBAGlobal::NetworkStatus != CIBAGlobal::emNetworkBroken)
	{
		IBA_LOG(_T("CServletMethod::Status - %s"), strTmp);
	}	

	if (!bRet) //ʧ����
	{
		m_RespFields.ResetAllValue();
		m_RespFields.SetValue(_T("Status"), _T("-1")); //����Ϊ-1
	}
}

BOOL CServletMethod::Execute()
{
	if (!OnBegin())//��ʼ
	{
		return FALSE;
	}

	if(CIBAGlobal::NetworkStatus != CIBAGlobal::emNetworkBroken)
	{
		IBA_LOG(_T("CServletMethod::Execute - %s"), GetMethodName());
	}	
	//����
	m_RespFields.ResetAllValue();
	m_RespFields.SetValue(_T("Status"), _T("-1")); //����Ϊ-1

	BOOL bRet = FALSE;
	
	CString strParams(MakeParamsString());//��ϳɲ����ִ�
	
	CString strResp;  
	
	//��ȡ�������ݣ�����3��
	bRet = GetDecodeVerify(strParams,strResp);
	
	//�����ǰ״̬ΪRestore,���ΪNormal
	if(bRet)
	{
		if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkRestore)
		{
			CBrokenNetwork::GetInstance()->EnableNetwork();
		}
	}
	else //���֮ǰ״̬ΪNormal,���ΪRestore,ͬʱ������������Ͽ��߳�
	{
		if(CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkNormal)
		{
			CBrokenNetwork::GetInstance()->RestoreNetwork();
			if(FALSE == CBrokenNetwork::GetInstance()->bTestBrokenStarted)
			{
				CBrokenNetwork::GetInstance()->bTestBrokenStarted = TRUE;
				CBrokenNetwork::GetInstance()->m_threadTestBroken->Start();
			}
		}
	}

	OnEnd(bRet);

	return bRet;
}

BOOL CServletMethod::GetDecodeVerify(CString strParams,CString& strResp)
{
	//��ȡ�������ݣ���־��ȡ������ʱ��
	DWORD dwTick = GetTickCount();
	BOOL bRet = GetCenterData(strParams, strResp);
	if(CIBAGlobal::NetworkStatus != CIBAGlobal::emNetworkBroken)
	{
		IBA_LOG(_T("connect center using time: %u"), GetTickCount() - dwTick);
	}	

	//��ȡ�ɹ��ҽ���У��ɹ�������ɹ�
	if (bRet) //�����Ļ��������
	{	//����			
		bRet = DecodeResp(strResp); //�Ƚ���������У���أ�

		ASSERT(bRet);				//��Ϊ�޷��϶���������ʧ�ܳɹ��Ƿ������ܣ����������Ƚ�����У�飡

		if (bRet && GetStatusCode() == 0) //ֻУ������ɹ������ݣ���ʧ�ܵ�����û��Ҫ��У��
		{
			bRet = VerifyResp(strResp);//��֤
			ASSERT(bRet);
		}
	}
	return bRet;
}

BOOL CServletMethod::GetHttpData(LPCTSTR lpszURL, CString& strResp)
{
	strResp.Empty();

	static const LONG CodePage_GB2312 = 936; // �������� (GB2312)

	static const LONG WinHttpRequestOption_URLCodePage = 2;

	CWinHttpRequest HttpRequest(FALSE); 
	
	try//��ʱ�����˴�try
	{
		HttpRequest.Create();
		HttpRequest.SetOption(WinHttpRequestOption_URLCodePage, COleVariant(CodePage_GB2312));
		HttpRequest.SetTimeouts(5000, 5000, 10000, 1000 * m_nRevTimeout);
		HttpRequest.Open(lpszURL);
		HttpRequest.Send();
	}
	catch (...)
	{
		IBA_LOG(_T("CServletMethod::GetHttpData Error!"));

		return FALSE;
	}

	if (HttpRequest.GetReadyState() == 4)//���ӳɹ�
	{
		strResp = HttpRequest.GetResponseText(); //�������TRIM

		if(CIBAGlobal::NetworkStatus != CIBAGlobal::emNetworkBroken)
		{
			WriteLog(_T("Resp:") + strResp);
		}		

		if (strResp.GetAt(0) == '<') //��������
		{
			strResp.Empty();

			IBA_LOG(_T("Center is being upgraded!"));

			return FALSE;
		}
	}

	if (HttpRequest.GetReadyState() == 4)
	{
		return TRUE;
	}
	else
	{
		//if (!CBrokenNetwork::GetInstance()->m_bHasFirstTrying)
		//{
		//	theApp.UpdateIBAStatus(TRUE);  //����״̬����ʾ

		//	CBrokenNetwork::GetInstance()->m_bHasFirstTrying = TRUE;
		//}
	}
	

	return (HttpRequest.GetReadyState() == 4);
}	

CString CServletMethod::MergeParams()
{
	return m_Params.GetAllValue();
}

CString CServletMethod::MakeMacString(CString strParams)
{
	if(CIBAGlobal::NetworkStatus != CIBAGlobal::emNetworkBroken)
	{
		WriteLog(_T("PSK:") + m_strPSK);
	}

	CString strMac;
	
	CEncrypt::CalcMac(strParams, strMac, m_strPSK, TRUE);

	return strMac;
}

BOOL CServletMethod::VerifyResp(CString strResp)
{
	CString strMac = strResp.Left(16);

	CString strTmp;

	CEncrypt::CalcMac(strResp.Mid(16), strTmp, m_strPSK, TRUE);

	if (0 == strMac.CompareNoCase(strTmp))
	{
		return TRUE;
	}
	else
	{
		CIBALog::GetInstance()->WriteFormat(_T("VerifyResp Failed. PSK:%s MAC:%s"), m_strPSK, strTmp);
		CIBALog::GetInstance()->WriteFormat(_T("Resp:%s"), strResp);
	}

	return FALSE;
}

BOOL CServletMethod::DecodeResp(CString strResp)
{
	return m_RespFields.DecodeResp(strResp);
}	

INT CServletMethod::GetStatusCode()
{
	CString strTmp = m_RespFields.GetValueAsString(_T("Status"));

	int nTmp = _ttoi(strTmp);

	return nTmp;
}

void CServletMethod::WriteLog(LPCTSTR lpszInfo)
{
	CIBALog::GetInstance()->Write(lpszInfo, CIBALog::ELT_SERVLET);
}

void CServletMethod::SetPSK(CString newVal)
{
	m_strPSK = newVal;
}

CString CServletMethod::GetPSK() const
{
	return m_strPSK;
}

ECARDTYPE CServletMethod::GetCardType() const
{
	return m_nCardType;
}

void CServletMethod::SetCardType(ECARDTYPE newVal)
{
	m_nCardType = newVal;
}

UINT CServletMethod::GetSerialNo()
{
	return m_RespFields.GetValueAsUINT(_T("serialNo"));
}

UINT CServletMethod::GetLocalSerialNo() const
{
	return m_nLocalSerialNo;
}

CString CServletMethod::GetStatusMessage()
{
	CStatusMessageMethod sm;
	sm.SetStatusCode(GetStatusCode());
	sm.Execute();
	return sm.GetMessage();
}

BOOL CServletMethod::GetCenterData(CString strParams, CString& strResp)
{
//#ifdef _DEBUG
//	m_bUseQueryHost = FALSE; //�����²�ʹ�ò�ѯ��
//#endif

	BOOL bRet = FALSE;

	CString strURL;

	if (m_bUseQueryHost)
	{
		for (UINT i = 0; i < 2; i++) //��ѯ��2������
		{
			UINT nIdx = m_nFirstQueryCenterIndex + i;

			if (nIdx > 2) nIdx -= 2; //���ܳ���2

			strURL = GetRequestAddr(nIdx + 3) + strParams;

			if (bRet = GetHttpData(strURL, strResp))
			{
#ifdef _DEBUG
				IBA_LOG(_T("���͵����ݣ�%s"), strURL);
				IBA_LOG(_T("���ص����ݣ�%s"), strResp);
#endif
				m_nFirstQueryCenterIndex = nIdx; //��¼�ɹ�������˳��
				break;
			}
		}
	}
	else
	{
		for (UINT i = 0; i < 3; i++) //��֧����3������
		{
			UINT nIdx = m_nFirstCenterIndex + i;

			if (nIdx > 3) nIdx -= 3; //���ܳ���3

			if (3 == nIdx && CNetBarConfig::GetInstance()->GetStandbyHost().IsEmpty())
			{
				continue; //û��������ط�����
			}

			CString strRequesAddr = GetUrlFromIndex(nIdx);

			strURL = GetRequestAddr(nIdx) + strParams;
			if (bRet = GetHttpData(strURL, strResp))
			{
#ifdef _DEBUG
				IBA_LOG(_T("���͵����ݣ�%s"), strURL);
				IBA_LOG(_T("���ص����ݣ�%s"), strResp);
#endif
				//�����ӵ��Ǳ����ģ�����IBA.ini�н��������ĺͱ��������ã�ʹ��ͨ��ʼ����������

				// 2014-8-15 - qsc ������������жϣ���Ҫ�ǽ�����߳�ͬʱ�������������ʱ��ͬ�������⣬��ֹ����滻����������
				if(0 != strRequesAddr.CompareNoCase(GetUrlFromIndex(E_MAIN_CENTER)) && m_nFirstCenterIndex != nIdx )	
				{
					// 2014-8-15 - qsc �����л����˱����Ļ�����������޸����ݿ������ĵ�ַ
					if(E_BACKUP_CENTER == nIdx)
					{
						CString strMain = GetUrlFromIndex(E_MAIN_CENTER);
						CString strBk = GetUrlFromIndex(nIdx);

						// ����
						CNetBarConfig::GetInstance()->SetMainHost(strBk);
						CNetBarConfig::GetInstance()->SetBackUpHost(strMain);

						m_nFirstCenterIndex = E_MAIN_CENTER;			// ����������ͨ��ʼ����������

					}
					else if(E_OTHER_CENTER == nIdx)
					{
						CString strMain = GetUrlFromIndex(E_MAIN_CENTER);
						CString strStandby = GetUrlFromIndex(nIdx);

						// ����
						CNetBarConfig::GetInstance()->SetMainHost(strStandby);
						CNetBarConfig::GetInstance()->SetStandbyHost(strMain);//��ط�����

						m_nFirstCenterIndex = E_MAIN_CENTER;			// ����������ͨ��ʼ����������
					}	
					else
					{
						m_nFirstCenterIndex = nIdx; //��¼�ɹ�������˳��
					}
				}
				break;
			}
			else
			{
				TRACE("\n\n");
				OutputDebugString(strURL);
			}
		}
	}

	return bRet;
}

CString CServletMethod::GetUrlFromIndex(UINT nIndex)
{
	static const TCHAR HttpFormatString[] = _T("%s:%d");

	CString strUrl;

	switch( nIndex )
	{
	case E_LOCAL_CENTER:
		{
			//��������
			strUrl.Format(HttpFormatString, CNetBarConfig::GetInstance()->GetLocalServerIP(),
				8080);
		}
		break;
	case E_MAIN_CENTER:
		{
			//������
			strUrl.Format(HttpFormatString, CNetBarConfig::GetInstance()->GetMainCenterIP(),
				CNetBarConfig::GetInstance()->GetMainCenterPort());
		}
		break;
	case E_BACKUP_CENTER:
		{
			//������
			strUrl.Format(HttpFormatString, CNetBarConfig::GetInstance()->GetBackupCenterIP(),
				CNetBarConfig::GetInstance()->GetBackupCenterPort());
		}
		break;
	case E_OTHER_CENTER:
		{
			//�������
			strUrl.Format(_T("%s"), CNetBarConfig::GetInstance()->GetStandbyHost());
		}
		break;
	case E_QUERY_CENTER:
		{
			//����ѯ��
			strUrl.Format(_T("%s"), CNetBarConfig::GetInstance()->GetQueryHost());
		}
		break;
	case E_QUERY_CENTER2:
		{
			//����ѯ��
			strUrl.Format(_T("%s"), CNetBarConfig::GetInstance()->GetQueryHost2());
		}
		break;
	default:
		{
			IBA_ASSERT2(false,"δ֪����λ�ã��������");
			WriteLog(_T("δ֪����λ�ã��������"));
		}
		break;
	}

	if(CIBAGlobal::NetworkStatus != CIBAGlobal::emNetworkBroken)
	{
		WriteLog(_T("GetUrlFromIndex:") + strUrl);
	}	

	return strUrl;
}
