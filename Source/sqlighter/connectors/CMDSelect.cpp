#include "CMDSelect.h"


#include "query_utils.h"

#include "base/connection/IConnection.h"
#include "consts/const_cast.h"
#include "exceptions/sqlighter_exceptions.h"


using namespace sqlighter;


CMDSelect::CMDSelect(const std::shared_ptr<IConnection>& connection) :
	CMD(connection)
{}


CMDSelect& CMDSelect::append_column_exp(std::string_view exp, const std::vector<BindValue>& bind)
{
	m_columns.append(exp, bind);
	return *this;
}

CMDSelect& CMDSelect::append_group_by(std::string_view exp, const std::vector<BindValue>& bind)
{
	m_groupBy.append(exp, bind);
	return *this;
}

CMDSelect& CMDSelect::append_having(std::string_view exp, const std::vector<BindValue>& bind)
{
	m_having.append(exp, bind);
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
	m_from.table(table);
	return *this;
}

CMDSelect& CMDSelect::from(std::string_view scheme, std::string_view table)
{
	m_from.table(scheme, table);
	return *this;
}

CMDSelect& CMDSelect::as(std::string_view as)
{
	m_from.as(as);
	return *this;
}

CMDSelect& CMDSelect::group_by_field(std::string_view by)
{
	m_groupBy << col(by);
	return *this;
}


void CMDSelect::assemble(std::ostringstream& ss) const
{
	ss << "SELECT";
	
	if (m_distinct)
	{
		ss << " DISTINCT";
	}
	
	if (!m_columns.is_empty_clause())
	{
		ss << ' ';
		m_columns.append_to(ss);
	}
	else
	{
		ss << " *";
	}
	
	if (m_from.has_table())
	{
		ss << " FROM ";
		m_from.append_to(ss);
	}
	
	if (!where_clause().is_empty_clause())
	{
		ss << " WHERE " << where_clause();
	}
	
	if (!m_groupBy.is_empty_clause())
	{
		ss << " GROUP BY ";
		m_groupBy.append_to(ss);
	}
	
	if (!m_having.is_empty_clause())
	{
		ss << " HAVING ";
		m_having.append_to(ss);
	}
	
	append_order_by(ss);
	append_limit(ss);
}

std::string CMDSelect::assemble() const
{
	return CMD::assemble();
}

std::vector<BindValue> CMDSelect::bind() const
{
	std::vector<BindValue> final = {};
	
	auto total =
		m_columns.binds_size() + 
		where_clause().binds_size() + 
		m_groupBy.binds_size() + 
		m_having.binds_size() + 
		order_by_clause().binds_size();
	
	if (total == 0)
		return {};
	
	final.reserve(total);
	
	m_columns.append_to(final);
	where_clause().append_to(final);
	m_groupBy.append_to(final);
	m_having.append_to(final);
	order_by_clause().append_to(final);
	
	return final;
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
		data.emplace_back(stmt.column_value_and_step(0));
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
	return execute().all(failsafeLimit);
}
