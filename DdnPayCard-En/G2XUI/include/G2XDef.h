#pragma once

/**************************************************************************
					G2XLib��UI�ؼ���windows�Ĵ��ڷ�����Ϣ�Ĵ����
**************************************************************************/

#define GVWN_BN_CLICKING   1
#define GVWN_BN_CLICKED    2
#define GVWN_BN_RCLICKING  3
#define GVWN_BN_RCLICKED   4

#define ON_GVWN_NORIFY(id, code, memberFxn) \
	{ WM_COMMAND, (WORD)code, \
	(WORD)id, (WORD)id, \
	AfxSigCmd_v, \
	(static_cast< AFX_PMSG > (memberFxn)) },

#define ON_GVWN_NORIFY_RANGE(idFrom,idTo,code,memberFxn) \
	{ WM_COMMAND, code,\
	(WORD)idFrom,(WORD)idTo,\
	AfxSigCmd_RANGE, \
	(AFX_PMSG)(static_cast< void (AFX_MSG_CALL CCmdTarget::*)(UINT) > (memberFxn)) },

#define ON_GVWN_CLICKING(id, memberFxn)    ON_GVWN_NORIFY(id, GVWN_BN_CLICKING, memberFxn)
#define ON_GVWN_CLICKED(id, memberFxn)     ON_GVWN_NORIFY(id, GVWN_BN_CLICKED, memberFxn)
#define ON_GVWN_RCLICKING(id, memberFxn)   ON_GVWN_NORIFY(id, GVWN_BN_RCLICKING, memberFxn)
#define ON_GVWN_RCLICKED(id, memberFxn)    ON_GVWN_NORIFY(id, GVWN_BN_RCLICKED, memberFxn)

#define ON_GVWN_CLICKING_RANGE(idFrom,idTo,memberFxn)   ON_GVWN_NORIFY_RANGE(idFrom,idTo,GVWN_BN_CLICKING,memberFxn)
#define ON_GVWN_CLICKED_RANGE(idFrom,idTo,memberFxn)    ON_GVWN_NORIFY_RANGE(idFrom,idTo,GVWN_BN_CLICKED,memberFxn)
#define ON_GVWN_RCLICKING_RANGE(idFrom,idTo,memberFxn)  ON_GVWN_NORIFY_RANGE(idFrom,idTo,GVWN_BN_RCLICKING,memberFxn)
#define ON_GVWN_RCLICKED_RANGE(idFrom,idTo,memberFxn)   ON_GVWN_NORIFY_RANGE(idFrom,idTo,GVWN_BN_RCLICKED,memberFxn)

#define GVWM_SCROLL_POSITION_CHANGE 1001

/*************************************************************************
					 G2XLib��UI�ڲ���Ϣ������ض���
**************************************************************************/
G2XUI_BEGIN

#ifndef GDI_MSG_CALL
#define GDI_MSG_CALL
#endif

typedef void (GDI_MSG_CALL CObject::*GDI_PMSG)(void);

struct GDI_MSGMAP_ENTRY
{
	UINT nMsg;
	WORD msgType;
	UINT nID;
	UINT nCode;
	GDI_PMSG pfn;
};
struct GDI_MSGMAP
{
	const GDI_MSGMAP* pBaseMap;
	const GDI_MSGMAP_ENTRY* pTheClass;
};

#define G2XMASK_VK 0x01
#define G2XMASK_CONTROL 0x02
#define G2XMASK_ALT 0x04
#define G2XMASK_SHIFT 0x08
#define G2XMASK_WINDOW 0x10

#define G2XACCFLAG_MAKE(byMask,key) (((DWORD(byMask))<<16)|key)
#define G2XACCFLAG_MASK(flag) (BYTE((DWORD(flag))>>16))


struct GDI_ACCELERATOR
{
	DWORD dwAccFlag; // hiword: ����, loword: ��
	DWORD dwCmd;     // command
};

