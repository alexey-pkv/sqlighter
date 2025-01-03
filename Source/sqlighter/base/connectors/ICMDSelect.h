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
		virtual void distinct() = 0;
		
		
	public:
		virtual void column(std::string_view column) = 0;
		virtual void column_as(std::string_view column, std::string_view as) = 0;
		virtual void column_as(std::string_view column, char as) = 0;
		virtual void columns(const std::vector<std::string_view>& columns) = 0;
		virtual void columns(std::initializer_list<std::string_view> columns) = 0;
		
		
	public:
		virtual void from(std::string_view table) = 0;
		virtual void from(std::string_view table, char alias) = 0;
		virtual void from(std::string_view table, std::string_view alias) = 0;
		
		
	public:
		virtual void where_null(std::string_view column) = 0;
		virtual void where_not_null(std::string_view column) = 0;
		virtual void by_field(std::string_view column, BindValue value) = 0;
		
		
	public:
		virtual void group_by_field(std::string_view by) = 0;
		
		
	public:
		inline void order_by_field_asc(std::string_view by)		{ order_by_field(by, OrderBy::ASC); };
		inline void order_by_field_desc(std::string_view by)	{ order_by_field(by, OrderBy::DESC); };
		
		virtual void order_by_field(std::string_view by) = 0;
		virtual void order_by_field(std::string_view by, OrderBy order) = 0;
		
		
	public:
		inline void page(int page, int page_size) { limit(page * page_size, page_size); };
		
		virtual void limit_by(int count) = 0;
		virtual void limit(int offset, int count) = 0;
		
		
	public:
		SQLIGHTER_EXPRESSION_CLAUSE(column_exp,	append_column_exp);
		SQLIGHTER_EXPRESSION_CLAUSE(where,		append_where);
		SQLIGHTER_EXPRESSION_CLAUSE(group_by,	append_group_by);
		SQLIGHTER_EXPRESSION_CLAUSE(having,		append_having);
		SQLIGHTER_EXPRESSION_CLAUSE(order_by,	append_order_by);
	};
}
