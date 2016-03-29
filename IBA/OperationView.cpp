// OperationView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "Cashier.h"
#include "IBAHelpper.h"
#include "dal\IBADAL.h"
#include ".\OperationView.h"

using namespace NS_DAL;
// COperationView

IMPLEMENT_DYNCREATE(COperationView, CIBAFormView)

COperationView::COperationView()
: CIBAFormView(COperationView::IDD)
, m_bSetDate(FALSE)
{
	m_nAllMoney1 = 0;
	m_nAllMoney2 = 0;
	m_nAllMoney3 = 0;
	m_nCurrPageNo = 1;
}

COperationView::~COperationView()
{
	
}

void COperationView::DoDataExchange(CDataExchange* pDX)
{
	CIBAFormView::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_STATIC_RESULT, m_strResult);
	DDX_Text(pDX, IDC_COMBO_CASHIER, m_strOperator);
	//DDX_Text(pDX, IDC_EDIT_PERSONALID, m_strPersonalID);
	//DDX_Text(pDX, IDC_EDIT_IDCARDSNO, m_strSerialNum);
	DDX_Control(pDX, IDC_LIST_COMMON, m_List);
	DDX_Control(pDX, IDC_COMBO_OPERATION, m_cboOperationType);
	DDX_Control(pDX, IDC_BUTTON_STARTTIME, m_StartTime);
	DDX_Control(pDX, IDC_BUTTON_ENDTIME, m_EndTime);
	DDX_Control(pDX, IDC_COMBO_USERCLASS, m_cboUserClass);
	DDX_Control(pDX, IDC_COMBO_CASHIER, m_cboCashier);
}

BEGIN_MESSAGE_MAP(COperationView, CIBAFormView)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, OnBnClickedButtonQuery)
	//ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_OPERATION, OnCbnSelchangeComboOperation)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_CLICK, IDC_LIST_COMMON, OnNMClickListCommon)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_PREV, OnBnClickedButtonPrev)
END_MESSAGE_MAP()

//��ʼ���б�
void COperationView::InitListView()
{
	ClearData();//�������
	
	//ȡ���б�ͷ��������Ŀ
	INT nCount = m_List.GetHeaderCtrl().GetItemCount();

	//�������
	for (INT i = 0; i < nCount; i++)
	{
		m_List.DeleteColumn(0);
	}

	CIBAString strHeader, strWidth;

	//ˢ���б�ͷ
	switch (m_cboOperationType.GetCurSel())
	{
	case 0:// ����
		//strHeader = L"�����˺�|����|֤������|�û���|�û�����|�������|����|Ѻ��|ʱ��|����Ա";
		strHeader.LoadString(IDS_OP_VIEW_OPEN_HEAD);
		strWidth = L"120, 70, 120, 70, 70, 70, 70, 70, 120, 70";

		break;

	case 1:// ��ֵ
		//strHeader = L"�����˺�|����|֤������|�û���|�û�����|��ֵ���|����|ʱ��|����Ա";
		strHeader.LoadString(IDS_OP_VIEW_CREDIT_HEAD);
		strWidth = L"120, 70, 120, 70, 70, 70, 70, 120, 70";
		break;

	case 2:// �˿�
		//strHeader = L"�����˺�|����|֤������|�û���|�û�����|���˽��|Ѻ��|ʱ��|����Ա";
		strHeader.LoadString(IDS_OP_VIEW_RETURN_HEAD);
		strWidth = L"120, 70, 120, 70, 70, 70, 70, 120, 70";

		break;

	case 3:// ������ֵ
		//strHeader = L"�����˺�|����|֤������|�û���|�û�����|�������|����|ʱ��|����Ա";
		strHeader.LoadString(IDS_OP_VIEW_REVERSE_HEAD);
		strWidth = L"120, 70, 120, 70, 70, 70, 70, 120, 70";

		break;

	case 4:// ��ֵ����ֵ
		////strHeader = L"�����˺�|����|֤������|�û���|�û�����|��ֵ���|����|ʱ��|����Ա";
		//strHeader = L"�����˺�|����|֤������|�û���|�û�����|��ֵ�����|����|����|ʱ��|����Ա";
		strHeader.LoadString(IDS_OP_VIEW_CARD_HEAD);
		strWidth = L"120, 70, 120, 70, 70, 70, 70, 120, 120, 70";

		break;

	case 5:// �û�����
		//strHeader = L"�����˺�|����|֤������|�û���|ԭ����|�¼���|ʱ��|����Ա|��������";
		strHeader.LoadString(IDS_OP_VIEW_UP_HEAD);
		strWidth = L"120, 70, 120, 70, 70, 70, 120, 70, 150";

		break;
	}

	CStringArray strArray, strWidthArray;
	CIBAHelpper::SplitLine(strHeader, strArray);
	CIBAHelpper::SplitLine(strWidth, strWidthArray, ',');

	for (INT i = 0; i < strArray.GetCount(); i++)
	{
		m_List.InsertColumn(i, strArray.GetAt(i), LVCFMT_LEFT, _ttoi(strWidthArray.GetAt(i)));
	}

	//m_List.SetFont(&m_Font);//��������
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);//������ʽ
}

void COperationView::InitTimeControl()
{//ʱ�䰴ť�ؼ�������
	const UINT stateMask = 
		CBCGPDateTimeCtrl::DTM_SPIN |
		CBCGPDateTimeCtrl::DTM_DROPCALENDAR | 
		CBCGPDateTimeCtrl::DTM_DATE |
		CBCGPDateTimeCtrl::DTM_TIME24H |
		CBCGPDateTimeCtrl::DTM_CHECKBOX |
		CBCGPDateTimeCtrl::DTM_TIME | 
		CBCGPDateTimeCtrl::DTM_TIME24HBYLOCALE;

	UINT stateFlags = 
		CBCGPDateTimeCtrl::DTM_SPIN |
		CBCGPDateTimeCtrl::DTM_DROPCALENDAR | 
		CBCGPDateTimeCtrl::DTM_DATE |
		CBCGPDateTimeCtrl::DTM_TIME24H |
		CBCGPDateTimeCtrl::DTM_TIME;  

	m_StartTime.m_monthFormat = 2;
	m_StartTime.SetState(stateFlags, stateMask);
	m_StartTime.SetFont(&m_Font);
	m_StartTime.SizeToContent();

	m_EndTime.m_monthFormat = 2;
	m_EndTime.SetState(stateFlags, stateMask);
	m_EndTime.SetFont(&m_Font);
	m_EndTime.SizeToContent();
}

