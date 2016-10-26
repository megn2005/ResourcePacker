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

// ResourcePackerView.cpp : CResourcePackerView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "ResourcePacker.h"
#endif

#include "ResourcePackerDoc.h"
#include "ResourcePackerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ��Ⱦʱ��ID
#define	RENDER_TIMER_ID		1


///////////////////////////////////////////////////////////////////////////////
// CResourcePackerView
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CResourcePackerView, CView)

// CResourcePackerView ���캯��
CResourcePackerView::CResourcePackerView()
	: m_spD3DDev(nullptr), m_spSprite(nullptr), m_spTexture(nullptr), m_spMesh(nullptr)
{
	// TODO: �ڴ˴���ӹ������
	
}

// ��������
CResourcePackerView::~CResourcePackerView()
{
}


// Ԥ��������
BOOL CResourcePackerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CResourcePackerView ����
void CResourcePackerView::OnDraw(CDC* /*pDC*/)
{
	CResourcePackerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���

	if (!m_spD3DDev)
	{
		// ��ʼ����Ⱦ�ӿ�
		if (initRenderView())
		{
			// ��Ⱦʱ��
			SetTimer(RENDER_TIMER_ID, 33, NULL);
		}
	}

}


///////////////////////////////////////////////////////////////////////////////
// CResourcePackerView ���
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

CResourcePackerDoc* CResourcePackerView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CResourcePackerDoc)));
	return (CResourcePackerDoc*)m_pDocument;
}
#endif //_DEBUG


///////////////////////////////////////////////////////////////////////////////
// CResourcePackerView ��Ϣ�������
///////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CResourcePackerView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CONTEXTMENU()

	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FILE_LIST, &CResourcePackerView::OnLvnItemChangedFileList)

END_MESSAGE_MAP()


// ���ڴ���
int CResourcePackerView::OnCreate(LPCREATESTRUCT lpcs)
{
	if (-1 == CView::OnCreate(lpcs))
		return -1;

	//

	return 0;
}

// ���ڸı��С
void CResourcePackerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if (m_spD3DDev)
	{
		// �޸ĺ�̨�����С
		MDirect3D::s_PP.BackBufferWidth = cx;
		MDirect3D::s_PP.BackBufferHeight = cy;

		// �޸�ͼƬ�����С
		m_spTexture.reset();
		m_spTexture = m_spD3DDev->CreateTexture(cx, cy, D3DFMT_A8R8G8B8);

		m_spD3DDev->Reset();
	}
}

// ��Ⱦʱ��
void CResourcePackerView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (m_spD3DDev)
	{
		m_spD3DDev->BeginRender();

		// ��������ģ��
		if (m_spMesh)
			m_spMesh->DrawSubset(0);

		// ����ͼƬ����
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

// ���ݲ˵�
void CResourcePackerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// �ļ��б�ѡ�����
void CResourcePackerView::OnLvnItemChangedFileList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	// TODO: �ڴ���ӿؼ�֪ͨ����������
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


// ��������
void CResourcePackerView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	
	// ������Ⱦʱ��
	KillTimer(RENDER_TIMER_ID);

}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

// ��ʼ����Ⱦ�ӿ�
bool CResourcePackerView::initRenderView()
{
	// ��ʼ��D3D
	if (MDirect3D::Init(m_hWnd, TRUE))
	{
		// ����D3D�豸
		m_spD3DDev = MDirect3D::CreateDevice();
		if (m_spD3DDev)
		{
			// ��������ģ�Ͷ���
			m_spMesh = m_spD3DDev->CreateMeshTeapot();

			// �����������
			m_spSprite = m_spD3DDev->CreateSprite();

			// �����������
			m_spTexture = m_spD3DDev->CreateTexture(
								MDirect3D::s_PP.BackBufferWidth,
								MDirect3D::s_PP.BackBufferHeight, 
								D3DFMT_A8R8G8B8);

			return m_spMesh && m_spSprite && m_spTexture;
		}
	}

	return false;
}


