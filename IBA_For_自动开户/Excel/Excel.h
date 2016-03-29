#pragma once
#include <shlwapi.h>


///{{ ����Excel COM���
#define OFFICEXP 1
#define OFFICE2000 2

// Default Settings
#define OFFICE_VER OFFICEXP            //ʹ��OFFICEXP���ں�
#define USE_PROGID 1
#define USE_LIBID 0

#define _M2STR(x) #x
#define M2STR(x) _M2STR(x)

#pragma warning(disable: 4192)


// Ignore hard wired default paths if MSDLL_PATH is
// defined from the command line
#ifndef MSDLL_PATH

// Paths to required MS OFFICE files.
// Make sure these paths are correct for your  machine

#if OFFICE_VER == OFFICEXP
#define _MSDLL_PATH "C:\Program Files\Common Files\Microsoft Shared\Office11\MSO.DLL"

#elif  OFFICE_VER == OFFICE2000

#define   _MSDLL_PATH "C:\Program Files\Microsoft Office\Office\MSO9.dll"

#endif

#else

#define _MSDLL_PATH M2STR(MSDLL_PATH)

#endif

//
// Delete the *.tlh files when changing import qualifiers
#import _MSDLL_PATH rename("RGB", "NWRGB") rename("DocumentProperties","NWDocumentProperties")

#ifdef VBE6EXT_PATH
#import M2STR(VBE6EXT_PATH) 
#else
#import "C:\Program Files\Common Files\Microsoft Shared\VBA\VBA6\VBE6EXT.OLB"
#endif

#if USE_PROGID
#import "progid:Excel.Sheet" auto_search auto_rename rename_search_namespace("Office10") 

#elif USE_LIBID
#import "libid:{00020813-0000-0000-C000-000000000046}" auto_search auto_rename version(1.3) lcid(0) no_search_namespace
#else

#ifndef MSEXCEL_PATH

#if  OFFICE_VER == OFFICEXP
#define   _MSEXCEL_PATH "C:\Program Files\Microsoft Office\Office11\excel.exe"
//#define   _MSEXCEL_PATH "C:\Program Files (x86)\Microsoft Office\Office14\excel.exe"
#elif  OFFICE_VER == OFFICE2000
#define   _MSEXCEL_PATH "C:\Program Files\Microsoft Office\Office\excel.exe"
#endif

#else
#define _MSEXCEL_PATH  M2STR(MSEXCEL_PATH) 
#endif

#import _MSEXCEL_PATH auto_search auto_rename dual_interfaces
#endif // USE_LIBID

///}}

/*
*	CExcel�Ļ�������,�������������򿪹����������湤���������빤�����������
*	CExcelExport: �̳���CExcel�������ݵ�Excel,һ�Ź��������ݲ������Զ�����һ���µĹ�����
*      ���ݵĲ�������Ե�ǰ����Ĺ�����.
*   CExcelExport::AppendData();�������ַ�����\t��ʾ����\n��ʾ����,����ַ���ĩβû��\n
	  AppendData���Զ���ӻ���
*
*Remark:���еķ����漰���Ĳ���, �е�ȡֵ��ΧΪ�� _A_~_IV_ ), �е�ȡֵ��ΧΪ ( 1~65536), ������ >= 1
*       �������ͼ������sheet
*Author:guoxuxing
*Date:2009-10-20
*
*copyright:guoxuxing
*/
#pragma warning(disable:4101)

using namespace Excel;
#include "gxx_base.h"
#include "queue.h"
#include "CriticalSection.h"

class CBitmap;

#ifdef _WINDOWS_
#undef _WINDOWS_
#endif

namespace NS_GXX
{
    //CExcel
    //Excel�Ĵ򿪣����棬�رգ����빤����
    class CExcel:CComBase
    {
    public:
        CExcel(void);
        ~CExcel(void);
    protected:
        _ApplicationPtr m_ptrApp;                   //һ��Ӧ�ó�������ָ��,��Ӧһ������������
        _ApplicationPtr m_ptrApp_tmp;               //��ʱ���̣����û�ʹ�ã���ֹ�û����ŵ�ǰ��������
        WorkbooksPtr m_ptrBooks;                    //����������
        _WorkbookPtr m_ptrBook;                     //һ��������
        SheetsPtr       m_ptrSheets;                //һ����������Ӧһ����������
	
