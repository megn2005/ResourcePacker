// FileView.cpp : 实现文件
//

#include "stdafx.h"
#include "ResourcePacker.h"
#include "FileView.h"
#include "MainFrm.h"



// 列信息数组
LVCOLUMN g_szLvCols[4] = 
{
	//{ UINT mask; int fmt; int cx; LPTSTR pszText; int cchTextMax; int iSubItem; int iImage; int iOrder; }
	{ LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, LVCFMT_IMAGE | LVCFMT_LEFT, 250, _T("名称"), 2, 0, 0 },
	{ LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, LVCFMT_LEFT, 150, _T("类型"), 2, 0, 0 },
	{ LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, LVCFMT_LEFT, 100, _T("大小"), 2, 0, 0 },
	{ LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, LVCFMT_LEFT, 150, _T("修改日期"), 4, 0, 0 }
};


///////////////////////////////////////////////////////////////////////////////
// CFileView
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CFileView, CDockablePane)

// 析构函数
CFileView::CFileView()
{
}

// 构造函数
CFileView::~CFileView()
{
}


// 更新列表
bool CFileView::UpdateFileList(DWORD dwFolderId)
{
	ATLASSERT(m_lcFileView.GetSafeHwnd());
	if (!m_lcFileView.GetSafeHwnd()) return false;

	// 清空列表
	m_lcFileView.DeleteAllItems();

	// 加载列表项
	return loadListItems(dwFolderId);
}


///////////////////////////////////////////////////////////////////////////////
// CFileView 消息处理程序
///////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()

	ON_COMMAND_RANGE(ID_FILE_LIST_ICON, ID_FILE_LIST_REPORT, &CFileView::OnPopupViewRangeCmds)
	ON_UPDATE_COMMAND_UI_RANGE(ID_FILE_LIST_ICON, ID_FILE_LIST_REPORT, &CFileView::OnPopupViewRangeUpdate)

	ON_COMMAND(ID_FILE_LIST_TEXTURE, &CFileView::OnPopupAddTexture)
	ON_COMMAND(ID_FILE_LIST_3D_MODEL, &CFileView::OnPopupAdd3DModel)
	ON_COMMAND(ID_FILE_LIST_OTHER, &CFileView::OnPopupAddOther)
	ON_COMMAND(ID_FILE_LIST_DELETE, &CFileView::OnPopupDelete)
	ON_COMMAND(ID_FILE_LIST_RENAME, &CFileView::OnPopupRename)

	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FILE_LIST, &CFileView::OnLvnItemChanged)

END_MESSAGE_MAP()


// 创建创建
int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 创建文件夹视图
	if (!m_lcFileView.Create(WS_CHILD | WS_VISIBLE | 
							LVS_ALIGNTOP | LVS_AUTOARRANGE | LVS_EDITLABELS | 
							LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SORTASCENDING, 
							CRect(0,0,0,0), this, IDC_FILE_LIST))
	{
		TRACE0("未能创建文件视图\n");
		return -1;      // 未能创建
	}

	// 初始化文件列表
	initFileList();

	return 0;
}

// 窗口改变大小
void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (GetSafeHwnd())
		m_lcFileView.SetWindowPos(NULL, 1, 1, cx - 2, cy - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

// 内容菜单
void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CListCtrl* pWndList = (CListCtrl*)&m_lcFileView;
	ASSERT_VALID(pWndList);

	if (pWnd != pWndList)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	pWndList->SetFocus();

	if (CPoint(-1, -1) != point)
	{
		// 选择已单击的项:
		CPoint ptList = point;
		pWndList->ScreenToClient(&ptList);

		UINT nFlags = 0;
		int nListItem = pWndList->HitTest(ptList, &nFlags);

		if (0 > nListItem || 
			pWndList->SetItem(nListItem, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED, LVIS_SELECTED, 0))
		{
			HMENU hMenu = theApp.GetContextMenuManager()->GetMenuById(IDR_POPUP_FILE_LIST);
			if (hMenu)
			{
				//CMenu* pMenu = CMenu::FromHandle(hMenu);
				//if (pMenu && pMenu->GetSubMenu(0))
				//{
				//	pMenu->GetSubMenu(0)->CheckMenuRadioItem(0, 4, nListItem, MF_BYPOSITION);

				//}

				
				CheckMenuRadioItem(hMenu, 
					ID_FILE_LIST_ICON, ID_FILE_LIST_REPORT, 
					ID_FILE_LIST_ICON + nListItem, MF_BYCOMMAND);
			}

			theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_FILE_LIST, point.x, point.y, this, TRUE);
		}
	}
}



