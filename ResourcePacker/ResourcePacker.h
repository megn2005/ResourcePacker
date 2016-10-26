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

// ResourcePacker.h : ResourcePacker Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

// ������
#include "resource.h"       


///////////////////////////////////////////////////////////////////////////////
// CResourcePackerApp:
// �йش����ʵ�֣������ ResourcePacker.cpp
///////////////////////////////////////////////////////////////////////////////

class CResourcePackerApp : public CWinAppEx
{
public:
	// ���캯��
	CResourcePackerApp();

// ��д
public:
	// ��ʼ��
	virtual BOOL InitInstance();
	// �˳�
	virtual int ExitInstance();

	//
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

protected:
	afx_msg void OnAppAbout();

	DECLARE_MESSAGE_MAP()

public:
	// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;
};

extern CResourcePackerApp theApp;
