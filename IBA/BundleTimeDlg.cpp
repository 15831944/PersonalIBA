// BundleTimeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Cashier.h"
#include "IBAPrint.h"
#include "IBAHelpper.h"
#include "tts\IBATTS.h"
#include "Socket\LocalServer.h"
#include ".\bundletimedlg.h"
#include "ComputerInfo.h"
#include "selcomputerdlg.h"
#include "MainFrm.h"
#include "NDCreditDlg.h"
#include "ComputerListView.h"
#include "selfdefinebundtimedlg.h"
#include "HelpFunc.h"
#include "DlgChoseForwardBundle.h"
#include "Dal\DALUSER.h"
#include "Dal\IBADAL.h"
#include "CashierHelper.h"

//2015-0825 liyajun ��������б�ؼ�
int CALLBACK CSortListCtrl::ListCompare (LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CSortListCtrl* pSortList = (CSortListCtrl*)lParamSort;
	CString strText1 = pSortList->GetItemText(lParam1,pSortList->m_nSortedCol);
	CString strText2 = pSortList->GetItemText(lParam2,pSortList->m_nSortedCol);

	int nRet = 0;
	BOOL bAreDigits(FALSE);
	float fTemp1(0),fTemp2(0);

	//�ж��Ƿ�����
	if(strText1 == strText1.SpanIncluding(_T("0123456789.")) &&          //ֻ�������ֺ�С����
		strText1.Find(_T('.')) == strText1.ReverseFind(_T('.')) &&		 //ֻ����һ��С����
		strText1.Find(_T('.')) != 0 &&                                   //С���㲻����λ
		strText1.Find(_T('.')) != (strText1.GetLength() - 1) &&          //С���㲻��ĩβ
		strText2 == strText2.SpanIncluding(_T("0123456789.")) &&
		strText2.Find(_T('.')) == strText2.ReverseFind(_T('.')) &&
		strText2.Find(_T('.')) != 0 &&
		strText2.Find(_T('.')) != (strText2.GetLength() - 1)
		)
	{
		bAreDigits = TRUE;
		fTemp1 = _tstof(strText1);
		fTemp2 = _tstof(strText2);
	}

	//����Ѿ������򣬸�Ϊ��������
	if(pSortList->m_bAsc)
	{
		if(bAreDigits)
		{
			nRet = fTemp1 - fTemp2;
		}
		else
		{
			nRet = strText1.CompareNoCase(strText2);
		}
	}
	else  //�����������������������
	{
		if(bAreDigits)
		{
			nRet = fTemp2 - fTemp1;
		}
		else
		{
			nRet = (-1) * strText1.CompareNoCase(strText2);
		}		
	}

	return nRet;
}

IMPLEMENT_DYNAMIC(CDlgBundleTime, CNDDialog)
CDlgBundleTime::CDlgBundleTime(CWnd* pParent /*=NULL*/)
: CNDDialog(CDlgBundleTime::IDD, pParent)
, m_nSelBundleTimeId(0), m_nSelBundleTimePrice(0)
, m_bAfterBundleTime(FALSE)
, m_nLastMemberID(0), m_nCurrBundleTimeIndex(0)
{
	m_nBudleTimeMoney = 0;
}

CDlgBundleTime::~CDlgBundleTime()
{
}

void CDlgBundleTime::DoDataExchange(CDataExchange* pDX)
{
	CNDDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_BUNDLETIME, m_BundleTimeList);
	DDX_Control(pDX, IDC_CHECK_PRINT, m_btnPrint);
	DDX_Control(pDX, IDC_STATIC_ALL, m_btnAllBalance);
	DDX_Control(pDX, IDC_BUTTON_CREDIT, m_btnCredit);
	DDX_Control(pDX, IDC_BUTTON_BALANCE, m_btnBalance);

	DDX_Text(pDX, IDC_STATIC_USERNAME, m_strName);
	DDX_Text(pDX, IDC_STATIC_NETBAR, m_strOpenNetBar);
	DDX_Text(pDX, IDC_STATIC_PERSONALID, m_strPersonalID);
	DDX_Text(pDX, IDC_STATIC_BTAREA, m_strBundleTimeArea);
	DDX_Text(pDX, IDC_STATIC_BTENDTIME, m_strBundleTimeEndTime);
	DDX_Text(pDX, IDC_STATIC_BTID, m_strBundleTimeID);
	DDX_Text(pDX, IDC_STATIC_BTMONEY, m_strBundleTimeMoney);
	DDX_Text(pDX, IDC_STATIC_BTMONEY2, m_strBundleTimeMoney2);
	DDX_Text(pDX, IDC_STATIC_BTSTARTTIME, m_strBundleTimeStartTime);
	DDX_Text(pDX, IDC_STATIC_BTYPE, m_strBundleTimeType);
	DDX_Text(pDX, IDC_STATIC_CLASS, m_strUserClass);
	DDX_Text(pDX, IDC_STATIC_ALL, m_strAllBalance);
}


BEGIN_MESSAGE_MAP(CDlgBundleTime, CNDDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_LIST_BUNDLETIME, OnNMClickListBundletime)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_BUNDLETIME, OnColumnListClicked) //2015-0825 liyajun ��Ӧ���������Ϣ
	//ON_MESSAGE(WM_BUNDLETIMESORT, OnColumnListClicked)      
	ON_BN_CLICKED(IDC_BUTTON_CONTINUEBT, OnBnClickedButtonContinuebt)
	ON_BN_CLICKED(IDC_BUTTON_CANCELBT, OnBnClickedButtonCancelbt)
	ON_BN_CLICKED(IDC_BUTTON_FORWARDBT, OnBnClickedButtonForwardbt)
	ON_BN_CLICKED(IDC_BUTTON_CHANGEBT, OnBnClickedButtonChangebt)
	ON_BN_CLICKED(IDC_BUTTON_REPRINT, OnBnClickedButtonReprint)
	ON_BN_CLICKED(IDC_CHECK_PRINT, OnBnClickedCheckPrint)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SELFBUNDTIME, OnBnClickedSelfbundtime)
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_STATIC_ALL, OnStnClickedStaticAll)
	ON_BN_CLICKED(IDC_BUTTON_CREDIT, OnBnClickedButtonCredit)
	ON_BN_CLICKED(IDC_BUTTON_BALANCE, OnBnClickedButtonBalance)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, OnBnClickedButtonQuery)
END_MESSAGE_MAP()


// CDlgBundleTime ��Ϣ�������

//   (��ʱ-��ǰ��ʱ-ת��ʱ-ȡ����ʱ-�Զ����ʱ)

