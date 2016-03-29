#pragma once

#include "RealNameBase.h"
#include ".\IZd.h"

namespace NS_REALNAME
{

class CZDRealName : public CRealNameBase
{
public:
	CZDRealName(void);
	virtual ~CZDRealName(void);

	virtual BOOL InitRealName();

	virtual BOOL UnInitRealName();
	
	virtual BOOL CheckRealName();

	virtual INT  OnRealNameMsg(UINT nMsgType, LPVOID lpData = NULL, DWORD dwLen = 0);

	//!<�Ƿ�������
	virtual BOOL IsRegisterEnabled();

	//!<�Ƿ�����ͻ��˵�¼
	virtual BOOL IsCheckInEnabled();

	virtual void OnCheckOut(const CheckoutInfo* pCheckoutInfo);

	BOOL OnOpenCloseUser(int nType, CIZd::T_IC_USER* pICUser);

	BOOL OnLoginOut(int nType, CIZd::T_IC_LOGIN* ptLogin);

	INT OnOpenUser(CNetBarUser & NetBarUser);

	INT OnCloseUser(CNetBarUser & NetBarUser);

	BOOL CheckTakeUp(CString strTermId);

	void CheckoutOldComputer(const CString& strTermID, int nMemberId);

	BOOL CheckIn(LPCTSTR szTermID, LPCTSTR szComputerIP, LPCTSTR szNetId, LPCTSTR szPwd, int nMemberId);

	BOOL AutoChangeRealNamePwd(CNetBarUser& NetBarUser);
private:

	//!<�Զ�����ʵ������
	// virtual BOOL AutoModifyMemberInfo();
};

}