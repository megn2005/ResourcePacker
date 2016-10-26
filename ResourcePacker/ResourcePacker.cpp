// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// ResourcePacker.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "ResourcePacker.h"
#include "MainFrm.h"

#include "ResourcePackerDoc.h"
#include "ResourcePackerView.h"

#include "DBOper.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


///////////////////////////////////////////////////////////////////////////////
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
///////////////////////////////////////////////////////////////////////////////

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	// DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange* pDX);    

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};


///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

// ���캯��
CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

// ��������
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


///////////////////////////////////////////////////////////////////////////////
// CAboutDlg ��Ϣ�������
///////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()






///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////







// Ψһ��һ�� CResourcePackerApp ����
CResourcePackerApp theApp;

///////////////////////////////////////////////////////////////////////////////
// CResourcePackerApp
///////////////////////////////////////////////////////////////////////////////

// CResourcePackerApp ����
CResourcePackerApp::CResourcePackerApp()
{
	m_bHiColorIcons = TRUE;

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("ResourcePacker.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	
	// �����洰��״̬
	m_bSaveState = FALSE;
}

// CResourcePackerApp ��ʼ��
BOOL CResourcePackerApp::InitInstance()
{
	CWinAppEx::InitInstance();

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	// ��ʼ�����ݿ�
	if (!CDBOper::Initialize())
	{
		AfxMessageBox(IDS_SQLITE_INIT_FAILED);
		return FALSE;
	}

	//
	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("ResourcePacker"));
	LoadStdProfileSettings(0);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)

	InitContextMenuManager();
	InitShellManager();
	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);


	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CResourcePackerDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CResourcePackerView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;


	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����

	return TRUE;
}

// �˳�
int CResourcePackerApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ

	// �ͷ����ݿ�
	CDBOper::Finalize();

	//
	AfxOleTerm(FALSE);

	// ������״̬
	//CleanState();

	return CWinAppEx::ExitInstance();
}


///////////////////////////////////////////////////////////////////////////////
// CResourcePackerApp ��Ϣ�������
///////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CResourcePackerApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CResourcePackerApp::OnAppAbout)
END_MESSAGE_MAP()


// �������жԻ����Ӧ�ó�������
void CResourcePackerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CResourcePackerApp �Զ������/���淽��
void CResourcePackerApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;

	//
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	
	// �ļ������˵�
	bNameValid = strName.LoadString(IDS_FOLDER_TREE_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_FOLDER_TREE);

	// �ļ��б�˵�
	bNameValid = strName.LoadString(IDS_FILE_LIST_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_FILE_LIST);
}

//
void CResourcePackerApp::LoadCustomState()
{
}

//
void CResourcePackerApp::SaveCustomState()
{
}



