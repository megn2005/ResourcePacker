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

#include "stdafx.h"
#include "ResourcePacker.h"
#include "FolderView.h"
#include "MainFrm.h"
#include "InputBoxDlg.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


///////////////////////////////////////////////////////////////////////////////
// CViewTree
///////////////////////////////////////////////////////////////////////////////

// 构造函数
CViewTree::CViewTree()
{
}

// 析构函数
CViewTree::~CViewTree()
{
}


///////////////////////////////////////////////////////////////////////////////
// CViewTree 消息处理程序
///////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CViewTree, CTreeCtrl)
END_MESSAGE_MAP()

// 
BOOL CViewTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}



///////////////////////////////////////////////////////////////////////////////
// CFolderView
///////////////////////////////////////////////////////////////////////////////

// 构造函数
CFolderView::CFolderView()
{
}

// 析构函数
CFolderView::~CFolderView()
{
}


// 获取选中节点ID
DWORD CFolderView::GetSelFolderID()
{
	ATLASSERT(m_tcFolderView.GetSafeHwnd());
	if (!m_tcFolderView.GetSafeHwnd()) return -1;

	// 
	HTREEITEM hItem = m_tcFolderView.GetSelectedItem();
	if (hItem)
	{
		CFolderInfo* pFI = (CFolderInfo*)m_tcFolderView.GetItemData(hItem);
		if (pFI)
			return pFI->dwId;
	}
		
	return -1;
}


///////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar 消息处理程序
///////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CFolderView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()

	ON_COMMAND(ID_FOLDER_TREE_NEW, &CFolderView::OnPopupNewFolder)
	ON_COMMAND(ID_FOLDER_TREE_DELETE, &CFolderView::OnPopupDelete)
	ON_COMMAND(ID_FOLDER_TREE_RENAME, &CFolderView::OnPopupRename)

	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_FOLDER_TREE, &CFolderView::OnTvnEndLabelEdit)
	ON_NOTIFY(NM_CLICK, IDC_FOLDER_TREE, &CFolderView::OnNMClick)
	//ON_NOTIFY(TVN_SELCHANGED, IDC_FOLDER_TREE, &CFolderView::OnTvnSelChanged)

END_MESSAGE_MAP()

// 创建创建
int CFolderView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 创建文件夹视图
	if (!m_tcFolderView.Create(WS_CHILD | WS_VISIBLE | 
							TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | 
							TVS_SHOWSELALWAYS | TVS_EDITLABELS, 
							CRect(0,0,0,0), this, IDC_FOLDER_TREE))
	{
		TRACE0("未能创建文件夹视图\n");
		return -1;      // 未能创建
	}

	// 初始化文件夹树
	initFolderTree();

	return 0;
}

// 窗口改变大小
void CFolderView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (GetSafeHwnd())
		m_tcFolderView.SetWindowPos(NULL, 1, 1, cx - 2, cy - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

// 内容菜单
void CFolderView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_tcFolderView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	pWndTree->SetFocus();

	if (CPoint(-1, -1) != point)
	{
		// 取消或选择已单击的项:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT nFlags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &nFlags);

		if (pWndTree->SelectItem(TVHT_ONITEM & nFlags ? hTreeItem : NULL))
		{
			theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_FOLDER_TREE, point.x, point.y, this, TRUE);
		}
	}
}



// 窗口绘制
void CFolderView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	CRect rectTree;
	m_tcFolderView.GetWindowRect(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

// 设置焦点
void CFolderView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_tcFolderView.SetFocus();
}


// 新文件夹
void CFolderView::OnPopupNewFolder()
{
	CInputBoxDlg dlg(this);
	dlg.m_strCaption = _T("新文件夹");

	// 取消
	if (IDOK != dlg.DoModal() || dlg.m_strInput.IsEmpty())
		return ;

	// 文件夹信息
	CFolderInfo fi;
	fi.strName = dlg.m_strInput;

	// 获取父节点Id
	HTREEITEM hSelItem = m_tcFolderView.GetSelectedItem();
	if (hSelItem)
	{
		// 检查节点类型，关联父节点
		CFolderInfo* pPI = (CFolderInfo*)m_tcFolderView.GetItemData(hSelItem);
		if (pPI)
			fi.dwParentId = pPI->dwId;
		else
			return ;
	}

	// 检查重名
	if (findTreeItem(fi.strName, hSelItem))
	{
		AfxMessageBox(_T("已存在同名的节点！"), MB_OK | MB_ICONINFORMATION);
		return ;
	}

	// 添加节点
	CFolderInfo* pFI = CDBOper::AddFolderInfo(fi);
	if (pFI)
		addTreeItem(*pFI, hSelItem);
}

