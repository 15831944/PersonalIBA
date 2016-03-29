#include "StdAfx.h"
#include "Excel.h"
#include "MyString.h"
#include "ClipBoardMgr.h"

#include <WinGDI.h>

#ifndef ASSERT
#define ASSERT(x) 
#endif

//#define //ExcelTrace GXX_AUTO_TRACE
////#define //ExcelTrace2 GXX_TRACE

namespace NS_GXX
{
	BOOL MyErrorMsg(_com_error& err)
	{
		_bstr_t br = err.Description();
		br += _T("  ");
		br += err.ErrorMessage();
		MessageBoxW(NULL,br,L"Excel error!",MB_OK|MB_ICONWARNING);
		return FALSE;
	}

    CExcel::CExcel(void)
    {
        m_bCreated = FALSE;
        m_bClosed = TRUE;
        memset(m_lpBookName,0,sizeof(TCHAR)*1024);

		m_hMainWnd = (HANDLE)0;
    }

    CExcel::~CExcel(void)
    {
        Close();
    }

    BOOL CExcel::CreateOneBlankBook(BOOL bShowApp /* = FALSE */,const TCHAR* lpszSheepName)
    {
		//ExcelTrace("�����µĹ�����");
        if( m_bCreated )
        {
             Close();
             m_bCreated = FALSE;
        }
        try
        {
             // add-s yang
             m_ptrApp_tmp.CreateInstance(L"Excel.Application");
             // add-e yang
             m_ptrApp.CreateInstance(L"Excel.Application");
             m_ptrApp->UserControl = VARIANT_FALSE;      //�����������û�����
			 m_ptrApp->PutDisplayClipboardWindow(0,VARIANT_FALSE);
             if( bShowApp )
             {
                 m_ptrApp->Visible[0] = VARIANT_TRUE;
             }
             else
             {
                m_ptrApp->Visible[0] = VARIANT_FALSE;
             }
             m_ptrBooks = m_ptrApp->GetWorkbooks();
             m_ptrBook = m_ptrBooks->Add((long)xlWorkbook);
			// m_ptrBook->MultiUserEditing[0] = VARIANT_TRUE;
             m_ptrSheets = m_ptrBook->GetSheets();
             m_bCreated = TRUE;
             m_bClosed = FALSE;
             SetSheetName(1,lpszSheepName);
             SetActiveSheet(1);
			 m_ptrApp->DisplayAlerts[0] = VARIANT_FALSE;
             return TRUE;
        }
        catch(_com_error &err)
        {	
			//ExcelTrace2(L"�����¹����������쳣,%s",(const wchar_t*)err.Description());
            ASSERT(MyErrorMsg( err ));
            m_bCreated = FALSE;
            return FALSE;
        }  
    }
    BOOL CExcel::OpenBook(TCHAR *lpszBookPath,BOOL bShowApp /* = FALSE */)
    {
		//ExcelTrace(L"�򿪹�����:%s",lpszBookPath);
        if( ! PathFileExists(lpszBookPath) )
        {
			//ExcelTrace2("�򿪹�����ʧ��,�ļ�������");
            return FALSE;
        }
        if( m_bCreated )
        {
            Close();
            m_bCreated = FALSE;
        }
        try
        {
			// add-s yang
			m_ptrApp_tmp.CreateInstance(L"Excel.Application");

            m_ptrApp.CreateInstance(L"Excel.Application");
            if( bShowApp )
            {
                m_ptrApp->Visible[0] = VARIANT_TRUE;
            }
            else
            {
                m_ptrApp->Visible[0] = VARIANT_FALSE;
            }
            m_ptrBooks = m_ptrApp->GetWorkbooks();
            m_ptrBook = m_ptrBooks->Open((_bstr_t)lpszBookPath);
            m_ptrSheets = m_ptrBook->GetSheets();
            m_bCreated = TRUE;
            m_bClosed = FALSE;
            SetActiveSheet(1);  //Ĭ�ϼ������1
            memcpy(m_lpBookName,lpszBookPath,(wcslen(lpszBookPath)+1)*sizeof(TCHAR) );
			m_ptrApp->DisplayAlerts[0] = VARIANT_FALSE;
            return TRUE;
        }
        catch(_com_error &err)
        {
			//ExcelTrace2(L"�򿪹����������쳣,%s",(const wchar_t*)err.Description());
            ASSERT(MyErrorMsg(err));
            m_bCreated = FALSE;		
            return FALSE;
        }  
    }
    BOOL CExcel::Save(LPCTSTR lpszFullPath)
    {
        try
        {
			if( ::PathFileExists(lpszFullPath) )
			{
				//ExcelTrace2("Դ�ļ��Ѵ���..");
				if( MessageBoxW((HWND)m_hMainWnd,L"�ļ��Ѵ��ڣ��Ƿ񸲸�ԭ�ļ�?",L"��ʾ",MB_YESNO|MB_ICONQUESTION) )
				{
					if( ::DeleteFileW(lpszFullPath) == FALSE)
					{
						NS_GXX::CMyStringW strErr;
						strErr.Format(L"%s    \r\n\r\n�ܾ�����!\r\n�ļ��������ط���ʹ��",lpszFullPath);
						MessageBox((HWND)m_hMainWnd,strErr,L"Error",MB_OK|MB_ICONERROR);
						return FALSE;
					}
				}
			}
            BSTR bstr = _bstr_t(lpszFullPath);
            VARIANT v1;
            v1.vt = VT_BSTR;
            v1.bstrVal = bstr;
            HRESULT hR = m_ptrBook->SaveAs(v1,vtMissing,vtMissing,vtMissing,FALSE,FALSE,xlNoChange);
            return SUCCEEDED(hR);
        }
        catch (_com_error &err)
        {
			//ExcelTrace2("���湤���������쳣:%s",(const wchar_t*)err.Description());
            ASSERT(MyErrorMsg(err));
            return FALSE;
        }
    }
    BOOL CExcel::Save()
    {
        //return Save(m_lpBookName);
		try
		{
			BOOL bRes = SUCCEEDED( m_ptrBook->Save() );
			return bRes;
		}
		catch (_com_error &err)
		{
			//ExcelTrace2("���湤���������쳣:%s",(const wchar_t*)err.Description());
			ASSERT(MyErrorMsg(err));
			return FALSE;
		}
        
    }
    void CExcel::Close()
    {
		//ExcelTrace("�رչ��������˳�ExcelӦ�ó���");
        if( m_bClosed == FALSE)
        {
            try
            {
                m_ptrBook->Saved[0] = VARIANT_TRUE;
                m_ptrApp->Quit();
                m_ptrSheets.Release();
                m_ptrBook.Release();
                m_ptrBooks.Release();
                m_ptrApp.Release();
                m_bClosed = TRUE;

                // add-s yang
                //�û���ʹ���򲻹رս���
                if (m_ptrApp_tmp->GetWorkbooks()->Count == 0)
                {
                    m_ptrApp_tmp->Quit();
                }
                // add-e yang
            }       
            catch(_com_error &err)
            {
                ASSERT(MyErrorMsg(err));
                m_bClosed= TRUE;
            }  
        }
    }
    int CExcel::GetSheetCount()
    {
        return m_ptrSheets->GetCount();
    }
	BOOL CExcel::DeleteSheet( int iIndex )
	{
		//ExcelTrace("ɾ��������,%d",iIndex);
		try
		{
			if( iIndex < 1 || iIndex > GetSheetCount() )
			{
				ASSERT(0);
				return FALSE;
			}
			_WorksheetPtr ptrSheet = m_ptrSheets->GetItem(iIndex);
			if( ptrSheet == NULL )
			{
				//�ж��ǲ���ͼ��sheet
				_ChartPtr ptrChart = m_ptrSheets->GetItem(iIndex);
				if( ptrChart != NULL)
				{
					ptrChart->Select();
					ptrChart->Delete();
				}
			}
			else //�ǹ�����sheet
			{
				ptrSheet->Select();
				ptrSheet->Delete();
			}	

			return TRUE;
		}
		catch (_com_error &err)
		{
			//ExcelTrace2("ɾ��������ʧ��");
			ASSERT(MyErrorMsg(err));
			return FALSE;
		}  
		
	}

