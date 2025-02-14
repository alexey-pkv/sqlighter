#include "BindValue.h"


#include "exceptions/sqlighter_exceptions.h"

#include <sqlite3.h>
#include <sstream>


using namespace sqlighter;


const BindValue BindValue::null { BindValue::type::NULL_VAL };


BindValue::BindValue() : 
	m_type(type::NULL_VAL),
	m_value({})
{
	
}

BindValue::BindValue(BindValue::type t) : 
	m_type(t),
	m_value({})
{
	
}

BindValue::BindValue(int32_t val) : 
	m_type(type::INT_32),
	m_value({ .i32 = val })
{
	
}

BindValue::BindValue(int64_t val) : 
	m_type(type::INT_64),
	m_value({ .i64 = val })
{
	
}

BindValue::BindValue(double val) : 
	m_type(type::DOUBLE),
	m_value({ .dbl = val })
{
	
}

BindValue::BindValue(std::string_view val) : 
	m_type(type::TEXT),
	m_value({}),
	m_strValue(val)
{
	
}

BindValue::BindValue(const char* val) : 
	m_type(type::TEXT),
	m_value({}),
	m_strValue(val)
{
	
}


void BindValue::bind(sqlite3_stmt* stmt, int offset) const
{
	int res = SQLITE_OK;
	
	switch (m_type)
	{
		case type::INT_32:
			res = sqlite3_bind_int(stmt, offset, m_value.i32);
			break;
			
		case type::INT_64:
			res = sqlite3_bind_int64(stmt, offset, m_value.i64);
			break;
			
		case type::DOUBLE:
			res = sqlite3_bind_double(stmt, offset, m_value.dbl);
			break;
		
		case type::NULL_VAL:
			res = sqlite3_bind_null(stmt, offset);
			break;
		
		case type::TEXT:
			res = sqlite3_bind_text(stmt, offset, m_strValue.c_str(), -1, SQLITE_TRANSIENT);
			break;
			
		case type::TEXT_16:
		case type::TEXT_64:
		default:
			throw SQLighterException(SQLIGHTER_ERR_BIND, "TEXT_64 bind is not implemented");
	}
	
	if (res != SQLITE_OK)
	{
		throw SQLighterException::failed_to_bind(res, offset, *this);
	}
}

void BindValue::to_error_message(std::ostream& stream) const
{
	switch (m_type)
	{
		case type::INT_32:
			stream << "int32:" << m_value.i32;
			break;
			
		case type::INT_64:
			stream << "int64:" << m_value.i64;
			break;
			
		case type::DOUBLE:
			stream << "double:" << m_value.dbl;
			break;
		
		case type::NULL_VAL:
			stream << "null";
			break;
		
		case type::TEXT:
			stream << "string[" << m_strValue.length() << "]:";
			
			if (m_strValue.length() > 32)
			{
				stream << m_strValue.substr(0, 32) << "...";
			}
			else
			{
				stream << m_strValue;
			}
			
			break;
			
		case type::TEXT_16:
		case type::TEXT_64:
		default:
			throw SQLighterException(SQLIGHTER_ERR_BIND, "TEXT_16, TEXT_64 binds are not implemented");
	}
}
