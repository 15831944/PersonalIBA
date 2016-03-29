#pragma once

namespace NS_DAL
{

//------------------------------------------------------------------------
class GPropertyClass CClientShortMessage
{
public:
	CClientShortMessage()
	{
		GPropertyInit3( 
			MsgID, MsgType, MemberId,
			0,     0,       0);
	}

public:
	GProperty( MsgID,      UINT ); //�ͻ�����ϢID
	GProperty( MsgType,    UINT ); //��Ϣ����
	GProperty( MemberId,   UINT ); //�û�ID
	GProperty( TermIP,  CString ); //�ͻ���IP
	GProperty( TermID,  CString ); //�ͻ���ID
	GProperty( MsgBody, CString );//��Ϣ����
	GProperty( RevTime, CString );//������Ϣ��ʱ��
};
typedef CArray<CClientShortMessage, CClientShortMessage&> CClientShortMsgArray;


//------------------------------------------------------------------------
class GPropertyClass CIDNameRecord
{
public :
	CIDNameRecord()
	{
		GPropertyInit( ID, 0 );
	}

public:
	GProperty( Name, CString );
	GProperty( ID,      UINT );
};

typedef CArray<CIDNameRecord, CIDNameRecord&> CIDNameRecordArray;


//------------------------------------------------------------------------
class GPropertyClass CIDTypeInfo
{
public:
	CIDTypeInfo(void)
	{
		GPropertyInit( IDTypeID, 0 );
	}

public:
	GProperty( IDTypeName, CString );
	GProperty( IDTypeID,      UINT );
};
typedef CArray<CIDTypeInfo, CIDTypeInfo&> CIDTypeInfoArray;


//------------------------------------------------------------------------
class GPropertyClass CShiftCash
{
public:
	CShiftCash()
	{
		GPropertyInit4(
			ID,            StandbyAmount,  Deposit,        CreditAmount,
			0,             0,              0,              0);
		GPropertyInit4(
			PresentAmount, CardSoldAmount, CashSoldAmount, CardCreditedAmount,
			0,             0,              0,              0);
		GPropertyInit4(
			TotalAmount,   CostExpense,    CashRegisterID, FetchAmount,
			0,             0,              0,              0);
		GPropertyInit4(
			ReturnAmount,  Payout,         CountAmount,    NextRemainAmount,
			0,             0,              0,              0);
		
		GPropertyInit4( ReturnAmount, Dianka, StoreInAmount, DutyIncome,
			            0,            0,      0,             0);
	}
private:
	GProperty( ID,                 INT ); //
	GProperty( StartDate,      CString ); //
	GProperty( StandbyAmount,      INT ); //;
	GProperty( Deposit,            INT ); //;
	GProperty( CreditAmount,       INT ); //
	GProperty( PresentAmount,      INT ); //
	GProperty( ReturnAmount,       INT ); //
	GProperty( CardSoldAmount,     INT ); //
	GProperty( CashSoldAmount,     INT ); //
	GProperty( CardCreditedAmount, INT ); //
	GProperty( TotalAmount,        INT ); //
	GProperty( CostExpense,        INT ); //
	GProperty( Operator,       CString ); //
	GProperty( NextOperator,   CString ); //
	GProperty( Description,    CString ); //
	GProperty( EndDate,        CString ); //
	GProperty( CashRegisterID,     INT ); //
	GProperty( FetchAmount,        INT ); //
	GProperty( RemainAmount,       INT ); // �ϰ�Ԥ��
	GProperty( NextRemainAmount,   INT ); // ��һ��Ԥ��
	GProperty( Payout,             INT ); //
	GProperty( CountAmount,        INT ); //
	GProperty( Dianka,             INT ); // �㿨����
	GProperty( StoreInAmount,      INT ); // ��Ʒ�ɹ�֧��
	GProperty( AccountPay,         INT ); // �˻�����
	GProperty( ZSBConsume,         INT ); // ���ձ�����
	GProperty( ZSBBalance,         INT ); // Ԥ������
	GProperty( DutyIncome,         INT ); // �������

};

typedef CArray<CShiftCash, CShiftCash&> CShiftCashArray;

typedef CArray<CString, CString&> COperatorArray;


//------------------------------------------------------------------------
class GPropertyClass CCheckOutinfo
{
public:
	CCheckOutinfo()
	{
		GPropertyInit3(
			MemberId,   ClassId,       BalanceMoney,
			0,          0,             0);

		GPropertyInit3(
			ClassState, GuarantyMoney, ReturnState,
			0,          0,             0);
	}
private:
	GProperty( CheckOutTime, CString ); // ����ʱ��
	GProperty( SerialNum,    CString ); // �����˺�
	GProperty( ClassName,    CString ); // �û���������
	GProperty( TerminalId,   CString ); // �ն˺�
	GProperty( UserName,     CString ); // �û�����
	GProperty( PersonalId,   CString ); // ֤������
	GProperty( ClassState,      UINT ); // �û�����״̬ 0:��ͨ�û���1:��Ա
	GProperty( MemberId,        UINT ); // �û�ID
	GProperty( ClassId,         UINT ); // �û�����ID
	GProperty( BalanceMoney ,   UINT ); // �������˿���
	GProperty( GuarantyMoney,   UINT ); // Ѻ��
	GProperty( ReturnState,     UINT ); // �˿�״̬
};

typedef CArray <CCheckOutinfo, CCheckOutinfo&> CCheckOutInfoArray;


//------------------------------------------------------------------------
class GPropertyClass CLastUserInfo
{
private:
	GProperty( TermId,     CString ); // �ն˺�
	GProperty( NetId,      CString ); // �����˺�
	GProperty( PassWord,   CString ); // ����
	GProperty( MemberId,      UINT ); // �û�ID
	GProperty( UpdateTime, CString ); // ����ʱ��
	GProperty( Suspend,       UINT ); // �һ�״̬
	GProperty( LoginStyle,    UINT );

public:
	CLastUserInfo() 
	{
		GPropertyInit3(
			MemberId, Suspend, LoginStyle,
			0,        0,       0);
	}
};

// 2011/07/20-8201-gxx: ��������״̬�������Ϣ
class GPropertyClass CLocalStatusInfo
{
public:
	GProperty( CasherAccess, CString ); // ����ԱȨ���ַ���
	GProperty( ControlParam, CString ); // ����ָ�������ɿ��Ʋ���
};

// 2011/07/14-8201-gxx: ע�᱾���û�
class GPropertyClass CLocalRegisterInfo
{
public:
	GProperty( NetbarId,      UINT ); // ����ID
	GProperty( MemberId,      UINT );
	GProperty( UserClass,     UINT ); // Ĭ����ʱ�û��������û�ֻ֧����ʱ�û�
	GProperty( NetId,      CString ); // �����˺�
	GProperty( IdType,        UINT ); // ֤������
	GProperty( PersonalId, CString ); // ֤����
	GProperty( Sex,           UINT );
	GProperty( UserName,   CString );
	GProperty( Money,         UINT ); // �������
	GProperty( Password,    CString); 

public:
	CLocalRegisterInfo()
	{
		GPropertyInit3( NetbarId, MemberId, UserClass,
			            0,        0,        0);
		GPropertyInit3( IdType, Sex, Money,
			            0,      0,   0);
	}
};

// 2011/07/18-8201-gxx: �������Ѽ�¼
class GPropertyClass CLocalConsumeInfo
{
public:
	GProperty( SubmitTime,   CString ); 
	GProperty( NetbarId,        UINT );
	GProperty( RefNo,           UINT );
	GProperty( SerialNo,        UINT );
	GProperty( SerialNum,    CString );
	GProperty( Name,         CString );
	GProperty( IdNumber,     CString );
	GProperty( MemberId,        UINT );
	GProperty( CreditAmount,    UINT );
	GProperty( ConsumeAmount,   UINT );
	GProperty( ReturnAmount,    UINT );
	GProperty( CheckinTime,  CString );
	GProperty( CheckoutTime, CString );
	GProperty( TermId,       CString );
	GProperty( TimeConsume,     UINT );  // �ɺ��ԣ�ʹ��Ĭ��
	GProperty( Operator,     CString );
	GProperty( DataStatus,       INT );  // �ɺ��ԣ�ʹ��Ĭ��
	GProperty( ClassId,         UINT );  
	GProperty( PayType,         UINT );
	GProperty( PCClass,         UINT );  // ��������ID
	GPropertyReadOnly( CheckData,    CString );

public:
	void MakeMD5();

public:
	CLocalConsumeInfo();
};

// 2011/10/25-8230-gxx: ������Ϣ
class GPropertyClass CRoomInfo
{
public:
	GProperty( RoomID,       UINT );       // ����ID�� ����������
	GProperty( RoomName,  CString );  // ��������
	GProperty( MemberID,     UINT ); // �������˺ŵ��û���,��������˺�,��ô�˰���״̬Ϊ:�����Ʒ�
	                             // ���û�����˺Ų��Ұ���������û�����ϻ�����ô����״̬Ϊ�����а���
	                             // ���û�����˺Ų��Ұ��������������ϻ�����ô����״̬Ϊ�������Ʒ�
	GProperty( NetId,     CString );    // �������˺�
	GProperty( OnlineCount,   INT );    // ������������
	GProperty( ComputerCount, INT );    // ������������
	GProperty( TermID,    CString );    // �������ĳ̨���Ե��ն˺�
	GProperty( OpenTime,  COleDateTime ); // ���Ϊ�����Ʒѣ���ô�ͱ�ʾ�����Ŀ�ͨʱ��,�������������Чʱ��

	GProperty( UserClass,     INT );    // �û�����
public:
	CRoomInfo()
	{
		GPropertyInit4(RoomID,MemberID,OnlineCount,ComputerCount,0,0,0,0);
		GPropertyInit(UserClass, 0);
	}
};
typedef CArray<CRoomInfo,CRoomInfo&> CRoomInfoList;


typedef CArray  <CLastUserInfo, CLastUserInfo&> CLastUserArray;

}