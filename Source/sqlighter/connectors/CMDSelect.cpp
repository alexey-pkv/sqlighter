#include "CMDSelect.h"


#include "query_utils.h"

#include "../consts/const_cast.h"
#include "../core/Stmt.h"
#include "../base/connection/IConnection.h"


using namespace sqlighter;


/******************************************/
/******* Helper Functions *****************/
/******************************************/


void append_delimiter(std::stringstream& stream, bool& has, std::string_view delimiter)
{
	if (has) 
	{
		stream << delimiter;
	}
	else 
	{
		has = true;
	}
}

inline void append_comma(std::stringstream& stream, bool& has)
{
	append_delimiter(stream, has, ", ");
}

inline void append_AND(std::stringstream& stream, bool& has)
{
	append_delimiter(stream, has, " AND ");
}

inline void append_field_comma(std::stringstream& stream, bool& has, std::string_view field)
{
	append_comma(stream, has);
	wrap_element(stream, field);
}

inline void append_field_AND(std::stringstream& stream, bool& has, std::string_view field)
{
	append_AND(stream, has);
	wrap_element(stream, field);
}

void append_expression(
	std::stringstream&			stream,
	std::vector<BindValue>&		values,
	bool&						has,
	std::string_view			exp,
	std::span<const BindValue>	span,
	std::string_view			delimiter)
{
	append_delimiter(stream, has, delimiter);
	stream << exp;
	
	if (span.size() == 1)
	{
		values.emplace_back(span[0]);
	}
	else if (!span.empty())
	{
		values.insert(values.end(), span.begin(), span.end());
	}
}

/************************************/
/******* CMD Select *****************/
/************************************/


CMDSelect::CMDSelect(const CMDSelect& cmd)
{
	*this = cmd;
}

CMDSelect& CMDSelect::operator=(const CMDSelect& cmd)
{
	if (&cmd == this)
		return *this;
	
	m_connection = cmd.m_connection;
	
	m_distinct		= cmd.m_distinct;
	m_from			= cmd.m_from;
	m_hasLimit		= cmd.m_hasLimit;
	m_limitOffset 	= cmd.m_limitOffset;
	m_limitCount 	= cmd.m_limitCount;
	
	m_hasColumns	= cmd.m_hasColumns;
	m_hasWhere		= cmd.m_hasWhere;
	m_hasGroupBy	= cmd.m_hasGroupBy;
	m_hasHaving		= cmd.m_hasHaving;
	m_hasOrderBy	= cmd.m_hasOrderBy;
	
	m_columnValues	= cmd.m_columnValues;
	m_whereValues	= cmd.m_whereValues;
	m_groupByValues	= cmd.m_groupByValues;
	m_havingValues	= cmd.m_havingValues;
	m_orderByValues	= cmd.m_orderByValues;
	
	m_columns.clear();
	m_where.clear();
	m_groupBy.clear();
	m_having.clear();
	m_orderBy.clear();
	
	if (m_hasColumns)	m_columns << cmd.m_hasColumns;
	if (m_hasWhere)		m_where << cmd.m_hasWhere;
	if (m_hasGroupBy)	m_groupBy << cmd.m_hasGroupBy;
	if (m_hasHaving)	m_having << cmd.m_hasHaving;
	if (m_hasOrderBy)	m_orderBy << cmd.m_hasOrderBy;
	
	return *this;
}

CMDSelect::CMDSelect(const std::shared_ptr<IConnection>& connection) : 
	m_connection(connection)
{
	if (connection == nullptr)
	{
		throw std::runtime_error("connection should not be null!");
	}
}


void CMDSelect::append_column_exp(std::string_view exp, std::span<const BindValue> span)
{
	append_expression(m_columns, m_columnValues, m_hasColumns, exp, span, ", ");
}

void CMDSelect::append_where(std::string_view exp, std::span<const BindValue> span)
{
	append_expression(m_where, m_whereValues, m_hasWhere, exp, span, " AND ");
}

void CMDSelect::append_group_by(std::string_view exp, std::span<const BindValue> span)
{
	append_expression(m_groupBy, m_groupByValues, m_hasGroupBy, exp, span, ", ");
}

void CMDSelect::append_having(std::string_view exp, std::span<const BindValue> span)
{
	append_expression(m_having, m_havingValues, m_hasHaving, exp, span, " AND ");
}

void CMDSelect::append_order_by(std::string_view exp, std::span<const BindValue> span)
{
	append_expression(m_orderBy, m_orderByValues, m_hasOrderBy, exp, span, ", ");
}


