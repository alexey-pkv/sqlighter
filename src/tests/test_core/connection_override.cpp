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

Stmt connection_override::prepare(std::string_view query)
{
	last_query = query;
	last_bind = {};
	
	return Stmt(nullptr);
}

void connection_override::execute_prepared(Stmt& stmt, const std::vector<BindValue>& values)
{
	last_query = stmt.query();
	last_bind = values;
}
