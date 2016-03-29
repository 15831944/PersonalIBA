// IBALoginDlg.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "IBA.h"
#include "Cashier.h"
#include "IBAHelpper.h"
#include "BossPwdDlg.h"
#include "NetBarInfoPage.h"
#include ".\ibalogindlg.h"
#include "dal\IBADAL.h"
#include "AllCheckOut.h"
#include ".\RealName\IBARealName.h"

using namespace NS_DAL;

// CIBALoginDlg �Ի���

IMPLEMENT_DYNAMIC(CIBALoginDlg, CDialog)
CIBALoginDlg::CIBALoginDlg(CWnd* pParent /*=NULL*/)
: CIBADialog(CIBALoginDlg::IDD, pParent)
, m_nLoginType(0)
{
	EnableVisualManagerStyle(FALSE);
}

CIBALoginDlg::~CIBALoginDlg()
{
}

void CIBALoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CIBADialog::DoDataExchange(pDX);
	
	DDX_Text(pDX, IDC_EDIT_PWD, m_strPassword);
	DDX_Control(pDX, IDC_COMBO_CASHIER, m_cboOperators);

	DDX_Control(pDX, IDOK, m_btnLogin);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BUTTON_SET, m_btnSet);
	DDX_Control(pDX, IDC_BUTTON_ENMERGENCY, m_btnEnmergency);
}

BEGIN_MESSAGE_MAP(CIBALoginDlg, CIBADialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnBnClickedButtonSet)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_ENMERGENCY, OnBnClickedButtonEnmergency)
END_MESSAGE_MAP()

// CIBALoginDlg ��Ϣ�������

void CIBALoginDlg::OnBnClickedOk()
{
	UpdateData();

	CString strOperater, strMsg;
	m_cboOperators.GetWindowText(strOperater);
	strOperater.Trim();//ȥ����β�ո�

#ifdef DEBUG
	if( strOperater==_T("gxx") )
	{
		//m_strPassword = _T("123");
	}
#endif

	if (!CheckInput(strOperater))
	{
		return;
	}
	//���뷽��
	theApp.GetCurCashier()->SetName(strOperater);
	theApp.GetCurCashier()->SetPassword(m_strPassword);

	//Normal��Restoreģʽ����������̨��½����Ϊ�м���ܻᵯ�����ڻ�ȴ������Ҫ����ť��Ϊ��Ч
	if (CIBAGlobal::NetworkStatus != CIBAGlobal::emNetworkBroken)
	{
		GetDlgItem(IDOK)->EnableWindow(FALSE);//��ʱʹ��ť��Ч���˳�ǰ��ʹ��ť��Ч

		CCashRegisterLoginMethod_atLogin crLogin;

		//theApp.GetCurCashier()->CheckIn(crLogin, strOperater);
		theApp.GetCurCashier()->CheckIn_atLogin(crLogin, strOperater);

		//���ķ�����
		if (crLogin.GetStatusCode() > 0) 
		{	
			SetToolTipPos(IDC_COMBO_CASHIER);

			switch (crLogin.GetStatusCode())
			{
			case NS_SERVLET::ESC_MACJYC30:     // MACУ���
				{
					strMsg.Format(LOAD_STRING(IDS_MACERROR), CIBAConfig::GetInstance()->GetCashRegisterID(), crLogin.GetPSK().Left(12));
					CIBAHelpper::PutTextToClipboard(crLogin.GetPSK().Left(12));

					GetDlgItem(IDC_BUTTON_SET)->ShowWindow(SW_SHOW);
				}
				break;

			case NS_SERVLET::ESC_WXSYTID50:    // ��Ч����̨ID
				{
					strMsg.Format(LOAD_STRING(IDS_CASHREGISTERID), CIBAConfig::GetInstance()->GetCashRegisterID(), crLogin.GetPSK().Left(12));
					CIBAHelpper::PutTextToClipboard(crLogin.GetPSK().Left(12));

					GetDlgItem(IDC_BUTTON_SET)->ShowWindow(SW_SHOW);
				}
				break;

			case NS_SERVLET::ESC_WXMM15:       // ��Ч����
				{
					strMsg = LOAD_STRING(IDS_LOGINERROR);
				}	
				break;

			default:
				{
					IBA_TRACE("��¼ʱ�����ķ�����Ĭ�ϴ���");

					strMsg = crLogin.GetStatusMessage();
				}		
			}

			m_strPassword.Empty();
			UpdateData(FALSE);

			ShowToolTip(strMsg);

			GetDlgItem(IDOK)->EnableWindow(TRUE);//�ָ�����
			return;
		}
		else if (crLogin.GetStatusCode() < 0) 
		{
			SetToolTipPos(IDOK);
			//ShowToolTip(strMsg);

			GetDlgItem(IDC_BUTTON_SET)->ShowWindow(SW_SHOW);

			if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
			{
				// 2011/07/13-8201-gxx: ���б��ص�¼
				if (!theApp.GetCurCashier()->LocalModeLogin())
				{
					MessageBox(_T("���������ж�,���б��ص�¼ʱ��֤����Ա������ʧ��,��ʹ��ǰһ��������¼���˺ź�����!"),
						NULL,MB_OK|MB_ICONINFORMATION);

					GetDlgItem(IDOK)->EnableWindow(TRUE); //��ť�ָ�
					return ;
				}
			}
			
			//�Ѿ���theApp.GetCurCashier()->CheckIn_atLogin�н��뱾��ģʽ��
			//CBrokenNetwork::GetInstance()->DisableNetwork();
			//IBA_LOG(L"m_threadTestResume => Start");
			//CBrokenNetwork::GetInstance()->m_threadTestResume->Start();//�����̡߳�����ָ���Ϊ�������û��������ؼ�¼�̺߳���������ȷ���ָ��Ժ�������ֻ��������״̬Ϊ��������������ָ�����localMemberɾ����¼��

			//return;
		}

		#ifndef _DEBUG // release �汾

		// 2011/07/14-8201-gxx: ��Ӷ������ж�
		if (CIBAGlobal::NetworkStatus != CIBAGlobal::emNetworkBroken)
		{
			//����֪ͨ
			if (!crLogin.GetNoticeInfo().IsEmpty())
			{
				MsgBox(crLogin.GetNoticeInfo());
			}

			//����֪ͨ��������
			if (!crLogin.GetPopupURL().IsEmpty())
			{
				ShellExecute(NULL, _T("open"), crLogin.GetPopupURL(), NULL, NULL, SW_NORMAL);
			}

			//ͬ������ʱ��
			CIBAHelpper::SyncLocalTime(CIBAHelpper::CenterTimeToOleDateTime(crLogin.GetServerTime()));
		}

		#endif
	}
	else
	{
		// 2011/07/13-8201-gxx: ���б��ص�¼
		if (!theApp.GetCurCashier()->LocalModeLogin())
		{
			GetDlgItem(IDOK)->EnableWindow(FALSE);

			MessageBox(_T("���������ж�,���б��ص�¼ʱ��֤����Ա������ʧ��,��ʹ��ǰһ��������¼���˺ź�����!"),
				NULL,MB_OK|MB_ICONINFORMATION);

			GetDlgItem(IDOK)->EnableWindow(TRUE);
			return ;
		}
		
		//CBrokenNetwork::DisableNetwork();
	}

	theApp.GetRealName()->OnCashierCheckIn();

	CBCGPDialog::OnOK();
}