	public:
		//
		// EXCEL: ���֧�� A~IV ��,���ƣ�256
		//
		enum emColumn
		{
			_A_=1,_B_,_C_,_D_,_E_,_F_,_G_,_H_,_I_,_J_,_K_,_L_,_M_,_N_,_O_,_P_,_Q_,_R_,_S_,_T_,_U_,_V_,_W_,_X_,_Y_,_Z_,
			_AA_,_AB_,_AC_,_AD_,_AE_,_AF_,_AG_,_AH_,_AI_,_AJ_,_AK_,_AL_,_AM_,_AN_,_AO_,_AP_,_AQ_,_AR_,_AS_,_AT_,_AU_,_AV_,_AW_,_AX_,_AY_,_AZ_,
			_BA_,_BB_,_BC_,_BD_,_BE_,_BF_,_BG_,_BH_,_BI_,_BJ_,_BK_,_BL_,_BM_,_BN_,_BO_,_BP_,_BQ_,_BR_,_BS_,_BT_,_BU_,_BV_,_BW_,_BX_,_BY_,_BZ_,
			_CA_,_CB_,_CC_,_CD_,_CE_,_CF_,_CG_,_CH_,_CI_,_CJ_,_CK_,_CL_,_CM_,_CN_,_CO_,_CP_,_CQ_,_CR_,_CS_,_CT_,_CU_,_CV_,_CW_,_CX_,_CY_,_CZ_,
			_DA_,_DB_,_DC_,_DD_,_DE_,_DF_,_DG_,_DH_,_DI_,_DJ_,_DK_,_DL_,_DM_,_DN_,_DO_,_DP_,_DQ_,_DR_,_DS_,_DT_,_DU_,_DV_,_DW_,_DX_,_DY_,_DZ_,
			_EA_,_EB_,_EC_,_ED_,_EE_,_EF_,_EG_,_EH_,_EI_,_EJ_,_EK_,_EL_,_EM_,_EN_,_EO_,_EP_,_EQ_,_ER_,_ES_,_ET_,_EU_,_EV_,_EW_,_EX_,_EY_,_EZ_,
			_FA_,_FB_,_FC_,_FD_,_FE_,_FF_,_FG_,_FH_,_FI_,_FJ_,_FK_,_FL_,_FM_,_FN_,_FO_,_FP_,_FQ_,_FR_,_FS_,_FT_,_FU_,_FV_,_FW_,_FX_,_FY_,_FZ_,
			_GA_,_GB_,_GC_,_GD_,_GE_,_GF_,_GG_,_GH_,_GI_,_GJ_,_GK_,_GL_,_GM_,_GN_,_GO_,_GP_,_GQ_,_GR_,_GS_,_GT_,_GU_,_GV_,_GW_,_GX_,_GY_,_GZ_,
			_HA_,_HB_,_HC_,_HD_,_HE_,_HF_,_HG_,_HH_,_HI_,_HJ_,_HK_,_HL_,_HM_,_HN_,_HO_,_HP_,_HQ_,_HR_,_HS_,_HT_,_HU_,_HV_,_HW_,_HX_,_HY_,_HZ_,
			_IA_,_IB_,_IC_,_ID_,_IE_,_IF_,_IG_,_IH_,_II_,_IJ_,_IK_,_IL_,_IM_,_IN_,_IO_,_IP_,_IQ_,_IR_,_IS_,_IT_,_IU_,_IV_
		};
    public:
        		//
        // ����һ���հ׹�����,ExcelĬ�ϴ�����һ��Sheet
        //> bShowApp:�Ƿ���ʾExcelӦ�ó���
        //> lpszSheetName:Ĭ�ϵĵ�һ�Ź����������
		//
        BOOL CreateOneBlankBook(BOOL bShowApp = FALSE,const TCHAR *lpszSheetName=_T("Sheet1"));
        
