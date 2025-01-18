#include "connectors/CMDInsert.h"


#include "db_mock.h"
#include "sqlighter.h"
#include "connection_override.h"
#include "base/connection/IConnection.h"
#include "exceptions/sqlighter_exceptions.h"

#include <gtest/gtest.h>


using namespace sqlighter;


TEST(CMDInsert, sanity)
{
	CMDInsert cmd { get_co() };
	
	
	cmd
		.into("my_table")
		.columns({ "my_column" })
		.record({ 1 });
	
	
	std::ostringstream ss;
	
	ss	<< "INSERT INTO `my_table` (`my_column`) VALUES " << std::endl 
		<< "(?)" << std::endl;
	
	ASSERT_EQ(ss.str(), cmd.assemble());
	ASSERT_EQ(1, cmd.bind().size());
	ASSERT_EQ(1, cmd.bind()[0].get_value().i32);
}


TEST(CMDInsert, no_columns)
{
	CMDInsert cmd { get_co() };
	
	
	cmd
		.into("my_table")
		.record({ 1, 2, 3 });
	
	
	std::ostringstream ss;
	
	ss	<< "INSERT INTO `my_table` VALUES " << std::endl 
		<< "(?, ?, ?)" << std::endl;
	
	ASSERT_EQ(ss.str(), cmd.assemble());
}

TEST(CMDInsert, mult_columns)
{
	CMDInsert cmd { get_co() };
	
	
	cmd
		.into("my_table")
		.column("a")
		.columns({ "b", "c" })
		.record({ 1, 2, 3 });
	
	
	std::ostringstream ss;
	
	ss	<< "INSERT INTO `my_table` (`a`, `b`, `c`) VALUES " << std::endl 
		<< "(?, ?, ?)" << std::endl;
	
	ASSERT_EQ(ss.str(), cmd.assemble());
}

TEST(CMDInsert, default_values)
{
	CMDInsert cmd { get_co() };
	
	
	cmd
		.into("my_table")
		.column("my_column")
		.default_values();
	
	
	ASSERT_EQ("INSERT INTO `my_table` (`my_column`) DEFAULT VALUES", cmd.assemble());
}

TEST(CMDInsert, on_conflict_do_nothing)
{
	CMDInsert cmd { get_co() };
	
	
	cmd
		.into("my_table")
		.column("my_column")
		.record({ 1 })
		.on_conflict_do_nothing();
	
	
	
	std::ostringstream ss;
	
	ss	<< "INSERT INTO `my_table` (`my_column`) VALUES " << std::endl 
		<< "(?)" << std::endl
		<< "ON CONFLICT DO NOTHING";
	
	ASSERT_EQ(ss.str(), cmd.assemble());
}


TEST(CMDInsert, or_abort)
{
	CMDInsert cmd { get_co() };
	
	
	cmd.into("my_table").or_abort().default_values();
	
	
	ASSERT_EQ("INSERT OR ABORT INTO `my_table` DEFAULT VALUES", cmd.assemble());
}

TEST(CMDInsert, or_ignore)
{
	CMDInsert cmd { get_co() };
	
	
	cmd.into("my_table").or_ignore().default_values();
	
	
	ASSERT_EQ("INSERT OR IGNORE INTO `my_table` DEFAULT VALUES", cmd.assemble());
}

TEST(CMDInsert, or_fail)
{
	CMDInsert cmd { get_co() };
	
	
	cmd.into("my_table").or_fail().default_values();
	
	
	ASSERT_EQ("INSERT OR FAIL INTO `my_table` DEFAULT VALUES", cmd.assemble());
}

TEST(CMDInsert, or_rollback)
{
	CMDInsert cmd { get_co() };
	
	
	cmd.into("my_table").or_rollback().default_values();
	
	
	ASSERT_EQ("INSERT OR ROLLBACK INTO `my_table` DEFAULT VALUES", cmd.assemble());
}

TEST(CMDInsert, or_replace)
{
	CMDInsert cmd { get_co() };
	
	
	cmd.into("my_table").or_replace().default_values();
	
	
	ASSERT_EQ("INSERT OR REPLACE INTO `my_table` DEFAULT VALUES", cmd.assemble());
}