BOOL CDlgBundleTime::OnInitDialog()
{
	CNDDialog::OnInitDialog();

	m_btnAllBalance.SetFont(&m_Font);
	m_btnAllBalance.SetLinkColor(RGB(0, 0, 255));  // Ǯ����ɫ��ʾ
	m_btnAllBalance.SetVisitedColor(RGB(0, 0, 255));

	m_btnBalance.SizeToContent();
	m_btnCredit.SizeToContent();

	InitListView();
	SetEditFont();

	GetDlgItem(IDC_BUTTON_CANCELBT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_FORWARDBT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CHANGEBT)->EnableWindow(FALSE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);

	// 2015/4/22 tmelody
	// { ����ʱ
	GetDlgItem(IDC_BUTTON_CONTINUEBT)->EnableWindow(FALSE);
	// }
	
	GetDlgItem(IDC_EDIT_IDCARDSNO)->SetFont(&m_BigFont);
	GetDlgItem(IDC_EDIT_IDCARDSNO)->SetFocus();
	
	GetDlgItem(IDC_EDIT_CREDITMONEY)->SetFont(&m_BigFont);
	GetDlgItem(IDC_STATIC_CREDITMONEY)->SetFont(&m_BigFont);

	// m_nBundleTimeUseBalance == 0 �ǽ�ֹ�˻���ʱ
	// m_nBundleTimeUseBalance == 1 �������˻���ʱ
	// m_nBundleTimeUseBalance == 2 ��ֻ������ʱ�û���ʱ
	if (CNetBarConfig::GetInstance()->GetBundleTimeWithCash(m_bIsMember))
	{
		CIBAString strTmp;
		strTmp.LoadString(IDS_BTNEEDMONEY); 

		GetDlgItem(IDC_STATIC_CAPTION)->SetWindowText(strTmp); //Ӧ���ֽ�(Ԫ)��������Ӧ��

		m_btnCredit.EnableWindow(FALSE);
		m_btnCredit.ShowWindow(SW_HIDE);   //���س�ֵ��ť

		m_btnAllBalance.ShowWindow(SW_HIDE); //���� �˻��ϼƺ���Ľ����ʾ

		GetDlgItem(IDC_STATIC0)->ShowWindow(SW_HIDE); //���� �˻��ϼƣ�

		GetDlgItem(IDC_STATIC_CREDITMONEY)->ShowWindow(SW_HIDE);//���� ��ֵ������֣���Ӧ���˻�Ԫ������
	}
	else
	{
		m_btnBalance.ShowWindow(SW_HIDE); //���ز鿴��ť

		GetDlgItem(IDC_EDIT_CREDITMONEY)->ShowWindow(SW_HIDE);//���س�ֵ���༭��
	}

	GetDlgItem(IDC_BUTTON_REPRINT)->ShowWindow(FALSE);//�ظ���ӡ��ť����
	GetDlgItem(IDC_SELFBUNDTIME)->EnableWindow(FALSE);//�Զ����ʱ
	GetDlgItem(IDC_SELFBUNDTIME)->ShowWindow(CheckPopedom(_T("IBAP_CUSTOMBUNDLE")));//Ȩ���ж�

	//{ 2011/05/24-gxx: �Զ���ѯ��ˢ����ɨ��֤��������²���
	if (m_bAutoQuery)
	{
		// 2011/07/08-8201-gxx: �ж����Ϊ�����û��������ʱ
		if (GetUserState() == CIBAGlobal::emUserStateLocal)
		{
			// �˺Ŷ�Ӧ���û�Ϊ�����û�
			theApp.IBAMsgBox(IDS_DISABLEBT);
			OnCancel();
		}
		//
		SetTimer(TimeIdOfAutoQuery, 100, NULL);//ֻ��ѯһ��
	}
	else
	{
		CWnd* pWnd = GetDlgItem(IDC_EDIT_IDCARDSNO);
		ASSERT(pWnd);
		if (pWnd)
		{
			pWnd->SetFocus();
		}
	}
	//}

	return FALSE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CDlgBundleTime::CheckPopedom(LPCTSTR lpszModuleName)
{
	BOOL bRet = theApp.GetCurCashier()->IsOnDuty(); 

	if (bRet)
	{
		bRet = theApp.GetCurCashier()->HasPopedomOf(lpszModuleName);
	}

	return bRet;
}

void CDlgBundleTime::OnGetBalanceOK(CBalanceMethod & Balance)
{
	CNDDialog::OnGetBalanceOK(Balance);

	// 2012/02/20-8237-gxx: ��EnableWindow(TRUE)�Ķ�
	GetDlgItem(IDC_SELFBUNDTIME)->EnableWindow(CheckPopedom(_T("IBAP_CUSTOMBUNDLE")));//Ȩ���ж�
	
	if (m_bAfterBundleTime || Balance.GetPayType() > 0
		|| 0 != CNetBarConfig::GetInstance()->GetCannotSelfBundleTime())
	{
		// �Ѿ���ʱ or ��������Ϊ:�ۼƷⶥ�����Ԥ��

		GetDlgItem(IDC_SELFBUNDTIME)->EnableWindow(FALSE); // �����Զ����ʱ
	}

	if (m_nMemberId != m_nLastMemberID)
	{
		// ���¶�ȡ��ʱ�б�

		GetBundleTimeList();
		m_nLastMemberID = m_nMemberId;
	}	

	GetBundleTimeAction();

	//{ 2011/05/20-gxx: 
	if (!m_strBundleTimeEndTime.IsEmpty())
	{
		COleDateTime dtEndTime;

		dtEndTime.ParseDateTime(m_strBundleTimeEndTime);
		COleDateTimeSpan dtSpan = dtEndTime - COleDateTime::GetCurrentTime();

		//{ 2011/06/09-gxx: 
		int nBTLimit = CNetBarConfig::GetInstance()->GetBTLimitTime();
		if ( nBTLimit==0 )
		{
			nBTLimit = 50;
		}
		//}

		if (dtSpan.GetTotalMinutes() < nBTLimit) //�����Զ�������ʱ
		{
			// 2012/02/20-8237-gxx: ��EnableWindow(TRUE)�Ķ�
			GetDlgItem(IDC_SELFBUNDTIME)->EnableWindow(CheckPopedom(_T("IBAP_CUSTOMBUNDLE")));//Ȩ���ж�
		}
	}
	
	//}
	
	m_btnPrint.SetCheck( CIBAConfig::GetInstance()->GetPrintTicketBT(m_bIsMember));

	


	UpdateData(FALSE);
}

void CDlgBundleTime::GetBundleTimeList()
{
	m_BundleTimeList.DeleteAllItems();

	BundleTimeList.SetMemberId(m_nMemberId);

	theApp.GetCurCashier()->DoBundleTimeList(BundleTimeList);

	if (BundleTimeList.GetStatusCode() == 0)
	{
		for (INT i = 0; i < (INT)BundleTimeList.GetRecordCount(); i++)
		{
			CString strTmp;

			strTmp.Format(_T("%d"), BundleTimeList.GetBundleId(i));
			m_BundleTimeList.InsertItem(i, strTmp);

			strTmp = BundleTimeList.GetStartTime(i);
			m_BundleTimeList.SetItemText(i, 1, strTmp);

			strTmp = BundleTimeList.GetEndTime(i);
			m_BundleTimeList.SetItemText(i, 2, strTmp);

			strTmp.Format(_T("%d"), BundleTimeList.GetTimePass(i));
			m_BundleTimeList.SetItemText(i, 3, strTmp);

			strTmp.Format(_T("%.2f"), BundleTimeList.GetPrice(i) / 100.0);
			m_BundleTimeList.SetItemText(i, 4, strTmp);

			strTmp = BundleTimePayType(BundleTimeList.GetBundleType(i));
			m_BundleTimeList.SetItemText(i, 5, strTmp);

			strTmp = BundleTimeList.GetDescription(i);
			m_BundleTimeList.SetItemText(i, 6, strTmp);

			m_BundleTimeList.SetItemData(i, i);
		}
	}
}

void CDlgBundleTime::GetBundleTimeAction()
{
	CBundleTimeActionMethod BundleTimeAction;
	BundleTimeAction.SetMemberId(m_nMemberId);

	theApp.GetCurCashier()->DoBundleTimeAction(BundleTimeAction);
	
	if (BundleTimeAction.GetStatusCode() == 0 && 
		BundleTimeAction.GetNetBarId() > 0  // 0��ʾ�ް�ʱ
		)
	{
		m_strBundleTimeID.Format(_T("%d"), BundleTimeAction.GetTimeId());
		
		for (INT i = 0; i < (INT)BundleTimeList.GetRecordCount(); i++)
		{
			if (BundleTimeAction.GetTimeId() == BundleTimeList.GetBundleId(i))
			{
				m_nCurrBundleTimeIndex = i;
				break;
			}
		}
		
		m_strBundleTimeStartTime = CIBAHelpper::FormatCenterTime(BundleTimeAction.GetStartTime());
		m_strBundleTimeEndTime = CIBAHelpper::FormatCenterTime(BundleTimeAction.GetEndTime());
		m_strBundleTimeType = BundleTimePayType(BundleTimeAction.GetPayType());
		m_strBundleTimeMoney.Format(_T("%.2f"), BundleTimeAction.GetAmount1() / 100.0);
		m_strBundleTimeMoney2.Format(_T("%.2f"), BundleTimeAction.GetAmount2() / 100.0);
		m_nBudleTimeMoney = BundleTimeAction.GetAmount1();
		
		m_strBundleTimeArea = CNetBarConfig::GetInstance()->GetPCClassName(BundleTimeAction.GetPcClass());
		
		if (m_strBundleTimeArea.IsEmpty())
		{
			m_strBundleTimeArea = LOAD_STRING(IDS_NOLIMITAREA);
		}

		GetDlgItem(IDC_BUTTON_CANCELBT)->EnableWindow(TRUE); // ����ȡ����ʱ
	}
}	

CString CDlgBundleTime::BundleTimePayType(UINT nBundleType)
{	
	CIBAString strTmp;

	strTmp.LoadString(IDS_BUNDLETIMETYPE);

	if (1 == nBundleType)
	{
		strTmp = strTmp.Left(strTmp.Find('|'));
	}
	else
	{
		strTmp = strTmp.Mid(strTmp.Find('|') + 1);
	}

	return strTmp;
}


void CDlgBundleTime::InitListView()
{
	//L"���|��ʼʱ��|����ʱ��|ʱ��|���|��ʱ����|������Ϣ"
	
	CIBAString strHeader;
	strHeader.LoadString(IDS_BUNDLETIMELISTHEADER);

	CStringArray strArray;
	CIBAHelpper::SplitLine(strHeader, strArray);

	const UINT nWidth[] = {40, 70, 70, 40, 50, 70, 220};

	const UINT nFormat[] = {LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_RIGHT, LVCFMT_LEFT, LVCFMT_LEFT};

	for (INT i = 0; i < strArray.GetCount(); i++)
	{
		m_BundleTimeList.InsertColumn(i, strArray.GetAt(i), nFormat[i], nWidth[i]);
	}

	m_BundleTimeList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//2015-0825 liyajun ʹ�б�������Լ�����״̬
	//m_BundleTimeList.ModifyStyle(NULL,LVS_REPORT,0);
	//m_BundleTimeList.EnableMultipleSort(TRUE);
	//for(int i=0; i < strArray.GetCount(); ++i)
	//{
	//	m_BundleTimeList.SetSortColumn(i,TRUE,TRUE);
	//}
	
}

void CDlgBundleTime::CheckBundleTimeCondition(INT nBtIndex, BOOL& bNowCanBT, BOOL& bNowCanForwardBT, BOOL& bNowChangeBT, BOOL& bNowCanContinueBT)
{
	CString strNow(COleDateTime::GetCurrentTime().Format(_T("%H:%M:%S")));//��ǰʱ��
	CString strStart(BundleTimeList.GetStartTime(nBtIndex));//��ӦID��ʱ���ԵĿ�ʼʱ��
	CString strEnd(BundleTimeList.GetEndTime(nBtIndex));//��ӦID��ʱ���ԵĽ���ʱ��

	if (strStart.IsEmpty())//û�п�ʼʱ��� - ���԰�ʱ
	{
		bNowCanBT = TRUE;
	}
	else
	{
		if (strNow.Compare(strStart) >= 0) //now >= start
		{
			if (strNow.Compare(strEnd) < 0) //now < end�����԰�ʱ
			{
				bNowCanBT = TRUE;
			}
			else if (strNow.Compare(_T("23:59:59")) <= 0 && strStart.Compare(strEnd) > 0) //now<24:00:00 �� now>end�����԰�ʱ�������ǲ��Ƕ����ԣ�
			{	
				bNowCanBT = TRUE;
			}
		}
		else//now < start
		{
			if (strNow.Compare(strEnd) < 0 && strStart.Compare(strEnd) > 0)//����ſ����ڰ�ʱ��ʼǰ���а�ʱ
			{
				bNowCanBT = TRUE;
			}
		}
	}

	//û�а�ʱ�����ڲ������ʱ�ģ��������ǰ��ʱ
	bNowCanForwardBT = (!bNowCanBT && m_strBundleTimeID.IsEmpty());

	if (bNowCanBT) //���԰�ʱ
	{
		if (!m_strBundleTimeID.IsEmpty()) //�Ѿ���ʱ��
		{
			COleDateTime dtEndTime;
			dtEndTime.ParseDateTime(m_strBundleTimeEndTime);
			COleDateTimeSpan dtSpan = dtEndTime - COleDateTime::GetCurrentTime();//���ʱ����ʱ��

			//{ 2011/06/09-gxx: 
			int nBTLimit = CNetBarConfig::GetInstance()->GetBTLimitTime();//��ʱ������ʱ��,��"���ʱ����XXX����֮�ڲ������ʱ"
			if ( nBTLimit==0 )
			{
				nBTLimit = 50;
			}
			//}
			
			if (dtSpan.GetTotalMinutes() <= nBTLimit) //�뱾�ΰ�ʱ����ʱ��<=�趨��ֵ�������԰�ʱ
			{
				bNowCanBT = FALSE; 
			}
			else//�뱾�ΰ�ʱ����ʱ��>�趨��ֵ���ſ���ת��ʱ
			{
				INT nCurrBtId = _ttoi(m_strBundleTimeID);

				//�жϿɲ�����ת��ʱ
				if (m_nSelBundleTimeId != nCurrBtId  //�����ڵ�ǰ�İ�ʱID������ͬ�İ�ʱ
					&& !strStart.IsEmpty()           //��ʱ���Կ�ʼʱ�䲻Ϊ��
					&& strStart == BundleTimeList.GetStartTime(m_nCurrBundleTimeIndex)  //У���ʱ��ʼʱ��
					&& strEnd == BundleTimeList.GetEndTime(m_nCurrBundleTimeIndex)      //У���ʱ����ʱ��
					&& m_nSelBundleTimePrice >= BundleTimeList.GetPrice(m_nCurrBundleTimeIndex)  //У��۸� Ϊ��Ҫ>?
					) 
				{
					bNowChangeBT = TRUE;

					bNowCanBT = FALSE; //����ת��ʱʱ,��ʱ��ť���û�

					m_nSelBundleTimePrice -= BundleTimeList.GetPrice(m_nCurrBundleTimeIndex); //ת��ʱ�����
				}
			}


			// 2015-4-22 tmelody
			// { ��ǰ�Ѿ�����������ʱ�����Ƶ�ʱ�����ڼ�����
			if (strStart.IsEmpty() && strEnd.IsEmpty())
			{
				INT nCurrBtId = _ttoi(m_strBundleTimeID);
				if (BundleTimeList.GetBundleIdTime(nCurrBtId, strStart, strEnd))
				{
					if (strStart.IsEmpty() && strEnd.IsEmpty())
					{
						bNowCanBT = FALSE;
						bNowCanContinueBT = TRUE;
					}
				}
			}
			// }
		}
	}
}

void CDlgBundleTime::OnNMClickListBundletime(NMHDR *pNMHDR, LRESULT *pResult)
{
	//��ǰѡ����Ŀ
	INT nIndex = m_BundleTimeList.GetSelectionMark();

	if (nIndex >= 0)
	{
		INT nBtIndex(0);//��ʱ������BunTimeList�е�Index

		//��ȡ��ǰѡ��Item�ĵ�һ�i.e.BundleTime��Id.�ٴ�BundleTimeMethod�л�ȡ��Ӧ��Index��
		CString strIndex = m_BundleTimeList.GetItemText(nIndex,0);
		TCHAR* lpIndex = strIndex.GetBuffer();
		INT nItemData(0);//��BundleTimeID
		nItemData = _ttoi(lpIndex);
		strIndex.ReleaseBuffer();

		//�ҵ�BundleTimeList��BundletimeID��nItemDataƥ��Ķ���
		for(int i=0;i<BundleTimeList.GetRecordCount();++i) //��ȡBundleTimeList�ж�Ӧ��nBtIndex
		{
			if(BundleTimeList.GetBundleId(i) == nItemData)//��ȡArray�е�i��BundleTimeInfo��BundleId
			{
				nBtIndex = i;
				break;
			}
		}

		//����BundleTimeList��Index�����Ҷ�Ӧ���Ե���Ϣ
		m_nSelBundleTimeId = BundleTimeList.GetBundleId(nBtIndex);//��ȡID,��ʵ = nItemData
		m_nSelBundleTimePrice = BundleTimeList.GetPrice(nBtIndex);//��ȡPrice

		BOOL bNowCanBT = FALSE; 
		BOOL bNowCanForwardBT = FALSE;
		BOOL bNowChangeBT = FALSE;
		BOOL bNowCanContinueBT = FALSE;
		
		CheckBundleTimeCondition(nBtIndex, bNowCanBT, bNowCanForwardBT, bNowChangeBT, bNowCanContinueBT);

		GetDlgItem(IDOK)->EnableWindow(bNowCanBT);
		GetDlgItem(IDC_BUTTON_FORWARDBT)->EnableWindow(bNowCanForwardBT);
		GetDlgItem(IDC_BUTTON_CHANGEBT)->EnableWindow(bNowChangeBT);
		// 2015/4/22 tmelody
		GetDlgItem(IDC_BUTTON_CONTINUEBT)->EnableWindow(bNowCanContinueBT);

		CString strTmp;
		strTmp.Format(_T("%.2f"), m_nSelBundleTimePrice / 100.0);
		GetDlgItem(IDC_STATIC_CREDITMONEY)->SetWindowText(strTmp);
		GetDlgItem(IDC_EDIT_CREDITMONEY)->SetWindowText(strTmp);
	}

	*pResult = 0;
}

void CDlgBundleTime::OnBnClickedButtonCancelbt()
{
	CString strTmp;
	strTmp.Format(IDS_CANCELBUNDLETIME, m_strName);

	if (IDOK != MsgBox(strTmp, MB_OKCANCEL | MB_ICONQUESTION))
	{
		return;
	}

	CWaitCursor Wait;

	CCancelBundleTimeMethod CancelBundleTime;

	CancelBundleTime.SetMemberId(m_nMemberId);

	theApp.GetCurCashier()->DoCancelBundleTime(CancelBundleTime);

	CString strRet;

	if (CancelBundleTime.GetStatusCode() == 0)
	{
		OnBnClickedButtonQuery();

		strRet = LOAD_STRING(IDS_BTCANCELOK);

		if (CNetBarConfig::GetInstance()->GetBundleTimeWithCash(m_bIsMember))
		{
			strRet = LOAD_STRING(IDS_BTCANCELOK2);
		}

		m_bAfterBundleTime = FALSE;

		m_nBudleTimeMoney = 0;

		/*CActiveMember ActiveMember;
		if (CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(m_nMemberId, ActiveMember))
		{
			
			CLocalServer::GetInstance()->ActiveMemberList.CheckActiveMember(m_nCardId, m_nMemberId, ActiveMember);
		}*/
	}
	else
	{
		strRet = CancelBundleTime.GetStatusMessage();
	}

	SetToolTipPos(IDC_BUTTON_CANCELBT);
	ShowToolTip(strRet, CancelBundleTime.GetStatusCode() != 0);
}

void CDlgBundleTime::ClearData()
{
	m_strName.Empty();
	m_strUserClass.Empty();
	m_strPersonalID.Empty();
	m_strBundleTimeID.Empty();
	m_strBundleTimeStartTime.Empty();
	m_strBundleTimeEndTime.Empty();
	m_strBundleTimeType.Empty();
	m_strBundleTimeMoney.Empty();
	m_strBundleTimeMoney2.Empty();
	m_strBundleTimeArea.Empty();
	m_nSelBundleTimeId = 0;

	GetDlgItem(IDC_BUTTON_CHANGEBT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CANCELBT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_FORWARDBT)->EnableWindow(FALSE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(IDC_SELFBUNDTIME)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CONTINUEBT)->EnableWindow(FALSE);

	//��ѯ���������־
	m_bAfterBundleTime = FALSE;
	GetDlgItem(IDC_BUTTON_REPRINT)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_CREDITMONEY)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_CREDITMONEY)->SetWindowText(_T(""));
}

