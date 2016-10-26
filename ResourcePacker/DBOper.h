#pragma once

#include "../mlt/predef.h"
#include "../mlt/sqlitedb.h"
using namespace mlt;

#include "TableStruct.h"


// 数据库操作类
class CDBOper
{
public:
	// 初始化数据库
	static bool Initialize();
	// 释放数据库
	static void Finalize();


	/**************************************************************************
	* 文件夹层次表
	**************************************************************************/

	// 查找文件夹信息
	static CFolderInfo* FindFolderInfo(DWORD dwId, DWORD dwParentId = -1);

	// 加载文件夹信息，成功返回加载的记录数量，失败返回-1
	static long LoadFolderInfos();
	// 添加文件夹信息，成功返回新添加的文件夹信息地址，失败返回NULL
	static CFolderInfo* AddFolderInfo(const CFolderInfo& fi);
	// 更新文件夹信息
	static bool UpdateFolderInfo(const CFolderInfo& fi);
	// 删除文件夹信息
	static void DeleteFolderInfo(DWORD dwId, DWORD dwParentId = -1);


	/**************************************************************************
	* 纹理信息表
	**************************************************************************/

	// 加载纹理信息，成功返回加载的记录数量，失败返回-1
	static long LoadTextureInfos(DWORD dwFolderId);
	// 添加纹理信息，成功返回新添加的图片纹理信息地址，失败返回NULL
	static CTextureInfo* AddTextureInfo(CTextureInfo& pi);
	// 更新纹理信息
	static bool UpdateTextureInfo(const CTextureInfo& pi);
	// 删除纹理信息
	static void DeleteTextureInfo(DWORD dwId, DWORD dwFolderId);

	// 清空纹理信息缓存
	static void ClearTextureInfos();


	/**************************************************************************
	* 动画文件表
	**************************************************************************/




public:
	// 数据库对象
	static MSQLiteDB	m_db;

	// 文件夹信息映射
	typedef multimap<DWORD, CFolderInfo>	FOLDER_INFO_MMAP;
	typedef pair<FOLDER_INFO_MMAP::iterator, 
				FOLDER_INFO_MMAP::iterator>		FOLDER_INFO_PAIR;
	static FOLDER_INFO_MMAP		m_mmapFolderInfo;

	// 图片信息映射
	typedef map<DWORD, CTextureInfo>	TEXTURE_INFO_MAP;
	static TEXTURE_INFO_MAP		m_mapTextureInfo;


};

