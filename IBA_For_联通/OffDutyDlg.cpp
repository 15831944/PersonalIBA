// OffDutyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "Cashier.h"
#include "IBAHelpper.h"
#include "IBAPrint.h"
#include "IBAShiftReport.h"
#include "Servlet\LastTranInfo.h"
#include ".\offdutydlg.h"
#include "storedlg.h"
#include ".\Socket\LocalServer.h"
#include "DdnPcpInc.h"

using namespace NS_DAL;

// COffDutyDlg �Ի���

IMPLEMENT_DYNAMIC(CDlgIBADuty, CIBADialog)
CDlgIBADuty::CDlgIBADuty(CWnd* pParent /*=NULL*/)
: CIBADialog(CDlgIBADuty::IDD, pParent)
, m_nAllMoney(0), m_nNextReserve(0)
{
	m_nDutyIncome = 0;
	m_nLocalCreditMoney = 0;
}

CDlgIBADuty::~CDlgIBADuty()
{
}

void CDlgIBADuty::DoDataExchange(CDataExchange* pDX)
{
	CIBADialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_RESERVE, m_edtReverveAmount);//���ý���Ŀ������̬��
	DDX_Control(pDX, IDC_COMBO_SHIFT, m_cboShift);//����Աת��

	DDX_Control(pDX, IDC_EDIT_PAYOUT, m_edtPayout);//֧��
	DDX_Text(pDX, IDC_EDIT_MEMO, m_strDescription);//����
	DDX_Control(pDX, IDC_EDIT_NEXTREVERSE, m_edtNextReverse);
	DDX_Text(pDX, IDC_EDIT_NEXTREVERSE, m_strNextReverse);
}

BEGIN_MESSAGE_MAP(CDlgIBADuty, CIBADialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_STAT, OnBnClickedButtonStat)
	ON_EN_CHANGE(IDC_EDIT_PAYOUT, OnEnChangeEditPayout)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnBnClickedButtonPrint)
	ON_BN_CLICKED(IDC_BUTTON_JXCPRINT, OnBnClickedButtonJxcprint)
	ON_EN_CHANGE(IDC_EDIT_NEXTREVERSE, OnEnChangeEditNextreverse)
	ON_BN_CLICKED(IDC_BUTTON_STORECHECK, OnBnClickedButtonStorecheck)
END_MESSAGE_MAP()


// COffDutyDlg ��Ϣ�������

void CDlgIBADuty::OnBnClickedOk()
{

	//UNIT_AUTO_TRACE("���°����");
	
	GetDlgItem(IDOK)->EnableWindow(FALSE);

	//CWaitCursor Wait;

	UpdateData();

	//�������
	if (theApp.GetCurCashier()->IsOnDuty())
	{
		if (IDOK != MsgBoxFormat(MB_OKCANCEL | MB_ICONQUESTION, IDS_OFFDUTYQUESTION, 
				theApp.GetCurCashier()->GetName()))
		{
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			return;
		}

		CArray<CLocalConsumeInfo,CLocalConsumeInfo&> ConsumeInfoArray;
		CIBADAL::GetInstance()->GetLocalConsumes(ConsumeInfoArray);
		if (ConsumeInfoArray.GetCount() > 0)
		{
			theApp.IBAMsgBox(IDS_LOCALOFFDUTYERROR);  // ����������Ϣû����ȫ�ϴ������ģ��������°�
			CBCGPDialog::OnCancel();
			return;
		}

		//�°�
		DoOffDuty();
	}
	else
	{//�ϰ�
		DoOnDuty();		
	}
	GetDlgItem(IDOK)->EnableWindow(TRUE);
}
//ͳ������,��ʾ��������
void CDlgIBADuty::OnBnClickedButtonStat()
{
	CWaitCursor Wait;

	m_nAllMoney = 0;
	m_nNextReserve = 0;

	SetToolTipPos(IDC_BUTTON_STAT);

	if (theApp.GetCurCashier()->IsOnDuty()) //�ϰ�
	{
		m_strOnDutyTime = theApp.GetCurCashier()->GetOnDutyTime();
		
		CLastTranInfo LastTranInfo;//����׷���
		LastTranInfo.Load();//����ע�����Ϣ

		m_ShiftTotalMethod.SetLastRefNo(LastTranInfo.GetLastRefNo());//���һ�ν��׵Ĳ�����
		m_ShiftTotalMethod.SetLastTranTime(LastTranInfo.GetLastTranTime());//���һ�ν��׵�ʱ��
		m_ShiftTotalMethod.SetLastTranType(LastTranInfo.GetLastTranType());//���һ�ν��׵�����

		theApp.GetCurCashier()->DoShiftTotal(m_ShiftTotalMethod);//

		if (m_ShiftTotalMethod.GetStatusCode() == 0)
		{			
			ShowShiftTotalInfo();//ȡ�ã���ʾ��������

			FillGird();//���������

			GetDlgItem(IDOK)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_MEMO)->SetFocus();
			GetDlgItem(IDC_BUTTON_PRINT)->EnableWindow(CIBAConfig::GetInstance()->GetShowShiftTotalInfo());
		}
		else
		{
			ShowToolTip(m_ShiftTotalMethod.GetStatusMessage(), TRUE);
		}
	}
	else//�°�
	{//�̵���
		GetDlgItem(IDOK)->EnableWindow(TRUE);

		//�����һ�������¼
		//CIBADAL::GetInstance()->ZeroAllCommodityStoreInQuantity();

		CStoreQuantityMethod StoreQuantity;//����̵�ͳ�Ʒ���

		theApp.GetCurCashier()->DoStoreQuantity(StoreQuantity);

		if (StoreQuantity.GetStatusCode() == 0)
		{	// 2011/04/27-gxx: ע�͵����д���://������н��ױ���������
			//CIBADAL::GetInstance()->ZeroAllCommodityRemainQuantity();

			for (UINT i = 0; i < StoreQuantity.GetRecordCount(); i++)
			{//���¿���¼����Ʒ���+ ���������
				CIBADAL::GetInstance()->UpdateCommodityRemainQuantity(StoreQuantity.GetCommodityCode(i), StoreQuantity.GetRemainQuantity(i));	
			}
			//����б������ݿ���ȡ�����ݣ�
			FillGird();

			m_edtReverveAmount.SetFocus();
		}
		else
		{
			ShowToolTip(StoreQuantity.GetStatusMessage(), TRUE);
		}
	}
}

