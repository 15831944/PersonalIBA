// SeatList.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IBA.h"
#include "SeatList.h"
#include "IBAHelpper.h"
#include "SeatLayoutView.h"

// CSeatList

CSeatList::CSeatList()
{

}

CSeatList::~CSeatList()
{
	DeleteAll();
}


// CSeatList ��Ա����
//������ݣ�������������ָ�룬���ظö�����������±�
INT CSeatList::Add(CSeat * newSeat)
{
	return m_SeatList.Add(newSeat);
}

//����newElement��ָ�룬���뵽nIndexλ��(not put the old)������nCount��
void CSeatList::InsertAt(INT nIndex, CSeat * newSeat,INT nCount)
{
	m_SeatList.InsertAt(nIndex, newSeat, nCount);
}

//ɾ��nCount��nIndexλ�ú��Ԫ��(ʵ�������ָ��)���������
void CSeatList::RemoveAt(INT nIndex, INT nCount)
{
	m_SeatList.RemoveAt(nIndex,nCount);
}

//ɾ������Ԫ�أ����������
void CSeatList::RemoveAll()
{
	m_SeatList.RemoveAll();
}

//���nCount��nIndex���ָ�룬�����ڴ�Ҳ���
void CSeatList::DeleteAt(INT nIndex, INT nCount)
{
	for (int ii = nIndex; ii < (nIndex + nCount); ii++)
	{
		CSeat* tp = m_SeatList.GetAt(ii);
		if(tp)
		{
			delete tp;
			tp = NULL;
		}
	}
	m_SeatList.RemoveAt(nIndex, nCount);
}

//�����������
void CSeatList::DeleteAll()
{
	for (int ii = 0 ; ii < m_SeatList.GetSize(); ii++)
	{
		CSeat* tp = m_SeatList.GetAt(ii);
		if(tp)
		{
			delete tp;
			tp = NULL;
		}
	}
	m_SeatList.RemoveAll();
}

//���鳤��
int CSeatList::GetSize()
{
	return m_SeatList.GetSize();
}

//ȡ���ݣ������������±꣬���ظö����ָ��
CSeat* CSeatList::GetAt(INT nIndex)
{
	return m_SeatList.GetAt(nIndex);
}

//����֪����ĩβ������飬��������ӵ������ָ�룬���ص�һ����ӵ�Ԫ���±�
INT CSeatList::Append(const CSeatList &src)
{
	return m_SeatList.Append(src.m_SeatList);
}

//���������鸲��ԭ�������飬������������ָ�룬�޷���
void CSeatList::Copy(const CSeatList &src)
{
	m_SeatList.Copy(src.m_SeatList);
}
///////////////////////////////////////////////////////
BOOL CSeatList::SaveList()
{
 	CString szPath;
	szPath = theApp.GetWorkPath() + _T("\\IBAConfig\\SeatConfig.ini");

	CFile tFile;
	if (tFile.Open(szPath, CFile::modeCreate|CFile::modeReadWrite))
	{
		tFile.Close();
	}
	
	for ( INT ii = 0; ii < GetSize(); ii++)
	{
		CSeat * tSeat = GetAt(ii);
		if ( tSeat )
		{
			CString strKey;
			strKey.Format(_T("SeatPosX %d"),ii);
			CString strPos;
			strPos.Format(_T(" %d"),tSeat->GetPosition().x);
			WritePrivateProfileString(_T("SeatConfig"), strKey, strPos, szPath);

			strKey.Empty();
			strKey.Format(_T("SeatPosY %d"),ii);
			strPos.Empty();
			strPos.Format(_T(" %d"),tSeat->GetPosition().y);
			WritePrivateProfileString(_T("SeatConfig"), strKey, strPos, szPath);

			strKey.Empty();
			strKey.Format(_T("TerminID %d"),ii);
			strPos.Empty();
			strPos.Format(_T("%s"),tSeat->GetTerminalID());
			WritePrivateProfileString(_T("SeatConfig"), strKey, strPos, szPath);

			strKey.Empty();
			strKey.Format(_T("TerminIP %d"),ii);
			strPos.Empty();
			strPos.Format(_T("%s"),tSeat->GetTerminalIP());
			WritePrivateProfileString(_T("SeatConfig"), strKey, strPos, szPath);

			strKey.Empty();
			strKey.Format(_T("ComputerDirection %d"),ii);
			strPos.Empty();
			strPos.Format(_T(" %d"),tSeat->GetDirection());
			WritePrivateProfileString(_T("SeatConfig"), strKey, strPos, szPath);
		}
		else
		{
			continue;
		}
	}

	return TRUE;
}