	BOOL CExcel::DeleteSheet( const TCHAR * lpszSheetName )
	{
		//ExcelTrace(L"ɾ��������,%s",lpszSheetName);
		try
		{
			if( lpszSheetName == NULL || lpszSheetName[0] == L'\0')
			{
				ASSERT(0);
				return FALSE;
			}
			_bstr_t bs = lpszSheetName;
			_WorksheetPtr ptrSheet = m_ptrSheets->GetItem(bs);
			if( ptrSheet == NULL )
			{
				//�ж��ǲ���ͼ��sheet
				_ChartPtr ptrChart = m_ptrSheets->GetItem(bs);
				if( ptrChart != NULL)
				{
					ptrChart->Select();
					ptrChart->Delete();
				}
			}
			else
			{
				ptrSheet->Select();
				ptrSheet->Delete();
			}
			
			
			return TRUE;
		}
		catch (_com_error &err)
		{
			//ExcelTrace2("ɾ��������ʧ��");
			ASSERT(MyErrorMsg(err));
			return FALSE;
		}

	}
    BOOL CExcel::InsertSheet(const TCHAR * lpszSheetName)
    {
		//ExcelTrace(L"���빤����:%s",lpszSheetName);
        try
        {
            long lNum = m_ptrSheets->GetCount();
			NS_GXX::CMyStringW strSheetName;
            if(lpszSheetName == NULL || lpszSheetName[0] == L'\0')
            {		
				strSheetName.Format(L"%s%d",L"sheet",lNum);
            }
			else
			{
				strSheetName = lpszSheetName;
			}

			VARIANT var;
			var.vt = VT_DISPATCH;

            _WorksheetPtr pSheet = m_ptrSheets->GetItem(lNum);
            if( pSheet == NULL )
			{
				_ChartPtr pChart = m_ptrSheets->GetItem(lNum);
				if( pChart == NULL )
				{
					return FALSE;
				}
				var.pdispVal = pChart;
			}
			else
			{
				var.pdispVal = pSheet;
			}
            
            //�����һ��������������һ��������
            pSheet = m_ptrSheets->Add(vtMissing,var);
            pSheet->PutName(strSheetName.GetData());
            pSheet->Activate();
            return TRUE;
        }
        catch (_com_error &err)
        {
            ASSERT( MyErrorMsg(err) );
            return FALSE;
        }  
    }
    BOOL CExcel::InsertSheet(int iIndex, const TCHAR * lpszSheetName,BOOL bAtBefore)
    {
		//ExcelTrace(L"���빤����:%s",lpszSheetName);
        if( iIndex <= 0 || iIndex > m_ptrSheets->GetCount() )
        {
            return InsertSheet(lpszSheetName);
        }
        try
        {
            long lNum = m_ptrSheets->GetCount();
			NS_GXX::CMyStringW strSheetName;

            if(lpszSheetName == NULL || lpszSheetName[0] == L'\0')
            {		
                strSheetName.Format(L"%s%d",L"sheet",lNum);
            }
			else
			{
				strSheetName = lpszSheetName;
			}

			VARIANT var;
			var.vt = VT_DISPATCH;

			_WorksheetPtr pSheet = m_ptrSheets->GetItem(lNum);
			if( pSheet == NULL )
			{
				_ChartPtr pChart = m_ptrSheets->GetItem(lNum);
				if( pChart == NULL )
				{
					return FALSE;
				}
				var.pdispVal = pChart;
			}
			else
			{
				var.pdispVal = pSheet;
			}

            //��iIndex������ǰ�����һ��������
            if( bAtBefore)
            {
                  pSheet = m_ptrSheets->Add(var,vtMissing);
            }
            else
            {
                 pSheet = m_ptrSheets->Add(vtMissing,var);
            }
            pSheet->PutName(strSheetName.GetData());
            pSheet->Activate();
            return TRUE;
        }
        catch (_com_error &err)
        {
            ASSERT(MyErrorMsg(err));
            return FALSE;
        }  
    }

    // add yang
    BOOL CExcel::InsertChartSheet(int iIndex, const TCHAR *lpszSheetName, BOOL bAtBefore)
    {
        //ExcelTrace(L"����graph��:%s",lpszSheetName);
        if( iIndex <= 0 || iIndex > m_ptrSheets->GetCount() )
        {
            // return InsertSheet(lpszSheetName);
            return FALSE;
        }
        try
        {
            long lNum = m_ptrSheets->GetCount();
            NS_GXX::CMyStringW strSheetName;

            if(lpszSheetName == NULL || lpszSheetName[0] == L'\0')
            {		
                strSheetName.Format(L"%s%d",L"graph",lNum);
            }
            else
            {
                strSheetName = lpszSheetName;
            }

           
            _WorksheetPtr pSheet = GetSheet(1);
            _ChartPtr pChart = m_ptrBook->GetCharts()->Add();

            RangePtr ptrRange = pSheet->GetRange(L"A1",L"B3");

            pChart->ChartType = xlLineMarkers;
            pChart->SetSourceData(ptrRange);
            pChart->PutName(strSheetName.GetData());
            pChart->Activate();
            return TRUE;
        }
        catch (_com_error &err)
        {
            ASSERT(MyErrorMsg(err));
            return FALSE;
        }  
    }
    // add yang

    BOOL CExcel::SetActiveSheet(int iIndex)
    {
		//ExcelTrace("�������%d",iIndex);
        if( iIndex <= 0 || iIndex > m_ptrSheets->GetCount() )
        {
            return FALSE;
        }
        try
        {
            _WorksheetPtr ptrSheet = m_ptrSheets->GetItem(iIndex);
			if( ptrSheet == NULL)
			{
				return FALSE;
			}
            HRESULT hr = ptrSheet->Activate();
            return SUCCEEDED(hr);
        }
        catch (_com_error& err)
        {
            ASSERT(MyErrorMsg(err));
            return FALSE;
        }      
    }
    BOOL CExcel::SetActiveSheet(TCHAR * lpszSheetName)
    {
		//ExcelTrace(L"�������%d",lpszSheetName);
        if( lpszSheetName == NULL )
        {
            return FALSE;
        }
        try
        {
            _bstr_t bstr = lpszSheetName;
            _WorksheetPtr ptrSheet = m_ptrSheets->GetItem(bstr);
			if( ptrSheet == NULL )
			{
				return FALSE;
			}
            HRESULT hr = ptrSheet->Activate();
            return SUCCEEDED(hr);
        }
        catch (_com_error& err)
        {
            ASSERT(MyErrorMsg(err));
            return FALSE;
        }

    }
    BOOL CExcel::SetSheetName(int iIndex,const TCHAR * lpszSheetName)
    {
        if( lpszSheetName == NULL || iIndex <= 0 || iIndex> m_ptrSheets->GetCount() )
        {
            return FALSE;
        }
        try
        {
            _WorksheetPtr ptrSheet = m_ptrSheets->GetItem(iIndex);
			if( ptrSheet == NULL )
			{
				_ChartPtr ptrChart = m_ptrSheets->GetItem(iIndex);
				if( ptrChart == NULL)
				{
					return FALSE;
				}
				ptrChart->PutName(lpszSheetName);
			}
			else
			{
				ptrSheet->PutName(lpszSheetName);
			}
            return TRUE;
        }
        catch (_com_error& err)
        {
            ASSERT(MyErrorMsg(err));
            return FALSE;
        }
        
    }