#define G2X_COMMAND_FIRST 100
#define G2X_COMMAND_USER  1024 // �û��Զ����IDӦ�ô���G2X_COMMAND_USER
#define G2X_COMMAND_LAST  10000

#define G2X_ID_COPY			101 // ��������
#define G2X_ID_CUT			102 // ����
#define G2X_ID_PASTE		103 // ճ��
#define G2X_ID_CTRL_HOME	104 // Ctrl+Home
#define G2X_ID_CTRL_END		105 // Ctrl+End
#define G2X_ID_SHIFT_HOME	106 // Shift+Home
#define G2X_ID_SHIFT_END	107 // Shift+End

//-------------------------------------------------------------------------
//- G2XLib ��Ϣӳ�亯�����͵Ķ���
//-------------------------------------------------------------------------
#define GDI_FUN_TYPE_end    0
#define GDI_FUN_TYPE_vv     1       // void CG2XObject::Fun(void)
#define GDI_FUN_TYPE_lww    2       // long CG2XObject::Fun(WPARAM wParam,LPARAM lParam)
#define GDI_FUN_TYPE_Notify 3       // void CG2XObject::Fun(NMHDR* pHdr,LONG* lRes)
#define GDI_FUN_TYPE_vup    4       // void CG2XObject::Fun(UINT,CPoint)
#define GDI_FUN_TYPE_vu     5       // void CG2XObject::Fun(UINT)
#define GDI_FUN_TYPE_vupp   6       // void CG2XObject::Fun(UINT,CPoint,CPoint)
#define GDI_FUN_TYPE_vusp   7       // void CG2XObject::Fun(UINT,short,CPoint);
#define GDI_FUN_TYPE_buuu 8         // BOOL CGdiVirtualObject::Fun(UINT,UINT,UINT)
#define GDI_FUN_TYPE_vu_range 9     // void CG2XObject::Fun(UINT), ��Χ�ڵ�id


//-------------------------------------------------------------------------
//- G2XLib ��Ϣ���͵Ķ���
//-------------------------------------------------------------------------
#define GDIM_MESSAGE 1              // ��ͨ����Ϣ
#define GDIM_MOUSE 2                // �������¼�����Ϣ
#define GDIM_COMMAND 3              // ������Ϣ
#define GDIM_NOTIFY 4               // ֪ͨ��Ϣ
#define GDIM_KEYBOARD 5             // ������Ϣ

//-------------------------------------------------------------------------
//- G2XWnd��������Ϣ�Ķ���
//-------------------------------------------------------------------------

//- ��ͨ��Ϣ����ض���
#define G2XM_CREATE   1          // G2XWnd������
#define G2XM_SIZE     2          // G2XWnd�Ĵ�С�����ı�
#define G2XM_SHOWWND  3          // ������ʾ/����
#define G2XM_CLICKING 4          // ĳG2XWnd���ڷ��������¼�
#define G2XM_CLICKED  5          // ĳG2XWnd�ѷ��������¼�
#define G2XM_DESTORY  6          // G2XWnd������

//- ��������Ϣ����, ע:���������Ϣ�Ķ������windows�Ķ���, ����: WM_MOUSE_MOVE
#define G2XM_DRAGGING 10         // ������ק
#define G2XM_DRAGGED  11         // ��ק���

#define G2XM_DESKTOPWND_KILLFOCUS 12    // CG2XDesktopWnd����ʧȥ����

//- ���ڿؼ��¼���Ϣ
#define G2XM_CALENDARDROPDOWNCLOSING 21 // ���ڿؼ������������������ڱ��ر�
										// wparam: CG2XCalendarWndָ��
#define G2XM_CALENDARDROPDOWNCLOSED  22 // ���ڿؼ����������������Ѿ����ر�
										// wparam: CG2XCalendarWndָ��

