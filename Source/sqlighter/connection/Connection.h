#pragma once


#include <memory>
#include <filesystem>

#include "base/connection/IConnection.h"


namespace sqlighter
{
	class DB;
	
	
	class Connection : public IConnection
	{
	private:
		std::shared_ptr<DB>	m_db;
		
		
	public:
		Connection() = delete;
		Connection(const Connection&) = delete;
		Connection(Connection&&) = delete;
		Connection& operator=(const Connection&) = delete;
		Connection& operator=(Connection&&) = delete;
		
		explicit Connection(const std::filesystem::path& db_path);
		explicit Connection(std::string_view db_path);
		explicit Connection(const std::shared_ptr<DB>& db);
		
		
	public:
		Stmt execute(std::string_view query, const std::vector<BindValue>& values) override;
	};
}