BOOL CSeatList::ReadList()
{
	if (GetSize() > 0)
	{
		DeleteAll();
	}

	CString szPath;
	szPath = theApp.GetWorkPath() + _T("\\IBAConfig\\SeatConfig.ini");

	INT ii = 0;
	do 
	{
		CString strKey;
		strKey.Format(_T("SeatPosX %d"),ii);
		CPoint ptTmp(0,0);
		ptTmp.x = GetPrivateProfileInt(_T("SeatConfig"), strKey, 0, szPath);

		strKey.Empty();
		strKey.Format(_T("SeatPosY %d"),ii);
		ptTmp.y = GetPrivateProfileInt(_T("SeatConfig"), strKey, 0, szPath);

		strKey.Empty();
		strKey.Format(_T("TerminID %d"),ii);
		CString strIDTmp;
		GetPrivateProfileString(_T("SeatConfig"), strKey, _T(""), strIDTmp.GetBuffer(MAX_PATH), MAX_PATH, szPath);
		strIDTmp.ReleaseBuffer();
		strIDTmp.Trim();

		strKey.Empty();
		strKey.Format(_T("TerminIP %d"),ii);
		CString strIPTmp;
		GetPrivateProfileString(_T("SeatConfig"), strKey, _T(""), strIPTmp.GetBuffer(MAX_PATH), MAX_PATH, szPath);
		strIPTmp.ReleaseBuffer();
		strIPTmp.Trim();

		strKey.Empty();
		strKey.Format(_T("ComputerDirection %d"),ii);
		UINT nDirection = 0;
		nDirection = GetPrivateProfileInt(_T("SeatConfig"), strKey, 0, szPath);

		if (ptTmp.x == 0 || ptTmp.y == 0 ||
			strIDTmp.CompareNoCase(_T("")) == 0 || 
			strIPTmp.CompareNoCase(_T("")) == 0)
		{
			break;
		}
		else
		{
			CSeat * tSeat = new CSeat;
			tSeat->SetPosition(ptTmp);
			tSeat->SetRadius(DefaultRadius);
			tSeat->SetTerminalID(strIDTmp);
			tSeat->SetTerminalIP(strIPTmp);
			tSeat->SetDirection(nDirection);
			Add(tSeat);

			ii++;
		}

	} while (1);

	return TRUE;
}


void CSeatList::DrawAllRect(CDC * pDC, double XRatio, double YRatio)
{
	if (!pDC)
	{
		return;
	}
	/////////////////////////////////////////
	for (INT ii = 0; ii < GetSize(); ii++)
	{
		CSeat * tSeat = GetAt(ii);
		if (tSeat)
		{
			//tSeat->DrawRect(pDC, XRatio, YRatio);
			tSeat->Draw(pDC, XRatio, YRatio);
		}
	}

}

INT CSeatList::InRect(CPoint MousePoint)
{
	INT ii = 0;
	for (ii = 0; ii < GetSize(); ii++)
	{
		CSeat * tSeat = GetAt(ii);
		if (tSeat)
		{
			CRect rect(0, 0, 0, 0);
			tSeat->GetRect(rect);
			if (rect.PtInRect(MousePoint))
			{
				break;
			}
		}
	}
	if (ii < GetSize())
	{
		return ii;
	}
	else
	{
		return -1;
	}
}


