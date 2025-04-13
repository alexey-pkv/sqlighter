#include "connectors/query_utils.h"


#include <gtest/gtest.h>


TEST(query_utils, sanity)
{
	ASSERT_EQ("",			sqlighter::create_questions(-1));
	ASSERT_EQ("",			sqlighter::create_questions(0));
	ASSERT_EQ("?",			sqlighter::create_questions(1));
	ASSERT_EQ("?, ?",		sqlighter::create_questions(2));
	ASSERT_EQ("?, ?, ?",	sqlighter::create_questions(3));
	ASSERT_EQ("?, ?, ?, ?",	sqlighter::create_questions(4));
}