
#pragma  once

namespace NS_IBACommDef{
	enum Seat2Type { MainIBA = 0, WC = 1, Entrance = 2, Refrigerator = 3, Seat2Count = 4};
	enum EUserClass{
		Euc_TempUser = 1,		//!>��ʱ�û�
		Euc_MemberUser = 2,		//!>��Ա�û�
		Euc_PostPaid = 3,		//!>�󸶷�
		Euc_Employee = 4,     //!>Ա����
	};
	// �û��ĸ�������
	enum EMemberPayType{
		EPayType_General = 0,    //!>��ͨ�Ʒ�
		EPayType_Capped = 1,     //!>�ۼƷⶥ��ʱ
		EPayType_Withholding =2,  //!>���Ԥ�۰�ʱ
		EPayType_Coupon		=3,	 //!>���װ�ʱ
		EPayType_Package	=4,	 //!>�Żݰ�ʱ
	};
	const UINT PostPaidMoney = 999900;
	const TCHAR SZPostPaidMoney[] = _T("9999");
}