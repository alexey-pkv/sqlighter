#ifndef SQLIGHTER_STMTGUARD_H
#define SQLIGHTER_STMTGUARD_H


#include <sqlite3.h>


namespace sqlighter
{
	class StmtGuard
	{
	private:
		sqlite3_stmt*	m_stmt = nullptr;
		
		
	public:
		~StmtGuard();
		explicit StmtGuard(sqlite3_stmt* stmt);
		
		
	public:
		[[nodiscard]] sqlite3_stmt* stmt();
		[[nodiscard]] const sqlite3_stmt* stmt() const;
		
		int close();
	};
}


#endif