//**********************************************************************
//>gxx_trace.h  
//__________________________________________________________________________________________________
//>�Զ������( ��_DEBUGģʽ�²��и������ )
//__________________________________________________________________________________________________
//>����:���� __gxx_trace__ ������������ʵ�ָ��ٵ�����͸��ٵ��ۼ�,��gxx_trace.cpp�ж�����
//        __gxx_trace__��һ��ȫ�ֶ��� __gxx_trace,��������ֱ��ʹ��__gxx_trace.

//>ʹ��˵��: ��ʹ�� �Զ������֮ǰ������ __GXX_TRACE�Ķ��壬����˵�ٰ��� gxx_trace.h֮ǰ
//       ���붨��__GXX_TRACE������ �Զ�����ٹ��ܣ�����Ҫ���ٹ���ʱ��ֻ��Ҫȡ��__GXX_TRACE
//       �Ķ��弴�ɣ�����Ķ���Ŀ�е�Դ���룩���� GXX_TRACE �����м�ʱ���٣���GXX_TRACE_COUNT
//       �������ۼƸ��٣�ʹ���ۼƸ���ʱ��һֱҪ�ȵ���һ����ͬ���ٵ����Ż������ͨ����������
//       �����ض��������أ��Դﵽѡ��ͬ�����Ŀ��.
//>ע��gxx_trace.h��������� stdafx.h�У��ڰ��� gxx_trace.h֮ǰ ͨ���Ƿ���__GXX_TRACE��
//       �����Ƿ�������. ������忪�أ�һ��Ҫע��رո��ٹ���ʱ��Ҫ�ر���Ӧ�Ŀ��ء�
//       ��Ҫֱ��ʹ��__gxx_trace,��Ҫ����Ŀ�����ط�����__gxx_trace__����.
//       ʹ��GXX_TRACE_COUNTʱ�����ڼ����Ľ�������δ֪�ģ����Ա�������GXX_TRACE���ã�GXX_TRACE_COUNT�����ݲŻ����
//>�ؼ���: 
//		__GXX_TRACE: ������GXX���ٺ�Ŀ��أ�����ڰ���gxx_trace.h֮ǰ����˺��ʾ�򿪵����еĸ��ٹ���
//      GXX_TRACE_SETTING_NAME: ���������ʱ��ʾ��һ����ǰ����б��⣬Ϊ��ͬ��ģ�����ò�ͬ�����ƣ���ô
//                       ��output�����п��Ժ����׵ؿ�������������������ĸ�ģ��
//      GXX_TRACE:  ���б�����кŵĻ������,ÿ����һ�θú꣬�ͻỻ��һ��
//      GXX_TRACE_COUNT: ���޷�Ԥ֪һ��ѭ��Ҫѭ�����ٴλ�һ������Ҫ�������ö��ٴˣ����Ե���
//                       �˺���ͳ�ƴ�����ע����ѭ����Χ�ڲ���������GXX���ʹ�ã����뿴��ͳ�ƴ���
//                       �ĵط��������һ��GXX���ٺ궼���Դ���ͳ�ƴ�������������磺
//                       char * p = "abcdferer.........";
//						 //�޷�Ԥ֪p�ĳ���
//                       char * t = p;
//                       while(*t != '\0')
//                       { ++t; GXX_TRACE_COUNT("�ַ����ĳ���Ϊ:"); }
//                       GXX_TRACE("ͳ�ƽ���"); //����
//                       // ������Ϊ:
//                       // �ַ����ĳ���Ϊ��...
//                       // ͳ�ƽ���
//      GXX_AUTO_TRACE: ��һ���������������ã�����������������������ͽ����㣬��"{"��ʼ��"}"����.
//                      ���磺
//                      int (int a)
//                      {
//							GXX_AUTO_TRACE("fun1��һ��������������");
//                          if( a %2 = 0 )
//                          {   GXX_TRACE("a��һ��ż��");   return a/2; }
//                          else
//                          {   GXX_TRACE("a��һ������");   return a+1; }
//                      }
//                      //���ô˺��� fun1(10);������Ϊ
//                      // =============fun1��һ��������������=============
//                      // {{{
//                      // a��һ��ż��
//                      // }}}
//      GXX_AUTO_TIMER: ͳ����һ���������ڣ���ʹ�ô˺꿪ʼ���������������������ĺ�����
//		GXX_FILE_LINE:  �����ǰ�Ĵ����ļ��ʹ����к�
//      GXX_ASSERT: ������ʽΪfalse���͵����ж϶Ի���

