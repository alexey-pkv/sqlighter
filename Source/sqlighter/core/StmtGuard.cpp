#include "StmtGuard.h"


#include "../exceptions/sqlighter_exceptions.h"


using namespace sqlighter;


StmtGuard::~StmtGuard()
{
	close();
}

StmtGuard::StmtGuard(sqlite3_stmt* stmt)
	: m_stmt(stmt)
{
	
}


sqlite3_stmt* StmtGuard::stmt()
{
	if (m_stmt == nullptr)
		throw StmtFinalizedException();
	
	return m_stmt;
}

const sqlite3_stmt* StmtGuard::stmt() const
{
	if (m_stmt == nullptr)
		throw StmtFinalizedException();
	
	return m_stmt;
}


int StmtGuard::close()
{
	if (m_stmt != nullptr)
	{
		return sqlite3_finalize(m_stmt);
	}
	
	return SQLITE_OK;
}
