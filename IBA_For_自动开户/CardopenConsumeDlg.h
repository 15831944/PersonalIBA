#pragma once

#include "IBAFormView.h"

//
//class:CCardopenConsumeView ����δ�����û���Ϣ��ѯ
//
class CCardopenConsumeView : public CIBAFormView
{
	DECLARE_DYNCREATE(CCardopenConsumeView)
	DECLARE_MESSAGE_MAP()

protected:
	CCardopenConsumeView();   // ��׼���캯��
	virtual ~CCardopenConsumeView();

// �Ի�������
	enum { IDD = IDV_CARDOPENCONSUME };

public:
	virtual void OnInitialUpdate();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonQuery();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMDblclkListInfo(NMHDR *pNMHDR, LRESULT *pResult);

private:
	// �����ʾ
	CString m_strNoConsumeResult;
	// ��ʼʱ��
	CBCGPDateTimeCtrl m_StartTime;
	// ����ʱ��
	CBCGPDateTimeCtrl m_EndTime;
	// ��ѯ���
	CStatic m_STATICResult;
	// ��Ϣ�б�
	CBCGPListCtrl m_InfoList;
	// ��ʼ��ʱ��ؼ�
	void InitTimeCtrl(void);
	// ����ʱ��ؼ�������
	void InitDate(void);

	// ��ʼ���б�ؼ�
	void InitList(void);
	// ��ѯͬʱΪ�б�ֵ
	void FillList(void);
	// �û�������ѯ
	void QueryRegister(void);
	// ��ѯ��ǰ�ϻ���¼���ϻ���¼
	BOOL QueryActiveConsume(CString strMemberId);
	// ȡ���˻�״̬�������ִ�
	CString GetCardStateAsString(BOOL bNewCardState = FALSE);
	// ��ѯ��¼
	BOOL QueryHistotyConsume(CString strMemberId, CString strCardOpenTime);

	// ������ȡ����ͨ�û�����ID����ϳ�SQL�ִ�
	CString GetCommonClassIdAsString(void);

};
