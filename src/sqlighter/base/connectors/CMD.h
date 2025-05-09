#ifndef SQLIGHTER_BASE_CONNECTORS_CMD_H
#define SQLIGHTER_BASE_CONNECTORS_CMD_H


#include "core/Stmt.h"
#include "base/connection/IConnection.h"


namespace sqlighter
{
	class CMD
	{
	private:
		std::shared_ptr<IConnection> m_connection;
		
		
	public:
		virtual ~CMD() = default; // LCOV_EXCL_LINE
		explicit CMD(const std::shared_ptr<IConnection>& connection);
		
		
	public:
		virtual void assemble(std::ostringstream& ss) const = 0;
		[[nodiscard]] virtual std::vector<BindValue> bind() const = 0;
		
		
	public:
		[[nodiscard]] virtual  std::string assemble() const;
		virtual Stmt execute() const;
	};
}


#endif
