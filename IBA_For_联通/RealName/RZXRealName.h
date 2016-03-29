#pragma once

#include "RealNameBase.h"

namespace NS_REALNAME
{

class CRZXRealName : public CRealNameBase
{
public:
	CRZXRealName(void);
	virtual ~CRZXRealName(void);

	virtual INT OnRealNameMsg(UINT nMsgType, LPVOID lpData = NULL, DWORD dwLen = 0);

	virtual INT InvokeRealName(UINT nMsgType, LPVOID lpData = NULL, DWORD dwLen = 0);

	virtual BOOL InitRealName();

	virtual BOOL UnInitRealName();

	virtual BOOL UploadImage(UINT nType, CNetBarUser& NetBarUser, UINT nOption = 0);

	//!<�Ƿ�����ʵ���˵�
	virtual BOOL IsMenuEnabled();

	//!<�Ƿ�����ʵ����չ�˵�
	virtual BOOL IsRealNameExtendMenuEnabled();

	//!<ˢ�����Ƿ�������
	virtual BOOL RobFocusAfterSwipingCard();

private:

	void Load();

	void UnLoad();

	INT  CheckOutTempUser(LPCTSTR lpCardNum);

	BOOL SendMsgToRZX(UINT nMsgType);

private:

	HMODULE m_hIdLogUpload;

	typedef INT (__stdcall *pfnRzx2008RzxCheckOut)(LPCSTR);

	pfnRzx2008RzxCheckOut m_pfnRzxCheckOut;

};

}