#include "ClauseTable.h"


#include <sstream>

#include "connectors/query_utils.h"


using namespace sqlighter;


void ClauseTable::as(std::string_view alias)
{
	m_as = alias;
}

void ClauseTable::table(std::string_view table, std::string_view as)
{
	m_as = as;
	element_name(table, m_scheme, m_table);
}

void ClauseTable::table_scheme(std::string_view scheme, std::string_view table, std::string_view as)
{
	m_scheme = scheme;
	m_table = table;
	m_as = as;
}

void ClauseTable::append_to(std::ostream& to) const
{
	if (has_scheme())
	{
		to << m_scheme << ".";
	}
	
	if (has_table())
	{
		to << m_table;
	}
	
	if (has_alias())
	{
		to << " AS " << m_as;
	}
}


std::ostream& sqlighter::operator<<(std::ostream& ss, const ClauseTable& ct)
{
	ct.append_to(ss);
	return ss;
}
