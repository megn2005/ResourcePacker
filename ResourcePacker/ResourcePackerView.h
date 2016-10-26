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

// ResourcePackerView.h : CResourcePackerView ��Ľӿ�
//

#pragma once


///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

class CResourcePackerView : public CView
{
	DECLARE_DYNCREATE(CResourcePackerView)

public:
	// ��������
	virtual ~CResourcePackerView();

	// ��ȡ�ĵ�
	CResourcePackerDoc* GetDocument() const;

	// ����irrlicht�豸
	bool CreateIrrDevice();

// ��д
public:
	// Ԥ��������
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	// ��д�Ի��Ƹ���ͼ
	virtual void OnDraw(CDC* pDC);  


	// ���
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


protected: 
	// �������л�����
	CResourcePackerView();


// ���ɵ���Ϣӳ�亯��
protected:
	// ���ڴ���
	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	// ���ڸı��С
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// ��Ⱦʱ��
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// ���ݲ˵�
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	// �ļ��б�ѡ�����
	afx_msg void OnLvnItemChangedFileList(NMHDR* pNMHDR, LRESULT* pResult);

	// ��������
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()


private:
	// ��ʼ����Ⱦ�ӿ�
	bool initRenderView();

private:
	// D3D�豸����
	SP_ID3DDEVICE	m_spD3DDev;
	// �������
	SP_ID3DSPRITE	m_spSprite;

	// ͼƬ�������
	SP_ID3DTEXTURE	m_spTexture;
	// ����ģ�Ͷ���
	SP_ID3DMESH		m_spMesh;
	
};

#ifndef _DEBUG  // ResourcePackerView.cpp �еĵ��԰汾
inline CResourcePackerDoc* CResourcePackerView::GetDocument() const
   { return reinterpret_cast<CResourcePackerDoc*>(m_pDocument); }
#endif

