// BundleTimeListDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "Cashier.h"
#include "IBAHelpper.h"
#include ".\bundletimelistdlg.h"

#define NetbarBundleTime CNetBarConfig::GetInstance()->m_NetbarBundleTime.m_BundleTimeInfoArray

// CDlgBundleTimeList �Ի���

IMPLEMENT_DYNAMIC(CDlgBundleTimeList, CIBADialog)
CDlgBundleTimeList::CDlgBundleTimeList(CWnd* pParent /*=NULL*/)
: CIBADialog(CDlgBundleTimeList::IDD, pParent)
, m_nBundleTimeId(0), m_nBundleTimeClassId(0), m_nBundleTimeType(0)
, m_nSelectStyle(0), m_nUserClassId(0), m_nBundleTimeMoney(0)
{
	m_strRemainTime = _T("00:00:00");
}

CDlgBundleTimeList::~CDlgBundleTimeList()
{
}

void CDlgBundleTimeList::DoDataExchange(CDataExchange* pDX)
{
	CIBADialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_BUNDLETIME, m_BundleTimeList);
}


BEGIN_MESSAGE_MAP(CDlgBundleTimeList, CIBADialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_BUNDLETIME, OnNMDblclkListBundletime)
END_MESSAGE_MAP()


// CDlgBundleTimeList ��Ϣ�������

