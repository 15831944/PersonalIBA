#pragma once

// ˵�����������������
#define GPropertyClass         

// ֻ�����ԵĶ���
#define GPropertyReadOnly(name,type) \
	private:\
	type name;\
	public: const type Get##name()\
	{\
	return name;\
	}

// �ɶ���д���ԵĶ���
#define GProperty(name,type) \
	private:\
	type name;\
	public:  type Get##name() const\
	{\
	return name;\
	}\
	public: void Set##name(const type& newVar )\
	{\
	name = newVar;\
	}\
	public: type& Set##name()\
	{\
	return name;\
	}

// �ɶ���д���ԵĶ��壬����Set����Ҫ�Զ���, �˺�ֻ�Ƕ����Ե���������
#define GPropertyNeedSet(name,type) \
	GPropertyReadOnly(name,type);

#define GPropertyNeedGet(name,type) \
	private:\
	type name;\
	public: void Set##name(const type& newVar )\
	{\
	name = newVar;\
	}\
	public: type& Set##name()\
	{\
	return name;\
	}

// ���Գ�ʼ��
#define GPropertyInit(name,value) \
	name = value;
#define GPropertyInit2(P1,P2,V1,V2) \
	GPropertyInit(P1,V1);\
	GPropertyInit(P2,V2);
#define GPropertyInit3(P1,P2,P3,V1,V2,V3) \
	GPropertyInit(P1,V1);\
	GPropertyInit2(P2,P3,V2,V3);
#define GPropertyInit4(P1,P2,P3,P4,V1,V2,V3,V4) \
	GPropertyInit(P1,V1);\
	GPropertyInit3(P2,P3,P4,V2,V3,V4);

#define GET(name) \
	Get##name()

#define SET(name) \
	Set##name()