void CDlgBundleTime::OnBnClickedButtonForwardbt()
{
	//2015-0827 liyajun ������ǰ��ʱ����
	int nForwardBundleMoney = 0;
	CDlgChoseForwardBundle dlg;

	dlg.SetForwarBundleInfo(_tstof(m_strAllBalance), m_nUserClassID, m_nSelBundleTimeId, m_bIsMember);

	if(IDOK != dlg.DoModal())
	{
		return ;
	}
	else
	{
		nForwardBundleMoney = dlg.GetForwardBundleMoney();

		// ���WK������"��ֹ�˻���ʱ"����ôҪ��ֵ��ǮӦ���ǣ�Ӧ���ֽ� - ��ʱ������
		// ��Ϊ��ֹ�˻���ʱʱ�������ֵ��Ӧ���ֽ𣬺��滹���ٳ��ʱ������
		// Ϊʲô��ֱ�ӽ�����ĳ�ֵȥ������Ϊ�������Բ��ı������߼�
		if (CNetBarConfig::GetInstance()->GetBundleTimeWithCash(m_bIsMember))
		{
			nForwardBundleMoney -= HELPFUNC::GetBundleTimeMoney(m_nUserClassID, m_nSelBundleTimeId);
		}
			// ѯ���Ƿ�Ҫ��ʱ
	//----------------------------------------------------------------------------------------------//

		CString strTmp;

		strTmp.Format(IDS_ASKBUNDLETIME);

		if (IDOK != MsgBox(strTmp, MB_OKCANCEL | MB_ICONQUESTION))
		{
			return;

	//2015-0827 liyajun ������ǰ��ʱ����
		}
		else
		{
			if(nForwardBundleMoney > 0)
			{
				m_strMoneyCredit.Format(_T("%d"), nForwardBundleMoney / 100);
				if(!DoCredit())
				{
					if(IDYES != AfxMessageBox(_T("��ֵʧ��!�Ƿ������"), MB_YESNO))
					{
						return;
					}
				}
			}
		}
	}
	//---------------------------------------------------------------------------------------------//

	if (!m_strBundleTimeID.IsEmpty())
	{
		CCancelBundleTimeMethod CancelBundleTime;
		CancelBundleTime.SetMemberId(m_nMemberId);
		theApp.GetCurCashier()->DoCancelBundleTime(CancelBundleTime);
	}

	GetDlgItem(IDC_BUTTON_FORWARDBT)->EnableWindow(FALSE);

	if (!DoBundleTime(1)) 
	{
		GetDlgItem(IDC_BUTTON_FORWARDBT)->EnableWindow(TRUE);
	}
	else//��ǰ��ʱ�ɹ��ˣ����ʱ����ǰ��ʱ��ť�û�
	{
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_FORWARDBT)->EnableWindow(FALSE);
	}


	//GetDlgItem(IDC_BUTTON_FORWARDBT)->EnableWindow(TRUE);
}

