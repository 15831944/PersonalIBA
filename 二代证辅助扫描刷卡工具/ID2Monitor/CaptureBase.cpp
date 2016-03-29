#include "StdAfx.h"
#include ".\capturebase.h"
#include <atlbase.h>

CCaptureBase::CCaptureBase(void)
{
	m_nNetidID = 0;
	m_nIdNumberID = 0;
	m_nNameID = 0;

	m_bEx = FALSE;
}

CCaptureBase::~CCaptureBase(void)
{
}

BOOL CCaptureBase::Create(BOOL bDialog, LPCTSTR lpszCaption, LPCTSTR lpszClassName)
{
	if (bDialog)
	{
		return m_wndFinder.Init(lpszCaption);
	}
	else
	{
		return m_wndFinder.Init(lpszClassName, lpszCaption);
	}
}


BOOL CCaptureBase::CreateFrom(const CaptureItem& item)
{
	m_nNetidID = item.nCardId;
	m_nIdNumberID = item.nNumberId;
	m_nNameID = item.nNameId;
	
	m_bEx = FALSE;
	return Create(item.bDialog, item.strWindowName, item.strClassName);
}

BOOL CCaptureBase::CreateFromEx(const CaptureItemEx& item)
{
	m_nNetidID = item.nCardId;
	m_nIdNumberID = item.nNumberId;
	m_nNameID = item.nNameId;

	m_bEx = TRUE;
	return Create(item.bDialog, item.strWindowName, item.strClassName);
}
void CCaptureBase::AddNetidID(UINT nID)
{
	m_nNetidID = nID;
}

void CCaptureBase::AddIdNumberID(UINT nID)
{
	m_nIdNumberID = nID;
}	

void CCaptureBase::AddNameID(UINT nID)
{
	m_nNameID = nID;
}

BOOL CCaptureBase::CaptureAndSend(HWND hIBAWnd)
{
	if (!hIBAWnd)
	{
		MessageBox(NULL, "��ţ����̨û������!", "��ʾ", MB_OK);
		GXX_LOG_WARNING("��ţ����̨û������!");
		return FALSE;
	}

	CString strNetId,strIdNumber,strName;
	m_wndFinder.GetChildCtrlText(m_nNetidID,strNetId);
	m_wndFinder.GetChildCtrlText(m_nIdNumberID,strIdNumber);
	m_wndFinder.GetChildCtrlText(m_nNameID,strName);

	if (strNetId.IsEmpty())
	{
		if (strIdNumber.IsEmpty())
		{
			GXX_LOG_WARNING("֤�����롢�����˺ž�Ϊ�գ���������Ϣ!");
			return FALSE;
		}
		strNetId = strIdNumber; // �������˺ź�֤������һ��
	}
	else
	{
		if (strIdNumber.IsEmpty())
		{
			GXX_LOG_WARNING("֤������Ϊ�գ���������Ϣ!");
			return FALSE;
		}
	}

	if (strName.IsEmpty())
	{
		strName = "�����û�";
	}

	return SendCardInfo(hIBAWnd, strNetId, strIdNumber, strName);
}

