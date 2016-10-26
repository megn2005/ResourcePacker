#include "stdafx.h"
#include "DBOper.h"

// 数据库对象
MSQLiteDB CDBOper::m_db;

// 文件夹信息映射
CDBOper::FOLDER_INFO_MMAP CDBOper::m_mmapFolderInfo;
// 图片信息映射
CDBOper::TEXTURE_INFO_MAP CDBOper::m_mapTextureInfo;


// 初始化数据库
bool CDBOper::Initialize()
{
	TCHAR szKey[] = { _T("32802841mlt") };

	if (m_db.Open(_T("data.pak")/*, szKey, sizeof(szKey)*/))
	{
		return m_db.Exec(folder_level) && 
			m_db.Exec(texture_info) && 
			m_db.Exec(anime_info) && 
			m_db.Exec(model_info) && 
			m_db.Exec(model_texture);
	}

	return false;
}

// 释放数据库
void CDBOper::Finalize()
{
	// 清空缓存
	ClearTextureInfos();

	// 关闭数据库
	m_db.Close();
}


// 查找文件夹信息
CFolderInfo* CDBOper::FindFolderInfo(DWORD dwId, DWORD dwParentId /*= -1*/)
{
	FOLDER_INFO_PAIR pairFI;

	// 确定搜索范围
	if (-1 != dwParentId)
		pairFI = m_mmapFolderInfo.equal_range(dwParentId);
	else
		pairFI = make_pair(m_mmapFolderInfo.begin(), m_mmapFolderInfo.end());

	// 开始遍历
	for (FOLDER_INFO_MMAP::iterator it = pairFI.first; it != pairFI.second; it++)
	{
		if (dwId == it->second.dwId)
			return &it->second;
	}

	return NULL;
}

// 加载文件夹信息
long CDBOper::LoadFolderInfos()
{
	sqlite3_stmt* pStmt = m_db.Prepare(_T("select * from folder_level"));
	if (pStmt)
	{
		CFolderInfo fi;

		while (SQLITE_ROW == m_db.Step(pStmt))
		{
			fi.dwId = (DWORD)m_db.GetInt64Result(pStmt, 0);
			fi.strName = m_db.GetTextResult(pStmt, 1);
			fi.dwParentId = (DWORD)m_db.GetInt64Result(pStmt, 2);

			m_mmapFolderInfo.insert(make_pair(fi.dwParentId, fi));
		}

		return m_mmapFolderInfo.size();
	}

	return -1;
}

// 添加文件夹信息
CFolderInfo* CDBOper::AddFolderInfo(const CFolderInfo& fi)
{
	sqlite3_stmt* pStmt = m_db.Prepare(_T("insert into folder_level (name, parent_id) values (?, ?)"));
	if (pStmt)
	{
		m_db.Bind(pStmt, 1, fi.strName);
		m_db.Bind(pStmt, 2, (INT64)fi.dwParentId);

		if (SQLITE_ROW == m_db.Step(pStmt))
		{
			FOLDER_INFO_MMAP::iterator it = m_mmapFolderInfo.insert(make_pair(fi.dwParentId, fi));
			it->second.dwId = (DWORD)m_db.GetLastRowId();
			return &it->second;
		}
		else
			AfxMessageBox(m_db.GetErrMsg(), MB_OK | MB_ICONERROR);
	}

	return NULL;
}

// 更新文件夹信息
bool CDBOper::UpdateFolderInfo(const CFolderInfo& fi)
{
	sqlite3_stmt* pStmt = m_db.Prepare(_T("update folder_level set name = ?, parent_id = ? where id = ?"));
	if (pStmt)
	{
		m_db.Bind(pStmt, 1, fi.strName);
		m_db.Bind(pStmt, 2, (INT64)fi.dwParentId);
		m_db.Bind(pStmt, 3, (INT64)fi.dwId);

		if (SQLITE_ROW == m_db.Step(pStmt))
		{
			// 更新缓存信息
			CFolderInfo* pFI = FindFolderInfo(fi.dwId, fi.dwParentId);
			if (!pFI)
				m_mmapFolderInfo.insert(make_pair(fi.dwParentId, fi));
			else
				*pFI = fi;

			return true;
		}
		else
			AfxMessageBox(m_db.GetErrMsg(), MB_OK | MB_ICONERROR);
	}

	return false;
}

