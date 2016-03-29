// HWConnectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HWConnect.h"
#include "HWConnectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHWConnectDlg dialog

CHWConnectDlg::CHWConnectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHWConnectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHWConnectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hCompleteEvent = ::CreateEvent(NULL, TRUE, FALSE, "CHWConnectDlg_SCAN_COMPLETE_FALG");
	if(m_hCompleteEvent == NULL)
	{
		AfxMessageBox("Create Event falied!");
		PostQuitMessage(0);
	}
	m_hScanPicCompleteEvent = ::CreateEvent(NULL, TRUE, FALSE, "CHWConnectDlg_SCAN_PIC_COMPLETE_FALG");
	if(m_hScanPicCompleteEvent == NULL)
	{
		AfxMessageBox("Create Scan PIC Event falied!");
		PostQuitMessage(0);
	}

	m_strScanImagePath = _T("HWTEMP.JPG");
}

void CHWConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHWConnectDlg)
	DDX_Control(pDX, IDC_COMBO_IDTYPE, m_objIDTypeSelect);
	DDX_Control(pDX, IDC_PROGRESS_SCAN, m_objCScanProgress);
	DDX_Control(pDX, IDC_HWIDCARDCTRL1, m_objCHWConnect);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHWConnectDlg, CDialog)
	//{{AFX_MSG_MAP(CHWConnectDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_Calibrate, OnBUTTONCalibrate)
	ON_BN_CLICKED(IDC_BUTTON_Init, OnBUTTONInit)
	ON_BN_CLICKED(IDC_BUTTON_Scan, OnBUTTONScan)
	ON_WM_COPYDATA()
	ON_CBN_KILLFOCUS(IDC_COMBO_IDTYPE, OnKillfocusComboIdtype)
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHWConnectDlg message handlers

BOOL CHWConnectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	ResetEvent(m_hScanPicCompleteEvent);
	ResetEvent(m_hCompleteEvent);

#ifndef _DEBUG	
	::SetWindowPos(m_hWnd, HWND_BOTTOM, -3, -3, 1, 1, SWP_HIDEWINDOW|SWP_NOACTIVATE);
#else
	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW|SWP_NOACTIVATE);
#endif

	m_objCScanProgress.SetPos(0);
	m_objIDTypeSelect.SetCurSel(0);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHWConnectDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CHWConnectDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CHWConnectDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	//CDialog::OnOK();
}

void CHWConnectDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	ResetEvent(m_hCompleteEvent);
	ResetEvent(m_hScanPicCompleteEvent);
#ifdef _DEBUG	
	CDialog::OnCancel();
#endif
}

void CHWConnectDlg::OnBUTTONCalibrate() 
{
	// TODO: Add your control notification handler code here
	ResetEvent(m_hScanPicCompleteEvent);
	ResetEvent(m_hCompleteEvent);
	m_objCScanProgress.SetPos(0);
	m_objCHWConnect.HWCalibrate();
}

void CHWConnectDlg::OnBUTTONInit() 
{
	// TODO: Add your control notification handler code here
	ResetEvent(m_hScanPicCompleteEvent);
	ResetEvent(m_hCompleteEvent);
	m_objCScanProgress.SetPos(0);

/*	char cTempPath[MAX_PATH] = {0};
	if(GetTempPath(MAX_PATH, cTempPath) > 0)
	{
		strcat(cTempPath, "HWSTEMP.JPG");
		
		m_objCHWConnect.HWInitScanner(cTempPath);	
	}
	else
	{
		CString str;
		str.Format("��ѯ��ʱĿ¼ʧ�ܣ�[%d]", GetLastError());
		AfxMessageBox(str);
	}
*/
	m_objCHWConnect.HWInitScanner(m_strScanImagePath);	

}