void CDlgBundleTime::OnBnClickedButtonChangebt()
{
	CString strTmp;

	strTmp.Format(IDS_ASKBUNDLETIME);

	if (IDOK != MsgBox(strTmp, MB_OKCANCEL | MB_ICONQUESTION))
	{
		return;
	}

	GetDlgItem(IDC_BUTTON_CHANGEBT)->EnableWindow(FALSE);

	if (!DoBundleTime(2)) //ת��ʱʧ��
	{
		GetDlgItem(IDC_BUTTON_CHANGEBT)->EnableWindow(TRUE);
	}
	else//ת��ʱ�ɹ������ʱ��ת��ʱ��ť�û�
	{
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	}

	GetDlgItem(IDC_BUTTON_CHANGEBT)->EnableWindow(TRUE);
}

// �����ʱ
void CDlgBundleTime::OnBnClickedOk()
{
	CString strTmp;

	if (m_strBundleTimeID.IsEmpty())
	{
		strTmp.Format(IDS_ASKBUNDLETIME);
	}
	else
	{
		strTmp.Format(IDS_ASKBUNDLETIMEAGEIN);
	}

	if (IDOK != MsgBox(strTmp, MB_OKCANCEL | MB_ICONQUESTION))
	{
		return;
	}

	if (!m_strBundleTimeID.IsEmpty())
	{
		CCancelBundleTimeMethod CancelBundleTime;
		CancelBundleTime.SetMemberId(m_nMemberId);
		theApp.GetCurCashier()->DoCancelBundleTime(CancelBundleTime);
	}

	GetDlgItem(IDOK)->EnableWindow(FALSE);

	if (!DoBundleTime(0)) //��ʱʧ��
	{
		GetDlgItem(IDOK)->EnableWindow(TRUE);
	}
	else//��ʱ�ɹ��󣬰�ʱ����ǰ��ʱ��ť�û�
	{
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_FORWARDBT)->EnableWindow(FALSE);
	}
	
}

