/**************************************************************************
file: G2XDestructorRunner.h
class: CG2XDestructorRunner 

     ������������������CG2XDestructorRunner�����������ڽ���ʱ���������������������ڳ�ʼ��
	    ʱ��ָ���Ļص�����
	
	ĳ������Խ�ĳ����Ա����ί�и�CG2XDestructorRunnerȥִ�С�
remark:
   
   CG2XDestructorRunner�ṩ4�ַ�ʽ��������.
   1. �������ڽ���ʱ������ĳ������ĳ�Ա����(������һ������)
   2. �������ڽ���ʱ������ĳ������ĳ�Ա����(�����޲���)
   3. �������ڽ���ʱ������ĳ����ͨ����(������һ������)
   4. �������ڽ���ʱ������ĳ����ͨ����(�����޲���)

   ����һ��ʹ�����:
   ĳ�������ķ���·����N����������ÿ��returnǰ��Ҫ������һ������,��ô��ʱ�Ϳ����õ�
   CG2XDestructorRunner, CG2XDestructorRunner���뱻����Ϊ�����ľֲ�����

date: 2011-5-18
writer: guoxuxing

mail: gemeryguo@126.com
***************************************************************************/

#pragma once

template<class TClass,typename TResult,typename TParam>
class CG2XDestructorRunner
{
	// ����ʱ������ĳ������ĺ���(�����߱�һ������)
public:
	typedef TResult (TClass::*Function)(TParam);

	CG2XDestructorRunner(Function callback,TParam param,TClass *pClassObj):
	m_callback(callback),m_pClassObj(pClassObj),m_param(param)
	{

	}
	~CG2XDestructorRunner()
	{
		(m_pClassObj->*m_callback)(m_param);
	}

	Function m_callback;
	TClass *m_pClassObj;
	TParam m_param;
};
template<class TClass,typename TResult>
class CG2XDestructorRunner<TClass,TResult,void>
{
	// ����ʱ������ĳ������ĺ���(�����޲���)
public:
	typedef TResult (TClass::*Function)(void);

	CG2XDestructorRunner(Function callback,TClass *pClassObj):
	m_callback(callback),m_pClassObj(pClassObj)
	{

	}
	~CG2XDestructorRunner()
	{
		(m_pClassObj->*m_callback)();
	}

	Function m_callback;
	TClass *m_pClassObj;
};

template<typename TResult,typename TParam>
class CG2XDestructorRunner<void,TResult,TParam>
{
	// ����ʱ������һ����ͨ����
public:
	typedef TResult(*Function)(TParam);
	CG2XDestructorRunner(Function callback,TParam param):
	m_callback(callback),m_param(param)
	{

	}
	~CG2XDestructorRunner()
	{
		(m_callback)(m_param);
	}

	Function m_callback;
	TParam m_param;
};

template<typename TResult>
class CG2XDestructorRunner<void,TResult,void>
{
	// ����ʱ������һ����ͨ����
public:
	typedef TResult(*Function)(void);
	CG2XDestructorRunner(Function callback):
	m_callback(callback)
	{

	}
	~CG2XDestructorRunner()
	{
		(m_callback)();
	}

	Function m_callback;
};