    _bstr_t CExcel::ConvertCol(int iCol)
    {
        _bstr_t br="";
		CMyStringW colR;
		colR.MakeExcelColumn(iCol);
		br = colR.GetData();
        return br;
	}
	_bstr_t CExcel::ConvertColRow( int iCol,int iRow )
	{
		_bstr_t br="";
		CMyStringW colR;
		colR.MakeExcelColumn(iCol);
		colR.AppendFormat(L"%d",iRow);
		br = colR.GetData();
		return br;
	}

	Excel::_WorksheetPtr CExcel::GetActiveSheet()
	{
		return m_ptrBook->GetActiveSheet();
	}

	Excel::_WorksheetPtr CExcel::GetSheet(const int iIndex)
	{
		return m_ptrSheets->GetItem(iIndex);
	}

	Excel::_WorksheetPtr CExcel::GetSheet( const WCHAR * lpszName )
	{
		_bstr_t bstr = lpszName;
		return m_ptrSheets->GetItem(bstr);
	}
	Excel::_ChartPtr CExcel::GetChart( const int iIndex )
	{
		if( iIndex < 1 || iIndex > GetSheetCount() )
		{
			ASSERT(0);
			return NULL;
		}
		return m_ptrSheets->GetItem(iIndex);
	}

	Excel::_ChartPtr CExcel::GetChart( const WCHAR* lpszName )
	{
		if( lpszName == NULL || lpszName[0] == L'\0')
		{
			ASSERT(0);
			return NULL;
		}
		return m_ptrSheets->GetItem(lpszName);
	}

	void CExcel::Format_ColsWidth( _WorksheetPtr ptrSheet,int iCol1,int iCol2,float fWidth /*= 8.50f*/ )
	{
		_bstr_t bstart = ConvertColRow(iCol1,1);
		_bstr_t bend = ConvertColRow(iCol2,1);
		RangePtr ptrRange = ptrSheet->Range[bstart][bend];
		ptrRange->Select();
		ptrRange->PutColumnWidth(fWidth)	;
	}

	void CExcel::Format_RangeBkColor( _WorksheetPtr ptrSheet,int iCol1,int iRow1,int iCol2,int iRow2,DWORD clr/*=RGB(196,196,196)*/ )
	{
		ptrSheet->Activate();
		_bstr_t bstart = ConvertColRow(iCol1,iRow1);
		_bstr_t bend = ConvertColRow(iCol2,iRow2);
		RangePtr ptrRange = ptrSheet->Range[bstart][bend];
		ptrRange->Select();
		ptrRange->Interior->Color = clr;
	}

	void CExcel::Format_RangeFontColor( _WorksheetPtr ptrSheet,int iCol1,int iRow1,int iCol2,int iRow2,DWORD clr/*=RGB(196,196,196) */ )
	{
		_bstr_t bstart = ConvertColRow(iCol1,iRow1);
		_bstr_t bend = ConvertColRow(iCol2,iRow2);
		RangePtr ptrRange = ptrSheet->Range[bstart][bend];
		ptrRange->Select();
		ptrRange->Font->Color = clr;
	}
	void CExcel::Format_RangeFont( _WorksheetPtr ptrSheet,int iCol1,int iRow1,int iCol2,int iRow2,DWORD clr/*=RGB(0,0,0)*/,const char* fontName /*= "����"*/,int nFontSize /*= 10 */,BOOL bBold ,BOOL bItalic  )
	{
		_bstr_t bstart = ConvertColRow(iCol1,iRow1);
		_bstr_t bend = ConvertColRow(iCol2,iRow2);
		RangePtr ptrRange = ptrSheet->Range[bstart][bend];
		ptrRange->Select();
		ptrRange->Font->Color = clr;
		ptrRange->Font->Name = fontName;
		ptrRange->Font->Size = nFontSize;
		ptrRange->Font->Bold = bBold;
		ptrRange->Font->Italic = bItalic;
	}
	void CExcel::Format_BorderRange( _WorksheetPtr ptrSheet,DWORD clr,int col1,int row1,int col2,int row2,int nWeight /*= 2*/ ,int nType )
	{
	/*	try
		{*/

		
		_bstr_t bstart = ConvertColRow(col1,row1);
		_bstr_t bend = ConvertColRow(col2,row2);
		RangePtr range = ptrSheet->Range[bstart][bend];
		range->Select();
		//range->BorderAround(xlContinuous,xlThin,xlColorIndexAutomatic,RGB(255,0,0));
		XlBorderWeight borWg;
		switch( nWeight )
		{
		case 1:
			borWg = xlHairline;
			break;
		case 2:
			borWg = xlThin;
			break;
		case 3:
			borWg = xlMedium;
			break;
		case 4:
			borWg = xlThick;
			break;
		default:
			borWg = xlThin;
			break;
		}

		int nTy = nType;
		if( nTy < 1)
		{
			nTy = 1;
		}
		if( nType > 3)
		{
			nTy = 3;
		}
		BordersPtr pBorders = range->GetBorders();
		pBorders->GetItem(xlDiagonalDown)->LineStyle = xlNone;  //���Ϻ����¶Խ���
		pBorders->GetItem(xlDiagonalUp)->LineStyle = xlNone;    //���º����϶Խ���

		if( nTy == 1 || nTy == 3)
		{

			pBorders->GetItem(xlEdgeLeft)->LineStyle = xlContinuous;
			pBorders->GetItem(xlEdgeLeft)->Weight = borWg;
			pBorders->GetItem(xlEdgeLeft)->ColorIndex = xlColorIndexAutomatic;
			pBorders->GetItem(xlEdgeLeft)->Color = clr;

			pBorders->GetItem(xlEdgeRight)->LineStyle = xlContinuous;
			pBorders->GetItem(xlEdgeRight)->Weight = borWg;
			pBorders->GetItem(xlEdgeRight)->ColorIndex = xlColorIndexAutomatic;
			pBorders->GetItem(xlEdgeRight)->Color = clr;

			pBorders->GetItem(xlEdgeBottom)->LineStyle = xlContinuous;
			pBorders->GetItem(xlEdgeBottom)->Weight = borWg;
			pBorders->GetItem(xlEdgeBottom)->ColorIndex = xlColorIndexAutomatic;
			pBorders->GetItem(xlEdgeBottom)->Color = clr;

			pBorders->GetItem(xlEdgeTop)->LineStyle = xlContinuous;
			pBorders->GetItem(xlEdgeTop)->Weight = borWg;
			pBorders->GetItem(xlEdgeTop)->ColorIndex = xlColorIndexAutomatic;
			pBorders->GetItem(xlEdgeTop)->Color = clr;

		}
		if( nTy == 2 || nTy == 3)
		{
			pBorders->GetItem(xlInsideHorizontal)->LineStyle = xlContinuous;
			pBorders->GetItem(xlInsideHorizontal)->Weight = borWg;
			pBorders->GetItem(xlInsideHorizontal)->ColorIndex = xlColorIndexAutomatic;
			pBorders->GetItem(xlInsideHorizontal)->Color = clr;

			pBorders->GetItem(xlInsideVertical)->LineStyle = xlContinuous;
			pBorders->GetItem(xlInsideVertical)->Weight = borWg;
			pBorders->GetItem(xlInsideVertical)->ColorIndex = xlColorIndexAutomatic;
			pBorders->GetItem(xlInsideVertical)->Color = clr;
		}

		//}
		//catch(...)
		//{

		//}
	}

