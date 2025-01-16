#include "CMDDirect.h"


#include "core/Stmt.h"


using namespace sqlighter;


CMDDirect::CMDDirect(const std::shared_ptr<IConnection>& connection) : 
	m_connection(connection)
{
	if (m_connection == nullptr)
	{
		throw std::runtime_error("Connection must be set");
	}
}

CMDDirect::CMDDirect(const CMDDirect& c) :
	m_connection(c.m_connection),
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


CMDDirect& CMDDirect::do_append(std::string_view exp, std::span<const BindValue> span)
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
	return m_stream.str();
}

std::vector<BindValue> CMDDirect::bind() const
{
	return m_binds;
}

Stmt CMDDirect::execute() const
{
	return m_connection->execute(
		assemble(),
		bind()
	);
}

void CMDDirect::execute_direct() const
{
	execute().require_done();
}