// 窗口绘制
void CFileView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	CRect rectTree;
	m_lcFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

// 设置焦点
void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_lcFileView.SetFocus();
}


// 查看方式菜单命令
void CFileView::OnPopupViewRangeCmds(UINT nID)
{
	switch (nID)
	{
	case ID_FILE_LIST_ICON: m_lcFileView.ModifyStyle(LVS_TYPEMASK, LVS_ICON); break;
	case ID_FILE_LIST_SMALL_ICON: m_lcFileView.ModifyStyle(LVS_TYPEMASK, LVS_SMALLICON); break;
	case ID_FILE_LIST_LIST: m_lcFileView.ModifyStyle(LVS_TYPEMASK, LVS_LIST); break;
	case ID_FILE_LIST_REPORT: m_lcFileView.ModifyStyle(LVS_TYPEMASK, LVS_REPORT); break;
	}

	// 更新显示图标
	onViewChange();
}

// 更新查看方式菜单
void CFileView::OnPopupViewRangeUpdate(CCmdUI* pCmdUI)
{
	switch (LVS_TYPEMASK & m_lcFileView.GetStyle())
	{
	case LVS_ICON: pCmdUI->SetRadio(ID_FILE_LIST_ICON == pCmdUI->m_nID); break;
	case LVS_SMALLICON: pCmdUI->SetRadio(ID_FILE_LIST_SMALL_ICON == pCmdUI->m_nID); break;
	case LVS_LIST: pCmdUI->SetRadio(ID_FILE_LIST_LIST == pCmdUI->m_nID); break;
	case LVS_REPORT: pCmdUI->SetRadio(ID_FILE_LIST_REPORT == pCmdUI->m_nID); break;
	}
}


// 添加纹理图片
void CFileView::OnPopupAddTexture()
{
	CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, theApp.GetMainWnd());
	if (!pMainFrame) return ;

	// 文件过滤数组
	static TCHAR szFilter[] = _T("位图文件 (*.bmp)|*.bmp|") 
							_T("JPG图像文件 (*.jpg;*.jpeg)|*.jpg;*.jpeg|") 
							_T("PNG图像文件 (*.png)|*.png|") 
							_T("GIF图像文件 (*.gif)|*.gif|") 
							_T("所有文件 (*.*)|*.*||");
	// 文件名缓存大小
	const int nBufSize = 100 * (MAX_PATH + 1) + 1;
	// 文件名缓存
	static TCHAR szFile[nBufSize] = { 0 };

	// 打开对话框
	CFileDialog dlgFile(TRUE, NULL, NULL, 
		OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY, 
		szFilter, this);

	ZeroMemory(szFile, sizeof(szFile));
	dlgFile.m_ofn.lpstrFile = szFile;
	dlgFile.m_ofn.nMaxFile = nBufSize;

	//
	if (IDOK == dlgFile.DoModal())
	{
		// 是否选择多个文件
		if (dlgFile.m_ofn.nFileOffset > _tcslen(szFile))
		{
			LPCTSTR pDir = szFile;
			LPCTSTR pFileName = szFile + dlgFile.m_ofn.nFileOffset;

			while (pFileName[0])
			{
				// 加载纹理图片
				CTextureInfo ti = loadTextureFromFile(pDir, pFileName);
				if (ti.pData)
				{	
					// 关联文件夹ID
					ti.dwFolderId = pMainFrame->m_wndFolderView.GetSelFolderID();

					// 添加图片到列表
					CTextureInfo* pTI = CDBOper::AddTextureInfo(ti);
					if (pTI)
						addListItem(pTI);
				}

				// 下一个文件名
				pFileName += _tcslen(pFileName) + 1;
			}
		}
		else // 只选中一个文件
		{
			CString strFile(szFile);
			CTextureInfo ti = loadTextureFromFile(strFile.Left(dlgFile.m_ofn.nFileOffset), 
												strFile.Mid(dlgFile.m_ofn.nFileOffset));
			if (ti.pData)
			{	
				// 关联文件夹ID
				ti.dwFolderId = pMainFrame->m_wndFolderView.GetSelFolderID();

				// 添加图片到列表
				CTextureInfo* pTI = CDBOper::AddTextureInfo(ti);
				if (pTI)
					addListItem(pTI);
			}
		}
	}
}

