#pragma once

#include "Singleton.h"
#include "Servlet\BalanceMethod.h"

using namespace NS_SERVLET;

class CCurrentNetBarUser
{
	SINGLETON_DECLARE(CCurrentNetBarUser)

public :

	static CCurrentNetBarUser* GetInstance();

public:



	CBalanceMethod Balance;

	UINT GetMemberId() const;

	CString GetNetId();

	// ���浱ǰҪ������NetID
	void SetNetId(CString strNetID){
		m_strNetId = strNetID;
	}

	BOOL QueryBalance(LPCTSTR lpszNetId);

	INT GetLastStatusCode() const;

private:

	UINT m_nMemberId;

	INT  m_nLastStatusCode;

	CString m_strNetId;
};