		//
		// ��һ���Ѵ��ڵĹ�����
		//
        BOOL OpenBook(TCHAR *lpszBookPath,BOOL bShowApp = FALSE);

		//
        // ���湤������OpenBook�򿪵�
		//
        BOOL  Save();

		//
		// ���湤����,CreateOneBlankBook������
		//
		BOOL  Save(LPCTSTR lpszFullPath);

		//
		// �رչ�����
		// 
		void Close();

		//
		// ��ȡ��������ܸ���
		// 
		int GetSheetCount();

		_WorkbookPtr GetWorkBook();

		//
		// �ڹ������ϵ���������һ�Ź�����
		// 
		virtual BOOL InsertSheet(const TCHAR * lpszSheetName);

		BOOL EmptyExcelClipboard();

		void SetMainWnd(HANDLE hWnd){ m_hMainWnd = hWnd; }

		//
		// ��ָ��λ�ò��빤����
		//> iIndex: ��1��ʼ
		//> bAtBefore:TRUE:��iIndex��ǰ�����,FALSE:��iIndex�ĺ������
		//
		virtual BOOL InsertSheet(int iIndex, const TCHAR * lpszSheetName,BOOL bAtBefore = TRUE);

		virtual BOOL DeleteSheet(int iIndex);
		virtual BOOL DeleteSheet(const TCHAR * lpszSheetName);

        // add yang
        BOOL InsertChartSheet(int iIndex, const TCHAR *lpszSheetName, BOOL bAtBefore = TRUE);
        // add yang

		//
		// �õ���ǰ�������
		//
		_WorksheetPtr GetActiveSheet();

		//
		// �õ�ָ���Ĺ�����,ͨ�����
		// iIndex >= 1
		//
		_WorksheetPtr GetSheet(const int iIndex);

		//
		// �õ�ָ���Ĺ�����ͨ������������
		// 
		_WorksheetPtr GetSheet(const WCHAR * lpszName);
		
		//
		//�õ�ָ����ͼ��
		// û�ҵ�������NULL
		//
		_ChartPtr GetChart(const int iIndex);

		//
		// �õ�ָ����ͼ��
		// û�ҵ�������NULL
		//
		_ChartPtr GetChart(const WCHAR* lpszName);
		//
		// �ƶ�������
		// lpszSrcSheetName:  Ҫ�ƶ��Ĺ����������
		// lpszDesSheetName:  �ƶ���Ŀ�Ĺ������ǰ����ߺ���
		// bBefore: TRUE:��Ŀ�Ĺ������ǰ��,FALSE:��Ŀ�Ĺ�����ĺ���
		//
		void MoveSheet(const WCHAR *lpszSrcSheetName,const WCHAR *lpszDesSheetName,BOOL bBefore = TRUE);
		//
		// �ƶ�������
		// lpszSrcSheetName:  Ҫ�ƶ��Ĺ����������
		// nDesSheetItem:     �ƶ���Ŀ����ŵĹ������ǰ����ߺ���
		// bBefore: TRUE:��Ŀ�Ĺ������ǰ��,FALSE:��Ŀ�Ĺ�����ĺ���
		//
		void MoveSheet(const WCHAR *lpszSrcSheetName,const int nDesSheetItem,BOOL bBefore = TRUE);

		//
		// ����ָ��������
		//> iIndex: ��1��ʼ
		//
		virtual BOOL SetActiveSheet(int iIndex);

		//
		// ����ָ�����ƵĹ�����
		//> lpszSheetName: ����������
		//
        virtual BOOL SetActiveSheet(TCHAR * lpszSheetName);

		//
        // ���ù���������
		// iIndex >= 1
		//
        BOOL SetSheetName(int iIndex,const TCHAR * lpszSheetName);

		//
        // ���к�ת�����б�����: 27 ="AA";
		// iCol >= 1
		//
        static _bstr_t ConvertCol(int iCol);

		//
		// ת��Ϊ��Ԫ�����꣺"A1"
		//	iCol >= 1, iRow >=1
		//
		static _bstr_t ConvertColRow(int iCol,int iRow);

