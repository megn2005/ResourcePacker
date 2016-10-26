/******************************************************************************
*	filename:	sqlitedb.cpp
*	created:	9/24/2012
*	author:		Loser
*	
*	purpose:	sqlite数据库类
******************************************************************************/

#include <string>
#include <hash_map>
using namespace std;
using namespace stdext;

#include "predef.h"
#include "sqlitedb.h"

namespace mlt
{
	//-------------------------------------------------------------------------
	// 构造函数
	//-------------------------------------------------------------------------
	MSQLiteDB::MSQLiteDB() 
		: m_pDatabase(NULL), m_ppResult(NULL), m_nRowCnt(0), m_nColCnt(0)
	{
		m_mapSQLStmt.clear();
	}

	//-------------------------------------------------------------------------
	// 析构函数
	//-------------------------------------------------------------------------
	MSQLiteDB::~MSQLiteDB()
	{
		Close();
	}

	//-------------------------------------------------------------------------
	// 打开文件
	//-------------------------------------------------------------------------
	bool MSQLiteDB::Open(pcstr pFilename /*= ":memory"*/, pcvoid pKey /*= NULL*/, int nKey /*= 0*/) 
	{
		// 打开数据库
		if (SQLITE_OK == SQLITE3_OPEN(pFilename, &m_pDatabase))
		{
			// 设置密码
			if (pKey && (0 < nKey))
				sqlite3_key(m_pDatabase, pKey, nKey);

			// 初始化环境变量
			return SQLITE_OK == sqlite3_exec(m_pDatabase, "PRAGMA synchronous = OFF", 0, 0, 0) &&	// 提高性能
				SQLITE_OK == sqlite3_exec(m_pDatabase, "PRAGMA cache_size = 8000", 0, 0, 0) &&		// 加大缓存
				SQLITE_OK == sqlite3_exec(m_pDatabase, "PRAGMA count_changes = 1", 0, 0, 0) &&		// 返回改变记录数
				SQLITE_OK == sqlite3_exec(m_pDatabase, "PRAGMA case_sensitive_like = 1", 0, 0, 0);	// 支持中文LIKE查询;
		}
		else
			return false;
	}
	
	//-------------------------------------------------------------------------
	// 关闭文件
	//-------------------------------------------------------------------------
	void MSQLiteDB::Close() 
	{
		// 释放结果集
		freeResult();

		// 清空SQL语句缓存
		clearStmtCache();

		// 关闭数据库
		if (m_pDatabase)
		{
			sqlite3_close(m_pDatabase);
			m_pDatabase = NULL;
		}
	}

	
	//-------------------------------------------------------------------------
	// 执行SQL语句
	//-------------------------------------------------------------------------
	bool MSQLiteDB::Exec(const char* pSQL, sqlite3_callback pFunc /*= NULL*/, pvoid pParam /*= NULL*/, ...) 
	{
		if (!m_pDatabase) 
			return false;

		// 释放结果集
		freeResult();

		// 格式化SQL语句
		va_list ap;
		va_start(ap, pSQL);
		char* pVmSQL = sqlite3_vmprintf(pSQL, ap);
		va_end(ap);

		// 执行SQL语句
		int nResult = SQLITE_ERROR;
		char* pErrMsg = NULL;

		if (pFunc)
			nResult = sqlite3_exec(m_pDatabase, pVmSQL, pFunc, pParam, &pErrMsg);
		else
			nResult = sqlite3_get_table(m_pDatabase, pVmSQL, &m_ppResult, &m_nRowCnt, &m_nColCnt, &pErrMsg);
		
		// 判断执行结果
		if (SQLITE_OK != nResult)
		{
			// 显示错误信息

			// 释放错误信息
			sqlite3_free(pErrMsg);
		}

		// 释放语句
		sqlite3_free(pVmSQL);
		
		return SQLITE_OK == nResult;
	}

	//-------------------------------------------------------------------------
	// 准备SQL语句
	//-------------------------------------------------------------------------
	sqlite3_stmt* MSQLiteDB::Prepare(pcstr pSQL)
	{
		if (!m_pDatabase) return false;

		// 缓存SQL语句
		sqlite3_stmt* pStmt = NULL;
		SQL_STMT_MAP::iterator it = m_mapSQLStmt.find(pSQL);

		if (m_mapSQLStmt.end() == it)
		{
			if (SQLITE_OK == SQLITE3_PREPARE_V2(m_pDatabase, pSQL, -1, &pStmt, NULL)) 
			{
				m_mapSQLStmt[pSQL] = pStmt;
			}
		}
		else
		{
			// 清空绑定参数，重置SQL语句
			pStmt = it->second;
			sqlite3_clear_bindings(pStmt);
			sqlite3_reset(pStmt);
		}

		return pStmt;
	}

	//-------------------------------------------------------------------------
	// 释放结果集
	//-------------------------------------------------------------------------
	void MSQLiteDB::freeResult() 
	{
		if (m_ppResult)
		{
			sqlite3_free_table(m_ppResult);
			m_ppResult = NULL;
			m_nRowCnt = m_nColCnt = 0;
		}
	}

	//-------------------------------------------------------------------------
	// 清空语句缓存
	//-------------------------------------------------------------------------
	void MSQLiteDB::clearStmtCache() 
	{
		SQL_STMT_MAP::iterator itEnd = m_mapSQLStmt.end();
		for (SQL_STMT_MAP::iterator it = m_mapSQLStmt.begin(); it != itEnd; it++)
		{
			if (it->second)
				sqlite3_finalize(it->second);
		}
		m_mapSQLStmt.clear();
	}
	

	
}
