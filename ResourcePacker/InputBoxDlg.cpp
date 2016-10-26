// InputBoxDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ResourcePacker.h"
#include "InputBoxDlg.h"
#include "afxdialogex.h"


//////////////////////////////////////////////////////////////////////////
// CInputBoxDlg �Ի���
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CInputBoxDlg, CDialogEx)

// ���캯��
CInputBoxDlg::CInputBoxDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInputBoxDlg::IDD, pParent)
{
}

// ��������
CInputBoxDlg::~CInputBoxDlg()
{
}


// ��ʼ���Ի���
BOOL CInputBoxDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ñ���
	SetWindowText(m_strCaption);

	// ��ʼ�������
	UpdateData(FALSE);

	return TRUE;
}


// DDX/DDV ֧��
void CInputBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUTBOX, m_strInput);
}


//////////////////////////////////////////////////////////////////////////
// CInputBoxDlg ��Ϣ�������
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CInputBoxDlg, CDialogEx)
END_MESSAGE_MAP()