	void CExcel::Format_RowsHeight( _WorksheetPtr ptrSheet,int row1,int row2,float fHeight )
	{
		CMyStringW str1(L"A"),str2(L"A");
		str1 += row1;
		str2 += row2;
		RangePtr ptrRange = ptrSheet->Range[str1.GetData()][str2.GetData()];
		ptrRange->Select();
		ptrRange->PutRowHeight(fHeight);		
	}

	Excel::RangePtr CExcel::GetRangePtr( _WorksheetPtr ptrSheet,int col1,int row1,int col2,int row2 )
	{
		_bstr_t bstart = ConvertColRow(col1,row1);
		_bstr_t bend = ConvertColRow(col2,row2);
		RangePtr ptrRange = ptrSheet->Range[bstart][bend];
		return ptrRange;
	}

	void CExcel::Format_RangeAlignment( _WorksheetPtr ptrSheet,int col1,int row1,int col2,int row2,int nHoriAlign /*= 2*/,int nVertAlign /*= 2*/ )
	{
		RangePtr range = GetRangePtr(ptrSheet,col1,row1,col2,row2);
		range->Select();
		_variant_t hAlign = xlCenter;
		_variant_t vAlign = xlCenter;

		switch( nHoriAlign )
		{
		case 1:
			hAlign = xlLeft;
			break;
		case 3:
			hAlign = xlRight;
			break;
		default:
			break;
		}

		switch( nVertAlign )
		{
		case 1:
			vAlign = xlTop;
			break;
		case 3:
			vAlign = xlBottom;
			break;
		default:
			break;
		}

		range->HorizontalAlignment = hAlign;
		range->VerticalAlignment = vAlign;
	}

	void CExcel::Format_PutNumberFormat( _WorksheetPtr ptrSheet,int col1,int row1,int col2,int row2,const char* nFormat )
	{
		RangePtr ptrRange = GetRangePtr( ptrSheet ,col1,row1,col2,row2);
		ptrRange->Select();
		ptrRange->NumberFormatLocal = nFormat;
	}

	void CExcel::Format_PutBorderAround( _WorksheetPtr ptrSheet,int col1,int row1,int col2,int row2, 
		short sLineStyle, long lWeight, long lCorIndex, DWORD clr)
	{
		// �Ҳ��������ڲ��߿�İ취��ͨ��һ���������ⲿ�߿����ﵽ�ڲ��߿��ߵ�Ŀ��
		for(int row = row1; row <= row2; row++)
			for(int col = col1; col <= col2; col++)
			{
				RangePtr ptrRange = GetRangePtr( ptrSheet ,col,row,col,row);
				ptrRange->BorderAround(COleVariant(sLineStyle), Excel::XlBorderWeight(lWeight),Excel::XlColorIndex(lCorIndex), clr);
			}	
	}
	
	void CExcel::Format_PutAutoFit( _WorksheetPtr ptrSheet,int col1,int row1,int col2,int row2, BOOL bCol/* = TRUE*/)
	{
		RangePtr ptrRange = GetRangePtr( ptrSheet ,col1,row1,col2,row2);
		ptrRange->Select();
		if(bCol)
		{
			ptrRange->GetColumns()->AutoFit();
		}
		else
		{
			ptrRange->GetRows()->AutoFit();
		}
		
	}

	void CExcel::Format_PutCellText( _WorksheetPtr ptrSheet,int col,int row,const WCHAR * wszText )
	{
		ptrSheet->Activate();
		RangePtr ptrRange = GetRangePtr( ptrSheet,col,row,col,row);
		ptrRange->Select();
		_bstr_t br = wszText;
		ptrRange->Value2 = br;   //���õ�Ԫ���ı�	
	}

	void CExcel::Format_Frozen( _WorksheetPtr ptrSheet,int col,int row )
	{
		GetRangePtr(ptrSheet,col,row,col,row)->Select();
		ptrSheet->GetApplication()->GetActiveWindow()->FreezePanes = TRUE;
	}

	Excel::_WorkbookPtr CExcel::GetWorkBook()
	{
		return m_ptrBook;
	}

	void CExcel::MoveSheet( const WCHAR *lpszSrcSheetName,const WCHAR *lpszDesSheetName,BOOL bBefore /*= TRUE*/ )
	{
		_WorksheetPtr ptrSrcSheet = m_ptrSheets->GetItem(lpszSrcSheetName);
		_ChartPtr ptrSrcChart = m_ptrSheets->GetItem( lpszSrcSheetName );
		_WorksheetPtr ptrDesSheet = m_ptrSheets->GetItem( lpszDesSheetName );
		_ChartPtr ptrDesChart = m_ptrSheets->GetItem( lpszDesSheetName );
		VARIANT var;
		var.vt = VT_DISPATCH;
		if( ptrDesSheet != NULL )
		{
			var.pdispVal = ptrDesSheet;
		}
		else if( ptrDesChart != NULL )
		{
			var.pdispVal = ptrDesChart;
		}
		else
		{
			return ;
		}

		if( ptrSrcSheet != NULL )
		{
			ptrSrcSheet->Select();
			if( bBefore )
			{
				ptrSrcSheet->Move( var);
			}
			else
			{
				ptrSrcSheet->Move( vtMissing,var );
			}
		}
		else if( ptrSrcChart != NULL )
		{
			ptrSrcChart->Select();
			if( bBefore )
			{
				ptrSrcChart->Move( var );
			}
			else
			{
				ptrSrcChart->Move( vtMissing,var );
			}
		}
	}

	void CExcel::MoveSheet( const WCHAR *lpszSrcSheetName,const int nDesSheetItem,BOOL bBefore /*= TRUE*/ )
	{
		_WorksheetPtr ptrSrcSheet = m_ptrSheets->GetItem(lpszSrcSheetName);
		_ChartPtr ptrSrcChart = m_ptrSheets->GetItem( lpszSrcSheetName );
		_WorksheetPtr ptrDesSheet = m_ptrSheets->GetItem( nDesSheetItem );
		_ChartPtr ptrDesChart = m_ptrSheets->GetItem( nDesSheetItem );
		VARIANT var;
		var.vt = VT_DISPATCH;
		if( ptrDesSheet != NULL )
		{
			var.pdispVal = ptrDesSheet;
		}
		else if( ptrDesChart != NULL )
		{
			var.pdispVal = ptrDesChart;
		}
		else
		{
			return ;
		}
		
		if( ptrSrcSheet != NULL )
		{
			ptrSrcSheet->Select();
			if( bBefore )
			{
				ptrSrcSheet->Move( var);
			}
			else
			{
				ptrSrcSheet->Move( vtMissing,var );
			}
		}
		else if( ptrSrcChart != NULL )
		{
			ptrSrcChart->Select();
			if( bBefore )
			{
				ptrSrcChart->Move( var );
			}
			else
			{
				ptrSrcChart->Move( vtMissing,var );
			}
		}
	}