// 2015-4-22 tmelody
// { �������ʱ
void CDlgBundleTime::OnBnClickedButtonContinuebt()
{
	CString strMsg;
	strMsg.Format(IDS_ASKBUNDLETIMEAGEIN);

	if (IDOK != MsgBox(strMsg, MB_OKCANCEL | MB_ICONQUESTION))
	{
		return;
	}

	GetDlgItem(IDC_BUTTON_CONTINUEBT)->EnableWindow(FALSE);

	if (!DoBundleTime(4))
	{
		// ʧ��
		GetDlgItem(IDC_BUTTON_CONTINUEBT)->EnableWindow(TRUE);
	}
	else//����ʱ�ɹ��������ٰ�ʱ����ǰ��ʱ�Լ�������ʱ
	{
		GetDlgItem(IDC_BUTTON_CONTINUEBT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_FORWARDBT)->EnableWindow(FALSE);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	}
}
// }

// 0:��ͨ��ʱ   
// 1:Ԥ��ʱ
// 2:ת��ʱ
// 3:�Զ����ʱ
// 4:����ʱ
BOOL CDlgBundleTime::DoBundleTime(UINT nType)
{
	CWaitCursor Wait;

	CBundleTimeMethod BundleTime;

	BundleTime.SetMemberId(m_nMemberId);
	BundleTime.SetReserve(nType);
	BundleTime.SetTimeId(m_nSelBundleTimeId);

	if (CNetBarConfig::GetInstance()->GetBundleTimeWithCash(m_bIsMember))
	{
		BundleTime.SetCreditAmount(m_nSelBundleTimePrice);
	}

	theApp.GetCurCashier()->DoBundleTime(BundleTime);

	if (BundleTime.GetStatusCode() >= 0 && BundleTime.GetNeedRelogin())
	{
		//���߰�ʱ,��Ҫ֪ͨ�ͻ������µ�¼

		CActiveMember ActiveMember;

		if (CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(m_nMemberId, ActiveMember)) 
		{
			CActiveMemberList::TReLoginRequest MyReLoginRequest = {0};
			MyReLoginRequest.lastRefNo = ActiveMember.GetRefNo();
			MyReLoginRequest.memberId = m_nMemberId;
			MyReLoginRequest.firstTryTime = GetTickCount();

			CLocalServer::GetInstance()->ActiveMemberList.AddReLoginRequest(MyReLoginRequest);
			CLocalServer::GetInstance()->LocalBundleTime(m_nMemberId);
		}
	}

	CString strRet;

	if (BundleTime.GetStatusCode() == 0)
	{
		// 2011/07/29-8201-gxx: �����ֽ��ʱʱ��Ԥ�������ϸ��ˢ��
		if (CNetBarConfig::GetInstance()->GetBundleTimeWithCash(m_bIsMember))
		{
			CLocalServer::GetInstance()->ActiveMemberList.UpdateActiveMember(m_nMemberId, m_nSelBundleTimePrice, 0, 0);
		}

		if (nType == 2)
		{
			CIBATTS::GetInstance()->SpeakBundleTimePatch(BundleTime.GetAmount());
		}
		else
		{
			CIBATTS::GetInstance()->SpeakBundleTimeResult(BundleTime.GetAmount());
		}
		

		ReQuery();

		strRet = LOAD_STRING(IDS_BTSUCCESSED);

		if (m_btnPrint.GetCheck() == BST_CHECKED)
		{
			PrintTicket();
		}

		m_bAfterBundleTime = TRUE;//��ʱ�ɹ���־��λ
		OnBnClickedCheckPrint();//�ظ���ӡ��ť��ʾ

		COperationInfo OperationInfo;
		OperationInfo.SetOperationType(7);
		OperationInfo.SetUserName(m_strName);
		OperationInfo.SetNetId(m_strNetId);
		OperationInfo.SetOperationAmount(BundleTime.GetAmount());
		OperationInfo.SetDescription(
			CIBAHelpper::CenterTimeToOleDateTime(BundleTime.GetStartTime()).Format(_T("%H:%M"))
			+ _T(" - ") +  
			CIBAHelpper::CenterTimeToOleDateTime(BundleTime.GetEndTime()).Format(_T("%H:%M")));

		::SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM)(&OperationInfo));

		/*UINT nAmount = BundleTime.GetAmount();
		CActiveMember ActiveMember;
		if (CLocalServer::GetInstance()->ActiveMemberList.GetActiveMember(m_nMemberId, ActiveMember)) 
		{
			nAmount = ActiveMember.GetAmount() + nAmount;
			ActiveMember.SetAmount( nAmount );
			CLocalServer::GetInstance()->ActiveMemberList.UpdateActiveMember(ActiveMember,TRUE);
		}*/
	}
	else
	{
		m_bAfterBundleTime = FALSE;
		strRet = BundleTime.GetStatusMessage();
	}

	if (nType == 0)
	{
		SetToolTipPos(IDOK);
	}
	else if (nType == 1)
	{
		SetToolTipPos(IDC_BUTTON_FORWARDBT);
	}
	else if (nType == 2)
	{
		SetToolTipPos(IDC_BUTTON_CHANGEBT);
	}
	else if (nType == 4)
	{
		SetToolTipPos(IDC_BUTTON_CONTINUEBT);
	}

	ShowToolTip(strRet, !m_bAfterBundleTime);
	
	if (BundleTime.GetStatusCode() > 0 && BundleTime.GetNeedRelogin())
	{
		CIBAHelpper::WaitFor(3000); //�ȴ�3��
	}

	GetDlgItem(IDC_EDIT_IDCARDSNO)->SetFocus();

	return m_bAfterBundleTime;
}

