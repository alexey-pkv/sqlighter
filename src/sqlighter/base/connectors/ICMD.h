#ifndef SQLIGHTER_BASE_CONNECTORS_ICMD_H
#define SQLIGHTER_BASE_CONNECTORS_ICMD_H


#include <string>
#include <vector>

#include "core/Stmt.h"
#include "base/connection/IConnection.h"


namespace sqlighter
{
	class BindValue;
	
	
	class ICMD
	{
	public:
		virtual ~ICMD() = default; // LCOV_EXCL_LINE
		
		
	public:
		virtual void assemble(std::ostringstream& ss) const = 0;
		[[nodiscard]] virtual std::string assemble() const = 0;
		[[nodiscard]] virtual std::vector<BindValue> bind() const = 0;
		virtual Stmt execute() const = 0; // NOLINT(*-use-nodiscard)
	};
	
	class CMD : public ICMD
	{
	private:
		std::shared_ptr<IConnection> m_connection;
		
		
	public:
		~CMD() override = default; // LCOV_EXCL_LINE
		explicit CMD(const std::shared_ptr<IConnection>& connection);
		
		
	public:
		void assemble(std::ostringstream& ss) const override = 0;
		[[nodiscard]] std::vector<BindValue> bind() const override = 0;
		
		
	public:
		[[nodiscard]] std::string assemble() const override;
		Stmt execute() const override;
	};
}


#endif