void COperationView::OnBnClickedButtonQuery()
{
	SetToolTipPos(IDC_BUTTON_QUERY);

	m_nCurrPageNo = 1;

	DoQuery();
}

//void COperationView::QueryRegister()
//{
//	CString strSQL;
//
//	strSQL.Append(_T("select cardopen.*, member.serialNum, member.name, credited.creditAmount, "));
//	
//	strSQL.Append(_T("credited.creditPresent, member.idNumber, member.classId "));
//	
//	strSQL.Append(_T("from cardopen, member, credited "));
//
//	strSQL.Append(_T("where cardopen.memberid=member.memberid and cardopen.memberid = credited.memberId "));
//
//	strSQL.Append(_T("and cardopen.operationDateTime = credited.creditDate and cardopen.serialNo = credited.serialNo "));
//
//	strSQL.AppendFormat(_T("and cardopen.operationDateTime between '%s' and '%s'")
//	, m_StartTime.ToString(), m_EndTime.ToString());
//
//	if (!m_strOperator.IsEmpty())
//	{
//		strSQL.AppendFormat(_T(" and cardopen.operator='%s'"), m_strOperator);
//	}
//
//	if (!m_strPersonalID.IsEmpty())
//	{
//		strSQL.AppendFormat(_T(" and member.idNumber='%s'"), m_strPersonalID);
//	}
//
//	if (!m_strSerialNum.IsEmpty())
//	{
//		strSQL.AppendFormat(_T(" and member.serialNum like '%%%s%%'"), m_strSerialNum);
//	}
//
//	strSQL.Append(_T(" order by cardopen.operationDateTime desc"));
//
//	CIBALog::GetInstance()->WriteFormat(CIBALog::ELT_SQL, _T("QueryRegister:%s"), strSQL);
//
//	CADODBConnInfo* pDb = NULL;
//
//	if (!CIBADAL::GetInstance()->GetDBConnInfo(pDb)) return;
//
//	CADORecordset Rs(pDb->GetConn());
//
//	if (Rs.Open(strSQL))
//	{
//		INT i = 0;
//
//		while (!Rs.IsEof())
//		{
//			INT nTmp = 0;
//			CString strTmp;
//
//			Rs.GetFieldValue(_T("serialNum"), strTmp);
//			m_List.InsertItem(i, strTmp);
//
//			Rs.GetFieldValue(_T("name"), strTmp);
//			m_List.SetItemText(i, 1, strTmp);
//
//			Rs.GetFieldValue(_T("idNumber"), strTmp);
//			m_List.SetItemText(i, 2, strTmp);
//
//			Rs.GetFieldValue(_T("memberId"), nTmp);
//			strTmp.Format(_T("%d"), nTmp);
//			m_List.SetItemText(i, 3, strTmp);
//
//			Rs.GetFieldValue(_T("classId"), nTmp);
//			StatUserCount(nTmp, strTmp);
//			m_List.SetItemText(i, 4, strTmp);
//
//			Rs.GetFieldValue(_T("creditAmount"), nTmp);
//			m_nAllMoney1 += nTmp;
//			strTmp.Format(_T("%.2f"), nTmp / 100.0);
//			m_List.SetItemText(i, 5, strTmp);
//
//			Rs.GetFieldValue(_T("creditPresent"), nTmp);
//			m_nAllMoney2 += nTmp;
//			strTmp.Format(_T("%.2f"), nTmp / 100.0);
//			m_List.SetItemText(i, 6, strTmp);
//
//			Rs.GetFieldValue(_T("deposit"), nTmp);
//			m_nAllMoney3 += nTmp;
//			strTmp.Format(_T("%.2f"), nTmp / 100.0);
//			m_List.SetItemText(i, 7, strTmp);
//
//			Rs.GetFieldValue(_T("operationDateTime"), strTmp);
//			m_List.SetItemText(i, 8, strTmp);
//
//			Rs.GetFieldValue(_T("operator"), strTmp);
//			m_List.SetItemText(i, 9, strTmp);
//
//			i++;
//			Rs.MoveNext();
//		}
//
//		m_nRecordCount = Rs.GetRecordCount();
//		Rs.Close();
//	}
//
//	pDb->Release();
//
//	m_strResult.Format(L"�ܹ���ѯ�� %d ����¼���������:%.2f��Ѻ��:%.2f�����ͽ��:%.2f���ϼ�:%.2f",
//		m_nRecordCount, m_nAllMoney1 / 100.0, m_nAllMoney3 / 100.0, m_nAllMoney2 / 100.0, 
//		(m_nAllMoney1 + m_nAllMoney2 + m_nAllMoney3) / 100.0);
//
//}
//
//void COperationView::QueryCredit(BOOL bReverse)
//{   //
//	CString strSQL;
//
//	strSQL.Append(_T("select credited.*, member.serialNum, member.name, member.idNumber,"));
//	strSQL.Append(_T("member.classId from credited, member where credited.memberid=member.memberid "));
//
//	if (bReverse)
//	{
//		strSQL.AppendFormat(_T("and credited.creditDate between '%s' and '%s' and credited.creditAmount < 0")
//			, m_StartTime.ToString(), m_EndTime.ToString());
//	}
//	else
//	{
//		strSQL.AppendFormat(_T("and credited.creditDate between '%s' and '%s' and credited.creditAmount > 0")
//			, m_StartTime.ToString(), m_EndTime.ToString());
//	}
//
//	if (!m_strOperator.IsEmpty())
//	{
//		strSQL.AppendFormat(_T(" and credited.operator='%s'"), m_strOperator);
//	}
//
//	if (m_cboUserClass.GetCurSel() > 0 )
//	{
//		strSQL.AppendFormat(_T(" and member.classId=%d"), m_cboUserClass.GetItemData(m_cboUserClass.GetCurSel()));
//	}
//
//	if (!m_strPersonalID.IsEmpty())
//	{
//		strSQL.AppendFormat(_T(" and member.idNumber='%s'"), m_strPersonalID);
//	}
//
//	if (!m_strSerialNum.IsEmpty())
//	{
//		strSQL.AppendFormat(_T(" and member.serialNum like '%%%s%%'"), m_strSerialNum);
//	}
//
//	strSQL.Append(_T(" order by credited.creditDate desc"));
//	//д�����ݿ�
//	CIBALog::GetInstance()->WriteFormat(CIBALog::ELT_SQL, _T("QueryCredit:%s"), strSQL);
//
//	CADODBConnInfo* pDb = NULL;
//	//DB����
//	if (!CIBADAL::GetInstance()->GetDBConnInfo(pDb)) return;
//	//����DB��¼��
//	CADORecordset Rs(pDb->GetConn());
//	
//	if (Rs.Open(strSQL))
//	{
//		INT i = 0;
//
//		while (!Rs.IsEof())
//		{
//			INT nTmp = 0;
//			CString strTmp;
//
//			Rs.GetFieldValue(_T("serialNum"), strTmp);
//			m_List.InsertItem(i, strTmp);
//
//			Rs.GetFieldValue(_T("name"), strTmp);
//			m_List.SetItemText(i, 1, strTmp);
//
//			Rs.GetFieldValue(_T("idNumber"), strTmp);
//			m_List.SetItemText(i, 2, strTmp);
//
//			Rs.GetFieldValue(_T("memberId"), nTmp);
//			strTmp.Format(_T("%d"), nTmp);
//			m_List.SetItemText(i, 3, strTmp);
//
//			Rs.GetFieldValue(_T("classId"), nTmp);
//			StatUserCount(nTmp, strTmp);
//			strTmp = CNetBarConfig::GetInstance()->GetUserClassName(nTmp);
//			m_List.SetItemText(i, 4, strTmp);
//
//			Rs.GetFieldValue(_T("creditAmount"), nTmp);
//			m_nAllMoney1 += nTmp;
//			strTmp.Format(_T("%.2f"), nTmp / 100.0);
//			m_List.SetItemText(i, 5, strTmp);
//			
//			Rs.GetFieldValue(_T("creditPresent"), nTmp);
//			m_nAllMoney2 += nTmp;
//			strTmp.Format(_T("%.2f"), nTmp / 100.0);
//			m_List.SetItemText(i, 6, strTmp);
//
//			Rs.GetFieldValue(_T("creditdate"), strTmp);
//			m_List.SetItemText(i, 7, strTmp);
//
//			Rs.GetFieldValue(_T("operator"), strTmp);
//			m_List.SetItemText(i, 8, strTmp);
//
//			i++;
//			Rs.MoveNext();
//		}
//
//		m_nRecordCount = Rs.GetRecordCount();
//
//		Rs.Close();
//	}
//
//	pDb->Release();
//
//	m_strResult.Format(L"�ܹ���ѯ�� %d ����¼����ֵ���:%.2f�����ͽ��:%.2f���ϼ�:%.2f",
//		m_nRecordCount, m_nAllMoney1 / 100.0, m_nAllMoney2 / 100.0, (m_nAllMoney1 + m_nAllMoney2) / 100.0);
//
//}
//
//void COperationView::QueryReturned()
//{
//	CString strSQL;
//
//	strSQL.Append(_T("select returned.*, member.serialNum, member.name, member.idNumber,"));
//	strSQL.Append(_T("member.classId from returned, member where returned.memberid=member.memberid "));
//	strSQL.AppendFormat(_T("and returned.returnDate between '%s' and '%s'")
//		, m_StartTime.ToString(), m_EndTime.ToString());
//
//	if (!m_strOperator.IsEmpty())
//	{
//		strSQL.AppendFormat(_T(" and returned.operator='%s'"), m_strOperator);
//	}
//
//	if (!m_strPersonalID.IsEmpty())
//	{
//		strSQL.AppendFormat(_T(" and member.idNumber='%s'"), m_strPersonalID);
//	}
//
//	if (!m_strSerialNum.IsEmpty())
//	{
//		strSQL.AppendFormat(_T(" and member.serialNum like '%%%s%%'"), m_strSerialNum);
//	}
//
//	strSQL.Append(_T(" order by returned.returnDate desc"));
//
//	CIBALog::GetInstance()->WriteFormat(CIBALog::ELT_SQL,  _T("QueryReturned:%s"), strSQL);
//
//	CADODBConnInfo* pDb = NULL;
//
//	if (!CIBADAL::GetInstance()->GetDBConnInfo(pDb)) return;
//
//	CADORecordset Rs(pDb->GetConn());
//
//	if (Rs.Open(strSQL))
//	{
//		INT i = 0;
//
//		while (!Rs.IsEof())
//		{
//			INT nTmp = 0;
//			CString strTmp;
//
//			Rs.GetFieldValue(_T("serialNum"), strTmp);
//			m_List.InsertItem(i, strTmp);
//
//			Rs.GetFieldValue(_T("name"), strTmp);
//			m_List.SetItemText(i, 1, strTmp);
//
//			Rs.GetFieldValue(_T("idNumber"), strTmp);
//			m_List.SetItemText(i, 2, strTmp);
//
//			Rs.GetFieldValue(_T("memberId"), nTmp);
//			strTmp.Format(_T("%d"), nTmp);
//			m_List.SetItemText(i, 3, strTmp);
//
//			Rs.GetFieldValue(_T("classId"), nTmp);
//			StatUserCount(nTmp, strTmp);
//			strTmp = CNetBarConfig::GetInstance()->GetUserClassName(nTmp);
//			m_List.SetItemText(i, 4, strTmp);
//
//			Rs.GetFieldValue(_T("returnAmount"), nTmp);
//			m_nAllMoney1 += nTmp;
//			strTmp.Format(_T("%.2f"), nTmp / 100.0);
//			m_List.SetItemText(i, 5, strTmp);
//
//			Rs.GetFieldValue(_T("deposit"), nTmp);
//			m_nAllMoney3 += nTmp;
//			strTmp.Format(_T("%.2f"), nTmp / 100.0);
//			m_List.SetItemText(i, 6, strTmp);
//
//			Rs.GetFieldValue(_T("returnDate"), strTmp);
//			m_List.SetItemText(i, 7, strTmp);
//
//			Rs.GetFieldValue(_T("operator"), strTmp);
//			m_List.SetItemText(i, 8, strTmp);
//
//			i++;
//			Rs.MoveNext();
//		}
//
//		m_nRecordCount = Rs.GetRecordCount();
//
//		Rs.Close();
//	}
//
//	pDb->Release();
//
//	m_strResult.Format(L"�ܹ���ѯ�� %d ����¼�����˽��:%.2f������Ѻ��:%.2f",
//		m_nRecordCount, m_nAllMoney1 / 100.0, m_nAllMoney3 / 100.0);
//}
//

