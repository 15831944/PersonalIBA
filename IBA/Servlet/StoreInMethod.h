#pragma once

#include "servletmethod.h"

namespace NS_SERVLET
{

class CStoreInRecord
{
public:

	CStoreInRecord()
	{
		m_nAmount = 0;
		m_nPrice = 0;
		m_nQuantity = 0;
		m_nRemainQuantity = 0;
	}

	/**
	*��Ʒ����
	*/
	void SetCommodityCode(CString newVal) { m_strCommodityCode = newVal; }
	CString GetCommodityCode() const { return m_strCommodityCode; }

	/**
	*������������λ*100
	*/
	void SetQuantity(UINT newVal){ m_nQuantity = newVal; }
	UINT GetQuantity() const { return m_nQuantity; }

	/**
	*���ۣ���λ����
	*/
	void SetPrice(UINT newVal) { m_nPrice = newVal; }
	UINT GetPrice() const { return m_nPrice; }

	/**
	*����λ����
	*/
	void SetAmount(UINT newVal) { m_nAmount = newVal; }
	UINT GetAmount() const { return m_nAmount; }
	/**
	*ԭ�����������
	*/
	UINT GetRemainQuantity() const { return m_nRemainQuantity; }
	void SetRemainQuantity(UINT val) { m_nRemainQuantity = val; }

	private:
		
	CString m_strCommodityCode;

	UINT m_nQuantity;
	UINT m_nPrice;
	UINT m_nAmount;
	UINT m_nRemainQuantity;
};

typedef CArray<CStoreInRecord, CStoreInRecord&> CStoreInRecordArray;


class CStoreInMethod :public CServletMethod
{
public:
	CStoreInMethod(void);
	~CStoreInMethod(void);

protected :

	void AddParams();
	void AddRespFields();

public:
	////////////////////д�뱨��///////////////
	//�ֿ�ID
	void SetStoreId(UINT newVal);
	//���ɺ�
	void SetNetBarId(UINT newVal);
	//����Ա����
	void SetOperator(CString newVal);
	//����̨ID
	void SetCashRegisterId(UINT newVal);
	//���ID
	void SetDutyId(UINT newVal);
	//��Ʒ������
	void SetCount(UINT newVal);
	//�����Ʒ��¼
	void SetStoreInRecords(CStoreInRecordArray& StoreInRecords);
	// 2011/05/27-gxx: �Ƿ��ֽ�֧��
    void SetPayType(BOOL bCash);

	/////////////////////����ֵ//////////////////
	//ϵͳ�ο���
	UINT GetRefNo();
	//���۵��ݱ��
	UINT GetVoucherNo();
	//��չ��������,�ɱ䳤��
	CString GetExtInfo();
	//���Ľ���ʱ��
	CString GetTrantime();
};

}
