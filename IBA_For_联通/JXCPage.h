#pragma once


// CJXCPage �Ի���

class CSetPageOfJXC : public CIBAPage
{
	DECLARE_DYNAMIC(CSetPageOfJXC)
	DECLARE_MESSAGE_MAP()

public:
	CSetPageOfJXC();
	virtual ~CSetPageOfJXC();

// �Ի�������
	enum { IDD = IDD_SET_JXC };

private:

	CBCGPComboBox m_cboStore;
	BOOL LoadStoreInfo();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnOK();
	virtual BOOL OnInitDialog();

public:
	BOOL m_bPayCash; // �Ƿ��ֽ�֧��
};