void COperationView::OnInitialUpdate()
{
	CIBAFormView::OnInitialUpdate();

	InitTimeControl();
	
	SetEditFont();
	
	GetDlgItem(IDC_STATIC_RESULT)->SetFont(&m_Font);

	m_cboOperationType.SetFont(&m_Font);

#ifndef __USING_INTER_RESOURCE
	CIBAString strTypes;
	strTypes.LoadString(IDS_OP_VIEW_TYPES);
	CStringArray strArr;
	CIBAHelpper::SplitLine(strTypes, strArr);
	ASSERT(strArr.GetCount() == 6);
	m_cboOperationType.ResetContent();
	for(int i=0; i < strArr.GetCount(); i++)
	{
		m_cboOperationType.AddString(strArr.GetAt(i));
	}
#endif
	m_cboOperationType.SetCurSel(1);
	
	m_cboUserClass.SetFont(&m_Font);
	m_cboCashier.SetFont(&m_Font);

	COperatorArray OperatorArray;
	
	CIBADAL::GetInstance()->GetOperatorRecord(OperatorArray);

	for (INT i = 0; i < OperatorArray.GetCount(); i++)
	{
		m_cboCashier.AddString(OperatorArray.GetAt(i));
	}
	
	InitListView();

	SetTimer(1000, 60 * 1000, NULL);
}