static int g_index = 0;
BOOL   CALLBACK   EnumChildProc1(HWND hwnd,LPARAM lParam)       //�ص�����
{
	if(hwnd) //����Ӵ��ڴ���
	{
		CString strOut;
		::SendMessage(hwnd,   WM_GETTEXT,   WPARAM(100),   LPARAM(strOut.GetBuffer(100)));
		strOut.ReleaseBuffer();
		CString strTmp;
		strTmp.Format("ID˳��:%d, �ı�����:%s", g_index, strOut);
		GXX_LOG_HIT(strTmp);

		g_index++;

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}



BOOL CCaptureBase::CaptureAndSendEx(HWND hIBAWnd)
{
	if (!hIBAWnd)
	{
		MessageBox(NULL, "��ţ����̨û������!", "��ʾ", MB_OK);
		GXX_LOG_WARNING("��ţ����̨û������!");
		return FALSE;
	}

	g_index = 0;
	::EnumChildWindows(m_wndFinder.m_hBaseWnd,  (WNDENUMPROC)EnumChildProc1, 0);

	////���������־
	//HWND hWnd = ::GetWindow(m_wndFinder.m_hBaseWnd, GW_CHILD);
	//UINT iIndex = 0;
	//while(hWnd)
	//{
	//	CString strOut;
	//	::SendMessage(hWnd,   WM_GETTEXT,   WPARAM(100),   LPARAM(strOut.GetBuffer(100)));
	//	strOut.ReleaseBuffer();
	//	CString strTmp;
	//	strTmp.Format("ID˳��:%d, �ı�����:%s", iIndex,strOut);
	//	GXX_LOG_HIT(strTmp);
	//	iIndex ++;

	//	hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);
	//}

	//

	CString strNetId,strIdNumber,strName;
	m_wndFinder.GetChildCtrlTextEx(m_nNetidID,strNetId);
	m_wndFinder.GetChildCtrlTextEx(m_nIdNumberID,strIdNumber);
	m_wndFinder.GetChildCtrlTextEx(m_nNameID,strName);


	if (strNetId.IsEmpty())
	{
		if (strIdNumber.IsEmpty())
		{
			GXX_LOG_WARNING("֤�����롢�����˺ž�Ϊ�գ���������Ϣ!");
			return FALSE;
		}
		strNetId = strIdNumber; // �������˺ź�֤������һ��
	}
	else
	{
		if (strIdNumber.IsEmpty())
		{
			GXX_LOG_WARNING("֤������Ϊ�գ���������Ϣ!");
			return FALSE;
		}
	}

	if (strName.IsEmpty())
	{
		strName = "�����û�";
	}

	return SendCardInfo(hIBAWnd, strNetId, strIdNumber, strName);
}

BOOL SendCardInfo(HWND hIBAWnd, LPCTSTR lpszNetid, LPCTSTR lpszIDNumber, LPCTSTR lpszName)
{
	// ��ˢ����Ϣ,������Ϣ������̨

	GXX_LOG_HIT("send card information: name:%s, idnumber:%s, netid:%s", lpszName, lpszIDNumber, lpszNetid);


	int		nIdType = 12;         // ����֤
	CString strPassword = "123";  // ����
//
//#ifdef DEBUG
//	strNetId = "420114198607072210";
//	strIdNumber = "420114198607072210";
//	strName = "С��";
//#endif

	CRegKey reg;

	if(0 == reg.Create(HKEY_CURRENT_USER, "SoftWare\\Sunlike\\IBA\\RealName\\GLW\\OpenUser"))/*Base\\NewUser"))*/
	{
		reg.SetStringValue("IDNo", lpszNetid);
		reg.SetStringValue("CertificateID", lpszIDNumber);
		reg.SetDWORDValue("CertificateType", nIdType);
		reg.SetStringValue("Name", lpszName);
		reg.SetStringValue("Password", strPassword);

		/*time_t tm = time(NULL);
		CString strTm;
		strTm.Format("%ld", tm);
		reg.SetStringValue(_T("tValue1"), strTm);
		CString strOrgCode;
		strOrgCode += lpszNetid;
		strOrgCode.AppendFormat("%d", tm);
		INT nLen = strOrgCode.GetLength();
		BYTE *pBuff = new BYTE[nLen+8];
		memcpy(pBuff,strOrgCode.GetString(), nLen);
		BYTE byCheck[8] = {0x03,0xF1,0x00,0x0A,0x0B,0xED,0x02,0xC1};
		memcpy(pBuff+nLen, byCheck, 8);
		char pCode[33] = {0};
		G2XUtility::CG2XMD5::CalculateMD5(pBuff, nLen+8, pCode);
		reg.SetStringValue("tValue2", pCode);
		delete []pBuff;*/

		reg.Close();
	}
	else
	{
		GXX_LOG_HIT("д��ʧ��!");
	}

	if( hIBAWnd )
	{
		::SendMessage(hIBAWnd, WM_REALNAME, 999, 0);
		//::SendMessage(hIBAWnd, WM_REALNAME, 0, 100);
	}
	else
	{
		MessageBox(NULL, "��ţ����̨û������!", "��ʾ", MB_OK);
		GXX_LOG_HIT("��ţ����̨û������!");
		return FALSE;
	}

	return TRUE;
}