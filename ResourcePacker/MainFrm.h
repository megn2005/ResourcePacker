// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// MainFrm.h : CMainFrame 类的接口
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

// 实现
public:
	// 析构函数
	virtual ~CMainFrame();

// 重写
public:
	// 预创建窗口
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected: 
	// 仅从序列化创建
	CMainFrame();

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);

	// 显示标题栏
	//afx_msg void OnViewCaptionBar();
	// 更新标题栏
	//afx_msg void OnUpdateViewCaptionBar(CCmdUI* pCmdUI);

	afx_msg void OnOptions();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	DECLARE_MESSAGE_MAP()

private:
	// 创建停靠窗口
	BOOL createDockingWindows();
	// 设置停靠窗口图标
	//void setDockingWindowIcons(BOOL bHiColorIcons);
	
	// 创建标题栏
	//BOOL createCaptionBar();

public:
	// 各停靠窗口
	CFileView			m_wndFileView;
	CFolderView			m_wndFolderView;
	CControlPanel		m_wndOutput;
	CPropertiesWnd		m_wndProperties;

private:  
	// 控件条嵌入成员
	CMFCRibbonBar		m_wndRibbonBar;
	CMFCToolBarImages	m_PanelImages;
	CMFCRibbonStatusBar	m_wndStatusBar;
	CMFCCaptionBar		m_wndCaptionBar;
	

};