// 删除文件夹信息
void CDBOper::DeleteFolderInfo(DWORD dwId, DWORD dwParentId /*= -1*/)
{
	// 删除子节点信息
	if (0 < m_mmapFolderInfo.count(dwId))
	{
		// 有问题，迭代器失效
		FOLDER_INFO_PAIR pairFI = m_mmapFolderInfo.equal_range(dwId);
		for (FOLDER_INFO_MMAP::iterator it = pairFI.first; it != pairFI.second; )
		{
			FOLDER_INFO_MMAP::iterator itDel = it++;
			DeleteFolderInfo(itDel->second.dwId, dwId);
		}
	}
	else
	{
		// 删除本节点
		sqlite3_stmt* pStmt = m_db.Prepare(_T("delete from folder_level where id = ?"));
		if (pStmt)
		{
			m_db.Bind(pStmt, 1, (INT64)dwId);

			if (SQLITE_ROW == m_db.Step(pStmt))
			{
				// 删除缓存信息
				FOLDER_INFO_PAIR pairFI;

				// 确定搜索范围
				if (-1 != dwParentId)
					pairFI = m_mmapFolderInfo.equal_range(dwParentId);
				else
					pairFI = make_pair(m_mmapFolderInfo.begin(), m_mmapFolderInfo.end());

				// 遍历删除
				for (FOLDER_INFO_MMAP::iterator it = pairFI.first; it != pairFI.second; it++)
				{
					if (dwId == it->second.dwId)
					{
						m_mmapFolderInfo.erase(it);
						break;
					}
				}
			}
			else
				AfxMessageBox(m_db.GetErrMsg(), MB_OK | MB_ICONERROR);
		}
	}
}


// 加载图片信息
long CDBOper::LoadTextureInfos(DWORD dwFolderId)
{
	sqlite3_stmt* pStmt = m_db.Prepare(_T("select * from texture_info where folder_id = ?"));
	if (pStmt)
	{
		CTextureInfo ti;

		// 绑定参数
		m_db.Bind(pStmt, 1, (INT64)dwFolderId);

		// 获取记录
		while (SQLITE_ROW == m_db.Step(pStmt))
		{
			ti.dwId = (DWORD)m_db.GetInt64Result(pStmt, 0);
			ti.strName = m_db.GetTextResult(pStmt, 1);
			ti.dwFolderId = (DWORD)m_db.GetInt64Result(pStmt, 2);
			ti.dwWidth = (DWORD)m_db.GetIntResult(pStmt, 3);
			ti.dwHeight = (DWORD)m_db.GetIntResult(pStmt, 4);
			ti.dwClrFormat = m_db.GetIntResult(pStmt, 5);
			ti.bCompressed = m_db.GetIntResult(pStmt, 6);
			ti.u64Size = m_db.GetInt64Result(pStmt, 7);
			ti.pData = new BYTE[ti.u64Size];

			// 保存blob字段数据
			CopyMemory(ti.pData, m_db.GetBlobResult(pStmt, 8), ti.u64Size);

			// 缓存记录
			m_mapTextureInfo[ti.dwId] = ti;
		}

		return m_mapTextureInfo.size();
	}

	return -1;
}

