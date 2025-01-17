#include "ScalarValue.h"


#include <sstream>

#include "consts/const_cast.h"
#include "exceptions/sqlighter_exceptions.h"


using namespace sqlighter;


template<class T>
using vector = std::vector<T>;


const std::string EMPTY_STR {};
const std::vector<uint8_t> EMPTY_VEC {};


SQLighterException create_error_unexpected_type(ScalarValue::type got, ScalarValue::type expected)
{
	std::ostringstream oss {};
	
	oss << "Unexpected value type. Expecting " << expected << " but got " << got;
	
	return SQLighterException(SQLIGHTER_ERR_VALUE, oss.str());
}


ScalarValue::ScalarValue(ScalarValue::type t) : 
	m_type(t)
{
	
}

ScalarValue::ScalarValue(int32_t val) :
	m_type(type::INT)
{
	m_i64 = (int64_t)val;
}

ScalarValue::ScalarValue(int64_t val) :
	m_type(type::INT)
{
	m_i64 = (int64_t)val;
}

ScalarValue::ScalarValue(double val) :
	m_type(type::DOUBLE)
{
	m_dbl = val;
}

ScalarValue::ScalarValue(std::string_view val) : 
	m_type(type::TEXT)
{
	m_str = val;
}

ScalarValue::ScalarValue(const char* val) :
	m_type(type::TEXT)
{
	m_str = val;
}

ScalarValue::ScalarValue(size_t size, void* blob) : 
	m_type(type::BLOB)
{
	if (blob && size > 0)
	{
		auto p = static_cast<std::uint8_t *>(blob);
		m_blob = { p, p + size };
	}
	else
	{
		m_blob.clear();
	}
}

ScalarValue::ScalarValue(blob_t blob) : 
	m_type(type::BLOB),
	m_blob(std::move(blob))
{
	
}


int32_t ScalarValue::get_int32(int32_t def) const
{
	if (is_null())
		return def;
	
	int32_t val;
	
	if (!try_get_int32(val))
	{
		throw SQLighterException(create_error_unexpected_type(m_type, type::INT));
	}
	
	return val;
}

int64_t ScalarValue::get_int64(int64_t def) const
{
	if (is_null())
		return def;
	
	int64_t val;
	
	if (!try_get_int64(val))
	{
		throw SQLighterException(create_error_unexpected_type(m_type, type::INT));
	}
	
	return val;
}

double ScalarValue::get_double(double def) const
{
	if (is_null())
		return def;
	
	double val;
	
	if (!try_get_double(val))
	{
		throw SQLighterException(create_error_unexpected_type(m_type, type::DOUBLE));
	}
	
	return val;
}

const std::string& ScalarValue::get_str() const
{
	if (is_null())
		return EMPTY_STR;
	
	if (m_type == ScalarValue::type::TEXT)
	{
		return m_str;
	}
	
	throw SQLighterException(create_error_unexpected_type(m_type, type::TEXT));
}

const std::vector<uint8_t>& ScalarValue::get_blob() const
{
	if (is_null())
		return EMPTY_VEC;
	
	if (m_type == ScalarValue::type::BLOB)
	{
		return m_blob;
	}
	
	throw SQLighterException(create_error_unexpected_type(m_type, type::BLOB));
}

bool ScalarValue::try_get_int32(int32_t& v) const noexcept
{
	if (is_null())
		return false;
	
	if (m_type == ScalarValue::type::INT)
	{
		v = (double)m_i64;
	}
	else if (m_type == ScalarValue::type::DOUBLE)
	{
		v = m_dbl;
	}
	else
	{
		return false;
	}
	
	return true;
}

bool ScalarValue::try_get_int64(int64_t& v) const noexcept
{
	if (is_null())
		return false;
	
	if (m_type == ScalarValue::type::INT)
	{
		v = m_i64;
	}
	else if (m_type == ScalarValue::type::DOUBLE)
	{
		v = (int64_t)(m_dbl);
	}
	else
	{
		return false;
	}
	
	return true;
}

bool ScalarValue::try_get_double(double& v) const noexcept
{
	if (is_null())
		return false;
	
	if (m_type == ScalarValue::type::INT)
	{
		v = (double)m_i64;
	}
	else if (m_type == ScalarValue::type::DOUBLE)
	{
		v = m_dbl;
	}
	else
	{
		return false;
	}
	
	return true;
}

bool ScalarValue::try_get_str(std::string& v) const noexcept
{
	if (is_null())
		return false;
	
	if (m_type == ScalarValue::type::TEXT)
	{
		v = m_str;
	}
	else
	{
		return false;
	}
	
	return true;
}

bool ScalarValue::try_get_blob(vector<uint8_t>& v) const noexcept
{
	if (is_null())
		return false;
	
	if (m_type == ScalarValue::type::BLOB)
	{
		v = m_blob;
	}
	else
	{
		return false;
	}
	
	return true;
}


