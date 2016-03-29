#include "stdafx.h"
#include "LocalModeMethod.h"

using namespace NS_SERVLET;

CLocalModeMethod::CLocalModeMethod(void)
: CServletMethod(_T("LocalMode")) //���ķ�������
{
	AddParams(); //��ʼ������
	AddRespFields(); //��ʼ�������ֶ�
}

CLocalModeMethod::~CLocalModeMethod(void)
{
}

void CLocalModeMethod::AddParams()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ�ϲ���
	CServletMethod::AddParams();
	m_Params.AddParam(_T("operationType"), EPT_NUMBER, 1);
	m_Params.AddParam(_T("lastOperationTime"), EPT_STRING, 14);
	m_Params.AddParam(_T("operator"), EPT_STRING, 10);
}

void CLocalModeMethod::SetOperationType(UINT newVal)
{
	m_Params.SetValue(_T("operationType"), newVal);
}

void CLocalModeMethod::SetLastOperationTime(CString newVal)
{
	m_Params.SetValue(_T("lastOperationTime"), newVal);
}

void CLocalModeMethod::SetOperator(CString newVal)
{
	m_Params.SetValue(_T("operator"), newVal);
}

void CLocalModeMethod::AddRespFields()
{
	//�벻Ҫ�޸ı��������룬�����ֶ�˳����ȷ������ȷ������
	CServletMethod::AddRespFields();

}

