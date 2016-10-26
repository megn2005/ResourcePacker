// InputBoxDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ResourcePacker.h"
#include "InputBoxDlg.h"
#include "afxdialogex.h"


//////////////////////////////////////////////////////////////////////////
// CInputBoxDlg 对话框
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CInputBoxDlg, CDialogEx)

// 构造函数
CInputBoxDlg::CInputBoxDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInputBoxDlg::IDD, pParent)
{
}

// 析构函数
CInputBoxDlg::~CInputBoxDlg()
{
}


// 初始化对话框
BOOL CInputBoxDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置标题
	SetWindowText(m_strCaption);

	// 初始化输入框
	UpdateData(FALSE);

	return TRUE;
}


// DDX/DDV 支持
void CInputBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUTBOX, m_strInput);
}


//////////////////////////////////////////////////////////////////////////
// CInputBoxDlg 消息处理程序
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CInputBoxDlg, CDialogEx)
END_MESSAGE_MAP()





