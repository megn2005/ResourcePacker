#pragma once


///////////////////////////////////////////////////////////////////////////////
// CFileView
///////////////////////////////////////////////////////////////////////////////

class CFileView : public CDockablePane
{
	DECLARE_DYNAMIC(CFileView)

public:
	// 构造/析构函数
	CFileView();
	virtual ~CFileView();


	// 更新文件列表
	bool UpdateFileList(DWORD dwFolderId);


protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	// 查看方式菜单命令
	afx_msg void OnPopupViewRangeCmds(UINT nID);
	// 更新查看方式菜单
	afx_msg void OnPopupViewRangeUpdate(CCmdUI* pCmdUI);

	// 添加纹理图片
	afx_msg void OnPopupAddTexture();
	// 添加3D模型
	afx_msg void OnPopupAdd3DModel();
	// 添加其它
	afx_msg void OnPopupAddOther();

	// 删除
	afx_msg void OnPopupDelete();
	// 重命名
	afx_msg void OnPopupRename();

	// 文件列表选择更改
	afx_msg void OnLvnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);


	DECLARE_MESSAGE_MAP()

private:
	// 初始化列表控件
	bool initFileList();
	// 初始化列头
	bool initColumnHeader();

	// 加载子项
	bool loadListItems(DWORD dwFolderId);
	// 添加子项
	int addListItem(const CResourceInfo* pRI);

private:
	// 加载纹理从文件
	CTextureInfo loadTextureFromFile(const CString& strDir, const CString& strFilename);
	// 获取图像颜色格式
	UINT getImgClrFormat(const CImage& img);

	// 显示图标更改
	void onViewChange();
	// 格式化文件大小
	CString formatSize(UINT64 n64Size);


private:
	// 列表控件窗口
	CListCtrl		m_lcFileView;
	// 图标列表
	CImageList		m_objImageList;
};