BOOL CDlgBundleTimeList::OnInitDialog()
{
	CIBADialog::OnInitDialog();

	InitListView();

	FillBundleTimeList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgBundleTimeList::FillBundleTimeList()
{
	m_BundleTimeList.DeleteAllItems();

	for (INT i = 0; i < NetbarBundleTime.GetCount(); i++)
	{
		if (NetbarBundleTime.GetAt(i).GetClassId() != m_nUserClassId)
		{
			if (m_BundleTimeList.GetItemCount() == 0)
			{
				continue;
			}
			else
			{
				break;
			}
		}

		if (0 == m_nSelectStyle)
		{
			InsertItem(i, m_BundleTimeList.GetItemCount());//�����б���
		}
		else
		{
			if (NetbarBundleTime.GetAt(i).IsNowEnabled())
			{
				InsertItem(i, m_BundleTimeList.GetItemCount());//�����б���
			}
		}
	}
}

void CDlgBundleTimeList::InitListView()
{
	CIBAString strHeader;
	strHeader.LoadString(IDS_BUNDLETIMELISTHEADER);

	//strHeader = L"���|��ʼʱ��|����ʱ��|ʱ��|���|��ʱ����|������Ϣ";

	CStringArray strArray;
	CIBAHelpper::SplitLine(strHeader, strArray);

	const UINT Width[] = {40, 70, 70, 40, 45, 70, 200};

	for (INT i = 0; i < strArray.GetCount(); i++)
	{
		m_BundleTimeList.InsertColumn(i, strArray.GetAt(i), LVCFMT_LEFT, Width[i]);
	}

	m_BundleTimeList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}

void CDlgBundleTimeList::OnOK()
{
	INT nIndex = m_BundleTimeList.GetSelectionMark();

	if (nIndex >= 0)
	{	
		//ȡ�ð�����
		INT nBtIndex = m_BundleTimeList.GetItemData(nIndex);
		
		m_nBundleTimeId = NetbarBundleTime.GetAt(nBtIndex).GetBundleTimeId();

		m_nBundleTimeClassId = NetbarBundleTime.GetAt(nBtIndex).IsNowEnabled() ? 0 : 1;

		//��ʼʱ��
		m_strStartTime = NetbarBundleTime.GetAt(nBtIndex).GetStartTime(TRUE).Left(5);
		//����ʱ��
		m_strEndTime = NetbarBundleTime.GetAt(nBtIndex).GetEndTime(TRUE).Left(5);
		//��ʱ�۷�����
		m_nBundleTimeType = NetbarBundleTime.GetAt(nBtIndex).GetBundleTimeType();

		COleDateTimeSpan RemainTime;

		if (NetbarBundleTime.GetAt(nBtIndex).GetStartTime().IsEmpty())
		{
			RemainTime = COleDateTimeSpan(0, NetbarBundleTime.GetAt(nBtIndex).GetTimePass(), 0, 0);
		}
		else
		{
			COleDateTime StartTime, EndTime, CurTime = COleDateTime::GetCurrentTime();
			
			StartTime.ParseDateTime(NetbarBundleTime.GetAt(nBtIndex).GetStartTime(TRUE));
			StartTime.SetDateTime(CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay(),
				StartTime.GetHour(), StartTime.GetMinute(), StartTime.GetSecond());

			EndTime.ParseDateTime(NetbarBundleTime.GetAt(nBtIndex).GetEndTime(TRUE));
			EndTime.SetDateTime(CurTime.GetYear(), CurTime.GetMonth(), CurTime.GetDay(),
				EndTime.GetHour(), EndTime.GetMinute(), EndTime.GetSecond());

			if (StartTime > EndTime) //�����ʱ
			{
				if (CurTime < EndTime)
				{
					StartTime -= COleDateTimeSpan(1, 0, 0, 0); //��ȥһ��
				}
				else
				{
					EndTime += COleDateTimeSpan(1, 0, 0, 0); //����һ��
				}
			}
			
			if (CurTime < StartTime) //��ʱ��û��ʼ
			{
				RemainTime = COleDateTimeSpan(0, NetbarBundleTime.GetAt(nBtIndex).GetTimePass(), 0, 0);
			}
			else if (CurTime > EndTime) //��ʱ�Ѿ����ڣ�ֻ�ܿ�ʼ������İ�ʱ
			{

			}
			else //��ʱ���ڽ���
			{
				RemainTime = EndTime - CurTime;
			}

			//AfxMessageBox(StartTime.Format(_T("%Y-%m-%d %H:%M:%S")));
			//AfxMessageBox(EndTime.Format(_T("%Y-%m-%d %H:%M:%S")));
			//AfxMessageBox(m_strRemainTime);
		} 
		
		//m_strRemainTime = RemainTime.Format(_T("%H:%M:%S"));

		m_strRemainTime.Format(_T("%02d:%02d:%02d"), 
			(long)RemainTime.GetTotalHours(), RemainTime.GetMinutes(), RemainTime.GetSeconds());

		//��ʱ���
		m_nBundleTimeMoney = NetbarBundleTime.GetAt(nBtIndex).GetBundleTimeMoney();

		CBCGPDialog::OnOK();
	}
}

void CDlgBundleTimeList::OnBnClickedOk()
{
	OnOK();
}

void CDlgBundleTimeList::OnNMDblclkListBundletime(NMHDR *pNMHDR, LRESULT *pResult)
{
	OnOK();

	*pResult = 0;
}

void CDlgBundleTimeList::InsertItem(INT nArrayOrder, INT nItem)
{
	CString strTmp;

	UINT nTmp = NetbarBundleTime.GetAt(nArrayOrder).GetBundleTimeId();
	strTmp.Format(_T("%d"), nTmp);
	m_BundleTimeList.InsertItem(nItem, strTmp);

	strTmp = NetbarBundleTime.GetAt(nArrayOrder).GetStartTime();
	m_BundleTimeList.SetItemText(nItem, 1, strTmp);

	strTmp = NetbarBundleTime.GetAt(nArrayOrder).GetEndTime();
	m_BundleTimeList.SetItemText(nItem, 2, strTmp);

	nTmp = NetbarBundleTime.GetAt(nArrayOrder).GetTimePass();
	strTmp.Format(_T("%d"), nTmp);
	m_BundleTimeList.SetItemText(nItem, 3, strTmp);

	nTmp = NetbarBundleTime.GetAt(nArrayOrder).GetBundleTimeMoney();
	strTmp.Format(_T("%.2f"), nTmp / 100.0);
	m_BundleTimeList.SetItemText(nItem, 4, strTmp);

	strTmp = NetbarBundleTime.GetAt(nArrayOrder).GetBundleTimeTypeString();
	m_BundleTimeList.SetItemText(nItem, 5, strTmp);

	strTmp = NetbarBundleTime.GetAt(nArrayOrder).GetDescription();
	m_BundleTimeList.SetItemText(nItem, 6, strTmp);

	m_BundleTimeList.SetItemData(nItem, nArrayOrder);
}

CString CDlgBundleTimeList::GetBundleTimeMoneyString() const
{
	CString strTmp;

	strTmp.Format(_T("%.2f"), m_nBundleTimeMoney / 100.0);

	return strTmp;
}