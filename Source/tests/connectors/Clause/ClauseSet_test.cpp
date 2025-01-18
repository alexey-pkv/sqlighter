#include "connectors/Clause/ClauseSet.h"


#include <sstream>
#include <gtest/gtest.h>


using namespace sqlighter;


TEST(ClauseSet, set_field)
{
	std::vector<BindValue> binds = {}; 
	std::stringstream ss = {};
	ClauseSet cs = {};
	
	cs.set_field("Some field", 32);
	cs.set_field("Hello", bind("World"));
	
	
	cs.append_binds(binds);
	cs.append_to(ss);
	
	
	ASSERT_EQ(" SET `Some field` = ?, `Hello` = ?", ss.str());
	ASSERT_EQ(2, cs.binds_size());
	ASSERT_EQ(2, binds.size());
	ASSERT_EQ(32, binds[0].get_value().i32);
	ASSERT_EQ("World", binds[1].get_str_value());
}
