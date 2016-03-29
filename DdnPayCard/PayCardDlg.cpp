// PayCardDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MD5.h"
#include "WinHttpRequest.h"
#include "PayVerifyDlg.h"
#include "PcpThread.h"
#include ".\PayCardDlg.h"
#include "Hooks.h"


#include <iphlpapi.h> 
#pragma comment(lib, "IPHLPAPI.lib") 
#include "ContainDlg.h"
#include "readhardwareinfo.h"

//******************************************************************************

BEGIN_DISPATCH_MAP(CPayCardDlg, CG2XDialog)
	DISP_FUNCTION(CPayCardDlg, "PrintText", PrintText, VT_BOOL, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CPayCardDlg, "EnableRemoteInput", EnableRemoteInput, VT_NULL, VTS_I4)
END_DISPATCH_MAP()

// CPayCardDlg �Ի���

IMPLEMENT_DYNAMIC(CPayCardDlg, CG2XDialog)
CPayCardDlg::CPayCardDlg(CWnd* pParent /*=NULL*/)
	: CG2XDialog(CPayCardDlg::IDD, _T("��ţ�㿨ƽ̨(����̨)"), pParent)
	, m_lpWebBrowser(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_MAIN);
	m_bOldNetbar = FALSE;

	EnableAutomation();
}

CPayCardDlg::~CPayCardDlg()
{
}

void CPayCardDlg::DoDataExchange(CDataExchange* pDX)
{
	CG2XDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPayCardDlg, CG2XDialog)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	//ON_WM_PAINT()
END_MESSAGE_MAP()


// CPayCardDlg ��Ϣ�������

void CPayCardDlg::OnCancel()
{
	//CDialog::OnCancel();
}

void CPayCardDlg::OnOK()
{
	//CDialog::OnOK();
}

void CPayCardDlg::OnClose()
{
	//if (IDOK != MessageBox(_T("ȷ���رյ㿨ƽ̨��"), NULL, MB_ICONQUESTION | MB_OKCANCEL))
	//{
	//	return;
	//}

	CPcpThread::HidePcp();
}

void CPayCardDlg::PostNcDestroy()
{
	CG2XDialog::PostNcDestroy();

	if (m_lpWebBrowser != NULL)
	{
		m_lpWebBrowser->DestroyWindow();
		delete m_lpWebBrowser;
		m_lpWebBrowser = NULL;
	}
}

BOOL CPayCardDlg::OnInitDialog()
{
	CG2XDialog::OnInitDialog();

	GetCaptionBar()->GetSysBtnBar()->EraseID(SC_MAXIMIZE);

	GetCaptionBar()->SetIcon(m_hIcon);

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	SetWindowPos(NULL, 0, 0, 950 + GetSystemMetrics(SM_CXDLGFRAME) * 2, 
		640 +GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYDLGFRAME) * 2, SWP_NOMOVE);

	CreateWebBrower();

	GetPanel()->AttachWindow(m_lpWebBrowser, FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CPayCardDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	/*if (m_lpWebBrowser != NULL)
	{
		RECT rectClient;
		GetClientRect(&rectClient);

		rectClient.top += 25;
		m_lpWebBrowser->MoveWindow(&rectClient);	
	}*/
}

BOOL CPayCardDlg::CreateWebBrower()
{
	m_lpWebBrowser = new CWebBrowser;	
	m_lpWebBrowser->SetFlatStyle();

	RECT rectClient;
	GetClientRect(&rectClient);

	rectClient.top += 25;

	BOOL bRet = m_lpWebBrowser->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rectClient, this, 0, NULL);

//#ifndef _DEBUG
//	m_lpWebBrowser->EnableContextMenu(FALSE);
//#endif
	
	m_lpWebBrowser->SetEventHandler(this);

	return bRet;

}

inline BYTE toHex(const BYTE& x)
{
	return x > 9 ? x + 55 : x + 48;
}

int UrlEncode(LPBYTE in, LPBYTE out)
{
	int count = 0;
	if (out)
	{
		while (*in)
		{
			if (isalnum(*in))
			{
				*out++ = *in;
				count++;
			}
			else if (isspace(*in))
			{
				*out++ = '+';
				count++;
			}
			else
			{
				*out++ = '%';
				*out++ = toHex(*in >> 4);
				*out++ = toHex(*in % 16);
				count += 3;
			}
			in++;
			if (count >= 1021)
				break;
		}
		*out = '\0';
	}
	return 1;
}



