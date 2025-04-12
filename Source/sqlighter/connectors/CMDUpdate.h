#ifndef SQLIGHTER_CONNECTORS_CMDUPDATE_H
#define SQLIGHTER_CONNECTORS_CMDUPDATE_H


#include "base/connectors/ICMD.h"

#include "connectors/Clause/ClauseSet.h"
#include "connectors/Clause/ClauseTable.h"
#include "connectors/Clause/ClauseWhere.h"


namespace sqlighter
{
	class CMDUpdate : public CMD,
		public ClauseWhere<CMDUpdate>,
		public ClauseSet<CMDUpdate>
	{
	private:
		std::string	m_or	{};
		ClauseTable	m_table {};
		
		
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
		std::string assemble() const override;
		void assemble(std::ostringstream& ss) const override;
		std::vector<BindValue> bind() const override;
	};
}


#endif