TEST(CMDInsert, scheme)
{
	CMDInsert cmd { get_co() };
	
	
	cmd.into("sc", "my_table").default_values();
	
	
	ASSERT_EQ("INSERT INTO `sc`.`my_table` DEFAULT VALUES", cmd.assemble());
}

TEST(CMDInsert, scheme_in_name)
{
	CMDInsert cmd { get_co() };
	
	
	cmd.into("sc.my_table").default_values();
	
	
	ASSERT_EQ("INSERT INTO `sc`.`my_table` DEFAULT VALUES", cmd.assemble());
}

TEST(CMDInsert, as)
{
	CMDInsert cmd { get_co() };
	
	
	cmd.into("my_table").as("something").default_values();
	
	
	ASSERT_EQ("INSERT INTO `my_table` AS `something` DEFAULT VALUES", cmd.assemble());
}


TEST(CMDInsert, default_values__with_records_error_thrown)
{
	CMDInsert cmd { get_co() };
	
	
	cmd
		.into("my_table")
		.column("my_column")
		.default_values()
		.record({ 1 })
		.record({ 2 });
	
	
	try
	{
		cmd.assemble();
		FAIL();
	}
	catch (const SQLighterException& e)
	{
		ASSERT_EQ(SQLIGHTER_ERR_GENERIC, e.code());
	}
}

TEST(CMDInsert, unknown_number_of_columns__error_thrown)
{
	CMDInsert cmd { get_co() };
	
	
	cmd.into("my_table");
	
	
	try
	{
		cmd.assemble();
		FAIL();
	}
	catch (const SQLighterException& e)
	{
		ASSERT_EQ(SQLIGHTER_ERR_GENERIC, e.code());
	}
}

TEST(CMDInsert, records)
{
	CMDInsert cmd { get_co() };
	
	auto a = std::vector<BindValue> { 7, 8, 9 };
	auto b = std::vector<BindValue> { "a", "b", "c" };
	
	
	cmd.into("my_table").columns({ "a", "b", "c" });
	
	cmd.record({1, 2, 3});
	cmd.record(std::vector<BindValue>({4, 5, 6}));
	cmd.records({ { bind(11), bind(12), bind(13) } });
	cmd.records({ a, b });
	cmd.records(std::vector<std::vector<BindValue>> { a, b });
	cmd.records({ std::vector<BindValue> { "A", "B", "C" }, { "D", "E", "F" } });
	
	
	std::ostringstream ss;
	
	ss	<< "INSERT INTO `my_table` (`a`, `b`, `c`) VALUES " << std::endl 
		<< "(?, ?, ?), " << std::endl
		<< "(?, ?, ?), " << std::endl
		<< "(?, ?, ?), " << std::endl
		<< "(?, ?, ?), " << std::endl
		<< "(?, ?, ?), " << std::endl
		<< "(?, ?, ?), " << std::endl
		<< "(?, ?, ?), " << std::endl
		<< "(?, ?, ?), " << std::endl
		<< "(?, ?, ?)" << std::endl;
	
	ASSERT_EQ(ss.str(), cmd.assemble());
	ASSERT_EQ(27, cmd.bind().size());
	
	auto dd = cmd.bind();
	
	ASSERT_EQ(1, cmd.bind()[0].get_value().i32);
	ASSERT_EQ(2, cmd.bind()[1].get_value().i32);
	ASSERT_EQ(3, cmd.bind()[2].get_value().i32);
	
	ASSERT_EQ(4, cmd.bind()[3].get_value().i32);
	ASSERT_EQ(5, cmd.bind()[4].get_value().i32);
	ASSERT_EQ(6, cmd.bind()[5].get_value().i32);
	
	ASSERT_EQ(11, cmd.bind()[6].get_value().i32);
	ASSERT_EQ(12, cmd.bind()[7].get_value().i32);
	ASSERT_EQ(13, cmd.bind()[8].get_value().i32);
	
	ASSERT_EQ(7, cmd.bind()[9].get_value().i32);
	ASSERT_EQ(8, cmd.bind()[10].get_value().i32);
	ASSERT_EQ(9, cmd.bind()[11].get_value().i32);
	
	ASSERT_EQ("a", cmd.bind()[12].get_str_value());
	ASSERT_EQ("b", cmd.bind()[13].get_str_value());
	ASSERT_EQ("c", cmd.bind()[14].get_str_value());
	
	ASSERT_EQ(7, cmd.bind()[15].get_value().i32);
	ASSERT_EQ(8, cmd.bind()[16].get_value().i32);
	ASSERT_EQ(9, cmd.bind()[17].get_value().i32);
	
	ASSERT_EQ("a", cmd.bind()[18].get_str_value());
	ASSERT_EQ("b", cmd.bind()[19].get_str_value());
	ASSERT_EQ("c", cmd.bind()[20].get_str_value());
	
	ASSERT_EQ("A", cmd.bind()[21].get_str_value());
	ASSERT_EQ("B", cmd.bind()[22].get_str_value());
	ASSERT_EQ("C", cmd.bind()[23].get_str_value());
	
	ASSERT_EQ("D", cmd.bind()[24].get_str_value());
	ASSERT_EQ("E", cmd.bind()[25].get_str_value());
	ASSERT_EQ("F", cmd.bind()[26].get_str_value());
}

