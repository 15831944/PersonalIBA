#pragma once

#include "ComputerInfo.h"
#include "Socket/LocalServer.h"
// CSeat ����Ŀ��

class CSeat : public CObject
{
public:
	CSeat();
	virtual ~CSeat();

private:
	//λ����Ϣ
	CPoint		m_cpCenter;//��λ��������
	INT_PTR		m_nRadius;//��λ���Բ�뾶
	CString		m_strTerminalID;//�ն˴���
	CString		m_strTerminalIP;//�ն�IP
	INT_PTR		m_nDirection;//���η��򣬷�Ϊ8����
	INT_PTR		m_ComputerStatus;//�����״̬
	CBitmap *	m_StatusBitmap;//����ͼ
	BOOL		m_bIsBmpAttach;//ͼ���Ƿ��Ѿ���
	COLORREF	m_crBlack;
	COLORREF	m_crWhite;

public:
	BOOL GetBmpFlag() const { return m_bIsBmpAttach; }

	void GetRect(CRect& denRect);

	INT_PTR GetComputerStatus() const { return m_ComputerStatus; }

	CPoint GetPosition() const { return m_cpCenter; }
	void  SetPosition(CPoint newPosition) { m_cpCenter = newPosition; }

	INT_PTR GetRadius() const { return m_nRadius; }
	void SetRadius(INT_PTR newRadius) { m_nRadius = newRadius; }

	INT_PTR GetDirection() const { return m_nDirection; }
	void SetDirection(INT_PTR newDirection) { m_nDirection = newDirection; }

	CString GetTerminalID() const { return m_strTerminalID; }
	void SetTerminalID(CString newValue) { m_strTerminalID = newValue; }

	CString GetTerminalIP() const { return m_strTerminalIP; }
	void SetTerminalIP(CString newValue) { m_strTerminalIP = newValue; }
 
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

private:
	// �󶨶�Ӧ����ͼ
	BOOL InitImage(void);
	//ȡ�ö�Ӧ�����״̬��ͼ�α��
	BOOL UpdateComputerStatus();
	
};


