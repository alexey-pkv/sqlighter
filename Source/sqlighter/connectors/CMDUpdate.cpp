#include "CMDUpdate.h"


using namespace sqlighter;


CMDUpdate::CMDUpdate(const std::shared_ptr<IConnection>& connection) : CMD(connection)
{
	
}


CMDUpdate& CMDUpdate::or_abort()	{ m_or = "ABORT"; return *this; }
CMDUpdate& CMDUpdate::or_fail()		{ m_or = "FAIL"; return *this; }
CMDUpdate& CMDUpdate::or_ignore()	{ m_or = "IGNORE"; return *this; }
CMDUpdate& CMDUpdate::or_replace()	{ m_or = "REPLACE"; return *this; }
CMDUpdate& CMDUpdate::or_rollback()	{ m_or = "ROLLBACK"; return *this; }


CMDUpdate& CMDUpdate::table(std::string_view table)
{
	m_table.table(table);
	return *this;
}

CMDUpdate& CMDUpdate::table(std::string_view scheme, std::string_view table)
{
	m_table.table(scheme, table);
	return *this;
}

CMDUpdate& CMDUpdate::as(std::string_view as)
{
	m_table.as(as);
	return *this;
}


void CMDUpdate::assemble(std::ostringstream& ss) const
{
	ss << "UPDATE ";
	
	if (!m_or.empty())
		ss << "OR " << m_or << ' ';
	
	ss << m_table << m_set << m_where;
}

std::vector<BindValue> CMDUpdate::bind() const
{
	std::vector<BindValue> final = {};
	
	final.reserve(
		m_set.binds_size() +
		m_where.binds_size());
	
	m_set.append_binds(final);
	m_where.append_binds(final);
	
	return final;
}

std::string CMDUpdate::assemble() const
{
	return CMD::assemble();
}
