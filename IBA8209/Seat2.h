#pragma once

#include "ComputerInfo.h"
#include "Socket/LocalServer.h"
#include "IBACommDefine.h"
// CSeat2 ����Ŀ��
// CSeat����������Ϊ��
// CSeat2�����Ͻ�����Ϊ��
using namespace NS_IBACommDef;
class CSeat2 : public CObject
{
public:
	CSeat2(Seat2Type type);
	virtual ~CSeat2();

private:
	//λ����Ϣ
	CPoint		m_cpCenter;//��λ��������
	CPoint		m_cpLT;//���Ͻ�
	INT_PTR		m_nRadius;//��λ���Բ�뾶
	CBitmap *	m_StatusBitmap;//����ͼ
	BOOL		m_bIsBmpAttach;//ͼ���Ƿ��Ѿ���
	COLORREF	m_crBlack;
	COLORREF	m_crWhite;
	//CString		m_strSeatName;
	Seat2Type	m_Seat2Type;
	int			m_nBmpHeight;
	int			m_nBmpWidth;
public:
	BOOL GetBmpFlag() const { return m_bIsBmpAttach; }

	void GetRect(CRect& denRect);

	CPoint GetPosition() const;
	CPoint GetCenterPos() const;
	void  SetPosition(CPoint newPosition);
	void SetCenterPos(CPoint newPosition);
	
	INT_PTR GetRadius() const { return m_nRadius; }
	void SetRadius(INT_PTR newRadius) { m_nRadius = newRadius; }

	CComputerInfo& GetComputerInfo(); //Ϊ��ֹ��һ�£������ⲿ���÷���

	/************************************************************************/
	/* ����ͼ�Σ����û������ͼ�Σ����ƾ���                                                                     */
	/************************************************************************/
	void Draw(CDC * pDC, double XRatio = 1.0, double YRatio = 1.0);
	void Draw(CDC * pDC, CPoint ptCenter, double XRatio = 1.0, double YRatio = 1.0);

	//���ƾ��Σ�Ĭ��
	void DrawRect(CDC * pDC = NULL, double XRatio = 1.0, double YRatio = 1.0);
	void DrawRect(CDC * pDC, CPoint Centerpoint, double XRatio = 1.0, double YRatio = 1.0);
	//����ͼ�񣬴���ͼ��ʱ
	void DrawBmp(CDC * pDC, CPoint ptCenter, double XRatio = 1.0, double YRatio = 1.0);
	void DrawBmp(CDC * pDC, double XRatio = 1.0, double YRatio = 1.0);
	
	Seat2Type GetSeat2Type() { return m_Seat2Type; }
private:
	// �󶨶�Ӧ����ͼ
	BOOL InitImage();
	void DrawSeatName(CDC* pDC, int x, int y);
	//ȡ�ö�Ӧ�����״̬��ͼ�α��
};