BOOL CIBALoginDlg::OnInitDialog()
{
	CIBADialog::OnInitDialog();

	LoadImage();
	
	SetEditFont();

	InitOperatorsCombo();
	GetDlgItem(IDC_COMBO_CASHIER)->SetFont(&m_Font);
	//���õ�����λ��
	if (m_nLoginType == 0)
	{//�����Ǵ�����Ȼ������ˣ���ʧȥ����
		SetWindowPos(&CWnd::wndTopMost, 0, 0, 318, 242, SWP_NOMOVE);
		GetDlgItem(IDC_EDIT_PWD)->SetFocus();
	}
	else
	{//����ԭ���ã�ֻ�ƶ��´���
		SetWindowPos(NULL, 0, 0, 318, 242, SWP_NOMOVE);
		m_cboOperators.SetFocus();
	}

	CRgn MyRgn;
	MyRgn.CreateRoundRectRgn(0, 0, 318, 242, 12, 12);
	SetWindowRgn(MyRgn, FALSE);

	//{ 2011/09/05-8201-gxx: 
	m_btnEnmergency.ShowWindow(FALSE);
	WINDOWPLACEMENT wp;
	m_btnEnmergency.GetWindowPlacement(&wp);
	CString strAllCheckOut;
	m_btnEnmergency.GetWindowText(strAllCheckOut);
	m_btnAllCheckOut.Create( strAllCheckOut, WS_VISIBLE|WS_CHILD|SS_NOTIFY,wp.rcNormalPosition,this,IDC_BUTTON_ENMERGENCY);
	m_btnAllCheckOut.SetFont(&m_FontSmallBold);
	m_btnAllCheckOut.SetBkColor(RGB(245, 236, 216));
	m_btnAllCheckOut.SetLinkColor(RGB(218,186,129));
	m_btnAllCheckOut.SetHoverColor(RGB(235, 207, 156));
	m_btnAllCheckOut.SetVisitedColor(RGB(218, 186, 129));
	m_btnAllCheckOut.SetTransparent(FALSE);
	m_btnAllCheckOut.ShowWindow(CIBAConfig::GetInstance()->GetEnmergencyVisible());
	
	//}
	
	//m_btnEnmergency.ShowWindow(CIBAConfig::GetInstance()->GetEnmergencyVisible());

	
	//���û����������ID����ʶ����
	if (CNetBarConfig::GetInstance()->GetNetBarId() == 0 
		|| CNetBarConfig::GetInstance()->GetNetBarKey().GetLength() < 16)
	{
		CDlgNetbarConfig dlg;
		if (dlg.DoModal() == IDOK)
		{
			CNetBarConfig::GetInstance()->LoadFromDB();
		}
	}

	return FALSE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CIBALoginDlg::OnCancel()
{
	// 2015-0828 liyajun
	CRegKey regKey;
	LONG lRet = regKey.Open(HKEY_CURRENT_USER, _T("Software\\Sunlike\\IBA"));

	if (ERROR_SUCCESS == lRet)
	{
		DWORD dwValue = 0;
		lRet = regKey.QueryDWORDValue(_T("IsFN"), *(DWORD*)&dwValue);
		if (ERROR_SUCCESS == lRet)
		{
			regKey.DeleteValue(_T("IsFN"));
			regKey.DeleteSubKey(_T("IsFN"));
		}

		regKey.Close();
	}
	CIBADialog::OnCancel();
}

void CIBALoginDlg::InitOperatorsCombo()
{
	COperatorArray OperatorArray;
	CIBADAL::GetInstance()->GetOperatorRecord(OperatorArray);
	if (OperatorArray.GetCount() > 0)
	{//�����ݿ����
		for (INT i = 0; i < OperatorArray.GetCount(); i++)
		{
			m_cboOperators.AddString(OperatorArray.GetAt(i));
		}
	}
	else
	{//���ݿ������ݣ���ע������
		CString strTmp = theApp.GetProfileString(_T("NetBarInfo"), _T("Operators"));

		CStringArray strArray;

		CIBAHelpper::SplitLine(strTmp, strArray);

		for (INT i = 0; i < strArray.GetCount(); i++)
		{
			m_cboOperators.AddString(strArray.GetAt(i));
		}
	}
	m_cboOperators.SetWindowText(theApp.GetProfileString(_T("NetBarInfo"), _T("LastOperator"), 0));

}

BOOL CIBALoginDlg::CheckInput(CString &strOperater)
{
	if (strOperater.IsEmpty())
	{	
		SetToolTipPos(IDC_COMBO_CASHIER);
		ShowToolTip(IDS_INPUTACCOUNT);
		m_cboOperators.SetFocus();
		return FALSE;
	}

	m_strPassword.Trim();

	if (m_strPassword.IsEmpty())
	{
		GetDlgItem(IDC_EDIT_PASSWORD)->SetFocus();

		return FALSE;
	}

	return TRUE;
}

void CIBALoginDlg::OnBnClickedButtonSet()
{
	CDlgBossPwd dlg;

	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	CDlgNetbarConfig dlg2;
	dlg2.DoModal();
}

BOOL CIBALoginDlg::OnEraseBkgnd(CDC* pDC)
{
	if (m_BKImage.IsNull())
	{
		CIBADialog::OnEraseBkgnd(pDC);
	}
	else
	{
		m_BKImage.Draw(pDC->GetSafeHdc(), 0, 0);
	}

	m_btnLogin.SetBkGnd(pDC);
	m_btnCancel.SetBkGnd(pDC);
	m_btnSet.SetBkGnd(pDC);
	m_btnEnmergency.SetBkGnd(pDC);

	return TRUE;
}

void CIBALoginDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	ReleaseCapture();//ȡ������
	SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y)); //������Ϣ

	CIBADialog::OnLButtonDown(nFlags, point);
}