		//
		// �õ�һ������
		//
		static RangePtr GetRangePtr(_WorksheetPtr ptrSheet,int col1,int row1,int col2,int row2);
		
		///{{{ ������ʽ��������

		//
		// ���ù�������п�
		//
		static void Format_ColsWidth(_WorksheetPtr ptrSheet,int iCol1,int iCol2,float fWidth = 8.50f);

		static void Format_RangeBkColor(_WorksheetPtr ptrSheet,int iCol1,int iRow1,int iCol2,int iRow2,DWORD clr=RGB(196,196,196));
		
		static void Format_RangeFontColor(_WorksheetPtr ptrSheet,int iCol1,int iRow1,int iCol2,int iRow2,DWORD clr=RGB(196,196,196) );

		static void Format_RangeFont(_WorksheetPtr ptrSheet,int iCol1,int iRow1,int iCol2,int iRow2,DWORD clr=RGB(0,0,0),const char* fontName = "����",int nFontSize = 10,BOOL bBold = FALSE,BOOL bItalic = FALSE );
		// Method:  Format_BorderRange   [ public ]
		// Description:  ��ָ��������� ����߿�
		// Returns [void]: 
		// Parameters: 
		//    [ _WorksheetPtr ptrSheet ]:  ָ��������
		//    [ DWORD clr]: �߿���ɫ
		//    [ int col1 ]: ��ʼ�� >=1
		//    [ int row1 ]: ��ʼ�� >=1
		//    [ int col2 ]: ������ >=1
		//    [ int row2 ]: ������ >=1
		//    [ int nWeight ]: ��ȼ���ȡֵ 1,2,3,4  //1��4��խ����
		//    [ int nType   ]: �߿����� 1:ֻ����߿�,2:ֻ���ڱ߿�,3:����߿�
		// Author:  Guoxuxing   
		// Modify-Date:2009/12/04
		//************************************
		static void Format_BorderRange(_WorksheetPtr ptrSheet,DWORD clr,int col1,int row1,int col2,int row2,int nWeight = 2,int nType = 3 );

		//
		// �豸ĳ����Χ���еĸ߶�
		//
		static void Format_RowsHeight(_WorksheetPtr ptrSheet,int row1,int row2,float fHeight = 19.0f);
		
		//
		//
		//
		static void Format_RangeMerge(_WorksheetPtr ptrSheet,int col1,int row1,int col2,int row2);

		//
		//	���÷�Χ���ı��Ķ��뷽ʽ
		//
		//	nHoriAlign:ȡֵΪ1,2,3.  1��ʾ�����,2��ʾ���У�3��ʾ�Ҷ���
		//  nVertAlign:ȡֵΪ1,2,3.  1��ʾ�������룬2��ʾ���У�3��ʾ�ײ�����
		//
		static void Format_RangeAlignment(_WorksheetPtr ptrSheet,int col1,int row1,int col2,int row2,int nHoriAlign = 2,int nVertAlign = 2);

		//
		// Method:  Format_MoveShapeToCell   [ public static ]
		// Description:   ��ָ�� ��״���� �ƶ���ָ����Ԫ��
		// Returns [void]: 
		// Parameters: 
		//    [ _WorksheetPtr ptrSheet ]:   
		//    [ int nShapeIndex ]:      ��״����
		//    [ int col1 ]:  ָ����
		//    [ int row1 ]:  ָ����
		//    [ OUT float * pfWidth ]:   ��� pfWidht��Ϊ�գ��õ�ƫ�ƵĿ��
		//    [ OUT float * pfHeight ]:  ��� pfHeight��Ϊ�գ��õ�ƫ�Ƶĸ߶�
		// Author:  Guoxuxing   
		// Modify-Date:2009/12/11
		//************************************
		static ShapeRangePtr Format_MoveShapeToCell(_WorksheetPtr ptrSheet,int nShapeIndex,int col1,int row1,OUT float *pfWidth = NULL , OUT float *pfHeight = NULL); 
		