void CPayCardDlg::WebBrowser_BeforeNavigate2(CWebBrowser& Sender, LPCTSTR lpszURL, DWORD nFlags, 
											 LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, 
											 LPCTSTR lpszHeaders, BOOL* pbCancel)
{
	CString strURL(lpszURL);

	CString strTmp = strURL.Left(3);

	strTmp.MakeLower();

	if (strTmp.Find(_T("ddn")) == -1)
	{
		return;
	}

	TRACE(strURL);

	*pbCancel = TRUE;

	CString strDdn(_T("http"));
	strDdn.Append(strURL.Mid(3));

	//GXX_LOG_HIT(_T("ddn����ǰ:") + strDdn);	
	char szResult[1024] = {0};
	UrlEncode((BYTE*)(LPCSTR)strDdn, (BYTE*)szResult);
	strDdn = szResult;
	//GXX_LOG_HIT(szResult);	

	CString postData;

	postData.Format(_T("domainId=%s&netBarId=%s&cashier=%s&dutyId=%s&ddn=%s&version=%d&mydodo=%d&t=%d"),
		m_strDomainId, m_strNetbarId, m_strCashier,m_strDutyId, strDdn, 51, 0, time(NULL));

	CString strMD5Src = postData + _T("&payKey=05633018A178AD52");

	postData = postData + _T("&Hash=") + CMD5::MD5String(strMD5Src);

	TRACE("%s",postData);

	CContainDlg dlg;
	
	dlg.m_dlg.m_strURL.Format(_T("http://%s/dodonew/front/index.jsp"), m_strDdnServer);
	//dlg.m_dlg.m_strURL.Format(_T("http://%s/dodonew/front/index.jsp"), "localhost:8090");
	//GXX_LOG_HIT(_T("��ַ1:") + dlg.m_dlg.m_strURL);	
	//GXX_LOG_HIT(postData);	

	dlg.m_dlg.m_strPostData = postData;

	dlg.DoModal();

	
	COleDateTime dt = COleDateTime::GetCurrentTime();
	CString str = _T("DDN:point card , time:%d");

	str += dt.Format(_T("%Y-%m-%d %H:%M:%S"));
	
	GXX_LOG_HIT(str);
}