void CDlgBundleTime::PrintTicket()
{	
	CString strTicket;

	CIBAPrint::GetBundleTimeTicket(strTicket);

	strTicket.Replace(_T("[name]"), m_strName);
	strTicket.Replace(_T("[idnumber]"), m_strPersonalID);
	strTicket.Replace(_T("[memberclass]"), m_strUserClass);
	strTicket.Replace(_T("[cardserialnumber]"), m_strNetId);
	strTicket.Replace(_T("[netid]"), m_strNetId.Right(8));
	strTicket.Replace(_T("[pcclass]"), m_strBundleTimeArea);
	strTicket.Replace(_T("[money]"), m_strBundleTimeMoney);
	strTicket.Replace(_T("[starttime]"), m_strBundleTimeStartTime.Mid(11, 5));
	strTicket.Replace(_T("[endtime]"), m_strBundleTimeEndTime.Mid(11, 5));
	strTicket.Replace(_T("[totalmoney]"), m_strAllBalance);
	
	CIBAPrint::Print(strTicket);
}


void CDlgBundleTime::OnBnClickedButtonReprint()
{
	PrintTicket();
}

void CDlgBundleTime::OnBnClickedCheckPrint()
{
	UpdateData(TRUE);

	if (m_btnPrint.GetCheck() == BST_CHECKED && m_bAfterBundleTime)
	{
		GetDlgItem(IDC_BUTTON_REPRINT)->ShowWindow(SW_NORMAL);
	} 
	else
	{
		GetDlgItem(IDC_BUTTON_REPRINT)->ShowWindow(SW_HIDE);
	}
}

