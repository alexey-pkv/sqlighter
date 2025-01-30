#pragma once


#include "base/connectors/ICMD.h"
#include "connectors/Clause/ClauseTable.h"
#include "connectors/Clause/ClauseSet.h"


namespace sqlighter
{
	class CMDInsert : 
		public CMD, 
		public ClauseSet<CMDInsert>
	{
	private:
		int m_columnsCount	= -1;
		
		ClauseTable	m_into	{};
		std::string m_or	{};
		ClauseSet	m_set	{};
		
		std::vector<std::string>	m_columns	{};
		std::vector<BindValue>		m_binds		{};
		
		bool m_isDefaultValues	{ false };
		bool m_doNothing		{ false };
		
		std::string m_onConflictColumn	{};
		
		
	public:
		CMDInsert(CMDInsert&&) noexcept = default;
		CMDInsert(const CMDInsert&) = default;
		CMDInsert& operator=(const CMDInsert&) = default;
		CMDInsert& operator=(CMDInsert&&) = default;
		
		CMDInsert() = delete;
		
		explicit CMDInsert(const std::shared_ptr<IConnection>& connection);
		
		
	public:
		CMDInsert& or_abort();
		CMDInsert& or_fail();
		CMDInsert& or_ignore();
		CMDInsert& or_replace();
		CMDInsert& or_rollback();
		
		CMDInsert& as(std::string_view alias);
		CMDInsert& into(std::string_view table);
		CMDInsert& into(std::string_view scheme, std::string_view table);
		
		CMDInsert& column(std::string_view name);
		CMDInsert& columns(const std::vector<std::string>& names);
		CMDInsert& record(const std::vector<BindValue>& values);
		CMDInsert& record(std::initializer_list<BindValue> values);
		CMDInsert& records(const std::vector<std::vector<BindValue>>& values);
		CMDInsert& records(std::initializer_list<const std::vector<BindValue>> values);
		CMDInsert& records(std::initializer_list<std::initializer_list<BindValue>> values);
		
		CMDInsert& default_values();
		CMDInsert& on_conflict_do_nothing();
		CMDInsert& on_conflict(std::string_view column);
		
		
	public:
		void assemble(std::ostringstream& ss) const override;
		std::vector<BindValue> bind() const override;
		std::string assemble() const override;
	};
}

