#include "ICMD.h"


#include <iostream>
#include <sstream>

#include "exceptions/sqlighter_exceptions.h"


using namespace sqlighter;


CMD::CMD(const std::shared_ptr<IConnection>& connection) : 
	m_connection(connection)
{
	if (connection == nullptr)
	{
		throw SQLighterException(SQLIGHTER_ERR_GENERIC, "Connection must be set");
	}
}

std::string CMD::assemble() const
{
	std::ostringstream ss;
	
	assemble(ss);
	
	return ss.str();
}

Stmt CMD::execute() const
{
	return m_connection->execute(
		assemble(),
		bind()
	);
}