// 添加3D模型
void CFileView::OnPopupAdd3DModel()
{

}

// 添加其它
void CFileView::OnPopupAddOther()
{

}

// 删除
void CFileView::OnPopupDelete()
{
	CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, theApp.GetMainWnd());
	if (!pMainFrame) return ;
	
	// 获取文件夹Id
	DWORD dwFolderId = pMainFrame->m_wndFolderView.GetSelFolderID();
		
	// 删除选中文件
	POSITION pos = m_lcFileView.GetFirstSelectedItemPosition();
	while (pos)
	{
		int nItem = m_lcFileView.GetNextSelectedItem(pos);

		// 删除数据库记录
		CResourceInfo* pRI = (CResourceInfo*)m_lcFileView.GetItemData(nItem);
		if (pRI)
		{
			switch (pRI->btType)
			{
			case rtTexture: CDBOper::DeleteTextureInfo(pRI->dwId, dwFolderId); break;
			case rt3DModel: break;
			}
		}

		// 删除列表项
		m_lcFileView.DeleteItem(nItem);

		pos = m_lcFileView.GetFirstSelectedItemPosition();
	}
}

// 重命名
void CFileView::OnPopupRename()
{

}


// 文件列表选择更改
void CFileView::OnLvnItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	// TODO: 在此添加控件通知处理程序代码
	
	// 消息转发
	CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, theApp.GetMainWnd());
	if (pMainFrame)
	{
		CView* pView = pMainFrame->GetActiveView();
		if (pView)
			pView->SendMessage(WM_NOTIFY, (WPARAM)pNMLV->hdr.idFrom, (LPARAM)pNMLV);
	}

	*pResult = 0;
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

// 初始化文件列表
bool CFileView::initFileList()
{
	// 初始化列头
	initColumnHeader();

	// 更新显示图标
	onViewChange();

	return false;
}

// 初始化列头
bool CFileView::initColumnHeader()
{
	for (int i = 0; i < _countof(g_szLvCols); i++)
	{
		if (0 > m_lcFileView.InsertColumn(i, &g_szLvCols[i]))
			return false;
	}

	return true;
}

// 加载子项
bool CFileView::loadListItems(DWORD dwFolderId)
{
	// 填充文件夹列表项
	//CDBOper::FOLDER_INFO_PAIR pairFI = CDBOper::m_mmapFolderInfo.equal_range(dwFolderId);
	//for (CDBOper::FOLDER_INFO_MMAP::iterator itFI = pairFI.first; itFI != pairFI.second; itFI++)
	//{
	//	if (0 > addListItem(&itFI->second))
	//		return false;
	//}

	// 填充纹理图片列表项
	CDBOper::TEXTURE_INFO_MAP::iterator itEnd = CDBOper::m_mapTextureInfo.end();
	for (CDBOper::TEXTURE_INFO_MAP::iterator it = CDBOper::m_mapTextureInfo.begin(); it != itEnd; it++)
	{
		if (dwFolderId == it->second.dwFolderId)
		{
			if (0 > addListItem(&it->second))
			{
				ASSERT(FALSE);
				return false;
			}
		}	
	}
	
	return true;
}

// 添加子项
int CFileView::addListItem(const CResourceInfo* pRI)
{
	int nIndex = -1;

	switch (pRI->btType)
	{
	case rtTexture: 
		{
			const CTextureInfo* pTI = static_cast<const CTextureInfo*>(pRI);
			if (pTI)
			{
				nIndex = m_lcFileView.InsertItem(m_lcFileView.GetItemCount(), pTI->strName, 1);
				if (0 <= nIndex)
				{
					// 类型
					m_lcFileView.SetItemText(nIndex, 1, _T("图片"));	
					// 大小
					m_lcFileView.SetItemText(nIndex, 2, formatSize(pTI->u64Size));	
					// 修改日期
					m_lcFileView.SetItemText(nIndex, 3, _T(""));		
				}
			}
		}
		break;

	case rt3DModel: 
		{
			const CModelInfo* pMI = static_cast<const CModelInfo*>(pRI);
			if (pMI)
			{
				nIndex = m_lcFileView.InsertItem(m_lcFileView.GetItemCount(), pMI->strName, 2);
				if (0 <= nIndex)
				{
					// 类型
					m_lcFileView.SetItemText(nIndex, 1, _T("模型"));
					// 大小
					m_lcFileView.SetItemText(nIndex, 2, formatSize(pMI->u64Size));
					// 修改日期
					m_lcFileView.SetItemText(nIndex, 3, _T(""));
				}
			}
		}
		break;

	case rtOther: 
		break;

	default: 
		nIndex = m_lcFileView.InsertItem(m_lcFileView.GetItemCount(), _T("???"), -1);
		if (0 <= nIndex)
		{
			// 类型
			m_lcFileView.SetItemText(nIndex, 1, _T("未知"));
			// 大小
			m_lcFileView.SetItemText(nIndex, 2, _T("???"));
			// 修改日期
			m_lcFileView.SetItemText(nIndex, 3, _T("???"));
		}
		break;
	}

	// 关联数据
	if (0 <= nIndex)
		m_lcFileView.SetItemData(nIndex, (DWORD_PTR)pRI);

	return nIndex;
}


