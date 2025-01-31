#include "connectors/Clause/ClauseSet.h"


#include <sstream>
#include <gtest/gtest.h>


using namespace sqlighter;


class test_ClauseSet_Helper : public ClauseSet<test_ClauseSet_Helper>
{
public:
	void append_binds(std::vector<BindValue>& to)
	{
		set_clause().append_to(to);
	}
	
	void append_query(std::stringstream& to)
	{
		set_clause().append_to(to);
	}
	
	int binds_size() const
	{
		return set_clause().binds_size();
	}
};


TEST(ClauseSet, set_field)
{
	std::vector<BindValue> binds = {}; 
	std::stringstream ss = {};
	test_ClauseSet_Helper cs = {};
	
	cs.set("Some field", 32);
	cs.set("Hello", bind("World"));
	
	
	cs.append_binds(binds);
	cs.append_query(ss);
	
	
	ASSERT_EQ("Some field = ?, Hello = ?", ss.str());
	ASSERT_EQ(2, cs.binds_size());
	ASSERT_EQ(2, binds.size());
	ASSERT_EQ(32, binds[0].get_value().i32);
	ASSERT_EQ("World", binds[1].get_str_value());
}
