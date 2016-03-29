#pragma once

namespace NS_DAL
{

// ��Ʒ����
class GPropertyClass CCommodity
{
public:

	CCommodity();

	GProperty( CommodityCode, CString );
	GProperty( CommodityName, CString );
	GProperty( ShortName,     CString );
	GProperty( GroupId,       CString );
	GProperty( GroupName,     CString );
	GProperty( BarCode,       CString );
	GProperty( StatusFlag,    CString );
	GProperty( CoreFlag,      CString );
	GProperty( DeliveryFlag,  CString );
	GProperty( StoreFlag,     CString );
	GProperty( PurchaseFlag,  CString );
	GProperty( StoreUnit,     CString );
	GProperty( RetailUnit,    CString );
	GProperty( ProducerCode,  CString );
	GProperty( ProductCode,   CString );
	GProperty( Description,   CString );
	GProperty( Image,         CString ) ;

	GProperty( UnitMultiple,     UINT );
	GProperty( ContractPrice,    UINT );
	GProperty( AdvicePrice,      UINT );
	GProperty( UnitCost,         UINT );
	GProperty( AccountType,      UINT );
	GProperty( RemainQuantity,    INT );
	GProperty( Point,            UINT );
	GProperty( StoreInQuantity,  UINT );
	GProperty( StoreInAmount,    UINT );
};

typedef CArray<CCommodity, CCommodity&> CCommodityArray;


class GPropertyClass CRetailVoucher
{
public:
	
	CRetailVoucher();

	GProperty( VoucherNo,      CString );
	GProperty( OldVoucherNo,   CString );
	GProperty( NetBarId,          UINT );
	GProperty( StoreId,           UINT );
	GProperty( RefNo,             UINT );
	GProperty( SerialNo,          UINT );
	GProperty( PayType,           UINT );
	GProperty( RetailType,        UINT );
	GProperty( MemberId,          UINT );
	GProperty( Week,              UINT );
	GProperty( ClassId,           UINT );
	GProperty( UserId,         CString );
	GProperty( VoucherTime,    CString );
	GProperty( Description,    CString );
	GProperty( CashRegisterID,    UINT );
	GProperty( Point,             UINT );
	GProperty( AccountYear,    CString );
	GProperty( VoucherStatus,  CString );
	GProperty( AccountMonth,   CString )
};

class GPropertyClass CRetailDetail
{
public:
	
	CRetailDetail();

	GProperty( VoucherNo,      CString );
	GProperty( CommodityCode,  CString );
	GProperty( Quantity,           INT );
	GProperty( Price,             UINT );
	GProperty( Amount,             INT );
	GProperty( Cost,               INT );
	GProperty( Point,             UINT );
	GProperty( ReturnQuantity,     INT );
};

class CRetailInfo
{
public:

	CRetailVoucher RetailVoucher;

	CRetailDetail RetailDetail;

	CCommodity Commodity;
	
};

typedef CArray<CRetailInfo, CRetailInfo&> CRetailInfoArray;

class GPropertyClass CCommoditySoldInfo
{
public:

	CCommoditySoldInfo()
	{
		GPropertyInit( SoldCount1,        0 );
		GPropertyInit( SoldAmout1,        0 );
		GPropertyInit( ReturnCount1,      0 );
		GPropertyInit( ReturnAmout1,      0 );
		GPropertyInit( SoldCount2,        0 );
		GPropertyInit( SoldAmout2,        0 );
		GPropertyInit( ReturnCount2,      0 );
		GPropertyInit( ReturnAmout2,      0 );
		GPropertyInit( PresentCount,      0 );
		GPropertyInit( ExchangePoint,     0 );
		GPropertyInit( RetailType,        0 ); // ��������1: ���ۣ����߹���Ҳ�����֣� 2:�˻� 3: ����
		GPropertyInit( PayType,           0 ); // ��������
	}

	GProperty( SoldCount1,        INT );
	GProperty( SoldAmout1,        INT );
	GProperty( ReturnCount1,      INT );
	GProperty( ReturnAmout1,      INT );
	GProperty( SoldCount2,        INT );
	GProperty( SoldAmout2,        INT );
	GProperty( ReturnCount2,      INT );
	GProperty( ReturnAmout2,      INT );
	GProperty( PresentCount,      INT );
	GProperty( ExchangePoint,     INT );
	GProperty( RetailType,        INT ); // ��������1: ���ۣ����߹���Ҳ�����֣� 2:�˻� 3: ����
	GProperty( PayType,           INT ); // ��������
	GProperty( Commoditycode, CString ); // ��Ʒ����
};

typedef CArray<CCommoditySoldInfo, CCommoditySoldInfo&> CCommoditySoldInfoArray;

//!< ��Ʒ���۲�ѯ����

class CRetailQueryCondition
{
public:
	
	CRetailQueryCondition()
	{
		m_nRetailType = 1;
		m_nPayType = 0;
		m_nStoreId = 0;
	}

	CString m_strStartTime;
	CString m_strEndTime;
	CString m_strCashier;
	CString m_strCommodityId;

	UINT m_nRetailType;
	UINT m_nPayType;
	UINT m_nStoreId;
	
};

//!< ��Ʒ��ѯ����

class CCommodityQueryCondition
{
public:

	CCommodityQueryCondition()
	{
		m_bHasPoint = FALSE;
	}

	CString m_strCommodityId;
	
	BOOL m_bHasPoint;
};

}