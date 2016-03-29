// MemberInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "IBAHelpper.h"
#include "Cashier.h"
#include "Dal\IBADAL.h"
#include ".\memberinfodlg.h"

// CDlgMemberInfo �Ի���

IMPLEMENT_DYNAMIC(CDlgMemberInfo, CIBADialog)
CDlgMemberInfo::CDlgMemberInfo(CWnd* pParent /*=NULL*/)
: CIBADialog(CDlgMemberInfo::IDD, pParent)
, m_nQueryStyle(0)
{
	GPropertyInit( MemberId, 0 );
}

CDlgMemberInfo::~CDlgMemberInfo()
{
}

void CDlgMemberInfo::DoDataExchange(CDataExchange* pDX)
{
	CIBADialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MEMBERINFO, m_MemberInfoList);
}


BEGIN_MESSAGE_MAP(CDlgMemberInfo, CIBADialog)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MEMBERINFO, OnNMDblclkListMemberinfo)
END_MESSAGE_MAP()


// CDlgMemberInfo ��Ϣ�������

void CDlgMemberInfo::OnClose()
{
	CBCGPDialog::OnClose();
}

void CDlgMemberInfo::OnOK()
{
	if (GetMemberInfoFromList())
	{
		CBCGPDialog::OnOK();
	}
}

void CDlgMemberInfo::OnCancel()
{
	CBCGPDialog::OnCancel();
}

void CDlgMemberInfo::OnNMDblclkListMemberinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	OnOK();
	*pResult = 0;
}

// �����б���������
void CDlgMemberInfo::SetMemberInfoToList(UINT nQueryStyle)
{
	m_MemberInfoList.DeleteAllItems();
	
	switch (nQueryStyle)
	{
	case 0://�����Ĳ�ѯ
		{
			FindInfoFromServer(3);//�Ѿ��˿���û�
			break;
		}
	case 1://�ӱ��ز�ѯ��Ч�û�
		{
			FindInfoFromLocal(TRUE);
			break;
		}
	case 2://�����Ĳ�ѯ
		{
			FindInfoFromServer(1);//��Ч�û���ѯ
			break;
		}
	case 4://�ӱ��ز�ѯ�����û�
		{
			FindInfoFromLocal(FALSE);
			break;
		}
	default:
		IBA_ASSERT2(0,"�޷�ʶ��Ĳ�ѯ����");
		break;
	}
}

// ��ʼ���б�
void CDlgMemberInfo::InitList(void)
{
	CIBAString strHead;
	strHead.LoadString(IDS_MEMBERINFOLISTHEAD);
	//strHead = _T("�����˺�|����|�Ա�|�û�����|֤������|֤������|�û���");

	CStringArray strArray;
	CIBAHelpper::SplitLine(strHead, strArray);

	const UINT Width[] = {120, 65, 35, 70, 70, 120, 60};
	
	for (INT i = 0; i < strArray.GetCount(); i++)
	{
		m_MemberInfoList.InsertColumn(i, strArray.GetAt(i), LVCFMT_LEFT, Width[i]);
	}

	m_MemberInfoList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}

// ȡ��ѡ�е��û���Ϣ
int CDlgMemberInfo::GetMemberInfoFromList(void)
{
	INT SelIndex = m_MemberInfoList.GetSelectionMark();

	if (SelIndex >= 0)
	{
		GPropertyInit( NetId, m_MemberInfoList.GetItemText(SelIndex, 0) );
		GPropertyInit( UserName, m_MemberInfoList.GetItemText(SelIndex, 1) ); 
		GPropertyInit( Sex, m_MemberInfoList.GetItemText(SelIndex, 2) );
		GPropertyInit( UserClass, m_MemberInfoList.GetItemText(SelIndex, 3) );
		GPropertyInit( IDType, m_MemberInfoList.GetItemText(SelIndex, 4) );
		GPropertyInit( PersonalID, m_MemberInfoList.GetItemText(SelIndex, 5) );
		GPropertyInit( MemberId, _ttoi(m_MemberInfoList.GetItemText(SelIndex, 6)) );
		
		return TRUE;
	}

	return FALSE;
}

