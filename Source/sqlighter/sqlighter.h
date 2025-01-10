#pragma once


#include "connection/Connection.h"
#include "base/connectors/ICMDSelect.h"
#include "base/connectors/ICMDDirect.h"


namespace sqlighter
{
	class SQLighter
	{
	private:
		std::shared_ptr<DB>			m_db;
		std::shared_ptr<Connection>	m_connection;
		
		
	public:
		~SQLighter() = default;
		SQLighter() = delete;
		
		SQLighter(SQLighter&&) = default;
		SQLighter(const SQLighter&) = default;
		SQLighter& operator=(SQLighter&&) = default;
		SQLighter& operator=(const SQLighter&) = default;
		
		explicit SQLighter(const std::filesystem::path& db_path);
		explicit SQLighter(std::string_view db_path);
		explicit SQLighter(const std::shared_ptr<DB>& db);
		
		
	public:
		std::shared_ptr<ICMDSelect> select() const;
		std::shared_ptr<ICMDDirect> direct() const;
		
		
	public:
		const std::string& path() const;
	};
}
