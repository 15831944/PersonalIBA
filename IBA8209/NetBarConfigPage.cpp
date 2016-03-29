// NetBarConfigPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "NetBarConfig.h"
#include ".\netbarconfigpage.h"
#include ".\dal\IBADAL.h"


// CClientConfigCommonPage �Ի���

IMPLEMENT_DYNAMIC(CClientConfigCommonPage, CIBAPage)
CClientConfigCommonPage::CClientConfigCommonPage()
: CIBAPage(CClientConfigCommonPage::IDD)
, m_nCheckInPwdType(0), m_nShutDownMinutes(1), m_nAfterCheckOut(0)
, m_bTmpUserChangeComputer(FALSE)
, m_bMemberUserChangeComputer(FALSE)
, m_bShowClientUseTime(TRUE)
, m_bAllowSuspend(FALSE)
, m_bAllowAutoCheckIn(TRUE)
, m_nNoEnoughTime(10)
{
}

CClientConfigCommonPage::~CClientConfigCommonPage()
{
}

void CClientConfigCommonPage::DoDataExchange(CDataExchange* pDX)
{
	CIBAPage::DoDataExchange(pDX);

	//DDV_MinMaxUInt(pDX, m_nNoEnoughTime, 2, 30);
	DDX_Text(pDX, IDC_EDIT_SHUTDOWNTIME, m_nShutDownMinutes);
	DDX_Text(pDX, IDC_EDIT_NOENOUGHTIME, m_nNoEnoughTime);

	DDX_CBIndex(pDX, IDC_COMBO_PASSWORDTYPE, m_nCheckInPwdType);
	DDX_CBIndex(pDX, IDC_COMBO_AFFTERCHECKOUT, m_nAfterCheckOut);

	DDX_Check(pDX, IDC_CHECK_TMPCHANGECOMPUTER, m_bTmpUserChangeComputer);
	DDX_Check(pDX, IDC_CHECK_MEMBERCHANGECOMPUTER, m_bMemberUserChangeComputer);
	DDX_Check(pDX, IDC_CHECK_SHOWUSETIME, m_bShowClientUseTime);
	DDX_Check(pDX, IDC_CHECK_ALLOWSUSPEND, m_bAllowSuspend);
	DDX_Check(pDX, IDC_CHECK_AUTO_START, m_bAllowAutoCheckIn);
	
}

BEGIN_MESSAGE_MAP(CClientConfigCommonPage, CIBAPage)
	
END_MESSAGE_MAP()