BOOL CDlgIBADuty::OnInitDialog()
{
	CIBADialog::OnInitDialog();

	InitContrls();

	InitGridTree();

	OnBnClickedButtonStat();

	return FALSE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgIBADuty::InitContrls()
{	
	SetEditFont();
	SetStaticFont();
	//���
	GetDlgItem(IDC_STATIC_DEPOSIT)->SetWindowText(_T(""));

	m_cboShift.AddString(CIBAConfig::GetInstance()->GetShiftName1());
	m_cboShift.AddString(CIBAConfig::GetInstance()->GetShiftName2());
	m_cboShift.AddString(CIBAConfig::GetInstance()->GetShiftName3());

	m_cboShift.SetCurSel(GetShiftType() - 1);
	m_cboShift.SetFont(&m_Font);

	GetDlgItem(IDC_STATIC_ALL)->SetFont(&m_BigFont);
	GetDlgItem(IDC_STATIC_HANDLEIN)->SetFont(&m_BigFont);
	GetDlgItem(IDC_STATIC2_HANDLEIN)->SetFont(&m_FontPlus1Bold);
	GetDlgItem(IDC_STATIC_SELFDUTY)->SetFont(&m_FontPlus1Bold);
	GetDlgItem(IDC_STATIC_HELP)->SetFont(&m_FontSmall);

	m_edtReverveAmount.DisableMask();
	m_edtReverveAmount.SetValidChars(_T("0123456789."));
	m_edtReverveAmount.SetLimitText(7);

	m_edtNextReverse.SetLimitText(7);
	m_edtPayout.SetLimitText(7);

	GetDlgItem(IDC_BUTTON_JXCPRINT)->EnableWindow(FALSE);

	GetDlgItem(IDOK)->EnableWindow(FALSE);

	// 2011/05/19-gxx: �Ƿ�ɱ༭"������һ����"
	GetDlgItem(IDC_EDIT_NEXTREVERSE)->EnableWindow( CIBAConfig::GetInstance()->GetAllowNextDutyMoney() );


	//����ĳЩ��
	GetDlgItem(IDC_STATIC_HANDLEIN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC2_HANDLEIN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_NEXTREVERSE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC2_NEXTRESERVE)->ShowWindow(SW_HIDE);

	if (!CNetBarConfig::GetInstance()->IsUseDeposit())
	{
		GetDlgItem(IDC_STATIC_DEPOSIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC2_DEPOSIT)->ShowWindow(SW_HIDE);

		// 2011/06/22-gxx: ��ʽ��û��Ѻ��
		CIBAString str;
		str.LoadString(IDS_DUTY_REMARK);
		GetDlgItem(IDC_STATIC_HELP)->SetWindowText(str);
	}
	else
	{
		// 2011/06/22-gxx: ��ʽ����Ѻ��
		CIBAString str;
		str.LoadString(IDS_DUTY_REMARK2);
		GetDlgItem(IDC_STATIC_HELP)->SetWindowText(str);
	}

	if (theApp.GetCurCashier()->IsOnDuty())
	{
		if (CIBAConfig::GetInstance()->GetDynamicReserve())
		{
			GetDlgItem(IDC_STATIC_HANDLEIN)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_STATIC2_HANDLEIN)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_EDIT_NEXTREVERSE)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_STATIC2_NEXTRESERVE)->ShowWindow(SW_NORMAL);
		} 
	
		m_cboShift.EnableWindow(FALSE);
		m_edtReverveAmount.EnableWindow(FALSE);

		//����༭������֧�������ж�
		GetDlgItem(IDC_EDIT_PAYOUT)->EnableWindow(CIBAConfig::GetInstance()->GetEditPayOut());

		GetDlgItem(IDC_BUTTON_PRINT)->EnableWindow(FALSE);
	}
	else
	{
		m_edtPayout.EnableWindow(FALSE);
		m_edtNextReverse.EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_MEMO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_PRINT)->ShowWindow(SW_HIDE);

		//��ʾ��һ�����ݿ���ֵ
		if (CIBAConfig::GetInstance()->GetDynamicReserve())
 		{	
			m_nNextReserve = CIBAConfig::GetInstance()->GetDynamicReserveMoney();

			//{ 2011/09/05-8201-gxx: 
			INT nNextReserve = 0;
			if (CIBADAL::GetInstance()->GetShiftCashRecord(nNextReserve))
			{
				if (m_nNextReserve != nNextReserve)
				{
					CString strOut;
					//��һ��Ԥ��������쳣,�ο�ֵΪ:%.2f , %.2f\r\nѡ��"��"��ʾѡ���һ��ֵΪ��һ��Ԥ��\r\nѡ��"��"��ʾѡ��ڶ�ֵΪ��һ��Ԥ��
					//strOut.Format(LOAD_STRING(IDS_DUTY_NEXTRESERVE_ERROR),m_nNextReserve/100.0);
					strOut.LoadString(IDS_DUTY_NEXTRESERVE_ERROR);
					GetDlgItem(IDC_STATIC_BYJ_HIT)->SetWindowText(strOut);
					GetDlgItem(IDC_STATIC_BYJ_HIT)->SetFont(&m_FontSmall);
					GetDlgItem(IDC_STATIC_BYJ_HIT)->ShowWindow(TRUE);
				}
			}
			
			//}
			
			
			////�����ȡ�ļ�ʧ�ܣ�������ݿ�
			//if (m_nNextReserve == 0)
			//{
			//	CIBADAL::GetInstance()->GetShiftCashRecord(m_nNextReserve);//��ȡ���ݿ�
			//}

 			CString strTmp;
 			strTmp.Format(_T("%.2f"), m_nNextReserve / 100.0);
 			GetDlgItem(IDC_EDIT_RESERVE)->SetWindowText(strTmp);
 			m_edtReverveAmount.EnableWindow(FALSE);

			GetDlgItem(IDC_STATIC_HANDLEIN)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_STATIC2_HANDLEIN)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_EDIT_NEXTREVERSE)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_STATIC2_NEXTRESERVE)->ShowWindow(SW_NORMAL);
 		} 
 		else
 		{
 			GetDlgItem(IDC_EDIT_RESERVE)->SetWindowText(_T("0.0"));
 			m_edtReverveAmount.EnableWindow(TRUE);
 			m_edtReverveAmount.SetFocus();
 		}

		CIBAString strTmp;
		strTmp.LoadString(IDS_DOONDUTY);
		GetDlgItem(IDOK)->SetWindowText(strTmp);
	}

	//����ʾ������Ϣ
	if (!CIBAConfig::GetInstance()->GetShowShiftTotalInfo())
	{
		HideShiftTotalInfo();
	}
}

