#pragma once
#include <vector>

namespace G2XUtility{

#define G2XStatusFALSE 0
#define G2XStatusTRUE  1

template<typename ValueType>
class CG2XStatusArray
{
public:
	struct _VItem
	{
		unsigned char status;       // ״ֵ̬
		ValueType vtValue; // ����ֵ
	};

public:
	CG2XStatusArray(){
		m_bSingle = TRUE;
	}

	CG2XStatusArray(unsigned int nItemCount, ValueType defaultValue, bool bSingleTRUE=true){
		_VItem vm;
		vm.status = G2XStatusFALSE;
		vm.vtValue = defaultValue;
		m_items.resize(nItemCount, vm);
		SetStatusStyle(TRUE);
	}

	// ����״̬���
	// bSingleTRUE: [TRUE] ״̬���������ֻ��һ���״ֵ̬ΪG2XStatusTRUE
	// bSingleTRUE: [FALSE] ״̬�����п����ж�����״ֵ̬ΪG2XStatusTRUE
	// ע: ����ǵ�һ�ַ����ô����SetStatus(nAt2,TRUE)ʱ�����nAt1����״̬ΪTRUE,
	//       ��ônAt1����״̬�Զ������FALSE
	void SetStatusStyle(bool bSingleTRUE = true)
	{
		m_bSingle = bSingleTRUE;
	}

	void AddValue(const ValueType& val, unsigned char nStatus)
	{
		_VItem vi;
		vi.status = G2XStatusFALSE;
		vi.vtValue = val;
		m_items.push_back(vi);
		SetStatus(GetCount()-1, nStatus);
	}

	void InsertValue(int nIndex, const ValueType& val, unsigned char nStatus)
	{
		_VItem vi;
		vi.status = G2XStatusFALSE;
		vi.vtValue = val;
		m_items.insert(m_items.begin() + nIndex, vi);
		SetStatus(nIndex, nStatus);
	}
	void SetValue(int nIndex, const ValueType& val)
	{
		m_items[nIndex].vtValue = val;
	}
	
	int GetCount(){
		return (int)m_items.size();
	}

	// ����ָ��λ�ô���״̬, ����������״̬ǰ��״̬
	unsigned char SetStatus(int nAt, unsigned char status)
	{
		if (nAt >= GetCount())
		{
			throw _T("array overflow!");
			return G2XStatusFALSE;
		}
		if (m_bSingle)
		{
			int nOld = GetStatusTRUE();
			if (nOld == -1)
			{
				BYTE oldS = m_items.at(nAt).status;
				m_items.at(nAt).status = status;
				return oldS;
			}
			else
			{
				m_items.at(nOld).status = G2XStatusFALSE;
				BYTE oldS = m_items.at(nAt).status;
				m_items.at(nAt).status = status;
				return oldS;
			}
		}
		else
		{
			BYTE oldS = m_items.at(nAt).status;
			m_items.at(nAt).status = status;
			return oldS;
		}
	}

	// �õ�ָ��λ�ô���״̬ 
	unsigned char GetStatus(int nAt)
	{
		return m_items.at(nAt).status;
	}

	// �õ�״̬ΪG2XStatusTRUE�����λ��,û�з���-1
	int GetStatusTRUE()
	{
		int nAt = -1;
		for (int i=0; i < (int)m_items.size(); i++)
		{
			if (m_items[i].status == G2XStatusTRUE)
			{
				nAt = i;
				break;
			}
		}
		return nAt;
	}

	// �õ�����״̬ΪG2XStatusTRUE��λ���б�, û�з���0�����򷵻ظ���
	int GetStatusTRUE1(std::vector<int>* res)
	{
		if (res)
			res->clear();

		int nCount = 0;
		for(int i = 0; i < (int)m_items.size(); i++)
		{
			if (GetStatus(i) == G2XStatusTRUE)
			{
				nCount++;
				if (res)
				{
					res->push_back(i);
				}
			}
		}
		return nCount;
	}

	// �õ�����״̬ΪG2XStatusTRUE�Ķ����б�, û�з���0�����򷵻ظ���
	int GetStatusTRUE2(std::vector<ValueType>* res)
	{
		if (res)
			res->clear();

		int nCount = 0;
		for(int i = 0; i < (int)m_items.size(); i++)
		{
			if (GetStatus(i) == G2XStatusTRUE)
			{
				nCount++;
				if (res)
				{
					res->push_back(GetValue(i));
				}
			}
		}
		return nCount;
	}

	ValueType GetValue(int nIndex)
	{
		return m_items.at(nIndex).vtValue;
	}
	unsigned char operator[](int nIndex)
	{
		return GetStatus(nIndex);
	}
	void Swap(int index1, int index2)
	{
		_VItem viTmp = m_items[index1];
		m_items[index1] = m_items[index2];
		m_items[index2] = viTmp;
	}

	// ��ѡ��SetStatusStyle���õ��ǵ�ѡ���÷���������Ч
	void Inverse()
	{
		if (m_bSingle)
			return;

		for (int i = 0; i < GetCount(); i++)
		{
			if (GetStatus(i) == G2XStatusTRUE)
				SetStatus(i, G2XStatusFALSE);
			else
				SetStatus(i, G2XStatusTRUE);
		}
	}
	// ȫѡ��SetStatusStyle���õ��ǵ�ѡ���÷���������Ч
	void SelectAll()
	{
		if (m_bSingle)
			return;

		for (int i = 0; i < GetCount(); i++)
		{
			SetStatus(i, G2XStatusTRUE);
		}
	}

	// ����
	void Reset(int index)
	{
		SetStatus(index, G2XStatusFALSE);
	}

	// ����
	void Set(int index)
	{
		SetStatus(index, G2XStatusTRUE);
	}

	// ��������
	void ResetAll()
	{
		for (int i = 0; i < GetCount(); i++)
		{
			m_items[i].status = G2XStatusFALSE;
		}
	}

	// �Ƴ�ĳ��
	void Remove(int index)
	{
		m_items.erase(m_items.begin() + index);
	}

	// �Ƴ�����ֵΪvalue����
	void Remove2(const ValueType& val)
	{
		int nIndex = 0;
		std::vector<_VItem>::iterator it = m_items.begin();
		while(it != m_items.end())
		{
			if (it->vtValue == val)
			{
				m_items.erase(it);
			}
			else
			{
				nIndex ++;	
			}
			it = m_items.begin() + nIndex;
		}
	}

	void Clear()
	{
		m_items.clear();
	}

protected:
	std::vector<_VItem> m_items;
	bool m_bSingle;
};

}// end namespace