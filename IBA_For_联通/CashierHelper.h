#pragma once

class CActiveMember;

namespace NS_DAL{
class CCheckOutinfo;
class CLocalConsumeInfo;
}


class CCashierHelper
{
public:
	CCashierHelper(void);
	~CCashierHelper(void);

	static void MakeCheckOutInfo(CActiveMember& , NS_DAL::CCheckOutinfo&);

	static void MakeLocalConsumeInfo(CActiveMember& , NS_DAL::CLocalConsumeInfo&);

	static BOOL ReturnLocalUser(CActiveMember&, NS_DAL::CLocalConsumeInfo* lpLocalInfo = NULL );

	static BOOL ReturnLocalCenterUser(CActiveMember& , NS_DAL::CCheckOutinfo* lpCheckOut = NULL );

	static BOOL CreditLocalUser(CActiveMember&, UINT nCreditMoeny);

public:
	static BOOL InitDiankaPlatform();
	static BOOL UninitDiankaPlatform();

	// �㿨����
	static BOOL SellDianka();

	// ����ͻ��˷��͹������ֽ���㿨������
	static BOOL CashSellDianka(LPCTSTR lpURL);

	
};
