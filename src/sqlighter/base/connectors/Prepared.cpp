#include "Prepared.h"


#include "core/BindValue.h"

#include <sstream>


using namespace sqlighter;


std::string Prepared::assemble() const
{
	return m_command;
}

void Prepared::assemble(std::ostringstream& ss) const
{
	ss << m_command;
}

std::vector<BindValue> Prepared::bind() const
{
	return m_binds;
}
/*
Prepared::Prepared(
		const std::shared_ptr<IConnection>& connection, 
		std::string&& command,
		std::vector<BindValue>&& binds) :
	CMD(connection, std::move(command)),
	m_binds(std::move(binds))
{
	
}*/



Stmt Prepared::execute() const
{
	
	auto stmt = CMD::execute();
	/*
	if (m_autoReset)
	{
		m_binds.clear();
	}
	 */
	
	return stmt;
}

void Prepared::set_auto_reset_binds(bool autoReset)
{
	m_autoReset = autoReset;
}

void Prepared::reset_binds()
{
	m_binds.clear();
}