void COperationView::OnClose()
{
	INT nIdx = theApp.GetIBAView()->FindTab(GetSafeHwnd());
	theApp.GetIBAView()->GetTabControl().ShowTab(nIdx, FALSE);

	m_List.DeleteAllItems();

	m_strResult.Empty();
	m_strOperator.Empty();
	m_strPersonalID.Empty();
	m_strSerialNum.Empty();

	UpdateData(FALSE);

	//CIBAFormView::OnClose();
}

void COperationView::OnSize(UINT nType, int cx, int cy)
{
	MoveWindow(0, 0, cx, cy, FALSE);

	if (m_List.GetSafeHwnd() != NULL)
	{
		m_List.MoveWindow(10, 135, cx - 15, cy - 140, TRUE);

		InitData();
	}
}

void COperationView::OnTimer(UINT nIDEvent)
{
	if (1000 == nIDEvent)
	{
		COleDateTime dtTmp = COleDateTime::GetCurrentTime();
		dtTmp += COleDateTimeSpan(1, 0, 0, 0);
		m_EndTime.SetDate(dtTmp);
	}

	CIBAFormView::OnTimer(nIDEvent);
}

void COperationView::InitData()
{
	if (m_bSetDate)
	{
		return;
	}

	COleDateTime dtTmp;

	if (theApp.GetCurCashier()->IsOnDuty())
	{
		dtTmp.ParseDateTime(theApp.GetCurCashier()->GetOnDutyTime());
		m_bSetDate = TRUE;

		InitUserClassComboBox();
		m_cboCashier.SetWindowText(theApp.GetCurCashier()->GetName());
	}
	else
	{
		dtTmp = COleDateTime::GetCurrentTime();
		dtTmp -= COleDateTimeSpan(1, 0, 0, 0);
	}

	m_StartTime.SetDate(dtTmp);

	dtTmp = COleDateTime::GetCurrentTime();
	dtTmp += COleDateTimeSpan(1, 0, 0, 0);
	m_EndTime.SetDate(dtTmp);
}

void COperationView::OnCbnSelchangeComboOperation()
{
	InitListView();

	GetDlgItem(IDC_BUTTON_PREV)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
}

HBRUSH COperationView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CIBAFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	if (CTLCOLOR_STATIC == nCtlColor)
	{
		if (pWnd->GetDlgCtrlID() == IDC_STATIC_RESULT)
		{
			pDC->SetTextColor(RGB(0, 0, 255));
		}
	}
	return hbr;
}
//ȡ���û�����ͬʱͳ���û�����,ͬʱͳ��δ֪�û�����
void COperationView::StatUserCount(UINT nUserClass, CString& strClassName)
{
	strClassName.Empty();

	for (INT j = 0; j < CNetBarConfig::GetInstance()->m_UserClassArray.GetCount(); j++)
	{
		if (CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(j).GetClassID() == nUserClass)
		{
			strClassName = CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(j).GetClassName();
			
			UINT nTmp = m_nUserCount.GetAt(j);

			nTmp++;
			m_nUserCount.SetAt(j, nTmp);

			break;
		}
	}

	if (strClassName.IsEmpty())
	{
		strClassName.Format(_T("%d"), nUserClass);
		m_nUnkonwUserClass++;
	}
}

void COperationView::ClearData()
{
	m_nUserCount.RemoveAll();
	m_nUserCount.SetSize(CNetBarConfig::GetInstance()->m_UserClassArray.GetCount());
	m_nUnkonwUserClass = 0;
	m_nRecordCount = 0;
	m_nAllMoney1 = 0;
	m_nAllMoney2 = 0;
	m_nAllMoney3 = 0;
	m_strResult.Empty();
	UpdateData(FALSE);

	m_List.DeleteAllItems();
}

void COperationView::OnNMClickListCommon(NMHDR *pNMHDR, LRESULT *pResult)
{
	INT nIdx = m_List.GetSelectionMark();

	if (nIdx >= 0)
	{
		m_strSerialNum = m_List.GetItemText(nIdx, 0);
		UpdateData(FALSE);
	}

	*pResult = 0;
}

