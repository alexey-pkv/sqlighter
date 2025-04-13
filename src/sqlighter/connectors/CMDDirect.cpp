#include "CMDDirect.h"


#include "core/Stmt.h"
#include "exceptions/sqlighter_exceptions.h"


using namespace sqlighter;


CMDDirect::CMDDirect(const std::shared_ptr<IConnection>& connection) : CMD(connection) {}

CMDDirect::CMDDirect(const CMDDirect& c) :
	CMD(c),
	m_stream(c.m_stream.str()),
	m_binds(c.m_binds)
{
	
}

CMDDirect& CMDDirect::operator=(const CMDDirect& c)
{
	m_stream = {};
	
	m_binds = c.m_binds;
	m_stream << c.m_stream.str();
	
	return *this;
}


CMDDirect& CMDDirect::do_append(std::string_view exp, const std::vector<BindValue>& span)
{
	m_stream << exp;
	
	m_binds.insert(m_binds.end(), span.begin(), span.end());
	
	return *this;
}


void CMDDirect::assemble(std::ostringstream& ss) const
{
	ss << m_stream.str();
}

std::string CMDDirect::assemble() const
{
	return CMD::assemble();
}

std::vector<BindValue> CMDDirect::bind() const
{
	return m_binds;
}


void CMDDirect::execute_direct() const
{
	execute().require_done();
}