// 添加图片信息
CTextureInfo* CDBOper::AddTextureInfo(CTextureInfo& ti)
{
	sqlite3_stmt* pStmt = m_db.Prepare(
		_T("insert into texture_info (name, folder_id, width, height, color_format, compressed, size, data) ")
		_T("values (?, ?, ?, ?, ?, ?, ?, ?)"));
	if (pStmt)
	{
		m_db.Bind(pStmt, 1, ti.strName);
		m_db.Bind(pStmt, 2, (INT64)ti.dwFolderId);
		m_db.Bind(pStmt, 3, (INT64)ti.dwWidth);
		m_db.Bind(pStmt, 4, (INT64)ti.dwHeight);
		m_db.Bind(pStmt, 5, (INT64)ti.dwClrFormat);
		m_db.Bind(pStmt, 6, ti.bCompressed);
		m_db.Bind(pStmt, 7, (INT64)ti.u64Size);
		m_db.Bind(pStmt, 8, ti.pData, ti.u64Size);

		if (SQLITE_ROW == m_db.Step(pStmt))
		{
			// 资源ID
			ti.dwId = (DWORD)m_db.GetLastRowId();

			// 缓存资源信息
			pair<TEXTURE_INFO_MAP::iterator, bool> pairIt = m_mapTextureInfo.insert(make_pair(ti.dwId, ti));
			if (pairIt.second)
				return &pairIt.first->second;
		}
		else
			AfxMessageBox(m_db.GetErrMsg(), MB_OK | MB_ICONERROR);
	}

	return NULL;
}

// 更新图片信息
bool CDBOper::UpdateTextureInfo(const CTextureInfo& ti)
{
	sqlite3_stmt* pStmt = m_db.Prepare(
		_T("update texture_info set name = ?, folder_id = ?, width = ?, height = ?, ")
		_T("color_format = ?, compressed = ?, size = ?, data = ? where id = ?"));
	if (pStmt)
	{
		m_db.Bind(pStmt, 1, ti.strName);
		m_db.Bind(pStmt, 2, (INT64)ti.dwFolderId);
		m_db.Bind(pStmt, 3, (INT64)ti.dwWidth);
		m_db.Bind(pStmt, 4, (INT64)ti.dwHeight);
		m_db.Bind(pStmt, 5, (INT64)ti.dwClrFormat);
		m_db.Bind(pStmt, 6, ti.bCompressed);
		m_db.Bind(pStmt, 7, (INT64)ti.u64Size);
		m_db.Bind(pStmt, 8, ti.pData, ti.u64Size);
		m_db.Bind(pStmt, 9, (INT64)ti.dwId);

		if (SQLITE_ROW == m_db.Step(pStmt))
		{
			// 更新缓存信息
			m_mapTextureInfo[ti.dwId] = ti;
			return true;
		}
		else
			AfxMessageBox(m_db.GetErrMsg(), MB_OK | MB_ICONERROR);
	}

	return false;
}

// 删除图片信息
void CDBOper::DeleteTextureInfo(DWORD dwId, DWORD dwFolderId)
{
	sqlite3_stmt* pStmt = m_db.Prepare(_T("delete from texture_info where id = ? and folder_id = ?"));
	if (pStmt)
	{
		m_db.Bind(pStmt, 1, (INT64)dwId);
		m_db.Bind(pStmt, 2, (INT64)dwFolderId);

		if (SQLITE_ROW == m_db.Step(pStmt))
		{
			TEXTURE_INFO_MAP::iterator it = m_mapTextureInfo.find(dwId);
			if (m_mapTextureInfo.end() != it)
			{
				ATLASSERT(dwFolderId == it->second.dwFolderId);
				DELETE_ARY(it->second.pData);
				m_mapTextureInfo.erase(it);
			}
		}
		else
			AfxMessageBox(m_db.GetErrMsg(), MB_OK | MB_ICONERROR);
	}
}

// 清空图片缓存信息
void CDBOper::ClearTextureInfos()
{
	for (TEXTURE_INFO_MAP::iterator it = m_mapTextureInfo.begin(); 
		it != m_mapTextureInfo.end(); it++)
	{
		if (it->second.pData)
			delete [] it->second.pData;
	}
	m_mapTextureInfo.clear();
}







