#pragma once


//////////////////////////////////////////////////////////////////////////
// CInputBoxDlg 对话框
//////////////////////////////////////////////////////////////////////////

class CInputBoxDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInputBoxDlg)

public:
	// 标准构造函数
	CInputBoxDlg(CWnd* pParent = NULL);   
	virtual ~CInputBoxDlg();

// 对话框数据
	enum { IDD = IDD_INPUTBOX };

	// 初始化对话框
	virtual BOOL OnInitDialog();

protected:
	// DDX/DDV 支持
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

public:
	// 标题
	CString		m_strCaption;
	// 输入内容
	CString		m_strInput;
};
