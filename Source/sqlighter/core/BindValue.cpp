#include "BindValue.h"


#include <utility>
#include <sqlite3.h>


using namespace sqlighter;


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
	switch (m_type)
	{
		case type::INT_32:
			sqlite3_bind_int(stmt, offset, m_value.i32);
			break;
			
		case type::INT_64:
			sqlite3_bind_int64(stmt, offset, m_value.i64);
			break;
			
		case type::DOUBLE:
			sqlite3_bind_double(stmt, offset, m_value.dbl);
			break;
		
		case type::NULL_VAL:
			sqlite3_bind_null(stmt, offset);
			break;
		
		case type::TEXT:
			sqlite3_bind_text(stmt, offset, m_strValue.c_str(), -1, SQLITE_TRANSIENT);
			break;
			
		case type::TEXT_16:
		case type::TEXT_64:
			break;
	}
}