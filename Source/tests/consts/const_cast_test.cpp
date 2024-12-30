#include "consts/const_cast.h"


#include <gtest/gtest.h>


TEST(consts, const_cast__OrderByAsc)
{
	std::stringstream ss;
	
	
	ss << sqlighter::OrderBy::ASC;
	
	
	ASSERT_EQ("ASC", ss.str());
}

TEST(consts, const_cast__OrderByDesc)
{
	std::stringstream ss;
	
	
	ss << sqlighter::OrderBy::DESC;
	
	
	ASSERT_EQ("DESC", ss.str());
}