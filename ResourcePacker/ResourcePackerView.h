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

// ResourcePackerView.h : CResourcePackerView 类的接口
//

#pragma once


///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

class CResourcePackerView : public CView
{
	DECLARE_DYNCREATE(CResourcePackerView)

public:
	// 析构函数
	virtual ~CResourcePackerView();

	// 获取文档
	CResourcePackerDoc* GetDocument() const;

	// 创建irrlicht设备
	bool CreateIrrDevice();

// 重写
public:
	// 预创建窗口
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	// 重写以绘制该视图
	virtual void OnDraw(CDC* pDC);  


	// 诊断
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


protected: 
	// 仅从序列化创建
	CResourcePackerView();


// 生成的消息映射函数
protected:
	// 窗口创建
	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	// 窗口改变大小
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// 渲染时钟
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// 内容菜单
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	// 文件列表选择更改
	afx_msg void OnLvnItemChangedFileList(NMHDR* pNMHDR, LRESULT* pResult);

	// 窗口销毁
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()


private:
	// 初始化渲染视口
	bool initRenderView();

private:
	// D3D设备对象
	SP_ID3DDEVICE	m_spD3DDev;
	// 精灵对象
	SP_ID3DSPRITE	m_spSprite;

	// 图片纹理对象
	SP_ID3DTEXTURE	m_spTexture;
	// 网格模型对象
	SP_ID3DMESH		m_spMesh;
	
};

#ifndef _DEBUG  // ResourcePackerView.cpp 中的调试版本
inline CResourcePackerDoc* CResourcePackerView::GetDocument() const
   { return reinterpret_cast<CResourcePackerDoc*>(m_pDocument); }
#endif

