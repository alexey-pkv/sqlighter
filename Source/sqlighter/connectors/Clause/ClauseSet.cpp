#include "ClauseSet.h"


using namespace sqlighter;


ClauseSet::ClauseSet() : ClauseContainer(", ")
{
	
}


void ClauseSet::set_field(std::string_view column, BindValue value)
{
	clause() << col(column) << " = ?";
	clause().append_bind(value);
}

void ClauseSet::append_to(std::ostream& to) const
{
	to << " SET ";
	clause().append_to(to);
}
