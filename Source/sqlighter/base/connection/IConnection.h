#pragma once


#include <string>
#include <vector>


namespace sqlighter
{
	class Stmt;
	class BindValue;
	
	
	class IConnection
	{
	public:
		virtual ~IConnection() = default;
		
		
	public:
		virtual Stmt execute(const std::string& query, const std::vector<BindValue>& values) = 0;
	};
}
