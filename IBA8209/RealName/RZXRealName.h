#pragma once

#include "RealNameBase.h"

namespace NS_REALNAME
{

class CRZXRealName : public CRealNameBase
{
public:
	CRZXRealName(void);
	virtual ~CRZXRealName(void);

	// 2014-4-25 - qsc
	static int GetNationCode(const CString& strNation);
	static CString GetBirthdayFromSFZ( const CString& sfzh );

	virtual INT OnRealNameMsg(UINT nMsgType, LPVOID lpData = NULL, DWORD dwLen = 0);

	virtual INT InvokeRealName(UINT nMsgType, LPVOID lpData = NULL, DWORD dwLen = 0);

	virtual BOOL InitRealName();

	virtual BOOL UnInitRealName();

	virtual BOOL UploadImage(UINT nType, CNetBarUser& NetBarUser, UINT nOption = 0);

	virtual BOOL AutoMatchIDCard(CNetBarUser & NetBarUser);

	//!<�Ƿ�����ʵ���˵�
	virtual BOOL IsMenuEnabled();

	BOOL AutoChangeRealNamePwd(CNetBarUser& NetBarUser);
	//!<�Ƿ�����ʵ����չ�˵�
	virtual BOOL IsRealNameExtendMenuEnabled();

	//!<ˢ�����Ƿ�������
	virtual BOOL RobFocusAfterSwipingCard();

	static BOOL GetPhotoPath(CString& strFile, const CString& strID );

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