	BOOL NS_GXX::CExcel::EmptyExcelClipboard()
	{
		//��ȡExcel�����ھ��
		long nExcel = m_ptrApp->GetHwnd();

		//�����񴰸�
		/*PostMessage((HWND)nExcel, WM_KEYDOWN, VK_CONTROL, 0);
		PostMessage((HWND)nExcel, WM_KEYDOWN, VK_F1, 0);
		PostMessage((HWND)nExcel, WM_KEYUP, VK_F1, 0);
		PostMessage((HWND)nExcel, WM_KEYUP, VK_CONTROL, 0);*/

		try
		{
			Office::CommandBarPtr pCmdBar = NULL;
			pCmdBar = m_ptrApp->GetCommandBars()->GetItem(1);
			if (pCmdBar == NULL)
			{
				//ExcelTrace("EmptyExcelClipboard Fails!");
				return FALSE;
			}
			//�ҵ�������
			Office::CommandBarControlPtr pCmdBarCtl = NULL;
			pCmdBarCtl = pCmdBar->FindControl(vtMissing, 809, 
				vtMissing, vtMissing, TRUE);
			if (pCmdBarCtl == NULL)
			{
				//ExcelTrace("EmptyExcelClipboard Fails!");
				return FALSE;
			}
			//�򿪼�����
			pCmdBarCtl->Execute();

			_bstr_t btName = m_ptrApp->GetCommandBars()
				->GetItem("Task Pane")->GetNameLocal();
			m_ptrApp->GetCommandBars()->GetItem("Task Pane")
				->PutVisible(VARIANT_FALSE);

			//��������
			HWND hClipborad = NULL;
			//��ʱ���ھ��
			HWND hWindow = NULL;
			hWindow = FindWindowEx((HWND)nExcel, NULL, 
				L"MsoWorkPane", L"MsoWorkPane");
			hClipborad = FindWindowEx(hWindow, NULL, 
				L"bosa_sdm_XL9", L"Collect and Paste 2.0");

			if (hClipborad == NULL)
			{
				//ExcelTrace("Get Clipboard HWnd Error!");
				return FALSE;
			}
			//����ģ�������Ϣ  (x,y)=(100,10),ȡ(92,6)~(168,27)����
			int nXY = 100 + 10 * 65536;  
			PostMessage(hClipborad, WM_LBUTTONDOWN, 0, nXY);
			PostMessage(hClipborad, WM_LBUTTONUP, 0, nXY);
		}
		catch (_com_error& e)
		{
			//ExcelTrace((char*)e.Description());
			return FALSE;
		}

		//ExcelTrace("EmptyClipboard success!");
		return TRUE;
	}


	ShapeRangePtr NS_GXX::CExcel::Format_MoveShapeToCell( _WorksheetPtr ptrSheet,int nShapeIndex,int col1,int row1 ,OUT float *pfWidth /*= NULL*/ , OUT float *pfHeight /*= NULL*/)
	{
		try
		{
			ShapeRangePtr ptrShape = ptrSheet->Shapes->GetRange(nShapeIndex);

			ptrShape->Select();
			ptrShape->IncrementLeft( -3000.0);
			ptrShape->IncrementTop( -3000.0);

			float nWidth = 0.0;
			float nHeight = 0.0;

			if( col1 > 1 )
			{
				RangePtr pRange = GetRangePtr( ptrSheet , 1,1,col1-1,1 );
				nWidth = pRange->GetWidth();
			}
			if( row1 > 1)
			{
				RangePtr pRange = GetRangePtr( ptrSheet , 1,1,1,row1-1 );
				nHeight = pRange->GetHeight();
			}
			ptrShape->Select();
			ptrShape->IncrementLeft( nWidth );
			ptrShape->IncrementTop( nHeight );

			if( pfWidth != NULL )
			{
				*pfWidth = nWidth;
			}
			if( pfHeight != NULL )
			{
				*pfHeight = nHeight;
			}

			return ptrShape;
		}
		catch (_com_error &e)
		{
			ASSERT(0);
			e.ErrorMessage();
			return NULL;
		}  
	}
	

	Excel::ShapeRangePtr NS_GXX::CExcel::Format_MoveShape( _WorksheetPtr ptrSheet,int nShapeIndex,IN float fWidth,IN float fHeight )
	{
		try
		{
			ShapeRangePtr ptrShape = ptrSheet->Shapes->GetRange(nShapeIndex);

			ptrShape->Select();
			ptrShape->IncrementLeft( -3000.0);
			ptrShape->IncrementTop( -3000.0);
			
			ptrShape->IncrementLeft( fWidth );
			ptrShape->IncrementTop( fHeight );


			return ptrShape;
		}
		catch (_com_error &e)
		{
			ASSERT(0);
			e.ErrorMessage();
			return NULL;
		}  
	}


