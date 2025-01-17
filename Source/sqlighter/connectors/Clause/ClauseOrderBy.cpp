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
