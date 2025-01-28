#include "CMDDelete.h"


#include <sstream>


using namespace sqlighter;


CMDDelete::CMDDelete(const std::shared_ptr<IConnection>& connection) : CMD(connection)
{
	
}


CMDDelete& CMDDelete::as(std::string_view alias)
{
	m_from.as(alias);
	return *this;
}

CMDDelete& CMDDelete::from(std::string_view table)
{
	m_from.table(table);
	return *this;
}

CMDDelete& CMDDelete::from(std::string_view scheme, std::string_view table)
{
	m_from.table(scheme, table);
	return *this;
}


void CMDDelete::assemble(std::ostringstream& ss) const
{
	ss << "DELETE FROM " 
		<< m_from;
	
	if (!where_clause().is_empty_clause())
		ss << " WHERE " << where_clause();
	
	ss 
		<< m_orderBy
		<< m_limit;
}

std::vector<BindValue> CMDDelete::bind() const
{
	std::vector<BindValue> final = {};
	
	auto total = 
		where_clause().binds_size() +  
		m_orderBy.binds_size();
	
	if (total == 0)
		return {};
	
	final.reserve(total);
	
	where_clause().append_to(final);
	m_orderBy.append_binds(final);
	
	return final;
}

std::string CMDDelete::assemble() const
{
	return CMD::assemble();
}
