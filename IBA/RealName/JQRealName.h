#pragma once

#include "RealNameBase.h"
#include "..\Servlet\IDCheckCardMethod.h"

namespace NS_REALNAME
{

class CJQRealName : public CRealNameBase
{
public:

	CJQRealName(void);

	virtual ~CJQRealName(void);

	//!<ʵ����ʼ��
	virtual BOOL InitRealName();

	virtual INT OnRealNameMsg(UINT nMsgType, LPVOID lpData = NULL, DWORD dwLen = 0);

	//!<���ڶ���ִ��ˢ����Ϣ
	virtual INT  OnExecuteMsg(UINT nExecuteType, LPVOID lpData = NULL, DWORD dwLen = 0);

	//!<�Ƿ�����ʵ���˵�
	virtual BOOL IsMenuEnabled();

protected:

	virtual BOOL ReadUserInfo(CNetBarUser & NetBarUser);
private:
	
	//������ĺ�ʵ���������Ƿ���ͬ
	BOOL AutoChangeRealNamePwd(CNetBarUser& NetBarUser);

	INT ChangeCertificateType(INT nTmp);

	INT OnCloseUser(CNetBarUser & NetBarUser);

	INT OnOpenUser(CNetBarUser & NetBarUser);
};

}