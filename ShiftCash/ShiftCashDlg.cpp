// ShiftCashDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ShiftCash.h"
#include "ShiftCashDlg.h"
#include ".\shiftcashdlg.h"
#include <shlwapi.h>
#include ".\Excel\ExportOutputListThread.h"
#include <afxdb.h>
#include <vector>
#include <atlbase.h>

#include "ExportRecord.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


const ExcelColCount = 15;
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CShiftCashDlg �Ի���



CShiftCashDlg::CShiftCashDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShiftCashDlg::IDD, pParent)
	, m_strFilePath(_T("C:\\�����¼��.xls"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CShiftCashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strFilePath);
}

BEGIN_MESSAGE_MAP(CShiftCashDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_EXPORT_EXCEL, OnBnClickedBtnExportExcel)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BTN_EXPORT_ONKEY_COST, OnBnClickedBtnExportOnkeyCost)
	ON_BN_CLICKED(IDC_BTN_EXPORT_CARDOPEN, OnBnClickedBtnExportCardopen)
	ON_BN_CLICKED(IDC_BTN_EXPORT_CASH_PAY, OnBnClickedBtnExportCashPay)
END_MESSAGE_MAP()


// CShiftCashDlg ��Ϣ�������

BOOL CShiftCashDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CRegKey RegKey;
	if (ERROR_SUCCESS == RegKey.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\ShiftCash")))
	{
		ULONG ulLen = 256;
		RegKey.QueryStringValue(_T("ExcelPath"), m_strFilePath.GetBuffer(256), &ulLen);
		m_strFilePath.ReleaseBuffer();	
	}
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CShiftCashDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CShiftCashDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CShiftCashDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

struct ShiftCashInfo
{
	CString ID;
	CString StartDate;
	CString StandbyAmount;
	CString Deposit;
	CString CreditAmount;
	CString PresentAmount; 
	CString ReturnAmount;
	CString CardSoldAmount;
	CString CashSoldAmount; 
	CString CardCreditedAmount;
	CString TotalAmount;
	CString CostExpense; 
	CString Operator; 
	CString NextOperator;
	CString Description; 
	CString EndDate;
	CString Payout;
	CString remainAmount; 
	CString CountAmount;
	CString CashRegisterID;

	CString Combine()
	{
		CString strCombine;
		/*strCombine.AppendFormat(_T("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t\%s\t%s\t%s\t%s\t%s"), 
							 ID,
							 StartDate,
							 StandbyAmount,
							 Deposit,
							 CreditAmount,
							 PresentAmount,
							 ReturnAmount,
							 CardSoldAmount,
							 CashSoldAmount,
							 CardCreditedAmount,
							 TotalAmount,
							 CostExpense, 
							 Operator,
							 NextOperator,
							 Description,
							 EndDate,
							 Payout,
							 remainAmount,
							 CountAmount,
							 CashRegisterID);*/

		strCombine.AppendFormat(_T("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t"),
							 StartDate,			// ��ο�ʼʱ��
							 EndDate,			// ��ν���ʱ��
							 CreditAmount,		// ��ֵ�ܶ�
							 ReturnAmount,		// �˿��ܶ�				
							 Deposit,			// ��һ��Ԥ��		 
							 CardSoldAmount,		// ��ֵ�����
							 CashSoldAmount,		// �ֽ���
							 CardCreditedAmount,	// �㿨��ֵ�ܶ�
							 TotalAmount,			// �Ͻɽ�� 
							 CostExpense,			// ������
							 Operator,				// ����Ա
							 NextOperator,			// ��һ���������
							 Payout,				// ����֧��
							 remainAmount,			// Ԥ�����
							 Description			// �������
							 /*CountAmount,mount,
							 StandbyAmount,	*/ 
							 );
		// ��ο�ʼʱ��\t��ν���ʱ��\t��ֵ�ܶ�\t�˿��ܶ�\t��һ��Ԥ��\t��ֵ�����\t�ֽ���\t�㿨��ֵ�ܶ�\t�Ͻɽ��\t������\t����Ա\t��һ���������\t����֧��\tԤ�����\t�������
		return strCombine;
	}
};

BOOL GetShiftCash(std::vector<ShiftCashInfo> &vecSci)
{
	CString strSql;
	strSql.Format(_T("SELECT * FROM SHIFTCASH"));

	CDatabase database;

	CString strDb;

	try
	{
		//strDb.Format( _T("ODBC;Database=%s;Provider=SQLOLEDB;Driver={MySQL ODBC 3.51 Driver};Server=%s;Uid=%s;Pwd=%s"), 
		//	_T("netbar"), _T("127.0.0.1"), _T("root"), _T("szsunlikedataserver"));
		//strDb.Format( _T("DSN=%s"), _T("netbar") );

		strDb = _T("DSN=netbar;Uid=root;Pwd=szsunlikedataserver");

		database.OpenEx( strDb, CDatabase::openReadOnly | 
			CDatabase::noOdbcDialog );

		if( !database.IsOpen() )
		{
			CString strError(_T(""));
			strError.Format(_T("�����ݿ�ʧ�ܣ�%d"), GetLastError());
			AfxMessageBox(strError);
			return FALSE;
		}
		
		CRecordset ds(&database);
		if(!ds.Open(AFX_DB_USE_DEFAULT_TYPE, strSql))
		{
			AfxMessageBox(_T("��ִ��ʧ�ܣ�"));
			return FALSE;
		}

		ShiftCashInfo shiftCashInfo;
		while(!ds.IsEOF())
		{
			{
				ds.GetFieldValue(_T("ID"), shiftCashInfo.ID);
				ds.GetFieldValue(_T("StartDate"), shiftCashInfo.StartDate);
				ds.GetFieldValue(_T("StandbyAmount"), shiftCashInfo.StandbyAmount);
				ds.GetFieldValue(_T("Deposit"), shiftCashInfo.Deposit);
				ds.GetFieldValue(_T("CreditAmount"), shiftCashInfo.CreditAmount);
				ds.GetFieldValue(_T("PresentAmount"), shiftCashInfo.PresentAmount);
				ds.GetFieldValue(_T("ReturnAmount"), shiftCashInfo.ReturnAmount);
				ds.GetFieldValue(_T("CardSoldAmount"), shiftCashInfo.CardSoldAmount);
				ds.GetFieldValue(_T("CashSoldAmount"), shiftCashInfo.CashSoldAmount);
				ds.GetFieldValue(_T("CardCreditedAmount"), shiftCashInfo.CardCreditedAmount);
				ds.GetFieldValue(_T("TotalAmount"), shiftCashInfo.TotalAmount);
				ds.GetFieldValue(_T("CostExpense"), shiftCashInfo.CostExpense);
				ds.GetFieldValue(_T("Operator"), shiftCashInfo.Operator);
				ds.GetFieldValue(_T("NextOperator"), shiftCashInfo.NextOperator);
				ds.GetFieldValue(_T("Description"), shiftCashInfo.Description);
				ds.GetFieldValue(_T("EndDate"), shiftCashInfo.EndDate);
				ds.GetFieldValue(_T("Payout"), shiftCashInfo.Payout);
				ds.GetFieldValue(_T("remainAmount"), shiftCashInfo.remainAmount);
				ds.GetFieldValue(_T("CountAmount"), shiftCashInfo.CountAmount);
				ds.GetFieldValue(_T("CashRegisterID"), shiftCashInfo.CashRegisterID);
				vecSci.push_back(shiftCashInfo);
			}
			ds.MoveNext();
		}
		//CString str;
		//ds.GetFieldValue((short)0, str);
		return TRUE;
		//return (0 == nCount);
	}
	catch(CDBException* e )
	{
		TCHAR szMeg[256] = {0};
		e->GetErrorMessage(szMeg, 256);
		
		CString strMsg;
		strMsg.Format(_T("�������ݿ��쳣��%s"), szMeg);
		AfxMessageBox(strMsg);
	}
	return FALSE;
}

void CShiftCashDlg::OnBnClickedBtnExportExcel()
{
	GetDlgItemText(IDC_EDIT1, m_strFilePath);

	CRegKey RegKey;
	if (ERROR_SUCCESS == RegKey.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\ShiftCash")))
	{
		RegKey.SetStringValue(_T("ExcelPath"), m_strFilePath);
	}
	
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ��ȡ����������
	/*int nRowCount = m_listCheckOut.GetItemCount();
	CBCGPHeaderCtrl& Header = m_listCheckOut.GetHeaderCtrl();
	int nColCount = Header.GetItemCount();*/
	int nRowCount = 0;
	int nColCount = ExcelColCount;
	std::vector<ShiftCashInfo> vecSci;
	GetShiftCash(vecSci);
	nRowCount = vecSci.size();


	CString strAppend("");
	CString strTmp("");

	//for(int nRow = 0; nRow < nRowCount; nRow++)
	//{
	//	for(int nCol = 0; nCol < nColCount; nCol++)
	//	{
	//		strTmp.Empty();
	//		//strTmp = m_listCheckOut.GetItemText(nRow, nCol);
	//		// "\t" ��ʾ������һ����Ԫ��
	//		strAppend += strTmp + _T("\t");
	//	}
	//	// "\n" ��ʾ������һ��
	//	strAppend += _T("\n");	
	//}
	//strAppend += _T("\n");	

	CString strFullPath;
	/*GetTempPath(MAX_PATH, strFullPath.GetBuffer(MAX_PATH));
	strFullPath.ReleaseBuffer();*/
	strFullPath = m_strFilePath;

	//strFullPath += _T("�����¼��.xls");


	if(PathFileExists(strFullPath))
	{
		while(!DeleteFile(strFullPath))
		{
			static UINT nCount = 0;
			CString strReplace("");
			strReplace.Format(_T("tmp%d.xls"), nCount++);
			strFullPath.Replace(_T(".xls"), strReplace);		// ����ļ��޷�ɾ�������޸��ļ���

			if(nCount > 5)		// ����5���Ͳ�Ҫ�ٳ�����
				break;
		}
	}

	for(std::vector<ShiftCashInfo>::iterator iter = vecSci.begin(); iter != vecSci.end(); ++iter)
	{
		strAppend += iter->Combine();
		strAppend += _T("\n");
	}
	//strAppend = 
	// nRowCount+1 ������������һ�б�����
	static CExportOutputListThread expThread;
	expThread.SetParam(strAppend, strFullPath, nRowCount, nColCount);
	expThread.Start();
}

void CShiftCashDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog    dlgFile(FALSE, m_strFilePath, m_strFilePath, OFN_HIDEREADONLY, _T("Excel (*.xls)|*.xls|All Files (*.*)|*.*||"), NULL);


	if (IDOK == dlgFile.DoModal())
	{
		m_strFilePath = dlgFile.GetPathName();
		UpdateData(FALSE);
	}
}

void CShiftCashDlg::OnBnClickedBtnExportOnkeyCost()
{
	// �ϻ����Ѽ�¼
	GetDlgItemText(IDC_EDIT1, m_strFilePath);

	static CExportRecordOnkeyCost onkeyCost;
	if (onkeyCost.Init(m_strFilePath.GetBuffer()))
		onkeyCost.Work();
}

void CShiftCashDlg::OnBnClickedBtnExportCardopen()
{
	// ������¼
	GetDlgItemText(IDC_EDIT1, m_strFilePath);

	static CExportRecordCardOpen cardOpen;
	if (cardOpen.Init(m_strFilePath.GetBuffer()))
		cardOpen.Work();
}

void CShiftCashDlg::OnBnClickedBtnExportCashPay()
{
	// �ֽ��ֵ��¼
	GetDlgItemText(IDC_EDIT1, m_strFilePath);

	static CExportRecordCashPay cashPay;
	if (cashPay.Init(m_strFilePath.GetBuffer()))
		cashPay.Work();
}