//void COperationView::QueryCardCredit(BOOL bReverse)
//{	//����SQL�������ִ�
//	CString strSQL;
//
//	strSQL.Append(_T("select cardcredited.*, member.serialNum, member.name, member.idNumber"));
//	strSQL.Append(_T(" from cardcredited, member where cardcredited.memberid=member.memberid "));
//
//	if (bReverse)
//	{
//		strSQL.AppendFormat(_T("and cardcredited.creditDate between '%s' and '%s' and cardcredited.creditAmount < 0")
//			, m_StartTime.ToString(), m_EndTime.ToString());
//	}
//	else
//	{
//		strSQL.AppendFormat(_T("and cardcredited.creditDate between '%s' and '%s' and cardcredited.creditAmount > 0")
//			, m_StartTime.ToString(), m_EndTime.ToString());
//	}
//
//	if (!m_strOperator.IsEmpty())
//	{
//		strSQL.AppendFormat(_T(" and cardcredited.operator='%s'"), m_strOperator);
//	}
//
//	if (!m_strPersonalID.IsEmpty())
//	{
//		strSQL.AppendFormat(_T(" and member.idNumber='%s'"), m_strPersonalID);
//	}
//
//	if (!m_strSerialNum.IsEmpty())
//	{
//		strSQL.AppendFormat(_T(" and member.serialNum like '%%%s%%'"), m_strSerialNum);
//	}
//
//	strSQL.Append(_T(" order by cardcredited.creditDate desc"));
//	//д����־
//	CIBALog::GetInstance()->WriteFormat(CIBALog::ELT_SQL, _T("QueryCardCredit:%s"), strSQL);
//
//	CADODBConnInfo* pDb = NULL;
//	//DB����
//	if (!CIBADAL::GetInstance()->GetDBConnInfo(pDb)) return;
//	//����DB��¼��
//	CADORecordset Rs(pDb->GetConn());//����
//	//��
//	if (Rs.Open(strSQL))
//	{
//		INT i = 0;
//		//ö�ټ�¼��
//		while (!Rs.IsEof())
//		{
//			INT nTmp = 0;
//			CString strTmp;
//
//			Rs.GetFieldValue(_T("serialNum"), strTmp);
//			m_List.InsertItem(i, strTmp);
//
//			Rs.GetFieldValue(_T("name"), strTmp);
//			m_List.SetItemText(i, 1, strTmp);
//
//			Rs.GetFieldValue(_T("idNumber"), strTmp);
//			m_List.SetItemText(i, 2, strTmp);
//
//			Rs.GetFieldValue(_T("memberId"), nTmp);
//			strTmp.Format(_T("%d"), nTmp);
//			m_List.SetItemText(i, 3, strTmp);
//
//			Rs.GetFieldValue(_T("classId"), nTmp);
//			StatUserCount(nTmp, strTmp);
//			m_List.SetItemText(i, 4, strTmp);
//
//			Rs.GetFieldValue(_T("creditAmount"), nTmp);
//			m_nAllMoney1 += nTmp;
//			strTmp.Format(_T("%.2f"), nTmp / 100.0);
//			m_List.SetItemText(i, 5, strTmp);
//
//			Rs.GetFieldValue(_T("creditPresent"), nTmp);
//			m_nAllMoney2 += nTmp;
//			strTmp.Format(_T("%.2f"), nTmp / 100.0);
//			m_List.SetItemText(i, 6, strTmp);
//
//			Rs.GetFieldValue(_T("creditdate"), strTmp);
//			m_List.SetItemText(i, 7, strTmp);
//
//			Rs.GetFieldValue(_T("operator"), strTmp);
//			m_List.SetItemText(i, 8, strTmp);
//
//			i++;
//			Rs.MoveNext();
//		}
//		//��¼��Ŀ
//		m_nRecordCount = Rs.GetRecordCount();
//		//�ر�
//		Rs.Close();
//	}
//	//���ݿ����ְ��
//	pDb->Release();
//	//��̬�ı���ʾ
//	m_strResult.Format(L"�ܹ���ѯ�� %d ����¼����ֵ���:%.2f�����ͽ��:%.2f���ϼ�:%.2f",
//		m_nRecordCount, m_nAllMoney1 / 100.0, m_nAllMoney2 / 100.0, (m_nAllMoney1 + m_nAllMoney2) / 100.0);
//
//}
//
//void COperationView::QueryUpgrade()
//{
//	//����SQL�������ִ�
//	CString strSQL;
//
//	strSQL.Append(_T("select memberupgrade.*, member.serialNum, member.name, member.idNumber "));
//	strSQL.Append(_T("from memberupgrade, member where memberupgrade.memberid=member.memberid "));
//
//	strSQL.AppendFormat(_T("and memberupgrade.operationTime between '%s' and '%s'")
//		, m_StartTime.ToString(), m_EndTime.ToString());
//
//	if (!m_strOperator.IsEmpty())
//	{
//		strSQL.AppendFormat(_T(" and memberupgrade.operator='%s'"), m_strOperator);
//	}
//
//	if (!m_strPersonalID.IsEmpty())
//	{
//		strSQL.AppendFormat(_T(" and member.idNumber='%s'"), m_strPersonalID);
//	}
//
//	if (!m_strSerialNum.IsEmpty())
//	{
//		strSQL.AppendFormat(_T(" and member.serialNum like '%%%s%%'"), m_strSerialNum);
//	}
//
//	strSQL.Append(_T(" order by memberupgrade.operationTime desc"));
//	//д����־
//	CIBALog::GetInstance()->WriteFormat(CIBALog::ELT_SQL, _T("QueryMemberUpgrade:%s"), strSQL);
//
//	CADODBConnInfo* pDb = NULL;
//	//DB����
//	if (!CIBADAL::GetInstance()->GetDBConnInfo(pDb)) return;
//	//����DB��¼��
//	CADORecordset Rs(pDb->GetConn());//����
//	//��
//	if (Rs.Open(strSQL))
//	{
//		INT i = 0;
//		//ö�ټ�¼��
//		while (!Rs.IsEof())
//		{
//			INT nTmp = 0;
//			CString strTmp;
//
//			Rs.GetFieldValue(_T("serialNum"), strTmp);
//			m_List.InsertItem(i, strTmp);
//
//			Rs.GetFieldValue(_T("name"), strTmp);
//			m_List.SetItemText(i, 1, strTmp);
//
//			Rs.GetFieldValue(_T("idNumber"), strTmp);
//			m_List.SetItemText(i, 2, strTmp);
//
//			Rs.GetFieldValue(_T("memberId"), nTmp);
//			strTmp.Format(_T("%d"), nTmp);
//			m_List.SetItemText(i, 3, strTmp);
//
//			Rs.GetFieldValue(_T("classId"), nTmp);
//			StatUserCount(nTmp, strTmp);
//			m_List.SetItemText(i, 4, strTmp);
//
//			Rs.GetFieldValue(_T("oldClassId"), nTmp);
//			CString strOldclass = CNetBarConfig::GetInstance()->GetUserClassName(nTmp);
//			strOldclass += _T(" -> ") + strTmp;
//			m_List.SetItemText(i, 5, strOldclass);
//
//			Rs.GetFieldValue(_T("operationTime"), strTmp);
//			m_List.SetItemText(i, 6, strTmp);
//
//			Rs.GetFieldValue(_T("operator"), strTmp);
//			m_List.SetItemText(i, 7, strTmp);
//			
//			Rs.GetFieldValue(_T("reason"), strTmp);
//			m_List.SetItemText(i, 8, strTmp);
//
//			i++;
//			Rs.MoveNext();
//		}
//		//��¼��Ŀ
//		m_nRecordCount = Rs.GetRecordCount();
//		//�ر�
//		Rs.Close();
//	}
//	//���ݿ����ְ��
//	pDb->Release();
//	//��̬�ı���ʾ
//	m_strResult.Format(L"�ܹ���ѯ�� %d ����¼", m_nRecordCount);
//
//}