void CMDSelect::distinct()
{
	m_distinct = true;
}


void CMDSelect::column(std::string_view column)
{
	append_field_comma(m_columns, m_hasColumns, column);
}

void CMDSelect::column_as(std::string_view column, std::string_view as)
{
	append_delimiter(m_columns, m_hasColumns, ", ");
	wrap_element(m_columns, column, as);
}

void CMDSelect::column_as(std::string_view column, char as)
{
	append_delimiter(m_columns, m_hasColumns, ", ");
	wrap_element(m_columns, column, { &as, 1 });
}

void CMDSelect::columns(const std::vector<std::string_view>& columns)
{
	for (const auto& c : columns)
	{
		column(c);
	}
}

void CMDSelect::columns(std::initializer_list<std::string_view> columns)
{
	for (const auto& c : columns)
	{
		column(c);
	}
}


void CMDSelect::from(std::string_view table)
{
	m_from = wrap_element(table);
}

void CMDSelect::from(std::string_view table, char alias)
{
	m_from = wrap_element(table, { &alias, 1 });
}

void CMDSelect::from(std::string_view table, std::string_view alias)
{
	m_from = wrap_element(table, alias);
}

void CMDSelect::where_null(std::string_view column)
{
	append_field_AND(m_where, m_hasWhere, column);
	m_where << " IS NULL";
}

void CMDSelect::where_not_null(std::string_view column)
{
	append_AND(m_where, m_hasWhere);
	wrap_element(m_where, column);
	
	m_where << " IS NOT NULL";
}

void CMDSelect::by_field(std::string_view column, BindValue value)
{
	append_AND(m_where, m_hasWhere);
	wrap_element(m_where, column);
	
	m_where << " = ?";
	
	m_whereValues.emplace_back(value);
}

void CMDSelect::group_by_field(std::string_view by)
{
	append_comma(m_groupBy, m_hasGroupBy);
	wrap_element(m_groupBy, by);
}


void CMDSelect::order_by_field(std::string_view by)
{
	append_comma(m_orderBy, m_hasOrderBy);
	wrap_element(m_orderBy, by);
}

void CMDSelect::order_by_field(std::string_view by, OrderBy order)
{
	append_comma(m_orderBy, m_hasOrderBy);
	wrap_element(m_orderBy, by);
	
	if (order == OrderBy::DESC)
	{
		m_orderBy << ' ' << order;
	}
}


void CMDSelect::limit_by(int count)
{
	m_hasLimit = true;
	
	m_limitCount = count;
	m_limitOffset = 0;
}

void CMDSelect::limit(int offset, int count)
{
	m_hasLimit = true;
	
	m_limitCount = count;
	m_limitOffset = offset;
}


void CMDSelect::assemble(std::stringstream& ss) const
{
	ss << "SELECT";
	
	if (m_distinct)
	{
		ss << " DISTINCT";
	}
	
	if (m_hasColumns)
	{
		ss << ' ' << m_columns.str();
	}
	else
	{
		ss << " *";
	}
	
	if (!m_from.empty())
	{
		ss << " FROM " << m_from;
	}
	
	if (m_hasWhere)
	{
		ss << " WHERE " << m_where.str();
	}
	
	if (m_hasGroupBy)
	{
		ss << " GROUP BY " << m_groupBy.str();
	}
	
	if (m_hasHaving)
	{
		ss << " HAVING " << m_having.str();
	}
	
	if (m_hasOrderBy)
	{
		ss << " ORDER BY " << m_orderBy.str();
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
	std::stringstream ss;
	
	assemble(ss);
	
	return ss.str();
}

std::vector<BindValue> CMDSelect::bind() const
{
	std::vector<BindValue> final = {};
	
	auto total =
		m_columnValues.size() + 
		m_whereValues.size() + 
		m_groupByValues.size() + 
		m_havingValues.size() + 
		m_orderByValues.size();
	
	if (total == 0)
		return {};
	
	final.reserve(total);
	
	final.insert(final.end(), m_columnValues.begin(), m_columnValues.end());
	final.insert(final.end(), m_whereValues.begin(), m_whereValues.end());
	final.insert(final.end(), m_groupByValues.begin(), m_groupByValues.end());
	final.insert(final.end(), m_havingValues.begin(), m_havingValues.end());
	final.insert(final.end(), m_orderByValues.begin(), m_orderByValues.end());
	
	return final;
}


Stmt CMDSelect::execute() const
{
	return m_connection->execute(
		assemble(),
		bind()
	);
}
