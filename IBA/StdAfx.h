
#pragma once

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <afxmt.h>
#include <afxsock.h>
#include <afxtempl.h>
#include <afxdlgs.h>

#include "..\BCGCBPro\BCGCBProInc.h"			// BCGControlBar Pro

#define L2_SAFE_DELETE(p) { if (p != NULL) { delete p;  p = 0; } }

//******************************************************************************

#define L2_EXT_CLASS __declspec(dllimport)

#include "..\L2Camera\L2CameraPro.h"
#include "..\L2ADOKit\ADODBConnPool.h"
#include "..\L2ToolKit\L2ToolKitPro.h"
#include "..\L2HTMLKit\L2HTMLKitPro.h"

#include "IBA_DEBUG.h"
#include "gxx_property.h"
#include <afxhtml.h>

//*******************����������Դ��ȡ�÷�ʽ����,������壬��ô��ʹ��Ĭ����Դ����ַ���.����ʹ�õ������ַ�����Դ.
#define __USING_INTER_RESOURCE

#ifndef __USING_INTER_RESOURCE
#pragma message("                                           ����:���ڱ�������԰汾...")
#endif

#include ".\common\StrProviderLoader.h"
typedef CStringLoaderPage<CBCGPPropertyPage> CIBAPage;

#ifdef __USING_INTER_RESOURCE
#define CIBAString CString
#else
#include "IBAString.h"
#endif



//********************


//******************************************************************************

#define WM_REPLYMESSAGE			WM_APP + 1
#define WM_RECEIVECARD			WM_APP + 2
#define WM_CHILDCLOSE			WM_APP + 3
#define WM_EXECUTEREALNAME		WM_APP + 4
#define WM_OPERATIONLOG			WM_APP + 5
#define WM_TOOLBAR_LABEL_TEXT   WM_APP + 6
#define WM_UPDATE_IBA_STATUS    WM_APP + 7
#define WM_CLIENT_DIANKA        WM_APP + 8
#define WM_COMPVIEW_REFRESH		WM_APP + 9

#define WM_UPDATESEAT			(WM_APP + 11)	// ���³�����ͼ
#define WM_UPDATETERMINALINFO	(WM_APP + 12)	//�����ն���Ϣ
const int C_CANCLOSE		= 888;		//�Ƿ�������̱��ر�,����̨û���յ�����Ϣʱ,����
//SFREG,��֤��������
const int C_SFREGSTARTED	= 889;		//SFREG�����ɹ�

#ifdef _DEBUG
#define OPEN_GLW_CARD_TESTING
#endif

#ifdef OPEN_GLW_CARD_TESTING
#pragma message("                    Debug������GLW����...")
#endif
