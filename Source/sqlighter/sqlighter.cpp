#include "sqlighter.h"


#include "core/DB.h"
#include "connectors/CMDSelect.h"
#include "connectors/CMDDirect.h"


using namespace sqlighter;


SQLighter::SQLighter(const std::filesystem::path& db_path) :
	m_db(std::make_shared<DB>(db_path.c_str())),
	m_connection(std::make_shared<Connection>(m_db))
{
	
}

SQLighter::SQLighter(std::string_view db_path) :
	m_db(std::make_shared<DB>(db_path)),
	m_connection(std::make_shared<Connection>(m_db))
{
	
}

SQLighter::SQLighter(const std::shared_ptr<DB>& db) :
	m_db(db),
	m_connection(std::make_shared<Connection>(db))
{
	
}


std::shared_ptr<ICMDSelect> SQLighter::select() const
{
	return std::make_shared<CMDSelect>(m_connection);
}

std::shared_ptr<ICMDDirect> SQLighter::direct() const
{
	return std::make_shared<CMDDirect>(m_connection);
}


const std::string& SQLighter::path() const
{
	return m_db->path();
}
