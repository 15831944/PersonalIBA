#pragma once

#include "Servlet\LastTranInfo.h"
#include "Servlet\CommonMethod.h"
#include "Servlet\CashRegisterLoginMethod.h"
#include "Servlet\DutyMethod.h"
#include "Servlet\RegisterMethod.h"
#include "Servlet\CreditMethod.h"
#include "Servlet\MemberInfoMethod.h"
#include "Servlet\DeregisterMethod.h"
#include "Servlet\BalanceMethod.h"
#include "Servlet\IDCheckCardMethod.h"
#include "Servlet\ResetPwdMethod.h"
#include "Servlet\CreditOnlineMethod.h"
#include "Servlet\ActivationMethod.h"
#include "Servlet\AssetStatusMethod.h"
#include "Servlet\BundleTimeMethod.h"
#include "Servlet\BundleTimeListMethod.h"
#include "Servlet\BundleTimeActionMethod.h"
#include "Servlet\ModifyMemberMethod.h"
#include "Servlet\NetBarInfoMethod.h"
#include "Servlet\StoreQuantityMethod.h"
#include "Servlet\RetailMethod.h"
#include "Servlet\ShiftTotalMethod.h"
#include "Servlet\CreditReverseMethod.h"
#include "Servlet\Checkout2Method.h"
#include "Servlet\CancelBundleTimeMethod.h"
#include "Servlet\CardReplaceMethod.h"
#include "Servlet\ShiftCreditMethod.h"
#include "Servlet\ConsumeListMethod.h"
#include "Servlet\TermConsumeMethod.h"
#include "Servlet\UpgradeMethod.h"
#include "Servlet\CardCreditMethod.h"
#include "Servlet\UsePointMethod.h"
#include "servlet\localmodemethod.h"
#include "Servlet\MergeMemberMethod.h"
#include "servlet\storeinmethod.h"
#include "Servlet\BundleTime2Method.h"
#include "Servlet\BundleTimeList2Method.h"
#include "Servlet\DetailDataMethod.h"
#include "Servlet\ShiftListMethod.h"
#include "Servlet\CheckInMethod.h"
#include "Servlet\CreditDirectMethod.h"

#include "BrokenNetworkResume.h"
#include "Servlet\ConsumeListMethodEx.h"

#include "BrokenNetwork.h"
#include "Servlet\LocalConsumeMethod.h"
#include "Servlet\NetBarRoomMethod.h"
#include "Servlet\RoomCheckIn.h"
#include "Servlet\RoomCheckOut.h"

using namespace NS_SERVLET;

class CCashier 
{
public:
	friend class CBrokenNetwork;

	CCashier(void);
	~CCashier(void);

protected :

	CString m_strName;		//!< ����Ա����

	CString m_strPassword;	//!< ����Ա����

	UINT m_nShiftType;      //!< ������� 1:��� 2:�а� 3:���
	
	BOOL m_bLogin;  //!<�Ƿ��¼
	UINT m_nDutyID; //!<���ID
	BOOL m_bOnDuty; //!<�Ƿ��ϰ�

	CString m_OnDutyTime; //!<�ϰ�ʱ��
	
	BOOL m_bIsBoss; //!<�Ƿ����ϰ�ģʽ

	CString m_strEnableModuleList;

	// 2011/07/20-8201-gxx: 
	CLocalStatusInfo LocalInfo;

public:	

	BOOL m_bIsLocalModeLogin;  // 2011/07/13-8201-gxx: �Ƿ��Ա���ģʽ�ķ�ʽ��¼��

private :

	virtual void WriteDutyInfo();//��������Ϣ
	void WriteLoginInfo(CString strOperator);//��¼��дע���

public :

	CString GetName() const { return m_strName; }
	void SetName(CString newVal) { m_strName = newVal; }

	CString GetPassword() const { return m_strPassword; }
	void SetPassword(CString newVal) { m_strPassword = newVal; }

	BOOL IsBoss() const { return m_bIsBoss; }
	void SetIsBoss(BOOL newVal) { m_bIsBoss = newVal; }

	BOOL IsOnDuty(); //!<�Ƿ��ϰ�
	BOOL IsLogin() const { return m_bLogin; } //!<��¼���
	
	CString GetOnDutyTime() const; //!<�ϰ�ʱ�䣬���û�ϰ��򷵻ؿ�
	UINT GetDutyID() const { return m_nDutyID; }
	UINT GetShiftType() const { return m_nShiftType; }