//      __GXX_LOG: ��־��Ϣ����Ŀ��غ�
//      GXX_LOG_CREATE: ���ô˺괴����־�ļ�,ÿ���Զ�����һ���ļ�(�����ָ��������Ϊǰ׺+���� ��Ϊ�ļ���)
//      GXX_LOG_HIT:  �����־����Ϊ��ʾ����־��Ϣ
//      GXX_LOG_WARNING: �����־����Ϊ�������־��Ϣ
//      GXX_LOG_ERROR: �����־����Ϊ�������־��Ϣ
//      GXX_LOG_BAD:  �����־����Ϊ���ص���־��Ϣ
//      
//>���ߣ�Guoxuxing
//>Date��2009/05/25
//>LastTime:2011/6/29
//>����
//	GXX_ASSERT
//__________________________________________________________________________________________________
//>Log��־������ļ�(��_DEBUG��Releaseģʽ�¶���ʹ��)
//__________________________________________________________________________________________________
//__gxx_log__�࣬ʵ����־�����
//>ע�����붨�� __GXX_LOG��������־����
#pragma  once

#include <Windows.h>
#include <stdio.h>

//_DEBUG״̬�²������,����Ԥ�ȶ��� __GXX_TRACE���ܿ����Զ������
using namespace std;

#ifndef _DEBUG
#ifdef __GXX_TRACE
#undef __GXX_TRACE
#endif
#endif

#ifdef __GXX_TRACE

#define __GXX_COUNT            0           //��ʾ��ʼͳ�ƣ�������������仯��ֱ����ʾͳ�Ƽ����Ľ��
class __gxx_trace__;
class __gxx_auto_trace__;
extern __gxx_trace__  __gxx_trace;

/*�����������������������и���*/
#define GXX_TRACE_SETTING_NAME(name) __gxx_trace.SetOutName(name)
#define GXX_TRACE  __gxx_trace                  
#define GXX_TRACE_COUNT(strOut)  __gxx_trace(__GXX_COUNT,strOut);/*�ú������ۼƸ��ټ�������ΪĳЩ�ط��ظ��Ĵ���̫�࣬�������ʾ�������۲���Եúܳ���*/
#define GXX_AUTO_TRACE __gxx_auto_trace__  gxx_auto_trace
#define GXX_AUTO_TIMER __gxx_auto_timer__  gxx_atuo_timer
#define GXX_FILE_LINE() GXX_TRACE("file:%s",__FILE__);GXX_TRACE("line:%d",__LINE__)
#define GXX_ASSERT(expr,_string)	{if(!(expr)){GXX_TRACE(#_string);GXX_FILE_LINE();_ASSERTE((_string,0));}}
class  __gxx_trace__ 
{
public:
    __gxx_trace__();
    ~__gxx_trace__();
	void SetOutName(const wchar_t *lpszName=NULL);
	void SetOutName(const char* lpszName =NULL);
    void operator ()(const char *lpszOut,...);
    void operator ()(const wchar_t *lpszOut,...);
    void operator()(BYTE traceFlag,const char *lpszOut);
    void operator()(BYTE traceFlag,const wchar_t *lpszOut);
private:
	static const long BUF_SIZE = 10240;  //һ���������ֽ���
	static const long BUF_SIZEW = BUF_SIZE/2;
	char _outNameA[256];  //�����ÿ�е�ǰ׺����,���˹�����Խ������Ľ��Խ����
	WCHAR _outNameW[256];

