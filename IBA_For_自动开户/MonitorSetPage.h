#pragma once

#include "resource.h"
//
//class:CSetPageOfMonitor ����̨����->�����ɫ���öԻ���
//
class CSetPageOfMonitor : public CIBAPage
{
	DECLARE_DYNAMIC(CSetPageOfMonitor)
	DECLARE_MESSAGE_MAP()

public:
	CSetPageOfMonitor();
	virtual ~CSetPageOfMonitor();

// �Ի�������
	enum { IDD = IDD_SET_MONITORCOLOR };

protected:

	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnOK();

private:
	
	CBCGPColorButton m_CPOnLine;
	CBCGPColorButton m_CPOffLine;
	CBCGPColorButton m_CPThief;
	CBCGPColorButton m_CPLittleMoney;
	CBCGPColorButton m_CPLockScreen;
	CBCGPColorButton m_CPUnLock;
	CBCGPColorButton m_CPBlack;
	CBCGPColorButton m_CPVIP;
	CBCGPColorButton m_CPSuspend;
	CBCGPColorButton m_CPBirthDay;

	// ��ʱ�����ɫ
	CBCGPColorButton m_CPBundTimeColor;
};
