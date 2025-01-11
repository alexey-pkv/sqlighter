#include "CMDSelect.h"


#include "query_utils.h"

#include "consts/const_cast.h"
#include "core/Stmt.h"
#include "base/connection/IConnection.h"


using namespace sqlighter;


CMDSelect::CMDSelect(const std::shared_ptr<IConnection>& connection) : 
	m_connection(connection)
{
	if (connection == nullptr)
	{
		throw std::runtime_error("connection should not be null!");
	}
}


CMDSelect& CMDSelect::append_column_exp(std::string_view exp, std::span<const BindValue> span)
{
	m_columns.append(exp, span);
	return *this;
}

CMDSelect& CMDSelect::append_where(std::string_view exp, std::span<const BindValue> span)
{
	m_where.append(exp, span);
	return *this;
}

CMDSelect& CMDSelect::append_group_by(std::string_view exp, std::span<const BindValue> span)
{
	m_groupBy.append(exp, span);
	return *this;
}

CMDSelect& CMDSelect::append_having(std::string_view exp, std::span<const BindValue> span)
{
	m_having.append(exp, span);
	return *this;
}

CMDSelect& CMDSelect::append_order_by(std::string_view exp, std::span<const BindValue> span)
{
	m_orderBy.append(exp, span);
	return *this;
}


CMDSelect& CMDSelect::distinct()
{
	m_distinct = true;
	return *this;
}


CMDSelect& CMDSelect::column(std::string_view column)
{
	m_columns << col(column);
	return *this;
}

CMDSelect& CMDSelect::column_as(std::string_view column, std::string_view as)
{
	m_columns << col_as(column, as);
	return *this;
}

CMDSelect& CMDSelect::column_as(std::string_view column, char as)
{
	m_columns << col_as(column, { &as, 1 });
	return *this;
}

CMDSelect& CMDSelect::columns(const std::vector<std::string_view>& columns)
{
	for (const auto& c : columns)
	{
		m_columns << col(c);
	}
	
	return *this;
}

CMDSelect& CMDSelect::columns(std::initializer_list<std::string_view> columns)
{
	for (const auto& c : columns)
	{
		m_columns << col(c);
	}
	
	return *this;
}


CMDSelect& CMDSelect::from(std::string_view table)
{
	m_from = wrap_element(table);
	return *this;
}

CMDSelect& CMDSelect::from(std::string_view table, char alias)
{
	m_from = wrap_element(table, { &alias, 1 });
	return *this;
}

CMDSelect& CMDSelect::from(std::string_view table, std::string_view alias)
{
	m_from = wrap_element(table, alias);
	return *this;
}

CMDSelect& CMDSelect::where_null(std::string_view column)
{
	m_where << col(column) << " IS NULL";
	return *this;
}

CMDSelect& CMDSelect::where_not_null(std::string_view column)
{
	m_where << col(column) << " IS NOT NULL";
	return *this;
}

CMDSelect& CMDSelect::by_field(std::string_view column, BindValue value)
{
	m_where << col(column) << " = ?";
	
	m_where.append_bind(value);
	
	return *this;
}

CMDSelect& CMDSelect::group_by_field(std::string_view by)
{
	m_groupBy << col(by);
	return *this;
}

CMDSelect& CMDSelect::order_by_field(std::string_view by)
{
	m_orderBy << col(by);
	return *this;
}

CMDSelect& CMDSelect::order_by_field(std::string_view by, OrderBy order)
{
	m_orderBy << col(by);
	
	if (order == OrderBy::DESC)
	{
		m_orderBy.direct() << ' ' << order;
	}
	
	return *this;
}


CMDSelect& CMDSelect::limit_by(int count)
{
	m_hasLimit = true;
	
	m_limitCount = count;
	m_limitOffset = 0;
	
	return *this;
}

CMDSelect& CMDSelect::limit(int offset, int count)
{
	m_hasLimit = true;
	
	m_limitCount = count;
	m_limitOffset = offset;
	
	return *this;
}


void CMDSelect::assemble(std::ostringstream& ss) const
{
	ss << "SELECT";
	
	if (m_distinct)
	{
		ss << " DISTINCT";
	}
	
	if (!m_columns.empty_clause())
	{
		ss << ' ';
		m_columns.append_to(ss);
	}
	else
	{
		ss << " *";
	}
	
	if (!m_from.empty())
	{
		ss << " FROM " << m_from;
	}
	
	if (!m_where.empty_clause())
	{
		ss << " WHERE ";
		m_where.append_to(ss);
	}
	
	if (!m_groupBy.empty_clause())
	{
		ss << " GROUP BY ";
		m_groupBy.append_to(ss);
	}
	
	if (!m_having.empty_clause())
	{
		ss << " HAVING ";
		m_having.append_to(ss);
	}
	
	if (!m_orderBy.empty_clause())
	{
		ss << " ORDER BY ";
		m_orderBy.append_to(ss);
	}
	
	if (m_hasLimit)
	{
		ss << " LIMIT ";
		
		if (m_limitOffset != 0)
		{
			ss << m_limitOffset << ", ";
		}
		
		ss << m_limitCount;
	}
}

std::string CMDSelect::assemble() const
{
	std::ostringstream ss;
	
	assemble(ss);
	
	return ss.str();
}

std::vector<BindValue> CMDSelect::bind() const
{
	std::vector<BindValue> final = {};
	
	auto total =
		m_columns.binds_size() + 
		m_where.binds_size() + 
		m_groupBy.binds_size() + 
		m_having.binds_size() + 
		m_orderBy.binds_size();
	
	if (total == 0)
		return {};
	
	final.reserve(total);
	
	m_columns.append_to(final);
	m_where.append_to(final);
	m_groupBy.append_to(final);
	m_having.append_to(final);
	m_orderBy.append_to(final);
	
	return final;
}


Stmt CMDSelect::execute() const
{
	return m_connection->execute(
		assemble(),
		bind()
	);
}

int CMDSelect::query_int()
{
	auto stmt = execute();
	int value;
	
	stmt.require_one_column();
	
	value = stmt.column_int(0);
	
	stmt.step();
	stmt.require_done();
	
	return value;
}
