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


ScalarValue::value::~value() {}
ScalarValue::value::value() {}

void ScalarValue::value::make_string() { new (&str) std::string(); }
void ScalarValue::value::make_string(std::string&& s) { new (&str) std::string(std::move(s)); }
void ScalarValue::value::make_string(const std::string& s) { new (&str) std::string(s); }

void ScalarValue::value::make_blob() { new (&blob)  blob_t(); }
void ScalarValue::value::make_blob(blob_t&& b) { new (&blob) blob_t(std::move(b)); }
void ScalarValue::value::make_blob(const blob_t& b) { new (&blob) blob_t(std::move(b)); }


void ScalarValue::value::destroy_string()
{
	typedef std::string string;
	str.~string();
}

void ScalarValue::value::destroy_blob()
{
	blob.~vector<std::uint8_t>();
}


ScalarValue::~ScalarValue()
{
	switch (m_type)
	{
		case ScalarValue::type::BLOB:
			m_value.destroy_blob();
			break;
			
		case ScalarValue::type::TEXT:
			m_value.destroy_string();
			break;
			
		case type::INT:
		case type::DOUBLE:
		case type::NULL_VAL:
			break;
	}
}


ScalarValue::ScalarValue() : 
	m_type(type::NULL_VAL)
{
	m_value.i64 = 0;
}

ScalarValue::ScalarValue(ScalarValue&& v)
{
	*this = std::move(v);
}

ScalarValue& ScalarValue::operator=(ScalarValue&& v)
{
	switch (m_type)
	{
		case ScalarValue::type::BLOB:
			m_value.destroy_blob();
			break;
			
		case ScalarValue::type::TEXT:
			m_value.destroy_string();
			break;
			
		case type::INT:
		case type::NULL_VAL:
		case type::DOUBLE:
			break;
	}
	
	m_type = v.m_type;
	
	switch (m_type)
	{
		case ScalarValue::type::BLOB:
			m_value.make_blob(v.m_value.blob);
			break;
			
		case ScalarValue::type::TEXT:
			m_value.make_string(v.m_value.str);
			break;
			
		case type::NULL_VAL:
		case type::INT:
			m_value.i64 = v.m_value.i64;
			break;
			
		case type::DOUBLE:
			m_value.dbl = v.m_value.dbl;
			break;
	}
	
	v.m_type = type::NULL_VAL;
	
	return *this;
}

ScalarValue::ScalarValue(ScalarValue::type t) : 
	m_type(t),
	m_value()
{
	switch (m_type)
	{
		case ScalarValue::type::BLOB:
			m_value.make_blob();
			break;
			
		case ScalarValue::type::TEXT:
			m_value.make_string();
			break;
			
		case type::NULL_VAL:
		case type::INT:
			m_value.i64 = 0;
			break;
			
		case type::DOUBLE:
			m_value.dbl = 0.0;
			break;
	}
}

ScalarValue::ScalarValue(int32_t val) :
	m_type(type::INT)
{
	m_value.i64 = (int64_t)val;
}

ScalarValue::ScalarValue(int64_t val) :
	m_type(type::INT)
{
	m_value.i64 = val;
}

ScalarValue::ScalarValue(double val) :
	m_type(type::DOUBLE)
{
	m_value.dbl = val;
}

ScalarValue::ScalarValue(std::string_view val) : 
	m_type(type::TEXT)
{
	m_value.make_string(std::string { val });
}

ScalarValue::ScalarValue(const char* val) :
	m_type(type::TEXT)
{
	m_value.make_string(std::string { val });
}

ScalarValue::ScalarValue(size_t size, void* blob) : 
	m_type(type::BLOB)
{
	if (blob && size > 0)
	{
		auto p = static_cast<std::uint8_t *>(blob);
		blob_t vec(p, p + size);
		m_value.make_blob(std::move(vec));
	}
	else
	{
		m_value.make_blob({});
	}
}

ScalarValue::ScalarValue(blob_t blob) : 
	m_type(type::BLOB)
{
	m_value.make_blob(std::move(blob));
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
		return m_value.str;
	}
	
	throw SQLighterException(create_error_unexpected_type(m_type, type::TEXT));
}

const std::vector<uint8_t>& ScalarValue::get_blob() const
{
	if (is_null())
		return EMPTY_VEC;
	
	if (m_type == ScalarValue::type::BLOB)
	{
		return m_value.blob;
	}
	
	throw SQLighterException(create_error_unexpected_type(m_type, type::BLOB));
}

bool ScalarValue::try_get_int32(int32_t& v) const noexcept
{
	if (is_null())
		return false;
	
	if (m_type == ScalarValue::type::INT)
	{
		v = (double)m_value.i64;
	}
	else if (m_type == ScalarValue::type::DOUBLE)
	{
		v = m_value.dbl;
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
		v = m_value.i64;
	}
	else if (m_type == ScalarValue::type::DOUBLE)
	{
		v = (int64_t)(m_value.dbl);
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
		v = (double)m_value.i64;
	}
	else if (m_type == ScalarValue::type::DOUBLE)
	{
		v = m_value.dbl;
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
		v = m_value.str;
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
		v = m_value.blob;
	}
	else
	{
		return false;
	}
	
	return true;
}


