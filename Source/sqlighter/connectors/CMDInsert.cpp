#include "CMDInsert.h"


#include "query_utils.h"
#include "exceptions/sqlighter_exceptions.h"

#include <sstream>


using namespace sqlighter;


CMDInsert::CMDInsert(const std::shared_ptr<IConnection>& connection) : CMD(connection) {}


CMDInsert& CMDInsert::or_abort()	{ m_or = "ABORT"; return *this; }
CMDInsert& CMDInsert::or_fail()		{ m_or = "FAIL"; return *this; }
CMDInsert& CMDInsert::or_ignore()	{ m_or = "IGNORE"; return *this; }
CMDInsert& CMDInsert::or_replace()	{ m_or = "REPLACE"; return *this; }
CMDInsert& CMDInsert::or_rollback()	{ m_or = "ROLLBACK"; return *this; }


CMDInsert& CMDInsert::into(std::string_view table)
{
	element_name(table, m_scheme, m_table);
	return *this;
}

CMDInsert& CMDInsert::into(std::string_view scheme, std::string_view table)
{
	m_scheme = scheme;
	m_table = table;
	return *this;
}

CMDInsert& CMDInsert::as(std::string_view alias)
{
	m_as = alias;
	return *this;
}

CMDInsert& CMDInsert::column(std::string_view name)
{
	m_columns.emplace_back(name);
	return *this;
}

CMDInsert& CMDInsert::columns(const std::vector<std::string>& names)
{
	if (m_columns.empty())
	{
		m_columns = names;
	}
	else if (!names.empty())
	{
		m_columns.reserve(m_columns.size() + names.size());
		m_columns.insert(m_columns.end(), names.begin(), names.end());
	}
	
	return *this;
}

CMDInsert& CMDInsert::record(const std::vector<BindValue>& values)
{
	m_columnsCount = values.size();
	
	m_binds.insert(m_binds.end(), values.begin(), values.end());
	return *this;
}

CMDInsert& CMDInsert::record(std::initializer_list<BindValue> values)
{
	m_columnsCount = values.size();
	
	m_binds.insert(m_binds.end(), values.begin(), values.end());
	return *this;
}

CMDInsert& CMDInsert::records(const std::vector<std::vector<BindValue>>& values)
{
	for (const auto& v : values)
	{
		record(v);
	}
	
	return *this;
}

CMDInsert& CMDInsert::records(std::initializer_list<const std::vector<BindValue>> values)
{
	for (const auto& v : values)
	{
		record(v);
	}
	
	return *this;
}

CMDInsert& CMDInsert::records(std::initializer_list<std::initializer_list<BindValue>> values)
{
	for (const auto& v : values)
	{
		record(v);
	}
	
	return *this;
}


CMDInsert& CMDInsert::default_values()
{
	m_isDefaultValues = true;
	return *this;
}


void CMDInsert::assemble(std::ostringstream& ss) const
{
	ss << "INSERT ";
	
	if (!m_or.empty())
		ss << "OR " << m_or << ' ';
	
	ss << "INTO ";
	
	if (!m_scheme.empty())
		wrap_element(ss, m_scheme) << '.';
	
	wrap_element(ss, m_table) << ' ';
	
	if (!m_as.empty())
		ss << "AS " << wrap_element(m_as) << ' ';
	
	if (!m_columns.empty())
	{
		ss << '(';
		
		for (int i = 0; i < m_columns.size(); i++)
		{
			if (i > 0) ss << ", ";
			
			wrap_element(ss, m_columns[i]);
		}
		
		ss << ") ";
	}
	
	if (m_isDefaultValues)
	{
		if (!m_binds.empty())
			throw SQLighterException(SQLIGHTER_ERR_GENERIC, "DEFAULT VALUES can not be used with bind values");
		
		ss << "DEFAULT VALUES";
		return;
	}
	
	ss << "VALUES " << std::endl;
	
	int count = std::max((int)m_columns.size(), m_columnsCount);
	
	if (count <= 0)
	{
		throw SQLighterException(
			SQLIGHTER_ERR_GENERIC, 
			"Could not determine number of columns for the insert statement");
	}
	
	auto questions = create_questions(count);
	int records = m_binds.size() / count;
	
	for (int i = 0; i < records; i++)
	{
		if (i > 0) ss << ", " << std::endl;
		
		ss << '(' << questions << ')';
	}
}

std::vector<BindValue> CMDInsert::bind() const
{
	return m_binds;
}

std::string CMDInsert::assemble() const
{
	return CMD::assemble();
}
