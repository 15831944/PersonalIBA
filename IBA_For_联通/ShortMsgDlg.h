#pragma once



// CShortMsgDlg �Ի���

class CShortMsgDlg : public CStringLoaderDlg<CBCGPDialog>
{
	DECLARE_DYNAMIC(CShortMsgDlg)

public:
	CShortMsgDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShortMsgDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SHORTMSG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedSendshormsg();
	afx_msg void OnQuickMsg(UINT id);
	afx_msg void OnEditQuickMsg();

	void ShowPopupMenu();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);


private:
	CFont m_TextFont;//��������
	CString m_ShortMsgContent;//��������
	CBCGPComboBox m_cboPCClass;//
	CUIntArray m_nReceivers;//������
	CStringArray m_QucikMsgs;//���Ŷ���
	
	UINT m_nSendType;//��������

public:

	void AddReceiver(UINT newVal) { m_nReceivers.Add(newVal); }
	void SetReceivers(CUIntArray& newVal) { m_nReceivers.Copy(newVal); }

	// nType = 1: ���͸���ѡ�ն�,
	// nType = 2: ���͸���������̨
	void SetSendType(UINT nType);
};