void CDlgBundleTime::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == TimeOfOnlineBundTime)
	{	
		m_bAutoQuery = FALSE;

		KillTimer(TimeOfOnlineBundTime);
	}
	else if (nIDEvent == TimeIdOfAutoQuery)
	{
		KillTimer(TimeIdOfAutoQuery);//ֻ��ѯһ��
		OnBnClickedButtonQuery();
	}

	CNDDialog::OnTimer(nIDEvent);
}

void CDlgBundleTime::OnBnClickedSelfbundtime()
{
	CDlgBundTimeSelfDef SelfDefineBundTimeDlg;
	
	SelfDefineBundTimeDlg.SetMemberId(m_nMemberId);
	SelfDefineBundTimeDlg.SetIsMember(m_bIsMember);

	if (SelfDefineBundTimeDlg.DoModal() == IDOK)
	{
		ReQuery();
	}
}

HBRUSH CDlgBundleTime::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CNDDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (CNetBarConfig::GetInstance()->GetBundleTimeWithCash(m_bIsMember))
	{
		if (pWnd->GetDlgCtrlID() == IDC_EDIT_CREDITMONEY)
		{
			pDC->SetTextColor(RGB(255, 0, 0));		
		}
	}
	else
	{
		if (pWnd->GetDlgCtrlID() == IDC_STATIC_CREDITMONEY)
		{
			pDC->SetTextColor(RGB(0, 0, 255));		
		}
	}

	return hbr;
}

void CDlgBundleTime::OnStnClickedStaticAll()
{
	if (m_nMemberId > 0)
	{
		CString strTmp;
		
		strTmp.Format(IDS_DETAILBALANCE, 
			m_nMoneyBalanceW / 100.0, m_nMoneyBalanceY / 100.0, 
			m_nMoneyBalanceZ / 100.0, m_nMoneyOnlineAmount / 100.0,
			(m_nMoneyBalanceW + m_nMoneyBalanceY  + m_nMoneyBalanceZ + m_nMoneyOnlineAmount) / 100.0);


		SetToolTipPos(IDC_STATIC_ALL);

		ShowToolTip(strTmp);
	}
}

void CDlgBundleTime::OnBnClickedButtonCredit()
{
	if (0 == m_nMemberId) //û���û�
	{
		return;
	}


	CNDCreditDlg dlg;
	
	dlg.SetLinkEnabled(FALSE);
	dlg.SetNetId(m_strNetId);

	if (IDOK == dlg.DoModal())
	{
		ReQuery();

		m_BundleTimeList.SetFocus();
	}
}

BOOL CDlgBundleTime::PreTranslateMessage(MSG* pMsg)
{
	//if (WM_KEYDOWN == pMsg->message)
	//{
	//	if (VK_F3 == pMsg->wParam && m_btnCredit.IsWindowVisible())
	//	{
	//		OnBnClickedButtonCredit();
	//		return TRUE;
	//	}	
	//}

	//if (LVN_COLUMNCLICK == pMsg->message)
	//{
	//	OnColumnListClicked((NMHDR*)pMsg->lParam, NULL);
	//}
	return CNDDialog::PreTranslateMessage(pMsg);
}

void CDlgBundleTime::OnBnClickedButtonBalance()
{
	if (m_nMemberId > 0)
	{
		CString strTmp;

		strTmp.Format(IDS_DETAILBALANCE, 
			m_nMoneyBalanceW / 100.0, m_nMoneyBalanceY / 100.0, 
			m_nMoneyBalanceZ / 100.0, m_nMoneyOnlineAmount / 100.0,
			(m_nMoneyBalanceW + m_nMoneyBalanceY  + m_nMoneyBalanceZ + m_nMoneyOnlineAmount) / 100.0);


		SetToolTipPos(IDC_BUTTON_BALANCE);

		ShowToolTip(strTmp);
	}
}

//2015-0723 liyajun ����ʱ����-��Ա��Ӧ���˻�(Ԫ)�����ǻ�Ա��Ӧ���˻�(Ԫ)����
void CDlgBundleTime::OnBnClickedButtonQuery()
{
	// 2016-3-17 - qsc
	CNDDialog::OnBnClickedButtonQuery();
	BOOL bBundleTimeWithCash = CNetBarConfig::GetInstance()->GetBundleTimeWithCash(m_bIsMember);

	// ��ֹ�˻���ʱ
	if(bBundleTimeWithCash)
	{
		CString sShow(LOAD_STRING(IDS_BTNEEDMONEY));
		GetDlgItem(IDC_STATIC_CAPTION)->SetWindowText(sShow);
	}
	else
	{
		CString sShow(LOAD_STRING(IDS_BUNDLETIME_DEDUCTION));
		GetDlgItem(IDC_STATIC_CAPTION)->SetWindowText(sShow);
	}
}

//2015-0825 liyajun ��Ӧ�б������
void CDlgBundleTime::OnColumnListClicked(NMHDR* pNMhdr, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMhdr;
	//�����ǰѡ����Ϊ֮ǰѡ�е��У�˵������Ҫ�������򣬽�˳��ߵ�������Ͱ��������С�
	if(pNMListView->iSubItem == m_BundleTimeList.m_nSortedCol)
	{
		m_BundleTimeList.m_bAsc = !m_BundleTimeList.m_bAsc;
	}
	else
	{
		m_BundleTimeList.m_bAsc = TRUE;
		m_BundleTimeList.m_nSortedCol = pNMListView->iSubItem;
	}

	if(!m_BundleTimeList.GetItemCount()) return;  //�б���û��Ԫ�أ�ֱ�ӷ���

	//����ǰ����Itemû��Data,Ҫ����һ��ItemData
	for(int i=0 ; i<m_BundleTimeList.GetItemCount();++i)
	{
		m_BundleTimeList.SetItemData(i,i);
	}

	m_BundleTimeList.SortItems((PFNLVCOMPARE)CSortListCtrl::ListCompare,(DWORD_PTR)&m_BundleTimeList); 

	//������֮������������ItemData
	for(int i=0 ; i<m_BundleTimeList.GetItemCount();++i)
	{
		m_BundleTimeList.SetItemData(i,i);
	}

}

