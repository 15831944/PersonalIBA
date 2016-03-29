// OnOffDuty.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "IBAHelpper.h"
#include ".\onoffdutysetpage.h"


// CSetPageOfDuty �Ի���

IMPLEMENT_DYNAMIC(CSetPageOfDuty, CIBAPage)
CSetPageOfDuty::CSetPageOfDuty()
: CIBAPage(CSetPageOfDuty::IDD)
, m_bPrintShiftReport(FALSE), m_nShiftCount(3)
, m_bDynamicReverseMoney(FALSE), m_nShiftReportType(0)
, m_bIsFetchAmount(FALSE), m_bEditPayout(TRUE), m_bAllowNextDutyMoney(TRUE)
{
	EnableVisualManagerStyle();
}

CSetPageOfDuty::~CSetPageOfDuty()
{

}

void CSetPageOfDuty::DoDataExchange(CDataExchange* pDX)
{
	CIBAPage::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_PRINTSHIFT, m_bPrintShiftReport);
	DDX_Check(pDX, IDC_CHECK_DYNAMICREVERSE, m_bDynamicReverseMoney);
	DDX_CBIndex(pDX, IDC_COMBO_SHIFTREPORTTYPE, m_nShiftReportType);
	DDX_Check(pDX, IDC_CHECK_FETCHAMOUNT, m_bIsFetchAmount);
	DDX_Check(pDX, IDC_CHECK_EDITPAYOUT, m_bEditPayout);
	DDX_Check(pDX, IDC_CHECK_ALLOWNEXT, m_bAllowNextDutyMoney);
}


BEGIN_MESSAGE_MAP(CSetPageOfDuty, CIBAPage)
	ON_CBN_SELCHANGE(IDC_COMBO_SHIFTCOUNT, OnCbnSelchangeComboShiftcount)
END_MESSAGE_MAP()


// CSetPageOfDuty ��Ϣ�������

void CSetPageOfDuty::OnOK()
{
	UpdateData();

	//���������
	CIBAConfig::GetInstance()->SetShiftReportType(m_nShiftReportType);
	///////////////////�Զ���ӡ�����/////////////////////
	CIBAConfig::GetInstance()->SetPrintShiftReport(m_bPrintShiftReport);
	/////////////////��̬���ý���//////////////////////////
	CIBAConfig::GetInstance()->SetDynamicReserve(m_bDynamicReverseMoney);
	/////////////////�Ͻ����///////////////////
	CIBAConfig::GetInstance()->SetShowShiftTotalInfo(m_bIsFetchAmount);
	/////////////////����༭������֧����////////////////
	CIBAConfig::GetInstance()->SetEditPayOut(m_bEditPayout);

	// 2011/05/19-gxx: 
	CIBAConfig::GetInstance()->SetAllowNextDutyMoney(m_bAllowNextDutyMoney);

	CString strTmp;

	strTmp.Format(_T("%d-%d"), ((CComboBox*)GetDlgItem(IDC_COMBO_SHIFT1STARTTIME))->GetCurSel(),
		((CComboBox*)GetDlgItem(IDC_COMBO_SHIFT1ENDTIME))->GetCurSel());

	CIBAConfig::GetInstance()->SetShift1(strTmp);

	strTmp.Format(_T("%d-%d"), ((CComboBox*)GetDlgItem(IDC_COMBO_SHIFT2STARTTIME))->GetCurSel(),
		((CComboBox*)GetDlgItem(IDC_COMBO_SHIFT2ENDTIME))->GetCurSel());

	CIBAConfig::GetInstance()->SetShift2(strTmp);

	strTmp.Format(_T("%d-%d"), ((CComboBox*)GetDlgItem(IDC_COMBO_SHIFT3STARTTIME))->GetCurSel(),
		((CComboBox*)GetDlgItem(IDC_COMBO_SHIFT3ENDTIME))->GetCurSel());

	CIBAConfig::GetInstance()->SetShift3(strTmp);

	m_nShiftCount = 3 - ((CComboBox*)GetDlgItem(IDC_COMBO_SHIFTCOUNT))->GetCurSel();
	CIBAConfig::GetInstance()->SetShiftCount(m_nShiftCount);

	CIBAPage::OnOK();
}

