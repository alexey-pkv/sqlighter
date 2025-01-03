#include "Clause.h"

#include "connectors/query_utils.h"


using namespace sqlighter;


Clause::Clause(const char* delimiter) : 
	m_delimiter(delimiter)
{
	
}

Clause::Clause(const Clause& c) : 
	m_delimiter(c.m_delimiter),
	m_isEmpty(c.m_isEmpty),
	m_binds(c.m_binds)
{
	m_stream.clear();
	
	m_stream << c.m_stream.rdbuf();
}

Clause& Clause::operator=(const Clause& c)
{
	if (&c == this)
		return *this;
	
	m_delimiter	= c.m_delimiter;
	m_isEmpty	= c.m_isEmpty;
	m_binds		= c.m_binds;
	
	m_stream.clear();
	
	m_stream << c.m_stream.rdbuf();
	
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
	m_stream << str;
	
	if (m_delimiter != nullptr && m_isEmpty)
	{
		m_isEmpty = false;
	}
}

void Clause::append(const BindValue& bind)
{
	m_binds.emplace_back(bind);
}

void Clause::append(std::span<const BindValue> binds)
{
	m_binds.insert(m_binds.end(), binds.begin(), binds.end());
}

void Clause::append(std::string_view str, std::span<const BindValue> binds)
{
	append(str);
	append(binds);
}

void Clause::append_directly(std::string_view str, std::span<const BindValue> binds)
{
	append_directly(str);
	append(binds);
}

int Clause::bind(sqlite3_stmt* to, int offset) const
{
	for (const auto& b : m_binds)
	{
		b.bind(to, offset++);
	}
	
	return offset;
}

void Clause::append_to(std::stringstream& to) const
{
	to << m_stream.str();
}

void Clause::append_to(std::vector<BindValue>& to) const
{
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
