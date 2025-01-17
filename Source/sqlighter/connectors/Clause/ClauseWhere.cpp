#include "ClauseWhere.h"


using namespace sqlighter;


ClauseWhere::ClauseWhere() :
	ClauseContainer(" AND ")
{
	
}


void ClauseWhere::where_null(std::string_view column)
{
	clause() << col(column) << " IS NULL";
}

void ClauseWhere::where_not_null(std::string_view column)
{
	clause() << col(column) << " IS NOT NULL";
}

void ClauseWhere::by_field(std::string_view column, BindValue value)
{
	clause() << col(column) << " = ?";
	clause().append_bind(value);
}