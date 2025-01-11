#pragma once


#include "ICMD.h"
#include "connector_utils.h"
#include "consts/OrderBy.h"


namespace sqlighter
{
	class ICMDSelect : public ICMD
	{
	public:
		~ICMDSelect() override = default;
		
		
	public:
		virtual ICMDSelect& distinct() = 0;
		
		
	public:
		virtual ICMDSelect& column(std::string_view column) = 0;
		virtual ICMDSelect& column_as(std::string_view column, std::string_view as) = 0;
		virtual ICMDSelect& column_as(std::string_view column, char as) = 0;
		virtual ICMDSelect& columns(const std::vector<std::string_view>& columns) = 0;
		virtual ICMDSelect& columns(std::initializer_list<std::string_view> columns) = 0;
		
		
	public:
		virtual ICMDSelect& from(std::string_view table) = 0;
		virtual ICMDSelect& from(std::string_view table, char alias) = 0;
		virtual ICMDSelect& from(std::string_view table, std::string_view alias) = 0;
		
		
	public:
		virtual ICMDSelect& where_null(std::string_view column) = 0;
		virtual ICMDSelect& where_not_null(std::string_view column) = 0;
		virtual ICMDSelect& by_field(std::string_view column, BindValue value) = 0;
		
		
	public:
		virtual ICMDSelect& group_by_field(std::string_view by) = 0;
		
		
	public:
		inline ICMDSelect& order_by_field_asc(std::string_view by)	{ return order_by_field(by, OrderBy::ASC); };
		inline ICMDSelect& order_by_field_desc(std::string_view by)	{ return order_by_field(by, OrderBy::DESC); };
		
		virtual ICMDSelect& order_by_field(std::string_view by) = 0;
		virtual ICMDSelect& order_by_field(std::string_view by, OrderBy order) = 0;
		
		
	public:
		inline ICMDSelect& page(int page, int page_size) { return limit(page * page_size, page_size); };
		
		virtual ICMDSelect& limit_by(int count) = 0;
		virtual ICMDSelect& limit(int offset, int count) = 0;
		
		
	public:
		SQLIGHTER_EXPRESSION_CLAUSE(column_exp,	append_column_exp,	ICMDSelect);
		SQLIGHTER_EXPRESSION_CLAUSE(where,		append_where,	 	ICMDSelect);
		SQLIGHTER_EXPRESSION_CLAUSE(group_by,	append_group_by,	ICMDSelect);
		SQLIGHTER_EXPRESSION_CLAUSE(having,		append_having,		ICMDSelect);
		SQLIGHTER_EXPRESSION_CLAUSE(order_by,	append_order_by,	ICMDSelect);
	};
}
