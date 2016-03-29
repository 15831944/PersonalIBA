#pragma once

#define __GXX_TRACE    //��������
#include "gxx_trace.h"

#define IBA_TRACE   GXX_TRACE
#define IBA_ASSERT  ASSERT
#define IBA_ASSERT2 GXX_ASSERT

//-------------------------------------------------------
#define __UNIT_TRACE   // ������Ԫ����

#ifdef __UNIT_TRACE

#define UNIT_TRACE                    GXX_TRACE
#define UNIT_AUTO_TRACE               GXX_AUTO_TRACE

#else

#define UNIT_TRACE(lpFmtStr,...)      void(0)
#define UNIT_AUTO_TRACE(lpFmtStr,...) void(0)

#endif
//-------------------------------------------------------

// ����һ��������ʽ��UINT��ʽתΪdoubleʱ��ת���ɵ����doubleֵ����һ���ܴ��ֵ��
// ���������޷�Ԥ�ϵĽ��. �˺�����޷������ݵĸ�����ʽ���

#ifdef DEBUG
#define IBA_CHECK_UINT(nUintNum) {\
	IBA_ASSERT(sizeof(nUintNum)==4);\
	int nIntN = (int)nUintNum;\
	if (nIntN<0)\
	{\
		IBA_ASSERT2(0,"�˴��޷��������������һ������,��ע����밲ȫ!");\
	}\
}
#else
#define IBA_CHECK_UINT(nUintNum) 
#endif

// ��ȫ��ת��(UINT to Double)
#define IBA_U_TO_D(nUintNum) (double((int)nUintNum))

//-------------------------------------------------------

#define IBA_LOG0 CIBALog::GetInstance()->Write
#define IBA_LOG  CIBALog::GetInstance()->WriteFormat