// 删除
void CFolderView::OnPopupDelete()
{
	// 获取选中节点
	HTREEITEM hSelItem = m_tcFolderView.GetSelectedItem();
	if (hSelItem)
	{
		// 删除确认
		if (IDYES == AfxMessageBox(_T("确认删除选中节点及其所有子节点吗？"), 
									MB_YESNO | MB_ICONQUESTION))
		{
			deleteTreeItem(hSelItem);
		}
	}
}

// 重命名
void CFolderView::OnPopupRename()
{
	// 获取选中节点
	HTREEITEM hSelItem = m_tcFolderView.GetSelectedItem();
	if (hSelItem)
	{
		// 编辑节点
		m_tcFolderView.EditLabel(hSelItem);
	}
}


// 结束标签编辑
void CFolderView::OnTvnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);

	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if (pTVDispInfo->item.pszText)
	{
		// 父节点
		HTREEITEM hParent = m_tcFolderView.GetParentItem(pTVDispInfo->item.hItem);

		// 检查重名
		if (!findTreeItem(pTVDispInfo->item.pszText, hParent))
		{
			CFolderInfo* pFI = (CFolderInfo*)m_tcFolderView.GetItemData(pTVDispInfo->item.hItem);
			if (pFI)
			{
				// 修改节点名称
				pFI->strName = CString(pTVDispInfo->item.pszText).Trim();

				// 更新节点信息
				if (CDBOper::UpdateFolderInfo(*pFI))
					*pResult = 1;
			}
		}
		else
			AfxMessageBox(_T("已存在同名的节点！"), MB_OK | MB_ICONINFORMATION);
	}
}


// 单击
void CFolderView::OnNMClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	CPoint pt;
	GetCursorPos(&pt);
	m_tcFolderView.ScreenToClient(&pt);

	UINT nFlags = 0;
	HTREEITEM hItem = m_tcFolderView.HitTest(pt, &nFlags);

	if (hItem && (TVHT_ONITEM & nFlags))
	{
		// 清理纹理信息缓存
		if (100 < CDBOper::m_mapTextureInfo.size())
			CDBOper::ClearTextureInfos();
		
		// 加载文件夹资源列表
		CFolderInfo* pFI = (CFolderInfo*)m_tcFolderView.GetItemData(hItem);
		if (pFI)
		{
			if (!pFI->bLoaded)
			{
				// 加载纹理信息
				CDBOper::LoadTextureInfos(pFI->dwId);
		
				pFI->bLoaded = TRUE;
			}
		
			// 更新文件夹资源列表
			CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, theApp.GetMainWnd());
			if (pMainFrame)
				pMainFrame->m_wndFileView.UpdateFileList(pFI->dwId);
		}
	}

	*pResult = 0;
}

// 选定项改变
//void CFolderView::OnTvnSelChanged(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
//
//	// TODO: 在此添加控件通知处理程序代码
//	
//	// 清理纹理信息缓存
//	if (100 < CDBOper::m_mapTextureInfo.size())
//		CDBOper::ClearTextureInfos();
//
//	// 加载文件夹资源列表
//	CFolderInfo* pFI = (CFolderInfo*)pNMTreeView->itemNew.lParam;
//	if (pFI)
//	{
//		if (!pFI->bLoaded)
//		{
//			// 加载纹理信息
//			CDBOper::LoadTextureInfos(pFI->dwId);
//
//			pFI->bLoaded = TRUE;
//		}
//
//		// 更新文件夹资源列表
//		CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, theApp.GetMainWnd());
//		if (pMainFrame)
//			pMainFrame->m_wndFileView.UpdateFileList(pFI->dwId);
//	}
//
//	*pResult = 0;
//}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