BOOL CPayCardDlg::AnalyzeParamString(LPCTSTR lpszParams)
{
	//AfxMessageBox(lpszParams);

	TRACE(lpszParams);

	CComPtr<IXMLDOMDocument> pDoc;
	HRESULT hr = pDoc.CoCreateInstance(__uuidof(DOMDocument30));

	if (FAILED(hr))
	{  
		return FALSE;
	}

	_bstr_t strXML(lpszParams);

	VARIANT_BOOL bRet = VARIANT_FALSE;

	hr = pDoc->loadXML(strXML, &bRet);
	
	if (!bRet)
	{
		AfxMessageBox(_T("XML��������"));

		return FALSE;
	}

	//******************************************************************************

	_bstr_t strNode(_T("//DdnPayCard"));

	IXMLDOMNode* pNode = NULL;
	pDoc->selectSingleNode(strNode, &pNode);

	if (!pNode)
	{
		AfxMessageBox(_T("�Ҳ����ڵ�DdnPayCard��"));

		return FALSE;
	}

	//******************************************************************************

	_bstr_t strVal;
	pNode->get_text(strVal.GetAddress());
	CString strURLS = (LPCTSTR)strVal;

	INT nSp = strURLS.ReverseFind(_T('|'));
	if (nSp >= 0)
	{
		// IBA8237����ʹ�õ������л���ʽ
		// ��ַ�в�������
		m_strURLMain = strURLS.Left(nSp);
		m_strURLBakup = strURLS.Mid(nSp+1);
	}
	else
	{
		int nOldVersion = strURLS.Find(_T("userId"));
		int nICC = strURLS.Find(_T("type="));
		if (nOldVersion > 0)
		{
			GXX_LOG_HIT(_T("���ݾɰ汾����̨"));
			// �ϰ汾����̨ or ICC, ��ַ���в���
			m_strURLMain = strURLS;

			m_bOldNetbar = TRUE;
		}
		else
		{
			m_strURLMain = strURLS;
			m_strURLMain = strURLS.Left(strURLS.Find(_T(".jsp"))+4);
			if (nICC > 0)
			{
				m_strURLMain += _T("?");
				m_strURLMain += strURLS.Mid(nICC,6);
			}
		}
		GXX_LOG_HIT(_T("����̨����: %s"), strURLS);

	}
	
	GXX_LOG_HIT(_T("MainCenterURL: %s"), m_strURLMain);
	GXX_LOG_HIT(_T("BakupCenterURL: %s"), m_strURLBakup);

	//******************************************************************************

	IXMLDOMNamedNodeMap* pAttrMap = NULL;
	pNode->get_attributes(&pAttrMap);

	if (!pAttrMap)
	{
		return FALSE;
	}

	//******************************************************************************

	IXMLDOMNode* pAttrItem = NULL;
	_variant_t variantValue;

	_bstr_t strAttrDomainId(_T("DomainId"));
	pAttrMap->getNamedItem(strAttrDomainId, &pAttrItem);

	if (pAttrItem != NULL)
	{
		pAttrItem->get_nodeTypedValue(&variantValue);
		_bstr_t strVal(variantValue.bstrVal);
		m_strDomainId = (LPCTSTR)strVal;
	}

	//*******************************
	_bstr_t strCashID(_T("CashId"));
	pAttrMap->getNamedItem(strCashID, &pAttrItem);

	if (pAttrItem != NULL)
	{
		pAttrItem->get_nodeTypedValue(&variantValue);
		_bstr_t strVal(variantValue.bstrVal);
		m_strCashID = (LPCTSTR)strVal;
	}

	if (m_strCashID.IsEmpty())
	{
		m_strCashID = _T("1");
	}

	//*******************************
	_bstr_t strCasherPwd(_T("CasherPwd"));
	pAttrMap->getNamedItem(strCasherPwd, &pAttrItem);

	if (pAttrItem != NULL)
	{
		pAttrItem->get_nodeTypedValue(&variantValue);
		_bstr_t strVal(variantValue.bstrVal);
		m_strCasherPwd = (LPCTSTR)strVal;
	}

	//******************************************************************************

	_bstr_t strAttrNetbarId (_T("NetbarId"));
	pAttrMap->getNamedItem(strAttrNetbarId, &pAttrItem);

	if (pAttrItem != NULL)
	{
		pAttrItem->get_nodeTypedValue(&variantValue);
		_bstr_t strVal(variantValue.bstrVal);
		m_strNetbarId = (LPCTSTR)strVal;
	}

	//******************************************************************************

	_bstr_t strAttrCashier(_T("Cashier"));
	pAttrMap->getNamedItem(strAttrCashier, &pAttrItem);

	if (pAttrItem != NULL)
	{
		pAttrItem->get_nodeTypedValue(&variantValue);
		_bstr_t strVal(variantValue.bstrVal);
		m_strCashier = (LPCTSTR)strVal;
	}

	//******************************************************************************

	_bstr_t strAttrDutyId(_T("DutyId"));
	pAttrMap->getNamedItem(strAttrDutyId, &pAttrItem);

	if (pAttrItem != NULL)
	{
		pAttrItem->get_nodeTypedValue(&variantValue);
		_bstr_t strVal(variantValue.bstrVal);
		m_strDutyId = (LPCTSTR)strVal;
	}

	//******************************************************************************

	_bstr_t strAttrDdnServer(_T("DdnServer"));
	pAttrMap->getNamedItem(strAttrDdnServer, &pAttrItem);

	if (pAttrItem != NULL)
	{
		pAttrItem->get_nodeTypedValue(&variantValue);
		_bstr_t strVal(variantValue.bstrVal);
		m_strDdnServer = (LPCTSTR)strVal;
	}

	//******************************************************************************

	return TRUE;
}

HRESULT CPayCardDlg::WebBrowser_GetExternal(CWebBrowser& Sender, LPDISPATCH *lppDispatch)
{
	*lppDispatch = GetIDispatch(TRUE);

	return S_OK;
}

