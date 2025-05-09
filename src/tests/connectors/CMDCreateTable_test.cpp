#include "connectors/CMDCreateTable.h"


#include "core/Stmt.h"
#include "base/connection/IConnection.h"

#include "db_mock.h"
#include "sqlighter.h"
#include "exceptions/sqlighter_exceptions.h"

#include <gtest/gtest.h>


using namespace sqlighter;


class connection_override : public IConnection
{
public:
	bool IS_CALLED = false;
	std::string QUERY {};
	std::vector<BindValue> BIND {};
	
	
	Stmt execute(std::string_view query, const std::vector<BindValue>& values) override
	{
		IS_CALLED = true;
		QUERY = query;
		BIND = values;
		
		return Stmt(nullptr);
	}
	
	Stmt prepare(std::string_view query) override
	{
		IS_CALLED = true;
		QUERY = query;
		BIND = {};
		
		return Stmt();
	}
	
	void execute_prepared(Stmt& stmt, const std::vector<BindValue>& values) override
	{
		IS_CALLED = true;
		QUERY = stmt.query();
		BIND = values;
	}
};


TEST(CMDCreateTable, constructor__connection_is_null__exception_thrown)
{
	try
	{
		CMDCreateTable ct(nullptr);
		FAIL();
	}
	catch (const SQLighterException& e)
	{
		ASSERT_EQ(SQLIGHTER_ERR_GENERIC, e.code());
	}
}


TEST(CMDCreateTable, sanity__bind)
{
	CMDCreateTable ct(std::make_shared<connection_override>());
	
	
	ASSERT_TRUE(ct.bind().empty());
}


TEST(CMDCreateTable, sanity__simple)
{
	CMDCreateTable ct(std::make_shared<connection_override>());
	
	
	ct.table("tbl");
	ct.column_exp("hello");
	
	
	std::ostringstream expected;
	
	expected << "CREATE TABLE tbl (" << std::endl;
	expected << "  hello"			 << std::endl;
	expected << ")";
	
	ASSERT_EQ(expected.str(), ct.assemble());	
}


TEST(CMDCreateTable, sanity__create)
{
	SQLighter sql { setup_db("test_select.db") };
	
	sql.create()
		.table("my_table")
		.column_exp("ID INTEGER PRIMARY KEY")
		.column_exp("Name TEXT NOT NULL")
		.column_exp("Age INTEGER NOT NULL")
		.column_exp("Balance REAL DEFAULT 0.0")
		.execute();
	
	
	sql.execute(
		"INSERT INTO my_table (ID, Name, Age, Balance) VALUES "
		"	(1, 'Alice',	30,	2500.50),   "
		"	(2, 'Bob',		45,	NULL)		"
	);
	
	
	ASSERT_EQ(2, sql.count_rows("my_table"));
}


TEST(CMDCreateTable, execute)
{
	auto conn = std::make_shared<connection_override>();
	CMDCreateTable ct(conn);
	
	
	ct.table("tbl");
	ct.column_exp("hello");
	
	std::ostringstream expected;
	
	expected << "CREATE TABLE tbl (" << std::endl;
	expected << "  hello"			 << std::endl;
	expected << ")";
	
	
	ct.execute();
	
	
	ASSERT_TRUE(conn->IS_CALLED);
	ASSERT_TRUE(conn->BIND.empty());
	ASSERT_EQ(expected.str(), conn->QUERY);
}


TEST(CMDCreateTable, temp)
{
	CMDCreateTable ct(std::make_shared<connection_override>());
	
	
	ct.temp()
		.table("tbl")
		.column_exp("hello");
	
	
	std::ostringstream expected;
	
	expected << "CREATE TEMP TABLE tbl ("	<< std::endl;
	expected << "  hello"					<< std::endl;
	expected << ")";
	
	ASSERT_EQ(expected.str(), ct.assemble());
}

TEST(CMDCreateTable, if_not_exists)
{
	CMDCreateTable ct(std::make_shared<connection_override>());
	
	
	ct.if_not_exists()
		.table("tbl")
		.column_exp("hello");
	
	
	std::ostringstream expected;
	
	expected << "CREATE TABLE IF NOT EXISTS tbl ("	<< std::endl;
	expected << "  hello"							<< std::endl;
	expected << ")";
	
	ASSERT_EQ(expected.str(), ct.assemble());
}

TEST(CMDCreateTable, scheme)
{
	CMDCreateTable ct(std::make_shared<connection_override>());
	
	
	ct.scheme("sc")
		.table("tbl")
		.column_exp("hello");
	
	
	std::ostringstream expected;
	
	expected << "CREATE TABLE sc.tbl ("	<< std::endl;
	expected << "  hello"				<< std::endl;
	expected << ")";
	
	ASSERT_EQ(expected.str(), ct.assemble());
}

TEST(CMDCreateTable, table__with_dot_in_name)
{
	CMDCreateTable ct(std::make_shared<connection_override>());
	
	
	ct
		.table("sc.tbl")
		.column_exp("hello");
	
	
	std::ostringstream expected;
	
	expected << "CREATE TABLE sc.tbl ("	<< std::endl;
	expected << "  hello"				<< std::endl;
	expected << ")";
	
	ASSERT_EQ(expected.str(), ct.assemble());
}

TEST(CMDCreateTable, table__str_str)
{
	CMDCreateTable ct(std::make_shared<connection_override>());
	
	
	ct
		.table("sc", "tbl")
		.column_exp("hello");
	
	
	std::ostringstream expected;
	
	expected << "CREATE TABLE sc.tbl ("	<< std::endl;
	expected << "  hello"				<< std::endl;
	expected << ")";
	
	ASSERT_EQ(expected.str(), ct.assemble());
}

TEST(CMDCreateTable, table)
{
	CMDCreateTable ct(std::make_shared<connection_override>());
	
	
	ct
		.table("my_tbl_name")
		.column_exp("hello");
	
	
	std::ostringstream expected;
	
	expected << "CREATE TABLE my_tbl_name ("	<< std::endl;
	expected << "  hello"						<< std::endl;
	expected << ")";
	
	ASSERT_EQ(expected.str(), ct.assemble());
}

TEST(CMDCreateTable, without_rowid)
{
	CMDCreateTable ct(std::make_shared<connection_override>());
	
	
	ct.temp()
		.table("tbl")
		.column_exp("hello")
		.without_rowid();
	
	
	std::ostringstream expected;
	
	expected << "CREATE TEMP TABLE tbl ("	<< std::endl;
	expected << "  hello"					<< std::endl;
	expected << ") WITHOUT ROWID";
	
	ASSERT_EQ(expected.str(), ct.assemble());
}

TEST(CMDCreateTable, strict)
{
	CMDCreateTable ct(std::make_shared<connection_override>());
	
	
	ct.temp()
		.table("tbl")
		.column_exp("hello")
		.strict();
	
	
	std::ostringstream expected;
	
	expected << "CREATE TEMP TABLE tbl ("	<< std::endl;
	expected << "  hello"					<< std::endl;
	expected << ") STRICT";
	
	ASSERT_EQ(expected.str(), ct.assemble());
}

TEST(CMDCreateTable, all_optiopns)
{
	CMDCreateTable ct(std::make_shared<connection_override>());
	
	
	ct.temp()
		.table("tbl")
		.column_exp("hello")
		.strict()
		.without_rowid();
	
	
	std::ostringstream expected;
	
	expected << "CREATE TEMP TABLE tbl ("	<< std::endl;
	expected << "  hello"					<< std::endl;
	expected << ") WITHOUT ROWID, STRICT";
	
	ASSERT_EQ(expected.str(), ct.assemble());
}


// We expect SQLite to throw the exception, as theoretically it's a syntax error. 
// Syntax errors should not be handled directly bt the library.
TEST(CMDCreateTable, no_columns__no_exception)
{
	CMDCreateTable ct(std::make_shared<connection_override>());
	
	
	ct.table("tbl");
	
	
	std::ostringstream expected;
	
	expected << "CREATE TABLE tbl (" << std::endl;
	expected << std::endl << ")";
	
	ASSERT_EQ(expected.str(), ct.assemble());
}

TEST(CMDCreateTable, multiple_columns)
{
	CMDCreateTable ct(std::make_shared<connection_override>());
	
	
	ct
		.table("my_tbl_name")
		.column_exp("hello")
		.column_exp("world")
		.column_exp("field INTEGER NULL");
	
	
	std::ostringstream expected;
	
	expected << "CREATE TABLE my_tbl_name ("	<< std::endl;
	expected << "  hello,"						<< std::endl;
	expected << "  world,"						<< std::endl;
	expected << "  field INTEGER NULL"			<< std::endl;
	expected << ")";
	
	ASSERT_EQ(expected.str(), ct.assemble());
}