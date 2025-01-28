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
	
	ss << m_table << m_set;
	
	if (!where_clause().is_empty_clause())
	{
		ss << " WHERE " << where_clause();
	}
}

std::vector<BindValue> CMDUpdate::bind() const
{
	std::vector<BindValue> final = {};
	
	final.reserve(
		m_set.binds_size() +
		where_clause().binds_size());
	
	m_set.append_binds(final);
	where_clause().append_to(final);
	
	return final;
} // LCOV_EXCL_LINE

std::string CMDUpdate::assemble() const
{
	return CMD::assemble();
}