TEST(CMDUpdate, sanity__on_conflict_do_nothing)
{
	SQLighter sql { setup_db("test_select.db") };
	
	sql.execute(
		"CREATE TABLE ExampleTable (                     		"
		"	Name			TEXT NOT NULL PRIMARY KEY,		    "
		"	Age				INTEGER,            			    "
		"	Balance			REAL,                  			 	"
		"	IsActive		BOOLEAN DEFAULT 1        			"
		")"
	);
	
	sql.execute(
		"INSERT INTO ExampleTable (Name, Age, Balance, IsActive) VALUES "
		"	('Alice',	30,		2500.50,	1), "
		"	('Bob',		NULL,	1500.00,	0)	"	
	);
	
	
	// Control Test: Make sure it actually fails. 
	try
	{
		sql.insert()
			.into("ExampleTable")
			.columns({ "Name", "Age", "Balance", "IsActive" })
			.record({ "Alice", 60, 1.1, 0 })
			.execute();
		
		FAIL();
	}
	catch (const SQLighterException& e)
	{
		ASSERT_EQ(SQLIGHTER_ERR_STEP, e.code());
	}
	
	sql.insert()
		.into("ExampleTable")
		.columns({ "Name", "Age", "Balance", "IsActive" })
		.record({ "Alice", 60, 1.1, false })
		.on_conflict_do_nothing()
		.execute();
	
	ASSERT_EQ(2, sql.count_rows("ExampleTable"));
	ASSERT_EQ(1, sql.select()
		.from("ExampleTable")
		.by_field("Name", "Alice")
		.by_field("Age", 30)
		.by_field("Balance", 2500.50)
		.by_field("IsActive", true)
		.query_count());
}

TEST(CMDUpdate, sanity__on_conflict_update)
{
	SQLighter sql { setup_db("test_select.db") };
	
	sql.execute(
		"CREATE TABLE ExampleTable (                     		"
		"	Name			TEXT NOT NULL PRIMARY KEY,		    "
		"	Age				INTEGER,            			    "
		"	Balance			REAL,                  			 	"
		"	IsActive		BOOLEAN DEFAULT 1        			"
		")"
	);
	
	sql.execute(
		"INSERT INTO ExampleTable (Name, Age, Balance, IsActive) VALUES "
		"	('Alice',	30,		2500.50,	1), "
		"	('Bob',		NULL,	1500.00,	0)	"	
	);
	
	
	// Control Test: Make sure it actually fails.
	try
	{
		sql.insert()
			.into("ExampleTable")
			.columns({ "Name", "Age", "Balance", "IsActive" })
			.record({ "Alice", 60, 1.1, 0 })
			.execute();
		
		FAIL();
	}
	catch (const SQLighterException& e)
	{
		ASSERT_EQ(SQLIGHTER_ERR_STEP, e.code());
	}
	
	
	sql.insert()
		.into("ExampleTable")
		.columns({ "Name", "Age", "Balance", "IsActive" })
		.record({ "Alice", 60, 1.1, false })
		.on_conflict("Name")
		.set("Age", 90)
		.execute();
	
	
	ASSERT_EQ(2, sql.count_rows("ExampleTable"));
	ASSERT_EQ(1, sql.select()
		.from("ExampleTable")
		.by_field("Name", "Alice")
		.by_field("Age", 90)
		.by_field("Balance", 2500.50)
		.by_field("IsActive", true)
		.query_count());
}