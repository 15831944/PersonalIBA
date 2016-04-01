// RegisterDlg.cpp : ʵ���ļ�

#include "stdafx.h"
#include "IBA.h"
#include "PwdDlg.h"
#include "Encrypt.h"
#include "IBAPrint.h"
#include "dal\IBADAL.h"
#include "tts\IBATTS.h"
#include "Socket\LocalServer.h"
#include "RealName\IBARealName.h"
#include "MainFrm.h"
#include "IBAHelpper.h"
#include "BundleTimeListDlg.h"
#include "IdentityNumber.h"
#include "memberinfodlg.h"
#include "CurrentNetBarUser.h"
#include "ActiveMemberView.h"
#include ".\registerdlg.h"
#include "BossPwdDlg.h"
#include "DlgInputMobile.h"
#include "RealName\WDRealName.h"
#include "RealName\RealNameBase.h"
#include "NetBarUser.h"
#include "RealName\RZXRealName.h"
#include "DlgCapPhoto.h"
#include "ChinaUnicomConfig.h"
#include "RealName\AuditInterface.h"
#include "DlgCapture.h"
#include "DlgInputUserPwd.h"
#include "LocalCenter/LocalCenter.h"

// CRegisterDlg �Ի���

HWND CRegisterDlg::m_StaticPrehWnd = NULL;

BOOL CRegisterDlg::m_StaticTmpMember = FALSE;

IMPLEMENT_DYNAMIC(CRegisterDlg, CNDDialog)
CRegisterDlg::CRegisterDlg(CWnd* pParent /*=NULL*/)
: CNDDialog(CRegisterDlg::IDD, pParent)
, m_bIsTempUser(TRUE), m_bNoPresent(FALSE)
, m_bWaitReg(FALSE), m_bClickRnReg(FALSE)
{
	InitData();

	m_bPhoneNumberOpen = FALSE;
	IBA_LOG0(_T("Register dialog object created!"));
}

CRegisterDlg::~CRegisterDlg()
{
#ifndef _DEBUG
	HWND hWnd = theApp.GetMainWnd()->GetSafeHwnd();
	if(NULL != hWnd)
	{
		// �Ȱ���������Ϊ���
		::SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE );  
		//::SetWindowPos(GetSafeHwnd(), hWnd, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

		//// ��δ��뽫��ǰ���ڴ���������㣬������Ϊ�����
		//HWND hForegdWnd = ::GetForegroundWindow();
		//DWORD dwCurID = ::GetCurrentThreadId();
		//DWORD dwForeID = ::GetWindowThreadProcessId(hForegdWnd, NULL);
		//::AttachThreadInput(dwCurID, dwForeID, TRUE);
		//::SetForegroundWindow(GetSafeHwnd());
		//::AttachThreadInput(dwCurID, dwForeID, FALSE);


		//// ���ý�������Ϊ���뽹��
		//m_edtMoney.SetFocus();

	}
#endif
}

void CRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CNDDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_IDCARDSNO, m_strNetId);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_PERSONALID, m_strPersonalID);
	DDX_Check(pDX, IDC_CHECK_USERHASCARD, m_bUserHasCard);
	DDX_Check(pDX, IDC_CHECK_PRESENT, m_bNoPresent);
	DDX_Check(pDX, IDC_CHECK_PRINT, m_bPrintTicket);
	DDX_Control(pDX, IDC_COMBO_SEX, m_cboSex);
	DDX_Control(pDX, IDC_COMBO_USERCLASS, m_cboUserClass);
	DDX_Control(pDX, IDC_COMBO_IDTYPE, m_cboIDType);
	DDX_Control(pDX, IDC_EDIT_MONEY, m_edtMoney);
	DDX_Control(pDX, IDC_CHECK_TEMPMEMBER, m_chkTempMember);
	DDX_Control(pDX, IDC_COMBO_BTSEL, m_cboBundTimeSel);
	DDX_Control(pDX, IDC_EDIT_PERSONALID, m_edtPersonalID);
	DDX_Control(pDX, IDC_COMBO_TEMPUSERCLASS, m_cboTempUserClass);
	DDX_Control(pDX, IDC_RADIO_TEMPUSER, m_btnTempUserClass);
	DDX_Control(pDX, IDC_RADIO_MEMBER, m_btnMemberUserClass);
	DDX_Control(pDX, IDC_COMBO_COMPUTER, m_cboComputer);

	DDX_Control(pDX, IDC_BTN_CUSTOM_BOUNDLETIME, m_btnSefDefine);
	DDX_Text(pDX, IDC_EDIT_TELNUM, m_strTelNum);
	DDV_MaxChars(pDX, m_strTelNum, 20);
}

BEGIN_MESSAGE_MAP(CRegisterDlg, CNDDialog)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_BUNDLETIME, OnBnClickedButtonBundletime)
	ON_BN_CLICKED(IDC_BUTTON_READID2, OnBnClickedButtonReadid2)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_TEMPMEMBER, OnBnClickedCheckTempmember)
	ON_BN_CLICKED(IDC_BUTTON_REPRINT, OnBnClickedButtonReprint)
	ON_BN_CLICKED(IDC_CHECK_PRINT, OnBnClickedCheckPrint)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, OnBnClickedButtonQuery)
	ON_MESSAGE(WM_RECEIVECARD, OnRealName)
	ON_COMMAND(IDM_SCAN2_BACK, OnScan2Back)
	ON_COMMAND(IDM_SCAN2, OnScan2)
	ON_COMMAND(IDM_SCAN1, OnScan1)
	ON_COMMAND(IDM_READ2, OnRead2)
	ON_COMMAND(IDC_BUTTON_READCARD, OnRead2)
	ON_COMMAND(IDM_SCAN_PASSPORT, OnScanPassport)
	ON_COMMAND(ID_SCAN_OTHER, OnScanOther)
	ON_EN_SETFOCUS(IDC_EDIT_IDCARDSNO, OnEnSetfocusEditIdcardsno)
	ON_CBN_SELCHANGE(IDC_COMBO_BTSEL, OnCbnSelchangeComboBtsel)
	ON_EN_CHANGE(IDC_EDIT_PERSONALID, OnEnChangeEditPersonalid)
	ON_CBN_SELCHANGE(IDC_COMBO_USERCLASS, OnCbnSelchangeComboUserclass)
	ON_BN_CLICKED(IDC_RADIO_TEMPUSER, OnBnClickedRadioTempuser)
	ON_BN_CLICKED(IDC_RADIO_MEMBER, OnBnClickedRadioMember)
	ON_CBN_SELCHANGE(IDC_COMBO_TEMPUSERCLASS, OnCbnSelchangeComboTempuserclass)
	ON_BN_CLICKED(IDC_BTN_CUSTOM_BOUNDLETIME, OnBnClickedBtnCustomBoundletime)
	ON_BN_CLICKED(IDC_BTN_INPUT_NUMBER, OnBnClickedBtnInputNumber)
	ON_BN_CLICKED(IDC_BTN_JUBAO, OnBnClickedBtnCapphtoto)
	ON_BN_CLICKED(IDC_BTN_RN_REG, OnBnClickedBtnRnReg)
	ON_MESSAGE(C_CANCLOSE, OnCanClose)
	ON_BN_CLICKED(IDC_BUTTON_QUERY_TEL, OnBnClickedButtonQueryTel)
	ON_BN_CLICKED(IDC_BTN_PHOTO, OnBnClickedBtnPhoto)
END_MESSAGE_MAP()

// CRegisterDlg ��Ϣ�������

void CRegisterDlg::OnBnClickedOk()
{
	UpdateData();
	
	if (!theApp.GetRealName()->IsRegisterEnabled())
	{
		theApp.IBAMsgBox(_T("ʵ����������!"));
		return;//ʵ���Ƿ�������
	}

	if (!CheckRealNameData()) return; //У��ʵ����������
	
	GetRegisterInfo();//�ӿؼ�����ע����Ϣ
	
	if (!CheckInput()) return; //��������

	if (!CheckMoney()) return;//����������

	MakeUserPwd();//�Զ���������

	if(2 == CNetBarConfig::GetInstance()->GetSoftSfreg())
	{
		if(!CIBAHelpper::TakePhoto(CIBAHelpper::GetUserZcImgPath(m_nIdType, m_strPersonalID), m_strPersonalID))
		{
			return ;
		}
	}

	// 2014-6-26 - qsc
	if(0 != CNetBarConfig::GetInstance()->GetSoftSfreg())
	{
		CString strDest = CIBAHelpper::GetUserZjImgFilePath(m_nIdType, m_strPersonalID);
		// 2014-8-13 - qsc ÿ�ζ����� if(!PathFileExists(strDest))
		{
			const CString strSource = theApp.GetWorkPath() + _T("\\IBATemp\\ID.jpg");
			CopyFile(strSource, strDest, FALSE);

			CString strBak = theApp.GetWorkPath() + _T("\\IBATemp\\ID_bak.jpg");
			CopyFile(strSource, strBak, FALSE);
			DeleteFile(strSource);
		}		
	}

	if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		//2015-0918 liyajun �Ͻ�ע�᱾���û�
		//RegisterLocalUser();
		return;
	}
	else
	{	
		//{
		//	// QSCT1
		//	// 2014-4-25 qsc  ȡ������sfregע��
		//	// ��Ӧ��OnCanClose��ЩҲû���ˣ�����������ȷ����������������̬�ȣ���������
		//	//// 2013-10-12 qsc
		//	if(2 <= CNetBarConfig::GetInstance()->GetSoftSfreg()	||
		//		m_bClickRnReg)		// �Զ�ע���ͨ��ʵ��ע��򿪵Ķ�Ҫ��SFREG
		//	{
		//		m_bClickRnReg = FALSE;
		//		m_bWaitReg = TRUE;
		//		ExeSfreg();
		//		return ;

		//	}
		//	else
		//	{
		//		RegisterCenterUser();
		//	}
		//}
		//
		/*if(CNetBarConfig::GetInstance()->GetAuditType() == CIBAGlobal::emRealnameJQ)
		{
		try
		{
		SendRegDataToProxy();
		}
		//catch (...)
		{
		IBA_LOG(_T("��ʵ���ӿڷ���ע����Ϣ����! %d"), GetLastError());
		}
		RegisterCenterUser();
		}*/
		//{
		// 2014-4-28 - qsc ���ڵ��� QSCT1
		//if(CNetBarConfig::GetInstance()->GetAuditType() == CIBAGlobal::emRealnameRZX)
		if(1 == CNetBarConfig::GetInstance()->GetDomainId() ||	CNetBarConfig::GetInstance()->GetAuditType() == CIBAGlobal::emRealnameJQ)
		{
			try
			{
			SendRegDataToProxy();
			}
			catch (...)
			{
				IBA_LOG(_T("��ʵ���ӿڷ���ע����Ϣ����! %d"), GetLastError());
			}

		}

		RegisterCenterUser();
		//}		

		if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
		{
			ResetNetworkBrokenControl();
		}
	}

	if(1 == CNetBarConfig::GetInstance()->GetIsShowRzxPhoto())
	{
		CString strPhotoPath;
		if(NS_REALNAME::CRZXRealName::GetPhotoPath(strPhotoPath, m_strPersonalID))
		{	
			CString strPath, strNewFile;
			strPath.Format(_T("%s\\%s"), theApp.GetWorkPath(), _T("UserImages"));
			CIBAHelpper::MakeSurePathExists(strPath);	// ȷ��·������
			strNewFile.Format(_T("%s\\%s.jpg"), strPath, CIBAHelpper::FormatNetId(m_strNetId));
			IBA_LOG(_T("��ȡ��������Ƭ·��%s, ����·��%s"), strPhotoPath, strNewFile);
			if(!CopyFile(strPhotoPath, strNewFile,FALSE))
			{
				IBA_LOG(_T("������������Ƭʧ��"));
			}
		}	
		else
		{
			IBA_LOG(_T("��ȡ��������Ƭʧ��"));
		}
	}
	
}

