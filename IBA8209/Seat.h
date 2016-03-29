#pragma once

#include "ComputerInfo.h"
#include "Socket/LocalServer.h"
// CSeat ����Ŀ��
// CSeat����������Ϊ��
// CSeat2�����Ͻ�����Ϊ��
class CSeat : public CObject
{
public:
	CSeat(LPCTSTR szSeatPath = NULL);
	virtual ~CSeat();
private:
	//λ����Ϣ
	CPoint		m_cpCenter;//��λ��������
	INT_PTR		m_nRadius;//��λ���Բ�뾶
	CString		m_strTerminalID;//�ն˴���
	CString		m_strTerminalIP;//�ն�IP
	INT_PTR		m_nDirection;//���η��򣬷�Ϊ8����
	INT_PTR		m_ComputerStatus;//�����״̬
	static CBitmap		*m_StatusBitmap;//����ͼ				// 2014-8-19 - qsc ��Ϊstatic
	static BOOL			m_bIsBmpAttach;//ͼ���Ƿ��Ѿ���		// 2014-8-19 - qsc ��Ϊstatic
	COLORREF	m_crBlack;
	COLORREF	m_crWhite;
	BOOL		m_bStatusChange;
	INT_PTR		m_PreComputerStatus;//�����״̬
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
	void DrawRect(CDC * pDC = NULL, double XRatio = 1.0, double YRatio = 1.0, BOOL bDrawTermID = TRUE);
	void DrawRect(CDC * pDC, CPoint Centerpoint, double XRatio = 1.0, double YRatio = 1.0, BOOL bDrawTermID = TRUE);
	//����ͼ�񣬴���ͼ��ʱ
	void DrawBmp(CDC * pDC, CPoint ptCenter, double XRatio = 1.0, double YRatio = 1.0);
	void DrawBmp(CDC * pDC, double XRatio = 1.0, double YRatio = 1.0);

	BOOL InvalidateIfStatusChange(HWND hWnd);
	BOOL IsStatusChange();
private:
	// �󶨶�Ӧ����ͼ
	BOOL InitImage(LPCTSTR szSeatPath = NULL);
	//ȡ�ö�Ӧ�����״̬��ͼ�α��
	BOOL UpdateComputerStatus();
	void DrawTermID(CDC* pDC, int x, int y);
	BOOL UpdateComputerStatus2();
	
};


