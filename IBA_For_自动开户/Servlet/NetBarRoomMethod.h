#pragma once
#include "servletmethod.h"

// 2011/10/24-8230-gxx: ��ӣ�������ѯ�ӿ�
// class: CConsumeListMethodEx 
//
namespace NS_SERVLET
{

class CNetbarRoomInfo
{
public :
	
	CNetbarRoomInfo();

	// ����ID ������������ID��
	UINT GetPcClass();  
	
	// ��������
	CString GetAreaName();

	// ���������˺�ID
	UINT GetMemberId();

	// �����Ŀ�ͨʱ��
	CString GetBeginTime();

public :

	CRespFieldList  m_RespFields;	//!< ����ֵ�б�

	friend class CNetbarRoomMethod;

};

typedef CArray<CNetbarRoomInfo, CNetbarRoomInfo&> CNetbarRoomInfoArray;

class CNetbarRoomMethod : public CServletMethod
{
public :
	CNetbarRoomMethod(void);
	~CNetbarRoomMethod(void);

protected :

	void AddParams();
	void AddRespFields();

	BOOL DecodeResp(CString strResp);

	CNetbarRoomInfoArray m_RoomInfoList;

public :
	/**
	*����Ա
	*/
	void SetOperator(CString newVal);

public :

	/**
	*��¼����, Ŀǰ���100��
	*/
	UINT GetRecordCount();

	// ����ID, ������ID
	UINT GetRoomID(INT nIndex);
	
	// ��������,����������
	CString GetRoomName(INT nIndex);

	// �û�ID
	UINT GetMemberID(INT nIndex);

	// ��ͨʱ��
	CString GetOpenTime(INT nIndex);
};

}