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
		ICMDSelect& append_column_exp(std::string_view exp, std::span<const BindValue> span) override;
		ICMDSelect& append_where(std::string_view exp, std::span<const BindValue> span) override;
		ICMDSelect& append_order_by(std::string_view exp, std::span<const BindValue> span) override;
		ICMDSelect& append_group_by(std::string_view exp, std::span<const BindValue> span) override;
		ICMDSelect& append_having(std::string_view exp, std::span<const BindValue> span) override;
	
	
	public:
		ICMDSelect& distinct() override;
		
		
	public:
		ICMDSelect& column(std::string_view column) override;
		ICMDSelect& column_as(std::string_view column, std::string_view as) override;
		ICMDSelect& column_as(std::string_view column, char as) override;
		ICMDSelect& columns(const std::vector<std::string_view>& columns) override;
		ICMDSelect& columns(std::initializer_list<std::string_view> columns) override;
		
		
	public:
		ICMDSelect& from(std::string_view table) override;
		ICMDSelect& from(std::string_view table, char alias) override;
		ICMDSelect& from(std::string_view table, std::string_view alias) override;
		
		
	public:
		ICMDSelect& where_null(std::string_view column) override;
		ICMDSelect& where_not_null(std::string_view column) override;
		ICMDSelect& by_field(std::string_view column, BindValue value) override;
		
		
	public:
		ICMDSelect& group_by_field(std::string_view by) override;
		
		
	public:
		ICMDSelect& order_by_field(std::string_view by, OrderBy order) override;
		ICMDSelect& order_by_field(std::string_view by) override;
		
		
	public:
		ICMDSelect& limit_by(int count) override;
		ICMDSelect& limit(int offset, int count) override;
		
		
	public:
		void assemble(std::ostringstream& ss) const override;
		[[nodiscard]] std::string assemble() const override;
		[[nodiscard]] std::vector<BindValue> bind() const override;
		
		
	public:
		Stmt execute() const override;
	};
}


#endif