BOOL CRegisterDlg::CheckInput()
{
	//�����˺�

	//guoxuxing-2013-6-25: ���ǿ��ʹ���ֻ��ſ���
	if (CNetBarConfig::GetInstance()->GetMobileOpen())
	{
		if (m_strNetId.GetLength() != 11)
		{
			SetToolTipPos(IDC_EDIT_IDCARDSNO);
			ShowToolTip(IDS_INVALID_MOBILE);
			GetDlgItem(IDC_EDIT_IDCARDSNO)->SetFocus();
			return FALSE;
		}
	}
	else
	{
		if (m_strNetId.GetLength() < 6 || m_strNetId.GetLength() > 20)
		{
			SetToolTipPos(IDC_EDIT_IDCARDSNO);
			ShowToolTip(IDS_SNNOLONG);
			GetDlgItem(IDC_EDIT_IDCARDSNO)->SetFocus();
			return FALSE;
		}
	}
	

	if (m_chkTempMember.GetCheck() != BST_UNCHECKED)
	{
		return TRUE;
	}

	if(!m_strTelNum.IsEmpty())
	{
		for(int i = 0; i < m_strTelNum.GetLength(); i++)
		{
			if(
				!isdigit(m_strTelNum.GetAt(i)) &&
				!(m_strTelNum.GetAt(i) != '_') &&
				!(m_strTelNum.GetAt(i) != '-')
			  )
			{
				SetToolTipPos(IDC_EDIT_TELNUM);
				ShowToolTip(IDS_ERROR_TELNUM_TIPS);
				GetDlgItem(IDC_EDIT_TELNUM)->SetFocus();
				return FALSE;
			}
		}

		// ���ܴ���13λ
		if(m_strTelNum.GetLength() > 13)
		{
			SetToolTipPos(IDC_EDIT_TELNUM);
			ShowToolTip(IDS_ERROR_TELNUM_COUNT);
			GetDlgItem(IDC_EDIT_TELNUM)->SetFocus();
			return FALSE;
		}
	}
	else
	{
		if(0 != CNetBarConfig::GetInstance()->GetTelVerify())
		{
			CString strTips;
			strTips.LoadString(IDS_ERROR_TELNUM_EMPTY);
			GetDlgItem(IDC_EDIT_TELNUM)->SetFocus();
			SetToolTipPos(IDC_EDIT_TELNUM);
			ShowToolTip(strTips, TRUE);
			return FALSE;
		}
	}

	//����
	if (m_strUserName.GetLength() < 2)
	{
		SetToolTipPos(IDC_EDIT_USERNAME);
		ShowToolTip(IDS_ERRORUSERNAME);
		GetDlgItem(IDC_EDIT_USERNAME)->SetFocus();

		return FALSE;
	}

	if (m_strUserName.GetLength() > 10)
	{
		m_strUserName = m_strUserName.Left(10);
	}

	//֤������
	if (m_strPersonalID.IsEmpty())
	{
		SetToolTipPos(IDC_EDIT_PERSONALID);
		ShowToolTip(IDS_INPUTCERTIFICATENUM);
		GetDlgItem(IDC_EDIT_PERSONALID)->SetFocus();
		return FALSE;
	}
	else
	{
		// 2012/06/28-8246-gxx: ǿ��ɨ��Ҳ��֤.
		//if (CNetBarConfig::GetInstance()->GetMustScan() == 0) //���ǿ��ɨ��Ͳ���֤���֤
		{
			if (11 == m_nIdType || 12 == m_nIdType) //���֤Ҫ����֤
			{	
				CIdentityNumber IdentityNumber(m_strPersonalID);
				
				if (!IdentityNumber.IsValid()) //��Ч���֤
				{
					if (CIBAConfig::GetInstance()->GetCheckIDCardNumber()) //��������Ҫ�������֤
					{
						SetToolTipPos(IDC_EDIT_PERSONALID);
						ShowToolTip(IDS_IDNUMERROR);
						GetDlgItem(IDC_EDIT_PERSONALID)->SetFocus();

						return FALSE;
					}
				}
				else 
				{	
					// ����Ҫ�󿪻������֤��18����֤�� LimitAge =0������
					if(CNetBarConfig::GetInstance()->GetLimitAge() != 0)
					{
						if(IdentityNumber.GetRealAge() < CNetBarConfig::GetInstance()->GetLimitAge())
						{
							SetToolTipPos(IDC_EDIT_PERSONALID);
							ShowToolTip(_T("���û�δ���꣬��ֹ������"));
							GetDlgItem(IDC_EDIT_PERSONALID)->SetFocus();
							return FALSE;
						}
					}

					if (IdentityNumber.GetAge() < CIBAConfig::GetInstance()->GetRegisterAge())
					{
						CIBAString strTmp;
						strTmp.LoadString(IDS_NONAGEERROR);
						
						if (MsgBox(strTmp, MB_OKCANCEL) == IDCANCEL)
						{
							GetDlgItem(IDC_EDIT_PERSONALID)->SetFocus();

							return FALSE;
						} 
					}

					m_cboSex.SetCurSel(IdentityNumber.GetSex());

				} 
			}
		}
	}
	
	return TRUE;
}

BOOL CRegisterDlg::OnInitDialog()
{
	IBA_LOG(_T("Temp.CRegisterDlg::OnInitDialog - Start"));
	//CIBADialog::OnInitDialog();
	IBA_LOG(_T("Temp.CNDDialog::OnInitDialog - Start"));
	CNDDialog::OnInitDialog();
	IBA_LOG(_T("Temp.CNDDialog::OnInitDialog - End"));
	IBA_LOG(_T("Register dialog init start!"));

	if (!theApp.GetCurCashier()->MonitorAuditProxy())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	CIBAGlobal::m_hCurRegisterDlg = GetSafeHwnd();

	InitContrls();

	if (m_strPersonalID.IsEmpty())
	{
		GetDlgItem(IDC_EDIT_IDCARDSNO)->SetFocus();
	}
	else
	{
		m_edtMoney.SetFocus();
	}

	m_StaticPrehWnd = GetSafeHwnd();

	AddFocusEnter(IDC_EDIT_USERNAME, IDC_BUTTON_QUERY);
	AddFocusEnter(IDC_EDIT_PERSONALID, IDC_BUTTON_QUERY);

	UpdateData(FALSE);

	SetTimer(TimeIdOfAutoQuery,400,NULL);
	
	if (CNetBarConfig::GetInstance()->GetMobileOpen())
	{
		SetDlgItemText(IDC_EDIT_IDCARDSNO, _T(""));			// ����������ֻ��˺ţ�����ʲô����¶�����˺ſ�
	}	
	IBA_LOG0(_T("Register dialog init end!"));

	return FALSE;  
}

HBRUSH CRegisterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CNDDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (CTLCOLOR_EDIT == nCtlColor)
	{
		if (pWnd->GetDlgCtrlID() == IDC_EDIT_MONEY)
		{
			pDC->SetTextColor(RGB(255, 0, 0));
		}	
	}
	else if (CTLCOLOR_STATIC == nCtlColor)
	{
		if (pWnd->GetDlgCtrlID() == IDC_STATIC_RESULT)
		{
			pDC->SetTextColor(RGB(0, 0, 255));
		}
		else if (pWnd->GetDlgCtrlID() == IDC_STATIC_DEPOSIT)
		{
			pDC->SetTextColor(RGB(0, 0, 255));
		}
	}

	return hbr;
}

void CRegisterDlg::InitContrls()
{
	// �Ա�
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_SEX);
		pCombo->AddString(LOAD_STRING(IDS_MAN));
		pCombo->AddString(LOAD_STRING(IDS_WOMAN));
	}
	
	//�Ա�
	m_cboSex.SetCurSel(0);

	//֤������
	InitIdTypeComboBox();

	//�û�����
	InitUserClassComboBox();

	InitComputerComboBox();

	CIBAHelpper::CreateFont(m_MoneyFont, 28, 900);

	m_edtMoney.DisableMask();
	m_edtMoney.SetValidChars(_T("0123456789."));
	m_edtMoney.SetLimitText(6);
	m_edtMoney.SetFont(&m_MoneyFont);

	m_btnSefDefine.SizeToContent();

	GetDlgItem(IDC_STATIC0)->SetFont(&m_Font);

	//ǿ��ɨ��
	if (CNetBarConfig::GetInstance()->GetMustScan() == 1)
	{
		GetDlgItem(IDC_BUTTON_QUERY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_IDCARDSNO)->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_IDCARDSNO))->SetReadOnly(TRUE);

		EnableUserInfoControls(m_bCanEditUserInfo);

		SetTimer(TimerIdOfDisableInput, 300, NULL);
	}

#ifndef _DEBUG
	
	// 2011/09/28-8201-gxx: 
	if (m_cboTempUserClass.GetCount() == 0)
	{
		GetDlgItem(IDC_CHECK_TEMPMEMBER)->ShowWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_TEMPMEMBER)->ShowWindow(CNetBarConfig::GetInstance()->GetTempMember());
	}

#endif

	CWnd* pScanBtn = GetDlgItem(IDC_BUTTON_READID2);

	if (pScanBtn != NULL)
	{
	//QSC	pScanBtn->ShowWindow(theApp.GetRealName()->IsScanEnabled());
	}

	//ɨ��֤�����ܣ�ǿ��ɨ��= 2ʱ����
	if (CNetBarConfig::GetInstance()->GetMustScan() == 2)
	{
		GetDlgItem(IDC_BUTTON_READID2)->ShowWindow(SW_HIDE);
	}

	//�ظ���ӡСƱ��ť��ʼ����
	GetDlgItem(IDC_BUTTON_REPRINT)->ShowWindow(SW_HIDE);
	
	//����Ĭ�Ͻ��
	if (CIBAConfig::GetInstance()->GetRegisterMoney() > 0)
	{
		CString strTmpMoney;
		strTmpMoney.Format(_T("%.2f"), CIBAConfig::GetInstance()->GetRegisterMoney() / 100.0);
		m_edtMoney.SetWindowText(strTmpMoney);
	}

	//�û��Դ�������
	GetDlgItem(IDC_CHECK_USERHASCARD)->ShowWindow(CIBAConfig::GetInstance()->GetHasCard());
	
	//Ѻ��
	GetDlgItem(IDC_STATIC_DEPOSIT)->ShowWindow(CNetBarConfig::GetInstance()->IsUseDeposit());

	//�Զ������˺�
	if (m_StaticTmpMember)
	{
		m_chkTempMember.SetCheck(TRUE);
		OnBnClickedCheckTempmember();
	}

	//{ 2011/07/14-8201-gxx: 

	if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
	{
		ResetNetworkBrokenControl();
	}
	//}

	//��Ҫ����ʾ�ð�ť
	/*if (CNetBarConfig::GetInstance()->GetAuditType() == CIBAGlobal::emRealnameWD)
	{
		CWnd* pWnd = GetDlgItem(IDC_BTN_INPUT_NUMBER);
		if (pWnd)
		{
			pWnd->ShowWindow(TRUE);
		}
		else
		{
			IBA_ASSERT2(0,"�����쳣���ؼ�IDC_BTN_INPUT_NUMBER�Ѳ�����");
			IBA_LOG0(_T("CRegisterDlg �����쳣���ؼ�IDC_BTN_INPUT_NUMBER�Ѳ�����"));
		}
	}*/
	
	if (CNetBarConfig::GetInstance()->GetMobileOpen())
	{
		GetDlgItem(IDC_EDIT_IDCARDSNO)->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_IDCARDSNO))->SetReadOnly(FALSE);
	}

	if(1 == CNetBarConfig::GetInstance()->GetEnableJuBao() 
		&& !m_bFromCard && !m_bFromScan)
	{
		GetDlgItem(IDC_BTN_JUBAO)->ShowWindow(SW_SHOW);	
		((CEdit*)GetDlgItem(IDC_EDIT_IDCARDSNO))->SetReadOnly(TRUE);
		GetDlgItem(IDC_EDIT_IDCARDSNO)->EnableWindow(FALSE);
		EnableUserInfoControls(FALSE);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	}
	else 
	{
		GetDlgItem(IDC_BTN_JUBAO)->ShowWindow(SW_HIDE);	
	}


	if(1 != CNetBarConfig::GetInstance()->GetSoftSfreg())
	{
		GetDlgItem(IDC_BTN_RN_REG)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_PHOTO)->ShowWindow(SW_HIDE);
	}
	// 2014-4-25 - qsc ����ȷ�ϲ���Ҫʵ��ע�ᰴť�����ɾ����ť
	// QSCT1 
	GetDlgItem(IDC_BTN_RN_REG)->ShowWindow(SW_HIDE);

	if(0 != CNetBarConfig::GetInstance()->GetCannotSelfBundleTime())
	{
		GetDlgItem(IDC_BTN_CUSTOM_BOUNDLETIME)->EnableWindow(FALSE);
	}

	if((0 != CNetBarConfig::GetInstance()->GetTelVerify()) && (0 != CNetBarConfig::GetInstance()->GetHideTelInput()))	// �绰��֤
	{
		GetDlgItem(IDC_EDIT_TELNUM)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_8)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_QUERY_TEL)->ShowWindow(SW_SHOW);
	}
	else
	{
		if(0 == CNetBarConfig::GetInstance()->GetHideTelInput())
		{
			GetDlgItem(IDC_EDIT_TELNUM)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_8)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BUTTON_QUERY_TEL)->ShowWindow(SW_HIDE);
		}
	}

}