		//
		// ��ָ�� ͼ���ƶ��� ������Ͻǵ�ƫ�ƿ�Ⱥ͸߶�
		//
		static ShapeRangePtr Format_MoveShape(_WorksheetPtr ptrSheet,int nShapeIndex,IN float fWidth,IN float fHeight);
		
		//
		//	����ָ����Χ�ĵ�Ԫ�����ֵĸ�ʽ������ "0.00" , "0.00%"...
		//
		static void Format_PutNumberFormat(_WorksheetPtr ptrSheet,int col1,int row1,int col2,int row2,const char* nFormat);
		
		//
		//	���ñ߿�
		//
		void Format_PutBorderAround( _WorksheetPtr ptrSheet,int col1,int row1,int col2,int row2, short sLineStyle = 1, 
			long lWeight =2 , long lCorIndex = 1, DWORD clr=RGB(0,0,0));

		//
		//	����ָ����Χ�ĵ�Ԫ�Ŀ������Ӧ
		//
		static void Format_PutAutoFit( _WorksheetPtr ptrSheet,int col1,int row1,int col2,int row2 , BOOL bCol = TRUE);
		//
		//	����ָ����Ԫ����ı�
		//
		static void Format_PutCellText(_WorksheetPtr ptrSheet,int col,int row,const WCHAR * wszText);

		//
		//	����ָ����Ԫ������Ͻ�����
		//
		static void Format_Frozen(_WorksheetPtr ptrSheet,int col,int row);

		///}}}
	private:
        TCHAR m_lpBookName[1024];
        BOOL m_bCreated ;          //��ʶ�Ƿ��Ѿ�����Excelʵ��
        BOOL m_bClosed ;           //��ʾ�Ƿ��Ѿ��ر�Excelʵ��

		HANDLE m_hMainWnd;

        static const DWORD m_dwMaxLine = 65536;			//Excel��Ĭ���������Ϊ 65536,�������Ϊ"VI",�������������õ���ô����
    };

////------------------------------------------------------------
//// CExcelExport
///------------------------------------------------------

    class CExcelExport:public CExcel
    {
    public:
        CExcelExport();
        virtual ~CExcelExport();
		///{{{ ����CExcel�������ĸ�������ʵ���˶Ը���������Ĳ���

		//
        //��ָ��λ�ò��빤����
        //>iIndex: ��1��ʼ
        //>bAtBefore:�Ƿ���iIndex��ǰ�����
		//
        virtual BOOL InsertSheet(int iIndex, const TCHAR * lpszSheetName,BOOL bAtBefore  = TRUE );
        
		//
		//��ĩβ���һ��������
		//
		virtual BOOL InsertSheet(const TCHAR *lpszSheetName);

		//
        //����ָ��������
        //>iIndex: ��1��ʼ
		//
        virtual BOOL SetActiveSheet(int iIndex);

		//
		// ����ָ��������
		//
        virtual BOOL SetActiveSheet(TCHAR * lpszSheetName);

		///}}}
    public:
		//
        // Method:  AppendData   [ public ]
		// Description:     ��������ǰ��Ĺ������������,��׷�ӵķ�ʽ
		//					�������ݵ��ڴ�,1�л��߶���
        // Returns [void]: 
        // Parameters: 
        //    [ char * lpszData ]:  =NULL:����հ���
		//                ���ĩ��û��'\n',InsertOneLine�Զ����'\n'.'\t'��ʾ��һ��Ԫ��
        // Author:  Guoxuxing   
        // Modify-Date:2009/10/23
        //************************************
        void AppendData(char *lpszData = NULL);

		//
		// ͬAppendData������������������ݵ�����ʹ�ô˷���Ч�ʸ�
		//
		void AppendData(char *lpszData,int nLineCount,int nColumnCount);

		//
		// �ú������Զ����õģ��������Ϊ�ĵ��ã��������.
		// ������±�Ϊ��ǰ����Ĺ�����.���ش˷���ʵ������Ĺ���
		//
        // ���ݲ���65536���б�Ҫ�ڵ�ǰ�������ĺ���׷��һ���µĹ������¹�����
        // �������Ǹû����������+�����������,�����ǰ������������Ϊabc,��ô�¹����������Ϊabc__2,����Ϊabc__3
		//
        virtual void InsertAppendSheet(TCHAR * lpszNewName = NULL);