BOOL CSetPageOfDuty::OnInitDialog()
{
	CIBAPage::OnInitDialog();
	
	//���������
	m_nShiftReportType = CIBAConfig::GetInstance()->GetShiftReportType();
	//�Զ���ӡ�����
	m_bPrintShiftReport = CIBAConfig::GetInstance()->GetPrintShiftReport();
	//��̬���ý�
	m_bDynamicReverseMoney = CIBAConfig::GetInstance()->GetDynamicReserve();
	//�Ͻ����
	m_bIsFetchAmount = CIBAConfig::GetInstance()->GetShowShiftTotalInfo();
	//����༭������֧����
	m_bEditPayout = CIBAConfig::GetInstance()->GetEditPayOut();

	// 2011/05/19-gxx: 
	m_bAllowNextDutyMoney = CIBAConfig::GetInstance()->GetAllowNextDutyMoney();

	UpdateData(FALSE);

	CStringArray strArray;

	CIBAHelpper::SplitLine(CIBAConfig::GetInstance()->GetShift1(), strArray, '-');

	INT nStart = _ttoi(strArray.GetAt(0));
	INT nEnd = _ttoi(strArray.GetAt(1));
	
	((CComboBox*)GetDlgItem(IDC_COMBO_SHIFT1STARTTIME))->SetCurSel(nStart);
	((CComboBox*)GetDlgItem(IDC_COMBO_SHIFT1ENDTIME))->SetCurSel(nEnd);

	CIBAHelpper::SplitLine(CIBAConfig::GetInstance()->GetShift2(), strArray, '-');

	nStart = _ttoi(strArray.GetAt(0));
	nEnd = _ttoi(strArray.GetAt(1));

	((CComboBox*)GetDlgItem(IDC_COMBO_SHIFT2STARTTIME))->SetCurSel(nStart);
	((CComboBox*)GetDlgItem(IDC_COMBO_SHIFT2ENDTIME))->SetCurSel(nEnd);

	CIBAHelpper::SplitLine(CIBAConfig::GetInstance()->GetShift3(), strArray, '-');

	nStart = _ttoi(strArray.GetAt(0));
	nEnd = _ttoi(strArray.GetAt(1));

	((CComboBox*)GetDlgItem(IDC_COMBO_SHIFT3STARTTIME))->SetCurSel(nStart);
	((CComboBox*)GetDlgItem(IDC_COMBO_SHIFT3ENDTIME))->SetCurSel(nEnd);

	m_nShiftCount = CIBAConfig::GetInstance()->GetShiftCount();
	
	INT nIndex = 3 - m_nShiftCount;

	((CComboBox*)GetDlgItem(IDC_COMBO_SHIFTCOUNT))->SetCurSel(nIndex);
	OnCbnSelchangeComboShiftcount();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CSetPageOfDuty::OnCbnSelchangeComboShiftcount()
{
	INT nSel = ((CComboBox*)GetDlgItem(IDC_COMBO_SHIFTCOUNT))->GetCurSel();

	if (nSel == 0)
	{
		GetDlgItem(IDC_COMBO_SHIFT2STARTTIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_SHIFT2ENDTIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_SHIFT3STARTTIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_SHIFT3ENDTIME)->EnableWindow(TRUE);
	}
	else if (nSel == 1)
	{
		GetDlgItem(IDC_COMBO_SHIFT2STARTTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_SHIFT2ENDTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_SHIFT3STARTTIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_SHIFT3ENDTIME)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_COMBO_SHIFT2STARTTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_SHIFT2ENDTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_SHIFT3STARTTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_SHIFT3ENDTIME)->EnableWindow(FALSE);
	}
}