void CRegisterDlg::MakeUserPwd()
{
	//ʹ���κ�ʵ������(���������)
	m_strPassword.Trim();

	if(CNetBarConfig::GetInstance()->GetAuditType() == CIBAGlobal::emRealnameZD)
	{	
		// 2014-10-8 - qsc �Ƕ�ʵ��ʹ�ÿ�����
		m_strPassword.Empty();
		return ;
	}

	if (m_bFromCard)//ˢ���õ�����Ϣ
	{
		//������ù���������
		if (CNetBarConfig::GetInstance()->GetRealNamePassword() == 2)
		{
			return;
		}
		
		// 2014-8-28 - qsc ����ʵ��ʹ�ý�������
		if (CNetBarConfig::GetInstance()->GetAuditType() == CIBAGlobal::emRealnameJQ)
		{
			return;
		}

		//ʹ��ʵ�����룬��ǿ�ƣ����Ϊ����ʹ���Զ�������
		if (CNetBarConfig::GetInstance()->GetRealNamePassword() > 0)
		{
			if (!m_strPassword.IsEmpty())
			{
				return;
			}
		}
	}

	INT randNum = 0;

	INT nPwdType = CIBAConfig::GetInstance()->GetTempPasswordType();
	
	CString strTmp = CIBAConfig::GetInstance()->GetTempDefPassword();

	INT nTmp = m_cboUserClass.GetCurSel();

	if (!m_bIsTempUser) //��Ա
	{
		nPwdType = CIBAConfig::GetInstance()->GetPasswordType();
		strTmp = CIBAConfig::GetInstance()->GetDefPassword();
	}

	switch (nPwdType)
	{
	case CIBAGlobal::emPwdTypeRandom: //���

		srand(time(NULL));
		randNum = rand() % 1000;	
		strTmp.Format(_T("%.3d"), randNum);

		break;

	case CIBAGlobal::emPwdTypeIDTail6: //֤����6
		
		if (m_strPersonalID.GetLength() < 6)
		{
			strTmp.Format(_T("%.6s"), m_strPersonalID);
		}
		else
		{
			strTmp = m_strPersonalID.Right(6);
		}
		
		break;

	case CIBAGlobal::emPwdTypeFixed: //�̶�
		
		//strTmp = CIBAConfig::GetInstance()->GetDefPassword();

		break;

	case CIBAGlobal::emPwdTypeInput: // �û�����
		{
			CDlgPwd dlg;
			dlg.DoModal();
			strTmp = dlg.GetPwd();
		}		
		break;

	case CIBAGlobal::emPwdTypeEmpty: // ������---ֻ֧����ʱ�û�
		//if (m_bIsTempUser)
		{
			strTmp = _T(""); 
		}
		
		
		break;
	default:
		IBA_ASSERT2(FALSE, "�޷�ʶ�����������");
		break;
	}

	m_strPassword = strTmp;

	if (m_strPassword.IsEmpty() && nPwdType != CIBAGlobal::emPwdTypeEmpty )
	{
		if (m_bFromScan || m_bFromCard)//!<��Ϣ�Ƿ�����ɨ��|��ˢ��
		{
			//����Ϊ�գ���������
			MsgBox(IDS_PSWEMPTY);
			return;
		}
	}
}

void CRegisterDlg::PrintTicket()
{	
	if(0 == CNetBarConfig::GetInstance()->GetIsPrintA4())	// û�����ò���
	{
		PrintTicketPos();
	}
	else
	{
		PrintTicketA4();
	}
}

void CRegisterDlg::PrintTicket2()
{	
	CString strTicket, strTmp;

	CIBAPrint::GetRegisterAndBundleTimeTicket(strTicket);

	strTicket.Replace(_T("[name]"), m_strUserName);
	strTicket.Replace(_T("[idnumber]"), m_strPersonalID);

	if (CNetBarConfig::GetInstance()->GetRealNamePassword() >= 2)
	{
		strTicket.Replace(_T("[password]"), _T("*****"));
	}
	else
	{
		strTicket.Replace(_T("[password]"), m_strPassword);
	}
	strTicket.Replace(_T("[memberclass]"), m_strUserClassName);

	strTmp.Format(_T("%.2f"), m_nCreditMoney / 100.0);
	strTicket.Replace(_T("[money]"), strTmp);
	strTicket.Replace(_T("[cardserialnumber]"), m_strNetId);
	strTicket.Replace(_T("[netid]"), m_strNetId.Right(8));
	strTmp.Format(_T("%d"), m_nMemberId);
	strTicket.Replace(_T("[memberid]"), strTmp);

	strTicket.Replace(_T("[pcclass]"), m_strBundleTimeArea);
	strTmp.Format(_T("%.2f"), m_nBundTimeMoney / 100.0);
	strTicket.Replace(_T("[bundletimemoney]"), strTmp);
	strTicket.Replace(_T("[starttime]"), m_strBundleTimeStartTime.Mid(11, 5));
	strTicket.Replace(_T("[endtime]"), m_strBundleTimeEndTime.Mid(11, 5));
	strTicket.Replace(_T("[totalmoney]"), m_strBalanceAfterBundleTime);

	CIBAPrint::Print(strTicket);
}


void CRegisterDlg::PrintTicketPos()
{
	CString strTicket, strTmp;

	CIBAPrint::GetRegisterTicket(strTicket);

	strTicket.Replace(_T("[name]"), m_strUserName);
	strTicket.Replace(_T("[idnumber]"), m_strPersonalID);

	if (CNetBarConfig::GetInstance()->GetRealNamePassword() >= 2)
	{
		strTicket.Replace(_T("[password]"), _T("*****"));
	}
	else
	{
		strTicket.Replace(_T("[password]"), m_strPassword);
	}

	strTicket.Replace(_T("[memberclass]"), m_strUserClassName);

	strTmp.Format(_T("%.2f"), m_nCreditMoney / 100.0);
	strTicket.Replace(_T("[money]"), strTmp);
	strTicket.Replace(_T("[cardserialnumber]"), m_strNetId);
	strTicket.Replace(_T("[netid]"), m_strNetId.Right(8));

	strTmp.Format(_T("%d"), m_nMemberId);
	strTicket.Replace(_T("[memberid]"), strTmp);

	CIBAPrint::Print(strTicket);
}

void CRegisterDlg::PrintTicketA4()
{
	CIBAPrintA4::GetInstance()->SetOwner(theApp.m_pMainWnd);
	CIBAPrintA4::GetInstance()->m_strName = m_strUserName;
	CIBAPrintA4::GetInstance()->m_strNumber = m_strNetId;
	CIBAPrintA4::GetInstance()->m_strID = m_strPersonalID;
	CIBAPrintA4::GetInstance()->m_strAddress = m_userInfo.GetAddr();

	CChinaUnicomConfig::ClassIDItem item;
	if (CChinaUnicomConfig::GetInstance()->GetClassIDItem(m_nUserClassID, item))
	{
		CIBAPrintA4::GetInstance()->m_strContent[0] = _T("��Ʒ���ƣ�") + item.strName;
		CIBAPrintA4::GetInstance()->m_strContent[1] = _T("��Ʒ������") + item.strDetail;
		CIBAPrintA4::GetInstance()->m_strContent[2] = _T("���ʱ�䣺") + item.strTime;
	}

#ifdef DEBUG
	CIBAPrintA4::GetInstance()->DoPrintPriview();
#else
	CIBAPrintA4::GetInstance()->DoPrint();
#endif
}

INT_PTR CRegisterDlg::DoModal()
{
	if(m_StaticPrehWnd && ::IsWindow(m_StaticPrehWnd))
	{
		// �Ѿ����������������ˣ����ٴ���, ֱ�ӷ���
		return IDCANCEL;
	}
	INT_PTR nRet = CIBADialog::DoModal();
	m_StaticPrehWnd = NULL;
	return nRet;
}

void CRegisterDlg::ClearData()
{
	m_strNetId.Empty();
	m_strNetId2.Empty();
	m_strPassword.Empty();
	m_strPersonalID.Empty();
	m_strUserName.Empty();
}

BOOL CRegisterDlg::GetCardIdFromCenter()
{
	CString strTmp = m_strNetId;
	//���˺�ǰ�β���0
	if (strTmp.GetLength() < 10)
	{
		strTmp.Insert(0, CString(_T("0000000000")).Left(10 - strTmp.GetLength()));
	}

	BOOL bRet = FALSE;

	CIDCheckCardMethod checkCard;
	checkCard.SetSerialNum(strTmp);


	theApp.GetCurCashier()->DoCheckCard(checkCard);
	
	if (checkCard.GetStatusCode() == 0)
	{
		if (checkCard.GetMemberId() != 0) //�Ѿ������Ŀ�,��ֵ�˿�
		{
			SetToolTipPos(IDC_EDIT_IDCARDSNO);
			ShowToolTip(IDS_HASUSED);
		}
		else //�¿�ע��
		{
			SetCardId(checkCard.GetCardIdAsString());
			bRet = TRUE;
		}
	}
	else
	{
		SetToolTipPos(IDOK);
		ShowToolTip(checkCard.GetStatusMessage(), TRUE);
	}

	return bRet;
}

void CRegisterDlg::OnBnClickedButtonBundletime() 
{
	CDlgBundleTimeList dlg;

	GetRegisterInfo();

	dlg.SetUserClassId(m_nUserClassID);

	if (dlg.DoModal() == IDOK)
	{
		//���ݶԻ���ѡ������ѡ��

		for (INT i = 0; i < m_cboBundTimeSel.GetCount(); i++)
		{
			if (m_cboBundTimeSel.GetItemData(i) == dlg.GetBundleTimeId())
			{
				m_cboBundTimeSel.SetCurSel(i);
				break;
			}
		}

		OnCbnSelchangeComboBtsel();
	}
}

BOOL CRegisterDlg::IsBundleTime()
{
	if (0 == m_nBundleTimeId)
	{
		return FALSE;
	}

	CBundleTimeActionMethod BundleTimeAction;
	BundleTimeAction.SetMemberId(m_nMemberId);

	theApp.GetCurCashier()->DoBundleTimeAction(BundleTimeAction);

	if (BundleTimeAction.GetStatusCode() == 0 && BundleTimeAction.GetNetBarId() > 0)
	{
		CIBATTS::GetInstance()->SpeakBundleTimeRegisterResult(BundleTimeAction.GetAmount1(), m_nCreditMoney + m_nCreditPresent - BundleTimeAction.GetAmount1());

		m_strBundleTimeStartTime = CIBAHelpper::FormatCenterTime(BundleTimeAction.GetStartTime());
		m_strBundleTimeEndTime = CIBAHelpper::FormatCenterTime(BundleTimeAction.GetEndTime());
		//m_strBundleTimeMoney.Format(_T("%.2f"), BundleTimeAction.GetAmount1() / 100.0);
		m_nBundTimeMoney = BundleTimeAction.GetAmount1();
		m_strBundleTimeArea = CNetBarConfig::GetInstance()->GetPCClassName(BundleTimeAction.GetPcClass());
		m_strBalanceAfterBundleTime.Format(_T("%.2f"), (m_nCreditMoney + m_nCreditPresent -  BundleTimeAction.GetAmount1()) / 100.0);

		if (m_strBundleTimeArea.IsEmpty())
		{
			m_strBundleTimeArea = LOAD_STRING(IDS_NOLIMITAREA);
		}

		ShowRegisterBundleTimeResult();
		if (m_bPrintTicket)
		{
			PrintTicket2();
		}

		return TRUE;
	}

	return FALSE;
}

void CRegisterDlg::OnBnClickedButtonReadid2()
{
	CMenu menu;
	menu.LoadMenu(IDR_MENU_SCAN);
	CMenu* pPopup = menu.GetSubMenu(0);

	CRect rect;
	GetDlgItem(IDC_BUTTON_READID2)->GetWindowRect(rect);
	
	//// 2014-6-26 - qsc �Ƴ�����ȡ�������ŵ���ť����ʵ��
	//pPopup->RemoveMenu(IDM_READ2, MF_BYCOMMAND);
	// 2013-8-2 qsc ���������ʵ������ɾ��"ɨ������֤��"�˵�
	if(CIBAGlobal::emRealnameWD == CNetBarConfig::GetInstance()->GetAuditType())
	{
		pPopup->RemoveMenu(ID_SCAN_OTHER, MF_BYCOMMAND);
	}
	
	theApp.GetContextMenuManager()->ShowPopupMenu(pPopup->Detach(), rect.CenterPoint().x, rect.CenterPoint().y, this, TRUE);
}

void CRegisterDlg::OnScan1()
{
	ScanReco(CIBAGlobal::SRT_SCAN1);
}

void CRegisterDlg::OnScan2()
{
	ScanReco(CIBAGlobal::SRT_SCAN2);
}

void CRegisterDlg::OnScanPassport()
{
	ScanReco(CIBAGlobal::SRT_SCAN_PASSPORT);
}

void CRegisterDlg::OnScan2Back()
{
	CWaitCursor Wait;

	CNetBarUser NetBarUser;

	if (theApp.GetRealName()->ScanUserInfo(CIBAGlobal::SRT_SCAN2_BACK, NetBarUser, TRUE))
	{
		m_strOfficer = NetBarUser.GetDepartment();
		//NetBarUser.GetSignDate();
		m_strOfficer =NetBarUser.GetDepartment();

		CString strTable, strShow;
		m_strOfficer.Trim();
		if(m_strOfficer.IsEmpty())
		{
			strShow.LoadString(IDS_SCAN2_BACK_FAILED);
		}
		else
		{
			strTable.LoadString(IDS_SCAN2_BACK_SUCCESS);
			strShow.Format(strTable, m_strOfficer);
		}
		

		SetToolTipPos(IDC_BUTTON_READID2);
		ShowToolTip(strShow);
	}
	else
	{
		SetToolTipPos(IDC_BUTTON_READID2);
		ShowToolTip(IDS_SCANFAILED);
	}
}

void CRegisterDlg::OnRead2()
{
	ScanReco(CIBAGlobal::SRT_READ2);
}

void CRegisterDlg::OnScanOther()
{
	ScanReco(CIBAGlobal::SRT_SCAN_OTHER);
}