void COperationView::InitUserClassComboBox()
{
	m_cboUserClass.ResetContent();

	m_cboUserClass.AddString(LOAD_STRING(IDS_ALL));
	
	for (INT i = 0; i < CNetBarConfig::GetInstance()->m_UserClassArray.GetCount(); i++)
	{
		INT nIdx = m_cboUserClass.AddString(CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(i).GetClassName());
		m_cboUserClass.SetItemData(nIdx, CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(i).GetClassID());
	}

	m_cboUserClass.SetCurSel(0);
}

void COperationView::InitDetailData(CDetailDataMethod &DetailData)
{
	CString strTmp = m_StartTime.ToString();
	strTmp.Remove('-');
	strTmp.Remove(':');
	strTmp.Remove(' ');
	strTmp.Remove('/');
	strTmp.Append(_T("00"));
	DetailData.SetBeginTime(strTmp);

	strTmp = m_EndTime.ToString();
	strTmp.Remove('-');
	strTmp.Remove(':');
	strTmp.Remove(' ');
	strTmp.Remove('/');
	strTmp.Append(_T("59"));

	DetailData.SetEndTime(strTmp);
	DetailData.SetClassId(m_cboUserClass.GetItemData(m_cboUserClass.GetCurSel()));
	DetailData.SetPageNum(m_nCurrPageNo);
	DetailData.SetOperator(m_strOperator);
}

void COperationView::QueryCreditCenter(BOOL bReverse)
{
	CDetailDataMethod DetailData;

	InitDetailData(DetailData);

	//��������: 1���� 2��ֵ 3�˿� 4���� 5��ֵ�� 6�û�����

	if (bReverse)
	{
		DetailData.SetTranType(4);
	}
	else
	{
		DetailData.SetTranType(2);
	}

	theApp.GetCurCashier()->DoDetailData(DetailData);

	if (DetailData.GetStatusCode() != 0)
	{
		ShowToolTip(DetailData.GetStatusMessage(), TRUE);
		return;
	}

	//TRACE(DetailData.GetDetailInfo());

	m_nRecordCount = DetailData.GetRecordCount();

	CheckMoreRecord(DetailData);

	CStringArray strRecords;

	if (m_nRecordCount > 0)
	{
		CIBAHelpper::SplitLine(DetailData.GetDetailInfo().Mid(1), strRecords,  '|');
	}

	for (INT i = 1; i < strRecords.GetCount(); i++)
	{
		//creditDate,memberId,classId,creditAmount,creditPresent,operator

		//�����˺�|����|֤������|�û���|�û�����|��ֵ���|����|ʱ��|����Ա

		CStringArray strFileds;
		
		CString strRecord = strRecords.GetAt(i);
			
		CIBAHelpper::SplitLine(strRecord, strFileds,  ',');

		UINT nMemberId = _ttoi(strFileds.GetAt(1));

		CNetBarUser NetBarUser;

		CIBADAL::GetInstance()->GetNetBarUserInfo(nMemberId, NetBarUser);

		const INT nIdx = m_List.InsertItem(m_List.GetItemCount(), NetBarUser.GetNetId());

		m_List.SetItemText(nIdx, 1, NetBarUser.GetUserName());
		m_List.SetItemText(nIdx, 2, NetBarUser.GetCertificateID());
		m_List.SetItemText(nIdx, 3, strFileds.GetAt(1));

		CString strTmp;
		INT nTmp = _ttoi(strFileds.GetAt(2));
		StatUserCount(nTmp, strTmp);
		strTmp = CNetBarConfig::GetInstance()->GetUserClassName(nTmp);
		m_List.SetItemText(nIdx, 4, strTmp);

		nTmp = _ttoi(strFileds.GetAt(3));
		m_nAllMoney1 += nTmp;
		strTmp.Format(_T("%.2f"), nTmp / 100.0);
		m_List.SetItemText(nIdx, 5, strTmp);

		nTmp = _ttoi(strFileds.GetAt(4));
		m_nAllMoney2 += nTmp;

		if (nTmp > 0)
		{
			strTmp.Format(_T("%.2f"), nTmp / 100.0);
			m_List.SetItemText(nIdx, 6, strTmp);
		}

		m_List.SetItemText(nIdx, 7, strFileds.GetAt(0));
		m_List.SetItemText(nIdx, 8, strFileds.GetAt(5));
	}

	//L"�ܹ���ѯ�� %d ����¼����ֵ���:%.2f�����ͽ��:%.2f���ϼ�:%.2f"
	m_strResult.Format(LOAD_STRING(IDS_OP_VIEW_RESULT),
		m_nRecordCount, m_nAllMoney1 / 100.0, m_nAllMoney2 / 100.0, (m_nAllMoney1 + m_nAllMoney2) / 100.0);
}

