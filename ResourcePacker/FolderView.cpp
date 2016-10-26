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

// ���캯��
CViewTree::CViewTree()
{
}

// ��������
CViewTree::~CViewTree()
{
}


///////////////////////////////////////////////////////////////////////////////
// CViewTree ��Ϣ�������
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

// ���캯��
CFolderView::CFolderView()
{
}

// ��������
CFolderView::~CFolderView()
{
}


// ��ȡѡ�нڵ�ID
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
// CWorkspaceBar ��Ϣ�������
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

// ��������
int CFolderView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// �����ļ�����ͼ
	if (!m_tcFolderView.Create(WS_CHILD | WS_VISIBLE | 
							TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | 
							TVS_SHOWSELALWAYS | TVS_EDITLABELS, 
							CRect(0,0,0,0), this, IDC_FOLDER_TREE))
	{
		TRACE0("δ�ܴ����ļ�����ͼ\n");
		return -1;      // δ�ܴ���
	}

	// ��ʼ���ļ�����
	initFolderTree();

	return 0;
}

// ���ڸı��С
void CFolderView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (GetSafeHwnd())
		m_tcFolderView.SetWindowPos(NULL, 1, 1, cx - 2, cy - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

// ���ݲ˵�
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
		// ȡ����ѡ���ѵ�������:
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



// ���ڻ���
void CFolderView::OnPaint()
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������

	CRect rectTree;
	m_tcFolderView.GetWindowRect(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

// ���ý���
void CFolderView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_tcFolderView.SetFocus();
}


// ���ļ���
void CFolderView::OnPopupNewFolder()
{
	CInputBoxDlg dlg(this);
	dlg.m_strCaption = _T("���ļ���");

	// ȡ��
	if (IDOK != dlg.DoModal() || dlg.m_strInput.IsEmpty())
		return ;

	// �ļ�����Ϣ
	CFolderInfo fi;
	fi.strName = dlg.m_strInput;

	// ��ȡ���ڵ�Id
	HTREEITEM hSelItem = m_tcFolderView.GetSelectedItem();
	if (hSelItem)
	{
		// ���ڵ����ͣ��������ڵ�
		CFolderInfo* pPI = (CFolderInfo*)m_tcFolderView.GetItemData(hSelItem);
		if (pPI)
			fi.dwParentId = pPI->dwId;
		else
			return ;
	}

	// �������
	if (findTreeItem(fi.strName, hSelItem))
	{
		AfxMessageBox(_T("�Ѵ���ͬ���Ľڵ㣡"), MB_OK | MB_ICONINFORMATION);
		return ;
	}

	// ��ӽڵ�
	CFolderInfo* pFI = CDBOper::AddFolderInfo(fi);
	if (pFI)
		addTreeItem(*pFI, hSelItem);
}

// ɾ��
void CFolderView::OnPopupDelete()
{
	// ��ȡѡ�нڵ�
	HTREEITEM hSelItem = m_tcFolderView.GetSelectedItem();
	if (hSelItem)
	{
		// ɾ��ȷ��
		if (IDYES == AfxMessageBox(_T("ȷ��ɾ��ѡ�нڵ㼰�������ӽڵ���"), 
									MB_YESNO | MB_ICONQUESTION))
		{
			deleteTreeItem(hSelItem);
		}
	}
}

// ������
void CFolderView::OnPopupRename()
{
	// ��ȡѡ�нڵ�
	HTREEITEM hSelItem = m_tcFolderView.GetSelectedItem();
	if (hSelItem)
	{
		// �༭�ڵ�
		m_tcFolderView.EditLabel(hSelItem);
	}
}


// ������ǩ�༭
void CFolderView::OnTvnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	if (pTVDispInfo->item.pszText)
	{
		// ���ڵ�
		HTREEITEM hParent = m_tcFolderView.GetParentItem(pTVDispInfo->item.hItem);

		// �������
		if (!findTreeItem(pTVDispInfo->item.pszText, hParent))
		{
			CFolderInfo* pFI = (CFolderInfo*)m_tcFolderView.GetItemData(pTVDispInfo->item.hItem);
			if (pFI)
			{
				// �޸Ľڵ�����
				pFI->strName = CString(pTVDispInfo->item.pszText).Trim();

				// ���½ڵ���Ϣ
				if (CDBOper::UpdateFolderInfo(*pFI))
					*pResult = 1;
			}
		}
		else
			AfxMessageBox(_T("�Ѵ���ͬ���Ľڵ㣡"), MB_OK | MB_ICONINFORMATION);
	}
}


// ����
void CFolderView::OnNMClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CPoint pt;
	GetCursorPos(&pt);
	m_tcFolderView.ScreenToClient(&pt);

	UINT nFlags = 0;
	HTREEITEM hItem = m_tcFolderView.HitTest(pt, &nFlags);

	if (hItem && (TVHT_ONITEM & nFlags))
	{
		// ����������Ϣ����
		if (100 < CDBOper::m_mapTextureInfo.size())
			CDBOper::ClearTextureInfos();
		
		// �����ļ�����Դ�б�
		CFolderInfo* pFI = (CFolderInfo*)m_tcFolderView.GetItemData(hItem);
		if (pFI)
		{
			if (!pFI->bLoaded)
			{
				// ����������Ϣ
				CDBOper::LoadTextureInfos(pFI->dwId);
		
				pFI->bLoaded = TRUE;
			}
		
			// �����ļ�����Դ�б�
			CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, theApp.GetMainWnd());
			if (pMainFrame)
				pMainFrame->m_wndFileView.UpdateFileList(pFI->dwId);
		}
	}

	*pResult = 0;
}

