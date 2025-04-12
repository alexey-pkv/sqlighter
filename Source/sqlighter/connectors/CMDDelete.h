#ifndef SQLIGHTER_CONNECTORS_CMDDELETE_H
#define SQLIGHTER_CONNECTORS_CMDDELETE_H


#include "base/connectors/ICMD.h"
#include "connectors/Clause/ClauseFrom.h"
#include "connectors/Clause/ClauseLimit.h"
#include "connectors/Clause/ClauseWhere.h"
#include "connectors/Clause/ClauseTable.h"
#include "connectors/Clause/ClauseOrderBy.h"


namespace sqlighter
{
	class CMDDelete :
		public CMD,
		public ClauseFrom<CMDDelete>,
		public ClauseWhere<CMDDelete>,
		public ClauseOrderBy<CMDDelete>,
		public ClauseLimit<CMDDelete>
	{
	private:
		ClauseOrderBy	m_orderBy;
		
		
	public:
		CMDDelete(CMDDelete&&) noexcept = default;
		CMDDelete(const CMDDelete&) = default;
		CMDDelete& operator=(const CMDDelete&) = default;
		CMDDelete& operator=(CMDDelete&&) = default;
		
		CMDDelete() = delete;
		
		explicit CMDDelete(const std::shared_ptr<IConnection>& connection);
		
		
	public:
		void assemble(std::ostringstream& ss) const override;
		std::string assemble() const override;
		std::vector<BindValue> bind() const override;
	};
}


#endif
