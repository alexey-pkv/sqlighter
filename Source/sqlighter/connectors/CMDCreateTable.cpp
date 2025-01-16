#include "CMDCreateTable.h"


#include <sstream>

#include "exceptions/sqlighter_exceptions.h"


using namespace sqlighter;


CMDCreateTable::CMDCreateTable(const std::shared_ptr<IConnection>& connection) : 
	m_connection(connection)
{
	if (connection == nullptr)
	{
		throw SQLighterException(SQLIGHTER_ERR_UNEXPECTED, "connection should not be null!");
	}
}


CMDCreateTable& CMDCreateTable::temp()
{
	m_isTemp = true;
	return *this;
}

CMDCreateTable& CMDCreateTable::if_not_exists()
{
	m_ifNotExists = true;
	return *this;
}

CMDCreateTable& CMDCreateTable::scheme(std::string_view name)
{
	m_scheme = name;
	return *this;
}

CMDCreateTable& CMDCreateTable::table(std::string_view name)
{
	auto at = name.find('.');
	
	if (at != std::string::npos)
	{
		m_scheme = name.substr(0, at);
		m_table = name.substr(at + 1);
	}
	else
	{
		m_table = name;
	}
	
	return *this;
}

CMDCreateTable& CMDCreateTable::table(std::string_view scheme, std::string_view table)
{
	m_scheme = scheme;
	m_table = table;
	
	return *this;
}

CMDCreateTable& CMDCreateTable::column_exp(std::string_view exp)
{
	m_columns.emplace_back(exp);
	return *this;
}

CMDCreateTable& CMDCreateTable::without_rowid()
{
	m_withoutRowid = true;
	return *this;
}

CMDCreateTable& CMDCreateTable::strict()
{
	m_strict = true;
	return *this;
}

void CMDCreateTable::assemble(std::ostringstream& ss) const
{
	ss << "CREATE ";
	
	if (m_isTemp)
		ss << "TEMP ";
	
	ss << "TABLE ";
	
	if (m_ifNotExists)
		ss << "IF NOT EXISTS ";
	
	if (!m_scheme.empty())
		ss << m_scheme << ".";
	
	ss << m_table << " (" << std::endl;
	
	for (int i = 0; i < m_columns.size(); i++)
	{
		if (i > 0)
			ss << ',' << std::endl;
		
		ss << "  " << m_columns[i]; 
	}
	
	ss << std::endl << ")";
	
	if (m_withoutRowid && m_strict)
	{
		ss << " WITHOUT ROWID, STRICT";
	}
	else if (m_withoutRowid)
	{
		ss << " WITHOUT ROWID";
	}
	else if (m_strict)
	{
		ss << " STRICT";
	}
}

std::string CMDCreateTable::assemble() const
{
	std::ostringstream ss;
	
	assemble(ss);
	
	return ss.str();
}

std::vector<BindValue> CMDCreateTable::bind() const
{
	return {};
}

Stmt CMDCreateTable::execute() const
{ 
	return m_connection->execute(assemble(), {});
}