void CIBALoginDlg::LoadImage()
{
	m_BKImage.Load(theApp.GetWorkPath() + _T("\\IBAImage\\LoginBK.png"));//����
	m_btnLogin.LoadStdImageFile(theApp.GetWorkPath() + _T("\\IBAImage\\LoginBtn.png"));//��ť
	m_btnCancel.LoadStdImageFile(theApp.GetWorkPath() + _T("\\IBAImage\\CancelBtn.png"));//ȡ��
	m_btnSet.LoadStdImageFile(theApp.GetWorkPath() + _T("\\IBAImage\\SetBtn.png"));//����

	m_btnEnmergency.LoadStdImage(IDR_PNG_ENMERGENCY, _T("PNG"));
}

void CIBALoginDlg::OnBnClickedButtonEnmergency()
{
	CDlgAllCheckOut dlg;
	dlg.DoModal();
}

void CIBALoginDlg::SetLoginType(UINT newVal)
{

	m_nLoginType = newVal;
}

LRESULT CIBALoginDlg::OnShowToolTip(WPARAM wParam, LPARAM lParam)
{
	SetToolTipPos(IDOK);
	CString strShow(_T("ԭ�����޷����ӣ������л����ĵ�¼"));
	ShowToolTip(strShow);
	return 0;
}

