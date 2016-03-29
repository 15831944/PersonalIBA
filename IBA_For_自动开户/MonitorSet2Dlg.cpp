// MonitorSet2Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include ".\monitorset2dlg.h"


// CSetPageOfMonitor2 �Ի���

IMPLEMENT_DYNAMIC(CSetPageOfMonitor2, CIBAPage)
CSetPageOfMonitor2::CSetPageOfMonitor2()
: CIBAPage(CSetPageOfMonitor2::IDD)
, m_bOpenAfterEnter(TRUE)
{
}

CSetPageOfMonitor2::~CSetPageOfMonitor2()
{
}

void CSetPageOfMonitor2::DoDataExchange(CDataExchange* pDX)
{
	CIBAPage::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_MINIMONEY, m_strLittleMoney);
	DDX_Control(pDX, IDC_COMBO_USERTYPE, m_cboUserType);
	DDX_Control(pDX, IDC_BUTTON_CLUSERTYPE, m_CPUserType);
	DDX_Control(pDX, IDC_SHOWZONELIST, m_ShowZoneList);

	DDX_Check(pDX, IDC_CHECK_OPENAFTERENTER, m_bOpenAfterEnter);
}

BEGIN_MESSAGE_MAP(CSetPageOfMonitor2, CIBAPage)
	ON_CBN_SELCHANGE(IDC_COMBO_USERTYPE, OnCbnSelchangeComboUsertype)
	ON_BN_CLICKED(IDC_BUTTON_CLUSERTYPE, OnBnClickedButtonClusertype)
END_MESSAGE_MAP()


// CSetPageOfMonitor2 ��Ϣ�������

BOOL CSetPageOfMonitor2::OnInitDialog()
{
	CIBAPage::OnInitDialog();

	m_strLittleMoney.Format(_T("%.2f"), CIBAConfig::GetInstance()->GetMinMoney() / 100.0);
	///////////////////////////////////////////////////////////////////
	CIBAString strTmp;
	strTmp.LoadString(IDS_MORE);//�����ִ�������...��

	const UINT ColumnsNumber = 10;//��ť�б�����
	m_CPUserType.EnableOtherButton(strTmp);
	m_CPUserType.SetColumnsNumber(ColumnsNumber);
	m_CPUserType.SetColor(RGB(0,0,0));

	//�û�������ɫ���
	int nTmp = 0;
	UINT nClassID = 0;
	//�û�����
	for (INT i = 0; i < CNetBarConfig::GetInstance()->m_UserClassArray.GetCount(); i++)
	{
		CString str = CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(i).GetClassName();
		int nIdx = m_cboUserType.AddString(CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(i).GetClassName());

		nClassID = CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(i).GetClassID();
		//������Ͽ�Ĺ���
		m_cboUserType.SetItemData(nIdx, nClassID);
		//ȡ��������Ĭ�ϵ��û�����
		if (CIBAConfig::GetInstance()->GetDefUserClassID() == nClassID)
		{
			nTmp = nIdx;
		}
	}

	m_cboUserType.SetCurSel(nTmp);
	nClassID = CIBAConfig::GetInstance()->GetDefUserClassID();
	m_CPUserType.SetColor(CIBAConfig::GetInstance()->GetUserTypeColor(nClassID));

	/////////////��ʾ������������////////////
	for (INT i = 0; i < CNetBarConfig::GetInstance()->m_PCClassInfoArray.GetCount(); i++)
	{
		CString strPCName = CNetBarConfig::GetInstance()->m_PCClassInfoArray.GetAt(i).GetName();
		int nIdx = m_ShowZoneList.AddString(strPCName);

		int nClassID = CNetBarConfig::GetInstance()->m_PCClassInfoArray.GetAt(i).GetID();
		//������Ͽ�Ĺ���
		m_ShowZoneList.SetItemData(nIdx, nClassID);
		//����״̬
		m_ShowZoneList.SetCheck(nIdx, CIBAConfig::GetInstance()->GetShowPCClassType(nClassID));
	}

	m_bOpenAfterEnter = CIBAConfig::GetInstance()->GetOpenAfterEnter();

	UpdateData(FALSE);

	return FALSE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CSetPageOfMonitor2::OnOK()
{
	CIBAConfig::GetInstance()->SetMinMoney(_tstof(m_strLittleMoney) * 100);

	CIBAConfig::GetInstance()->SetOpenAfterEnter(m_bOpenAfterEnter);

	for (INT i = 0; i < m_ShowZoneList.GetCount(); i++)
	{
		int nClassID = m_ShowZoneList.GetItemData(i);
		CIBAConfig::GetInstance()->SetShowPCClassType(nClassID, m_ShowZoneList.GetCheck(i));
	}

	CIBAPage::OnOK();
}

void CSetPageOfMonitor2::OnCbnSelchangeComboUsertype()
{
	for (INT i = 0; i < CNetBarConfig::GetInstance()->m_UserClassArray.GetCount(); i++)
	{
		UINT nClassID = CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(i).GetClassID();
		//ȡ��������Ĭ�ϵ��û�����
		if (m_cboUserType.GetItemData(m_cboUserType.GetCurSel()) == nClassID)
		{
			m_CPUserType.SetColor(CIBAConfig::GetInstance()->GetUserTypeColor(nClassID));
		}
	}
}

void CSetPageOfMonitor2::OnBnClickedButtonClusertype()
{
	UINT nClassID = m_cboUserType.GetItemData(m_cboUserType.GetCurSel());
	CIBAConfig::GetInstance()->SetUserTypeColor(nClassID, m_CPUserType.GetColor());
}