//����ͳ�Ƶ�����
void CDlgIBADuty::SetStaticFont()
{
	CWnd* pChildWnd = GetWindow(GW_CHILD);   

	while (pChildWnd != NULL)   
	{
		TCHAR szClassName[MAX_PATH] = {0};
		GetClassName(pChildWnd->GetSafeHwnd(), szClassName, MAX_PATH);

		if (lstrcmp(szClassName, _T("Static")) == 0)
		{
			switch(pChildWnd->GetDlgCtrlID())
			{
			case IDC_STATIC_CREDIT:
			case IDC_STATIC_RETURNED:
			case IDC_STATIC_CASHPAY:
			case IDC_STATIC_DIANKA:
			case IDC_STATIC_STOREIN:
			case IDC_STATIC_DEPOSIT:
			case IDC_STATIC_NETCASH:
			case IDC_STATIC_RESERVE:
			case IDC_STATIC_SELFDUTY:
			case IDC_STATIC_CREDITCARD:
			case IDC_STATIC_NETBARPAY:
			case IDC_STATIC_ZSB_CONSUME:
			case IDC_STATIC_ZSB_BALANCE:
				GetDlgItem(pChildWnd->GetDlgCtrlID())->SetFont(&m_Font);
				break;

			}
			if (pChildWnd->GetDlgCtrlID() != IDC_STATIC && 
				pChildWnd->GetDlgCtrlID() != IDC_STATIC2_HANDLEIN && 
				pChildWnd->GetDlgCtrlID() != IDC_STATIC2_NEXTRESERVE)
			{
				//GetDlgItem(pChildWnd->GetDlgCtrlID())->SetFont(&m_Font);
			}
		}
		pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);  
	}
}
//
void CDlgIBADuty::OnEnChangeEditPayout()
{
	UpdateData();
	INT nPayout = 0, nRemainAmount = 0, nFetchAmount = 0;

	GetEachMoney(nPayout, nRemainAmount, nFetchAmount);

	CString strTmp;

	INT nAllMoney = m_nAllMoney;//���ϴζ�ȡ�еõ����ݣ������ȫ�ֱ���
	nAllMoney -= nPayout;

	strTmp.Format(_T("%.2f"), nAllMoney / 100.0);
	GetDlgItem(IDC_STATIC_ALL)->SetWindowText(strTmp);

	////////////////////////��̬�����Ͻɽ��.���ౣ����////////////////////////
	//�°ౣ����
	m_nNextReserve = nRemainAmount;//��һ�ౣ�����

	//�Ͻ����
	nAllMoney -= nRemainAmount;//�Ͻ����
	strTmp.Empty();
	strTmp.Format(_T("%.2f"), nAllMoney / 100.0);
	GetDlgItem(IDC_STATIC_HANDLEIN)->SetWindowText(strTmp);

}

void CDlgIBADuty::OnEnChangeEditNextreserve()
{
	OnEnChangeEditPayout();
}

void CDlgIBADuty::OnEnChangeEditAll()
{
	INT nPayout = 0, nRemainAmount = 0, nFetchAmount = 0;

	GetEachMoney(nPayout, nRemainAmount, nFetchAmount);

	CString strTmp;

	INT nAllMoney = m_nAllMoney;

	nAllMoney -= nPayout;
	nAllMoney -= nFetchAmount;
}

void CDlgIBADuty::GetEachMoney(INT& nPayout, INT& nRemainAmount, INT& nFetchAmount)
{
	//����֧��
	m_edtPayout.GetWindowText(m_strPayout);
	double dblPayout = _tstof(m_strPayout);
	nPayout = Round(dblPayout * 100);
	
	//Ԥ�����
	m_edtNextReverse.GetWindowText(m_strNextReverse);
	double dblRemainAmount = _tstof(m_strNextReverse);
	nRemainAmount = Round(dblRemainAmount * 100);
	
	//�Ͻ�
	GetDlgItem(IDC_STATIC_HANDLEIN)->GetWindowText(m_strFetchAmount);
	double dblFetchAmount = _tstof(m_strFetchAmount);
	nFetchAmount = Round(dblFetchAmount * 100);

	//���°������
	//GetDlgCtrlID(IDC_STATIC_NETBARPAY)->GetWindowText(strValue);
	int nDutyIncome = m_nDutyIncome - nPayout;
	CString str;
	str.Format(_T("%.2f"),nDutyIncome/100.0);
	GetDlgItem(IDC_STATIC_NETCASH)->SetWindowText(str);

}

//��ӡСƱ
void CDlgIBADuty::PrintTicket(CShiftCash &ShiftCash)
{
	CString strTicket;
	CIBAPrint::GetShiftCashTicket(strTicket);

	CString strTmp;

	strTicket.Replace(_T("[ondutytime]"), ShiftCash.GetStartDate());
	strTicket.Replace(_T("[offdutytime]"), ShiftCash.GetEndDate());

	strTmp.Format(_T("%.2f"), ShiftCash.GetRemainAmount() / 100.0);
	strTicket.Replace(_T("[remain]"), strTmp);

	strTmp.Format(_T("-%.2f"), ShiftCash.GetReturnAmount() / 100.0);
	strTicket.Replace(_T("[return]"), strTmp);

	strTmp.Format(_T("%.2f"), ShiftCash.GetCreditAmount() / 100.0);
	strTicket.Replace(_T("[credit]"), strTmp);

	// 2011/12/20-8237-gxx: 
	//strTmp.Format(_T("%.2f"), (ShiftCash.GetRemainAmount() + ShiftCash.GetCreditAmount() - ShiftCash.GetReturnAmount()) / 100.0);
	strTmp.Format(_T("%.2f"), ShiftCash.GetDutyIncome() / 100.0);
	strTicket.Replace(_T("[netcash]"), strTmp);

	strTmp.Format(_T("%.2f"), ShiftCash.GetCashSoldAmount() / 100.0);
	strTicket.Replace(_T("[cashpay]"), strTmp);

	strTmp.Format(_T("%.2f"), ShiftCash.GetPayout() / 100.0);
	strTicket.Replace(_T("[payout]"), strTmp);

	strTmp.Format(_T("%.2f"), ShiftCash.GetTotalAmount() / 100.0);
	strTicket.Replace(_T("[allcash]"), strTmp);

	strTmp.Format(_T("%.2f"), ShiftCash.GetNextRemainAmount() / 100.0);
	strTicket.Replace(_T("[nextremain]"), strTmp);

	strTmp.Format(_T("%.2f"), ShiftCash.GetFetchAmount() / 100.0);
	strTicket.Replace(_T("[fetchmoney]"), strTmp);

	strTicket.Replace(_T("[description]"), ShiftCash.GetDescription());


	// 2011/08/26-8201-gxx: ��ӵ㿨
	strTmp.Format(_T("%.2f"),ShiftCash.GetDianka() / 100.0);
	strTicket.Replace(_T("[paycard]"), strTmp);

	strTmp.Format(_T("%.2f"),ShiftCash.GetStoreInAmount() / 100.0);
	strTicket.Replace(_T("[storein]"), strTmp);

	strTmp.Format(_T("%.2f"),ShiftCash.GetAccountPay() / 100.0);
	strTicket.Replace(_T("[accountpay]"), strTmp);

	strTmp.Format(_T("%.2f"),ShiftCash.GetCardCreditedAmount() / 100.0);
	strTicket.Replace(_T("[creditcard]"), strTmp);

	strTmp.Format(_T("%.2f"),ShiftCash.GetZSBConsume() / 100.0);
	strTicket.Replace(_T("[ddnpay]"), strTmp);

	strTmp.Format(_T("%.2f"),ShiftCash.GetZSBBalance() / 100.0);
	strTicket.Replace(_T("[ddnbalance]"), strTmp);





	CIBAPrint::Print(strTicket);
}