BOOL CDlgMemberInfo::OnInitDialog()
{
	CIBADialog::OnInitDialog();

	InitList();
	
	SetMemberInfoToList(m_nQueryStyle);

	//û�м�¼���Զ��ر�
	if (m_MemberInfoList.GetItemCount() == 0)
	{
		OnCancel();
	}
	else if (m_MemberInfoList.GetItemCount() == 1)
	{
		m_MemberInfoList.SetSelectionMark(0);
		OnOK();
	}
	else
	{
		m_MemberInfoList.SetSelectionMark(0);
	}

	m_MemberInfoList.SetFocus();
	m_MemberInfoList.SetHotItem(0);
	
	return FALSE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgMemberInfo::FindInfoFromServer(UINT nConsumeType /*= 0*/)
{
	//���뱣��
	if (!GetUserName().IsEmpty() || !GetPersonalID().IsEmpty())
	{	
		//ͨ���û�����֤���ŷ����û���Ϣ
		CMemberInfoMethod MemberInfo;

		MemberInfo.SetPersonalId( GetPersonalID() );
		MemberInfo.SetUsername( GetUserName() );
		MemberInfo.SetAvailable(nConsumeType);// 0ȫ����ѯ, 1��Ч��ѯ
		theApp.GetCurCashier()->DoMemberInfo(MemberInfo);

		if (MemberInfo.GetStatusCode() == 0)
		{
			INT ToTalCount = 0;//�����ʾ10����¼������ģ�
			if (MemberInfo.GetMemberCount() >= 20)
			{
				ToTalCount = 20;
			}
			else
			{
				if (MemberInfo.GetMemberCount() > 0 )
				{
					ToTalCount = MemberInfo.GetMemberCount();
				} 
			}

			for (INT ii = 0; ii < ToTalCount; ii++)
			{
				CIBAString strTmp;
				INT jj = 0;
				m_MemberInfoList.InsertItem(ii, MemberInfo.GetSerailNum(ii));
				m_MemberInfoList.SetItemText(ii, 1, MemberInfo.GetUserName(ii));

				if (MemberInfo.GetSex(ii) == 0)
				{
					strTmp.LoadString(IDS_MAN);
					m_MemberInfoList.SetItemText(ii, 2, strTmp);
				} 
				else
				{
					strTmp.LoadString(IDS_WOMAN);
					m_MemberInfoList.SetItemText(ii, 2, strTmp);
				}

				strTmp = CNetBarConfig::GetInstance()->GetUserClassName(MemberInfo.GetUserClass(ii));
				m_MemberInfoList.SetItemText(ii, 3, strTmp);

				m_MemberInfoList.SetItemText(ii, 4, CNetBarConfig::GetInstance()->GetIDTypeName(MemberInfo.GetIdType(ii)));

				m_MemberInfoList.SetItemText(ii, 5, MemberInfo.GetPersonalId(ii));

				strTmp.Format(_T("%d"), MemberInfo.GetMemberId(ii));
				m_MemberInfoList.SetItemText(ii, 6, strTmp);
			}
		} 
	} 
}

// �ӱ��ز�������
void CDlgMemberInfo::FindInfoFromLocal(BOOL bValidUser)
{
	//�˺ŷǿ�
	if (GetNetId().IsEmpty())
	{
		return;
	}

	CString strSQL;

	strSQL.Append(_T("SELECT * FROM member "));

	strSQL.AppendFormat(_T("WHERE serialNum LIKE '%%%s'"), GetNetId());

	if (bValidUser)
	{
		strSQL.Append(_T(" and (available = 0 or available = 1)"));	
	}

	strSQL.Append(_T(" and operationDateTime > '2011-1-1 0:0:0'"));	

	strSQL.Append(_T(" ORDER BY operationDateTime DESC"));

	CIBALog::GetInstance()->WriteFormat(CIBALog::ELT_SQL, _T("FindInfoFromLocal:%s"), strSQL);

	CADODBConnInfo* pDb = NULL;

	if (!CIBADAL::GetInstance()->GetDBConnInfo(pDb)) return;//�����ݿ�ʧ��

	CADORecordset Rs(pDb->GetConn());//���Ӽ�¼��

	if (Rs.Open(strSQL))//�򿪼�¼��
	{
		int i = 0;
		
		while (!Rs.IsEof())//��¼��δ����
		{
			INT nTmp = 0;
			CIBAString strTmp;
			
			Rs.GetFieldValue(_T("serialNum"), strTmp);
			m_MemberInfoList.InsertItem(i, strTmp);

			Rs.GetFieldValue(_T("name"), strTmp);
			m_MemberInfoList.SetItemText(i, 1, strTmp);

			Rs.GetFieldValue(_T("sex"), strTmp);
			
			if (_ttoi(strTmp) == 0)
			{
				strTmp.LoadString(IDS_MAN);
			} 
			else
			{
				strTmp.LoadString(IDS_WOMAN);
			}
			m_MemberInfoList.SetItemText(i, 2, strTmp);
			
			Rs.GetFieldValue(_T("classid"), nTmp);
			strTmp = CNetBarConfig::GetInstance()->GetUserClassName(nTmp);
			m_MemberInfoList.SetItemText(i, 3, strTmp);

			Rs.GetFieldValue(_T("idType"), nTmp);
			m_MemberInfoList.SetItemText(i, 4, CNetBarConfig::GetInstance()->GetIDTypeName(nTmp));

			Rs.GetFieldValue(_T("idNumber"), strTmp);
			m_MemberInfoList.SetItemText(i, 5, strTmp);

			Rs.GetFieldValue(_T("memberid"), strTmp);
			m_MemberInfoList.SetItemText(i, 6, strTmp);

			i++;
			Rs.MoveNext();//��һ����¼
		}
		Rs.Close();
	}

	pDb->Release();
}

