#pragma once

#include "Socket\BufferProcess.h"

//
//class:CActiveMember  ��û���(������:�û��Ļ�������)
//
class GPropertyClass CActiveMember
{
	friend class CActiveMemberList;

public:
	CActiveMember(void);
	~CActiveMember(void);

	// �û���״̬
	enum EMemberStatus{
		EStatus_Waiting = 0 ,    //!>���ϻ�
		EStatus_LocalOnline =1,  //!>�������� (UpdateDataTime��ʾISERVER�ϱ�ʱ��)
		EStatus_CenterOnLine =2, //!>�������� (UpdateDataTime��ʾ�´������Ĳ�ѯʱ��)
		EStatus_AutoReturn =3,   //!>�Զ��˿� (UpdateDataTime��ʾ�»�ʱ��)
		EStatus_RoomAutoReturn = 4 // 2011/11/01-8210-gxx: �����û����Զ��˿��̨
	};
	// �û��ĸ�������
	enum EMemberPayType{
		EPayType_General = 0,    //!>��ͨ�Ʒ�
		EPayType_Capped = 1,     //!>�ۼƷⶥ��ʱ
		EPayType_Withholding =2  //!>���Ԥ�۰�ʱ
	};

	//-----------------------����-------------------------//
public:
	GProperty( MemberID,               UINT ); // �û�ID
	GProperty( NetId,               CString ); // �����˺�
	GProperty( PersonalID,          CString ); // ֤������
	GProperty( IdType,                 UINT ); // ֤������
	GProperty( NetbarID,               UINT ); // ����ID
	GProperty( CasherID,               UINT ); // 2011-3-29-gxx-����̨ID
	GProperty( RefNo,                  UINT ); // ���ú�
	GProperty( Sex,                    UINT ); // �Ա�
	GProperty( CreditProcess,       CString ); // ����
	GProperty( AdvancePayment,          INT ); // Ԥ����
	GProperty( Remark,              CString ); // ��ע
	GProperty( IsRegisterBT,           BOOL ); // �Ƿ񿪻���ʱ
	GProperty( PayType,                UINT ); // ���ʽ
	GProperty( IsOnLine,               BOOL ); // �ϻ�
	GProperty( PCClassID,              UINT ); // �ϻ�����ID
	GProperty( Amount,                  INT ); // ����
	GProperty( CheckInTime,    COleDateTime ); // �ϻ�ʱ��
	GProperty( UpdateDataTime, COleDateTime ); // ���һ�εĸ���ʱ��
	GProperty( ActivationTime, COleDateTime ); // ����ʱ��
	GProperty( NextCheckinTime,        UINT ); // 2011/08/10-8201-gxx: ��һ���ϻ�ʱ�䣬��ʱʱ�õ�
	GProperty( DynamicMaxAmount,        INT ); // 2011/07/08-8201-gxx: �ۼƷⶥ�� 
	GProperty( IsLocalUser,            BOOL ); // 2011/07/08-8201-gxx: �Ƿ�Ϊ�����û�
	GProperty( Password,            CString ); // 2011/07/08-8201-gxx: �ϻ�����
	GProperty( AvailavleBalance,        INT ); // ���
	GProperty( NetIdAlias,          CString ); // 2011/07/12-8201-gxx: �����˺ű���, ����̨��������,Ϊ��д�����ݿ⣬����ͻ��˷���
	GProperty( PolicyInfo,          CString ); // 2011/07/18-8201-gxx: ������Ϣ

	GPropertyNeedSet( TerminalID,   CString ); // �ն˺�

	GPropertyNeedGet( UserName,     CString ); // �û�����
	GPropertyNeedSet( ClassId,         UINT ); // �û�����ID

	GPropertyReadOnly(UserClassName, CString); // �û���������

	GProperty( RoomMemberId,            UINT); // �����������û���
	GProperty( ScanType,                 INT); // ɨ������


	//-----------------------����-------------------------//
public:
	void Attach(MANAGER_COMPUT_INFO & computerInfo);

	BOOL LoadFromDB(CString strNetId);

	//���ؿ���������
	BOOL IsOffLinePossibility(); 

	BOOL IsMember(); //��Ա

	BOOL IsNeedToQuery(); //���ûǮ��

	//!> ��ȡ�û�״̬  EMemberStatus
	INT GetStatus() const { return m_nStatus; }
	//!> �����û�״̬  EMemberStatus
	void SetStatus(INT newVal) { m_nStatus = newVal; }

	BOOL IsActive(); //��������
	void SetActive(BOOL newVal) { m_bIsActive = newVal; }

	
	void SetTerminalID(CString newVal);	

	CString GetTermIdInfo();
	
	
	CString GetUserName(BOOL bWithSex = FALSE);

	
	CString GetPayTypeAsString();

	CString GetAvailavleBalanceAsString();

	
	CString GetPCClass(); // �ϻ���������

	CString GetSexAsString(); 

	
	CString GetAmountAsString();

	CString GetCheckInTimeAsString();
	CString GetUseTimeAsString();
	
	
	void SetClassId(UINT newVal);

	CString GetActivationTimeAsString();
	CString GetAdvancePaymentAsString();

	// 2011/05/16-gxx: ��ȡ�Զ��˿�ʱ��
	static UINT GetRefundmentTime();

	// 2011/07/18-8201-gxx: ��ȡ���ط���, bFilter�����Ƿ����0.5Ԫ������ȡ������
	UINT GetLocalExpense(BOOL bFilter=TRUE);

private:

	INT m_nStatus;

	BOOL m_bIsActive;
};
