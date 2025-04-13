#include "sqlighter_exceptions.h"


#include <sstream>

#include "error_message.h"


using namespace sqlighter;


SQLighterException::SQLighterException(int code) noexcept : 
	m_lighter_code(code)
{
	
}

SQLighterException::SQLighterException(int code, int sqlite_code) : 
	m_lighter_code(code),
	m_sqlite_code(sqlite_code)
{
	
}

SQLighterException::SQLighterException(int code, int sqlite_code, std::string_view message) : 
	m_lighter_code(code),
	m_sqlite_code(sqlite_code),
	m_message(message)
{
	
}

SQLighterException::SQLighterException(int code, std::string_view message) : 
	m_lighter_code(code),
	m_message(message)
{
	
}


const std::string& SQLighterException::message() const noexcept
{
	return m_message;
}

const char* SQLighterException::what() const noexcept
{
	return what_str().c_str();
}

const char* SQLighterException::what_sqlite3() const
{
	if (!has_sqlite_code())
		return nullptr;
	
	return sqlite3_errstr(m_sqlite_code);
}

const std::string& SQLighterException::what_str() const
{
	if (m_what.empty())
	{
		std::ostringstream ss {};
		
		ss << error_message(m_lighter_code);
		
		if (m_sqlite_code > 0)
			ss << std::endl << "    > sqlite : " << what_sqlite3();
		
		if (!m_message.empty())
			ss << std::endl << "    > message: " << m_message;
		
		if (!m_query.empty())
			ss << std::endl << "    > query  : " << m_query;
		
		m_what = ss.str();
	}
	
	return m_what;
}

SQLighterException SQLighterException::invalid_enum(const char* enum_name, int value)
{
	return SQLighterException(
		SQLIGHTER_ERR_INVALID_ENUM, 
		std::string(enum_name) + ": " + std::to_string(value));
}

SQLighterException SQLighterException::failed_to_bind(int sqlite_code, int offset, const BindValue& bv)
{
	std::ostringstream err;
	
	err << "At offset " << offset << ", value ";
	bv.to_error_message(err);
	
	return SQLighterException(SQLIGHTER_ERR_BIND, sqlite_code, err.str());
}

SQLighterException SQLighterException::no_column(int at, int count, std::string_view query)
{
	std::ostringstream err;
	
	err << "Requested column at position " 
		<< at 
		<< ", but statement has only " 
		<< count 
		<< " columns";
	
	return SQLighterException(SQLIGHTER_ERR_NO_COLUMN, err.str()).query(query);
}
