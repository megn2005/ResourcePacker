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

// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once

#include "FileView.h"
#include "FolderView.h"
#include "ControlPanel.h"
#include "PropertiesWnd.h"
#include "Resource.h"


///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

class CMainFrame : public CFrameWndEx
{
	DECLARE_DYNCREATE(CMainFrame)

// ʵ��
public:
	// ��������
	virtual ~CMainFrame();

// ��д
public:
	// Ԥ��������
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected: 
	// �������л�����
	CMainFrame();

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);

	// ��ʾ������
	//afx_msg void OnViewCaptionBar();
	// ���±�����
	//afx_msg void OnUpdateViewCaptionBar(CCmdUI* pCmdUI);

	afx_msg void OnOptions();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	DECLARE_MESSAGE_MAP()

private:
	// ����ͣ������
	BOOL createDockingWindows();
	// ����ͣ������ͼ��
	//void setDockingWindowIcons(BOOL bHiColorIcons);
	
	// ����������
	//BOOL createCaptionBar();

public:
	// ��ͣ������
	CFileView			m_wndFileView;
	CFolderView			m_wndFolderView;
	CControlPanel		m_wndOutput;
	CPropertiesWnd		m_wndProperties;

private:  
	// �ؼ���Ƕ���Ա
	CMFCRibbonBar		m_wndRibbonBar;
	CMFCToolBarImages	m_PanelImages;
	CMFCRibbonStatusBar	m_wndStatusBar;
	CMFCCaptionBar		m_wndCaptionBar;
	

};