// 加载纹理从文件
CTextureInfo CFileView::loadTextureFromFile(const CString& strDir, const CString& strName)
{
	CTextureInfo ti;
	CImage img;
	CString strPath;

	// 合成文件路径
	if (_T("\\") == strDir.Right(1))
		strPath = strDir + strName;
	else
		strPath = strDir + _T("\\") + strName;

	// 加载纹理图片
	if (SUCCEEDED(img.Load(strPath)))
	{
		ti.strName = strName;
		ti.dwWidth = img.GetWidth();
		ti.dwHeight = img.GetHeight();
		ti.dwClrFormat = getImgClrFormat(img);
		ti.bCompressed = FALSE;
		ti.u64Size = img.GetWidth() * img.GetHeight() * img.GetBPP() / 8;
		ti.pData = new BYTE[ti.u64Size];

		// 拷贝图像数据到缓存
		for (int i = 0; i < img.GetHeight(); i++)
		{
			CopyMemory(((PBYTE)ti.pData) + i * img.GetWidth() * img.GetBPP() / 8, 
				((PBYTE)img.GetBits()) + i * img.GetPitch(), img.GetWidth() * img.GetBPP() / 8);
		}
	}

	return ti;
}

// 获取图像颜色格式
UINT CFileView::getImgClrFormat(const CImage& img)
{
	switch (img.GetBPP())
	{
	case 32: return TEXFMT_A8R8G8B8;
	case 24: return TEXFMT_R8G8B8;
	case 16: return TEXFMT_R5G6B5;
	default: return TEXFMT_UNKNOWN;
	}
}

// 显示更改
void CFileView::onViewChange()
{
	// 清空旧图像
	m_objImageList.DeleteImageList();

	// 图像资源ID
	UINT uBmpId = LVS_ICON == (LVS_TYPEMASK & m_lcFileView.GetStyle()) ? 
						IDB_FILE_ICON_32 : IDB_FILE_ICON_16;

	// 加载位图
	CBitmap bmp;
	if (!bmp.LoadBitmap(uBmpId))
	{
		TRACE(_T("无法加载位图: %x\n"), uBmpId);
		ASSERT(FALSE);
		return;
	}

	// 创建图像列表
	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	m_objImageList.Create(bmpObj.bmWidth / 5, bmpObj.bmHeight, ILC_COLOR24 | ILC_MASK, 0, 0);
	m_objImageList.Add(&bmp, RGB(255, 255, 255));

	m_lcFileView.SetImageList(&m_objImageList, 
		LVS_ICON == (LVS_TYPEMASK & m_lcFileView.GetStyle()) ? LVSIL_NORMAL : LVSIL_SMALL);
}

// 格式化文件大小
CString CFileView::formatSize(UINT64 n64Size)
{
	static LPCTSTR szUnits[] = { _T("B"), _T("KB"), _T("MB"), _T("GB"), _T("TB"), _T("") };

	CString strSize;

	for (int i = 0; i < _countof(szUnits); i++)
	{
		if (n64Size < 1024)
		{
			strSize.Format(_T("%lld%s"), n64Size, szUnits[i]);
			break;
		}
		else if (1024 < n64Size && n64Size < 1024 * 1024)
		{
			strSize.Format(_T("%lld,%lld%s"), n64Size / 1024, n64Size % 1024, szUnits[i]);
			break;
		}
		else
			n64Size /= 1024;
	}

	return strSize;
}


