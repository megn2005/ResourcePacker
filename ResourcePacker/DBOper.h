#pragma once

#include "../mlt/predef.h"
#include "../mlt/sqlitedb.h"
using namespace mlt;

#include "TableStruct.h"


// ���ݿ������
class CDBOper
{
public:
	// ��ʼ�����ݿ�
	static bool Initialize();
	// �ͷ����ݿ�
	static void Finalize();


	/**************************************************************************
	* �ļ��в�α�
	**************************************************************************/

	// �����ļ�����Ϣ
	static CFolderInfo* FindFolderInfo(DWORD dwId, DWORD dwParentId = -1);

	// �����ļ�����Ϣ���ɹ����ؼ��صļ�¼������ʧ�ܷ���-1
	static long LoadFolderInfos();
	// ����ļ�����Ϣ���ɹ���������ӵ��ļ�����Ϣ��ַ��ʧ�ܷ���NULL
	static CFolderInfo* AddFolderInfo(const CFolderInfo& fi);
	// �����ļ�����Ϣ
	static bool UpdateFolderInfo(const CFolderInfo& fi);
	// ɾ���ļ�����Ϣ
	static void DeleteFolderInfo(DWORD dwId, DWORD dwParentId = -1);


	/**************************************************************************
	* ������Ϣ��
	**************************************************************************/

	// ����������Ϣ���ɹ����ؼ��صļ�¼������ʧ�ܷ���-1
	static long LoadTextureInfos(DWORD dwFolderId);
	// ���������Ϣ���ɹ���������ӵ�ͼƬ������Ϣ��ַ��ʧ�ܷ���NULL
	static CTextureInfo* AddTextureInfo(CTextureInfo& pi);
	// ����������Ϣ
	static bool UpdateTextureInfo(const CTextureInfo& pi);
	// ɾ��������Ϣ
	static void DeleteTextureInfo(DWORD dwId, DWORD dwFolderId);

	// ���������Ϣ����
	static void ClearTextureInfos();


	/**************************************************************************
	* �����ļ���
	**************************************************************************/




public:
	// ���ݿ����
	static MSQLiteDB	m_db;

	// �ļ�����Ϣӳ��
	typedef multimap<DWORD, CFolderInfo>	FOLDER_INFO_MMAP;
	typedef pair<FOLDER_INFO_MMAP::iterator, 
				FOLDER_INFO_MMAP::iterator>		FOLDER_INFO_PAIR;
	static FOLDER_INFO_MMAP		m_mmapFolderInfo;

	// ͼƬ��Ϣӳ��
	typedef map<DWORD, CTextureInfo>	TEXTURE_INFO_MAP;
	static TEXTURE_INFO_MAP		m_mapTextureInfo;


};

