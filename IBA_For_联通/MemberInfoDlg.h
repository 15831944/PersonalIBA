#pragma once
/**************************************************************************
�ṩ���Һ�ѡ���û����м�Ի���
**************************************************************************/

#include "IBADialog.h"

//
//class:CDlgMemberInfo ��ѯ��Ա�ĶԻ����ڿ���ͨ���������˺ſ��ܶ�Ӧ����û���CDlgMemberInfoȷ����ȡΨһ��һ����Ա
//    ������ΪCDlgMemberInfo�ǹ��ɴ���
//
class GPropertyClass CDlgMemberInfo : public CIBADialog
{
	DECLARE_DYNAMIC(CDlgMemberInfo)
	DECLARE_MESSAGE_MAP()

public:
	CDlgMemberInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgMemberInfo();

// �Ի�������
	enum { IDD = IDD_DIALOG_MEMBERINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnNMDblclkListMemberinfo(NMHDR *pNMHDR, LRESULT *pResult);

public:
	GProperty( UserName,   CString ); // �û�����
	GProperty( PersonalID, CString ); // ֤������
	GProperty( Sex,        CString ); // �Ա�
	GProperty( NetId,      CString ); // �û��˺�
	GProperty( UserClass,  CString ); // �û�����
	GProperty( IDType,     CString ); // ֤������

	GPropertyReadOnly( MemberId, UINT ); // �û�ID
	
public:
	
	// ��ѯ��ʽ
	// 0: Ϊ��������֤����������Ĳ�ѯ
	// 1: �ӱ���member���ѯ
	// 2: ���Ĳ�ѯ��ʽ����Ч�û�
	void SetQueryStyle(UINT NewValue) { m_nQueryStyle = NewValue; }

private:
	
	UINT m_nQueryStyle; // ��ѯ��ʽ

	// �ؼ�����
	CBCGPListCtrl m_MemberInfoList;

private:
	//�����Ĳ�ѯ,��ѯ��ʽ0Ϊ��������֤����������Ĳ�ѯ��1Ϊ���˺Ŵӱ���member���ѯ
	void FindInfoFromServer(UINT nConsumeType = 0);

	// �����б���������
	void SetMemberInfoToList(UINT nQueryStyle);

	// ��ʼ���б�
	void InitList(void);

	// ȡ��ѡ�е��û���Ϣ
	int GetMemberInfoFromList(void);

	// �ӱ��ز�������
	void FindInfoFromLocal(BOOL bValidUser);
};
