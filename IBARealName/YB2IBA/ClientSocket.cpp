// ClientSocket.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "YB2IBA.h"
#include ".\clientsocket.h"


// CClientSocket

CClientSocket::CClientSocket()
{
}

CClientSocket::~CClientSocket()
{
}


// CClientSocket ��Ա����

void CClientSocket::OnConnect(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���

	CAsyncSocket::OnConnect(nErrorCode);
}

void CClientSocket::OnReceive(int nErrorCode)
{
	AfxGetMainWnd()->SendMessage(WM_SOCKETNOTIFY, 2);

	CAsyncSocket::OnReceive(nErrorCode);
}
