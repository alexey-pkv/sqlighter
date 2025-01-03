#pragma once


#include "base/connection/IConnection.h"


namespace sqlighter
{
	class connection_override : public IConnection
	{
	public:
		std::string				last_query	= {};
		std::vector<BindValue>	last_bind	= {};
		
		
	public:
		Stmt execute(std::string_view query, const std::vector<BindValue>& values) override;
	};
}