void COperationView::QueryRegisterCenter()
{
	CDetailDataMethod DetailData;
	
	InitDetailData(DetailData);

	//��������: 1���� 2��ֵ 3�˿� 4���� 5��ֵ�� 6�û�����

	DetailData.SetTranType(1);

	theApp.GetCurCashier()->DoDetailData(DetailData);

	if (DetailData.GetStatusCode() != 0)
	{
		ShowToolTip(DetailData.GetStatusMessage(), TRUE);
		return;
	}

	m_nRecordCount = DetailData.GetRecordCount();

	CheckMoreRecord(DetailData);

	CStringArray strRecords;

	if (m_nRecordCount > 0)
	{
		CIBAHelpper::SplitLine(DetailData.GetDetailInfo().Mid(1), strRecords,  '|');
	}

	for (INT i = 1; i < strRecords.GetCount(); i++)
	{
		//operationDateTime,memberId,classId,deposit,creditAmount,creditPresent,operator

		//�����˺�|����|֤������|�û���|�û�����|�������|����|Ѻ��|ʱ��|����Ա

		CStringArray strFileds;

		CString strRecord = strRecords.GetAt(i);

		CIBAHelpper::SplitLine(strRecord, strFileds,  ',');

		UINT nMemberId = _ttoi(strFileds.GetAt(1));

		CNetBarUser NetBarUser;

		CIBADAL::GetInstance()->GetNetBarUserInfo(nMemberId, NetBarUser);

		const INT nIdx = m_List.InsertItem(m_List.GetItemCount(), NetBarUser.GetNetId());

		m_List.SetItemText(nIdx, 1, NetBarUser.GetUserName());
		m_List.SetItemText(nIdx, 2, NetBarUser.GetCertificateID());
		m_List.SetItemText(nIdx, 3, strFileds.GetAt(1));

		CString strTmp;
		INT nTmp = _ttoi(strFileds.GetAt(2));
		StatUserCount(nTmp, strTmp);
		strTmp = CNetBarConfig::GetInstance()->GetUserClassName(nTmp);
		m_List.SetItemText(nIdx, 4, strTmp);

		nTmp = _ttoi(strFileds.GetAt(4));
		m_nAllMoney1 += nTmp;
		strTmp.Format(_T("%.2f"), nTmp / 100.0);
		m_List.SetItemText(nIdx, 5, strTmp);

		nTmp = _ttoi(strFileds.GetAt(5));
		m_nAllMoney2 += nTmp;

		if (nTmp > 0)
		{
			strTmp.Format(_T("%.2f"), nTmp / 100.0);
			m_List.SetItemText(nIdx, 6, strTmp);
		}

		nTmp = _ttoi(strFileds.GetAt(3));
		m_nAllMoney3 += nTmp;

		if (nTmp > 0)
		{
			strTmp.Format(_T("%.2f"), nTmp / 100.0);
			m_List.SetItemText(nIdx, 7, strTmp);
		}

		m_List.SetItemText(nIdx, 8, strFileds.GetAt(0));
		m_List.SetItemText(nIdx, 9, strFileds.GetAt(6));
	}

	m_strResult.Format(L"�ܹ���ѯ�� %d ����¼���������:%.2f��Ѻ��:%.2f�����ͽ��:%.2f���ϼ�:%.2f",
		m_nRecordCount, m_nAllMoney1 / 100.0, m_nAllMoney3 / 100.0, m_nAllMoney2 / 100.0, 
		(m_nAllMoney1 + m_nAllMoney2 + m_nAllMoney3) / 100.0);
}

void COperationView::QueryReturnedCenter()
{
	CDetailDataMethod DetailData;

	InitDetailData(DetailData);

	//��������: 1���� 2��ֵ 3�˿� 4���� 5��ֵ�� 6�û�����

	DetailData.SetTranType(3);

	theApp.GetCurCashier()->DoDetailData(DetailData);

	if (DetailData.GetStatusCode() != 0)
	{
		ShowToolTip(DetailData.GetStatusMessage(), TRUE);
		return;
	}

	m_nRecordCount = DetailData.GetRecordCount();

	CheckMoreRecord(DetailData);

	CStringArray strRecords;

	if (m_nRecordCount > 0)
	{
		CIBAHelpper::SplitLine(DetailData.GetDetailInfo().Mid(1), strRecords,  '|');
	}

	for (INT i = 1; i < strRecords.GetCount(); i++)
	{
		//returnDate,memberId,classId,returnAmount,deposit,operator

		//�����˺�|����|֤������|�û���|�û�����|���˽��|Ѻ��|ʱ��|����Ա

		CStringArray strFileds;

		CString strRecord = strRecords.GetAt(i);

		CIBAHelpper::SplitLine(strRecord, strFileds,  ',');

		UINT nMemberId = _ttoi(strFileds.GetAt(1));

		CNetBarUser NetBarUser;

		CIBADAL::GetInstance()->GetNetBarUserInfo(nMemberId, NetBarUser);

		const INT nIdx = m_List.InsertItem(m_List.GetItemCount(), NetBarUser.GetNetId());

		m_List.SetItemText(nIdx, 1, NetBarUser.GetUserName());
		m_List.SetItemText(nIdx, 2, NetBarUser.GetCertificateID());
		m_List.SetItemText(nIdx, 3, strFileds.GetAt(1));

		CString strTmp;
		INT nTmp = _ttoi(strFileds.GetAt(2));
		StatUserCount(nTmp, strTmp);
		strTmp = CNetBarConfig::GetInstance()->GetUserClassName(nTmp);
		m_List.SetItemText(nIdx, 4, strTmp);

		nTmp = _ttoi(strFileds.GetAt(3));
		m_nAllMoney1 += nTmp;
		strTmp.Format(_T("%.2f"), nTmp / 100.0);
		m_List.SetItemText(nIdx, 5, strTmp);

		nTmp = _ttoi(strFileds.GetAt(4));
		m_nAllMoney3 += nTmp;

		if (nTmp > 0)
		{
			strTmp.Format(_T("%.2f"), nTmp / 100.0);
			m_List.SetItemText(nIdx, 6, strTmp);
		}

		m_List.SetItemText(nIdx, 7, strFileds.GetAt(0));
		m_List.SetItemText(nIdx, 8, strFileds.GetAt(5));
	}

	m_strResult.Format(L"�ܹ���ѯ�� %d ����¼�����˽��:%.2f������Ѻ��:%.2f",
		m_nRecordCount, m_nAllMoney1 / 100.0, m_nAllMoney3 / 100.0);
}

void COperationView::QueryCreditRevserCenter()
{
	QueryCreditCenter(TRUE);
}

