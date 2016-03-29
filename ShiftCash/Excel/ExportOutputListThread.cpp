#include "StdAfx.h"
#include "ExportOutputListThread.h"
#include "Excel.h"

CExportOutputListThread::CExportOutputListThread()
:m_strExportExcel(""), m_strFullPath(""), m_nRowCount(0)
{
}

CExportOutputListThread::~CExportOutputListThread()
{
}

int CExportOutputListThread::Run()
{
	ExportExcel();
	return 0;
}

void CExportOutputListThread::ExportExcel()
{
	if(m_strExportExcel.IsEmpty() )
	{
		AfxMessageBox(_T("��������Ϊ��"));
		return;
	}

	if(m_strFullPath.IsEmpty())
	{
		AfxMessageBox(_T("����·��Ϊ��"));
		return;
	}
	
	try
	{	
		// û��Flush�ᵼ��һЩ����������Ϊ�˰�ȫ�������ÿ��������Ӧ��Flushһ��
		// ��ͬ�ĵ�Ԫ���ʽ�������õ�λ�ò�ͬ��������Ч��Ҳ��ͬ����Ҫ����ʵ��Ч������

		// �������ӵ�����
		UINT nAddRowCount = 0;

		NS_GXX::CExcelExport exEp;
		// ��������
		exEp.CreateOneBlankBook();

		exEp.AppendData("ϣ֮������̨�����¼\t\n");
		exEp.Flush();
		nAddRowCount++;

		//exEp.AppendData("����ʱ��\t�û�����\t�ն˺�\t�ϻ�ʱ��\t�����˺�\t����\t֤������\t���\t��ע\n");
		CStringA strTitleA;
		/*strTitleA += ("ID\tStartDate\tStandbyAmount\tDeposit\tCreditAmount");
		strTitleA += ("\tPresentAmount\tReturnAmount\tCardSoldAmount\tCashSoldAmount");
		strTitleA += ("\tCardCreditedAmount\tTotalAmount\tCostExpense\tOperator");
		strTitleA += ("\tNextOperator\tDescription\tEndDate\tPayout\tremainAmount");
		strTitleA += ("\tCountAmount\tCashRegisterID");*/
		strTitleA += ("��ο�ʼʱ��\t��ν���ʱ��\t��ֵ�ܶ�\t�˿��ܶ�");
		strTitleA += ("\t��һ��Ԥ��\t��ֵ�����\t�ֽ���\t�㿨��ֵ�ܶ�");
		strTitleA += ("\t�Ͻɽ��\t������\t����Ա\t��һ���������");
		strTitleA += ("\t����֧��\tԤ�����\t�������");
		
		exEp.AppendData((LPSTR)strTitleA.GetString());
		exEp.Flush();
		nAddRowCount++;

		// �ܵ�����
		UINT nAllRowCount = m_nRowCount + nAddRowCount;
		// ���ݿ�ʼ��һ��
		UINT nBegRow = nAddRowCount + 1;

		// ���õ�Ԫ���ʽΪ�ı��� ���Ҫ�����ݼ���ǰ����
		exEp.Format_PutNumberFormat(exEp.GetActiveSheet(), m_nColCount, nBegRow, m_nColCount, nAllRowCount,"@");
		exEp.Flush();
		exEp.Format_PutNumberFormat(exEp.GetActiveSheet(), m_nColCount, nBegRow, m_nColCount, nAllRowCount,"@");
		exEp.Flush();

		USES_CONVERSION;		// ʹ��W2A������Ҫ���������
		exEp.AppendData(CT2A((LPCTSTR)m_strExportExcel));
		exEp.Flush();

		// ��һ�б��⣬���塢���塢�ֺ�20�� �ϲ�����
		exEp.Format_RangeFont(exEp.GetActiveSheet(), NS_GXX::CExcel::_A_, 1, m_nColCount, 1,
			RGB(0, 0, 0), "����", 20, TRUE);
		exEp.Flush();
		exEp.Format_RangeAlignment(exEp.GetActiveSheet(), NS_GXX::CExcel::_A_, 1, m_nColCount, 1, 2);
		exEp.Flush();

		exEp.Format_RangeMerge(exEp.GetActiveSheet(), NS_GXX::CExcel::_A_, 1, m_nColCount, 1);
		exEp.Flush();

		exEp.Format_RangeAlignment(exEp.GetActiveSheet(), NS_GXX::CExcel::_A_, 2, m_nColCount, nAllRowCount, 2);
		exEp.Flush();

		// ���õڶ��б���������Լ�������ɫ
		exEp.Format_RangeFont(exEp.GetActiveSheet(), NS_GXX::CExcel::_A_, 2, m_nColCount, 2,
			RGB(0, 0, 0), "����", 14, TRUE);
		exEp.Flush();

		// ���ñ�����ɫ
		exEp.Format_RangeBkColor(exEp.GetActiveSheet(), NS_GXX::CExcel::_A_, 2, m_nColCount, 2,
			RGB(204, 204, 255));
		exEp.Flush();

		// ���ñ߿� �ӵڶ��б��⿪ʼ
		exEp.Format_PutBorderAround(exEp.GetActiveSheet(), NS_GXX::CExcel::_A_, 2, m_nColCount, nAllRowCount);
		exEp.Flush();

		// ���õ�Ԫ�������
		exEp.Format_RangeAlignment(exEp.GetActiveSheet(), NS_GXX::CExcel::_A_, 2, m_nColCount, nAllRowCount, 1);
		exEp.Flush();

		// �����ݵĲ���
		// ���õ�Ԫ���Ϊ20��
		//exEp.Format_ColsWidth(exEp.GetActiveSheet(), NS_GXX::CExcel::_A_, m_nColCount, 20);

		// �������е�Ԫ��������Ӧ�����Ҫ��������������������ã����������Ч��
		exEp.Format_PutAutoFit(exEp.GetActiveSheet(), NS_GXX::CExcel::_A_, 1, m_nColCount, nAllRowCount, TRUE);
		exEp.Flush();
		
		// ��ӵ�ǰ���� 2013-3-12 ���ڶ� 15:37:23, �ϲ�����
		UINT DayOfWeek[] = {
			LOCALE_SDAYNAME7,   // Sunday
				LOCALE_SDAYNAME1,   
				LOCALE_SDAYNAME2,
				LOCALE_SDAYNAME3,
				LOCALE_SDAYNAME4, 
				LOCALE_SDAYNAME5, 
				LOCALE_SDAYNAME6   // Saturday
		};

		TCHAR strWeekday[256];
		CTime time(CTime::GetCurrentTime());   // Initialize CTime with current time
		::GetLocaleInfo(LOCALE_USER_DEFAULT,   // Get string for day of the week from system
			DayOfWeek[time.GetDayOfWeek()-1],   // Get day of week from CTime
			strWeekday, sizeof(strWeekday));

		CString strData;
		strData.Format(_T("�Ʊ����ڣ� %s %s %s\n"), time.Format("%Y-%m-%d"), strWeekday, time.Format("%H:%M:%S"));
		
		exEp.AppendData(CT2A((LPCTSTR)strData));
		exEp.Flush();

		exEp.Format_RangeAlignment(exEp.GetActiveSheet(), NS_GXX::CExcel::_A_, nAllRowCount + 1, m_nColCount, nAllRowCount + 1, 1);
		exEp.Flush();

		exEp.Format_RangeMerge(exEp.GetActiveSheet(), NS_GXX::CExcel::_A_, nAllRowCount + 1, m_nColCount, nAllRowCount + 1);
		exEp.Flush();



		if(exEp.Save(m_strFullPath))
		{
			exEp.Close();
			ShellExecute(NULL, _T("open"), m_strFullPath, NULL, NULL, SW_HIDE);
		//	CString strMsg;
		//	strMsg.Format(_T("������\"%s\"�ɹ�"), m_strFullPath);
		//	AfxMessageBox(strMsg);
		}
		
	}
	catch(...)
	{
		AfxMessageBox(_T("����ʧ��"));
	}
}

void CExportOutputListThread::SetParam( LPCTSTR szExportExcel, LPCTSTR szFullPath, UINT nRowCount, UINT nColCount)
{
	m_strExportExcel = szExportExcel;
	m_strFullPath = szFullPath;
	m_nRowCount = nRowCount;
	m_nColCount = nColCount;
}

