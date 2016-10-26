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

// ResourcePacker.h : ResourcePacker 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

// 主符号
#include "resource.h"       


///////////////////////////////////////////////////////////////////////////////
// CResourcePackerApp:
// 有关此类的实现，请参阅 ResourcePacker.cpp
///////////////////////////////////////////////////////////////////////////////

class CResourcePackerApp : public CWinAppEx
{
public:
	// 构造函数
	CResourcePackerApp();

// 重写
public:
	// 初始化
	virtual BOOL InitInstance();
	// 退出
	virtual int ExitInstance();

	//
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

protected:
	afx_msg void OnAppAbout();

	DECLARE_MESSAGE_MAP()

public:
	// 实现
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;
};

extern CResourcePackerApp theApp;
