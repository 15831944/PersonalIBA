#pragma once

#include "RealNameBase.h"
#include "..\Servlet\IDCheckCardMethod.h"

namespace NS_REALNAME
{

class CGLWRealName : public CRealNameBase
{
public:

	CGLWRealName(void);

	virtual ~CGLWRealName(void);

	//!<ʵ����ʼ��
	virtual BOOL InitRealName();

	//!<����ʵ����ع���
	virtual INT  InvokeRealName(UINT nMsgType, LPVOID lpData = NULL, DWORD dwLen = 0);

	virtual INT OnRealNameMsg(UINT nMsgType, LPVOID lpData = NULL, DWORD dwLen = 0);

	//!<���ڶ���ִ��ˢ����Ϣ
	virtual INT  OnExecuteMsg(UINT nExecuteType, LPVOID lpData = NULL, DWORD dwLen = 0);

	//!<�Ƿ�����ʵ���˵�
	virtual BOOL IsMenuEnabled();

	//!<�Ƿ���ҪУ�鿪��ʵ������
	virtual BOOL NeedToCheckRegisterData();

protected:

	virtual BOOL OnCardInfo(DataX_UserInfo& dxUserInfo);

	virtual BOOL OnForceCheckOut(DataX_UserInfo& dxUserInfo);

	virtual BOOL ReadUserInfo(CNetBarUser & NetBarUser);
private:
	
	//������ĺ�ʵ���������Ƿ���ͬ
	BOOL AutoChangeRealNamePwd(CNetBarUser& NetBarUser);

	INT ChangeCertificateType(INT nTmp);

	

	INT OnCloseUser(CNetBarUser & NetBarUser);

	INT OnOpenUser(CNetBarUser & NetBarUser);

	BOOL SendMsgToGLW(UINT nMsgType);
};

}