HBRUSH CDlgIBADuty::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CIBADialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (CTLCOLOR_STATIC == nCtlColor)
	{
		UINT nID = pWnd->GetDlgCtrlID();
		switch(nID)
		{
		
		case IDC_STATIC_CREDIT:
		case IDC_STATIC_RETURNED:
		case IDC_STATIC_CASHPAY:
		case IDC_STATIC_DIANKA:
		case IDC_STATIC_STOREIN:
		case IDC_STATIC_DEPOSIT:
		case IDC_STATIC_NETCASH:
		case IDC_STATIC_RESERVE:
		case IDC_STATIC_ALL:
		case IDC_STATIC_SELFDUTY:
		case IDC_STATIC2_HANDLEIN:
		case IDC_STATIC_HANDLEIN:
			pDC->SetTextColor(RGB(0,0,255));
			break;
		
		case IDC_STATIC_HELP:
			pDC->SetTextColor(RGB(120,120,120));
			break;

		case IDC_STATIC_BYJ_HIT:
			pDC->SetTextColor(RGB(220,0,0));
			break;
		

		}
	}
	else if (CTLCOLOR_EDIT == nCtlColor)
	{
		if (pWnd->GetDlgCtrlID() == IDC_EDIT_PAYOUT || 
			pWnd->GetDlgCtrlID() == IDC_EDIT_NEXTREVERSE)
		{
			pDC->SetTextColor(RGB(255, 0, 0));
		}
	}

	return hbr;
}
//��ӡ�����
void CDlgIBADuty::OnBnClickedButtonPrint()
{
	UpdateData();

	INT nPayout = 0, nRemainAmount = 0, nFetchAmount = 0;
	GetEachMoney(nPayout, nRemainAmount, nFetchAmount);//ȡ֧����Ϊ�°�ı������Ͻɽ�

	if (CIBAConfig::GetInstance()->GetShiftReportType() == 0)
	{
		CIBAShiftReport sr(FALSE);

		if (sr.Create())
		{
			//sr.SetNetBarName(CNetBarConfig::GetInstance()->GetNetBarName());
			//sr.SetOperator(theApp.GetCurCashier()->GetName());
			//sr.SetOnDutyTime(m_strOnDutyTime);
			//sr.SetCardCreditAmount(m_ShiftTotalMethod.GetCreditCard());
			//sr.SetCashSoldAmount(m_ShiftTotalMethod.GetCashPay());
			//sr.SetCostExpense(m_ShiftTotalMethod.GetCostExpense());
			//sr.SetCreditAmount(m_ShiftTotalMethod.GetCreditAmount());
			//sr.SetDeposit(m_ShiftTotalMethod.GetDeposit());
			//sr.SetDescription(m_strDescription);
			//sr.SetPayout(nPayout);
			//sr.SetRemainAmount(m_ShiftTotalMethod.GetReserveAmount());
			//sr.SetReturnAmount(m_ShiftTotalMethod.GetReturnedAmount());
			//sr.SetTotalAmount(nFetchAmount + nRemainAmount);
			//sr.SetNextRemain(m_nNextReserve);//��һ�౸�ý�
			//sr.SetFetchAmount(nFetchAmount);

			//strXML = "<ShiftInfo NetbarName=\"NetbarName\" Cashier=\"Cashier\" OnDutyTime=\"OnDutyTime\" 
			//OffDutyTime=\"OffDutyTime\" CreditMoney=\"123\" ReturnMoney=\"234\" CashSold=\"332\" 
			// PayCard=\"555\" StoreIn=\"3323\" Payout=\"233\" UseDeposit=\"1\" Deposit=\"455\" 
			// ShiftIn=\"344\" LastRemain=\"434\" AllCash=\"43434\" NextRemain=\"4343\" FetchMoney=\"434\" 
			// CreditCard=\"434\" CardSold=\"434\" Dodonew=\"43434\" Description=\"Description\" />";

			CString strReport(_T("<ShiftInfo "));
			strReport.AppendFormat(_T("NetbarName=\"%s\" "), CNetBarConfig::GetInstance()->GetNetBarName());
			strReport.AppendFormat(_T("Cashier=\"%s\" "), theApp.GetCurCashier()->GetName());
			strReport.AppendFormat(_T("OnDutyTime=\"%s\" "), m_strOnDutyTime);
			strReport.AppendFormat(_T("OffDutyTime=\"%s\" "), CIBAHelpper::GetCurrentTimeAsString(TRUE));
			strReport.AppendFormat(_T("CreditMoney=\"%d\" "), m_ShiftTotalMethod.GetCreditAmount());
			strReport.AppendFormat(_T("ReturnMoney=\"%d\" "), m_ShiftTotalMethod.GetReturnedAmount());
			strReport.AppendFormat(_T("CashSold=\"%d\" "), m_ShiftTotalMethod.GetCashPay());
			strReport.AppendFormat(_T("PayCard=\"%d\" "), m_ShiftTotalMethod.GetDiankaAmount());
			strReport.AppendFormat(_T("StoreIn=\"%d\" "), m_ShiftTotalMethod.GetStoreInAmount());
			strReport.AppendFormat(_T("Payout=\"%d\" "), nPayout);
			strReport.AppendFormat(_T("UseDeposit=\"%d\" "), 0);
			strReport.AppendFormat(_T("Deposit=\"%d\" "), m_ShiftTotalMethod.GetDeposit());
			strReport.AppendFormat(_T("ShiftIn=\"%d\" "), m_nDutyIncome - nPayout);
			strReport.AppendFormat(_T("LastRemain=\"%d\" "), m_ShiftTotalMethod.GetReserveAmount());
			strReport.AppendFormat(_T("AllCash=\"%d\" "), nFetchAmount + nRemainAmount);
			strReport.AppendFormat(_T("NextRemain=\"%d\" "), nRemainAmount);
			strReport.AppendFormat(_T("FetchMoney=\"%d\" "), nFetchAmount);
			strReport.AppendFormat(_T("CreditCard=\"%d\" "), m_ShiftTotalMethod.GetCreditCard());
			strReport.AppendFormat(_T("CardSold=\"%d\" "), m_ShiftTotalMethod.GetNetBarPay());
			strReport.AppendFormat(_T("Dodonew=\"%d\" "), m_ShiftTotalMethod.GetZSBConsume());
			strReport.AppendFormat(_T("Description=\"%s\" "), m_strDescription);
			
			strReport += _T("/>");
			sr.ShowReport(1,strReport);
		}
		else
		{
			CIBALog::GetInstance()->Write(_T("Can't create IBAShiftReport!"));
		}
	}
	else
	{
		CShiftCash ShiftCash;
		ShiftCash.SetStartDate(m_strOnDutyTime);
		ShiftCash.SetOperator(theApp.GetCurCashier()->GetName());
		ShiftCash.SetCashRegisterID(CIBAConfig::GetInstance()->GetCashRegisterID());
		ShiftCash.SetCardCreditedAmount(m_ShiftTotalMethod.GetCreditCard());
		ShiftCash.SetCostExpense(m_ShiftTotalMethod.GetCostExpense());
		ShiftCash.SetCreditAmount(m_ShiftTotalMethod.GetCreditAmount());
		ShiftCash.SetDeposit(m_ShiftTotalMethod.GetDeposit());
		ShiftCash.SetReturnAmount(m_ShiftTotalMethod.GetReturnedAmount());
		ShiftCash.SetDescription(m_strDescription);
		CString strRemain;
		GetDlgItem(IDC_STATIC_RESERVE)->GetWindowText(strRemain);
		ShiftCash.SetRemainAmount((INT)(_tstof(strRemain) * 100));
		ShiftCash.SetNextRemainAmount(m_nNextReserve);//��һ�౸�ý�
		ShiftCash.SetCashSoldAmount(m_ShiftTotalMethod.GetCashPay());
		ShiftCash.SetPayout(nPayout);
		ShiftCash.SetTotalAmount(nFetchAmount + nRemainAmount);
		ShiftCash.SetFetchAmount(nFetchAmount);
		ShiftCash.SetEndDate(CIBAHelpper::GetCurrentTimeAsString(TRUE));
		ShiftCash.SetDianka(m_ShiftTotalMethod.GetDiankaAmount());
		ShiftCash.SetAccountPay(m_ShiftTotalMethod.GetNetBarPay());
		ShiftCash.SetZSBConsume(m_ShiftTotalMethod.GetZSBConsume());
		ShiftCash.SetZSBBalance(m_ShiftTotalMethod.GetZSBBalance());
		ShiftCash.SetStoreInAmount(m_ShiftTotalMethod.GetStoreInAmount());

		CString strDutyIncome;
		GetDlgItem(IDC_STATIC_NETCASH)->GetWindowText(strDutyIncome);
		ShiftCash.SetDutyIncome((INT)(_tstof(strDutyIncome) * 100));

		PrintTicket(ShiftCash);
	}
}