	void NS_GXX::CExcel::Format_RangeMerge( _WorksheetPtr ptrSheet,int col1,int row1,int col2,int row2 )
	{
		RangePtr ptrRange = GetRangePtr(ptrSheet,col1,row1,col2,row2);
		ptrRange->Select();
		ptrRange->Merge();
	}
////--------------------------------------------------------------------------------
//// CExcelExport
////--------------------------------------------------------------------------------
    //
    CExcelExport::CExcelExport()
    {
        m_hGlobalMem = GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,MEM_SIZE+1); //+1��Ϊ�� *m_pMem = '\0';���ᷢ������
        m_pMem = (char*)GlobalLock(m_hGlobalMem);
        memset(m_pMem,0,MEM_SIZE);
        m_nUsedMem = 0;
        m_iUsedLines = 0;
        m_iMemLines = 0;
        m_iMaxColumns = 1;
        m_bFirstInsertData = TRUE;
    }
    CExcelExport::~CExcelExport()
    {
		//������ʣ��û�е���������
        Flush();
        if(m_hGlobalMem )
        {
            GlobalUnlock(m_hGlobalMem);
            GlobalFree(m_hGlobalMem);
        }
    }
    BOOL CExcelExport::InsertSheet(int iIndex, const TCHAR * lpszSheetName,BOOL bAtBefore /* = TRUE */ )
    {
        BOOL bRel = CExcel::InsertSheet(iIndex,lpszSheetName,bAtBefore);
        if( bRel )
        {
            UpdateLines();
        }
        return bRel;
    }
    BOOL CExcelExport::InsertSheet(const TCHAR *lpszSheetName)
    {
        BOOL bRel = CExcel::InsertSheet(lpszSheetName);
        if( bRel )
        {
            UpdateLines();
        }
        return bRel;
    }
    BOOL CExcelExport::SetActiveSheet(TCHAR * lpszSheetName)
    {
        BOOL bRel = CExcel::SetActiveSheet(lpszSheetName);
        if( bRel )
        {
            UpdateLines();
        }
        return bRel;
    }
    BOOL CExcelExport::SetActiveSheet(int iIndex)
    {
        BOOL bRel = CExcel::SetActiveSheet(iIndex);
        if( bRel )
        {
            UpdateLines();
        }
        return bRel;
    }
    void CExcelExport::AppendData(char *lpszData)
    {
        if( m_bFirstInsertData )
        {
            m_bFirstInsertData = FALSE;
            if( m_iUsedLines == MAX_LINE )   //�״β���ʱ�жϲ���λ���Ƿ�Ϊ65536
            {
                //�ǣ��ù������Ѿ������ˣ���Ҫ����һ���µĹ�����
                InsertAppendSheet();
            }
        }

		static char * lpEmpty = "\n";
        int nMaxColumn = 1;
        char *pWriteBuf = NULL;     
        size_t nBufLen = 0;
        int nCurLine = 0;
		bool bN = false;
        if( lpszData == NULL || lpszData[0] == '\0')
        {
            pWriteBuf = lpEmpty;
            nBufLen = 1;
			bN = true;
        }
        else
        {
            size_t nLen = strlen(lpszData);
			pWriteBuf = lpszData;
			if( pWriteBuf[nLen-1] == '\n' )
			{
				bN = true;
			}
			nBufLen = nLen;

        }
        for(int i = 0 ; i < nBufLen ; i ++)
        {
            if( pWriteBuf[i] == '\t' )
            {
                nMaxColumn++;
            }
            if( pWriteBuf[i] == '\n')
            {
                if( nMaxColumn > m_iMaxColumns)
                {
                    m_iMaxColumns = nMaxColumn;
                    nMaxColumn = 1;
                }
                nCurLine ++;
            }
        }//end for
		if( !bN )
		{
			nBufLen += 1;
			nMaxColumn ++;
			if( nMaxColumn > m_iMaxColumns)
			{
				m_iMaxColumns = nMaxColumn;
			}
			nCurLine ++;
		}

        //д��֮ǰ��Ҫ�жϵ�ǰ�����������Ƿ�Ҫ���� 65536
        if( nCurLine + m_iUsedLines + m_iMemLines > MAX_LINE )
        {
            //����
            Flush();         //���ڴ�����д��Excel�����Ҵ���һ���µĹ�����
            InsertAppendSheet();
        }
        //д���ڴ�֮ǰ�ж��ڴ�ռ乻����
        if( m_nUsedMem + nBufLen > MEM_SIZE )
        {
            //����
            Flush();     //���ڴ�����д��Excel
        }
        //����д���ڴ���
        m_nUsedMem += nBufLen;
        m_iMemLines += nCurLine;
        memcpy(m_pMem,pWriteBuf,nBufLen-1);
		m_pMem[nBufLen-1] = '\n';
        m_pMem += nBufLen;
    }
	void CExcelExport::AppendData(char *lpszData,int nLineCount,int nColumnCount)
	{
		if( m_bFirstInsertData )
		{
			m_bFirstInsertData = FALSE;
			if( m_iUsedLines == MAX_LINE )   //�״β���ʱ�жϲ���λ���Ƿ�Ϊ65536
			{
				//�ǣ��ù������Ѿ������ˣ���Ҫ����һ���µĹ�����
				InsertAppendSheet();
			}
		}
		static char * lpEmpty = "\n";
		int nMaxColumn = 1;
		char *pWriteBuf = NULL;     
		size_t nBufLen = 0;
		int nCurLine = 0;
		bool bN = false;
		if( lpszData == NULL || lpszData[0] == '\0')
		{
			pWriteBuf = lpEmpty;
			nBufLen = 1;
			bN = true;
		}
		else
		{
			size_t nLen = strlen(lpszData);
			pWriteBuf = lpszData;
			if( pWriteBuf[nLen-1] == '\n' )
			{
				bN = true;
			}
			nBufLen = nLen;

		}
		if( !bN )
		{
			nBufLen += 1;
		}

		if( nColumnCount > m_iMaxColumns )
		{
			m_iMaxColumns = nColumnCount;
		}
		nCurLine = nLineCount;

		//д��֮ǰ��Ҫ�жϵ�ǰ�����������Ƿ�Ҫ���� 65536
		if( nCurLine + m_iUsedLines + m_iMemLines > MAX_LINE )
		{
			//����
			Flush();         //���ڴ�����д��Excel�����Ҵ���һ���µĹ�����
			InsertAppendSheet();
		}
		//д���ڴ�֮ǰ�ж��ڴ�ռ乻����
		if( m_nUsedMem + nBufLen > MEM_SIZE )
		{
			//����
			Flush();     //���ڴ�����д��Excel
		}
		//����д���ڴ���
		m_nUsedMem += nBufLen;
		m_iMemLines += nCurLine;
		memcpy(m_pMem,pWriteBuf,nBufLen-1);
		m_pMem[nBufLen-1] = '\n';
		m_pMem += nBufLen;
	}
    void CExcelExport::InsertAppendSheet(TCHAR * lpszNewName /*= NULL*/)
    {
		//ExcelTrace(L"�Զ����빤����:%s",lpszNewName);
        try
        {
			_WorksheetPtr pSheet = m_ptrBook->GetActiveSheet();
			m_iUsedLines = 0;  //���¼ӵĹ�����Ϊ�������ʹ�õ�����Ҫ����
			m_iMaxColumns = 1;
			_bstr_t bName ;
			int nMaxIndex = pSheet->GetIndex();  //��ǰ�����������ƹ����������ֵ
			if( lpszNewName != NULL && lpszNewName[0] != L'\0' )
			{
				bName = lpszNewName;
			}
			else
			{
				


				bName = pSheet->GetName();
				CMyStringW strOrgName((WCHAR*)bName);

				//�õ�����ǰ׺��ͬ�ĸ���
				CMyStringW strNumCount = strOrgName.Clip( strOrgName.FindFromRight(L"__",strOrgName.GetLength() - 1) +2, strOrgName.GetLength() );

				int nCount = strNumCount.ParseInt();

				if( nCount == 0 )
				{   
					//�����ñ� Ϊ ͬ��ǰ׺ ��ĵڶ������ѵ�һ����������
					CMyStringW strNewName = strOrgName;
					strNewName += L"__1";
					this->SetSheetName( this->GetSheet(strOrgName)->GetIndex(),strNewName);
					nCount ++;
				}
				strOrgName.CutAway( strOrgName.FindFromRight(L"__",strOrgName.GetLength()-1 ));
				
				//�����±������
				CMyStringW strNewName = strOrgName;
				strNewName.AppendFormat(L"__%d",nCount + 1);


				//�õ�ԭʼ����
				
				int nSheetCount = m_ptrSheets->GetCount();

				//�����±�Ӧ�÷������
				for(int i = 1 ; i <= nSheetCount ; i ++)
				{
					_WorksheetPtr ptrSheet = m_ptrSheets->GetItem(i);
					bName = ptrSheet->GetName();
					CMyStringW strTemp((WCHAR*)bName);
					strTemp.CutAway( strTemp.FindFromRight(L"__",strTemp.GetLength()-1 ));
					if( strTemp == strOrgName )
					{
						nMaxIndex = i;  //ѭ�������󣬵õ��ľ����������ֵ
					}
				}
				
				bName = strNewName.GetData();
			}
			InsertSheet(nMaxIndex,(wchar_t*)bName,FALSE);
        }
         catch(_com_error &err)
         {
            ASSERT(MyErrorMsg(err));
         }  
    }