void CRegisterDlg::ScanReco(UINT nType)
{
	CWaitCursor Wait;

	CNetBarUser NetBarUser;

	if (theApp.GetRealName()->ScanUserInfo(nType, NetBarUser, TRUE))
	{
		m_strPersonalID = NetBarUser.GetCertificateID();
		m_strUserName = NetBarUser.GetUserName();
		m_strNation = NetBarUser.GetNation();
		m_strOfficer = NetBarUser.GetDepartment();
		
		if (!m_strNetId.IsEmpty() && CNetBarConfig::GetInstance()->GetAuditType() == 0) 
		{
			//�����˺Ų�Ϊ����û��ʵ�����򲻸ı������˺�
		}
		else
		{
			// qscadd 2013-10-25
			if(1 == CNetBarConfig::GetInstance()->GetEnableAccount12())
			{
				CString strCID = NetBarUser.GetCertificateID();
				if(12 < strCID.GetLength() && 
					(NetBarUser.GetCertificateType() == 11 ||
					NetBarUser.GetCertificateType() == 12))
				{
					NetBarUser.SetNetId(strCID.Right(12));
					m_strNetId = strCID.Right(12);
				}
			}
			else
			{
				m_strNetId = m_strPersonalID;
			}
		}

		m_cboSex.SetCurSel(NetBarUser.GetSex());

		if (NetBarUser.GetCertificateType() == 12)
		{
			if (CB_ERR == m_cboIDType.SelectString(0, _T("����֤")))
			{
				m_cboIDType.SelectString(0, _T("���֤"));
			}
		}
		else if (NetBarUser.GetCertificateType() == 99)
		{
			m_cboIDType.SelectString(0, _T("����֤��"));
			m_bNetIdFromPersonalId = TRUE;
		}
		else
		{
			m_cboIDType.SelectString(0, _T("���֤"));
		}

		m_bCanEditUserInfo = NetBarUser.GetCanEditUserInfo();
		
		EnableUserInfoControls(m_bCanEditUserInfo);

		UpdateData(FALSE);

		m_bFromScan = TRUE;
		m_bFromCard = FALSE;
		m_edtMoney.SetFocus();

		SetUserInfo(NetBarUser);
	}
	else
	{
		if(CIBAGlobal::SRT_READ2 == nType)
		{
			SetToolTipPos(IDC_BUTTON_READCARD);
			ShowToolTip(IDS_READCARD_FAILED);
			
		}
		else
		{
			SetToolTipPos(IDC_BUTTON_READID2);
			ShowToolTip(IDS_SCANFAILED);
		}	
	}
}

void CRegisterDlg::SetUserInfo(CNetBarUser& NetBarUser)
{
	m_bFromCard = FALSE;
	m_bFromScan = FALSE;

	if (NetBarUser.GetDataFrom() == 1)
	{
		m_bFromCard = TRUE;
	}
	else if (NetBarUser.GetDataFrom() == 2)
	{
		m_bFromScan = TRUE;
	}
	
	m_bCanEditUserInfo = NetBarUser.GetCanEditUserInfo();
	m_nUserSex = NetBarUser.GetSex();

	m_strUserName = NetBarUser.GetUserName();
	m_strPersonalID = NetBarUser.GetCertificateID();
	m_nIdType = NetBarUser.GetCertificateType();
	m_bIsMember = NetBarUser.GetIsMember();
	m_strPassword = NetBarUser.GetPassword();

	m_strNetId = NetBarUser.GetNetId();

	m_strNetId2 = m_strNetId;

	m_strOfficer = NetBarUser.GetDepartment();
	m_strNation = NetBarUser.GetNation();

	m_userInfo = NetBarUser;

	// 2014-10-29 - qsc
	// ���֤�Ա��Լ����㣬 ��Ϊɨ��������п��ܲ�׼ȷ
	if (11 == m_nIdType || 12 == m_nIdType) //���֤Ҫ����֤
	{	
		CIdentityNumber IdentityNumber(m_strPersonalID);

		if (IdentityNumber.IsValid()) //��Ч���֤
		{
			m_nUserSex = IdentityNumber.GetSex();
		}
	}
}

//����ˢ����Ϣ

LRESULT CRegisterDlg::OnRealName(WPARAM wp, LPARAM lp)
{
	CNetBarUser* pNetBarUser = (CNetBarUser*)lp;

	SetUserInfo(*pNetBarUser);

	SelectIDType();

	EnableUserInfoControls(m_bCanEditUserInfo);

	UpdateData(FALSE);

	return 0;
}

void CRegisterDlg::SelectIDType()
{
	for (INT i = 0; i < m_cboIDType.GetCount(); i++)
	{
		if (m_cboIDType.GetItemData(i) == m_nIdType)
		{
			m_cboIDType.SetCurSel(i);
			break;
		}
	}

	m_cboSex.SetCurSel(m_nUserSex);
}

void CRegisterDlg::OnEnSetfocusEditIdcardsno()
{
	if (m_bAfterRegister)
	{
		m_bAfterRegister = FALSE;

		m_strUserName.Empty();
		m_strPersonalID.Empty();
		m_strNetId.Empty();
		m_strOfficer.Empty();
		m_strNation.Empty();
		m_strTel.Empty();

		UpdateData(FALSE);
		
		m_nIdType = CIBAConfig::GetInstance()->GetDefIDTypeId();
		SelectIDType();

		m_cboUserClass.SetCurSel(0);
		m_cboTempUserClass.SetCurSel(0);

		//��ӡСƱ��״̬����
		GetDlgItem(IDC_BUTTON_REPRINT)->ShowWindow(SW_HIDE);
	}
}

void CRegisterDlg::SaveRegisterInfo()
{
	CCardOpenInfo CardOpenInfo;

	CardOpenInfo.SetClassId(m_nUserClassID);
	CardOpenInfo.SetOperationDateTime(CIBAHelpper::FormatCenterTime(UserRegister.GetTrantime()));
	CardOpenInfo.SetRefNo(UserRegister.GetRefNo());
	CardOpenInfo.SetOperator(theApp.GetCurCashier()->GetName());
	CardOpenInfo.SetNetBarId(CNetBarConfig::GetInstance()->GetNetBarId());
	CardOpenInfo.SetCashRegisterID(CIBAConfig::GetInstance()->GetCashRegisterID());
	CardOpenInfo.SetMemberId(UserRegister.GetMemberId());
	CardOpenInfo.SetSerialNo(UserRegister.GetSerialNo());
	CardOpenInfo.SetCostExpense(m_nCostExpense);
	CardOpenInfo.SetDeposit(m_nGuarantyMoney);
	CardOpenInfo.SetCardId(_ttoi(m_strCardId));
	CardOpenInfo.SetIdType(m_nIdType);
	CardOpenInfo.SetIdNumber(m_strPersonalID);
	CardOpenInfo.SetSerialNum(m_strNetId);
	CardOpenInfo.SetUserName(m_strUserName);
	CardOpenInfo.SetUserSex(m_cboSex.GetCurSel());

	CIBADAL::GetInstance()->AddCardOpenRecord(CardOpenInfo);

	//******************************************************************************

	CCreditInfoDB CreditInfo;

	CreditInfo.SetCardSerial(m_strNetId);
	CreditInfo.SetCreditDate(CIBAHelpper::FormatCenterTime(UserRegister.GetTrantime()));
	CreditInfo.SetRefNo(UserRegister.GetRefNo1());
	CreditInfo.SetCreditPresent(UserRegister.GetCreditPresent());
	CreditInfo.SetCreditAmount(m_nCreditMoney);
	CreditInfo.SetOperator(theApp.GetCurCashier()->GetName());
	CreditInfo.SetNetBarId(CNetBarConfig::GetInstance()->GetNetBarId());
	CreditInfo.SetCashRegisterID(CIBAConfig::GetInstance()->GetCashRegisterID());
	CreditInfo.SetMemberId(UserRegister.GetMemberId());
	CreditInfo.SetSerialNo(UserRegister.GetLocalSerialNo());
	CreditInfo.SetCassId(m_nUserClassID);

	CIBADAL::GetInstance()->AddCreditRecord(CreditInfo);

	CIBADAL::GetInstance()->UpdateMemberInfo(UserRegister.GetMemberId(),
		m_strNation,
		m_strOfficer,
		m_userInfo.GetAddr(),
		m_strTelNum);
}

void CRegisterDlg::GetRegisterInfo()
{
	INT nTmp = m_cboIDType.GetCurSel();
	m_nIdType = m_cboIDType.GetItemData(nTmp);

	if (m_bIsTempUser)
	{
		nTmp = m_cboTempUserClass.GetCurSel();
		nTmp = m_cboTempUserClass.GetItemData(nTmp);
	}
	else
	{
		nTmp = m_cboUserClass.GetCurSel();
		nTmp = m_cboUserClass.GetItemData(nTmp);
	}

	m_strUserClassName = CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(nTmp).GetClassName();
	m_nUserClassID = CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(nTmp).GetClassID();
	m_nCostExpense = CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(nTmp).GetCostExpense();
	m_nGuarantyMoney = CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(nTmp).GetDeposit();
}

BOOL CRegisterDlg::CheckMoney()
{
	SetToolTipPos(IDC_EDIT_MONEY);

	CString strMoney;
	m_edtMoney.GetWindowText(strMoney);

	if (strMoney.IsEmpty())
	{
		ShowToolTip(IDS_MONEYERROR);
		m_edtMoney.SetFocus();
		return FALSE;
	}

	double dblMoney = _tstof(strMoney);

	//��С�����3λ�������룬��ֹ����
	INT nMoney = (dblMoney + 0.005) * 100; //ת���ɷ�
	//�Դ�����Ϊ�˷��㴦�������Ѻ�Ѻ������Ϊ0
	if (m_bUserHasCard)
	{
		m_nCostExpense = 0;
		m_nGuarantyMoney = 0;
	}

	// 2012/03/05-8238-gxx: 
	if (CIBAConfig::GetInstance()->GetMaxCreditMoney() != 0)
	{
		INT nMax = CIBAConfig::GetInstance()->GetMaxCreditMoney() * 100;
		if (nMoney > nMax)
		{
			ShowToolTip(IDS_OVERFLOW_MAXMONEY);
			m_edtMoney.SetFocus();
			m_edtMoney.SetSel(0, strMoney.GetLength());
			return FALSE;
		}
	}
	else
	{
		INT nMax = 10000*100;
		if (nMoney > nMax)
		{
			ShowToolTip(IDS_OVERFLOW_MAXMONEY2);
			m_edtMoney.SetFocus();
			m_edtMoney.SetSel(0, strMoney.GetLength());
			return FALSE;
		}
	}
	// --end

	///////////////////////////////////
	
	nMoney = nMoney - m_nCostExpense - m_nGuarantyMoney;

	m_nCreditMoney = nMoney;//��ֵ���

	if (m_nBundleTimeId > 0)
	{
		nMoney -= m_nBundTimeMoney;
	}

	if (nMoney < 0) // Ǯ����
	{
		ShowToolTip(IDS_NOENOUGHMONEY);
		m_edtMoney.SetFocus();
		return FALSE;
	}

	return TRUE;
}

void CRegisterDlg::SetRegisterInfo()
{
	UserRegister.SetUserClass(m_nUserClassID);
	if (m_userInfo.GetCertificateType() == 98)
	{
		// ��������֤Ҳ�Ƕ���֤
		m_nIdType = 12; 
	}
	UserRegister.SetIdType(m_nIdType);
	UserRegister.SetPersonalId(m_strPersonalID);
	UserRegister.SetCountry(_T("CN"));
	UserRegister.SetUserName(m_strUserName);
	UserRegister.SetSex(m_cboSex.GetCurSel());
	UserRegister.SetCostExpense(m_nCostExpense);//�����Դ���Ӱ�죬��Ϊ��Ϊ0
	UserRegister.SetGuarantyMoney(m_nGuarantyMoney);

	UserRegister.SetCardId(m_strCardId);
	UserRegister.SetNoCard(m_bUserHasCard);
	//UserRegister.SetNoCard(0); //�̶���Ѻ��
	UserRegister.SetNoPresent(m_bNoPresent);
	UserRegister.SetPassword(m_strPassword);
	
	UserRegister.SetCreditAmount(m_nCreditMoney);
	UserRegister.SetAuditId(m_strNetId);

	//��������ļ����ֶ�
	UserRegister.SetTel(m_strTel);
	UserRegister.SetIdDepart(m_strOfficer);
	UserRegister.SetJob(m_strNation);
	UserRegister.SetAddress(m_userInfo.GetAddr());

	// ���ü���ˢ������
	UserRegister.SetActivationType(m_userInfo.GetActivationType());

	if(!m_strTelNum.IsEmpty())
		UserRegister.SetTel(m_strTelNum);
	
	if (m_DefineBundleTimeDlg.m_BundleTimeInfo.bIsSelected)
	{
		UserRegister.SetTimeId( m_DefineBundleTimeDlg.m_BundleTimeInfo.TimeId);
		UserRegister.SetAccountType( m_DefineBundleTimeDlg.m_BundleTimeInfo.AccountType);
		UserRegister.SetPcClass( m_DefineBundleTimeDlg.m_BundleTimeInfo.PcClass );
		UserRegister.SetTimePass( m_DefineBundleTimeDlg.m_BundleTimeInfo.TimePass );
		UserRegister.SetBeginTime( m_DefineBundleTimeDlg.m_BundleTimeInfo.BeginTime );
		UserRegister.SetBundleAmount( m_DefineBundleTimeDlg.m_BundleTimeInfo.Amount);

		m_nBundleTimeId = m_DefineBundleTimeDlg.m_BundleTimeInfo.TimeId;
		m_nBundTimeMoney = m_DefineBundleTimeDlg.m_BundleTimeInfo.Amount;
	}
	else
	{
		UserRegister.SetTimeId(m_nBundleTimeId);
	}
}