/* CG2XListBox */
#define G2XM_LISTBOX_ITEM_CLICKING   101 // ListBox�������������,		wparam: �ؼ�ID, lparam: ����
#define G2XM_LISTBOX_ITEM_CLICKED    102 // ListBox���������º��ͷ�,	wparam: �ؼ�ID, lparam: ����
#define G2XM_LISTBOX_SEL_CHANGING    103 // ѡ�����������ı�,			wparam: �ؼ�ID, lparam: ��word��ʾ�ı�ǰ����������word��ʾ�ı���
#define G2XM_LISTBOX_SEL_CHANGED     104 // ѡ������ѷ����ı�,			wparam: �ؼ�ID, lparam: ��word��ʾ�ı�ǰ����������word��ʾ�ı���

/* CG2XDropList */
#define G2XM_DROP_LIST_CLOSING		 111
#define G2XM_DROP_LIST_CLOSED		 112	
#define G2XM_DROP_LIST_ITEM_CHANGE   113

/* CG2XTreeCtrl */
#define G2XM_TREE_ITEM_CLICKING      31  // wparam: CG2XTreeCtrl ָ��, lparam: CG2XTreeNode ָ��
#define G2XM_TREE_ITEM_CLICKED       32  // wparam: CG2XTreeCtrl ָ��, lparam: CG2XTreeNode ָ��

#define G2XN_CLICKING 1
#define G2XN_CLICKED  2
#define G2XN_RCLICKING 3
#define G2XN_RCLICKED  4

//- ������
struct GDI_SCROLLBAR_NMHDR
{
	NMHDR hdr;
	HG2XWND hScrollBar;
	INT nOldPos;
	INT nNewPos;
};
#define GDIN_SCROLLBAR_POS_CHANGE 0x0004

#define DECLARE_GDI_MESSAGE_MAP() \
private: \
	static const GDI_MSGMAP_ENTRY _messageEntries[]; \
protected: \
	static const GDI_MSGMAP msgMap;\
	virtual const GDI_MSGMAP* GetMessageMap() const; 

#define BEGIN_GDI_MESSAGE_MAP(theClass,baseClass) \
	const GDI_MSGMAP* theClass::GetMessageMap() const\
{  return &msgMap; }\
	AFX_COMDAT const GDI_MSGMAP theClass::msgMap = \
{ &baseClass::msgMap, &theClass::_messageEntries[0] };\
	AFX_COMDAT const GDI_MSGMAP_ENTRY theClass::_messageEntries[] = \
{ 

#define END_GDI_MESSAGE_MAP() \
{0,GDI_FUN_TYPE_end, 0, 0, (GDI_PMSG)0 } \
};

#define ON_GDI_MSG_MESSAGE(nMsg,memberFxn) \
{GDIM_MESSAGE, GDI_FUN_TYPE_lww, nMsg, 0, \
	(GDI_PMSG)\
	(static_cast< LONG (GDI_MSG_CALL CG2XWnd::*)(WPARAM,LPARAM) > \
	(memberFxn)) },

#define ON_GDI_MSG_COMMAND(nID,memberFxn) \
{GDIM_COMMAND, GDI_FUN_TYPE_vv, nID, 0, \
	(GDI_PMSG) \
	(static_cast< void (GDI_MSG_CALL CG2XWnd::*)(void) > \
	(memberFxn)) },

#define ON_GDI_MSG_NOTIFY(nID,nCode,memberFxn) \
{GDIM_NOTIFY, GDI_FUN_TYPE_Notify, nID, nCode, \
	(GDI_PMSG)\
	(static_cast< void (GDI_MSG_CALL CG2XWnd::*)(NMHDR*,LONG*) > \
	(memberFxn)) },

#define ON_GDI_CREATE() \
{GDIM_MESSAGE, GDI_FUN_TYPE_vv, G2XM_CREATE, 0,\
	(GDI_PMSG)\
	(static_cast< void (GDI_MSG_CALL CG2XWnd::*)(void) > \
	(OnCreate)) },