void CHWConnectDlg::OnBUTTONScan() 
{
	// TODO: Add your control notification handler code here
	ResetEvent(m_hScanPicCompleteEvent);
	ResetEvent(m_hCompleteEvent);
/*
  CardType = 1 ���պ���
  CardType = 2 ��ʻ֤
  CardType = 3 һ�����֤���ᣩ
  CardType = 4 �Զ���ɨ��
  CardType = 5 ǿ��ɨ�裬�������⻵�˵�ʱ��
  CardType = 6 �������֤������
*/
	USHORT usLastIDType = 3;
	switch(m_usIDType)
	{
	case IDENTITY_ID1:// 10 һ�����֤
		{
			usLastIDType = 3;
		}
		break;
		
	case IDENTITY_ID2:// 11 �������֤
		{
			usLastIDType = 6;
		}
		break;
		
	case IDENTITY_STUDENT:// 15 ѧ��֤
		{
		}
		break;
		
	case IDENTITY_OFFICER:// 90 ����֤
		{
			
		}
		break;
		
	case IDENTITY_POLICE:// 91 ����֤
		{
			
		}
		break;
		
	case IDENTITY_SOLDIER:// 92 ʿ��֤
		{
			
		}
		break;
		
	case IDENTITY_HUKOUBU:// 93 ���ڲ�
		{
			
		}
		break;
		
	case IDENTITY_PASSPORT:// 94 ����
		{
			usLastIDType = 1;
		}
		break;
		
	case IDENTITY_TAIBAO:// 95 ̨��֤
		{
		}
		break;
		
	case IDENTITY_OTHER:// 99 ����֤��
		{
			usLastIDType = 3;
		}
		break;
		
	default:
		break;
	}

	CHWConnectApp *pApp = (CHWConnectApp *)AfxGetApp();
	const CString strSection = "ScanValue";
	pApp->m_objCIniFileManager.WriteProfileString(strSection, "ID", "");
	pApp->m_objCIniFileManager.WriteProfileString(strSection, "Name", "");
	pApp->m_objCIniFileManager.WriteProfileString(strSection, "Sex", "");
	pApp->m_objCIniFileManager.WriteProfileString(strSection, "Nation", "");
	pApp->m_objCIniFileManager.WriteProfileString(strSection, "BirthDay", "");
	pApp->m_objCIniFileManager.WriteProfileString(strSection, "Address", "");
	
	pApp->m_objCIniFileManager.WriteProfileString(strSection, "CarType", "");
	pApp->m_objCIniFileManager.WriteProfileString(strSection, "IDSignDate", "");


	if(!m_objCHWConnect.HWScan(usLastIDType))
	{
		Beep(800, 800);
	}

	SetEvent(m_hScanPicCompleteEvent);
}

