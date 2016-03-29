#pragma once
//Enumerator.h
//�����ߣ������й�Ա��ְ��
//����Ӿ�ء����ͳ����ſڶ�����Ʒ�й�Ա�����Ǹ���ѿͻ�����Ʒ������ӵ�ĳ�����ӣ�
// ÿ�����Ӷ���һ����ţ��й�Ա���ͻ��ı���ƶ�Ӧ�����ţ���ÿ����һλ�ͻ����й�
// Ա��Ҫѡ��һ���յĸ��Ӹ��ͻ������ͻ���������ȡ����Ʒʱ��������Ӿ��ڳ��ˡ���Ȼ
// �й�Ա���ܰ��Ѿ���ʹ�õĸ��ӷ���������Ŀͻ�������ҪѰ��һ���հ׵ĸ��ӡ�
// 
// ����ҲҪʵ���й�Ա�Ĺ��ܣ����Զ��� CEnumerator��.
// ���еĻ���ܵĸ������ǹ̶��ģ�CEnumerator�ĸ������������Ӻͼ���.
// �� ĳ��������Ҫ ��� "��Ʒ"ʱ��CEnumerator�� �������һ�����ӣ����ͱ�ţ�,
// ��������� ȡ��"��Ʒ"ʱ��CEnumerator������������ù��������ӣ��ڳ��ռ�
//
//2009-10-30
//Author:guoxuxing

namespace G2XUtility
{

template<class ShelfType>    //ShelfType����ܵ�����
class CG2XEnumerator
{
public:
	CG2XEnumerator()
	{
		m_pShelf = 0;   //Ĭ��û�����
		m_pUsedState = 0;
		m_pAreaFlag = 0;
		m_nItemCount = 0;  //û�и���
		m_nAreaCount = 0;
		AREA_SIZE = 0;
	}
	//��ʼ����ܸ�������
	CG2XEnumerator(unsigned int nAreasCount, unsigned int nAreaSize)
	{
		m_pShelf = 0;   //Ĭ��û�����
		m_pUsedState = 0;
		m_pAreaFlag = 0;
		m_nItemCount = 0;  //û�и���
		m_nAreaCount = 0;
		AREA_SIZE = 0;
		Buy( nAreasCount, nAreaSize );  //����
	}
	~CG2XEnumerator()
	{
		if( m_pShelf != NULL )
		{
			delete []m_pShelf;
		}
		if( m_pUsedState != NULL )
		{
			delete []m_pUsedState;
		}
		if( m_pAreaFlag != NULL )
		{
			delete []m_pAreaFlag;
		}
	}
	//�ϰ�ͨ����������������
	void Buy(unsigned int nAreasCount, unsigned int nAreaSize)
	{
		if( nAreasCount == 0)
		{
			throw "��ʼ����ܵ�����������Ҫ����0";
			return;
		}
		if( nAreaSize == 0)
		{
			throw "����ܵ������С����Ϊ0";
			return;
		}
		m_nItemCount = nAreasCount * nAreaSize;
		m_nAreaCount = nAreasCount;
		AREA_SIZE = nAreaSize;

		if( m_pShelf )
		{
			delete []m_pShelf;
			m_pShelf = NULL;
		}
		m_pShelf = new ShelfType[ m_nItemCount ];
		if( m_pShelf == NULL )
		{
			m_nItemCount = 0;
			return;
		}
		if( m_pUsedState )
		{
			delete []m_pUsedState;
			m_pUsedState = NULL;
		}
		m_pUsedState = new unsigned char[ m_nItemCount ];
		int i = 0;
		for(i = 0 ; i < m_nItemCount ; i ++)
		{
			m_pUsedState[i] = nobody;
		}

		//���������
		if( m_pAreaFlag )
		{
			delete []m_pAreaFlag;
			m_pAreaFlag  = NULL;
		}
		m_pAreaFlag = new unsigned int[ m_nAreaCount ];
		for(i = 0 ; i < m_nAreaCount ; i ++)
		{
			m_pAreaFlag[i] = 0;
		}
	}
	unsigned int GetItemCount()
	{
		return m_nItemCount;
	}
	//�ڳ�����
	void Vacate(unsigned int nID)
	{
		if( nID >= m_nItemCount )
		{
			return;
		}
		m_pUsedState[nID] = nobody;
	}
	//�ڳ����и���
	void VacateAll()
	{
		for(int i = 0 ; i < m_nItemCount ; i ++)
		{
			m_pUsedState[nID] = nobody;
		}
	}
	//�õ�ʣ���������
	unsigned int GetSurplusSignCount()
	{
		unsigned int nCount = 0;
		for(int i = 0 ; i < m_nAreaCount ;i ++)
		{
			if( m_pAreaFlag[i] != AREA_SIZE )
			{	//������ʣ��ĸ���
				int iStartPos = i*AREA_SIZE;
				int iEndPos = iStartPos + AREA_SIZE;
				for(int j = iStartPos; j < iEndPos ; j ++ )
				{
					if( m_pUsedState[j] == nobody )
						nCount ++;
				}
			}
		}
		return nCount;
	}
	//�õ�һ������ID,���û�п�ʹ�õĸ��ӣ�����-1
	unsigned int GetSign(ShelfType goods)
	{
		//�ҵ�����
		int iArea = -1;
		int i = 0;
		for(i = 0 ; i < m_nAreaCount ; i ++ )
		{
			if( m_pAreaFlag[i] != AREA_SIZE)
			{
				iArea = i;
				break;
			}
		}
		if( iArea == -1)
		{
			return -1;
		}
		int iStartPos = iArea*AREA_SIZE;
		int iEndPos = iStartPos + AREA_SIZE? AREA_SIZE;
		//
		int iID = -1;
		int nCount = 0;
		for(i = iStartPos ; i < iEndPos ; i ++)
		{
			if( m_pUsedState[i] == nobody)
			{
				m_pUsedState[i] = someone;
				m_pShelf[i] = goods;
				m_pAreaFlag[ iArea ] += 1;
				return i;
			}
		}
		return -1;
	}
	//�黹����
	ShelfType* GiveBack(unsigned char nID)
	{
		
		ShelfType* goods = LookUpGoods(nID);
		if (goods)
		{
			int iArea = nID / AREA_SIZE;
			m_pUsedState[nID] = nobody;
			m_pAreaFlag[ iArea ] -= 1;
		}
	}
	//�鿴��Ʒ
	ShelfType* LookUpGoods(unsigned int nID)
	{
		if( m_pUsedState[nID] == nobody )
		{	//����͵��������
			return NULL;   
		}
		return &m_pShelf[nID];
	}
protected:
	enum
	{
		nobody = 0,   //����
		someone       //����
	};
	unsigned int m_nItemCount;     //��ܸ��ӵ����� = AREA_SIZE * m_nAreaCount
	unsigned int AREA_SIZE;
	unsigned int m_nAreaCount;  //�������ĸ�����ÿ�����ĸ�ʽ��Ϊ

	ShelfType* m_pShelf;   //���
	unsigned char* m_pUsedState;    //���ӵ�ʹ��״̬

	
	unsigned int* m_pAreaFlag;  //��������ռ����ʶ
};



}