	virtual BOOL CheckLastDuty(CString& strLastOperator);//������������Ƿ�����
	virtual BOOL HasPopedomOf(LPCTSTR lpszModuleName); //!<Ȩ��

public :

	virtual BOOL CheckIn(CCashRegisterLoginMethod & CashRegisterLogin, CString strOperator);//����
	virtual BOOL DoOnDuty(COnDutyMethod & OnDuty);
	virtual BOOL DoOffDuty(COffDutyMethod & OffDuty);
	virtual BOOL DoRegister(CRegisterMethod & Register);
	virtual BOOL DoCredit(CCreditMethod & Credit);
	virtual BOOL DoMemberInfo(CMemberInfoMethod & MemberInfo);
	virtual BOOL DoDeregister(CDeregisterMethod & Deregister);
	virtual BOOL DoBalance(CBalanceMethod & Balance);
	virtual BOOL DoCheckCard(CIDCheckCardMethod & CheckCard);
	virtual BOOL DoResetPwd(CResetPwdMethod & ResetPwd);
	virtual BOOL DoActivation(CActivationMethod & Activation);
	virtual BOOL DoShiftTotal(CShiftTotalMethod & ShiftTotal);
	virtual BOOL DoCreditReverse(CCreditReverseMethod & CreditReverse);
	virtual BOOL DoCheckout2(CCheckout2Method & Checkout2);
	virtual BOOL DoModifyMember(CModifyMemberMethod & ModifyMember);
	virtual BOOL DoBundleTimeList(CBundleTimeListMethod & BundleTimeList);
	virtual BOOL DoBundleTimeAction(CBundleTimeActionMethod & BundleTimeAction);
	virtual BOOL DoBundleTime(CBundleTimeMethod & BundleTime);
	virtual BOOL DoCancelBundleTime(CCancelBundleTimeMethod & CancelBundleTime);
	virtual BOOL DoCardReplace(CCardReplaceMethod & CardReplace);
	virtual BOOL DoShiftCredit(CShiftCreditMethod & ShiftCredit);
	virtual BOOL DoConsumeList(CConsumeListMethod & ConsumeList);
	virtual BOOL DoTermConsume(CTermConsumeMethod & TermConsume);
	virtual BOOL DoUpgrade(CUpgradeMethod & Upgrade);
	virtual BOOL DoCardCredit(CCardCreditMethod & CardCredit);
	virtual BOOL DoStoreQuantity(CStoreQuantityMethod & StoreQuantity);
	virtual BOOL DoRetail(CRetailMethod & RetailMethod);
	virtual BOOL DoRetailOnline(CRetailOnlineMethod & RetailOnlineMethod);
	virtual BOOL DoUsePoint(CUsePointMethod & UsePoint);
	virtual BOOL DoLocalMode();// ִ�������ķ��Ͷ�����֪ͨ
	virtual BOOL DoMergeMember(CMergeMemberMethod & MergeMember);//�ϲ��˻�
	virtual BOOL DoAssetStatus();//�汾��
	virtual BOOL DoStoreIn(CStoreInMethod & StoreIn);//���
	virtual BOOL DoBundleTime2(CBundleTime2Method & BundleTime2);
	virtual BOOL DoBundleTimeList2(CBundleTimeList2Method & BundleTimeList2);
	virtual BOOL DoShiftList(CShiftListMethod & ShiftList);
	virtual BOOL DoDetailData(CDetailDataMethod & DetailData);
	virtual BOOL DoCheckIn(CCheckInMethod & CheckInData);
	virtual BOOL DoCreditDirect(CCreditDirectMethod& CreditDirect);

	// 2011/05/09-gxx: ���ձ��㲥(���������Ѽ�¼��ѯ)
	virtual BOOL DoConsumeListEx(CConsumeListMethodEx & ConsumeListEx);

	// 2011/07/19-8201-gxx: 
	virtual BOOL DoLocalConsume(CLocalConsumeMethod& LocalConsume);

	// 2011/10/25-8230-gxx: 
	virtual BOOL DoNetbarRoom(CNetbarRoomMethod& RoomMethod);

	// 2011/10/26-8230-gxx: 
	virtual BOOL DoRoomCheckIn(CRoomCheckIn& RoomCheckInMethod);

	// 2011/10/26-8230-gxx: 
	virtual BOOL DoRoomCheckOut(CRoomCheckOut& RoomCheckOutMethod);

	// 2011/07/13-8201-gxx: 
	BOOL LocalModeLogin();
};