void CRegisterDlg::ShowRegisterResult(BOOL bLocalUser)
{
	CIBATTS::GetInstance()->SpeakRegisterResult(m_nCreditMoney, m_nCreditPresent);
	COperationInfo OperationInfo;
	if (bLocalUser)
	{
		OperationInfo.SetOperationType(CIBAGlobal::emOpTypeOpenLocal);
	}
	else
	{
		OperationInfo.SetOperationType(CIBAGlobal::emOpTypeOpen);
	}
		
	OperationInfo.SetIsBundTime(FALSE);
	
	CString strTmp;
	m_cboSex.GetWindowText(strTmp);
	strTmp.Insert(0, '(');
	strTmp.AppendChar(')');
	strTmp.Insert(0, m_strUserName);

	OperationInfo.SetUserName(strTmp);
	OperationInfo.SetNetId(m_strNetId);
	OperationInfo.SetMemberId(UserRegister.GetMemberId());
	OperationInfo.SetOperationAmount(m_nCreditMoney);
	OperationInfo.SetPresentAmount(UserRegister.GetCreditPresent());
	OperationInfo.SetDeposit(m_nGuarantyMoney);

	
	
	if (m_bIsTempUser) //ֻ��ʾ��ʱ�û�������
	{
		OperationInfo.SetPassWord(m_strPassword);
	}
	
	::SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM)&OperationInfo);
}

void CRegisterDlg::ShowRegisterBundleTimeResult()
{
	COperationInfo OperationInfo;
	OperationInfo.SetOperationType(3);
	OperationInfo.SetIsBundTime(TRUE);
	
	CString strTmp;
	m_cboSex.GetWindowText(strTmp);
	strTmp.Insert(0, '(');
	strTmp.AppendChar(')');
	strTmp.Insert(0, m_strUserName);

	OperationInfo.SetUserName(strTmp);
	OperationInfo.SetNetId(m_strNetId);
	OperationInfo.SetMemberId(UserRegister.GetMemberId());
	OperationInfo.SetOperationAmount(m_nCreditMoney);
	OperationInfo.SetDeposit(m_nGuarantyMoney);
	OperationInfo.SetPresentAmount(UserRegister.GetCreditPresent());
	OperationInfo.SetBundTimeAmount(m_nBundTimeMoney);

	if (m_bIsTempUser) //ֻ��ʾ��ʱ�û�������
	{
		OperationInfo.SetPassWord(m_strPassword);
	}

	::SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM)&OperationInfo);
}

void CRegisterDlg::InitData()
{
	//ʵ����ؿ��Ʊ���
	m_bCanEditUserInfo = (CNetBarConfig::GetInstance()->GetMustScan() == 0); //�Ƿ�༭�û���Ϣ
	m_bNetIdFromPersonalId = FALSE;
	m_bFromCard = FALSE;
	m_bFromScan = FALSE;

	//�Ʒ���ؿ��Ʊ���
	m_bUserHasCard = FALSE;
	m_bNoPresent = FALSE;
	m_nDefClassIndex = 0;
	m_nBundleTimeId = 0;
	m_nIdType = CIBAConfig::GetInstance()->GetDefIDTypeId();
	m_bIsMember = FALSE;
	m_nCreditPresent = 0;
	m_bAfterRegister = FALSE;
	m_nCreditMoney = 0;	
	m_nCreditPresent = 0;  
	m_nUserClassID = 0;	
	m_nCostExpense = 0;
	m_nGuarantyMoney = 0; 
	m_nMemberId = 0;
	m_nUserSex = 0;
	m_nBundleTimeType = 0;
	m_nBundTimeClass = 0;

	m_nBundTimeMoney = 0;

}

void CRegisterDlg::AutoUploadImage()
{
	CNetBarUser NetBarUser;
	NetBarUser.SetNetId(m_strPersonalID);
	NetBarUser.SetCertificateID(m_strPersonalID);
	NetBarUser.SetCertificateType(m_nIdType);
	NetBarUser.SetUserName(m_strUserName);

	theApp.GetRealName()->UploadImage(0, NetBarUser, m_bFromScan);

	m_bFromScan = FALSE;
}

void CRegisterDlg::OnBnClickedCheckTempmember()
{
	if (BST_UNCHECKED != m_chkTempMember.GetCheck())
	{
		MakeTmpMemberInfo();
		
		//GetDlgItem(IDC_EDIT_TELNUM)->EnableWindow(FALSE);
		UpdateData(FALSE);
	}
	else
	{
		m_cboIDType.EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PERSONALID)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_IDCARDSNO)->EnableWindow(TRUE);
		//GetDlgItem(IDC_EDIT_TELNUM)->EnableWindow(TRUE);
	}
	
	//��̬
	m_StaticTmpMember = (BST_UNCHECKED != m_chkTempMember.GetCheck());

	m_edtMoney.SetFocus();
}

void CRegisterDlg::OnBnClickedButtonReprint()
{
	if (m_nBundleTimeId == 0)
	{
		PrintTicket();
	}
	else
	{
		PrintTicket2();
	}
}

void CRegisterDlg::OnBnClickedCheckPrint()
{
	UpdateData(TRUE);

	if (m_bPrintTicket && m_bAfterRegister)
	{
		GetDlgItem(IDC_BUTTON_REPRINT)->ShowWindow(SW_NORMAL);
	} 
	else
	{
		GetDlgItem(IDC_BUTTON_REPRINT)->ShowWindow(SW_HIDE);
	}
}

void CRegisterDlg::OnBnClickedButtonQuery()
{
	UpdateData();

	CDlgMemberInfo dlg;
	dlg.SetQueryStyle(0);
	dlg.SetNetId(m_strNetId);
	dlg.SetPersonalID(m_strPersonalID);
	dlg.SetUserName(m_strUserName);

	if (dlg.DoModal() == IDOK)
	{
		m_strUserName = dlg.GetUserName();
		m_strPersonalID = dlg.GetPersonalID();

		m_cboSex.SelectString(-1, dlg.GetSex());
		m_cboIDType.SelectString(-1, dlg.GetIDType());
		m_cboUserClass.SelectString(-1, dlg.GetUserClass());
	}
	UpdateData(FALSE);
}

void CRegisterDlg::OnCbnSelchangeComboBtsel()
{
	m_nBundleTimeId = m_cboBundTimeSel.GetItemData(m_cboBundTimeSel.GetCurSel());
	
	CFullBundleTimeInfo BundleTimeInfo;

	if ( theApp.GetCurCashier()->HasPopedomOf(_T("IBAP_CUSTOMBUNDLE")) &&
		m_cboBundTimeSel.GetCurSel() == 1 ) //�Զ���
	{
		// 2011-4-15-gxx-���
		OnBnClickedBtnCustomBoundletime();
	}
	else if (m_nBundleTimeId > 0 &&
		CNetBarConfig::GetInstance()->m_NetbarBundleTime.GetBundleTimeInfo(
		m_nUserClassID, m_nBundleTimeId, BundleTimeInfo))
	{
		m_nBundTimeClass = BundleTimeInfo.IsNowEnabled() ? 0 : 1;

		//��ʼʱ��
		m_strBundleTimeStartTime = BundleTimeInfo.GetStartTime(TRUE).Left(5);
		//����ʱ��
		m_strBundleTimeEndTime = BundleTimeInfo.GetEndTime(TRUE).Left(5);
		//��ʱ�۷�����
		m_nBundleTimeType = BundleTimeInfo.GetBundleTimeType();
		//��ʱ���
		m_nBundTimeMoney = BundleTimeInfo.GetBundleTimeMoney();

		CString strBundleTimeMoney;
		strBundleTimeMoney.Format(_T("%.2f"), (m_nBundTimeMoney + m_nGuarantyMoney) / 100.0);
		m_edtMoney.SetWindowText(strBundleTimeMoney);

		// 2011-4-15-gxx
		m_DefineBundleTimeDlg.m_BundleTimeInfo.bIsSelected = FALSE;
	}
	else //û��ѡ��ʱ
	{
		m_edtMoney.SetWindowText(_T(""));
		m_cboBundTimeSel.SetCurSel(0);

		m_nBundleTimeId = 0;
		m_nBundleTimeType = 0;
		m_nBundTimeClass = 0;
	
		m_strBundleTimeStartTime.Empty();
		m_strBundleTimeEndTime.Empty();
		m_nBundTimeMoney = 0;
		m_strBundleTimeArea.Empty();
		m_strBalanceAfterBundleTime.Empty();

		// 2011-4-5-gxx
		m_DefineBundleTimeDlg.m_BundleTimeInfo.bIsSelected = FALSE;
	} 
}

void CRegisterDlg::EnableUserInfoControls(BOOL bEnable)
{
	GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_PERSONALID)->EnableWindow(bEnable);
	
	((CEdit*)GetDlgItem(IDC_EDIT_USERNAME))->SetReadOnly(!bEnable);
	((CEdit*)GetDlgItem(IDC_EDIT_PERSONALID))->SetReadOnly(!bEnable);

	m_cboIDType.EnableWindow(bEnable);
	m_cboSex.EnableWindow(bEnable);
}

void CRegisterDlg::OnTimer(UINT nIDEvent)
{
	CNDDialog::OnTimer(nIDEvent);

	if (TimerIdOfDisableInput == nIDEvent)
	{
		if (CNetBarConfig::GetInstance()->GetMobileOpen())
		{
			GetDlgItem(IDC_EDIT_IDCARDSNO)->EnableWindow(TRUE);
			((CEdit*)GetDlgItem(IDC_EDIT_IDCARDSNO))->SetReadOnly(FALSE);
		}
		else
		{
			GetDlgItem(IDC_EDIT_IDCARDSNO)->EnableWindow(FALSE);
		}
		

		if (m_bCanEditUserInfo)
		{
			if (m_bNetIdFromPersonalId)
			{
				m_cboIDType.EnableWindow(FALSE);
				m_cboUserClass.SetCurSel(0);
				m_cboUserClass.EnableWindow(FALSE);
			}
		}
		else
		{
			EnableUserInfoControls(m_bCanEditUserInfo);
		}
	}
	if (TimeIdOfAutoQuery == nIDEvent)
	{
		KillTimer(nIDEvent);

		// �ٴλ�ȡ����
		if (theApp.GetRealName()->RobFocusAfterSwipingCard())
		{
			SimulateClickMe();	
		}

		// 2012/05/15-8243-gxx: 
		if (m_bPhoneNumberOpen)
		{
			OnBnClickedBtnInputNumber();
		}

#ifndef _DEBUG
		//SetWindowPos(&CWnd::wndTopMost, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);

		HWND hWnd = theApp.GetMainWnd()->GetSafeHwnd();
		if(NULL != hWnd)
		{
			// �Ȱ���������Ϊ���
			::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE );  
			//::SetWindowPos(GetSafeHwnd(), hWnd, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

			// ��δ��뽫��ǰ���ڴ���������㣬������Ϊ�����
			HWND hForegdWnd = ::GetForegroundWindow();
			DWORD dwCurID = ::GetCurrentThreadId();
			DWORD dwForeID = ::GetWindowThreadProcessId(hForegdWnd, NULL);
			::AttachThreadInput(dwCurID, dwForeID, TRUE);
			::SetForegroundWindow(GetSafeHwnd());
			::AttachThreadInput(dwCurID, dwForeID, FALSE);


			// ���ý�������Ϊ���뽹��
			m_edtMoney.SetFocus();

		}

#endif
	}
}

BOOL CRegisterDlg::CheckRealNameData()
{
	BOOL bRet = TRUE;

	if (CNetBarConfig::GetInstance()->GetMustScan() == 1 
		&& theApp.GetRealName()->NeedToCheckRegisterData())
	{
		// 2012/04/23-8242-gxx:
		if (CNetBarConfig::GetInstance()->GetAuditType() == CIBAGlobal::emRealnameGLW &&
			
			(m_strPersonalID != m_userInfo.GetCertificateID() ||
			m_strUserName != m_userInfo.GetUserName()))
		{
			IBA_LOG0(_T("������Ϣ��ʵ��ˢ����Ϣ����"));
			theApp.IBAMsgBox(_T("�Ƿ��Ŀ�����Ϣ"));

			return FALSE;
		}
		//

		if (!(m_bFromCard || m_bFromScan))
		{
			CIBALog::GetInstance()->Write(_T("CheckScan failed!"));

			bRet = FALSE;
		}

		if (m_bFromScan) //ɨ������Ҫ��֤������������˺�Ҫһ��
		{
			if (m_strPersonalID.GetAt(0) == '0' || m_strPersonalID.GetLength() <= 10)
			{
				MsgBox(_T("֤�����볤�Ȳ���С��11�ҵ�һλ����Ϊ0��"));

				bRet = FALSE;

				return bRet;
			}

			if (m_strNetId.CollateNoCase(m_strPersonalID) != 0) //��һ��
			{
				bRet = FALSE;
			}
		}

		if (m_bFromCard) //����ˢ����ҪУ�鿨���Ƿ��б�
		{
			if (m_strNetId.CollateNoCase(m_strNetId2) != 0) //��һ��
			{
				bRet = FALSE;
			}
		}

		if (!bRet)
		{
			MsgBox(_T("����ˢʵ��������ɨ��֤����"));
		}
	}

	return bRet;
}

