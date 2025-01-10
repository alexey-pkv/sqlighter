#include "Stmt.h"


#include "exceptions/sqlighter_exceptions.h"

#include <cstring>


using namespace sqlighter;


Stmt::~Stmt()
{
	close();
}

Stmt::Stmt(std::shared_ptr<DB> db) : 
	m_db(db ? std::move(db) : nullptr)
{
	
}

Stmt::Stmt(sqlite3_stmt* stmt) :
	m_stmt(stmt)
{
	
}

Stmt::Stmt(sqlite3_stmt* stmt, std::shared_ptr<DB> db) : 
	m_stmt(stmt),
	m_db(db ? std::move(db) : nullptr)
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
		throw SQLighterException(SQLIGHTER_ERR_STEP, m_lastCode, m_db);
	}
	
	return m_lastCode;
}


int Stmt::close()
{
	if (m_stmt == nullptr)
		return SQLITE_OK;
	
	auto res = sqlite3_finalize(m_stmt);
	
	m_stmt = nullptr;
	// TODO: Fail on error
	
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

bool Stmt::column_int_n(int at, int& into) const
{
	if (!column_is_null(at))
	{
		into = column_int(at);
		return true;
	}
	else
	{
		into = 0;
		return false;
	}
}

bool Stmt::column_int64_n(int at, int64_t& into) const
{
	if (!column_is_null(at))
	{
		into = column_int64(at);
		return true;
	}
	else
	{
		into = 0;
		return false;
	}
}

bool Stmt::column_bool_n(int at, bool& into) const
{
	if (!column_is_null(at))
	{
		into = column_bool(at);
		return true;
	}
	else
	{
		into = false;
		return false;
	}
}

bool Stmt::column_double_n(int at, double& into) const
{
	if (!column_is_null(at))
	{
		into = column_double(at);
		return true;
	}
	else
	{
		into = 0.0;
		return false;
	}
}

bool Stmt::column_string_n(int at, std::string& into) const
{
	if (!column_is_null(at))
	{
		into = column_string(at);
		return true;
	}
	else
	{
		into.clear();
		return false;
	}
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


template <>
int Stmt::column<int>(int at) const
{
    return column_int(at);
}

template <>
int64_t Stmt::column<int64_t>(int at) const
{
    return column_int64(at);
}

template <>
bool Stmt::column<bool>(int at) const
{
    return column_bool(at);
}

template <>
float Stmt::column<float>(int at) const
{
    return column_double(at);
}

template <>
double Stmt::column<double>(int at) const
{
    return column_double(at);
}

template <>
std::string Stmt::column<std::string>(int at) const
{
    return column_string(at);
}



template <>
bool Stmt::column_n<int>(int at, int& i) const
{
    return column_int_n(at, i);
}

template <>
bool Stmt::column_n<int64_t>(int at, int64_t& i) const
{
    return column_int64_n(at, i);
}

template <>
bool Stmt::column_n<bool>(int at, bool& i) const
{
    return column_bool_n(at, i);
}

template <>
bool Stmt::column_n<float>(int at, float& i) const
{
	double d;
    bool res = column_double_n(at, d);
	
	i = (float)d;
	
	return res;
}

template <>
bool Stmt::column_n<double>(int at, double& i) const
{
    return column_double_n(at, i);
}

template <>
bool Stmt::column_n<std::string>(int at, std::string& i) const
{
    return column_string_n(at, i);
}
