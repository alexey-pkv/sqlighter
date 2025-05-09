#include "PreparedCMD.h"

#include "core/BindValue.h"
#include "exceptions/sqlighter_exceptions.h"


using namespace sqlighter;


sqlighter::PreparedCMD::PreparedCMD(
		const std::shared_ptr<IConnection>& connection,
		Stmt&& stmt,
		std::string&& query,
		std::vector<BindValue>&& default_binds) :
	m_stmt(std::move(stmt)),
	m_connection(connection),
	m_query(std::move(query)),
	m_defaultBinds(std::move(default_binds)),
	m_currentBinds(m_defaultBinds)
{
	
}


const std::string& PreparedCMD::query() const
{
	return m_query;
}

const std::vector<BindValue>& PreparedCMD::default_bind() const
{
	return m_defaultBinds;
}

std::vector<BindValue> PreparedCMD::bind() const
{
	return m_currentBinds;
}

Stmt PreparedCMD::execute()
{
	m_connection->execute_prepared(m_stmt, m_currentBinds);
	
	if (m_autoReset)
	{
		m_currentBinds = m_defaultBinds;
	}
		
	return m_stmt.weak();
}

void PreparedCMD::reset()
{
	m_currentBinds = m_defaultBinds;
	
	auto stmt = m_stmt.stmt_ref().get();
	
	if (stmt)
	{
		int code = sqlite3_reset(stmt);
		
		if (code != SQLITE_OK)
		{
			throw SQLighterException(SQLIGHTER_ERR_RESET, code, m_query);
		}
	}
}

void PreparedCMD::set_is_auto_reset(bool is_auto_reset)
{
	m_autoReset = is_auto_reset;
}

bool PreparedCMD::get_is_auto_reset()
{
	return m_autoReset;
}