BEGIN_EVENTSINK_MAP(CHWConnectDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CHWConnectDlg)
	ON_EVENT(CHWConnectDlg, IDC_HWIDCARDCTRL1, 1 /* OnScanFinish */, OnOnScanFinishHwidcardctrl1, VTS_I2)
	ON_EVENT(CHWConnectDlg, IDC_HWIDCARDCTRL1, 2 /* OnScanBtnDown */, OnOnScanBtnDownHwidcardctrl1, VTS_I2)
	ON_EVENT(CHWConnectDlg, IDC_HWIDCARDCTRL1, 3 /* OnProgress */, OnOnProgressHwidcardctrl1, VTS_I2 VTS_I2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CHWConnectDlg::OnOnScanFinishHwidcardctrl1(short btnType) 
{
	ResetEvent(m_hCompleteEvent);

	CHWConnectApp *pApp = (CHWConnectApp *)AfxGetApp();

	//ɨ��ʶ������󴥷���
	CString TempStr;
	// TODO: Add your control notification handler code here
/*
  btnType = 1 ���պ���
  btnType = 2 ��ʻ֤
  btnType = 3 һ�����֤���ᣩ
  btnType = 4 �Զ���ɨ��
  btnType = 6 �������֤������
*/
	
	// TODO: Add your control notification handler code here
	if ((btnType == 3) || (btnType == 6))
	{
		TempStr = "֤�����룺" + m_objCHWConnect.GetIDNumber() + "\r\n";
		TempStr = TempStr + "������" + m_objCHWConnect.GetIDName() + "\r\n";
		TempStr = TempStr + "�Ա�" + m_objCHWConnect.GetIDSex() + "\r\n";
		TempStr = TempStr + "���壺" + m_objCHWConnect.GetIDNation() + "\r\n";
		TempStr = TempStr + "�������ڣ�" + m_objCHWConnect.GetIDBirthday() + "\r\n";
		TempStr = TempStr + "סַ��" + m_objCHWConnect.GetIDAdrr() + "\r\n";

		//::MessageBox(NULL, TempStr, "ɨ����:", 0);
		const CString strSection = "ScanValue";
		pApp->m_objCIniFileManager.WriteProfileString(strSection, "ID", m_objCHWConnect.GetIDNumber());
		pApp->m_objCIniFileManager.WriteProfileString(strSection, "Name", m_objCHWConnect.GetIDName());
		pApp->m_objCIniFileManager.WriteProfileString(strSection, "Sex", m_objCHWConnect.GetIDSex());
		pApp->m_objCIniFileManager.WriteProfileString(strSection, "Nation", m_objCHWConnect.GetIDNation());
		pApp->m_objCIniFileManager.WriteProfileString(strSection, "BirthDay", m_objCHWConnect.GetIDBirthday());
		pApp->m_objCIniFileManager.WriteProfileString(strSection, "Address", m_objCHWConnect.GetIDAdrr());
		
		pApp->m_objCIniFileManager.WriteProfileString(strSection, "CarType", "");
		pApp->m_objCIniFileManager.WriteProfileString(strSection, "IDSignDate", "");

	}

	//��ʻ֤
	if (btnType == 2)
	{
		TempStr = "֤�����룺" + m_objCHWConnect.GetIDNumber() + "\r\n";
		TempStr = TempStr + "������" + m_objCHWConnect.GetIDName() + "\r\n";
		TempStr = TempStr + "���ͣ�" + m_objCHWConnect.GetCarType() + "\r\n";
		TempStr = TempStr + "��֤���ڣ�" + m_objCHWConnect.GetIDSignDate() + "\r\n";
		TempStr = TempStr + "סַ��" + m_objCHWConnect.GetIDAdrr() + "\r\n";

		//::MessageBox(NULL, TempStr, "ɨ����:", 0);
		const CString strSection = "ScanValue";
		pApp->m_objCIniFileManager.WriteProfileString(strSection, "ID", m_objCHWConnect.GetIDNumber());
		pApp->m_objCIniFileManager.WriteProfileString(strSection, "Name", m_objCHWConnect.GetIDName());
		pApp->m_objCIniFileManager.WriteProfileString(strSection, "Sex", m_objCHWConnect.GetIDSex());
		pApp->m_objCIniFileManager.WriteProfileString(strSection, "Nation", m_objCHWConnect.GetIDNation());
		pApp->m_objCIniFileManager.WriteProfileString(strSection, "BirthDay", m_objCHWConnect.GetIDBirthday());
		pApp->m_objCIniFileManager.WriteProfileString(strSection, "Address", m_objCHWConnect.GetIDAdrr());
		
		pApp->m_objCIniFileManager.WriteProfileString(strSection, "CarType", m_objCHWConnect.GetCarType());
		pApp->m_objCIniFileManager.WriteProfileString(strSection, "IDSignDate", m_objCHWConnect.GetIDSignDate());
	}

	//���պ���
	if (btnType == 1)
	{
		TempStr = m_objCHWConnect.GetPassportNo();
		//::MessageBox(NULL, TempStr, "ɨ����:", 0);
		const CString strSection = "ScanValue";
		pApp->m_objCIniFileManager.WriteProfileString(strSection, "ID", m_objCHWConnect.GetPassportNo());
		pApp->m_objCIniFileManager.WriteProfileString(strSection, "Name", m_objCHWConnect.GetIDName());
		pApp->m_objCIniFileManager.WriteProfileString(strSection, "Sex", m_objCHWConnect.GetIDSex());
		pApp->m_objCIniFileManager.WriteProfileString(strSection, "Nation", m_objCHWConnect.GetIDNation());
		pApp->m_objCIniFileManager.WriteProfileString(strSection, "BirthDay", m_objCHWConnect.GetIDBirthday());
		pApp->m_objCIniFileManager.WriteProfileString(strSection, "Address", m_objCHWConnect.GetIDAdrr());
		
		pApp->m_objCIniFileManager.WriteProfileString(strSection, "CarType", m_objCHWConnect.GetCarType());
		pApp->m_objCIniFileManager.WriteProfileString(strSection, "IDSignDate", m_objCHWConnect.GetIDSignDate());
	}

	m_objCScanProgress.SetPos(0);
	SetEvent(m_hCompleteEvent);
}

void CHWConnectDlg::OnOnScanBtnDownHwidcardctrl1(short btnType) 
{
	// TODO: Add your control notification handler code here
	if (btnType == 3)
	{
		//AfxMessageBox("ɨ�����Ѿ�׼���ã�������֤�����⣩��������");
	}

	if (btnType == 6)
	{
		//AfxMessageBox("ɨ�����Ѿ�׼���ã�������֤���̹⣩��������");
	}

	if (btnType == 1)
	{
		//AfxMessageBox("ɨ�����Ѿ�׼���ã���ѻ��շ�������");
	}

	if (btnType == 2)
	{
		//AfxMessageBox("ɨ�����Ѿ�׼���ã���Ѽ�ʻ֤��������");
	}

	if (btnType == 4)
	{
		//AfxMessageBox("ɨ�����Ѿ�׼���ã���ɨ����ͼ��û��ʶ������");
	}
}

void CHWConnectDlg::OnOnProgressHwidcardctrl1(short Min, short Max, short Position) 
{
	// TODO: Add your control notification handler code here
	m_objCScanProgress.SetRange(Min, Max);
	m_objCScanProgress.SetPos(Position);
}

BOOL CHWConnectDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	// TODO: Add your message handler code here and/or call default
#define CMD_HOSTID	  0xFA

#define CMD_CALIBRATE 0x10
#define CMD_INIT	  0x11
#define CMD_SCAN	  0x12
#define CMD_GetVALUE  0x13
	
	typedef struct tag_CMD
	{
		unsigned short usCMD;
		unsigned char  ucDataBuf[512];
		
	}TAG_CMD;
	COPYDATASTRUCT stRecv = {0};
	TAG_CMD stTAG_CMD = {0};

	memcpy(&stRecv, pCopyDataStruct, sizeof(COPYDATASTRUCT));
	if(stRecv.dwData == CMD_HOSTID)
	{
		memcpy(&stTAG_CMD, stRecv.lpData, stRecv.cbData);
		switch(stTAG_CMD.usCMD)
		{
		case CMD_CALIBRATE:
			{
				const int iID = IDC_BUTTON_Calibrate;
				PostMessage(WM_COMMAND, MAKEWPARAM(iID, BN_CLICKED), (LPARAM)GetDlgItem(iID)->m_hWnd);  
			}
			break;

		case CMD_INIT:
			{
				m_strScanImagePath = stTAG_CMD.ucDataBuf;
				const int iID = IDC_BUTTON_Init;
				PostMessage(WM_COMMAND, MAKEWPARAM(iID, BN_CLICKED), (LPARAM)GetDlgItem(iID)->m_hWnd);  
			}
			break;

		case CMD_SCAN:
			{
				memcpy(&m_usIDType, &stTAG_CMD.ucDataBuf, sizeof(USHORT));
				for(int i = 0; i < 10; i++)
				{
					CString str;
					m_objIDTypeSelect.GetLBText(i, str);
					int iTemp = atoi(str.GetBuffer(128));
					str.ReleaseBuffer();

					if(iTemp > 0 && m_usIDType == iTemp)
					{
						m_objIDTypeSelect.SetCurSel(i);
					}
				}

				const int iID = IDC_BUTTON_Scan;
				PostMessage(WM_COMMAND, MAKEWPARAM(iID, BN_CLICKED), (LPARAM)GetDlgItem(iID)->m_hWnd);  
			}
			break;

		case CMD_GetVALUE:
			{
				//const int iID = IDC_BUTTON_Calibrate;
				//PostMessage(WM_COMMAND, MAKEWPARAM(iID, BN_CLICKED), (LPARAM)GetDlgItem(iID)->m_hWnd);  
			}
			break;

		default:
			;
		}
	}
	
	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
}

void CHWConnectDlg::OnKillfocusComboIdtype() 
{
	// TODO: Add your control notification handler code here
	CString str;
	m_objIDTypeSelect.GetWindowText(str);
	int iTemp = atoi(str.GetBuffer(128));
	str.ReleaseBuffer();
	m_usIDType = iTemp;	
}

int CHWConnectDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
#ifndef _DEBUG	
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
#endif
	
	return 0;
}

void CHWConnectDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnRButtonDown(nFlags, point);
}
