#pragma once

#include "IBADialog.h"
#include "resource.h"


class CSetPageOfCommon : public CIBAPage
{
	DECLARE_DYNAMIC(CSetPageOfCommon)
	DECLARE_MESSAGE_MAP()

public:
	CSetPageOfCommon();   // ��׼���캯��
	virtual ~CSetPageOfCommon();

// �Ի�������
	enum { IDD = IDD_DIALOG_IBASET };

private:

	BOOL m_bShowAllComputer;
	BOOL m_bOpenAfterSave;
	BOOL m_bEnmergencyVisible;
	BOOL m_bShowLastUserNetId;

	// 2011-4-13-gxx-��ӶԿͻ�����Ϣ�����Ŀ���
	BOOL m_bMsgLeftBottom;
	BOOL m_bMsgAutoHide;

	BOOL m_bPauseRevMsg;
	
	INT m_nFontType;

	INT m_nScan;

	INT m_nOperationAfterSwipingCard;
	INT m_nOperationAferEnter;
	INT m_nOperationAfterDClick;

	// 2011/12/12-8235-gxx:
	BOOL m_bForbidCheckIn; // �����Ҽ��ϻ�

	// 2012/06/28-8245-gxx:  �Ƿ��ֹ����Ա����
	BOOL m_bForbidCaptureScreen;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	
};
