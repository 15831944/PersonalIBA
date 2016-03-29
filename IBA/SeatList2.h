#pragma once


#include "Seat2.h"
// CSeatList�����ڿͻ�����ʾ��
// CSeatList2��������������̨��WC��ʾ��
// CSeatList2 ����Ŀ��
#include "IBACommDefine.h"

using namespace NS_IBACommDef;

class CSeatList2 : public CObject
{
public:
	CSeatList2();
	virtual ~CSeatList2();

private:
	CTypedPtrArray <CObArray, CSeat2 *> m_SeatList;
	
	static const UINT SizeOfBMPHalf = 24;
public:
	INT Add(CSeat2 * Seat);
	BOOL SaveList();
	BOOL ReadList();
	void DeleteAll();
	void DrawAllRect(CDC * pDC = NULL, double XRatio = 1.0, double YRatio = 1.0);
	//���Ƿ����Ԫ�ط�Χ�������򷵻���ţ���֮����-1
	INT InRect(CPoint MousePoint);
	//CString GetComputerIP(CString strTreminID) const;
	int GetSize() { return m_SeatList.GetCount(); }
	CSeat2* GetAt(int nIndex);
	void DeleteAt(INT nIndex, INT nCount = 1);
};