BOOL CPayCardDlg::PrintText(INT nFontSize, LPCTSTR lpszText)
{
	CString strContent(lpszText);

	if (strContent.IsEmpty())
	{
		return FALSE;
	}

	CPrintDialog PrintDlg(FALSE, PD_DISABLEPRINTTOFILE);// ��ӡ��ѡ��Ի���

	if (!PrintDlg.GetDefaults())//û��ѡ���ӡ��
	{
		return FALSE;
	}

	CDC dcPrinter;
	dcPrinter.Attach(PrintDlg.GetPrinterDC());//�����ӡDC��Ҳ���Զ�ȡע���õ���ӡ���󴴽�DC��

	//��ӡ��������
	LOGFONT lf = {0};
	lf.lfHeight = -MulDiv(nFontSize, dcPrinter.GetDeviceCaps(LOGPIXELSY), 72);
	lf.lfWeight = FW_NORMAL;
	lf.lfOutPrecision = OUT_TT_ONLY_PRECIS;
	lstrcpy(lf.lfFaceName, _T("����"));

	CFont PrinterFont;
	PrinterFont.CreateFontIndirect(&lf);
	dcPrinter.SelectObject(PrinterFont.GetSafeHandle());

	CStringArray strLines;
	SplitLine(strContent, strLines, '\n');//�õ����ָ��ִ����ûس��ָ�

	INT nX = 0;
	INT nY = 0;
	//��ʼ��ӡ
	dcPrinter.StartDoc(_T("DdnPcp"));

	for (INT i = 0; i < strLines.GetCount(); i++)
	{
		dcPrinter.TextOut(nX, nY, strLines.GetAt(i));
		nY += dcPrinter.GetTextExtent(strLines.GetAt(i)).cy;//�����ִ��Ŀ��
	}

	dcPrinter.EndDoc();//������ӡ
	dcPrinter.DeleteDC();//�ͷŶ���

	return TRUE;
}

void CPayCardDlg::SplitLine(CString& strLine, CStringArray& strArray, TCHAR ch /*= '|'*/)
{
	strArray.RemoveAll();

	if (strLine.IsEmpty())
	{
		return;
	}

	INT nPos = 0;

	INT nIndex = strLine.Find(ch);

	CString strTmp;

	while (nIndex >= 0)
	{
		strTmp.Empty();

		strTmp = strLine.Mid(nPos, nIndex - nPos);

		strArray.Add(strTmp);

		nPos = nIndex + 1;

		nIndex = strLine.Find(ch, nPos);
	}

	if (nPos == 0) //û���ҵ��ָ����
	{
		strArray.Add(strLine);
	}
	else
	{
		if ((strLine.GetLength() - 1) >= nPos)
		{
			strTmp.Empty();
			strTmp = strLine.Mid(nPos, strLine.GetLength() - nPos);
			strArray.Add(strTmp);
		}
	}
}