// CClientConfigCommonPage ��Ϣ�������
BOOL CClientConfigCommonPage::OnInitDialog()
{
	CIBAPage::OnInitDialog();

	((CEdit*)GetDlgItem(IDC_EDIT_SHUTDOWNTIME))->SetLimitText( CIBAConfig::MAX_TIME_LIMIT );
	((CEdit*)GetDlgItem(IDC_EDIT_NOENOUGHTIME))->SetLimitText( CIBAConfig::MAX_TIME_LIMIT );

	m_NetBarConfig2.LoadFromDB();

	m_nCheckInPwdType = m_NetBarConfig2.GetClientLoginStyle();
	m_nShutDownMinutes = m_NetBarConfig2.GetShutDownMinutes();
	m_nAfterCheckOut = m_NetBarConfig2.GetShutDownFlag();

	///////////////////�û�������������������///////////////
	m_bMemberUserChangeComputer = m_NetBarConfig2.GetMemberUserChangecomputer();
	m_bTmpUserChangeComputer = m_NetBarConfig2.GetTmpUserChangeComputer();

	m_bShowClientUseTime = m_NetBarConfig2.GetShowUseTime();

	//����һ�
	m_bAllowSuspend = m_NetBarConfig2.GetAllowSuspend();

	m_nNoEnoughTime = m_NetBarConfig2.GetNoEnoughTime();

	//2011-03-15-gxx-����ͻ��������Զ���¼
	m_bAllowAutoCheckIn = m_NetBarConfig2.GetAllowAutoCheckIn();

	/**
	*
	������� 
	0��IC��
	1: ID��
	2: ţ��V1
	3�����
	4: ţ��V2
	*/
	
	if (CNetBarConfig::GetInstance()->GetSoftwareType() < 3)
	{
		GetDlgItem(IDC_CHECK_SHOWUSETIME)->ShowWindow(SW_HIDE);
	}
	else if (CNetBarConfig::GetInstance()->GetSoftwareType() >= 3)
	{
		m_nCheckInPwdType = 1;
		GetDlgItem(IDC_COMBO_PASSWORDTYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_PASSWORDTYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC0)->ShowWindow(SW_HIDE);
	}

	// 2012/04/09-8241-gxx: ����ʵ��Ĭ�Ͻ��˺�����
	if (CNetBarConfig::GetInstance()->GetAuditType() == CIBAGlobal::emRealnameHengXin)
	{
		CComboBox* pComb = (CComboBox*)GetDlgItem(IDC_COMBO_AFFTERCHECKOUT);
		if (m_nAfterCheckOut == CIBAGlobal::emAfterCheckOutLock)
		{
			m_nAfterCheckOut = CIBAGlobal::emAfterCheckOutRestart;
			m_NetBarConfig2.SetShutDownFlag(m_nAfterCheckOut); // ���浽���ݿ�
		}
		pComb->DeleteString(0); // ɾ��������

		if (m_nAfterCheckOut == 0)
		{
			m_nAfterCheckOut = 1;
		}
		m_nAfterCheckOut -= 1;
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CClientConfigCommonPage::OnOK()
{
	UpdateData();

	if (m_nCheckInPwdType != m_NetBarConfig2.GetClientLoginStyle())
	{
		m_NetBarConfig2.SetClientLoginStyle(m_nCheckInPwdType);
	}

	if (CNetBarConfig::GetInstance()->GetAuditType() == CIBAGlobal::emRealnameHengXin)
	{
		m_nAfterCheckOut += 1;
	}

	if (m_nAfterCheckOut!= m_NetBarConfig2.GetShutDownFlag())
	{
		m_NetBarConfig2.SetShutDownFlag(m_nAfterCheckOut);
	}

	if (m_nShutDownMinutes != m_NetBarConfig2.GetShutDownMinutes())
	{
		m_NetBarConfig2.SetShutDownMinutes(m_nShutDownMinutes);
	}

	////////////////�û������������������/////////////////
	if (m_bTmpUserChangeComputer != m_NetBarConfig2.GetTmpUserChangeComputer())
	{
		m_NetBarConfig2.SetTmpUserChangeComputer(m_bTmpUserChangeComputer);
	}

	if (m_bMemberUserChangeComputer != m_NetBarConfig2.GetMemberUserChangecomputer())
	{
		m_NetBarConfig2.SetMemberUserChangeComputer(m_bMemberUserChangeComputer);
	}

	if (m_bShowClientUseTime != m_NetBarConfig2.GetShowUseTime())
	{
		m_NetBarConfig2.SetShowUseTime(m_bShowClientUseTime);
	}

	/////////////////����ػ�//////////
	if (m_bAllowSuspend != m_NetBarConfig2.GetAllowSuspend())
	{
		m_NetBarConfig2.SetAllowSuspend(m_bAllowSuspend);
	}

	if (m_nNoEnoughTime != m_NetBarConfig2.GetNoEnoughTime())
	{
		m_NetBarConfig2.SetNoEnoughTime(m_nNoEnoughTime);
	}

	// 2011-03-15-gxx-����ͻ��������Զ���¼
	if (m_bAllowAutoCheckIn != m_NetBarConfig2.GetAllowAutoCheckIn())
	{
		m_NetBarConfig2.SetAllowAutoCheckIn(m_bAllowAutoCheckIn);
	}

	CIBAPage::OnOK();
}




