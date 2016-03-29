// PayCardHtmlDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include ".\PayVerifyDlg.h"
#include "MDataXSender.h"

// CPayCardHtmlDlg �Ի���

IMPLEMENT_DYNCREATE(CPayVerifyDlg, CDHtmlDialog)

BEGIN_MESSAGE_MAP(CPayVerifyDlg, CDHtmlDialog)
	ON_WM_CLOSE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CPayVerifyDlg)

END_DHTML_EVENT_MAP()

BEGIN_DISPATCH_MAP(CPayVerifyDlg, CDHtmlDialog)
	DISP_FUNCTION(CPayVerifyDlg, "AutoClose", OnClose, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CPayVerifyDlg, "PrintText", PrintText, VT_BOOL, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CPayVerifyDlg, "PcpNotify", PcpNotify, VT_BOOL, VTS_I4 VTS_BSTR)
END_DISPATCH_MAP()

CPayVerifyDlg::CPayVerifyDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CPayVerifyDlg::IDD, 0, pParent)
{
	EnableAutomation();
}

CPayVerifyDlg::~CPayVerifyDlg()
{
}

void CPayVerifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CPayVerifyDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();

	SetExternalDispatch(GetIDispatch(FALSE));

	CString headData = _T("User-Agent: DODONEW\r\nContent-Type: application/x-www-form-urlencoded\r\nAccept-Language: zh-cn\r\n");

	Navigate(m_strURL, 0, NULL, headData, m_strPostData.GetBuffer(), m_strPostData.GetLength());



	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// CPayCardHtmlDlg ��Ϣ�������

void CPayVerifyDlg::PostNcDestroy()
{
	CDHtmlDialog::PostNcDestroy();
}

void CPayVerifyDlg::OnOK()
{
	//CDHtmlDialog::OnOK();
}

void CPayVerifyDlg::OnCancel()
{
	//CDHtmlDialog::OnCancel();
}

STDMETHODIMP CPayVerifyDlg::ShowContextMenu(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, 
										   IDispatch *pdispReserved)
{

#if _DEBUG
	return S_FALSE;
#endif

	return S_OK; //�������˵�
}

void CPayVerifyDlg::OnClose()
{
	//CDHtmlDialog::OnClose();

	//CDHtmlDialog::OnCancel();

	((CDialog*)GetParent())->EndDialog(IDCANCEL);
}

BOOL CPayVerifyDlg::IsExternalDispatchSafe()
{
	return TRUE;
}

BOOL CPayVerifyDlg::PrintText(INT nFontSize, LPCTSTR lpszText)
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

void CPayVerifyDlg::SplitLine(CString& strLine, CStringArray& strArray, TCHAR ch /*= '|'*/)
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

BOOL CPayVerifyDlg::PcpNotify(INT nCmd, LPCTSTR lpszText)
{
	if (1 == nCmd)
	{
		NS_DataX::CDataXMSender Sender;

		Sender.Create(1, _T("DdnPcp-0ED70A77-233A-43c0-9490-962283436DF5"));

		CString strTmp(lpszText);
		int nSpl = strTmp.Find(_T('|'),0);
		int nMemberId = 0;
		if (nSpl >= 0)
		{
			nMemberId = _ttoi(strTmp.Left(nSpl));
		}
		strTmp = strTmp.Mid(nSpl+1);

		Sender.PostData(1, nMemberId, 0, strTmp.GetBuffer(), strTmp.GetLength());

		strTmp.ReleaseBuffer();
	}

	return TRUE;
}
BOOL CPayVerifyDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	return CDHtmlDialog::OnEraseBkgnd(pDC);
}