#ifndef _WINDOWS_
	void NS_GXX::CExcelExport::InsertOneBmp( IN CBitmap& bmp,int iCol /*= 0*/, int iRow /*= 0*/ )
	{
		AppendData();  //�Ȳ���һ���հ���
		Flush();
		
		BITMAP  bm ;
		bmp.GetBitmap(&bm);								

		if( iRow == 0 && iCol == 0 )
		{ //��Ĭ�ϴ���
			
			int nLineWidth = (int)GetRangePtr(GetActiveSheet(),1,m_iUsedLines,1,m_iUsedLines)->GetRowHeight();
			nLineWidth = int( ((float)nLineWidth)*19.0/14.0);
			//��������

			int nRows = bm.bmHeight / nLineWidth + 1;
			
			
			if( nRows + m_iUsedLines > MAX_LINE )
			{
				_bstr_t brSheet = GetActiveSheet()->GetName();
				InsertSheet( (WCHAR*)brSheet );
			}
			for(int i = 0 ; i < nRows ; i ++ )
			{
				AppendData();
			}
			Flush();

			OpenClipboard(NULL);								// �򿪼�����
			EmptyClipboard();									// ���ǰ����
			SetClipboardData(CF_BITMAP,bmp.GetSafeHandle());	// ��λͼ���õ���������	
			CloseClipboard();                                   // �ر�ǰ����	

			RangePtr ptrRange;
			ptrRange = GetRangePtr( GetActiveSheet(),1,m_iUsedLines - nRows,1,m_iUsedLines - nRows);
			VARIANT var;
			var.vt = VT_DISPATCH;
			var.pdispVal = ptrRange;
			ptrRange->Select();
			GetActiveSheet()->Paste(var); 
			
		}
		else
		{
			_WorksheetPtr ptrSheet = GetActiveSheet();
			RangePtr ptrRange = GetRangePtr( ptrSheet,iCol,iRow,iCol,iRow);

			OpenClipboard(NULL);								// �򿪼�����
			EmptyClipboard();									// ���ǰ����
			SetClipboardData(CF_BITMAP,bmp.GetSafeHandle());	// ��λͼ���õ���������	
			CloseClipboard(); 

			VARIANT var;
			var.vt = VT_DISPATCH;
			var.pdispVal = ptrRange;
			ptrRange->Select();
			ptrSheet->Paste(var); 
		}

		//wchar_t wcsRow[16];
		//_bstr_t bstrFirst,bstrTemp1;
		//wsprintf(wcsRow,L"%d",m_dwUsedLine + 1);
		//bstrTemp1 = SysAllocString(wcsRow);
		//bstrFirst = L"A" + bstrTemp1;

		//RangePtr range = pSheet->Range[bstrFirst][vtMissing];
		//VARIANT var;
		//var.vt = VT_DISPATCH;
		//var.pdispVal = range;		
		//range->Select();
		//pSheet->Paste(var);


		//GetRangePtr( GetActiveSheet(), )
	}
#endif
	void CExcelExport::Flush(BOOL bNewSheet)
    {
		//ExcelTrace("����д�빤����");
        if(m_pMem == NULL || m_iMemLines == 0)
        {
            return ;
        }

        //���ַ����Ľ�β��ӽ������
        *m_pMem = '\0';

        //�򿪼��а�
        if(!OpenClipboard(NULL))
        {
            return ;
        }
        //��ռ��а�
        EmptyClipboard();

        //����Զ�ջ������
        GlobalUnlock(m_hGlobalMem);

        //���ü��а�
        SetClipboardData(CF_TEXT,m_hGlobalMem);

        //�ͷŶ�ջ�ռ�
        //	GlobalFree(m_hGlobalMem);			//���а���Զ��ͷŴ˿ռ�

        //�رռ��а�
        CloseClipboard();

        //ճ�����ݵ�EXCEL
        _bstr_t first,last;
      
        char buf[6];
        wsprintfA(buf,"%d",m_iUsedLines+1);
        first = "A";
        first += buf;
        wsprintfA(buf,"%d",m_iUsedLines+m_iMemLines);
        _bstr_t col = ConvertCol(m_iMaxColumns+1);
        last = col;
        last += buf;
        //ѡ��д�����ݵĹ�����
        
		try
		{_WorksheetPtr pSheet;
		pSheet = m_ptrBook->GetActiveSheet();
		RangePtr range = pSheet->Range[first][last];
		VARIANT var;
		var.vt = VT_DISPATCH;
		var.pdispVal = range;		
		range->Select();
			pSheet->Paste(var);
		}
		catch (_com_error &err)
		{
			ASSERT(MyErrorMsg(err));
		}
        

        //��ԭ����ʼ״̬

        //�����ջ
        m_hGlobalMem  = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,MEM_SIZE+1);

        //���õ�ǰд�����ݵĴ�С
        m_nUsedMem = 0;

        //����д���ڴ����ݵ�����
        m_iUsedLines += m_iMemLines;
        m_iMemLines = 0;

        //��ö�ջ��ָ��
        m_pMem  = (char*)GlobalLock(m_hGlobalMem);    
        memset(m_pMem,0,MEM_SIZE+1);   
    }
    void CExcelExport::UpdateLines()
    {
        Flush();
        m_iUsedLines = 0 ;
        _WorksheetPtr ptrSheet = m_ptrBook->GetActiveSheet();
        int Gap = 100;
        RangePtr ran = ptrSheet->UsedRange;
        int count = ran->Rows->Count;                    //�õ�ʹ���˵�����
        if( count == 1)  //�հ׵Ĺ����� ���� ֻ��һ�����ݵĹ�����
        {
            _bstr_t first = "A1";
            _bstr_t last = "IV1";
            RangePtr ran= ptrSheet->Range[first][last];
            ran->Select();
            ran->Copy();
            if( OpenClipboard(NULL) )
            {
                HANDLE hMem = GetClipboardData(CF_TEXT);
                char *pMem = (char*)GlobalLock(hMem);
                size_t nLen = strlen(pMem);
                if( strlen(pMem) == 2 )  //�հ׹�����
                {
                    count =0;
                }
                CloseClipboard();
            }
        }
        m_iUsedLines = count;
        m_pMem =(char*) GlobalLock(m_hGlobalMem);
        m_iMemLines = 0;
        m_iMaxColumns = 0;
        m_nUsedMem = 0;
        m_bFirstInsertData = TRUE;            //����Ϊ�״β���
    }	

	void CExcelExport::PasteText( const char *lpszData,int iCol1,int iRow1,int iCol2,int iRow2 )
	{
		try
		{
			CClipBoardTextBuffer clipB;
			if( !clipB.Lock() )
			{
				return;			
			}
			clipB.AppendData(lpszData);
			clipB.UnLock();

			RangePtr ptrRange;
			ptrRange = GetRangePtr( GetActiveSheet(),iCol1,iRow1,iCol2,iRow2);
			VARIANT var;
			var.vt = VT_DISPATCH;
			var.pdispVal = ptrRange;
			ptrRange->Select();
			GetActiveSheet()->Paste(var); 
		}
		catch (_com_error &err)
		{
			ASSERT(MyErrorMsg(err));
		}
		//catch (...)
		//{
		//	// GXX_TRACE("ճ���ı�ʧ��");
		//}
	}

