#pragma once


#include <sstream>
#include <memory>

#include "base/connectors/ICMD.h"
#include "consts/OrderBy.h"
#include "connectors/Clause.h"


namespace sqlighter
{
	class IConnection;
	
	
	class CMDSelect : public ICMD
	{
	private:
		std::shared_ptr<IConnection>	m_connection;
		
		
	private:
		bool m_distinct	= false;
		bool m_hasLimit	= false;
		
		int m_limitOffset = 0;
		int m_limitCount = 0;
		
		std::string m_from = {};
		
		Clause m_columns	{ ", " };
		Clause m_where		{ " AND " };
		Clause m_groupBy	{ ", " };
		Clause m_having		{ " AND " };
		Clause m_orderBy	{ ", " };
		
		
	public:
		CMDSelect(CMDSelect&&) noexcept = default;
		CMDSelect(const CMDSelect&) = default;
		CMDSelect& operator=(const CMDSelect&) = default;
		CMDSelect& operator=(CMDSelect&&) = default;
		
		CMDSelect() = delete;
		
		explicit CMDSelect(const std::shared_ptr<IConnection>& connection);
		
		
	protected:
		CMDSelect& append_column_exp(std::string_view exp, const std::vector<BindValue>& bind);
		CMDSelect& append_where(std::string_view exp, const std::vector<BindValue>& bind);
		CMDSelect& append_order_by(std::string_view exp, const std::vector<BindValue>& bind);
		CMDSelect& append_group_by(std::string_view exp, const std::vector<BindValue>& bind);
		CMDSelect& append_having(std::string_view exp, const std::vector<BindValue>& bind);
		
		
	public:
		SQLIGHTER_INLINE_CLAUSE(column_exp,	append_column_exp,	CMDSelect);
		SQLIGHTER_INLINE_CLAUSE(where,		append_where,	 	CMDSelect);
		SQLIGHTER_INLINE_CLAUSE(group_by,	append_group_by,	CMDSelect);
		SQLIGHTER_INLINE_CLAUSE(having,		append_having,		CMDSelect);
		SQLIGHTER_INLINE_CLAUSE(order_by,	append_order_by,	CMDSelect);
	
	
	public:
		CMDSelect& distinct();
		
		
	public:
		CMDSelect& column(std::string_view column);
		CMDSelect& column_as(std::string_view column, std::string_view as);
		CMDSelect& column_as(std::string_view column, char as);
		CMDSelect& columns(const std::vector<std::string_view>& columns);
		CMDSelect& columns(std::initializer_list<std::string_view> columns);
		
		
	public:
		CMDSelect& from(std::string_view table);
		CMDSelect& from(std::string_view table, char alias);
		CMDSelect& from(std::string_view table, std::string_view alias);
		
		
	public:
		CMDSelect& where_null(std::string_view column);
		CMDSelect& where_not_null(std::string_view column);
		CMDSelect& by_field(std::string_view column, BindValue value);
		
		
	public:
		CMDSelect& group_by_field(std::string_view by);
		
		
	public:
		CMDSelect& order_by_field(std::string_view by, OrderBy order);
		CMDSelect& order_by_field(std::string_view by);
		
		inline CMDSelect& order_by_field_asc(std::string_view by)	{ return order_by_field(by, OrderBy::ASC); };
		inline CMDSelect& order_by_field_desc(std::string_view by)	{ return order_by_field(by, OrderBy::DESC); };
		
		
	public:
		CMDSelect& limit_by(int count);
		CMDSelect& limit(int offset, int count);
		
		inline CMDSelect& page(int page, int page_size) { return limit(page * page_size, page_size); };
		
		
	public:
		void assemble(std::ostringstream& ss) const override;
		[[nodiscard]] std::string assemble() const override;
		[[nodiscard]] std::vector<BindValue> bind() const override;
		Stmt execute() const override;
		
		
	public:
		ScalarValue query_scalar() const;
		std::vector<ScalarValue> query_column() const;
		std::vector<ScalarValue> query_row(bool expect_one) const;
		std::vector<std::vector<ScalarValue>> query_all(int failsafeLimit = 10000) const;
		
		int64_t query_int() const;
		double query_double() const;
		bool query_bool() const;
		bool query_is_null() const;
		std::string query_str() const;
		blob_t query_blob() const;
		int64_t query_count() const;
	};
}
