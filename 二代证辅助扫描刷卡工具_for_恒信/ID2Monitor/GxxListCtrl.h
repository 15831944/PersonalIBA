#pragma once
/*********************************************************************************************
CGxxHeaderCtrl:
	������CHeadCtrl���ṩ�ı�����ɫ���ù��ܣ���ͷ�ĸ߶����ù��ܣ�������ɫ���ù���

CGxxListCtrl:
	������CListCtrl,�ṩ���¼��ֹ��ܣ�
		1���ṩ��Ԫ��֧�ֳ��ÿؼ����Զ���ؼ��Ĺ���
		2���ṩ��ͷ�����ù���
		3���ṩ��Ԫ���ı�����ɫ/����ɫ���õĹ���
		4��֧�����ַ�ʽ�ĵ�Ԫ��ؼ����ܣ��й���ʽ���зǹ���ʽ
		5���ṩ�б�������ƹ���
	ע��
	1���й���ʽ�����й���һ���ؼ����Ӵ������ͻ��Ӵ���ָ���ڲ�����ʱ��ָ��
	2�����й���ʽ���Ӵ������ͻ��Ӵ���ָ���ڲ�����ʱָ��,����SetItem
		��ָ����Ԫ�������ı�/�ؼ��������Ӵ��ڻ�һֱ��ʾ,������ʧ

	3�����Ҫ��������Լ��Ŀؼ����ͣ���Ҫ����CreateChildWnd����,���µ�
		�ؼ����Ͳ��ܺ����еĿռ�������ͬ��

Author:guoxuxing
Date:2010/3/1
*********************************************************************************************/

#include <vector>
#include <afxcmn.h>
#include <afxwin.h>
using namespace std;


///
///
/// CGxxListCtrl
/// CGxxListCtrl::CListCtrl 
///
/// ע��ע���б��ʺϴ������ݵ���ʾ����Ϊ������ռ�úܴ�Ĵ洢�ռ�
///
	typedef struct NMHDR_GLC_CONTROL
	{
		NMHDR hdr;
		int nRowID;
		int nColID;
		int nKind;  // CGxxListCtrl::emChildWndKind
		LPNMHDR pCtrlHdr;
	}*LPNMHDR_GLC_CONTROL;

