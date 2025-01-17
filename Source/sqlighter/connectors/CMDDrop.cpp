#include "CMDDrop.h"


#include <sstream>

#include "query_utils.h"
#include "exceptions/sqlighter_exceptions.h"


using namespace sqlighter;


CMDDrop::CMDDrop(const std::shared_ptr<IConnection>& connection) : CMD(connection) {}


CMDDrop& CMDDrop::if_exists()
{
	m_ifExists = true;
	return *this;
}

CMDDrop& CMDDrop::element(std::string_view e, std::string_view name)
{
	m_element = e;
	m_scheme.clear();
	
	element_name(name, m_scheme, m_name);
	
	return *this;
}

CMDDrop& CMDDrop::element(std::string_view e, std::string_view scheme, std::string_view name)
{
	m_element = e;
	m_scheme = scheme;
	m_name = name;
	
	return *this;
}

std::string CMDDrop::assemble() const
{
	return CMD::assemble();
}

void CMDDrop::assemble(std::ostringstream& ss) const
{
	ss << "DROP " << m_element << " ";
	
	if (m_ifExists)
		ss << "IF EXISTS ";
	
	if (!m_scheme.empty())
		ss << '`' << m_scheme << "`.";
	
	ss << '`' << m_name << '`';
}

std::vector<BindValue> CMDDrop::bind() const
{
	return {};
}
