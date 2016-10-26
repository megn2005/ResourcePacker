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

// ResourcePackerView.cpp : CResourcePackerView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ResourcePacker.h"
#endif

#include "ResourcePackerDoc.h"
#include "ResourcePackerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 渲染时钟ID
#define	RENDER_TIMER_ID		1


///////////////////////////////////////////////////////////////////////////////
// CResourcePackerView
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CResourcePackerView, CView)

// CResourcePackerView 构造函数
CResourcePackerView::CResourcePackerView()
	: m_spD3DDev(nullptr), m_spSprite(nullptr), m_spTexture(nullptr), m_spMesh(nullptr)
{
	// TODO: 在此处添加构造代码
	
}

// 析构函数
CResourcePackerView::~CResourcePackerView()
{
}


// 预创建窗口
BOOL CResourcePackerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CResourcePackerView 绘制
void CResourcePackerView::OnDraw(CDC* /*pDC*/)
{
	CResourcePackerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码

	if (!m_spD3DDev)
	{
		// 初始化渲染视口
		if (initRenderView())
		{
			// 渲染时钟
			SetTimer(RENDER_TIMER_ID, 33, NULL);
		}
	}

}


///////////////////////////////////////////////////////////////////////////////
// CResourcePackerView 诊断
///////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void CResourcePackerView::AssertValid() const
{
	CView::AssertValid();
}

void CResourcePackerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CResourcePackerDoc* CResourcePackerView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CResourcePackerDoc)));
	return (CResourcePackerDoc*)m_pDocument;
}
#endif //_DEBUG


///////////////////////////////////////////////////////////////////////////////
// CResourcePackerView 消息处理程序
///////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CResourcePackerView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CONTEXTMENU()

	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FILE_LIST, &CResourcePackerView::OnLvnItemChangedFileList)

END_MESSAGE_MAP()


// 窗口创建
int CResourcePackerView::OnCreate(LPCREATESTRUCT lpcs)
{
	if (-1 == CView::OnCreate(lpcs))
		return -1;

	//

	return 0;
}

// 窗口改变大小
void CResourcePackerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_spD3DDev)
	{
		// 修改后台缓存大小
		MDirect3D::s_PP.BackBufferWidth = cx;
		MDirect3D::s_PP.BackBufferHeight = cy;

		// 修改图片纹理大小
		m_spTexture.reset();
		m_spTexture = m_spD3DDev->CreateTexture(cx, cy, D3DFMT_A8R8G8B8);

		m_spD3DDev->Reset();
	}
}

// 渲染时钟
void CResourcePackerView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (m_spD3DDev)
	{
		m_spD3DDev->BeginRender();

		// 绘制网格模型
		if (m_spMesh)
			m_spMesh->DrawSubset(0);

		// 绘制图片纹理
		/*if (m_spSprite)
		{
		m_spSprite->BeginDraw();
		if (m_spTexture)
		m_spSprite->Draw(m_spTexture);
		m_spSprite->EndDraw();
		}*/

		m_spD3DDev->EndRender();
	}

	CView::OnTimer(nIDEvent);
}

// 内容菜单
void CResourcePackerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// 文件列表选择更改
void CResourcePackerView::OnLvnItemChangedFileList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	// TODO: 在此添加控件通知处理程序代码
	CResourceInfo* pRI = (CResourceInfo*)pNMLV->lParam;
	if (pRI)
	{
		switch (pRI->btType)
		{
		case rtTexture: 
			if (m_spTexture) 
			{
				CTextureInfo* pTI = static_cast<CTextureInfo*>(pRI);

				m_spTexture->Clear(0);
				m_spTexture->LoadSurfaceFromMemory(0, pTI->pData, (D3DFORMAT)pTI->dwClrFormat, 
												CRect(0, 0, pTI->dwWidth, pTI->dwHeight), 
												pTI->dwWidth * 3, NULL, 
												CRect(0, 0, pTI->dwWidth, pTI->dwHeight));
			}
			break;

		case rt3DModel: 
			break;
		}
	}

	*pResult = 0;
}


// 窗口销毁
void CResourcePackerView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	
	// 销毁渲染时钟
	KillTimer(RENDER_TIMER_ID);

}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

// 初始化渲染视口
bool CResourcePackerView::initRenderView()
{
	// 初始化D3D
	if (MDirect3D::Init(m_hWnd, TRUE))
	{
		// 创建D3D设备
		m_spD3DDev = MDirect3D::CreateDevice();
		if (m_spD3DDev)
		{
			// 创建网格模型对象
			m_spMesh = m_spD3DDev->CreateMeshTeapot();

			// 创建精灵对象
			m_spSprite = m_spD3DDev->CreateSprite();

			// 创建纹理对象
			m_spTexture = m_spD3DDev->CreateTexture(
								MDirect3D::s_PP.BackBufferWidth,
								MDirect3D::s_PP.BackBufferHeight, 
								D3DFMT_A8R8G8B8);

			return m_spMesh && m_spSprite && m_spTexture;
		}
	}

	return false;
}


