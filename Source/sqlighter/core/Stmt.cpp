#include "Stmt.h"


#include "exceptions/sqlighter_exceptions.h"

#include <cstring>


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
		throw SQLighterException(SQLIGHTER_ERR_STMT_FINALIZED);
	
	return m_stmt;
}

const sqlite3_stmt* Stmt::stmt() const
{
	if (m_stmt == nullptr)
		throw SQLighterException(SQLIGHTER_ERR_STMT_FINALIZED);
	
	return m_stmt;
}


int Stmt::step()
{
	if (m_stmt == nullptr)
		throw SQLighterException(SQLIGHTER_ERR_STMT_FINALIZED, "When calling step()");
	
	m_lastCode = sqlite3_step(m_stmt);
	
	if (m_lastCode != SQLITE_OK && 
		m_lastCode != SQLITE_DONE && 
		m_lastCode != SQLITE_ROW)
	{
		throw SQLighterException(SQLIGHTER_ERR_STEP, m_lastCode);
	}
	
	return m_lastCode;
}


int Stmt::close()
{
	if (m_stmt == nullptr)
		return SQLITE_OK;
	
	auto res = sqlite3_finalize(m_stmt);
	
	m_stmt = nullptr;
	
	return res;
}

void Stmt::require_row() const
{
	if (!hsa_row())
	{
		throw SQLighterException(SQLIGHTER_ERR_NO_ROWS);
	}
}


int Stmt::column_int(int at) const
{
	require_row();
	return sqlite3_column_int(m_stmt, at);
}

int64_t Stmt::column_int64(int at) const
{
	require_row();
	return sqlite3_column_int64(m_stmt, at);
}

bool Stmt::column_bool(int at) const
{
	require_row();
	return sqlite3_column_int(m_stmt, at) != 0;
}

double Stmt::column_double(int at) const
{
	require_row();
	return sqlite3_column_double(m_stmt, at);
}

std::string Stmt::column_string(int at) const
{
	require_row();
	return reinterpret_cast<const char*>(sqlite3_column_text(m_stmt, at));
}

size_t Stmt::column_blob(int at, void** into) const
{
	require_row();
	
	*into = nullptr;
	
	const void* data = sqlite3_column_blob(m_stmt, at);
	
	if (!data)
	{
		return 0;
	}
	
	auto size = sqlite3_column_bytes(m_stmt, at);
	
	*into = malloc(size);
	
	if (!*into)
	{
		throw std::bad_alloc();
	}
	
	memcpy(*into, data, size);

	return size;
}

bool Stmt::column_is_null(int at) const
{
	return column_type(at) == SQLITE_NULL;
}

int Stmt::column_type(int at) const
{
	require_row();
	return sqlite3_column_type(m_stmt, at);
}

int Stmt::column_count() const
{
	require_row();
	return sqlite3_column_count(m_stmt);
}