// 初始化文件夹树
bool CFolderView::initFolderTree()
{
	// 加载资源类型图标
	CBitmap bmp;
	if (!bmp.LoadBitmap(IDB_FILE_ICON_16))
	{
		TRACE(_T("无法加载位图: %x\n"), IDB_FILE_ICON_16);
		ASSERT(FALSE);
		return false;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	// 创建图像列表
	m_objImageList.Create(16, bmpObj.bmHeight, ILC_COLOR24 | ILC_MASK, 0, 0);
	m_objImageList.Add(&bmp, RGB(255, 255, 255));
	// 设置图像列表
	m_tcFolderView.SetImageList(&m_objImageList, TVSIL_NORMAL);

	
	// 加载文件夹信息
	if (0 > CDBOper::LoadFolderInfos())
	{
		TRACE(_T("加载文件夹信息失败！\n"));
		return false;
	}

	// 构建树节点
	return buildTreeItems(TVI_ROOT, -1);
}

// 构建子节点
bool CFolderView::buildTreeItems(HTREEITEM hParent, DWORD dwParentId)
{
	if (!hParent)
		hParent = TVI_ROOT;

	// 遍历子节点
	CDBOper::FOLDER_INFO_PAIR pairFI = CDBOper::m_mmapFolderInfo.equal_range(dwParentId);
	for (CDBOper::FOLDER_INFO_MMAP::iterator it = pairFI.first; it != pairFI.second; it++)
	{
		// 添加子节点
		HTREEITEM hItem = addTreeItem(it->second, hParent);
		// 构建下级节点
		if (!hItem || !buildTreeItems(hItem, it->second.dwId))
			return false;
	}

	return true;
}

// 添加树节点
HTREEITEM CFolderView::addTreeItem(const CFolderInfo& fi, HTREEITEM hParent)
{
	if (!hParent)
		hParent = TVI_ROOT;

	// 添加树节点
	HTREEITEM hItem = m_tcFolderView.InsertItem(fi.strName, 0, 0, hParent, TVI_LAST);
	if (hItem)
	{
		// 关联节点和数据信息
		m_tcFolderView.SetItemData(hItem, (DWORD_PTR)&fi);
		// 展开父节点
		m_tcFolderView.Expand(hParent, TVE_EXPAND);
		// 选中新添加的节点
		m_tcFolderView.SelectItem(hItem);
	}

	return hItem;
}

// 查找子节点
HTREEITEM CFolderView::findTreeItem(const CString& strName, HTREEITEM hParent)
{
	HTREEITEM hItem = NULL;

	// 获取遍历起始节点
	if (hParent)
	{
		if (m_tcFolderView.ItemHasChildren(hParent))
			hItem = m_tcFolderView.GetNextItem(hParent, TVGN_CHILD);
	}
	else
		hItem = m_tcFolderView.GetRootItem();

	// 遍历子节点
	while (hItem)
	{
		// 获取节点文本并比较
		if (m_tcFolderView.GetItemText(hItem) == strName)
			break;
		else
			hItem = m_tcFolderView.GetNextItem(hItem, TVGN_NEXT);
	}

	return hItem;
}

// 删除子节点
void CFolderView::deleteTreeItem(HTREEITEM hItem)
{
	if (!hItem)
		hItem = TVI_ROOT;

	// 递归删除子节点
	if (m_tcFolderView.ItemHasChildren(hItem))
	{
		// 遍历子节点
		HTREEITEM hNext, hChild = m_tcFolderView.GetNextItem(hItem, TVGN_CHILD);
		while (hChild)
		{
			hNext = m_tcFolderView.GetNextItem(hChild, TVGN_NEXT);
			deleteTreeItem(hChild);
			hChild = hNext;
		}
	}

	// 删除本节点数据
	CFolderInfo* pFI = (CFolderInfo*)m_tcFolderView.GetItemData(hItem);
	if (pFI)
		CDBOper::DeleteFolderInfo(pFI->dwId, pFI->dwParentId);

	// 删除本节点
	m_tcFolderView.DeleteItem(hItem);
}

