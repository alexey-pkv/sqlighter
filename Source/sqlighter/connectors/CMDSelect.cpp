#include "CMDSelect.h"


#include "query_utils.h"

#include "core/Stmt.h"
#include "base/connection/IConnection.h"
#include "consts/const_cast.h"
#include "exceptions/sqlighter_exceptions.h"


using namespace sqlighter;


CMDSelect::CMDSelect(const std::shared_ptr<IConnection>& connection) : 
	m_connection(connection)
{
	if (connection == nullptr)
	{
		throw SQLighterException(SQLIGHTER_ERR_UNEXPECTED, "connection should not be null!");
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

ScalarValue CMDSelect::query_scalar() const
{
	ScalarValue v {};
	auto stmt = execute();
	
	stmt.require_one_column();
	
	v = stmt.column_value(0);
	
	stmt.step();
	stmt.require_done();
	
	return v;
}

int64_t CMDSelect::query_int() const
{
	return query_scalar().get_int64();
}

double CMDSelect::query_double() const
{
	return query_scalar().get_double();
}

bool CMDSelect::query_bool() const
{
	return query_scalar().get_int64() != 0;
}

bool CMDSelect::query_is_null() const
{
	return query_scalar().is_null();
}

std::string CMDSelect::query_str() const
{
	return query_scalar().get_str();
}

blob_t CMDSelect::query_blob() const
{
	return query_scalar().get_blob();
}

int64_t CMDSelect::query_count() const
{
	CMDSelect countSelect { *this };
	
	countSelect.m_columns.clear();
	countSelect.column_exp("COUNT(*)");
	
	return countSelect.query_int();
}

std::vector<ScalarValue> CMDSelect::query_column() const
{
	std::vector<ScalarValue> data {};
	auto stmt = execute();
	
	stmt.require_one_column();
	
	while (stmt.has_row())
	{
		data.emplace_back(stmt.column_value(0));
		stmt.step();
	}
	
	stmt.require_done();
	
	return data;
}

std::vector<ScalarValue> CMDSelect::query_row(bool expect_one) const
{
	auto stmt = execute();
	
	if (!expect_one && !stmt.has_row())
	{
		return {};
	}
	
	auto res = stmt.row();
	
	if (expect_one)
	{
		stmt.step();
		
		if (!stmt.is_done())
		{
			throw SQLighterException(SQLIGHTER_ERR_MULT_ROWS).query(assemble());
		}
	}
	
	return res;
}

std::vector<std::vector<ScalarValue>> CMDSelect::query_all(int failsafeLimit) const
{
	auto stmt = execute();
	
	if (!stmt.has_row())
		return {};
	
	std::vector<std::vector<ScalarValue>> data = {};
	
	while (stmt.has_row())
	{
		if (data.size() >= failsafeLimit)
			throw SQLighterException(SQLIGHTER_ERR_ROWS_OVERFLOW).query(assemble());
		
		data.emplace_back(stmt.row());
		stmt.step();
	}
	
	return data;
}

