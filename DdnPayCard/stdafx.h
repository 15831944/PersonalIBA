// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ͷ���ų�����ʹ�õ�����
#endif


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯����Ϊ��ʽ��

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ
#include <afxole.h>         // MFC OLE ��
#include <afxodlgs.h>       // MFC OLE �Ի�����
#include <afxdisp.h>        // MFC �Զ�����
#include <afxhtml.h>
#include <afxdhtml.h>


#include <MsXml2.h>
#include <comutil.h>
#pragma comment(lib, "comsuppw.lib")

#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")

#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#pragma warning(disable: 4267)

#define __GXX_LOG
#include "gxx_trace.h"

#include "G2XUI\\G2XIncS.h"
#ifdef DEBUG
#pragma comment(lib, "G2XUI\\G2XLibSSD.lib")
#else
#pragma comment(lib, "G2XUI\\G2XLibSS.lib")
#endif

using namespace G2XUI;
using namespace G2XUI::G2XUIFrame;

