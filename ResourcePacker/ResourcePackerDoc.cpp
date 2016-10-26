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

// ResourcePackerDoc.cpp : CResourcePackerDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ResourcePacker.h"
#endif

#include "ResourcePackerDoc.h"
#include "ResourcePackerView.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


///////////////////////////////////////////////////////////////////////////////
// CResourcePackerDoc
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CResourcePackerDoc, CDocument)

// CResourcePackerDoc 构造函数
CResourcePackerDoc::CResourcePackerDoc()
{
	// TODO: 在此添加一次性构造代码

}

// 析构函数
CResourcePackerDoc::~CResourcePackerDoc()
{
}

// 新建文档
BOOL CResourcePackerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	POSITION pos = GetFirstViewPosition();
	if (pos)
	{
		CResourcePackerView* pView = DYNAMIC_DOWNCAST(CResourcePackerView, GetNextView(pos));
		
	}

	return TRUE;
}


// CResourcePackerDoc 序列化
void CResourcePackerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CResourcePackerDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CResourcePackerDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CResourcePackerDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS


///////////////////////////////////////////////////////////////////////////////
// CResourcePackerDoc 诊断
///////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void CResourcePackerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CResourcePackerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


///////////////////////////////////////////////////////////////////////////////
// CResourcePackerDoc 命令
///////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CResourcePackerDoc, CDocument)
END_MESSAGE_MAP()






//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////












