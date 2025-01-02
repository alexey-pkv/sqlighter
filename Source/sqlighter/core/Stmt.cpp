#include "Stmt.h"


#include "../exceptions/sqlighter_exceptions.h"


using namespace sqlighter;


Stmt::~Stmt()
{
	close();
}

Stmt::Stmt(sqlite3_stmt* stmt)
	: m_stmt(stmt)
{
	
}


sqlite3_stmt* Stmt::stmt()
{
	if (m_stmt == nullptr)
		throw StmtFinalizedException();
	
	return m_stmt;
}

const sqlite3_stmt* Stmt::stmt() const
{
	if (m_stmt == nullptr)
		throw StmtFinalizedException();
	
	return m_stmt;
}


int Stmt::close()
{
	if (m_stmt != nullptr)
	{
		return sqlite3_finalize(m_stmt);
	}
	
	return SQLITE_OK;
}
