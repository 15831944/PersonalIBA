// NDUpgradeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "Cashier.h"
#include "MainFrm.h"
#include "IBAHelpper.h"
#include ".\dal\IBADAL.h"
#include ".\ndupgradedlg.h"


//using namespace NS_DAL;
// CNDUpgradeDlg �Ի���

IMPLEMENT_DYNAMIC(CNDUpgradeDlg, CNDDialog)
CNDUpgradeDlg::CNDUpgradeDlg(CWnd* pParent /*=NULL*/)
: CNDDialog(CNDUpgradeDlg::IDD, pParent)
{
}

CNDUpgradeDlg::~CNDUpgradeDlg()
{
}

void CNDUpgradeDlg::DoDataExchange(CDataExchange* pDX)
{
	CNDDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_STATIC_USERNAME, m_strName);
	DDX_Text(pDX, IDC_STATIC_MEMBERID, m_strMemberID);
	DDX_Text(pDX, IDC_STATIC_NETBAR, m_strNetBar);
	DDX_Text(pDX, IDC_STATIC_PERSONALID, m_strPersonalID);
	DDX_Text(pDX, IDC_STATIC_CLASS, m_strUserClass);
	DDX_Text(pDX, IDC_STATIC_ALL, m_strAllBalance);
	DDX_Text(pDX, IDC_STATIC_YSCZ, m_strBalanceY);
	DDX_Text(pDX, IDC_STATIC_WSCZ, m_strBalanceW);
	DDX_Text(pDX, IDC_STATIC_ZSZH, m_strBalanceZ);
	DDX_Text(pDX, IDC_STATIC_DEPOSIT, m_strDeposit);
	DDX_Text(pDX, IDC_EDIT_REESON, m_strReason);

	DDX_Control(pDX, IDC_COMBO_USERCLASS, m_cboUserClass);
}


BEGIN_MESSAGE_MAP(CNDUpgradeDlg, CNDDialog)
	ON_BN_CLICKED(IDC_BUTTON_UPGRADE, OnBnClickedButtonUpgrade)
END_MESSAGE_MAP()


// CNDUpgradeDlg ��Ϣ�������

BOOL CNDUpgradeDlg::OnInitDialog()
{
	CNDDialog::OnInitDialog();

	SetEditFont();

	//�û�����
	for (INT i = 0; i < CNetBarConfig::GetInstance()->m_UserClassArray.GetCount(); i++)
	{
		int nIdx = m_cboUserClass.AddString(CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(i).GetClassName());

		UINT nClassID = CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(i).GetClassID();

		m_cboUserClass.SetItemData(nIdx, nClassID);
	}

	m_cboUserClass.SetFont(&m_Font);
	m_cboUserClass.SetCurSel(0);

	GetDlgItem(IDC_EDIT_IDCARDSNO)->SetFont(&m_BigFont);
	GetDlgItem(IDC_EDIT_IDCARDSNO)->SetFocus();

	GetDlgItem(IDC_BUTTON_UPGRADE)->EnableWindow(FALSE);

	AddFocusEnter(IDC_COMBO_USERCLASS, IDC_BUTTON_UPGRADE);
	AddFocusEnter(IDC_EDIT_REESON, IDC_BUTTON_UPGRADE);

	return FALSE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CNDUpgradeDlg::OnOldCard(UINT nCardId, UINT nMemberId)
{
	m_strMemberID.Format(_T("%d"), nMemberId);

	QueryBalance(nMemberId, nCardId);

	UpdateData(FALSE);
}

void CNDUpgradeDlg::OnGetBalanceOK(NS_SERVLET::CBalanceMethod & Balance)
{
	m_strMemberName = Balance.GetMemberName();
	m_strName = Balance.GetUserName();
	m_strPersonalID = Balance.GetPersonalId();
	m_nUserClassID = Balance.GetUserClass();
	m_strUserClass = CNetBarConfig::GetInstance()->GetUserClassName(Balance.GetUserClass());
	m_strNetBar = CNetBarConfig::GetInstance()->GetNetBarName(Balance.GetOpenNetbarId());

	double dblTmp = 0.0, dblTmp1 = 0.0, dblTmp2 = 0.0;

	//����
	dblTmp = Balance.GetCashBalance();
	dblTmp2 = dblTmp / 100;
	m_strBalanceW.Format(_T("%.2f"), dblTmp2);

	dblTmp1 = dblTmp;

	//����
	dblTmp = Balance.GetUsePresent();
	dblTmp2 = dblTmp / 100;
	m_strBalanceY.Format(_T("%.2f"), dblTmp2);

	dblTmp1 += dblTmp;

	//����
	dblTmp = Balance.GetCreditPresent();
	dblTmp2 = dblTmp / 100;
	m_strBalanceZ.Format(_T("%.2f"), dblTmp2);

	dblTmp1 += dblTmp;

	//����Ԥ����
	dblTmp = Balance.GetOnlineAmount();
	dblTmp2 = dblTmp / 100;

	dblTmp1 += dblTmp;

	//�ϼ� = �����˻�+�����˻�+�����˻�+Ԥ��������
	dblTmp2 = dblTmp1 / 100;
	m_strAllBalance.Format(_T("%.2f"), dblTmp2);

	//Ѻ��
	//dblTmp = Balance.GetGuarantyMoney();
	//dblTmp2 = dblTmp / 100;
	//m_strDeposit.Format(_T("%.2f"), dblTmp2);
	
	m_strDeposit.Format(_T("%d"), Balance.GetCurrentPoint());

	GetDlgItem(IDC_BUTTON_UPGRADE)->EnableWindow(TRUE);

}

void CNDUpgradeDlg::OnBnClickedButtonUpgrade()
{
	UpdateData();
	
	INT nTmp = m_cboUserClass.GetCurSel();
	UINT nUserClass = m_cboUserClass.GetItemData(nTmp);
	
	if (nUserClass == m_nUserClassID) //�û�����һ��
	{
		SetToolTipPos(IDC_COMBO_USERCLASS);
		ShowToolTip(IDS_INPUTERROR);
		return;
	}
	//������������
	CUpgradeMethod Upgrade;
	Upgrade.SetMemberId(_ttoi(m_strMemberID));
	Upgrade.SetOldClassId(m_nUserClassID);
	Upgrade.SetReason(m_strReason);
	Upgrade.SetNewClassId(nUserClass);

	theApp.GetCurCashier()->DoUpgrade(Upgrade);

	///////////////////////////////////////////////////////////
	///////////////////////�����ɹ���д�뱾�����ݿ�///////////////////////
	if (Upgrade.GetStatusCode() == 0)
	{
		{//д�����ݿ�
			CMemberUpgradeDB UpgradeInfo;

			CString strTmp;

			UpgradeInfo.SetMemberId(_ttoi(m_strMemberID));
			UpgradeInfo.SetNetBarId(CNetBarConfig::GetInstance()->GetNetBarId());
			UpgradeInfo.SetClassId(nUserClass);
			UpgradeInfo.SetOldClassId(m_nUserClassID);
			UpgradeInfo.SetOperator(theApp.GetCurCashier()->GetName());
			UpgradeInfo.SetReason(m_strReason);
			UpgradeInfo.SetUpgradeTime(CIBAHelpper::FormatCenterTime(Upgrade.GetTranTime()));
			UpgradeInfo.SetRefNo(Upgrade.GetRefNo());
			UpgradeInfo.SetSerialNo(Upgrade.GetSerialNo());

			CIBADAL::GetInstance()->AddMemberUpgradeRecord(UpgradeInfo);
		}

		m_cboUserClass.SetCurSel(0);
		m_strReason.Empty();

		UpdateData(FALSE);
		GetDlgItem(IDC_BUTTON_UPGRADE)->EnableWindow(FALSE);

		OnBnClickedButtonQuery();
	}
	/////////////////////////////////////////////////////////////

	SetToolTipPos(IDC_BUTTON_UPGRADE);
	ShowToolTip(Upgrade.GetStatusMessage(), Upgrade.GetStatusCode() != 0);
}
