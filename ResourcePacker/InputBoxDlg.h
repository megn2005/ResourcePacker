#pragma once


//////////////////////////////////////////////////////////////////////////
// CInputBoxDlg �Ի���
//////////////////////////////////////////////////////////////////////////

class CInputBoxDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInputBoxDlg)

public:
	// ��׼���캯��
	CInputBoxDlg(CWnd* pParent = NULL);   
	virtual ~CInputBoxDlg();

// �Ի�������
	enum { IDD = IDD_INPUTBOX };

	// ��ʼ���Ի���
	virtual BOOL OnInitDialog();

protected:
	// DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

public:
	// ����
	CString		m_strCaption;
	// ��������
	CString		m_strInput;
};
