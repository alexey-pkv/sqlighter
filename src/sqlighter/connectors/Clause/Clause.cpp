#include "Clause.h"

#include "connectors/query_utils.h"


using namespace sqlighter;


void reset_stream(const std::ostringstream& source, std::ostringstream& target)
{
	target.str("");
	target.clear();
	
	target << source.str();
}


Clause::Clause(const char* delimiter) : 
	m_delimiter(delimiter)
{
	
}

Clause::Clause(const Clause& c)
{
	*this = c;
}

Clause& Clause::operator=(const Clause& c)
{
	if (&c == this)
		return *this;
	
	m_delimiter	= c.m_delimiter;
	m_isEmpty	= c.m_isEmpty;
	m_binds		= c.m_binds;
	
	reset_stream(c.m_stream, m_stream);
	
	return *this;
}


void Clause::append(std::string_view str)
{
	if (str.empty())
		return;
	
	if (m_delimiter != nullptr)
	{
		if (m_isEmpty)
		{
			m_isEmpty = false;
		}
		else
		{
			m_stream << m_delimiter;
		}
	}
	
	m_stream << str;
}

void Clause::append_directly(std::string_view str)
{
	if (str.empty())
		return;
	
	m_stream << str;
	
	if (m_delimiter != nullptr && m_isEmpty)
	{
		m_isEmpty = false;
	}
}

void Clause::append_bind(const BindValue& bind)
{
	m_binds.emplace_back(bind);
}

void Clause::append_binds(const std::vector<BindValue>& binds)
{
	m_binds.insert(m_binds.end(), binds.begin(), binds.end());
}

void Clause::append_binds(std::initializer_list<BindValue> binds)
{
	m_binds.insert(m_binds.end(), binds.begin(), binds.end());
}

void Clause::append(std::string_view str, const std::vector<BindValue>& binds)
{
	append(str);
	append_binds(binds);
}

void Clause::append_directly(std::string_view str, std::initializer_list<BindValue> binds)
{
	append_directly(str);
	append_binds(binds);
}

void Clause::append_directly(std::string_view str, const std::vector<BindValue>& binds)
{
	append_directly(str);
	append_binds(binds);
}

void Clause::append_to(std::ostream& to) const
{
	to << m_stream.str();
}

void Clause::append_to(std::vector<BindValue>& to) const
{
	if (m_binds.empty())
		return;
	
	to.insert(to.end(), m_binds.begin(), m_binds.end());
}

void Clause::append_delimiter()
{
	if (m_delimiter == nullptr)
		return;
	
	if (m_isEmpty)
	{
		m_isEmpty = false;
	}
	else
	{
		m_stream << m_delimiter;
	}
}

void Clause::clear()
{
	m_isEmpty = true;
	
	m_stream.str("");
	m_stream.clear();
	
	m_binds.clear();
}
