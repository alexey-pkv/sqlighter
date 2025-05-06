#include "connection_override.h"


#include <vector>

#include "core/Stmt.h"
#include "core/BindValue.h"


using namespace sqlighter;


Stmt connection_override::execute(std::string_view query, const std::vector<BindValue>& values)
{
	last_query = query;
	last_bind = values;
	
	return Stmt(nullptr);
}

StmtRef connection_override::prepare(std::string_view query)
{
	last_query = query;
	last_bind = {};
	
	return StmtRef();
}
