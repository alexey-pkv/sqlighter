#include "consts/const_cast.h"
#include "exceptions/sqlighter_exceptions.h"


#include <gtest/gtest.h>


TEST(consts, const_cast__OrderByAsc)
{
	std::ostringstream ss;
	
	
	ss << sqlighter::OrderBy::ASC;
	
	
	ASSERT_EQ("ASC", ss.str());
}

TEST(consts, const_cast__OrderByDesc)
{
	std::ostringstream ss;
	
	
	ss << sqlighter::OrderBy::DESC;
	
	
	ASSERT_EQ("DESC", ss.str());
}

TEST(consts, const_cast__OrderBy_InvalidValue__exception_thrown)
{
	try
	{
		std::ostringstream ss;
		ss << (sqlighter::OrderBy)(13);
		FAIL();
	}
	catch (const sqlighter::SQLighterException& e)
	{
		ASSERT_EQ(SQLIGHTER_ERR_INVALID_ENUM, e.code());
	}
}