////-----------------------------------------------------------------
//// CExcelExportEx
////-----------------------------------------------------------------

	CExcelExportEx::CExcelExportEx()
	{
		for(DWORD i=0; i <BLOCK_COUNT; i++)
		{
			m_blocks[i] = new CHAR[BLOCK_SIZE+1];
			if( m_blocks[i] == NULL )
			{
				//ExcelTrace2("�ڴ治��");
				ASSERT(0);
			}
			m_status[i] = S_Idle;
			m_usedLength[i] = 0;
			m_blockMaxCols[i] = 0;
		}
		m_iCurrentBlock = 0;
		m_status[m_iCurrentBlock] = S_Write;

		m_hEventCanRead = ::CreateEvent(NULL,TRUE,FALSE,NULL);
		m_hEventHaveIdleBlock = ::CreateEvent(NULL,TRUE,FALSE,NULL);
		m_hEventQueueEmpty = ::CreateEvent(NULL,TRUE,FALSE,NULL);
		m_hEventThreadFinish = ::CreateEvent(NULL,TRUE,FALSE,NULL);

		ResetEvent(m_hEventThreadFinish);
		m_bExit = false;
		SetEvent(m_hEventQueueEmpty);

		AllowIdleBlock();
		HoldCanRead();

		m_pWinThread = AfxBeginThread(_ThreadEntry ,this);

	}

	CExcelExportEx::~CExcelExportEx()
	{

		
		m_bExit = true;
		AllowCanRead();
		WaitForSingleObject(m_hEventThreadFinish,INFINITE);

		if( m_hEventCanRead != INVALID_HANDLE_VALUE )
		{
			CloseHandle(m_hEventCanRead);
		}
		if( m_hEventHaveIdleBlock != INVALID_HANDLE_VALUE )
		{
			CloseHandle(m_hEventHaveIdleBlock);
		}
		if( m_hEventThreadFinish != INVALID_HANDLE_VALUE )
		{
			CloseHandle(m_hEventThreadFinish);
		}
		
		if( m_hEventQueueEmpty != INVALID_HANDLE_VALUE )
		{
			CloseHandle(m_hEventQueueEmpty);
		}

		for(DWORD i=0; i < BLOCK_COUNT ; i ++)
		{
			if(m_blocks[i])
			{

				delete m_blocks[i];
			}
		}
	}

	void CExcelExportEx::AppendData( char *lpszData /*= NULL*/ )
	{
		static char *emptyBuf = "\n";

		int nUsedLen = m_usedLength[m_iCurrentBlock];

		ASSERT(m_status[m_iCurrentBlock]==S_Write);
		
		char *pWirteBuf = NULL;
		bool bHaveN = false;
		size_t nLen =0;
		if(lpszData == NULL || lpszData[0] == '\0')
		{
			pWirteBuf = emptyBuf;
			bHaveN = true;
			nLen = 1;
		}
		else
		{
			nLen = strlen(lpszData);
			pWirteBuf = lpszData;
			if(lpszData[nLen-1] == '\n')
				bHaveN = true;
		}
		
		int nWriteLen = nLen + (bHaveN?0:1);
		if( nUsedLen + nWriteLen > BLOCK_SIZE)
		{
			// ��ǰ���Ѿ�д��,��ʶΪ�ɶ�
			m_cs.Lock();
			m_status[m_iCurrentBlock] = S_CanRead;
			m_queueCanRead.enqueue(m_iCurrentBlock); //�Ž�"�ܶ�����"
			m_cs.Unlock();

			AllowCanRead();

			int iIdle = -1;
			//�ҵ�һ�����еĿ�
			m_cs.Lock();
			for(DWORD iBlock =0 ; iBlock <BLOCK_COUNT; iBlock ++)
			{
				if( iBlock != m_iCurrentBlock && m_status[iBlock] == S_Idle )
				{
					iIdle = iBlock;
					break;
				}
			}
			m_cs.Unlock();

			if( iIdle == -1 )
			{	
				HoldIdleBlock();
				//�ȵ��п��п�
				WaitIdleBlock();

				for(DWORD iBlock =0 ; iBlock <BLOCK_COUNT; iBlock ++)
				{
					if( iBlock != m_iCurrentBlock && m_status[iBlock] == S_Idle )
					{
						iIdle = iBlock;
					}
				}
				m_cs.Unlock();
			}

			ASSERT(iIdle!=-1);

			if( iIdle == -1 )
			{
				return;
			}

			//�ҵ����п�
			m_cs.Lock();
			m_iCurrentBlock = iIdle;  //����Ϊ��ǰ��
			m_usedLength[ iIdle ] = 0;  //ʹ�ó���Ϊ0
			m_status[ iIdle ] = S_Write; //��ʶ״̬Ϊд��
			m_cs.Unlock();	

			CExcelExportEx::AppendData(pWirteBuf);
		}
		else
		{	//��ǰ�����㹻�Ŀռ�д����
			char *pBlock = m_blocks[m_iCurrentBlock];
			memcpy(&pBlock[nUsedLen],pWirteBuf,nLen);
			nUsedLen += nWriteLen;
			pBlock[nUsedLen-1] = '\n';

			m_cs.Lock();
			m_usedLength[m_iCurrentBlock] = nUsedLen;
			m_cs.Unlock();
		}
	}

	void CExcelExportEx::AppendData( char *lpszData,int nLineCount,int nColumnCount )
	{

	}

	void CExcelExportEx::WaitCanRead()
	{
		WaitForSingleObject(m_hEventCanRead,INFINITE);
	}

	void CExcelExportEx::HoldCanRead()
	{
		ResetEvent(m_hEventCanRead);
	}

	void CExcelExportEx::AllowCanRead()
	{
		SetEvent(m_hEventCanRead);
	}
	void CExcelExportEx::HoldIdleBlock()
	{
		ResetEvent(m_hEventHaveIdleBlock);
	}

	void CExcelExportEx::AllowIdleBlock()
	{
		SetEvent(m_hEventHaveIdleBlock);
	}

	void CExcelExportEx::WaitIdleBlock()
	{
		WaitForSingleObject(m_hEventHaveIdleBlock,INFINITE);
	}

	UINT CExcelExportEx::_ThreadEntry( LPVOID pParam)
	{
		::CoInitialize(NULL);

		CExcelExportEx *pEE = (CExcelExportEx *)pParam;
		
		while(1)
		{
			pEE->WaitCanRead();

			if( pEE->m_bExit )
			{
				SetEvent( pEE->m_hEventThreadFinish );
				::CoUninitialize();
				return 0;
			}

			pEE->m_cs.Lock();
			if( pEE->m_queueCanRead.size() == 0 )
			{
				pEE->HoldCanRead(); // ���̵߳ȴ�
				SetEvent(pEE->m_hEventQueueEmpty);  //����Ϊ��
				pEE->m_cs.Unlock(); 
				continue;
			}
			pEE->m_cs.Unlock();

			pEE->m_cs.Lock();
			int iBlock = pEE->m_queueCanRead.dequeue();
			ASSERT( pEE->m_status[iBlock] == S_CanRead);
			CHAR* pBlock = pEE->m_blocks[iBlock];
			int nLen = pEE->m_usedLength[iBlock];
			pEE->m_cs.Unlock();

			CHAR* pAt = pBlock;
			CHAR* pIt = pBlock;
			
			CHAR tempChar;
			for(int i = 0 ; i < nLen ; i ++)
			{
				if( (pIt[i]) == '\n')
				{
					tempChar = pIt[i+1];
					pIt[i+1] = '\0';
					((CExcelExport*)pEE)->AppendData(pAt);
					pIt[i+1] = tempChar;
					pAt = &pIt[i+1];
				}
			}
			
			pEE->m_cs.Lock();
			pEE->m_status[iBlock] = S_Idle;
			pEE->m_usedLength[iBlock] = 0;
			pEE->m_cs.Unlock();
			pEE->AllowIdleBlock();
			
		}
		::CoUninitialize();
		return 0;
	}

	void CExcelExportEx::Flush()
	{
		//
		m_cs.Lock();
		bool bWrite = (m_status[m_iCurrentBlock] == S_Write);
		m_cs.Unlock();
		
		if( bWrite )
		{
			m_cs.Lock();
			m_status[m_iCurrentBlock] = S_CanRead;
			m_queueCanRead.enqueue(m_iCurrentBlock);
			m_cs.Unlock();
			
			m_cs.Lock();
			ResetEvent(m_hEventQueueEmpty); // ����Ϊ�ǿ�
			m_cs.Unlock();
		}

		AllowCanRead();  // �����̶߳�ȡ����
		WaitForSingleObject(m_hEventQueueEmpty,INFINITE); //�ȴ�����Ϊ��

		for(DWORD iBlock =0 ; iBlock <BLOCK_COUNT; iBlock ++)
		{
			m_status[iBlock] = S_Idle;
			m_usedLength[iBlock] = 0;
		}

		m_iCurrentBlock = 0;
		m_status[m_iCurrentBlock] = S_Write;
		m_usedLength[m_iCurrentBlock] = 0;

		CExcelExport::Flush();
		
	}
}
