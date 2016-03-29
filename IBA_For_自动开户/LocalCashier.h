#pragma once

#include "Cashier.h"

// CLocalCashier ����Ŀ��

class CLocalCashier : public CCashier
{
public:
	CLocalCashier();
	virtual ~CLocalCashier();

public :

	virtual BOOL CheckIn(CCashRegisterLoginMethod & CashRegisterLogin, CString strOperator);//������
	
	// ִ�������ķ��Ͷ�����֪ͨ
	virtual BOOL DoLocalMode();

	virtual BOOL CheckLastDuty(CString& strLastOperator);//������������Ƿ�����

private:

	// ��ʼ��ʱ���ñ������ݿ�
	BOOL SetLocaNetbarlInfo(void);

	virtual void WriteDutyInfo();//��������Ϣ
};


