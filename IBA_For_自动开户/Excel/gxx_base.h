#pragma  once

namespace NS_GXX
{
    
typedef DWORD  DATA_PTR;     //ָ���������ʽ
//com initialize and uninitialize
class CComBase 
{
protected:
	CComBase(){
		::CoInitialize(NULL);
	}
public:
	virtual ~CComBase(){
		::CoUninitialize();
	}
};

/*
 * My Clock
*/
class CMyClock
{
public:
	CMyClock();
	~CMyClock(){}
	//
	//��ʼ��ʱ
	void Start();
	//
	//����ʱ�ӣ�������ȥ�ĺ�����
	DWORD End();    
private:
	DWORD m_dwTickCount;
};

#ifdef USING_MYCLOCK
#define START_MYCLOCK(_idSymbol)\
	NS_GXX::CMyClock classMyClock##_idSymbol;\
	classMyClock##_idSymbol.Start();
#define END_MYCLOCK(_idSymbol)\
	(classMyClock##_idSymbol.End())
#else
#define START_MYCLOCK(_idSymbol)
#define END_MYCLOCK(_idSymbol)
#endif
//----------------------------------------------------------

}