CString CPayCardDlg::GetHomePageURL()
{
	CString strURL = m_strURLMain;

	// QSCTT
	GXX_LOG_HIT(_T("31"));
	// DdnPcp1.1
	if (strURL.Find(_T("userId"),0) >= 0)
	{
		// �ϵ�����̨Ҫ������
		// QSCTT
		GXX_LOG_HIT(_T("32"));
	}
	else
	{
		// QSCTT
		GXX_LOG_HIT(_T("33"));
		//
		CTime tm = CTime::GetCurrentTime();

		CString strOrgCode;
		strOrgCode += m_strCashier;  // ����Ա
		strOrgCode += m_strNetbarId; // ����ID
		strOrgCode += m_strDutyId;   // ���ID
		strOrgCode += m_strCashID;   // ����̨ID
		strOrgCode.AppendFormat(_T("%lld"), tm.GetTime()); // ʱ���
		CString strPwdMd5;
		strPwdMd5 = CMD5::MD5String(m_strCasherPwd);
		strOrgCode += strPwdMd5;
		CString strNewCode;
		strNewCode = CMD5::MD5String(strOrgCode);

		CString strParam;
		if (strURL.ReverseFind(_T('?')) >= 0)
		{
			strParam = _T("&");
		}
		else
		{
			strParam = _T("?");
		}
		strParam.AppendFormat(_T("userId=%s"),m_strCashier);
		strParam.AppendFormat(_T("&netBarId=%s"), m_strNetbarId);
		strParam.AppendFormat(_T("&dutyId=%s"), m_strDutyId);
		strParam.AppendFormat(_T("&cashRegisterId=%s"), m_strCashID);
		strParam.AppendFormat(_T("&serial=%lld"), tm.GetTime());
		strParam.AppendFormat(_T("&check=%s"), strNewCode);


		strURL += strParam;

		GXX_LOG_HIT(_T("GetHomePageURL: %s"), strURL);
	}
	
	CString strRedirectURL = strURL;

	//

	try
	{
		CWinHttpRequest HttpRequest;

		HttpRequest.CreateByReg();

		HttpRequest.SetOption(CWinHttpRequest::WinHttpRequestOption_EnableRedirects, COleVariant(0L));
		HttpRequest.SetOption(CWinHttpRequest::WinHttpRequestOption_URLCodePage, COleVariant(936L));

		// QSCTT
		GXX_LOG_HIT(_T("34"));
		HttpRequest.Open(_T("GET"), strURL);
		// QSCTT
		GXX_LOG_HIT(_T("35"));
		DWORD dwTick = GetTickCount();
		// QSCTT
		GXX_LOG_HIT(_T("�򿪵�ַ"), strURL);
		HttpRequest.Send();
		GXX_LOG_HIT(_T("GetHttpData elapsed time %d!"), GetTickCount()-dwTick);

		if (HttpRequest.GetStatus() == 302)
		{			
			strRedirectURL = HttpRequest.GetResponseHeader(_T("Location"));

			GXX_LOG_HIT(_T("����ַ���ӳɹ�!"));

			return strRedirectURL;
		}

		if (HttpRequest.GetReadyState() != 4)
		{
			GXX_LOG_HIT(_T("���������Ӳ�ͨ..���Ա�����"));

			CWinHttpRequest HttpRequest2;

			CString strURL2;
			INT nQu = strURL.ReverseFind(_T('?'));
			strURL2 = m_strURLBakup;
			strURL2 += strURL.Mid(nQu);

			HttpRequest2.CreateByReg();

			HttpRequest2.SetOption(CWinHttpRequest::WinHttpRequestOption_EnableRedirects, COleVariant(0L));
			HttpRequest2.SetOption(CWinHttpRequest::WinHttpRequestOption_URLCodePage, COleVariant(936L));

			HttpRequest2.Open(_T("GET"), strURL2);

			DWORD dwTick = GetTickCount();
			HttpRequest2.Send();
			GXX_LOG_HIT(_T("GetHttpData elapsed time %d!"), GetTickCount()-dwTick);

			if (HttpRequest2.GetStatus() == 302)
			{
				GXX_LOG_HIT(_T("���ӳɹ�!"));
				strRedirectURL = HttpRequest2.GetResponseHeader(_T("Location"));
			}
		}
	}
	catch (...)
	{
		GXX_LOG_HIT(_T("���������Ӳ�ͨ..���Ա�����"));

		CWinHttpRequest HttpRequest2;

		CString strURL2;
		INT nQu = strURL.ReverseFind(_T('?'));
		strURL2 = m_strURLBakup;
		strURL2 += strURL.Mid(nQu);

		HttpRequest2.CreateByReg();

		HttpRequest2.SetOption(CWinHttpRequest::WinHttpRequestOption_EnableRedirects, COleVariant(0L));
		HttpRequest2.SetOption(CWinHttpRequest::WinHttpRequestOption_URLCodePage, COleVariant(936L));

		HttpRequest2.Open(_T("GET"), strURL2);

		DWORD dwTick = GetTickCount();
		HttpRequest2.Send();
		GXX_LOG_HIT(_T("GetHttpData elapsed time %d!"), GetTickCount()-dwTick);

		if (HttpRequest2.GetStatus() == 302)
		{
			GXX_LOG_HIT(_T("���ӳɹ�!"));
			strRedirectURL = HttpRequest2.GetResponseHeader(_T("Location"));
		}
	}

	return strRedirectURL;
}