#define ON_GDI_LBUTTON_DOWN() \
{GDIM_MOUSE, GDI_FUN_TYPE_vup, WM_LBUTTONDOWN, 0,\
	(GDI_PMSG)\
	(static_cast< void (GDI_MSG_CALL CG2XWnd::*)(UINT,CPoint) > \
	(OnLButtonDown)) },

#define ON_GDI_LBUTTON_DOWN() \
{GDIM_MOUSE, GDI_FUN_TYPE_vup, WM_LBUTTONDOWN, 0,\
	(GDI_PMSG)\
	(static_cast< void (GDI_MSG_CALL CG2XWnd::*)(UINT,CPoint) > \
	(OnLButtonDown)) },

#define ON_GDI_LBUTTON_UP() \
{GDIM_MOUSE, GDI_FUN_TYPE_vup, WM_LBUTTONUP, 0,\
	(GDI_PMSG)\
	(static_cast< void (GDI_MSG_CALL CG2XWnd::*)(UINT,CPoint) > \
	(OnLButtonUp)) },

#define ON_GDI_LBUTTON_DCLICK() \
{GDIM_MOUSE, GDI_FUN_TYPE_vup, WM_LBUTTONDBLCLK, 0,\
	(GDI_PMSG)\
	(static_cast< void (GDI_MSG_CALL CG2XWnd::*)(UINT,CPoint) > \
	(OnLButtonDClick)) },

#define ON_GDI_RBUTTON_DOWN() \
{GDIM_MOUSE, GDI_FUN_TYPE_vup, WM_RBUTTONDOWN, 0,\
	(GDI_PMSG)\
	(static_cast< void (GDI_MSG_CALL CG2XWnd::*)(UINT,CPoint) > \
	(OnRButtonDown)) },

#define ON_GDI_RBUTTON_UP() \
{GDIM_MOUSE, GDI_FUN_TYPE_vup, WM_RBUTTONUP, 0,\
	(GDI_PMSG)\
	(static_cast< void (GDI_MSG_CALL CG2XWnd::*)(UINT,CPoint) > \
	(OnRButtonUp)) },

#define ON_GDI_RBUTTON_DCLICK() \
{GDIM_MOUSE, GDI_FUN_TYPE_vup, WM_RBUTTONDBLCLK, 0,\
	(GDI_PMSG)\
	(static_cast< void (GDI_MSG_CALL CG2XWnd::*)(UINT,CPoint) > \
	(OnRButtonDClick)) },

#define ON_GDI_MOUSE_HOVER() \
{GDIM_MOUSE, GDI_FUN_TYPE_vup, WM_MOUSEHOVER, 0,\
	(GDI_PMSG)\
	(static_cast< void (GDI_MSG_CALL CG2XWnd::*)(UINT,CPoint) > \
	(OnMouseHover)) },

#define ON_GDI_MOUSE_LEAVE() \
{GDIM_MOUSE, GDI_FUN_TYPE_vv, WM_MOUSELEAVE, 0,\
	(GDI_PMSG)\
	(static_cast< void (GDI_MSG_CALL CG2XWnd::*)(void) > \
	(OnMouseLeave)) },

#define ON_GDI_MOUSE_MOVE() \
{GDIM_MOUSE, GDI_FUN_TYPE_vup, WM_MOUSEMOVE, 0,\
	(GDI_PMSG)\
	(static_cast< void (GDI_MSG_CALL CG2XWnd::*)(UINT,CPoint) > \
	(OnMouseMove)) },

#define ON_GDI_MOUSE_WHEEL() \
{GDIM_MOUSE, GDI_FUN_TYPE_vusp, WM_MOUSEWHEEL, 0,\
	(GDI_PMSG)\
	(static_cast< void (GDI_MSG_CALL CG2XWnd::*)(UINT,short,CPoint) > \
	(OnMouseWheel)) },

#define ON_GDI_DRAGGING() \
{GDIM_MOUSE, GDI_FUN_TYPE_vupp, G2XM_DRAGGING, 0,\
	(GDI_PMSG)\
	(static_cast< void (GDI_MSG_CALL CG2XWnd::*)(UINT,CPoint,CPoint) > \
	(OnDragging)) },

