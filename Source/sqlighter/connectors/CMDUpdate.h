#pragma once


#include "base/connectors/ICMD.h"

#include "connectors/Clause/ClauseSet.h"
#include "connectors/Clause/ClauseTable.h"
#include "connectors/Clause/ClauseWhere.h"


namespace sqlighter
{
	class CMDUpdate : public CMD,
		public ClauseWhere<CMDUpdate> 
	{
	private:
		std::string	m_or	{};
		ClauseTable	m_table {};
		ClauseSet	m_set	{};
		
		
	public:
		CMDUpdate(CMDUpdate&&) noexcept = default;
		CMDUpdate(const CMDUpdate&) = default;
		CMDUpdate& operator=(const CMDUpdate&) = default;
		CMDUpdate& operator=(CMDUpdate&&) = default;
		
		CMDUpdate() = delete;
		
		explicit CMDUpdate(const std::shared_ptr<IConnection>& connection);
		
		
	public:
		CMDUpdate& table(std::string_view table);
		CMDUpdate& table(std::string_view scheme, std::string_view table);
		CMDUpdate& as(std::string_view as);
		
		CMDUpdate& or_abort();
		CMDUpdate& or_fail();
		CMDUpdate& or_ignore();
		CMDUpdate& or_replace();
		CMDUpdate& or_rollback();
		
		
	public:
		SQLIGHTER_SET_CLAUSE(m_set, CMDUpdate);
		
		
	public:
		std::string assemble() const override;
		void assemble(std::ostringstream& ss) const override;
		std::vector<BindValue> bind() const override;
	};
}