#pragma once


#include "base/connectors/ICMD.h"
#include "connectors/Clause/ClauseLimit.h"
#include "connectors/Clause/ClauseOrderBy.h"
#include "connectors/Clause/ClauseWhere.h"
#include "connectors/Clause/ClauseTable.h"


namespace sqlighter
{
	class CMDDelete : public CMD, 
		public ClauseWhere<CMDDelete>,
		public ClauseOrderBy<CMDDelete>,
		public ClauseLimit<CMDDelete>
	{
	private:
		ClauseTable		m_from;
		ClauseOrderBy	m_orderBy;
		
		
	public:
		CMDDelete(CMDDelete&&) noexcept = default;
		CMDDelete(const CMDDelete&) = default;
		CMDDelete& operator=(const CMDDelete&) = default;
		CMDDelete& operator=(CMDDelete&&) = default;
		
		CMDDelete() = delete;
		
		explicit CMDDelete(const std::shared_ptr<IConnection>& connection);
		
		
	public:
		CMDDelete& as(std::string_view alias);
		CMDDelete& from(std::string_view table);
		CMDDelete& from(std::string_view scheme, std::string_view table);
		
		
	public:
		void assemble(std::ostringstream& ss) const override;
		std::string assemble() const override;
		std::vector<BindValue> bind() const override;
	};
}