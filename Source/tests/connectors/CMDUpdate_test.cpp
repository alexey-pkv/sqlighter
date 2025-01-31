#include "connectors/CMDUpdate.h"


#include "db_mock.h"
#include "sqlighter.h"
#include "connection_override.h"
#include "base/connection/IConnection.h"
#include "exceptions/sqlighter_exceptions.h"

#include <gtest/gtest.h>


using namespace sqlighter;


TEST(CMDUpdate, or_)
{
	CMDUpdate cmd { get_co() };
	
	
	ASSERT_EQ("UPDATE OR ABORT my_table",		cmd.table("my_table").or_abort().assemble());
	ASSERT_EQ("UPDATE OR FAIL my_table",		cmd.table("my_table").or_fail().assemble());
	ASSERT_EQ("UPDATE OR IGNORE my_table",	cmd.table("my_table").or_ignore().assemble());
	ASSERT_EQ("UPDATE OR REPLACE my_table",	cmd.table("my_table").or_replace().assemble());
	ASSERT_EQ("UPDATE OR ROLLBACK my_table",	cmd.table("my_table").or_rollback().assemble());
}

TEST(CMDUpdate, table)
{
	CMDUpdate cmd { get_co() };
	
	
	ASSERT_EQ("UPDATE my_table",			cmd.table("my_table").assemble());
	ASSERT_EQ("UPDATE hello.world",		cmd.table("hello.world").assemble());
	ASSERT_EQ("UPDATE scheme.my_table",	cmd.table("scheme", "my_table").assemble());
}

TEST(CMDUpdate, as)
{
	CMDUpdate cmd { get_co() };
	
	
	ASSERT_EQ("UPDATE hello AS world",	cmd.table("hello").as("world").assemble());
}

TEST(CMDUpdate, sanity)
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
	
	
	ASSERT_FALSE(sql
		.select()
		.column("IsActive")
		.from("ExampleTable")
		.by_field("Name", "Bob")
		.query_bool());
	
	ASSERT_FALSE(sql
		.select()
		.column("IsActive")
		.from("ExampleTable")
		.by_field("Name", "Jack")
		.query_bool());
	
	
	sql.update()
		.table("ExampleTable")
		.set("IsActive", true)
		.where_null("Age")
		.execute();
	
	
	ASSERT_TRUE(sql
		.select()
		.column("IsActive")
		.from("ExampleTable")
		.by_field("Name", "Bob")
		.query_bool());
	
	ASSERT_TRUE(sql
		.select()
		.column("IsActive")
		.from("ExampleTable")
		.by_field("Name", "Jack")
		.query_bool());
	
	ASSERT_EQ(3, sql.select().from("ExampleTable").by_field("IsActive", true).query_count());
}