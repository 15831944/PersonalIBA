
#pragma once

#include "ComputerList.h"

class CIBAGrid : public CBCGPGridCtrl
{
	DECLARE_MESSAGE_MAP()

public:

	CIBAGrid();
	virtual ~CIBAGrid();

protected:

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

protected:
	//2011-3-29-gxx-�ȽϽ���ַ���
	virtual int CompareMoneyString(CString left, CString right)const;

	virtual int CompareItems(const CBCGPGridItem* pItem1, const CBCGPGridItem* pItem2)const;
};

class CComputerGrid : public CIBAGrid
{
	DECLARE_MESSAGE_MAP()

public:

	CComputerGrid(CComputerList* lpComputerList);
	virtual ~CComputerGrid();

private : 

	static const UINT TimeIdOfUpdateTime = 1000;
	static const UINT IntervalOfUpdateTime = 60 * 1000;

protected:

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual int GetExtraHierarchyOffset()const{ return 3;}

public :
	
	/**
	* ˢ����ʾ
	* @param bRedrawAll TRUE:�ػ�������Ϣ��FALSE:ֻ�ػ����µ���Ϣ
	*/
	void Refresh(BOOL bRedrawAll);

	CComputerList* const GetComputerList() const { return m_lpComputerList; }

private :
	/**
	* �Զ����յ�ǰ����������������
	*/
	void AutoSort();

	/**
	* ��ʼ����
	*/
	void InitColumnHead();

	/**
	* ����Ƿ������ʾ���������е���ʾ�����ж϶�������
	*/
	BOOL CheckShowCondition(CComputerInfo &computer);
	
	/**
	* ����Ƿ����������ʾ����
	*/
	BOOL CheckShowZone(CComputerInfo &computer);

	/**
	* �����ն�������Ϣ���л�����ӵ��µ���
	*/
	void UpdateComputerInfoToRow(DWORD_PTR dwData, CComputerInfo& computer);
	
	/**
	* ���ն�������Ϣд�뵽��
	*/
	void UpdateRowData(CBCGPGridRow* pRow, CComputerInfo& computer);

	/**
	* �����û�������ɫ
	*/
	void UpdateRowUserType(CBCGPGridRow* pRow, CComputerInfo& computer);
	
	/**
	* �����ϻ�ʱ��
	*/
	void UpdateAllComputersUseTime();
	
	/**
	* �����ն�״̬��ȡ������ʾ��ɫ
	*/
	COLORREF GetTextColor(CComputerInfo& computer);
	
	/**
	* ���������ն���Ϣ�����
	*/
	void RedrawAllComputers();

	/**
	* ֻ���±仯���ն���Ϣ�����
	*/
	void RedrawUpdateComputers();

	/**
	* �����û���Ϣ���ļ�
	*/
	void SaveToFile();

private : 

	CFont m_TextFont;

	CComputerList* const m_lpComputerList;  //��ָ��CLocalServer::ComputerList

};

