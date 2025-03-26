#pragma once


#include <memory>
#include <sstream>

#include "base/connectors/ICMD.h"

#include "connectors/Clause/ClauseFrom.h"
#include "connectors/Clause/ClauseJoin.h"
#include "connectors/Clause/ClauseWhere.h"
#include "connectors/Clause/ClauseLimit.h"
#include "connectors/Clause/ClauseOrderBy.h"
#include "connectors/Clause/ClauseTable.h"


namespace sqlighter
{
	class IConnection;
	
	
	class CMDSelect :
		public CMD,
		public ClauseJoin<CMDSelect>,
		public ClauseFrom<CMDSelect>,
		public ClauseWhere<CMDSelect>,
		public ClauseOrderBy<CMDSelect>,
		public ClauseLimit<CMDSelect>
	{
	private:
		bool m_distinct	= false;
		
		Clause			m_columns	{ ", " };
		Clause			m_groupBy	{ ", " };
		Clause			m_having	{ " AND " };
		
		
	public:
		CMDSelect(CMDSelect&&) noexcept = default;
		CMDSelect(const CMDSelect&) = default;
		CMDSelect& operator=(const CMDSelect&) = default;
		CMDSelect& operator=(CMDSelect&&) = default;
		
		CMDSelect() = delete;
		
		explicit CMDSelect(const std::shared_ptr<IConnection>& connection);
		
		
	protected:
		CMDSelect& append_column_exp(std::string_view exp, const std::vector<BindValue>& bind);
		CMDSelect& append_group_by(std::string_view exp, const std::vector<BindValue>& bind);
		CMDSelect& append_having(std::string_view exp, const std::vector<BindValue>& bind);
	
		
	public:
		SQLIGHTER_INLINE_CLAUSE(column_exp,	append_column_exp,	CMDSelect);
		SQLIGHTER_INLINE_CLAUSE(group_by,	append_group_by,	CMDSelect);
		SQLIGHTER_INLINE_CLAUSE(having,		append_having,		CMDSelect);
	
	
	public:
		CMDSelect& distinct();
		
		
	public:
		CMDSelect& column(std::string_view column);
		CMDSelect& column_as(std::string_view column, std::string_view as);
		CMDSelect& column_as(std::string_view column, char as);
		CMDSelect& columns(const std::vector<std::string_view>& columns);
		CMDSelect& columns(std::initializer_list<std::string_view> columns);
		
		
	public:
		// CMDSelect& join(std::string_view table, std::string_view alias, std::string_view condition, )
		
		
	public:
		CMDSelect& group_by_field(std::string_view by);
		
		
	public:
		void assemble(std::ostringstream& ss) const override;
		[[nodiscard]] std::string assemble() const override;
		[[nodiscard]] std::vector<BindValue> bind() const override;
		
		
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