void CDlgIBADuty::ShowShiftTotalInfo()
{
	CString strTmp;
	
	//��ֵ���
	strTmp.Format(_T("%.2f"), m_ShiftTotalMethod.GetCreditAmount() / 100.0);
	GetDlgItem(IDC_STATIC_CREDIT)->SetWindowText(strTmp);
	
	//�ֽ���
	strTmp.Format(_T("%.2f"), m_ShiftTotalMethod.GetCashPay() / 100.0);
	GetDlgItem(IDC_STATIC_CASHPAY)->SetWindowText(strTmp);
	
	//�˿���
	strTmp.Format(_T("%.2f"), m_ShiftTotalMethod.GetReturnedAmount() / -100.0); //��ʾΪ����
	GetDlgItem(IDC_STATIC_RETURNED)->SetWindowText(strTmp);
	
	//��ֵ�����������ֽ��ܶ�
	strTmp.Format(_T("%.2f"), m_ShiftTotalMethod.GetCreditCard() / 100.0);
	GetDlgItem(IDC_STATIC_CREDITCARD)->SetWindowText(strTmp);

	//{ 2011/05/10-gxx: ���ձ� �����ѽ����)
	
	INT nZSBCom = m_ShiftTotalMethod.GetZSBConsume();
	strTmp.Format(_T("%.2f"), nZSBCom / 100.0);
	GetDlgItem(IDC_STATIC_ZSB_CONSUME)->SetWindowText(strTmp);

	strTmp.Format(_T("%.2f"), m_ShiftTotalMethod.GetZSBBalance() / 100.0);
	GetDlgItem(IDC_STATIC_ZSB_BALANCE)->SetWindowText(strTmp);

	//}

	// 2011/07/26-8201-gxx: �㿨����
	strTmp.Format(_T("%.2f"), m_ShiftTotalMethod.GetDiankaAmount() / 100.0);
	GetDlgItem(IDC_STATIC_DIANKA)->SetWindowText(strTmp);
	

	//�˻�����������ֽ��ܶ�
	strTmp.Format(_T("%.2f"), m_ShiftTotalMethod.GetNetBarPay() / 100.0);
	GetDlgItem(IDC_STATIC_NETBARPAY)->SetWindowText(strTmp);

	//��Ʒ�ɹ�֧��
	strTmp.Format(_T("%.2f"), m_ShiftTotalMethod.GetStoreInAmount() / -100.0);
	GetDlgItem(IDC_STATIC_STOREIN)->SetWindowText(strTmp);

	//Ѻ��
	strTmp.Format(_T("%.2f"), m_ShiftTotalMethod.GetDeposit() / 100.0);
	GetDlgItem(IDC_STATIC_DEPOSIT)->SetWindowText(strTmp);

	//��һ������
	strTmp.Format(_T("%.2f"), m_ShiftTotalMethod.GetReserveAmount() / 100.0);
	GetDlgItem(IDC_STATIC_RESERVE)->SetWindowText(strTmp);
	m_edtReverveAmount.SetWindowText(strTmp);
	
	UINT nCasherID = CIBAConfig::GetInstance()->GetCashRegisterID();
	
	// �������
	INT nAllMoney = 0;
	nAllMoney += m_ShiftTotalMethod.GetCreditAmount();
	nAllMoney -= m_ShiftTotalMethod.GetReturnedAmount();
	nAllMoney += m_ShiftTotalMethod.GetCashPay();
	nAllMoney += m_ShiftTotalMethod.GetDeposit();
	nAllMoney -= m_ShiftTotalMethod.GetStoreInAmount();
	nAllMoney -= 0;
	nAllMoney += m_ShiftTotalMethod.GetCostExpense(); // +������
	nAllMoney += m_ShiftTotalMethod.GetDiankaAmount(); // 2011/07/26-8201-gxx: +�㿨����
	m_nDutyIncome = nAllMoney;
	strTmp.Format(_T("%.2f"), m_nDutyIncome / 100.0);
	GetDlgItem(IDC_STATIC_NETCASH)->SetWindowText(strTmp);

	

	//�����ֽ��ܶ�=�������+��һ��Ԥ��
	nAllMoney += m_ShiftTotalMethod.GetReserveAmount();
	m_nAllMoney = nAllMoney;
	strTmp.Format(_T("%.2f"), m_nAllMoney / 100.0);
	GetDlgItem(IDC_STATIC_ALL)->SetWindowText(strTmp);

	//����֧��
	m_edtPayout.SetWindowText(_T("0.0"));


	m_nNextReserve = 0;
	//���ö�̬���ý�
	if (CIBAConfig::GetInstance()->GetDynamicReserve())
	{
		//������һ��Ķ�̬���ý������ļ��㣬���ౣ��
		//m_nNextReserve = m_ShiftTotalMethod.GetTempBalance();

		//2011-03-21-gxx-�޸�Ԥ����Ļ�ȡ
		
		int nBlaSum = CLocalServer::GetInstance()->ActiveMemberList.GetDynamicReserve(nCasherID);
		m_nNextReserve = nBlaSum;

	} 

	//������һ��
	strTmp.Format(_T("%.2f"), m_nNextReserve / 100.0);
	GetDlgItem(IDC_EDIT_NEXTREVERSE)->SetWindowText(strTmp);

	//�Ͻɽ��
	strTmp.Format(_T("%.2f"), (m_nAllMoney - m_nNextReserve) / 100.0);
	GetDlgItem(IDC_STATIC_HANDLEIN)->SetWindowText(strTmp);
	
	
	//�ϰ�ʱ��
	strTmp = GetShiftName(theApp.GetCurCashier()->GetShiftType());
	strTmp.Append(_T(" "));
	strTmp.Append(theApp.GetCurCashier()->GetOnDutyTime());
	CIBAString strCaption;
	strCaption.LoadString(IDS_THIS_DUTY_INFO);
	strCaption += _T("  ");
	strCaption += strTmp;
	GetDlgItem(IDC_STATIC_GROUP_TEXT)->SetWindowText(strCaption);
}

