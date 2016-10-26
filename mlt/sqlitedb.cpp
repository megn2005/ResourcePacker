/******************************************************************************
*	filename:	sqlitedb.cpp
*	created:	9/24/2012
*	author:		Loser
*	
*	purpose:	sqlite���ݿ���
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
	// ���캯��
	//-------------------------------------------------------------------------
	MSQLiteDB::MSQLiteDB() 
		: m_pDatabase(NULL), m_ppResult(NULL), m_nRowCnt(0), m_nColCnt(0)
	{
		m_mapSQLStmt.clear();
	}

	//-------------------------------------------------------------------------
	// ��������
	//-------------------------------------------------------------------------
	MSQLiteDB::~MSQLiteDB()
	{
		Close();
	}

	//-------------------------------------------------------------------------
	// ���ļ�
	//-------------------------------------------------------------------------
	bool MSQLiteDB::Open(pcstr pFilename /*= ":memory"*/, pcvoid pKey /*= NULL*/, int nKey /*= 0*/) 
	{
		// �����ݿ�
		if (SQLITE_OK == SQLITE3_OPEN(pFilename, &m_pDatabase))
		{
			// ��������
			if (pKey && (0 < nKey))
				sqlite3_key(m_pDatabase, pKey, nKey);

			// ��ʼ����������
			return SQLITE_OK == sqlite3_exec(m_pDatabase, "PRAGMA synchronous = OFF", 0, 0, 0) &&	// �������
				SQLITE_OK == sqlite3_exec(m_pDatabase, "PRAGMA cache_size = 8000", 0, 0, 0) &&		// �Ӵ󻺴�
				SQLITE_OK == sqlite3_exec(m_pDatabase, "PRAGMA count_changes = 1", 0, 0, 0) &&		// ���ظı��¼��
				SQLITE_OK == sqlite3_exec(m_pDatabase, "PRAGMA case_sensitive_like = 1", 0, 0, 0);	// ֧������LIKE��ѯ;
		}
		else
			return false;
	}
	
	//-------------------------------------------------------------------------
	// �ر��ļ�
	//-------------------------------------------------------------------------
	void MSQLiteDB::Close() 
	{
		// �ͷŽ����
		freeResult();

		// ���SQL��仺��
		clearStmtCache();

		// �ر����ݿ�
		if (m_pDatabase)
		{
			sqlite3_close(m_pDatabase);
			m_pDatabase = NULL;
		}
	}

	
	//-------------------------------------------------------------------------
	// ִ��SQL���
	//-------------------------------------------------------------------------
	bool MSQLiteDB::Exec(const char* pSQL, sqlite3_callback pFunc /*= NULL*/, pvoid pParam /*= NULL*/, ...) 
	{
		if (!m_pDatabase) 
			return false;

		// �ͷŽ����
		freeResult();

		// ��ʽ��SQL���
		va_list ap;
		va_start(ap, pSQL);
		char* pVmSQL = sqlite3_vmprintf(pSQL, ap);
		va_end(ap);

		// ִ��SQL���
		int nResult = SQLITE_ERROR;
		char* pErrMsg = NULL;

		if (pFunc)
			nResult = sqlite3_exec(m_pDatabase, pVmSQL, pFunc, pParam, &pErrMsg);
		else
			nResult = sqlite3_get_table(m_pDatabase, pVmSQL, &m_ppResult, &m_nRowCnt, &m_nColCnt, &pErrMsg);
		
		// �ж�ִ�н��
		if (SQLITE_OK != nResult)
		{
			// ��ʾ������Ϣ

			// �ͷŴ�����Ϣ
			sqlite3_free(pErrMsg);
		}

		// �ͷ����
		sqlite3_free(pVmSQL);
		
		return SQLITE_OK == nResult;
	}

	//-------------------------------------------------------------------------
	// ׼��SQL���
	//-------------------------------------------------------------------------
	sqlite3_stmt* MSQLiteDB::Prepare(pcstr pSQL)
	{
		if (!m_pDatabase) return false;

		// ����SQL���
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
			// ��հ󶨲���������SQL���
			pStmt = it->second;
			sqlite3_clear_bindings(pStmt);
			sqlite3_reset(pStmt);
		}

		return pStmt;
	}

	//-------------------------------------------------------------------------
	// �ͷŽ����
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
	// �����仺��
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
