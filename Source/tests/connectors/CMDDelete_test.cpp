#include "connectors/CMDDelete.h"


#include "connection_override.h"
#include "exceptions/sqlighter_exceptions.h"
#include "sqlighter.h"
#include "db_mock.h"

#include <gtest/gtest.h>


using namespace sqlighter;


TEST(CMDDelete, from)
{
	CMDDelete cmd { get_co() };
	
	
	ASSERT_EQ("DELETE FROM my_table",		cmd.from("my_table").assemble());
	ASSERT_EQ("DELETE FROM temp.my_table",	cmd.from("temp.my_table").assemble());
	ASSERT_EQ("DELETE FROM temp.my_table",	cmd.from("temp", "my_table").assemble());
}


TEST(CMDDelete, as)
{
	CMDDelete cmd { get_co() };
	
	
	cmd.from("my_table").as("as");
	
	
	ASSERT_EQ("DELETE FROM my_table AS as", cmd.assemble());
}


TEST(CMDDelete, bind__none__empty)
{
	CMDDelete cmd { get_co() };
	
	
	cmd.from("my_table").where("A = B");
	
	
	ASSERT_TRUE(cmd.bind().empty());
}

TEST(CMDDelete, bind)
{
	CMDDelete cmd { get_co() };
	
	
	cmd.from("my_table").where("A = ?", 23);
	
	
	ASSERT_FALSE(cmd.bind().empty());
	ASSERT_EQ(1, cmd.bind().size());
	ASSERT_EQ(23, cmd.bind()[0].get_value().i32);
}


TEST(CMDDelete, sanity)
{
	SQLighter sql { setup_db("test_select.db") };
	
	
	sql.execute(
		"CREATE TABLE ExampleTable (                     		"
		"	Name			TEXT NOT NULL PRIMARY KEY,		    "
		"	Age				INTEGER,            			    "
		"	Balance			REAL,                  			 	"
		"	IsActive		BOOLEAN DEFAULT 1, 			     	"
		"	NullableField	BLOB	                 			"
		")"
	);
	
	sql.execute(
		"INSERT INTO ExampleTable (Name, Age, Balance, IsActive) VALUES "
		"	('Alice',	30,		2500.50,	1), "
		"	('Bob',		NULL,	1500.00,	0),	"
		"	('Jack',	NULL,	1544.00,	0),	"
		"	('Mick',	25,		2123.00,	0),	"
		"	('Dora',	31,		9999.00,	0)	"	
	);
	
	
	ASSERT_EQ(5, sql.count_rows("ExampleTable"));
	
	
	sql.del()
		.from("ExampleTable")
		.where("Age <= ?", 30)
		.execute();
	
	
	ASSERT_EQ(3, sql.count_rows("ExampleTable"));
	ASSERT_EQ(2, sql.select().from("ExampleTable").where_null("Age").query_count());
	ASSERT_EQ(1, sql.select().from("ExampleTable").by_field("Age", 31).query_count());
}