#ifndef SQLIGHTER_BASE_CONNECTORS_PREPARED_H
#define SQLIGHTER_BASE_CONNECTORS_PREPARED_H


#include <string>
#include <vector>

#include "core/Stmt.h"
#include "base/connection/IConnection.h"


namespace sqlighter
{
	class PreparedCMD
	{
	private:
		Stmt m_stmt;
		
		std::shared_ptr<IConnection>	m_connection;
		std::string 					m_query;
		std::vector<BindValue>			m_defaultBinds;
		std::vector<BindValue>			m_currentBinds;
		
		bool m_autoReset = false;
		
	public:
		virtual ~PreparedCMD() = default; // LCOV_EXCL_LINE
		PreparedCMD(PreparedCMD&&) = default;
		
		
		PreparedCMD(
				const std::shared_ptr<IConnection>& connection,
				Stmt&& stmt,
				std::string&& query,
				std::vector<BindValue>&& default_binds);
		
		const std::string& query() const;
		const std::vector<BindValue>& default_bind() const;
		std::vector<BindValue> bind() const;
		
		Stmt execute();
		
		void reset();
		void set_is_auto_reset(bool is_auto_reset);
		bool get_is_auto_reset();
	};
}


#endif