        // Method:  InsertOneBmp   [ public ]
        // Description:    ����һ��λͼ
        // Returns [void]: 
        // Parameters: 
        //    [ CBitmap & bmp ]: λͼ����
        //    [ int iRow ]: ��ʼ�к�(>=1),0��ʾĬ�ϴ���
        //    [ int iCol ]: ��ʼ�к�(>=1),0��ʾĬ�ϴ���
        // Author:  Guoxuxing   
        // Modify-Date:2009/10/23
		// Remark: λͼ�������빤����
        //************************************
#ifndef _WINDOWS_
        void InsertOneBmp(IN CBitmap& bmp,int iCol = 0, int iRow = 0);
#endif

		// ������sheet��ճ���ı�
		void PasteText(const char*lpszData,int iCol1,int iRow1,int iCol2,int iRow2);
    public:
		//
        //�����а������д��Excel�л�Ĺ�����
        //bNewSheet:>д��֮��Ҫ��Ҫ����һ���µĹ�����,��������µĹ������Ͱ��µĹ�����
        //          ��Ϊ��Ĺ�����
        void Flush(BOOL bNewSheet = FALSE);
	public:
		
	protected:
		//
		//��������ʱ������ô˺���
		// ������������ô˷�������Ҫ�ǳ�ע��
		//
		void UpdateLines();       
    protected:
        static const DWORD MAX_LINE    = 65536 ;   //Excel֧�ֵ������
        static const DWORD MEM_SIZE = 0x80000; //500K�ڴ��С
        HANDLE m_hGlobalMem;            //�ڴ�����GloableAllocate���
        CHAR *m_pMem;                //�ڴ�ָ��
        DWORD m_nUsedMem;       //��ʹ�õ��ڴ��ֽ���
        int m_iUsedLines ;          //��ĳ���������У��Ѿ�ʹ�õ�����
        int m_iMemLines;           //���ڴ����Ѿ�д�������
        int m_iMaxColumns;       //�����������������ճ������ķ�Χ
        
        BOOL m_bFirstInsertData;  //��ʾ����ĳ���������Ƿ�Ϊ�״β�������
    };

	class CExcelExportEx:public CExcelExport
	{
	public:
		CExcelExportEx();
		~CExcelExportEx();

	protected:
		enum emStatus{
			S_CanRead,
			S_Write,
			S_Idle,			
		};
		static const DWORD BLOCK_COUNT = 5;
		static const DWORD BLOCK_SIZE = MEM_SIZE;

		CHAR *m_blocks[BLOCK_COUNT];  //ָ����ָ��
		int m_status[BLOCK_COUNT];    //�����ʹ��״̬
		int m_usedLength[BLOCK_COUNT]; //������ʹ�õ��ֽڳ���
		int m_iCurrentBlock;  //��ǰ��
		int m_blockMaxCols[BLOCK_COUNT];

		NS_GXX::CQueue<int> m_queueCanRead;

		CCriticalSection m_cs;

		HANDLE m_hEventHaveIdleBlock;
		HANDLE m_hEventCanRead;

		HANDLE m_hEventQueueEmpty;

		//------ �߳̿���

		void HoldCanRead();   //��ֹ�ɶ�
		void WaitCanRead();   //�ȴ��ɶ�
		void AllowCanRead();  //����ɶ�

		void WaitIdleBlock(); //�ȴ��п��п����
		void HoldIdleBlock(); //û���п��п����
		void AllowIdleBlock(); //�п��п����

		//------

		static UINT _ThreadEntry(LPVOID);

		CWinThread *m_pWinThread;
		HANDLE m_hEventThreadFinish; //�����ڶ�������ʱ���ȵȴ��߳��˳�

		bool m_bExit;
	public:
		void Flush();
		void AppendData(char *lpszData = NULL);
		void AppendData(char *lpszData,int nLineCount,int nColumnCount);
	};


}

