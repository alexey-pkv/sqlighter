#include "CMDDelete.h"


#include <sstream>


using namespace sqlighter;


CMDDelete::CMDDelete(const std::shared_ptr<IConnection>& connection) : CMD(connection)
{
	
}


void CMDDelete::assemble(std::ostringstream& ss) const
{
	ss << "DELETE";
	
	append_from(ss);
	
	if (!where_clause().is_empty_clause())
		ss << " WHERE " << where_clause();
	
	append_order_by(ss);
	append_limit(ss);
}

std::vector<BindValue> CMDDelete::bind() const
{
	std::vector<BindValue> final = {};
	
	auto total = 
		where_clause().binds_size() +  
		order_by_clause().binds_size();
	
	if (total == 0)
		return {};
	
	final.reserve(total);
	
	where_clause().append_to(final);
	order_by_clause().append_to(final);
	
	return final;
}

std::string CMDDelete::assemble() const
{
	return CMD::assemble();
}