#define ON_GDI_DRAGGED() \
{GDIM_MOUSE, GDI_FUN_TYPE_vupp, G2XM_DRAGGED, 0,\
	(GDI_PMSG)\
	(static_cast< void (GDI_MSG_CALL CG2XWnd::*)(UINT,CPoint,CPoint) > \
	(OnDragged)) },

#define ON_GDI_TIMER() \
{GDIM_MESSAGE, GDI_FUN_TYPE_vu, WM_TIMER, 0,\
	(GDI_PMSG)\
	(static_cast< void (GDI_MSG_CALL CG2XWnd::*)(UINT) > \
	(OnTimer)) },

#define ON_GDI_KEYDOWN() \
{GDIM_KEYBOARD, GDI_FUN_TYPE_buuu, WM_KEYDOWN, 0,\
	(GDI_PMSG)\
	(static_cast< BOOL (GDI_MSG_CALL CG2XWnd::*)(UINT,UINT,UINT) > \
	(OnKeyDown)) },

#define ON_GDI_KEYUP() \
{GDIM_KEYBOARD, GDI_FUN_TYPE_buuu, WM_KEYUP, 0,\
	(GDI_PMSG)\
	(static_cast< BOOL (GDI_MSG_CALL CG2XWnd::*)(UINT,UINT,UINT) > \
	(OnKeyDown)) },

#define ON_GDI_SIZE() \
{GDIM_MESSAGE, GDI_FUN_TYPE_vv, G2XM_SIZE, 0,\
	(GDI_PMSG)\
	(static_cast< void (GDI_MSG_CALL CG2XWnd::*)(void) > \
	(OnSize)) },

#define ON_GDI_SHOWWND() \
{GDIM_MESSAGE, GDI_FUN_TYPE_vv, G2XM_SHOWWND, 0,\
	(GDI_PMSG)\
	(static_cast< void (GDI_MSG_CALL CG2XWnd::*)(void) > \
	(OnShowWnd)) },

#define ON_GDI_DESTORY() \
{GDIM_MESSAGE, GDI_FUN_TYPE_vv, G2XM_DESTORY, 0,\
	(GDI_PMSG)\
	(static_cast< void (GDI_MSG_CALL CG2XWnd::*)(void) > \
	(OnDestroy)) },

#define ON_G2XN_CLICKING(nID,memberFxn) \
{GDIM_NOTIFY, GDI_FUN_TYPE_vv, nID, G2XN_CLICKING, \
	(GDI_PMSG) \
	(static_cast< void (GDI_MSG_CALL CG2XWnd::*)(void) > \
	(memberFxn)) },

#define ON_G2XN_CLICKED(nID,memberFxn) \
{GDIM_NOTIFY, GDI_FUN_TYPE_vv, nID, G2XN_CLICKED, \
	(GDI_PMSG) \
	(static_cast< void (GDI_MSG_CALL CG2XWnd::*)(void) > \
	(memberFxn)) },

#define ON_G2XN_CLICKING_RANGE(nIDFrom, nIDTo, memberFxn) \
{GDIM_NOTIFY, GDI_FUN_TYPE_vu_range, MAKELONG(nIDFrom,nIDTo), G2XN_CLICKING, \
	(GDI_PMSG) \
	(static_cast< void (GDI_MSG_CALL CG2XWnd::*)(UINT) > \
	(memberFxn)) },

#define ON_G2XN_CLICKED_RANGE(nIDFrom, nIDTo, memberFxn) \
{GDIM_NOTIFY, GDI_FUN_TYPE_vu_range, MAKELONG(nIDFrom,nIDTo), G2XN_CLICKED, \
	(GDI_PMSG) \
	(static_cast< void (GDI_MSG_CALL CG2XWnd::*)(UINT) > \
	(memberFxn)) },


G2X_END