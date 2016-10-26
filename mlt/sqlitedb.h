/******************************************************************************
*	filename:	sqlitedb.h
*	created:	9/24/2012
*	author:		Loser
*	
*	purpose:	sqlite���ݿ���
******************************************************************************/

#ifndef __MLT_SQLITE_DB_H__
#define __MLT_SQLITE_DB_H__


#include "../wxsqlite/include/sqlite3.h"

#ifdef _DEBUG
	#pragma comment(lib, "../Debug/wxsqlite.lib")
#else
	#pragma comment(lib, "../Release/wxsqlite.lib")
#endif


namespace mlt
{
	/**************************************************************************
	*
	**************************************************************************/

	#ifdef _UNICODE
		#define SQLITE3_OPEN					sqlite3_open16
		#define SQLITE3_BIND_TEXT				sqlite3_bind_text16
		#define SQLITE3_COLUMN_BYTES			sqlite3_column_bytes16
		#define SQLITE3_COLUMN_DATABASE_NAME	sqlite3_column_database_name16
		#define SQLITE3_COLUMN_DECLTYPE			sqlite3_column_decltype16
		#define SQLITE3_COLUMN_NAME				sqlite3_column_name16
		#define SQLITE3_COLUMN_ORIGIN_NAME		sqlite3_column_origin_name16
		#define SQLITE3_COLUMN_TABLE_NAME		sqlite3_column_table_name16
		#define SQLITE3_COLUMN_TEXT				sqlite3_column_text16
		#define SQLITE3_COMPLETE				sqlite3_complete16
		#define SQLITE3_CREATE_COLLATION		sqlite3_create_collation16
		#define SQLITE3_CREATE_FUNCTION			sqlite3_create_function16
		#define SQLITE3_ERRMSG					sqlite3_errmsg16
		#define SQLITE3_PREPARE					sqlite3_prepare16
		#define SQLITE3_PREPARE_V2				sqlite3_prepare16_v2
		#define SQLITE3_RESULT_ERROR			sqlite3_result_error16
		#define SQLITE3_RESULT_TEXT				sqlite3_result_text16
		#define SQLITE3_VALUE_BYTES				sqlite3_value_bytes16
		#define SQLITE_VALUE_TEXT				sqlite3_value_text16
	#else
		#define SQLITE3_OPEN					sqlite3_open
		#define SQLITE3_BIND_TEXT				sqlite3_bind_text
		#define SQLITE3_COLUMN_BYTES			sqlite3_column_bytes
		#define SQLITE3_COLUMN_DATABASE_NAME	sqlite3_column_database_name
		#define SQLITE3_COLUMN_DECLTYPE			sqlite3_column_decltype
		#define SQLITE3_COLUMN_NAME				sqlite3_column_name
		#define SQLITE3_COLUMN_ORIGIN_NAME		sqlite3_column_origin_name
		#define SQLITE3_COLUMN_TABLE_NAME		sqlite3_column_table_name
		#define SQLITE3_COLUMN_TEXT				sqlite3_column_text
		#define SQLITE3_COMPLETE				sqlite3_complete
		#define SQLITE3_CREATE_COLLATION		sqlite3_create_collation
		#define SQLITE3_CREATE_FUNCTION			sqlite3_create_function
		#define SQLITE3_ERRMSG					sqlite3_errmsg
		#define SQLITE3_PREPARE					sqlite3_prepare
		#define SQLITE3_PREPARE_V2				sqlite3_prepare_v2
		#define SQLITE3_RESULT_ERROR			sqlite3_result_error
		#define SQLITE3_RESULT_TEXT				sqlite3_result_text
		#define SQLITE_VALUE_TEXT				sqlite3_value_text
	#endif


	/**************************************************************************
	*
	**************************************************************************/

	// �ص�����
	typedef int (*sqlite3_callback)(
				void*,    /* Data provided in the 4th argument of sqlite3_exec() */
				int,      /* The number of columns in row */
				char**,   /* An array of strings representing fields in the row */
				char**    /* An array of strings representing column names */
				);


	/**************************************************************************
	*
	**************************************************************************/

	// ����
	#define SQLITE_TABLE_BEGIN(name)			static const char* name = "create table if not exists "#name" ("
	//#define SQLITE_FIELD(name, type)			#name" "#type", "
	#define SQLITE_FIELD(name, type, option)	#name" "#type" "#option", "
	#define SQLITE_TABLE_END()					"COMMENT TEXT);";
	
	class MSQLiteDB
	{
	public:
		// ���캯��
		MSQLiteDB();
		// ��������
		virtual ~MSQLiteDB();
		
		// ���ļ�
		bool Open(pcstr pFilename = L(":memory"), pcvoid pKey = NULL, int nKey = 0);
		// �ر��ļ�
		void Close();

		// ��ȡ����
		inline int GetColCount() { return m_nColCnt; }
		// ��ȡ����
		inline int GetRowCount() { return m_nRowCnt; }

		// ��ȡ����
		inline char* GetTableResult(int nRow, int nCol) { return m_ppResult ? m_ppResult[nRow * m_nColCnt + nCol] : NULL; }