void CRegisterDlg::OnEnChangeEditPersonalid()
{
	if (m_bNetIdFromPersonalId)
	{
		UpdateData();
		m_strNetId = m_strPersonalID;
		UpdateData(FALSE);
	}
}

void CRegisterDlg::MakeTmpMemberInfo()
{
	((CButton*)GetDlgItem(IDC_RADIO_TEMPUSER))->SetCheck(TRUE);
	OnBnClickedRadioTempuser();

	//ʹĬ�ϵ���Ŀ��ԶΪ"����֤��"
	int nIndex(-1);
	for(int i=0;i< m_cboIDType.GetCount();++i)
	{
		if(99 == m_cboIDType.GetItemData(i))
		{
			nIndex = i;
		}
	}
	if(0 <= nIndex)
	{
		m_cboIDType.SetCurSel(nIndex);
	}
	else
	{
		m_cboIDType.SetCurSel(m_cboIDType.GetCount() - 1);
	}	
	m_cboIDType.EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PERSONALID)->EnableWindow(FALSE);
	//�����Զ��˺�
	GetDlgItem(IDC_EDIT_IDCARDSNO)->EnableWindow(FALSE);
	//�����ݿ���ȡ������
	m_strNetId = CIBADAL::GetInstance()->GetAutoNetIdFromRecord();

	m_strUserName = _T("��ͨ�û�");
	m_strPersonalID = m_strNetId;
}

void CRegisterDlg::OnCbnSelchangeComboUserclass()
{
	OnCbnSelchangeComboTempuserclass();
}

void CRegisterDlg::OnCbnSelchangeComboTempuserclass()
{
	GetRegisterInfo();

	CString strTmp;

	if (m_nGuarantyMoney > 0)
	{
		strTmp.Format(LOAD_STRING(IDS_REGDIPOSIT), m_nGuarantyMoney / 100.0);
	}

	GetDlgItem(IDC_STATIC_DEPOSIT)->SetWindowText(strTmp);

	InitBundleTimeComboBox();
}


void CRegisterDlg::OnBnClickedRadioTempuser()
{
	m_bIsTempUser = TRUE;
	m_cboUserClass.EnableWindow(FALSE);
	m_cboTempUserClass.EnableWindow(TRUE);
	OnCbnSelchangeComboTempuserclass();

	m_bPrintTicket = CIBAConfig::GetInstance()->GetPrintTicketOpenTemp();
	UpdateData(FALSE);
}

void CRegisterDlg::OnBnClickedRadioMember()
{
	m_bIsTempUser = FALSE;
	m_cboUserClass.EnableWindow(TRUE);
	m_cboTempUserClass.EnableWindow(FALSE);
	OnCbnSelchangeComboUserclass();

	m_bPrintTicket = CIBAConfig::GetInstance()->GetPrintTicketOpenMember();
	UpdateData(FALSE);
}

void CRegisterDlg::InitBundleTimeComboBox()
{
	#define NetbarBundleTime CNetBarConfig::GetInstance()->m_NetbarBundleTime.m_BundleTimeInfoArray

	m_cboBundTimeSel.ResetContent();

	CIBAString strTmp;
	strTmp.LoadString(IDS_NOTHING);
	m_cboBundTimeSel.SetItemData(m_cboBundTimeSel.AddString(strTmp), 0);
	m_cboBundTimeSel.SetCurSel(0);

	if (theApp.GetCurCashier()->HasPopedomOf(_T("IBAP_CUSTOMBUNDLE")))
	{
		m_btnSefDefine.ShowWindow(SW_SHOW);

		strTmp.LoadString(IDS_SELFDEFINEBUNDLETIME);
		m_cboBundTimeSel.SetItemData(m_cboBundTimeSel.AddString(strTmp), 9999);
	}

	for (INT i = 0; i < NetbarBundleTime.GetCount(); i++)
	{
		//�ж��û�����
		if (NetbarBundleTime.GetAt(i).GetClassId() != m_nUserClassID)
		{
			continue;
		}

		CString strBundleTimeInfo;

		strBundleTimeInfo.Format(LOAD_STRING(IDS_BTINFO),
			NetbarBundleTime.GetAt(i).GetBundleTimeId(),
			NetbarBundleTime.GetAt(i).GetStartTime(TRUE).Left(5),
			NetbarBundleTime.GetAt(i).GetEndTime(TRUE).Left(5),
			NetbarBundleTime.GetAt(i).GetTimePass(), 
			NetbarBundleTime.GetAt(i).GetBundleTimeMoney() / 100.0,
			NetbarBundleTime.GetAt(i).GetBundleTimeTypeString());

		m_cboBundTimeSel.SetItemData(m_cboBundTimeSel.AddString(strBundleTimeInfo), 
			NetbarBundleTime.GetAt(i).GetBundleTimeId());
	}
}

void CRegisterDlg::InitUserClassComboBox()
{
	BOOL bDefIsMember = FALSE;
	INT nDefIdx = 0;

	//����û�����

	INT nMemberCount = 0;
	INT nTempCount = 0;
	for (INT i = 0; i < CNetBarConfig::GetInstance()->m_UserClassArray.GetCount(); i++)
	{
		INT nIdx = 0;

		if (CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(i).GetIsMember())
		{
			nIdx = m_cboUserClass.AddString(CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(i).GetClassName());
			m_cboUserClass.SetItemData(nIdx, i);
			nMemberCount++;
		}
		else
		{
			nIdx = m_cboTempUserClass.AddString(CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(i).GetClassName());
			m_cboTempUserClass.SetItemData(nIdx, i);
			nTempCount++;
		}

		//Ĭ�ϼ���
		if (CIBAConfig::GetInstance()->GetDefUserClassID() == 
			CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(i).GetClassID())
		{
			nDefIdx = nIdx;
			bDefIsMember = CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(i).GetIsMember();
		}
	}

	//��ʱ�û�
	BOOL bHasTempUser = TRUE;

	if (m_cboTempUserClass.GetCount() == 0) //û��������ʱ�û�������Ͽ�
	{
		bHasTempUser = FALSE;
		m_cboTempUserClass.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_TEMPUSER)->ShowWindow(SW_HIDE);
	}
	else if (m_cboTempUserClass.GetCount() == 1) //ֻ��һ����ʱ�û��Ļ���������Ͽ�
	{
		m_cboTempUserClass.ShowWindow(SW_HIDE);
	}
	
	if (m_cboTempUserClass.GetCount() > 0)
	{
		m_cboTempUserClass.SetCurSel(0); //Ĭ��ѡ���һ��
	}

	//��Ա�û�

	if (m_cboUserClass.GetCount() == 0) //û�л�Ա�û�
	{
		GetDlgItem(IDC_RADIO_MEMBER)->ShowWindow(SW_HIDE);
		m_cboUserClass.ShowWindow(SW_HIDE);
	}
	else
	{
		m_cboUserClass.SetCurSel(0);//Ĭ��ѡ���һ��
	}

	//���ÿ���Ĭ�ϵ��û�����,�������ı�ѡ���¼�

	//{ 2011/09/28-8201-gxx: 
	if (nTempCount == 0 && nMemberCount > 0)
	{
		if (bDefIsMember)
		{
			m_cboUserClass.SetCurSel(nDefIdx);
		}
		((CBCGPButton*)GetDlgItem(IDC_RADIO_MEMBER))->SetCheck(TRUE);
		OnBnClickedRadioMember();

		m_chkTempMember.ShowWindow(FALSE);
	}
	else if(nTempCount > 0 && nMemberCount == 0)
	{
		if (nDefIdx >= nTempCount)
		{
			nDefIdx = 0;
		}
		m_cboTempUserClass.SetCurSel(nDefIdx);
		((CBCGPButton*)GetDlgItem(IDC_RADIO_TEMPUSER))->SetCheck(TRUE);
		OnBnClickedRadioTempuser();
	}
	else
	{
		if (bDefIsMember) //Ĭ���ǻ�Ա
		{
			m_cboUserClass.SetCurSel(nDefIdx);
			((CBCGPButton*)GetDlgItem(IDC_RADIO_MEMBER))->SetCheck(TRUE);
			OnBnClickedRadioMember();
		}
		else
		{
			m_cboTempUserClass.SetCurSel(nDefIdx);
			((CBCGPButton*)GetDlgItem(IDC_RADIO_TEMPUSER))->SetCheck(TRUE);
			OnBnClickedRadioTempuser();
		}
	}
	//}

	//{ 2013/05/03-gxx: 
	if (!CNetBarConfig::GetInstance()->IsEnableOpenMember())
	{
		GetDlgItem(IDC_RADIO_MEMBER)->ShowWindow(SW_HIDE);
		m_cboUserClass.ShowWindow(SW_HIDE);
		if (nTempCount > 0)
		{
			if (!bDefIsMember)
			{
				m_cboTempUserClass.SetCurSel(nDefIdx);
			}
			((CBCGPButton*)GetDlgItem(IDC_RADIO_TEMPUSER))->SetCheck(TRUE);
			OnBnClickedRadioTempuser();
		}
	}
	
	//}
	
	

	

}

void CRegisterDlg::InitIdTypeComboBox()
{
	for (INT i = 0; i < CNetBarConfig::GetInstance()->m_IDTypeInfoArray.GetCount(); i++)
	{
		int nIdx = m_cboIDType.AddString(CNetBarConfig::GetInstance()->m_IDTypeInfoArray.GetAt(i).GetIDTypeName());
		UINT nTmp = CNetBarConfig::GetInstance()->m_IDTypeInfoArray.GetAt(i).GetIDTypeID();
		m_cboIDType.SetItemData(nIdx, nTmp);
	}

	//���ÿ���Ĭ��֤������

	SelectIDType();
}

void CRegisterDlg::InitComputerComboBox()
{
	//������ʱ����Ͽ��Ƚ�������

	CBCGPComboBox* pTmpCbo = ((CBCGPComboBox*)GetDlgItem(IDC_COMBO_TEMPSORT));

	ASSERT(pTmpCbo);

	if (NULL == pTmpCbo)
	{
		return;
	}

	for (INT_PTR i = 0; i < CLocalServer::GetInstance()->ComputerList.GetCount(); i++)
	{
		CComputerInfo & computer = CLocalServer::GetInstance()->ComputerList.GetComputer(i);

		if (computer.GetComputerStatus() == CComputerInfo::ECS_OFFLINE
			|| computer.GetComputerStatus() == CComputerInfo::ECS_LOCKED)
		{
			INT nTmp = pTmpCbo->AddString(computer.GetTerminalID());

			pTmpCbo->SetItemData(nTmp, i);
		}
	}

	//ʵ�ʵ���Ͽ�����

	CIBAString strTmp;
	strTmp.LoadString(IDS_NOTHING);
	m_cboComputer.AddString(strTmp);

	for (INT i = 0; i < pTmpCbo->GetCount(); i++)
	{
		pTmpCbo->GetLBText(i, strTmp);
		
		INT nTmp = m_cboComputer.AddString(strTmp);
		m_cboComputer.SetItemData(nTmp, pTmpCbo->GetItemData(i));
	}

	//ɾ�����ߵ�

	CActiveMember ActiveMember;
	
	CSingleLock lock(&CLocalServer::GetInstance()->ActiveMemberList.ActiveMemberListLock, TRUE);
	
	CLocalServer::GetInstance()->ActiveMemberList.StartEnumActiveMember();

	while (CLocalServer::GetInstance()->ActiveMemberList.GetEachActiveMember(ActiveMember))
	{
		CString strFind;
		INT nTmp = m_cboComputer.SelectString(-1, ActiveMember.GetTerminalID());
		if (nTmp >= 0)
		{
			m_cboComputer.GetLBText(nTmp,strFind);
			if (strFind == ActiveMember.GetTerminalID())
			{
				m_cboComputer.DeleteString(nTmp);
			}
		}
	}

	m_cboComputer.SetCurSel(0);

	// 2012/03/21-8238-gxx: 
	if (CNetBarConfig::GetInstance()->GetRealNamePassword() == 4)
	{
		CWnd* pWnd = GetDlgItem(IDC_STATIC_6);
		if (pWnd)
		{
			pWnd->ShowWindow(FALSE);
		}
		m_cboComputer.ShowWindow(FALSE);
	}
}