UINT CDlgIBADuty::GetShiftType()
{
	if (CIBAConfig::GetInstance()->GetShiftCount() == 1)
	{
		return 1;
	}

	COleDateTime dtNow = COleDateTime::GetCurrentTime();

	CStringArray strArray;

	CIBAHelpper::SplitLine(CIBAConfig::GetInstance()->GetShift1(), strArray, '-');

	INT nStart = _ttoi(strArray.GetAt(0));
	INT nEnd = _ttoi(strArray.GetAt(1));

	if (nEnd > nStart)
	{
		if (dtNow.GetHour() >= nStart && dtNow.GetHour() < nEnd)
		{
			return 1;
		}
	}
	else
	{
		if (dtNow.GetHour() >= nStart || dtNow.GetHour() < nEnd)
		{
			return 1;
		}
	}

	if (CIBAConfig::GetInstance()->GetShiftCount() == 3)
	{
		CIBAHelpper::SplitLine(CIBAConfig::GetInstance()->GetShift2(), strArray, '-');

		nStart = _ttoi(strArray.GetAt(0));
		nEnd = _ttoi(strArray.GetAt(1));

		if (nEnd > nStart)
		{
			if (dtNow.GetHour() >= nStart && dtNow.GetHour() < nEnd)
			{
				return 2;
			}
		}
		else
		{
			if (dtNow.GetHour() >= nStart || dtNow.GetHour() < nEnd)
			{
				return 2;
			}
		}
	}

	return 3;
}

void CDlgIBADuty::DoOnDuty()
{
	//ȡ���û�����ı��ý�
	UpdateData();
	
	//���ö�̬���ý�
	if (CIBAConfig::GetInstance()->GetDynamicReserve())
	{
		GetDlgItem(IDC_EDIT_RESERVE)->EnableWindow(FALSE);
	} 

	//������ȡ�ñ��ý�
	CString strTmp;
	m_edtReverveAmount.GetWindowText(strTmp);
	double dblReverveAmount = _tstof(strTmp);
	m_nNextReserve = dblReverveAmount * 100;
	
	//�ϰ෽��
	COnDutyMethod OnDuty;

	OnDuty.SetShiftType(m_cboShift.GetCurSel() + 1);

	OnDuty.SetReserveAmount(m_nNextReserve);

	theApp.GetCurCashier()->DoOnDuty(OnDuty);

	if (OnDuty.GetStatusCode() == 0)
	{			
		MsgBoxFormat(MB_OK, IDS_ONDUTYOK, theApp.GetCurCashier()->GetName());

		CBCGPDialog::OnOK();
	}
	else
	{
		if (OnDuty.GetStatusCode() == ESC_BCCS74)
		{
			ShowToolTip(IDS_ERRORBCCS, IDOK, TRUE);
		}
		else if (OnDuty.GetStatusCode() == ESC_YJSB72)
		{
			CString strLastUser, strTmp;
			
			theApp.GetCurCashier()->CheckLastDuty(strLastUser);

			strTmp.Format(IDS_ERRORYJSB, strLastUser);

			SetToolTipPos(IDOK);

			ShowToolTip(strTmp, TRUE);
		}
		else
		{
			SetToolTipPos(IDOK);

			ShowToolTip(OnDuty.GetStatusMessage(), TRUE);
		}
	}
}

