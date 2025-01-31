#include "ClauseTable.h"


#include "connectors/query_utils.h"

#include <sstream>


using namespace sqlighter;


void ClauseTable::as(std::string_view alias)
{
	m_as = alias;
}

void ClauseTable::table(std::string_view table)
{
	element_name(table, m_scheme, m_table);
}

void ClauseTable::table(std::string_view scheme, std::string_view table)
{
	m_scheme = scheme;
	m_table = table;
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