		// ��ȡ�������
		inline int GetErrCode() { return m_pDatabase ? sqlite3_errcode(m_pDatabase) : -1; }
		// ��ȡ������Ϣ
		inline pcstr GetErrMsg() { return m_pDatabase ? (pcstr)SQLITE3_ERRMSG(m_pDatabase) : L(""); }

		// ��ȡ���������к�
		inline __int64 GetLastRowId() { return m_pDatabase ? sqlite3_last_insert_rowid(m_pDatabase) : -1; }

		// ִ��SQL��䣬���ص�����
		bool Exec(const char* pSQL, sqlite3_callback pFunc = NULL, pvoid pParam = NULL, ...);

	public:
		// ׼��SQL���
		sqlite3_stmt* Prepare(pcstr pSQL);

		// ���ֶ�ֵ
		inline bool Bind(sqlite3_stmt* pStmt, int nId, int nValue) { return pStmt ? SQLITE_OK == sqlite3_bind_int(pStmt, nId, nValue) : false; }
		inline bool Bind(sqlite3_stmt* pStmt, int nId, __int64 nValue) { return pStmt ? SQLITE_OK == sqlite3_bind_int64(pStmt, nId, nValue) : false; }
		inline bool Bind(sqlite3_stmt* pStmt, int nId, double fValue) { return pStmt ? SQLITE_OK == sqlite3_bind_double(pStmt, nId, fValue) : false; }
		inline bool Bind(sqlite3_stmt* pStmt, int nId, pcstr pText) { return pStmt ? SQLITE_OK == SQLITE3_BIND_TEXT(pStmt, nId, pText, -1, NULL) : false; }
		inline bool Bind(sqlite3_stmt* pStmt, int nId, pcvoid pBlob, int nSize) { return pStmt ? SQLITE_OK == sqlite3_bind_blob(pStmt, nId, pBlob, nSize, NULL) : false; }
		inline bool Bind(sqlite3_stmt* pStmt, int nId, pcvoid pBlob, unsigned __int64 n64Size) { return pStmt ? SQLITE_OK == sqlite3_bind_blob64(pStmt, nId, pBlob, n64Size, NULL) : false; }
		inline bool Bind(sqlite3_stmt* pStmt, int nId) { return pStmt ? SQLITE_OK == sqlite3_bind_null(pStmt, nId) : false; }
		
		// ִ��SQL��䣨����ֵ��SQLITE_BUSY�� SQLITE_DONE�� SQLITE_ROW�� SQLITE_ERROR �� SQLITE_MISUSE��
		inline int Step(sqlite3_stmt* pStmt) { return pStmt ? sqlite3_step(pStmt) : SQLITE_DONE; }

		// ��ȡ����
		inline int GetColCount(sqlite3_stmt* pStmt) { return pStmt ? sqlite3_column_count(pStmt) : 0; }

		// ��ȡ������
		inline int GetColType(sqlite3_stmt* pStmt, int nCol) { return pStmt ? sqlite3_column_type(pStmt, nCol) : 0; }

		// ��ȡ�ֽ���
		inline int GetBytesResult(sqlite3_stmt* pStmt, int nCol) { return pStmt ? SQLITE3_COLUMN_BYTES(pStmt, nCol) : 0; }

		// ��ȡ���
		inline int GetIntResult(sqlite3_stmt* pStmt, int nCol) { return pStmt ? sqlite3_column_int(pStmt, nCol) : 0; }
		inline __int64 GetInt64Result(sqlite3_stmt* pStmt, int nCol) { return pStmt ? sqlite3_column_int64(pStmt, nCol) : 0; }
		inline double GetDoubleResult(sqlite3_stmt* pStmt, int nCol) { return pStmt ? sqlite3_column_double(pStmt, nCol) : 0.0; }
		inline pcstr GetTextResult(sqlite3_stmt* pStmt, int nCol) { return pStmt ? (pcstr)SQLITE3_COLUMN_TEXT(pStmt, nCol) : NULL; }
		inline pcvoid GetBlobResult(sqlite3_stmt* pStmt, int nCol) { return pStmt ? sqlite3_column_blob(pStmt, nCol) : NULL; }
		inline sqlite3_value* GetResult(sqlite3_stmt* pStmt, int nCol) { return pStmt ? sqlite3_column_value(pStmt, nCol) : NULL; }

	private:
		// �ͷŽ����
		void freeResult();
		// �����仺��
		void clearStmtCache();
		
	private:
		// SQL��仺���
		#ifdef _UNICODE
			typedef hash_map<wstring, sqlite3_stmt*>	SQL_STMT_MAP;
		#else
			typedef hash_map<string, sqlite3_stmt*>		SQL_STMT_MAP;	
		#endif
		SQL_STMT_MAP	m_mapSQLStmt;	

		// ���ݿ����
		sqlite3*	m_pDatabase;

		// �����
		char**	m_ppResult;	
	
		// �������
		int		m_nRowCnt;	
		// �������	
		int		m_nColCnt;	
	};
}

#endif // __MLT_SQLITE_DB_H__



