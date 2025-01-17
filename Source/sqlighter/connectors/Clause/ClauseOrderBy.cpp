#include "ClauseOrderBy.h"


#include "consts/const_cast.h"


using namespace sqlighter;


ClauseOrderBy::ClauseOrderBy() :
	ClauseContainer(", ")
{
	
}


void ClauseOrderBy::order_by_field(std::string_view by, OrderBy order)
{
	clause() << col(by);
	
	if (order == OrderBy::DESC)
	{
		clause().direct() << ' ' << order;
	}
}

void ClauseOrderBy::append_to(std::ostream& to) const
{
	if (clause().empty_clause())
		return;
	
	to << " ORDER BY ";
	clause().append_to(to);
}