CString CPayCardDlg::GetLocalIP()
{
	CString strIP;
	BOOL bRet = FALSE, bFind = FALSE;

	DWORD AdapterInfoSize = 0;   

	bRet = (GetAdaptersInfo(0, &AdapterInfoSize) == ERROR_BUFFER_OVERFLOW); //��ȡ����  

	if (bRet)  
	{   
		PIP_ADAPTER_INFO pAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[AdapterInfoSize]; //�����ڴ�   

		bRet = (GetAdaptersInfo(pAdapterInfo, &AdapterInfoSize) == ERROR_SUCCESS); //��ȡ��Ϣ   

		if (bRet) 
		{   
			PIP_ADAPTER_INFO pAdapter = pAdapterInfo;   

			while (pAdapter != NULL)   
			{    
				strIP = CA2W(pAdapter->IpAddressList.IpAddress.String);

				if (strIP.Left(3) == _T("192") || strIP.Left(3) == _T("172") || strIP.Left(3) == "10.")
				{
					/*strMAC.Format(_T("%02X-%02X-%02X-%02X-%02X-%02X"),  				 
						pAdapter->Address[0], pAdapter->Address[1], pAdapter->Address[2],   
						pAdapter->Address[3], pAdapter->Address[4], pAdapter->Address[5]);*/

					bFind = TRUE;

					break;
				}

				pAdapter = pAdapter->Next; //��һ������  
			}   

			//DWORD dwCount = AdapterInfoSize / sizeof(_IP_ADAPTER_INFO)

			if (!bFind) //û�ҵ������õ�һ��
			{
				pAdapter = pAdapterInfo;  

				strIP = CA2W(pAdapter->IpAddressList.IpAddress.String);

				/*strMAC.Format(_T("%02X-%02X-%02X-%02X-%02X-%02X"),  				 
					pAdapter->Address[0], pAdapter->Address[1], pAdapter->Address[2],   
					pAdapter->Address[3], pAdapter->Address[4], pAdapter->Address[5]);*/
				bFind = TRUE;
			}
		}   

		delete[] pAdapterInfo; //�ͷ��ڴ�   
	}   

	if (bFind)
		return strIP;
	return _T("");
}
void CPayCardDlg::OpenHomePage(BOOL bCashWay,BOOL bOpened)
{
	// QSCTT
	GXX_LOG_HIT(_T("10"));
	CString strTmp = GetHomePageURL();
	// QSCTT
	GXX_LOG_HIT(_T("11"));
	if (bCashWay)
	{
		strTmp.Append(_T("&CashWay=1"));
	}

	// 
	strTmp.AppendFormat(_T("&IsOpen=%d"),bOpened);
	

	// &LocalIp=... &lt=.. &LTCode=MD5(IP+lt+�̶�key)
	// �̶�key=554kdfdfe33fnnkdf
	CString strExparam;
	CString strIP = GetLocalIP();
	time_t tm = time(NULL);
	CReadHardwareInfo reader;
	CString strHardwardCode, strHardwardCodeEn;
	strHardwardCode = reader.GetHostCode();
	strHardwardCodeEn = reader.GetHostCodeEx(strHardwardCode);

	strExparam.AppendFormat(_T("&LocalIp=%s"),strIP);
	strExparam.AppendFormat(_T("&lt=%lld"),tm);
	strExparam.AppendFormat(_T("&mc=%s"),strHardwardCodeEn);

	CString strCode;
	strCode.Format(_T("%s%lld%s"),strIP,tm,strHardwardCode);
	strCode += _T("554kdfdfe33fnnkdf");
	CString strMd5;
	strMd5 = CMD5::MD5String(strCode);
	strMd5.MakeUpper();
	strExparam.AppendFormat(_T("&LTCode=%s"),strMd5);
	strTmp += strExparam;

	//GXX_LOG_HIT(_T("��ҳ��ַ:%s"), strTmp);
	// QSCTT
	GXX_LOG_HIT(_T("21"));
	m_lpWebBrowser->Navigate( strTmp);
	// QSCTT
	GXX_LOG_HIT(_T("22"));
	GXX_LOG_HIT(_T("DDN:point card, CashierID=%s,cashier=%s,LocalIp=%s"),
		m_strCashID,m_strCashier,strIP);

}

void CPayCardDlg::EnableRemoteInput(BOOL bEnable)
{
	CHooks::gEnableRemoteInput = bEnable;
}

//CString CPayCardDlg::EncodeURL(LPCTSTR lpszUrl)
//{
//	CWinHttpRequest WinHttpRequest;
//
//	WinHttpRequest.CreateByReg();
//
//	WinHttpRequest.SetOption(CWinHttpRequest::WinHttpRequestOption_URLCodePage, 
//		CWinHttpRequest::CodePage_GB2312);
//
//	WinHttpRequest.Open(_T("OPEN"), lpszUrl);
//
//	COleVariant vRet = WinHttpRequest.GetOption(CWinHttpRequest::WinHttpRequestOption_URL);
//
//	_bstr_t strVal(vRet.bstrVal);
//
//	CString strRet = (LPCTSTR)strVal;
//
//	return strRet;
//}

void CPayCardDlg::OnPaint()
{
	CG2XDialog::OnPaint();
}