// ѡ����ı�
//void CFolderView::OnTvnSelChanged(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
//
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	
//	// ����������Ϣ����
//	if (100 < CDBOper::m_mapTextureInfo.size())
//		CDBOper::ClearTextureInfos();
//
//	// �����ļ�����Դ�б�
//	CFolderInfo* pFI = (CFolderInfo*)pNMTreeView->itemNew.lParam;
//	if (pFI)
//	{
//		if (!pFI->bLoaded)
//		{
//			// ����������Ϣ
//			CDBOper::LoadTextureInfos(pFI->dwId);
//
//			pFI->bLoaded = TRUE;
//		}
//
//		// �����ļ�����Դ�б�
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

// ��ʼ���ļ�����
bool CFolderView::initFolderTree()
{
	// ������Դ����ͼ��
	CBitmap bmp;
	if (!bmp.LoadBitmap(IDB_FILE_ICON_16))
	{
		TRACE(_T("�޷�����λͼ: %x\n"), IDB_FILE_ICON_16);
		ASSERT(FALSE);
		return false;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	// ����ͼ���б�
	m_objImageList.Create(16, bmpObj.bmHeight, ILC_COLOR24 | ILC_MASK, 0, 0);
	m_objImageList.Add(&bmp, RGB(255, 255, 255));
	// ����ͼ���б�
	m_tcFolderView.SetImageList(&m_objImageList, TVSIL_NORMAL);

	
	// �����ļ�����Ϣ
	if (0 > CDBOper::LoadFolderInfos())
	{
		TRACE(_T("�����ļ�����Ϣʧ�ܣ�\n"));
		return false;
	}

	// �������ڵ�
	return buildTreeItems(TVI_ROOT, -1);
}

// �����ӽڵ�
bool CFolderView::buildTreeItems(HTREEITEM hParent, DWORD dwParentId)
{
	if (!hParent)
		hParent = TVI_ROOT;

	// �����ӽڵ�
	CDBOper::FOLDER_INFO_PAIR pairFI = CDBOper::m_mmapFolderInfo.equal_range(dwParentId);
	for (CDBOper::FOLDER_INFO_MMAP::iterator it = pairFI.first; it != pairFI.second; it++)
	{
		// ����ӽڵ�
		HTREEITEM hItem = addTreeItem(it->second, hParent);
		// �����¼��ڵ�
		if (!hItem || !buildTreeItems(hItem, it->second.dwId))
			return false;
	}

	return true;
}

// ������ڵ�
HTREEITEM CFolderView::addTreeItem(const CFolderInfo& fi, HTREEITEM hParent)
{
	if (!hParent)
		hParent = TVI_ROOT;

	// ������ڵ�
	HTREEITEM hItem = m_tcFolderView.InsertItem(fi.strName, 0, 0, hParent, TVI_LAST);
	if (hItem)
	{
		// �����ڵ��������Ϣ
		m_tcFolderView.SetItemData(hItem, (DWORD_PTR)&fi);
		// չ�����ڵ�
		m_tcFolderView.Expand(hParent, TVE_EXPAND);
		// ѡ������ӵĽڵ�
		m_tcFolderView.SelectItem(hItem);
	}

	return hItem;
}

// �����ӽڵ�
HTREEITEM CFolderView::findTreeItem(const CString& strName, HTREEITEM hParent)
{
	HTREEITEM hItem = NULL;

	// ��ȡ������ʼ�ڵ�
	if (hParent)
	{
		if (m_tcFolderView.ItemHasChildren(hParent))
			hItem = m_tcFolderView.GetNextItem(hParent, TVGN_CHILD);
	}
	else
		hItem = m_tcFolderView.GetRootItem();

	// �����ӽڵ�
	while (hItem)
	{
		// ��ȡ�ڵ��ı����Ƚ�
		if (m_tcFolderView.GetItemText(hItem) == strName)
			break;
		else
			hItem = m_tcFolderView.GetNextItem(hItem, TVGN_NEXT);
	}

	return hItem;
}

// ɾ���ӽڵ�
void CFolderView::deleteTreeItem(HTREEITEM hItem)
{
	if (!hItem)
		hItem = TVI_ROOT;

	// �ݹ�ɾ���ӽڵ�
	if (m_tcFolderView.ItemHasChildren(hItem))
	{
		// �����ӽڵ�
		HTREEITEM hNext, hChild = m_tcFolderView.GetNextItem(hItem, TVGN_CHILD);
		while (hChild)
		{
			hNext = m_tcFolderView.GetNextItem(hChild, TVGN_NEXT);
			deleteTreeItem(hChild);
			hChild = hNext;
		}
	}

	// ɾ�����ڵ�����
	CFolderInfo* pFI = (CFolderInfo*)m_tcFolderView.GetItemData(hItem);
	if (pFI)
		CDBOper::DeleteFolderInfo(pFI->dwId, pFI->dwParentId);

	// ɾ�����ڵ�
	m_tcFolderView.DeleteItem(hItem);
}

