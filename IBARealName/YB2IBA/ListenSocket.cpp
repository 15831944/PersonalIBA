// ListenSocket.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "YB2IBA.h"
#include ".\listensocket.h"


// CListenSocket

CListenSocket::CListenSocket()
{
}

CListenSocket::~CListenSocket()
{
}


// CListenSocket ��Ա����

void CListenSocket::OnAccept(int nErrorCode)
{
	AfxGetMainWnd()->SendMessage(WM_SOCKETNOTIFY, 1);

	CAsyncSocket::OnAccept(nErrorCode);
}
