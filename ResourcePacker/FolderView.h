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

#pragma once


///////////////////////////////////////////////////////////////////////////////
// CViewTree ����
///////////////////////////////////////////////////////////////////////////////

class CViewTree : public CTreeCtrl
{

public:
	// ����/��������
	CViewTree();
	virtual ~CViewTree();

	// ��д
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
	// ����/��������
	CFolderView();
	virtual ~CFolderView();


	// ��ȡѡ���ļ���ID
	DWORD GetSelFolderID();


protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	// ���ļ���
	afx_msg void OnPopupNewFolder();
	// ɾ��
	afx_msg void OnPopupDelete();
	// ������
	afx_msg void OnPopupRename();

	// ������ǩ�༭
	afx_msg void OnTvnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	// ����
	afx_msg void OnNMClick(NMHDR* pNMHDR, LRESULT* pResult);
	// ѡ����ı�
	//afx_msg void OnTvnSelChanged(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

private:
	// ��ʼ���ļ�����
	bool initFolderTree();
	// �����ӽڵ�
	bool buildTreeItems(HTREEITEM hParent, DWORD dwParentId);
	// ����ӽڵ�
	HTREEITEM addTreeItem(const CFolderInfo& fi, HTREEITEM hParent);
	// �����ӽڵ�
	HTREEITEM findTreeItem(const CString& strName, HTREEITEM hParent);
	// ɾ�����ڵ�
	void deleteTreeItem(HTREEITEM hItem);

// ����
private:
	// �ļ�����
	CViewTree		m_tcFolderView;
	// ͼ���б�
	CImageList		m_objImageList;
};

