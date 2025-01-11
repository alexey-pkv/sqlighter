#pragma once


#include <sstream>

#include "base/connectors/ICMDDirect.h"
#include "base/connection/IConnection.h"


namespace sqlighter
{
	class CMDDirect : public ICMDDirect
	{
	private:
		std::shared_ptr<IConnection>	m_connection;
		
		std::ostringstream		m_stream;
		std::vector<BindValue>	m_binds;
		
		
	public:
		CMDDirect(CMDDirect&&) noexcept = default;
		CMDDirect& operator=(CMDDirect&&) = default;
		
		CMDDirect(const CMDDirect&);
		CMDDirect& operator=(const CMDDirect&);
		
		CMDDirect() = delete;
		
		explicit CMDDirect(const std::shared_ptr<IConnection>& connection);
		
		
	protected:
		ICMDDirect& do_append(std::string_view exp, std::span<const BindValue> span) override;
		
		
	public:
		void assemble(std::ostringstream& ss) const override;
		
		[[nodiscard]] std::string assemble() const override;
		[[nodiscard]] std::vector<BindValue> bind() const override;
		
		Stmt execute() const override;
		
		
	public:
		void execute_direct() const override;
	};
}