    char _buf[BUF_SIZE];
    char __buf[BUF_SIZE];
    wchar_t _wbuf[BUF_SIZEW];
    wchar_t __wbuf[BUF_SIZEW];
    int __index;
    CRITICAL_SECTION __cs;
    BOOL __bFinished;
    BOOL __bWChar;
    DWORD __dwID;
    UINT __count;
    BOOL _tracePre(DWORD dwID);
    void _traceW(wchar_t * wc);
    void _traceA(char * ca);
};
class __gxx_auto_trace__
{
public:
	__gxx_auto_trace__(char* szOut,...);
	__gxx_auto_trace__(wchar_t* szOut,...);
	~__gxx_auto_trace__();
private:
	static long s_levels;

	const static char _space = '-'; 
	const static wchar_t _wspace = L'-';
	const static int _iRepeat = 2;

	char _spaces[1024];
	wchar_t _wspaces[1024];
	char _buf[1024];
	wchar_t _wBuf[1024];
	bool _bChar;
};
class __gxx_auto_timer__
{
public:
	__gxx_auto_timer__(const char* szOut,...);
	__gxx_auto_timer__(const wchar_t* szOut,...);
	~__gxx_auto_timer__();

	char _spaces[1024];
	wchar_t _wspaces[1024];
	bool _bChar;
	DWORD _dwTick;
};

#else
//���û�п������٣���һ�·����Ѹ�������
#define GXX_TRACE_SETTING_NAME __noop
#define GXX_TRACE __noop
#define GXX_TRACE_COUNT __noop
#define GXX_AUTO_TRACE __noop
#define GXX_AUTO_TIMER __noop
#define GXX_FILE_LINE __noop
#define GXX_ASSERT __noop
#endif  // end #ifdef __GXX_TRACE


///----------------------------------------------������ٽ���

///----------------------------------------------������־���
#define __GXX_HIT            0           //��ʾ��Ϣ
#define __GXX_WARNING    1           //������Ϣ
#define __GXX_ERROR        2           //������Ϣ
#define __GXX_BAD           3           //������Ϣ

#ifdef __GXX_LOG
class __gxx_log__;
extern __gxx_log__  __gxx_log;    //ȫ�ֵ�log����

#define GXX_LOG_CREATE(logFilePath) __gxx_log(logFilePath) 

/*��������������������������־���*/
#define GXX_LOG_HIT				__gxx_log.log_hit
#define GXX_LOG_WARNING			__gxx_log.log_warning
#define GXX_LOG_ERROR			__gxx_log.log_error 
#define GXX_LOG_BAD				__gxx_log.log_bad 
                         
#define GXX_LOG  __gxx_log

typedef __gxx_log__  CGxxLog;

class __gxx_log__
{
public:
    __gxx_log__();
    ~__gxx_log__();

    void operator ()(const wchar_t *tcszLogFilePath);
    void operator ()(int iWay,const char* szOutFormat,...);
    void operator ()(int iWay,const wchar_t * tcszOutFormat,...);

	void log_out(int iWay,const char* szOutFormat,...);
	void log_out(int iWay,const wchar_t* tcszOutFormat,...);
	void log_error(const char* szOutFormat,...);
	void log_error(const wchar_t* tcszOutFormat,...);
	void log_hit(const char* szOutFormat,...);
	void log_hit(const wchar_t* tcszOutFormat,...);
	void log_warning(const char* szOutFormat,...);
	void log_warning(const wchar_t* tcszOutFormat,...);
	void log_bad(const char* szOutFormat,...);
	void log_bad(const wchar_t* tcszOutFormat,...);
private:
	//ÿ�촴��һ���ļ�
	void LogCreate();
    HANDLE m_hFile;
    CRITICAL_SECTION m_cs;
	WCHAR m_chFilePath[512];
	BOOL m_bInitPah;
};
#else
#define GXX_LOG_CREATE(closed) ;
#define GXX_LOG_HIT(closed) ;
#define GXX_LOG_WARNING(closed) ;
#define GXX_LOG_ERROR(closed);
#define GXX_LOG_BAD(closed) ;
//#define GXX_LOG(i,closed,...) void(0)
#endif

///���ܺ�----------------------------------------------
#define MY_ERROR    GXX_TRACE
#define MY_TRACE    GXX_TRACE

#define TRACE_ERROR GXX_TRACE
