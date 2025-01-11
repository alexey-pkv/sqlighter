#ifndef SQLIGHTER_CMDSELECT_H
#define SQLIGHTER_CMDSELECT_H


#include <sstream>
#include <memory>

#include "connectors/Clause.h"
#include "base/connectors/ICMDSelect.h"


namespace sqlighter
{
	class IConnection;
	
	
	class CMDSelect : public ICMDSelect
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
		~CMDSelect() override = default;
		CMDSelect(CMDSelect&&) noexcept = default;
		CMDSelect(const CMDSelect&) = default;
		CMDSelect& operator=(const CMDSelect&) = default;
		CMDSelect& operator=(CMDSelect&&) = default;
		
		CMDSelect() = delete;
		
		explicit CMDSelect(const std::shared_ptr<IConnection>& connection);
		
		
	protected:
		void append_column_exp(std::string_view exp, std::span<const BindValue> span) override;
		void append_where(std::string_view exp, std::span<const BindValue> span) override;
		void append_order_by(std::string_view exp, std::span<const BindValue> span) override;
		void append_group_by(std::string_view exp, std::span<const BindValue> span) override;
		void append_having(std::string_view exp, std::span<const BindValue> span) override;
	
	
	public:
		void distinct() override;
		
		
	public:
		void column(std::string_view column) override;
		void column_as(std::string_view column, std::string_view as) override;
		void column_as(std::string_view column, char as) override;
		void columns(const std::vector<std::string_view>& columns) override;
		void columns(std::initializer_list<std::string_view> columns) override;
		
		
	public:
		void from(std::string_view table) override;
		void from(std::string_view table, char alias) override;
		void from(std::string_view table, std::string_view alias) override;
		
		
	public:
		void where_null(std::string_view column) override;
		void where_not_null(std::string_view column) override;
		void by_field(std::string_view column, BindValue value) override;
		
		
	public:
		void group_by_field(std::string_view by) override;
		
		
	public:
		void order_by_field(std::string_view by, OrderBy order) override;
		void order_by_field(std::string_view by) override;
		
		
	public:
		void limit_by(int count) override;
		void limit(int offset, int count) override;
		
		
	public:
		void assemble(std::ostringstream& ss) const override;
		[[nodiscard]] std::string assemble() const override;
		[[nodiscard]] std::vector<BindValue> bind() const override;
		
		
	public:
		Stmt execute() const override;
	};
}


#endif
