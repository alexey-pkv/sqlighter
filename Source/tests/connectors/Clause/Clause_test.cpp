#include "connectors/Clause/Clause.h"


#include <gtest/gtest.h>


using namespace sqlighter;


TEST(Clause, append__no_delimiter)
{
	Clause c(nullptr);
	
	
	c.append("a");
	c.append("b");
	
	
	ASSERT_EQ("ab", c.get_clause_string());
}

TEST(Clause, append__empty_string__nothing_added)
{
	Clause c(", ");
	
	
	c.append("a");
	c.append("");
	
	ASSERT_EQ("a", c.get_clause_string());
	
	
	c.append("b");
	
	ASSERT_EQ("a, b", c.get_clause_string());
}

TEST(Clause, append__delimiter)
{
	Clause c(", ");
	
	
	c.append("a");
	c.append("b");
	
	
	ASSERT_EQ("a, b", c.get_clause_string());
}

TEST(Clause, append_directly__delimiter_ignored)
{
	Clause c(", ");
	
	
	c.append("a");
	c.append_directly("b");
	c.append_directly("c", { 2 });
	c.append_directly("c", std::vector<BindValue> { 2 });
	
	
	ASSERT_EQ("abcc", c.get_clause_string());
}

TEST(Clause, append_directly__next_value_will_have_delimiter)
{
	Clause c(", ");
	
	
	c.append_directly("a");
	c.append("b");
	
	
	ASSERT_EQ("a, b", c.get_clause_string());
}

TEST(Clause, append_directly__first_string_is_empty__delimiter_skipped)
{
	Clause c(", ");
	
	
	c.append_directly("");
	c.append("b");
	
	
	ASSERT_EQ("b", c.get_clause_string());
}

TEST(Clause, clear)
{
	Clause c(", ");
	
	c.append("hello");
	c.append("world");
	c.append_bind(123);
	
	c.clear();
	
	ASSERT_TRUE(c.empty_clause());
	ASSERT_TRUE(!c.has_binds());
	ASSERT_TRUE(c.get_clause_string().empty());
}