void CDlgIBADuty::DoOffDuty()
{
	//�����������ֹ�����ͳ���
	m_nNextReserve = (_tstof(m_strNextReverse) + 0.005) * 100;
	
	if (m_nNextReserve < 0)
	{
		SetToolTipPos(IDC_EDIT_NEXTREVERSE);
		ShowToolTip(IDS_INPUTEERRO);
		return;
	}

	CShiftCash ShiftCash;//����Ա���෽��
	ShiftCash.SetStartDate(theApp.GetCurCashier()->GetOnDutyTime());
	ShiftCash.SetOperator(theApp.GetCurCashier()->GetName());
	ShiftCash.SetCashRegisterID(CIBAConfig::GetInstance()->GetCashRegisterID());
	ShiftCash.SetCardCreditedAmount(m_ShiftTotalMethod.GetCreditCard());
	ShiftCash.SetCostExpense(m_ShiftTotalMethod.GetCostExpense());
	ShiftCash.SetCreditAmount(m_ShiftTotalMethod.GetCreditAmount());
	ShiftCash.SetDeposit(m_ShiftTotalMethod.GetDeposit());
	ShiftCash.SetReturnAmount(m_ShiftTotalMethod.GetReturnedAmount());
	ShiftCash.SetDescription(m_strDescription);
	ShiftCash.SetRemainAmount(m_nNextReserve );
	ShiftCash.SetCashSoldAmount(m_ShiftTotalMethod.GetCashPay());
	ShiftCash.SetNextOperator(GetShiftName(theApp.GetCurCashier()->GetShiftType()));

	COffDutyMethod OffDuty;//�°෽��
	OffDuty.SetDeposit(m_ShiftTotalMethod.GetDeposit());
	OffDuty.SetCostExpense(m_ShiftTotalMethod.GetCostExpense());
	OffDuty.SetCreditAmount(m_ShiftTotalMethod.GetCreditAmount());
	OffDuty.SetCreditCard(m_ShiftTotalMethod.GetCreditCard());
	OffDuty.SetCashPay(m_ShiftTotalMethod.GetCashPay());
	OffDuty.SetRecordCount(m_ShiftTotalMethod.GetRecordCount());
	OffDuty.SetClassInfoRecords(m_ShiftTotalMethod.GetCardInfoRecords());
	OffDuty.SetReturnedAmount(m_ShiftTotalMethod.GetReturnedAmount());
	OffDuty.SetDescription(m_strDescription);
	OffDuty.SetNetBarPay(m_ShiftTotalMethod.GetNetBarPay());
	OffDuty.SetStoreInAmount(m_ShiftTotalMethod.GetStoreInAmount());

	INT nPayout = 0, nRemainAmount = 0, nFetchAmount = 0;

	GetEachMoney(nPayout, nRemainAmount, nFetchAmount);

	OffDuty.SetPayout(nPayout);
	OffDuty.SetRemainAmount(nRemainAmount);
	OffDuty.SetFetchAmount(nFetchAmount);

	ShiftCash.SetPayout(nPayout);
	ShiftCash.SetTotalAmount(nFetchAmount);

	theApp.GetCurCashier()->DoOffDuty(OffDuty);

	if (OffDuty.GetStatusCode() == 0)
	{
		ShiftCash.SetEndDate(CIBAHelpper::FormatCenterTime(OffDuty.GetOffDutyTime()));

		CIBADAL::GetInstance()->AddShiftCashRecord(ShiftCash);//д�����ݿ�
		//д�����ļ�
		CIBAConfig::GetInstance()->SetDynamicReserveMoney(ShiftCash.GetRemainAmount());
		//�Զ������ӡ
		if (CIBAConfig::GetInstance()->GetPrintShiftReport())
		{
			if (GetDlgItem(IDC_BUTTON_PRINT)->IsWindowEnabled())
			{
				OnBnClickedButtonPrint();
			}
		}

		MsgBoxFormat(MB_OK, IDS_OFFDUTYOK, theApp.GetCurCashier()->GetName());

		DeleteOffDutyOperationFile();

		// 2011/07/27-8201-gxx: �رյ㿨ƽ̨
		DestroyPayCardPlatform();

		CBCGPDialog::OnOK();
	}
	else
	{
		SetToolTipPos(IDOK);
		ShowToolTip(OffDuty.GetStatusMessage(), TRUE);
	}
}

void CDlgIBADuty::InitGridTree()
{
	CRect rectGrid;
	GetDlgItem(IDC_STATIC_JXC)->GetClientRect(&rectGrid);
	GetDlgItem(IDC_STATIC_JXC)->MapWindowPoints(this, &rectGrid);

	m_wndGridTree.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rectGrid, this, (UINT)-1);

	CIBAString strHeader;
	strHeader.LoadString(IDS_DUTYJXC);
	//strHeader = L"��Ʒ���|��Ʒ����|��Ʒ����|�۸�|���|�ֽ���|���|�˻�����|����|�һ�����|�������|�����";

	CStringArray strArray;
	CIBAHelpper::SplitLine(strHeader, strArray);

	if (theApp.GetCurCashier()->IsOnDuty())
	{
		const UINT Width[] = {60, 180, 90, 40, 40, 60, 40, 60, 40, 60, 60, 60};

		for (INT i = 0; i < strArray.GetCount() - 2; i++)
		{
			m_wndGridTree.InsertColumn(i, strArray.GetAt(i), Width[i]);
			m_wndGridTree.SetColumnAlign(i, HDF_LEFT);
		}
	}
	else
	{
		m_wndGridTree.SetFont(&m_Font);

		const UINT Width[] = {70, 250, 140, 90, 90};

		for (INT i = 0; i < 5; i++)
		{
			m_wndGridTree.InsertColumn(i, strArray.GetAt(i), Width[i]);
			m_wndGridTree.SetColumnAlign(i, HDF_LEFT);
		}
	}
}

