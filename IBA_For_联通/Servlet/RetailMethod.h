#pragma once
#include "servletmethod.h"

namespace NS_SERVLET
{

class CSellRecord
{
public:

	CSellRecord();

	/**
	*��Ʒ����
	*/
	void SetCommodityCode(CString newVal);
	CString GetCommodityCode() const { return m_strCommodityCode; }

	/**
	*��Ʒ����
	*/
	void SetCommodityBarCode(CString newVal);
	CString GetCommodityBarCode() const { return m_strCommodityBarCode; }

	/**
	*��Ʒ����
	*/
	void SetCommodityName(CString newVal);
	CString GetCommodityName() const { return m_strCommodityName; }

	/**
	*������������λ*100
	*/
	void SetQuantity(UINT newVal);
	UINT GetQuantity() const { return m_nQuantity; }

	/**
	*���ۣ���λ����
	*/
	void SetPrice(UINT newVal);
	UINT GetPrice() const { return m_nPrice; }

	/**
	*����λ����
	*/
	void SetAmount(UINT newVal);
	UINT GetAmount() const { return m_nAmount; }
	/**
	*��Ʒ�������
	*/
	void SetPoint(UINT newVal);
	UINT GetPoint() const { return m_nPoint; }

private:

	CString m_strCommodityCode;
	CString m_strCommodityBarCode;
	CString m_strCommodityName;

	UINT m_nQuantity;
	UINT m_nPoint;
	UINT m_nPrice;
	UINT m_nAmount;
};

typedef CArray<CSellRecord, CSellRecord&> CSellRecordArray;

class CRetailMethod : public CServletMethod
{
public :
	CRetailMethod(void);
	~CRetailMethod(void);

protected :

	void AddParams();
	void AddRespFields();
	CString GetMethodName();

public :
	
	/**
	*�û��ţ��ÿ�����ʱ��д
	*/
	void SetMemberId(UINT newVal);

	/**
	*�û����ţ�������ʱ��д
	*/
	void SetCardId(UINT newVal);

	/**
	*���ɺ�
	*/
	void SetNetBarId(UINT newVal);

	/**
	*����(UNDES)
	*/
	void SetPassword(CString newVal);

	/**
	*�ֿ�ID
	*/
	void SetStoreId(UINT newVal);

	/**
	*1: �ֽ�2:��
	*/
	void SetPayType(UINT newVal);

	/**
	*1: ���� 2:�˻� 3: ����
	*/
	void SetRetailType(UINT newVal);

	/**
	*ԭ�����ţ��˻�ʱ��д
	*/
	void SetVoucherNo(CString newVal);

	/**
	*����Ա����
	*/
	void SetOperator(CString newVal);

	/**
	*��Ʒ������
	*/
	void SetCount(UINT newVal);


	void SetSellRecords(CSellRecordArray& SellRecords);


	/**
	*��ǰϵͳʱ��
	*/
	void SetTermTime(CString newVal);

	/**
	*����̨ID
	*/
	void SetCashRegisterId(UINT newVal);

	/**
	*���ID
	*/
	void SetDutyId(UINT newVal);


public :

	/**
	*ϵͳ�ο���
	*/
	UINT GetRefNo();

	/**
	*���۵��ݱ��
	*/
	CString GetVoucherNo();

	/**
	*��չ��������,�ɱ䳤��
	*/
	CString GetExtInfo();

	/**
	*���Ľ���ʱ��
	*/
	CString GetTranTime();

};

///////////////////////////////���߹���/////////////////////////
class CRetailOnlineMethod : public CServletMethod
{
public :
	CRetailOnlineMethod(void);
	~CRetailOnlineMethod(void);

protected :

	void AddParams();
	void AddRespFields();

public :

	/**
	*�û��ţ��ÿ�����ʱ��д
	*/
	void SetMemberId(UINT newVal);

	/**
	*���ɺ�
	*/
	void SetNetBarId(UINT newVal);

	/**
	*�ֿ�ID
	*/
	void SetStoreId(UINT newVal);

	/**
	*1: �ֽ�2:��
	*/
	void SetPayType(UINT newVal);

	/**
	*1: ���� 2:�˻� 3: ����
	*/
	void SetRetailType(UINT newVal);

	/**
	*ԭ�����ţ��˻�ʱ��д
	*/
	void SetVoucherNo(CString newVal);

	/**
	*����Ա����
	*/
	void SetOperator(CString newVal);

	/**
	*��Ʒ������
	*/
	void SetCount(UINT newVal);

	//����������Ʒ����Ϣ
	void SetSellRecords(CSellRecordArray& SellRecords);

	/**
	*��ǰϵͳʱ��
	*/
	void SetTermTime(CString newVal);

	/**
	*����̨ID
	*/
	void SetCashRegisterId(UINT newVal);

	/**
	*���ID
	*/
	void SetDutyId(UINT newVal);

public :

	/**
	*ϵͳ�ο���
	*/
	UINT GetRefNo();

	/**
	*���۵��ݱ��
	*/
	CString GetVoucherNo();

	/**
	*��չ��������,�ɱ䳤��
	*/
	CString GetExtInfo();

	/**
	*���Ľ���ʱ��
	*/
	CString GetTranTime();

};

}