/*************************************************
Function:       // AutoCheckIn
Description:    //
Calls:          //
Table Accessed: //
Table Updated:  //
Input:          //
                //
Output:         //
Return:         //
Others:         //
*************************************************/
void CRegisterDlg::AutoCheckIn()
{
	if (m_cboComputer.GetCurSel() > 0) //��û��ѡ�����
	{
		INT_PTR nIdx = m_cboComputer.GetItemData(m_cboComputer.GetCurSel());

		//�ǲ�����Ч�ĵ���

		if (!CLocalServer::GetInstance()->ComputerList.IsInvaildIndex(nIdx))
		{
			CComputerInfo & computer = CLocalServer::GetInstance()->ComputerList.GetComputer(nIdx);
			if (computer.GetComputerStatus() == CComputerInfo::ECS_OFFLINE
				|| computer.GetComputerStatus() == CComputerInfo::ECS_LOCKED)
			{
				{
					CActiveMember ActiveMember;
					if (CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(
						m_nMemberId, ActiveMember))
					{
						ActiveMember.SetTerminalID(computer.GetTerminalID());
						ActiveMember.SetPCClassID(computer.GetPCClassID());

						CLocalServer::GetInstance()->ActiveMemberList.UpdateActiveMember(ActiveMember, TRUE);
						CLocalServer::GetInstance()->ActiveMemberList.RefreshView();
					}
				}
				//��ӵ����ݿ�
				CLastUserInfo LastUserInfo;
				LastUserInfo.SetNetId(m_strNetId);
				LastUserInfo.SetMemberId(m_nMemberId);
				LastUserInfo.SetTermId(computer.GetTerminalID());
				LastUserInfo.SetLoginStyle(1); //ǿ�Ƶ�¼

				// 2014-10-27 - qsc �Ƕ�ʵ�����ֶ���������
				CString strPwd;
				if(CIBAGlobal::emRealnameZD != CNetBarConfig::GetInstance()->GetAuditType())
				{
					strPwd = CIBAHelpper::EncodePassword(m_strPassword);
				}
				else
				{
					CDlgInputUserPwd dlg;
					if(IDOK == dlg.DoModal())
					{
						strPwd = CIBAHelpper::EncodePassword(dlg.GetPwd(), false);
					}
					else
					{
						return ;
					}
				}

				

				LastUserInfo.SetPassWord(strPwd);
				LastUserInfo.SetUpdateTime(CIBAHelpper::GetCurrentTimeAsString(TRUE));

				CIBADAL::GetInstance()->DeleteLastUserInfo(LastUserInfo.GetTermId());
				CIBADAL::GetInstance()->AddLastUserInfo(LastUserInfo);

				//�����¼
				if (computer.GetComputerStatus() == CComputerInfo::ECS_LOCKED)
				{
					CLocalServer::GetInstance()->LocalAutoCheckIn(nIdx);
				}
			}
			else
			{
				MsgBox(IDS_TERMTAKEUP);
			}
		}
	}
}
void CRegisterDlg::OnBnClickedBtnCustomBoundletime()
{
	if (m_DefineBundleTimeDlg.DoModal() == IDOK)
	{
		if ( m_DefineBundleTimeDlg.m_BundleTimeInfo.bIsSelected )
		{
			IBA_ASSERT(m_DefineBundleTimeDlg.m_BundleTimeInfo.Amount<=999999);

			CString str;
			str.Format(_T("%.2f"),
				(m_DefineBundleTimeDlg.m_BundleTimeInfo.Amount + m_nGuarantyMoney) / 100.0);

			m_nBundTimeMoney = m_DefineBundleTimeDlg.m_BundleTimeInfo.Amount;

			m_edtMoney.SetWindowText(str);
		}

		m_cboBundTimeSel.SetCurSel(1);
	}
	else
	{
		m_cboBundTimeSel.SetCurSel(0);
	}
}

void CRegisterDlg::RegisterLocalUser()
{
	// 2011/07/26-8201-gxx: �ж������˺��Ƿ��ظ�
	CString strSQL;
	strSQL.Format(_T("select * from ActiveMember where netId ='%s'"), m_strNetId);

	//try
	//{
	CADODBConnInfo* pDb = NULL;

	if (!CIBADAL::GetInstance()->GetDBConnInfo(pDb)) return;

	CADORecordset Rs(pDb->GetConn());

	if (Rs.Open(strSQL) )
	{
		if (Rs.GetRecordCount() != 0)
		{
			theApp.IBAMsgBox(_T("����ʧ��,�����˺��ظ�"));

			Rs.Close();
			pDb->Release();
			return;
		}
		Rs.Close();	
	}
	pDb->Release();
	//}
	//catch(...){}

	CLocalRegisterInfo RegisterInfo;
	RegisterInfo.SetNetbarId( CNetBarConfig::GetInstance()->GetNetBarId() );
	RegisterInfo.SetIdType(m_nIdType);
	RegisterInfo.SetUserClass( m_nUserClassID );
	RegisterInfo.SetPersonalId( m_strPersonalID );
	RegisterInfo.SetUserName( m_strUserName );
	RegisterInfo.SetSex(m_cboSex.GetCurSel());
	RegisterInfo.SetPassword(m_strPassword);
	RegisterInfo.SetMemberId( CBrokenNetwork::RegisterMemberID() );

	CString strTmp = m_strNetId;
	//���˺�ǰ�β���0
	if (strTmp.GetLength() < 10)
	{
		strTmp.Insert(0, CString(_T("0000000000")).Left(10 - strTmp.GetLength()));
	}

	RegisterInfo.SetNetId( strTmp );
	RegisterInfo.SetMoney( m_nCreditMoney );

	//���ӵ��û���ͼ
	CCurrentNetBarUser::GetInstance()->SetNetId(strTmp);
	
	CLocalServer::GetInstance()->ActiveMemberList.AddActiveMember(RegisterInfo);

		//CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(FALSE);
	/*theApp.UpdateToolbarLastID(CCurrentNetBarUser::GetInstance()->
		Balance.GetSerailNum());*/   ////Temporarily comment out

	ShowRegisterResult(TRUE);

	m_nMemberId = RegisterInfo.GetMemberId();

	AutoCheckIn();

	// 2015/5/15 tmelody
	// ��¼������
	// {
	// ���챾�ؼ�¼��Ϣ
	USERINFO userInfo;
	userInfo._strAccount = m_strNetId;
	userInfo._strMemberID.Format(_T("%d"), m_nMemberId);
	userInfo._strPwd = m_strPassword;
	userInfo._strClassID.Format(_T("%d"), m_nUserClassID);
	userInfo._bLocalRegister = TRUE;
	
	// ��¼ע����Ϣ
	CLocalCenter::GetInstance()->RecordRegisteredUser(userInfo);
	// }

	CDialog::OnOK();
}

void CRegisterDlg::RegisterCenterUser()
{
	IBA_LOG0(_T("Enter ע�������û�..."));

	//if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkRestore)
	if (CIBAGlobal::NetworkStatus != CIBAGlobal::emNetworkBroken)
	{
		// 2011/07/26-8201-gxx: �ж������˺��Ƿ��ظ�����Ϊ��Member���ж�
		CString strSQL;
		strSQL.Format(_T("select * from ActiveMember where netId ='%s'"), m_strNetId);
		//strSQL.Format(_T("select * from member where serialNum ='%s'"), m_strNetId);

		//try
		//{
		CADODBConnInfo* pDb = NULL;

		if (CIBADAL::GetInstance()->GetDBConnInfo(pDb)) 
		{
			CADORecordset Rs(pDb->GetConn());

			if (Rs.Open(strSQL) )
			{
				if (Rs.GetRecordCount() != 0)
				{
					theApp.IBAMsgBox(_T("����ʧ��,�����˺��ظ�"));
					IBA_LOG0(_T("Leave ע�������û� ʧ��: ����ʧ��,�����˺��ظ�"));

					Rs.Close();
					pDb->Release();
					return;
				}
				Rs.Close();	
			}
			pDb->Release();
		}

	//	}
	//	catch(...){}
	}

	if (!GetCardIdFromCenter())
	{
		IBA_LOG0(_T("Leave ע�������û� ʧ��: �����ļ��鿨��"));
		return;//�����ļ��鿨��
	}

	SetRegisterInfo();//����ע�᷽����Ϣ

	theApp.GetCurCashier()->DoRegister(UserRegister);

	if (UserRegister.GetStatusCode() == 0) //�ɹ�
	{
		//���ӵ��û���ͼ
		CCurrentNetBarUser::GetInstance()->Balance.SetCardId(_ttoi(m_strCardId));
		CCurrentNetBarUser::GetInstance()->Balance.SetMemberId(UserRegister.GetMemberId());
		theApp.GetCurCashier()->DoBalance(CCurrentNetBarUser::GetInstance()->Balance);
		if (CCurrentNetBarUser::GetInstance()->Balance.GetStatusCode() == 0)
		{
			CLocalServer::GetInstance()->ActiveMemberList.AddActiveMember(
				CCurrentNetBarUser::GetInstance()->Balance,
				m_nBundleTimeId!=0,
				m_nBundTimeMoney,
				CCurrentNetBarUser::GetInstance()->Balance.GetPayType(),
				m_userInfo.GetScanType()
				);

			CCurrentNetBarUser::GetInstance()->SetNetId(CCurrentNetBarUser::GetInstance()->Balance.GetSerailNum());
			//CLocalServer::GetInstance()->GetActiveMemberView()->Refresh(FALSE);

			//theApp.UpdateToolbarLastID(CCurrentNetBarUser::GetInstance()->
			//	Balance.GetSerailNum());  ////Temporarily comment out
		}

		m_bNetIdFromPersonalId = FALSE;
		m_bCanEditUserInfo = (CNetBarConfig::GetInstance()->GetMustScan() == 0); //�Ƿ�༭�û���Ϣ

		m_bAfterRegister = TRUE;

		m_edtMoney.SetWindowText(_T(""));

		//�����ظ���ӡ��ť
		OnBnClickedCheckPrint();

		m_nMemberId = UserRegister.GetMemberId();
		m_nCreditPresent = UserRegister.GetCreditPresent();
		
		SaveRegisterInfo();//д��Ϣ�����ݿ�

		// 2015/5/15 tmelody
		// ��¼������
		// {
		USERINFO userInfo;
		userInfo._strAccount = m_strNetId;
		userInfo._strMemberID.Format(_T("%d"), m_nMemberId);
		userInfo._strPwd = m_strPassword;
		userInfo._strClassID.Format(_T("%d"), m_nUserClassID);
		userInfo._bLocalRegister = FALSE;
		CIBALog::GetInstance()->WriteFormat(_T("����ע��ɹ�------------"));
		CIBALog::GetInstance()->WriteFormat(_T("NetId - %s"),m_strNetId);
		CIBALog::GetInstance()->WriteFormat(_T("MemberId - %d"),m_nMemberId);		
		CLocalCenter::GetInstance()->RecordRegisteredUser(userInfo);

		if (!IsBundleTime())
		{
			ShowRegisterResult();//��ʾע����
			if (m_bPrintTicket)
			{
				PrintTicket();
			}
		}

		BOOL bLoad = TRUE;

		if (bLoad)
			AutoUploadImage();//�ϴ���Ƭ

		IBA_LOG0(_T("ע�������û� will end!"));
		// 2012/09/26-8249-gxx: ��������ר��
		// 2014-3-6 17:48:14 - qsc
		//OpenRServer();

		//try
		//{
		AutoCheckIn();
		//}
		//catch(...)
		//{
		//	IBA_LOG0(_T("�Զ��������"));
		//}

		/*try
		{*/
		AutoClose(IDOK);
		//}
	/*	catch(...)
		{
			IBA_LOG0(_T("�Զ��رնԻ������"));
		}*/
		
		/*AutoCheckIn();
		AutoClose(IDOK);*/
		CIDCheckCardMethod IDCheckCard;
		IDCheckCard.SetSerialNum(m_strNetId);
		IDCheckCard.SetSerialNumType(1);

		theApp.GetCurCashier()->DoCheckCard(IDCheckCard);

		if (IDCheckCard.GetStatusCode() == 0)
		{
			int nCardId = IDCheckCard.GetCardId();

			CBalanceMethod& Balance = CCurrentNetBarUser::GetInstance()->Balance;
			Balance.SetMemberId(m_nMemberId);
			Balance.SetCardId(nCardId);
			Balance.SetRound(0);

			theApp.GetCurCashier()->DoBalance(Balance);

			int nStatus = Balance.GetStatusCode();
			if (nStatus == 0)
			{
				int nTotal = Balance.GetTotalBalacne();
				int nRemain = Balance.GetBalance();
				int n = Balance.GetLastConsumeAmount();
				int m = 0;
			}
		}
	}
	else
	{
		SetToolTipPos(IDOK);
		ShowToolTip(UserRegister.GetStatusMessage(), TRUE);

		IBA_LOG(_T("ע����: %s"), UserRegister.GetStatusMessage());
	}

	IBA_LOG0(_T("Leave ע�������û�"));
}

