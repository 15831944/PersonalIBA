// MsgPopupWindow.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "IBAHelpper.h"
#include "MsgPopupWindow.h"


// CMsgPopupWindow

IMPLEMENT_DYNAMIC(CMsgPopupWindow, CBCGPPopupWindow)
CMsgPopupWindow::CMsgPopupWindow()
{
	//������ʽ���
	SetAnimationType(CBCGPPopupMenu::ANIMATION_TYPE::SLIDE);
	SetAnimationSpeed(100);
	SetTransparency(255);//͸��
	SetSmallCaption(FALSE);//�����
	SetAutoCloseTime(15000);//10s�Զ��ر�
}

CMsgPopupWindow::~CMsgPopupWindow()
{
	
}


BEGIN_MESSAGE_MAP(CMsgPopupWindow, CBCGPPopupWindow)
	ON_COMMAND(ID_COPY_MSG,OnCopyText)
END_MESSAGE_MAP()

BOOL CMsgPopupWindow::OnClickLinkButton(UINT uiCmdID)
{
	AfxGetMainWnd()->PostMessage(WM_REPLYMESSAGE, uiCmdID, NULL);

	return TRUE;
}

void CMsgPopupWindow::OnCopyText()
{
	CIBAHelpper::PutTextToClipboard(m_strText);
}

BOOL CMsgPopupWindow::Create(CWnd* pWndOwner, CBCGPPopupWndParams& params, HMENU hMenu /*= NULL*/, CPoint ptPos /*= CPoint (-1, -1)*/)
{
	m_strText = params.m_strText;

	params.m_strURL = LOAD_STRING(IDS_RELPYMSG);

	VERIFY(m_Menu.LoadMenu(IDR_MSG_MENU));//����

	CMenu* pPopup = m_Menu.GetSubMenu(0);

	// �������汾
	//if (pPopup)
	//{
	//	pPopup->RemoveMenu(ID_SCAN_OTHER, MF_BYCOMMAND);
	//}
	return CBCGPPopupWindow::Create(NULL, params, pPopup->Detach(), ptPos);
}

// CMsgPopupWindow ��Ϣ�������

