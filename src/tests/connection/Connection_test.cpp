#include "connection/Connection.h"


#include "exceptions/sqlighter_exceptions.h"
#include "db_mock.h"

#include <gtest/gtest.h>


using namespace sqlighter;


TEST(Connection, constructor__null_passed__exception_thrown)
{
	try
	{
		Connection c((std::shared_ptr<DB>)nullptr);
		FAIL();
	}
	catch (const SQLighterException& e)
	{
		ASSERT_EQ(SQLIGHTER_ERR_UNEXPECTED, e.code());
	}
}

TEST(Connection, constructor__string__sanity)
{
	Connection c("some path");
	
	
	ASSERT_EQ("some path", c.db()->path());
}

TEST(Connection, execute__empty_query__exception_thrown)
{
	Connection c("some path");
	
	try
	{
		c.execute("", { 1 });
		FAIL();
	}
	catch (const SQLighterException& e)
	{
		ASSERT_EQ(SQLIGHTER_ERR_EMPTY_QUERY, e.code());
	}
}

TEST(Connection, execute__connection_closed__exception_thrown)
{
	Connection c(setup_db("test_select.db"));
	c.db()->close();
	
	
	try
	{
		c.execute("SELECT 1", {});
	}
	catch (const SQLighterException& e)
	{
		ASSERT_EQ(SQLIGHTER_ERR_CONNECTION_WAS_CLOSED, e.code());
	}
}