void CRegisterDlg::ResetNetworkBrokenControl()
{
	GetDlgItem(IDC_RADIO_MEMBER)->ShowWindow(FALSE);
	GetDlgItem(IDC_COMBO_USERCLASS)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_GROUP)->ShowWindow(FALSE);
	GetDlgItem(IDC_COMBO_BTSEL)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_BUNDLETIME)->ShowWindow(FALSE);
	GetDlgItem(IDC_BTN_CUSTOM_BOUNDLETIME)->ShowWindow(FALSE);
	GetDlgItem(IDC_CHECK_PRESENT)->ShowWindow(FALSE);

	CIBAString str;
	str.LoadString(IDS_OPENLOCALUSER);
	SetWindowText(str);

	CEdit* pEdit = NULL;
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_IDCARDSNO);
	pEdit->EnableWindow(TRUE);
	pEdit->SetReadOnly(FALSE);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_USERNAME);
	pEdit->EnableWindow(TRUE);
	pEdit->SetReadOnly(FALSE);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PERSONALID);
	pEdit->EnableWindow(TRUE);
	pEdit->SetReadOnly(FALSE);
}

void CRegisterDlg::OnBnClickedBtnInputNumber()
{
	CDlgInputMobile dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_strNetId = dlg.m_strPhoneNumber;
		m_strPersonalID = dlg.m_strPhoneNumber;
		m_strUserName = _T("�ֻ�����");
		UpdateData(FALSE);

		m_bFromScan = TRUE;
		m_cboIDType.SelectString(0, _T("����֤��"));

		GetDlgItem(IDC_EDIT_MONEY)->SetFocus();
	}
}

void CRegisterDlg::SetPhoneNumberOpen()
{
	m_bPhoneNumberOpen = TRUE;
}

void CRegisterDlg::OpenRServer()
{
	CString strIniFile = theApp.GetWorkPath();
	strIniFile.Append(_T("\\IBAConfig\\nde_server.ini")); // base, Filename
	if (!::PathFileExists(strIniFile))
	{
		IBA_LOG(_T("�����ļ�: %s ������"), strIniFile);
		return;
	}
	TCHAR tRes[512] = {0};
	::GetPrivateProfileString(_T("base"), _T("Filename"), NULL, tRes, 512, strIniFile);
	if (!::PathFileExists(tRes))
	{
		IBA_LOG(_T("ִ���ļ�: %s ������"), tRes);
		return;
	}/*
	����û�:add|�û���|����

		ɾ���û�:del|�û���

*/
	CString strNetID = m_strNetId;
	if (strNetID.GetLength() < 10)
	{
		INT nFil = 10 - strNetID.GetLength();
		for (; nFil > 0; nFil --)
		{
			strNetID.Insert(0, _T('0'));
		}
	}
	INT nFil = 8 - m_strPassword.GetLength();
	CString strPwd = m_strPassword;
	for (; nFil > 0; nFil --)
	{
		strPwd.AppendChar(_T('0'));
	}

	CString strParam;
	strParam.Format(_T("add|%s|%s"), strNetID, strPwd);

	IBA_LOG(_T("ִ��: %s %s"), tRes, strParam);
	
	ShellExecute(NULL, _T("open"), tRes, strParam, NULL, SW_SHOWNORMAL);
}

void CRegisterDlg::OnBnClickedBtnCapphtoto()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(1 != CNetBarConfig::GetInstance()->GetEnableJuBao())
		return;
	// ��֤�����նԻ����������
	CDlgCapPhoto dlg;
	if(IDOK == dlg.DoModal())
	{
		dlg.GetNameID(m_strUserName, m_strPersonalID);
		m_strNetId = m_strPersonalID;
		GetDlgItem(IDC_EDIT_IDCARDSNO)->SetWindowText(m_strNetId);
		GetDlgItem(IDC_EDIT_USERNAME)->SetWindowText(m_strUserName);
		GetDlgItem(IDC_EDIT_PERSONALID)->SetWindowText(m_strNetId);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		UpdateData();
	}
}

void CRegisterDlg::OnBnClickedBtnRnReg()
{
	IBA_LOG0(_T("���ʵ��ע��"));
	m_bClickRnReg = TRUE;
	OnBnClickedOk();
}

LRESULT CRegisterDlg::OnCanClose(WPARAM wP,LPARAM lP)
{
	IBA_LOG0(_T("Enter CRegisterDlg::OnCanClose"));
	if(1 <= CNetBarConfig::GetInstance()->GetSoftSfreg() 
		&& TRUE == m_bWaitReg)
	{
		m_bWaitReg = FALSE;
		RegisterCenterUser();

		if (CIBAGlobal::NetworkStatus == CIBAGlobal::emNetworkBroken)
		{
			ResetNetworkBrokenControl();
		}

		if(1 == CNetBarConfig::GetInstance()->GetIsShowRzxPhoto())
		{
			CString strPhotoPath;
			if(NS_REALNAME::CRZXRealName::GetPhotoPath(strPhotoPath, m_strPersonalID))
			{	
				CString strPath, strNewFile;
				strPath.Format(_T("%s\\%s"), theApp.GetWorkPath(), _T("UserImages"));
				CIBAHelpper::MakeSurePathExists(strPath);	// ȷ��·������
				strNewFile.Format(_T("%s\\%s.jpg"), strPath, CIBAHelpper::FormatNetId(m_strNetId));
				IBA_LOG(_T("��ȡ��������Ƭ·��%s, ����·��%s"), strPhotoPath, strNewFile);
				if(!CopyFile(strPhotoPath, strNewFile,FALSE))
				{
					IBA_LOG(_T("������������Ƭʧ��"));
				}
			}	
			else
			{
				IBA_LOG(_T("��ȡ��������Ƭʧ��"));
			}
		}
	}
	IBA_LOG0(_T("Leave CRegisterDlg::OnCanClose"));
	return 0;
}

void CRegisterDlg::SendRegDataToProxy()
{
	UpdateData();
	if(!PathFileExists(theApp.GetWorkPath() + _T("\\IBABin\\AuditInterface.dll")))
	{
		return ;
	}

	CAuditInterface auditInterface;
	if(!auditInterface.Load())
	{
		IBA_LOG0(_T("����ʵ���ӿڶ�̬��AuditInterface.dllʧ�ܣ�����AuditInterface.dll�İ汾��"));
		return ;
	}
	auditInterface.uninit_();
	CStringA strIpA = CT2A(CNetBarConfig::GetInstance()->GetProxyIP());
	auditInterface.init_(strIpA, 7834);
	CString strSex;
	// �Ա�
	m_cboSex.GetLBText(m_cboSex.GetCurSel(), strSex);

	int isex;
	if (strSex == _T("��")) isex = 1;else isex  = 0;
	int nMemberId= 0;
	int nNation = NS_REALNAME::CRZXRealName::GetNationCode(m_strNation);
	UINT nNetbarId = CNetBarConfig::GetInstance()->GetNetBarId();
	COleDateTime dt;
	dt = COleDateTime::GetCurrentTime();
	CString strDatetime;
	strDatetime = dt.Format(_T("%Y-%m-%d"));

	CString validDate("2050-01-01");
	CString strBrithday = NS_REALNAME::CRZXRealName::GetBirthdayFromSFZ(m_strPersonalID);

	CStringA strPersonalIDA = CT2A(m_strPersonalID);
	CStringA strNameA = CT2A(m_strUserName);
	CStringA strAddrA = CT2A(m_userInfo.GetAddr());
	CStringA strTelNumA = CT2A(m_strTelNum);

	CStringA strBrithdayA = CT2A(strBrithday);
	CStringA strDatetimeA = CT2A(strDatetime);
	CStringA strValidDateA = CT2A(validDate);
	CStringA strOfficerA = CT2A(m_strOfficer);
	CStringA strNetIdA = CT2A(m_strNetId);

	int retcode = -1;
	try
	{
	int ret = auditInterface.regUser_(nNetbarId, m_nIdType, strPersonalIDA, strNameA, 
		isex, "CN", strAddrA, strTelNumA, nMemberId, strBrithdayA,
		strDatetimeA, strValidDateA, nNation, strOfficerA, m_nUserClassID, strNetIdA, &retcode);

	if (ret == 0)
	{
		if (retcode == 1 || retcode == -6 )
		{
			IBA_LOG0(_T("��ʵ���ӿڴ�ע����Ϣ�ɹ���������ʵ����!"));
		}
	}
	else if (ret == ERROR_NETWORK_FAILED)
	{
		IBA_LOG0(_T("�������,�ϴ��û���Ϣʧ��!"));
	}
	else
	{
		IBA_LOG0(_T("�����쳣!"));
	}
	}
	catch(...)
	{
		IBA_LOG(_T("��ʵ��ע���׳��쳣:%d"), GetLastError());
	}
	
}

void CRegisterDlg::ExeSfreg()
{
	IBA_LOG0(_T("CRegisterDlg::ExeSfreg"));
	UpdateData();
	// -A֤������	-B֤�����(����)   -C����		-D�Ա�
	// -E��ַ		-F�绰����		-G����		-H��Ա��
	// -I֤�������� -J����			-K��֤��λ	-L������ 
	// -Mӡˢ�� -R�Ƿ�ֻ��
	CString strParam, strIDType, strSex;

	// ��ȡ֤������
	m_cboIDType.GetLBText(m_cboIDType.GetCurSel(), strIDType);
	// �Ա�
	m_cboSex.GetLBText(m_cboSex.GetCurSel(), strSex);

	strParam.Format(_T("-A%s -B%s -C%s -D%s -E%s -F%s -G%s -H%d -I%d -J%s -K%s -L%d -M%s -R%d"), 
		m_strPersonalID, strIDType, m_strUserName, strSex,
		m_userInfo.GetAddr(), m_strTelNum, _T("CN"), 0, 
		m_nIdType, m_strNation, m_strOfficer, m_nUserClassID,
		m_strNetId, m_bCanEditUserInfo);
	CString str = m_userInfo.GetTel();
	IBA_LOG(_T("���ݸ�SFREG����Ϣ:%s"),strParam);
	const CString strSource = theApp.GetWorkPath() + _T("\\IBATemp\\ID.jpg");
	const CString strDest	  = theApp.GetWorkPath() + _T("\\IBABin\\sfreg\\imgID.jpg");

	if(PathFileExists(strSource))		// ����һ�������ݣ�����鿴
	{
		if(!CopyFile(strSource, theApp.GetWorkPath() + _T("\\IBATemp\\ID1.jpg"), FALSE))
		{
			IBA_LOG(_T("����%sʧ��1!,%d"), strSource, GetLastError());
		}
	}

	if(PathFileExists(strDest))
	{
		if(!DeleteFile(strDest))
		{
			IBA_LOG(_T("ɾ��%sʧ��!, %d"), strDest, GetLastError());
		}
	}

	if(PathFileExists(strSource))
	{
		if(!CopyFile(strSource, strDest, FALSE))
		{
			IBA_LOG(_T("����%sʧ��!,%d"), strSource, GetLastError());
		}
	}
	else
	{
		IBA_LOG(_T("%s�����ڣ�"), strSource);
	}

	if(PathFileExists(strSource))		// ������ɾ��
	{
		if(!DeleteFile(strSource))
		{
			IBA_LOG(_T("ɾ��%sʧ��!, %d"), strSource, GetLastError());
		}
	}
	
	CString strSFREGPath = theApp.GetWorkPath() + _T("\\IBABin\\sfreg\\sfreg.exe");
	if(CIBAHelpper::ExePath(strSFREGPath, strParam))
	{
		IBA_LOG0(_T("����sfreg�ɹ�"));
	}
	else
	{
		IBA_LOG0(_T("����sfregʧ��"));
	}
}


void CRegisterDlg::OnBnClickedButtonQueryTel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	CMemberInfoMethod MemberInfo;
	
	CString strTmp = m_strNetId;
	// 2014-3-18 - qsc ��0
	//���˺�ǰ�β���0
	if (strTmp.GetLength() < 10)
	{
		strTmp.Insert(0, CString(_T("0000000000")).Left(10 - strTmp.GetLength()));
	}

	MemberInfo.SetSerialNum(strTmp);
	MemberInfo.SetPersonalId( m_strPersonalID );
	MemberInfo.SetUsername( m_strUserName );
	MemberInfo.SetAvailable(0);// 0ȫ����ѯ, 1��Ч��ѯ
	theApp.GetCurCashier()->DoMemberInfo(MemberInfo);

	if(0 == MemberInfo.GetStatusCode() &&
		0 < MemberInfo.GetMemberCount())
	{
		m_strTelNum = MemberInfo.GetTel(0);
		UpdateData(FALSE);
	}
}

void CRegisterDlg::OnBnClickedBtnPhoto()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	CIBAHelpper::TakePhoto(CIBAHelpper::GetUserZcImgPath(m_nIdType, m_strPersonalID), m_strPersonalID);
}

void CRegisterDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	if(CIBAGlobal::emRealnameZD == CNetBarConfig::GetInstance()->GetAuditType())
	{
		if(!m_userInfo.GetUserName().IsEmpty())
		{
			NS_REALNAME::CheckoutInfo coi;
			ZeroMemory(&coi, sizeof(coi));
			_tcsncpy(coi.szName, m_userInfo.GetUserName(), 100);
			_tcsncpy(coi.szNetId, m_userInfo.GetNetId(), 100);

			coi.nCheckoutType = CIBAGlobal::cot_TempLogout;

			theApp.GetRealName()->CheckOut(&coi);
		}	
	}
	CNDDialog::OnCancel();
}
