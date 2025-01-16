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

Stmt::Stmt(std::shared_ptr<DB> db, std::string_view query) : 
	m_db(db ? std::move(db) : nullptr),
	m_query(query)
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

ScalarValue Stmt::to_value(int at) const
{
	switch (column_type(at))
	{
		case SQLITE_NULL:
			return ScalarValue::null();
		
		case SQLITE_INTEGER:
			return ScalarValue(column_int64(at));
			
		case SQLITE_FLOAT:
			return ScalarValue(column_double(at));
			
		case SQLITE_BLOB:
			return ScalarValue(column_blob(at));
			
		case SQLITE_TEXT:
			return ScalarValue(column_string(at));
			
		default:
			throw SQLighterException(SQLIGHTER_ERR_UNEXPECTED)
				.msg("Got unexpected column type: " + std::to_string(column_type(at)))
				.query(m_query);
	}
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
		auto e = SQLighterException(SQLIGHTER_ERR_STEP)
			.sqlite_error(m_lastCode)
			.query(m_query)
			.db_msg(m_db);
		
		throw e;
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
	if (!has_row())
	{
		throw SQLighterException(SQLIGHTER_ERR_NO_ROWS).query(m_query);
	}
}

void Stmt::require_column(int at) const
{
	require_row();
	
	if (at < 0)
	{
		throw SQLighterException(SQLIGHTER_ERR_INVALID_COLUMN, "Got: " + std::to_string(at));
	}
	else if (column_count() <= at)
	{
		throw SQLighterException::no_column(at, column_count());
	}
}

void Stmt::require_column_type(int at, int type) const
{
	if (type != column_type(at))
	{
		throw SQLighterException(SQLIGHTER_ERR_VALUE)
			.msg(
				"Expected column of type " + std::to_string(type) +
				" but got " + std::to_string(column_type(at)))
			.query(m_query);
	}
}


int Stmt::column_int(int at) const
{
	require_column(at);
	return sqlite3_column_int(m_stmt, at);
}

int64_t Stmt::column_int64(int at) const
{
	require_column(at);
	return sqlite3_column_int64(m_stmt, at);
}

bool Stmt::column_bool(int at) const
{
	require_column(at);
	return sqlite3_column_int(m_stmt, at) != 0;
}

double Stmt::column_double(int at) const
{
	require_column(at);
	return sqlite3_column_double(m_stmt, at);
}

std::string Stmt::column_string(int at) const
{
	require_column(at);
	return reinterpret_cast<const char*>(sqlite3_column_text(m_stmt, at));
}

size_t Stmt::column_blob(int at, void** into) const
{
	require_column(at);
	
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
		throw std::bad_alloc(); // LCOV_EXCL_LINE
	}
	
	memcpy(*into, data, size);

	return size;
}

blob_t Stmt::column_blob(int at) const
{
	if (column_type(at) == SQLITE_NULL)
		return {};
	
	require_column_type(at, SQLITE_BLOB);
		
	const void* data = sqlite3_column_blob(m_stmt, at);
	auto size = sqlite3_column_bytes(m_stmt, at);
	
	if (!data || size <= 0)
	{
		return {};
	}
	
	return {
		static_cast<const uint8_t*>(data), 
		static_cast<const uint8_t*>(data) + size
	};
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
	require_column(at);
	return sqlite3_column_type(m_stmt, at);
}

int Stmt::column_count() const
{
	return sqlite3_column_count(m_stmt);
}

const char* Stmt::column_name(int at) const
{
	if (at < 0)
	{
		auto e = SQLighterException(SQLIGHTER_ERR_INVALID_COLUMN)
			.msg("Got: " + std::to_string(at))
			.query(m_query);
		
		throw e;
	}
	else if (sqlite3_column_count(m_stmt) <= at)
	{
		throw SQLighterException::no_column(at, column_count());
	}
		
	return sqlite3_column_name(m_stmt, at);
}

const std::string& Stmt::column_name_str(int at) const
{
	column_names();
	
	require_column(at);
	
	return m_columns[at];
}

const std::vector<std::string>& Stmt::column_names() const
{
	if (!m_columns.empty())
		return m_columns;
	
	auto cc = sqlite3_column_count(m_stmt);
	
	m_columns.reserve(cc);
	
	for (int i = 0; i < cc; i++)
	{
		std::string name { column_name(i) };
		
		if (m_columnIndex.find(name) == m_columnIndex.end())
		{
			m_columnIndex[name] = i;
		}
		
		m_columns.emplace_back(std::move(name));
	}
	
	return m_columns;
}

void Stmt::require_one_column() const
{
	require_row();
	
	if (column_count() != 1)
	{
		throw SQLighterException(SQLIGHTER_ERR_MULT_COLUMNS);
	}
}

void Stmt::require_done() const
{
	if (!is_done())
	{
		throw SQLighterException(SQLIGHTER_ERR_GENERIC, "Query is not in done state")
			.query(m_query);
	}
}

ScalarValue Stmt::column_value(int at) const
{
	return to_value(at);
}

int Stmt::column_index(std::string_view name) const
{
	column_names();
	
	auto kvp = m_columnIndex.find(std::string { name });
	
	return kvp == m_columnIndex.end() ? -1 : kvp->second;
}

bool Stmt::has_column(std::string_view name) const
{
	column_names();
	
	return m_columnIndex.find(std::string { name }) != m_columnIndex.end();
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