//2015-0827 liyajun ������ǰ��ʱ����
BOOL CDlgBundleTime::DoCredit()
{
	CWaitCursor WaitCursor;

	if (!CheckInput()) return FALSE;//�����Ǯ����

	UINT nMoney = _tstof(m_strMoneyCredit) * 100;

	// ��ѯ�� if (!MakeSureCredit(nMoney)) return FALSE;//ѯ���Ƿ�Ҫ��ֵ

	//if (m_bNDHasLocalQuery && m_LocalActiveMember.GetIsLocalUser())
	//{
	//	return DoLocalModeCredit(nMoney);
	//}

	///////////////////////�˺ų�ֵ/////////////////////
	CCreditMethod Credit;

	Credit.SetMemberId(m_nMemberId);
	Credit.SetTermTime(CIBAHelpper::GetCurrentTimeAsString());
	Credit.SetActivityId(0);
	Credit.SetNoPresent(TRUE);	// Ĭ��Ϊ������
	//Credit.SetNoPresent(m_bNoPresent);
	Credit.SetAmount(nMoney);

	theApp.GetCurCashier()->DoCredit(Credit);
	///////////////////////��ֵ�ɹ������ø���״̬///////////////////////
	if (Credit.GetStatusCode() == 0)
	{	
		//������ǰ�ĳ�ֵ���
		//m_nCreditMoney = _tstof(m_strCreditMoney) * 100.0;

		//���ͽ��
		/*if (m_bNoPresent)
		{
			m_nPresentMoney = 0;
		} 
		else
		{
			m_nPresentMoney = Credit.GetCreditPresent();
		} */   

		if (m_bOnLine) //���ͳ�ֵ��Ϣ
		{			
			CString strTmp;
			strTmp.Format(IDS_ONLINECREDIT, nMoney / 100.0, Credit.GetCreditPresent() / 100.0);
			CLocalServer::GetInstance()->LocalSendShortMessageByMemberID(m_nMemberId, strTmp);
		}

		{//д����ֵ��Ϣ��־��¼��
			COperationInfo OperationInfo;
			OperationInfo.SetOperationType(1);
			OperationInfo.SetIsBundTime(FALSE);
			OperationInfo.SetUserName(m_strName);
			OperationInfo.SetNetId(m_strNetId);
			OperationInfo.SetMemberId(m_nMemberId);
			OperationInfo.SetOperationAmount(nMoney);
			//OperationInfo.SetDeposit(_ttoi(m_strDeposit) * 100);
			// OperationInfo.SetPassWord("");
			OperationInfo.SetPresentAmount(Credit.GetCreditPresent());
			::SendMessage(CMainFrame::m_hOperationLog, WM_OPERATIONLOG, 0, (LPARAM)(&OperationInfo));;
		}

		{//д�����ݿ�

			CCreditInfoDB CreditInfo;

			CreditInfo.SetCardSerial(m_strNetId);
			CreditInfo.SetCreditDate(CIBAHelpper::FormatCenterTime(Credit.GetTrantime()));
			CreditInfo.SetRefNo(Credit.GetRefNo());
			CreditInfo.SetCreditPresent(Credit.GetCreditPresent());
			CreditInfo.SetCreditAmount(nMoney);
			CreditInfo.SetOperator(theApp.GetCurCashier()->GetName());
			CreditInfo.SetNetBarId(CNetBarConfig::GetInstance()->GetNetBarId());
			CreditInfo.SetCashRegisterID(CIBAConfig::GetInstance()->GetCashRegisterID());
			CreditInfo.SetMemberId(m_nMemberId);
			CreditInfo.SetSerialNo(Credit.GetLocalSerialNo());
			CreditInfo.SetCassId(m_nUserClassID);

			CIBADAL::GetInstance()->AddCreditRecord(CreditInfo);
		}

		CString strResult;
		strResult.Format(IDS_CREDITRESULT, nMoney / 100.0, Credit.GetCreditPresent() / 100.0);
		SetToolTipPos(IDC_EDIT_CREDIT);
		ShowToolTip(strResult);

		////��ֵ�ɹ����ѯ����
		//if (m_bAlreadyBundTime || m_nBundleTimeId == 0 || m_bOnLine)//�Ѿ���ʱ��(û�а�ʱ��ûѡ���ʱ��Ϣ��
		//{
		//	//����
		//	CIBATTS::GetInstance()->SpeakCreditResult(nMoney, Credit.GetCreditPresent(), Credit.GetAvailableBalance());

		//	CheckCard(m_strNetId);

		//}

		return TRUE;
	}
	else
	{
		SetToolTipPos(IDOK);
		ShowToolTip(Credit.GetStatusMessage(), TRUE);

		return FALSE;
	}
}

BOOL CDlgBundleTime::CheckInput()
{
	//��֤��Ǯ����
	UpdateData();

	INT nMoney = _tstof(m_strMoneyCredit) * 100;

	if (nMoney <= 0)
	{
		//SetToolTipPos(IDC_EDIT_MONEYTOFORWORDBUNDLE);
		//ShowToolTip(IDS_INPUTERROR);
		//GetDlgItem(IDC_EDIT_MONEYTOFORWORDBUNDLE)->SetFocus();
		return FALSE;
	}

	//// 2012/03/05-8238-gxx: 
	//if (CIBAConfig::GetInstance()->GetMaxCreditMoney() != 0)
	//{
	//	INT nMax = CIBAConfig::GetInstance()->GetMaxCreditMoney()*100;
	//	if (nMoney > nMax)
	//	{
	//		ShowToolTip(IDS_OVERFLOW_MAXMONEY, IDC_EDIT_MONEYTOBUNDLE, TRUE);
	//		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MONEYTOBUNDLE);
	//		if (pEdit)
	//		{
	//			pEdit->SetFocus();
	//			pEdit->SetSel(0, m_strMoneyCredit.GetLength());
	//		}

	//		return FALSE;
	//	}
	//}
	//// --end

	return TRUE;
}

BOOL CDlgBundleTime::DoLocalModeCredit(UINT nCreditMoney)
{
	CString strErrMsg;
	strErrMsg = CCashierHelper::DoCredit_InLocal(m_ActiveMember, nCreditMoney);
	BOOL bOK = strErrMsg.IsEmpty()?TRUE:FALSE;

	if (bOK)
	{
		//�ظ���ӡ��ť����
		/*if (m_bPrint)
		{
			GetDlgItem(IDC_BUTTON_REPRINT)->ShowWindow(SW_NORMAL);
		}*/  
		
		CString strResult;
		strResult.Format(IDS_CREDITRESULT, nCreditMoney / 100.0, 0 / 100.0);
		SetToolTipPos(IDC_EDIT_CREDIT);
		ShowToolTip(strResult);

		////��ӡСƱ
		//if (m_bPrint)
		//{
		//	CString strTmp;
		//	strTmp.Format(_T("%.2f"), nCreditMoney / 100.0);
		//	PrintTicket(strTmp);
		//}

		//��ճ�ֵ�ı༭��
		m_strMoneyCredit.Empty();
		/*m_strCreditMoney.Empty();
		m_edtCreditMoney.SetWindowText(_T(""));
		m_edtCreditMoney.SetFocus();*/
		return TRUE;
	}
	else
	{
		SetToolTipPos(IDOK);
		ShowToolTip(LOAD_STRING(IDS_LOCAL_CREDIT_FAULT), TRUE);  // ���س�ֵʧ��

		return FALSE;
	}

}
