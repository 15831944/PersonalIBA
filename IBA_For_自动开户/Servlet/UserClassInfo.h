#pragma once


class CUserClassInfo
{
public:
	CUserClassInfo(void);
	~CUserClassInfo(void);

private:

	UINT m_ClassID;
	CString m_strClassName;
	UINT m_CostExpense;
	UINT m_Deposit;
	BOOL m_bIsMember;

public:

	void SetClassName(CString newVal);
	void SetCostExpense(UINT newVal);
	void SetDeposit(UINT newVal);
	void SetIsMember(BOOL newVal);
	void SetClassID(UINT newVal);
	/**
	*�û�����ID
	*/
	UINT GetClassID();

	/**
	*�û���������
	*/
	CString GetClassName();

	/**
	*������
	*/
	UINT GetCostExpense();

	/**
	*Ѻ��
	*/
	UINT GetDeposit();

	/**
	*�Ƿ��Ա
	*/
	BOOL GetIsMember();
};

typedef CArray<CUserClassInfo, CUserClassInfo&> CUserClassInfoArray;

