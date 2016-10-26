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

#pragma once


///////////////////////////////////////////////////////////////////////////////
// CViewTree 窗口
///////////////////////////////////////////////////////////////////////////////

class CViewTree : public CTreeCtrl
{

public:
	// 构造/析构函数
	CViewTree();
	virtual ~CViewTree();

	// 重写
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);


protected:
	DECLARE_MESSAGE_MAP()
};


///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

class CFolderViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};


///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

class CFolderView : public CDockablePane
{
public:
	// 构造/析构函数
	CFolderView();
	virtual ~CFolderView();


	// 获取选中文件夹ID
	DWORD GetSelFolderID();


protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	// 新文件夹
	afx_msg void OnPopupNewFolder();
	// 删除
	afx_msg void OnPopupDelete();
	// 重命名
	afx_msg void OnPopupRename();

	// 结束标签编辑
	afx_msg void OnTvnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	// 单击
	afx_msg void OnNMClick(NMHDR* pNMHDR, LRESULT* pResult);
	// 选定项改变
	//afx_msg void OnTvnSelChanged(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

private:
	// 初始化文件夹树
	bool initFolderTree();
	// 构建子节点
	bool buildTreeItems(HTREEITEM hParent, DWORD dwParentId);
	// 添加子节点
	HTREEITEM addTreeItem(const CFolderInfo& fi, HTREEITEM hParent);
	// 查找子节点
	HTREEITEM findTreeItem(const CString& strName, HTREEITEM hParent);
	// 删除树节点
	void deleteTreeItem(HTREEITEM hItem);

// 特性
private:
	// 文件夹树
	CViewTree		m_tcFolderView;
	// 图标列表
	CImageList		m_objImageList;
};

