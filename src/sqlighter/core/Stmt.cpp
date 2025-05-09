#include "Stmt.h"


#include "exceptions/sqlighter_exceptions.h"

#include <cstring>


using namespace sqlighter;


Stmt::Stmt(sqlite3_stmt* stmt) :
	m_stmt(stmt)
{
	
}

Stmt::Stmt(sqlite3_stmt* stmt, std::shared_ptr<DB> db) : 
	m_stmt(stmt),
	m_db(db ? std::move(db) : nullptr)
{
	
}

Stmt::Stmt(sqlite3_stmt* stmt, std::shared_ptr<DB> db, std::string_view query) : 
	m_stmt(stmt),
	m_db(db ? std::move(db) : nullptr),
	m_query(query)
{
	
}

Stmt::Stmt(std::shared_ptr<DB> db, std::string_view query) : 
	m_stmt(),
	m_db(db ? std::move(db) : nullptr),
	m_query(query)
{
	
}

Stmt::Stmt(StmtRef&& ref, std::shared_ptr<DB> db, std::string_view query) :
	m_stmt(std::move(ref)),
	m_db(db ? std::move(db) : nullptr),
	m_query(query)
{
	
}


Stmt::Stmt(Stmt&& s) noexcept
{
	*this = std::move(s);
}

Stmt& Stmt::operator=(Stmt&& s) noexcept
{
	m_stmt			= std::move(s.m_stmt);
	m_columns		= std::move(s.m_columns);
	m_columnIndex	= std::move(s.m_columnIndex);
	m_query			= std::move(s.m_query);
	m_db			= std::move(s.m_db);
	
	m_lastCode = s.m_lastCode;
	
	return *this;
}


sqlite3_stmt* Stmt::stmt()
{
	if (!m_stmt)
		throw SQLighterException(SQLIGHTER_ERR_STMT_FINALIZED);
	
	return m_stmt.get();
}

const sqlite3_stmt* Stmt::stmt() const
{
	auto stmt = m_stmt.get();
	
	if (!stmt)
		throw SQLighterException(SQLIGHTER_ERR_STMT_FINALIZED);
	
	return stmt;
}

ScalarValue Stmt::to_value(int at) const
{
	switch (column_type(at))
	{
		case SQLITE_NULL:
			return ScalarValue::null();
		
		case SQLITE_INTEGER:
			return { column_int64(at) };
			
		case SQLITE_FLOAT:
			return { column_double(at) };
			
		case SQLITE_BLOB:
			return { column_blob(at) };
			
		case SQLITE_TEXT:
			return ScalarValue { column_string(at) };
			
		// Seems like this case is not currently possible unless we somehow override the column_type
		// method somehow, but it's a bit of an overkill just to test this line.
		// LCOV_EXCL_START
		default:																			
			throw SQLighterException(SQLIGHTER_ERR_UNEXPECTED)						
				.msg("Got unexpected column type: " + std::to_string(column_type(at)))
				.query(m_query);
		// LCOV_EXCL_STOP
	}
}

int Stmt::step()
{
	if (!m_stmt)
		throw SQLighterException(SQLIGHTER_ERR_STMT_FINALIZED, "When calling step()").query(m_query);
	
	m_lastCode = sqlite3_step(m_stmt.get());
	
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


void Stmt::close()
{
	m_stmt.clear();
}

void Stmt::require_open() const
{
	if (!m_stmt)
	{
		throw SQLighterException(SQLIGHTER_ERR_STMT_FINALIZED);
	}
}

void Stmt::require_row() const
{
	require_open();
	
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
		throw SQLighterException(SQLIGHTER_ERR_INVALID_COLUMN, "Got: " + std::to_string(at)).query(m_query);
	}
	else if (column_count() <= at)
	{
		throw SQLighterException::no_column(at, column_count(), m_query);
	}
}

void Stmt::require_column_type(int at, int type) const
{
	if (type != column_type(at))
	{
		auto e = SQLighterException(SQLIGHTER_ERR_VALUE)
			.msg(
				"Expected column of type " + std::to_string(type) +
				" but got " + std::to_string(column_type(at)))
			.query(m_query);
		
		throw e;
	}
}


int Stmt::column_int(int at) const
{
	require_column(at);
	return sqlite3_column_int(m_stmt.get_cc(), at);
}

int64_t Stmt::column_int64(int at) const
{
	require_column(at);
	return sqlite3_column_int64(m_stmt.get_cc(), at);
}

bool Stmt::column_bool(int at) const
{
	require_column(at);
	return sqlite3_column_int(m_stmt.get_cc(), at) != 0;
}

double Stmt::column_double(int at) const
{
	require_column(at);
	return sqlite3_column_double(m_stmt.get_cc(), at);
}

std::string Stmt::column_string(int at) const
{
	require_column(at);
	return reinterpret_cast<const char*>(sqlite3_column_text(m_stmt.get_cc(), at));
}

size_t Stmt::column_blob(int at, void** into) const
{
	require_column(at);
	
	*into = nullptr;
	
	const void* data = sqlite3_column_blob(m_stmt.get_cc(), at);
	
	if (!data)
	{
		return 0;
	}
	
	auto size = sqlite3_column_bytes(m_stmt.get_cc(), at);
	
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
		
	auto p = m_stmt.get_cc();
	
	const void* data = sqlite3_column_blob(p, at);
	auto size = sqlite3_column_bytes(p, at);
	
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

std::vector<ScalarValue> Stmt::row() const
{
	require_row();
	
	auto c = column_count();
	std::vector<ScalarValue> res {};
	
	res.reserve(c);
	
	for (int i = 0; i < c; i++)
	{
		res.emplace_back(column_value(i));
	}
	
	return res;
} // LCOV_EXCL_LINE

std::vector<ScalarValue> Stmt::row_and_step()
{
	auto item = row();
	
	step();
	
	return item;
} // LCOV_EXCL_LINE

std::vector<std::vector<ScalarValue>> Stmt::all(int failsafeLimit)
{
	if (!has_row())
		return {};
	
	std::vector<std::vector<ScalarValue>> data = {};
	
	while (has_row())
	{
		if (data.size() >= failsafeLimit)
			throw SQLighterException(SQLIGHTER_ERR_ROWS_OVERFLOW).query(m_query);
		
		data.emplace_back(row_and_step());
	}
	
	return data;
}

int Stmt::column_type(int at) const
{
	require_column(at);
	return sqlite3_column_type(m_stmt.get_cc(), at);
}

int Stmt::column_count() const
{
	require_open();
	return sqlite3_column_count(m_stmt.get_cc());
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
	else if (sqlite3_column_count(m_stmt.get_cc()) <= at)
	{
		throw SQLighterException::no_column(at, column_count(), m_query);
	}
		
	return sqlite3_column_name(m_stmt.get_cc(), at);
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
	
	auto cc = column_count();
	
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
	if (column_count() != 1)
	{
		throw SQLighterException(SQLIGHTER_ERR_MULT_COLUMNS).query(m_query);
	}
}

void Stmt::require_done() const
{
	require_open();
	
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

ScalarValue Stmt::column_value_and_step(int at)
{
	auto val = to_value(at);
	
	step();
	
	return val;
} // LCOV_EXCL_LINE

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
	return (float)column_double(at);
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
