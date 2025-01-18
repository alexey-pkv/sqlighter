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

TEST(consts, const_cast__ScalarValue_type__invalid_value__exception_thrown)
{
	try
	{
		std::ostringstream ss;
		ss << (sqlighter::ScalarValue::type)(13);
		FAIL();
	}
	catch (const sqlighter::SQLighterException& e)
	{
		ASSERT_EQ(SQLIGHTER_ERR_INVALID_ENUM, e.code());
	}
}

TEST(consts, const_cast__ScalarValue_type)
{
	std::ostringstream s1;
	std::ostringstream s2;
	std::ostringstream s3;
	std::ostringstream s4;
	std::ostringstream s5;
	
	
	s1 << sqlighter::ScalarValue::type::INT;
	s2 << sqlighter::ScalarValue::type::DOUBLE;
	s3 << sqlighter::ScalarValue::type::NULL_VAL;
	s4 << sqlighter::ScalarValue::type::TEXT;
	s5 << sqlighter::ScalarValue::type::BLOB;
	
	
	ASSERT_EQ("INT_64",		s1.str());
	ASSERT_EQ("DOUBLE",		s2.str());
	ASSERT_EQ("NULL_VAL",	s3.str());
	ASSERT_EQ("TEXT",		s4.str());
	ASSERT_EQ("BLOB",		s5.str());
}