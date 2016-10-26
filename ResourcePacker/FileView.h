#pragma once


///////////////////////////////////////////////////////////////////////////////
// CFileView
///////////////////////////////////////////////////////////////////////////////

class CFileView : public CDockablePane
{
	DECLARE_DYNAMIC(CFileView)

public:
	// ����/��������
	CFileView();
	virtual ~CFileView();


	// �����ļ��б�
	bool UpdateFileList(DWORD dwFolderId);


protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	// �鿴��ʽ�˵�����
	afx_msg void OnPopupViewRangeCmds(UINT nID);
	// ���²鿴��ʽ�˵�
	afx_msg void OnPopupViewRangeUpdate(CCmdUI* pCmdUI);

	// �������ͼƬ
	afx_msg void OnPopupAddTexture();
	// ���3Dģ��
	afx_msg void OnPopupAdd3DModel();
	// �������
	afx_msg void OnPopupAddOther();

	// ɾ��
	afx_msg void OnPopupDelete();
	// ������
	afx_msg void OnPopupRename();

	// �ļ��б�ѡ�����
	afx_msg void OnLvnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);


	DECLARE_MESSAGE_MAP()

private:
	// ��ʼ���б�ؼ�
	bool initFileList();
	// ��ʼ����ͷ
	bool initColumnHeader();

	// ��������
	bool loadListItems(DWORD dwFolderId);
	// �������
	int addListItem(const CResourceInfo* pRI);

private:
	// ����������ļ�
	CTextureInfo loadTextureFromFile(const CString& strDir, const CString& strFilename);
	// ��ȡͼ����ɫ��ʽ
	UINT getImgClrFormat(const CImage& img);

	// ��ʾͼ�����
	void onViewChange();
	// ��ʽ���ļ���С
	CString formatSize(UINT64 n64Size);


private:
	// �б�ؼ�����
	CListCtrl		m_lcFileView;
	// ͼ���б�
	CImageList		m_objImageList;
};