//������Ʒ����
void CDlgIBADuty::FillGird()
{	
	m_wndGridTree.RemoveAll();

	CCommodityArray CommodityArray;//������Ʒ����
	CCommodityQueryCondition CommodityQueryCondition;//��Ʒ��ѯ����
	//�����ݿ�ȡ�ÿ������
	if (!CIBADAL::GetInstance()->GetStoreCommodity(CommodityArray, CommodityQueryCondition)) return;

	CCommoditySoldInfoArray CommoditySoldInfoArray;
	if (!CIBADAL::GetInstance()->GetCommoditySoldInfo(CommoditySoldInfoArray)) 
	{
		//
	}

	//ȡ������
	const INT nColumns = m_wndGridTree.GetColumnCount();

	INT nTmp = 0; //�������

	CString strLastGroupId;//��һ�ε���Ʒ
	CBCGPGridRow* pLastGroup = NULL;
	//��Ʒ��¼
	for (INT i = 0; i < CommodityArray.GetCount(); i++)
	{	//��Ʒ��Ϣ
		CCommodity & Commodity = CommodityArray.GetAt(i);
		//��ƷID
		CString strTmp = Commodity.GetGroupId();
		//����Ʒû�г��ֹ�
		if (strLastGroupId != strTmp)
		{	
			if (nTmp > 0)//��������0
			{
				pLastGroup->GetItem(4)->SetValue(nTmp);
				nTmp = 0;
			}

			strLastGroupId = strTmp;
			//������¼
			pLastGroup = m_wndGridTree.CreateRow(nColumns);
			pLastGroup->AllowSubItems();// ����ʽ�Ӳ˵�
			pLastGroup->GetItem(0)->SetValue(Commodity.GetGroupName().GetString());
			//��Ӽ�¼
			m_wndGridTree.AddRow(pLastGroup, FALSE);
		}
		//����Ʒ���ֹ�
		CBCGPGridRow* pCommodity = m_wndGridTree.CreateRow(nColumns);
		ASSERT_VALID(pCommodity);
		//��������
		pCommodity->GetItem(1)->SetValue(Commodity.GetCommodityName().GetString());
		pCommodity->GetItem(2)->SetValue(Commodity.GetBarCode().GetString());
		strTmp.Format(_T("%.2f"), Commodity.GetAdvicePrice() / 100.0);
		pCommodity->GetItem(3)->SetValue(strTmp.GetString());
		pCommodity->GetItem(4)->SetValue(Commodity.GetRemainQuantity());

		if (theApp.GetCurCashier()->IsOnDuty())
		{
			for (INT j = 0; j < CommoditySoldInfoArray.GetCount(); j++)
			{
				CCommoditySoldInfo CommoditySoldInfo = CommoditySoldInfoArray.GetAt(j);

				if (CommoditySoldInfo.GetCommoditycode().CompareNoCase(Commodity.GetBarCode()) == 0)
				{
					//�ֽ�ϼ�����
					INT nCount = CommoditySoldInfo.GetSoldCount1() + CommoditySoldInfo.GetReturnCount1();

					if (nCount > 0)
					{
						pCommodity->GetItem(5)->SetValue(nCount);
						strTmp.Format(_T("%.2f"), (CommoditySoldInfo.GetSoldAmout1() + CommoditySoldInfo.GetReturnAmout1()) / 100.0);
						pCommodity->GetItem(6)->SetValue(strTmp.GetString());
					}
					//�˻��ϼ�����
					nCount = CommoditySoldInfo.GetSoldCount2() + CommoditySoldInfo.GetReturnCount2();

					if (nCount > 0)
					{
						pCommodity->GetItem(7)->SetValue(nCount);
					}
					//���ͺϼ�����
					if (CommoditySoldInfo.GetPresentCount() > 0)
					{
						pCommodity->GetItem(8)->SetValue(CommoditySoldInfo.GetPresentCount());
					}

					if (CommoditySoldInfo.GetExchangePoint())
					{
						pCommodity->GetItem(9)->SetValue(CommoditySoldInfo.GetExchangePoint());
					}
				}
			}

			if (Commodity.GetStoreInQuantity() > 0)
			{
				pCommodity->GetItem(10)->SetValue(Commodity.GetStoreInQuantity());
				pCommodity->GetItem(11)->SetValue(Commodity.GetStoreInAmount());
			}
		}

		nTmp += Commodity.GetRemainQuantity();

		pLastGroup->AddSubItem(pCommodity, FALSE);//������б�
	}

	if (pLastGroup != NULL) //����Ʒ����
	{
		pLastGroup->GetItem(4)->SetValue(nTmp);
		GetDlgItem(IDC_BUTTON_JXCPRINT)->EnableWindow(TRUE);
	}

	m_wndGridTree.AdjustLayout();
}
//��ӡ��Ʒ
void CDlgIBADuty::OnBnClickedButtonJxcprint()
{
	m_wndGridTree.ExpandAll();
	m_wndGridTree.Print();
}

void CDlgIBADuty::OnEnChangeEditNextreverse()
{
	OnEnChangeEditPayout();
}

INT CDlgIBADuty::Round(double dsrc)
{
	if (dsrc >= 0)
	{
		return (INT)(dsrc + 0.5);
	}
	else
	{
		return (INT)(dsrc - 0.5);
	}
}

void CDlgIBADuty::OnBnClickedButtonStorecheck()
{
	CStoreDlg StoreDlg;
	StoreDlg.DoModal();
}

void CDlgIBADuty::HideShiftTotalInfo()
{
	GetDlgItem(IDC_STATIC_HANDLEIN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC2_HANDLEIN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_NEXTREVERSE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC2_NEXTRESERVE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_RESERVE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_CREDIT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_RETURNED)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_DEPOSIT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_NETCASH)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_CASHPAY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_PAYOUT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_ALL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_NEXTREVERSE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_HANDLEIN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_CREDITCARD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_NETBARPAY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_STOREIN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_PRINT)->EnableWindow(FALSE);
}


CString CDlgIBADuty::GetShiftName(UINT nShiftType)
{
	if (1 == nShiftType)
	{
		return CIBAConfig::GetInstance()->GetShiftName1();
	}
	else if (2 == nShiftType)
	{
		return CIBAConfig::GetInstance()->GetShiftName2();
	}
	else  if (3 == nShiftType)
	{
		return CIBAConfig::GetInstance()->GetShiftName3();
	}
	else
	{
		return _T("");
	}
}

void CDlgIBADuty::DeleteOffDutyOperationFile()
{
	static TCHAR* sStrOutDir  = _T("\\IBAConfig\\operateInfo\\");
	CString strFileName = theApp.GetWorkPath();
	strFileName += sStrOutDir;
	strFileName += theApp.GetCurCashier()->GetName();
	strFileName += L".dat";

	if (PathFileExists(strFileName))
	{
		DeleteFile(strFileName);
	}
}
