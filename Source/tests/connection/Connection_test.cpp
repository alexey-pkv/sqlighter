#include "connection/Connection.h"

#include "exceptions/sqlighter_exceptions.h"


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