void COperationView::QueryCardCreditCenter()
{
	CDetailDataMethod DetailData;

	InitDetailData(DetailData);

	//��������: 1���� 2��ֵ 3�˿� 4���� 5��ֵ�� 6�û�����

	DetailData.SetTranType(5);

	theApp.GetCurCashier()->DoDetailData(DetailData);

	if (DetailData.GetStatusCode() != 0)
	{
		ShowToolTip(DetailData.GetStatusMessage(), TRUE);
		return;
	}

	m_nRecordCount = DetailData.GetRecordCount();

	CheckMoreRecord(DetailData);

	CStringArray strRecords;

	if (m_nRecordCount > 0)
	{
		CIBAHelpper::SplitLine(DetailData.GetDetailInfo().Mid(1), strRecords,  '|');
	}

	for (INT i = 1; i < strRecords.GetCount(); i++)
	{
		//creditDate,memberId,classId,creditAmount,category,cardSerial,operator

		//�����˺�|����|֤������|�û���|�û�����|��ֵ�����|����|����|����Ա

		CStringArray strFileds;

		CString strRecord = strRecords.GetAt(i);

		CIBAHelpper::SplitLine(strRecord, strFileds,  ',');

		UINT nMemberId = _ttoi(strFileds.GetAt(1));

		CNetBarUser NetBarUser;

		CIBADAL::GetInstance()->GetNetBarUserInfo(nMemberId, NetBarUser);

		const INT nIdx = m_List.InsertItem(m_List.GetItemCount(), NetBarUser.GetNetId());

		m_List.SetItemText(nIdx, 1, NetBarUser.GetUserName());
		m_List.SetItemText(nIdx, 2, NetBarUser.GetCertificateID());
		m_List.SetItemText(nIdx, 3, strFileds.GetAt(1));

		CString strTmp;
		INT nTmp = _ttoi(strFileds.GetAt(2));
		StatUserCount(nTmp, strTmp);
		strTmp = CNetBarConfig::GetInstance()->GetUserClassName(nTmp);
		m_List.SetItemText(nIdx, 4, strTmp);

		nTmp = _ttoi(strFileds.GetAt(3));
		m_nAllMoney1 += nTmp;
		strTmp.Format(_T("%.2f"), nTmp / 100.0);
		m_List.SetItemText(nIdx, 5, strTmp);

		m_List.SetItemText(nIdx, 6, strFileds.GetAt(4).CompareNoCase(_T("XN")) == 0 ? LOAD_STRING(IDS_VISUAL_CARD) : LOAD_STRING(IDS_REAL_CARD));//category
		m_List.SetItemText(nIdx, 7, strFileds.GetAt(5));

		m_List.SetItemText(nIdx, 8, strFileds.GetAt(0));
		m_List.SetItemText(nIdx, 9, strFileds.GetAt(6));
	}

	m_strResult.Format(L"�ܹ���ѯ�� %d ����¼����ֵ�����:%.2f",
		m_nRecordCount, m_nAllMoney1 / 100.0);
}

void COperationView::QueryUpgradeCenter()
{
	CDetailDataMethod DetailData;

	InitDetailData(DetailData);

	//��������: 1���� 2��ֵ 3�˿� 4���� 5��ֵ�� 6�û�����

	DetailData.SetTranType(6);

	theApp.GetCurCashier()->DoDetailData(DetailData);

	if (DetailData.GetStatusCode() != 0)
	{
		ShowToolTip(DetailData.GetStatusMessage(), TRUE);
		return;
	}

	m_nRecordCount = DetailData.GetRecordCount();

	CheckMoreRecord(DetailData);

	CStringArray strRecords;

	if (m_nRecordCount > 0)
	{
		CIBAHelpper::SplitLine(DetailData.GetDetailInfo().Mid(1), strRecords,  '|');
	}

	for (INT i = 1; i < strRecords.GetCount(); i++)
	{
		//operationTime,memberId,oldClassId,classId,reason,operator

		//�����˺�|����|֤������|�û���|ԭ�û�����|���û�����|ʱ��|����Ա|��������

		CStringArray strFileds;

		CString strRecord = strRecords.GetAt(i);

		CIBAHelpper::SplitLine(strRecord, strFileds,  ',');

		UINT nMemberId = _ttoi(strFileds.GetAt(1));

		CNetBarUser NetBarUser;

		CIBADAL::GetInstance()->GetNetBarUserInfo(nMemberId, NetBarUser);

		const INT nIdx = m_List.InsertItem(m_List.GetItemCount(), NetBarUser.GetNetId());

		m_List.SetItemText(nIdx, 1, NetBarUser.GetUserName());
		m_List.SetItemText(nIdx, 2, NetBarUser.GetCertificateID());
		m_List.SetItemText(nIdx, 3, strFileds.GetAt(1));

		CString strTmp;
		INT nTmp = _ttoi(strFileds.GetAt(2));
		strTmp = CNetBarConfig::GetInstance()->GetUserClassName(nTmp);
		m_List.SetItemText(nIdx, 4, strTmp);

		nTmp = _ttoi(strFileds.GetAt(3));
		StatUserCount(nTmp, strTmp);
		strTmp = CNetBarConfig::GetInstance()->GetUserClassName(nTmp);
		m_List.SetItemText(nIdx, 5, strTmp);

		m_List.SetItemText(nIdx, 6, strFileds.GetAt(0));
		m_List.SetItemText(nIdx, 7, strFileds.GetAt(5));

		m_List.SetItemText(nIdx, 8, strFileds.GetAt(4));
	}

	m_strResult.Format(L"�ܹ���ѯ�� %d ����¼", m_nRecordCount);
}

void COperationView::OnBnClickedButtonNext()
{
	m_nCurrPageNo++;

	SetToolTipPos(IDC_BUTTON_NEXT);

	DoQuery();
}

void COperationView::OnBnClickedButtonPrev()
{
	m_nCurrPageNo--;

	SetToolTipPos(IDC_BUTTON_PREV);

	DoQuery();
}

void COperationView::CheckMoreRecord(CDetailDataMethod &DetailData)
{
	if (DetailData.GetPageSize() == DetailData.GetRecordCount())
	{
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
	}

	if (m_nCurrPageNo > 1)
	{
		GetDlgItem(IDC_BUTTON_PREV)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_PREV)->EnableWindow(FALSE);
	}
}

void COperationView::DoQuery()
{
	UpdateData();

	ClearData();

	switch (m_cboOperationType.GetCurSel())
	{
	case 0://����
		//QueryRegister();
		QueryRegisterCenter();
		break;

	case 1://��ֵ
		//QueryCredit();
		QueryCreditCenter();
		break;

	case 2://�˿�
		//QueryReturned();
		QueryReturnedCenter();
		break;

	case 3://��ֵ����
		//QueryCredit(TRUE);
		QueryCreditRevserCenter();
		break;

	case 4://��ֵ����ֵ
		//QueryCardCredit();
		QueryCardCreditCenter();
		break;

	case 5://�û�����
		//QueryUpgrade();
		QueryUpgradeCenter();
		break;

	}
	//�û��������
	for (INT i = 0; i < m_nUserCount.GetCount(); i++)
	{
		if (m_nUserCount.GetAt(i) > 0)
		{
			m_strResult.AppendFormat(_T("��%s:%d"), 
				CNetBarConfig::GetInstance()->m_UserClassArray.GetAt(i).GetClassName(),
				m_nUserCount.GetAt(i));
		}
	}

	if (m_nUnkonwUserClass > 0)
	{
		m_strResult.AppendFormat(_T("��δ֪:%d"), m_nUnkonwUserClass);
	}

	UpdateData(FALSE);
}