#define GLCN_CHILD_CONTROL (1)

	class  CGxxListCtrl:public CListCtrl
	{
		DECLARE_DYNAMIC(CGxxListCtrl)

	public:
		CGxxListCtrl();
		virtual ~CGxxListCtrl();

		///�����޷�ʶ��Ķ���Ϊ���Զ�����Ӵ���
		enum emChildWndKind
		{
			GLC_CUSTOM = 0,    //�Զ�һ�������Ҫ�ṩ�Ӵ��ڴ���ָ��
			
			/// ע:���Զ�����Ӵ��ڶ���Ҫɾ���ڴ�,CGxxListCtrl�����˴˹���
			///
			
			GLC_Button = 1,    //��ť�Ӵ���
			GLC_Edit   = 2,    //�༭���Ӵ���
			GLC_ComboBox = 3,  //��Ͽ��Ӵ���
			GLC_CheckBox = 4,  //checkBox�Ӵ���
			GLC_RadioBox = 5,  //RadioBox�Ӵ���
			GLC_Static = 6     //��̬�ı� 
		};

		///��Ԫ���������Ϣ
		struct SubItemStruct
		{
			CWnd* pChildWnd;     // �Ӵ���ָ��
			int   nChildWndKind; //�Ӵ��ڵ�����
			BOOL  bAutoAdjust;   //�Ӵ��ڵĴ�С�浥Ԫ��Ĵ�С�ı���ı�
			SIZE  szWnd;         //�Ӵ��ڵĴ�С��bAutoAdjustΪFALSE����Ч
			int   nAlignment;    //�Ӵ����ڵ�Ԫ���еĶ��뷽ʽ��bAutoAdjustΪFALSE����Ч��DT_LEFT,DT_CENTER,DT_RIGHT
			COLORREF clrTxt;     //��Ԫ���ı�����ɫ
			COLORREF clrBk;      //��Ԫ�񱳾�ɫ

			DWORD dataPtr;

			SubItemStruct()
			{
				pChildWnd = NULL;
				nChildWndKind = GLC_CUSTOM;
				bAutoAdjust = TRUE;
				szWnd.cx = 0;
				szWnd.cy = 0;
				nAlignment = DT_CENTER;
				clrTxt = RGB(0,0,0);
				clrBk = RGB(255,255,255);
				dataPtr = 0;
			}
		};

		///�е�������Ϣ
		struct ColInfo
		{
			BOOL	bHaveChildWnd;  //���������Ƿ�֧���Ӵ���,�ڲ�����ʱָ��

			/// ע:
			///   ����ʽ���Ӵ������ͻ��Ӵ���ָ���ڲ�����ʱָ��
			///   �ǹ���ʽ���Ӵ������ͻ��Ӵ���ָ���ڲ�����ʱָ��
			///
			BOOL	bShared;        //�������е����е�Ԫ����һ���Ӵ���
			BOOL	bAutoAdjust;    //�����Ӵ����Ƿ��浥Ԫ��Ĵ�С�ı���ı�

			/// ע��
			///   bHaveChildWnd �� bShared��ΪTrueʱ�����²����Ż�����
			///
			int		nChildWndKind;
			CWnd*	pChildWnd;
			BOOL    bShown;
			int		iShowAtItem;
			SIZE    szWnd;

			DWORD   txtAlign;

			ColInfo()
			{
				bHaveChildWnd = FALSE;
				bShared = FALSE;
				nChildWndKind = 0;
				pChildWnd = NULL;
				bAutoAdjust = TRUE;
				bShown = FALSE;
				iShowAtItem = -1;
				szWnd.cx = 0;
				szWnd.cy = 0;
				txtAlign = DT_LEFT;
			}
		};
	protected:
		DECLARE_MESSAGE_MAP()

		afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
		void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

		virtual void PreSubclassWindow();

	public:
		///-------------------------------------------
		/// Description:������
		/// Returns [int]: 
		/// Parameters:            
		///    [ int nCol ]:					����λ��
		///    [ LPCTSTR lpszColumnTxt ]:	�б���
		///    [ int nFormat ]:				�ж��뷽ʽ,LVCFMT_LEFT...
		///    [ int nWidth ]:				�п�
		///    [ BOOL bHaveChildWnd ]:		�����Ƿ�֧���Ӵ���
		///    [ BOOL bShared ]:				�Ƿ���ʽ��֧���Ӵ���
		///    [ BOOL bAutoAdjust ]:			�������е��Ӵ����Ƿ��浥Ԫ���С�ı���ı�
		///    [ int nChildWndKind ]:		���е��Ӵ������ͣ���bSharedΪTRUE������Ч
		///    [ CWnd * pChildWnd ]:			���е��Ӵ���ָ�룬��bShared��nChildWndKind=GLC_CUSTOMʱ������Ч
		/// Remark:
		///		��ָ��nChildWndKindΪ��GLC_CUSTOMʱ��CGxxListCtrl�����CreateChildWndΪ֧��
		///    �Ӵ��ڵĵ�Ԫ�񴴽��Ӵ��ڶ���.
		/// Author:      
		/// CreateTime:2010/03/24    
		///-------------------------------------------
		virtual int  InsertColumn(
			int     nCol,
			LPCTSTR	lpszColumnTxt,
			int		nFormat,
			int		nWidth,
			BOOL	bHaveChildWnd,
			BOOL	bShared=FALSE,
			BOOL	bAutoAdjust=FALSE,
			LPSIZE  lpCtrlSize = NULL,
			int		nChildWndKind=GLC_CUSTOM,
			CWnd*	pChildWnd=NULL);


		///-------------------------------------------
		/// Description:������,�����пؼ���֧��           
		/// Returns [int]: 
		/// Parameters:            
		///    [ int nCol ]:   ����λ��
		///    [ LPCTSTR lpszColumnTxt ]: �б���  
		///    [ int nFormat ]:  �ж��뷽ʽ 
		///    [ int nWidth ]:   �п�
		/// Author:       
		/// CreateTime:2010/03/24    
		///-------------------------------------------
		virtual int InsertColumn(
			int		nCol,
			LPCTSTR lpszColumnTxt,
			int     nFormat=LVCFMT_LEFT,
			int		nWidth = -1
			);

		/// ɾ����
		virtual int DeleteColumn(int nCol);
		
		/// ����һ��
		virtual int InsertItem(
			int nItem, CString str,
			int nChildWndKind=GLC_CUSTOM,
			CWnd* pChildWnd=NULL,
			LPSIZE lpSize=NULL);
		
		/// ɾ��һ��
		virtual BOOL DeleteItem(int nItem);

		/// ɾ������
		BOOL DeleteAllItems();
		
		/// ���õ�Ԫ����
		virtual int SetItem(
			int nItem,
			int nSubItem,
			CString str,
			int nChildWndKind=GLC_CUSTOM,
			CWnd* pChildWnd=NULL,
			LPSIZE lpSize=NULL);

		void GetItemIdByChildWndID(UINT nID,int* pItem,int*  pSubItem = NULL);
		
		/// ���ص�����ʽ���ӿؼ�
		void HideAllSharedWnd();

		/// ע��һ��ID
		UINT RegeisterCtrlID();

	protected:
		
	protected:

		virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
		virtual void DrawSubItem(CDC* pDC,int nItem,int nSubItem,UINT itemState=ODS_DEFAULT);
		virtual void DrawSubItemText(CDC* pDC,int nItem,int nSubItem,LPCTSTR lpTxt,LPRECT lpRect,UINT itemState);
		
		virtual CWnd* CreateChildWnd(int nChildWndKind);

		virtual CString GetChildWndText(CWnd*pWnd,int nItem,int nSubItem);
		virtual void SetChildWndText(CWnd*pWnd,CString str, int nItem,int nSubItem);

	public:
		RECT GetSubItemRect(int iItem,int iSubItem);

		void SetHeaderFont(CFont* pFont);
		/// �����и�
		void SetItemsHeight( int iHeight );
		
		/// ˢ�������Ӵ�����ʾ
		void ShowAllSubWindows();
		
		/// ��ȡ����
		int GetColumnCount();
		
		/// ĳ���Ƿ�֧���Ӵ���
		BOOL  GetColHasChildWnd(int iCol);
		/// ĳ���Ƿ�Ϊ�й���ʽ
		BOOL  GetColIsShared(int iCol);
		/// ĳ�е��Ӵ�������
		int  GetColChildWndKind(int iCol);
		/// ĳ�е��Ӵ���ָ��
		CWnd* GetColChildWnd(int iCol);
		/// �й���ʽ���Ӵ��ڵĵ�ǰ״̬�Ƿ�Ϊ��ʾ״̬
		BOOL  GetColChildWndShown(int iCol);
		/// �й���ʽ���Ӵ��ڵĵ�ǰ������
		int   GetColChildWndAtItem(int iCol);
		/// �����й���ʽ���Ӵ��ڵ���ʾ״̬
		void  SetColChildWndShown(int iCol,BOOL bShow);
		/// �����й���ʽ���Ӵ��ڵĵ�ǰ������
		void  SetColChildWndAtItem(int iCol,int iAtItem );

		/// �õ���Ԫ���Ӵ�������
		int GetSubItemWndKind(int iItem,int iSubItem);
		/// �õ���Ԫ���Ӵ���ָ�룬��Ч����NULL
		CWnd *GetSubItemWnd(int iItem,int iSubItem);
		/// �õ���Ԫ���Ӵ���ID����Ч����0
		UINT GetSubItemWndId(int iItem,int iSubItem);
		/// �õ���Ԫ���ı���ɫ
		COLORREF GetSubItemTxtColor(int iItem,int iSubItem);
		/// �õ���Ԫ�񱳾���ɫ
		COLORREF GetSubItemBkColor(int iItem,int iSubItem);
		/// ���õ�Ԫ���ı���ɫ
		void	SetSubItemTxtColor(int iItem,int iSubItem,COLORREF clr);
		/// ���õ�Ԫ������ɫ
		void	SetSubItemBkColor(int iItem,int iSubItem,COLORREF clr);

		/// ���õ�Ԫ������
		void SetSubItemData(int iItem,int iSubItem,DWORD dataPtr);
		DWORD GetSubItemData(int iItem,int iSubItem);

		int GetHeaderHeight();

		SubItemStruct* GetSubItemStruct(int iItem, int iSubItem);

		BOOL GetColIsAutosize(int iCol);

	protected:

		CButton m_testBtn;
		CEdit m_testEdit;

		int m_nItemHeight;

		vector< ColInfo > m_vecColsInfo;  //��Ÿ��е���Ϣ
		vector< vector<SubItemStruct> > m_vecSubItems; //��Ÿ���Ԫ�����Ϣ

		UINT m_nInnerMaxCtrlID;


	protected:
		afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
		afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
		afx_msg void OnHdnEndtrack(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
		afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnTimer(UINT_PTR nIDEvent);
		afx_msg BOOL OnEraseBkgnd(CDC*);

	public:
		virtual void OnClickItem(int nItem,int nSubItem);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};

