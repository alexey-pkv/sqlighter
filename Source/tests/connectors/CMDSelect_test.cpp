#include "connectors/CMDSelect.h"


#include "core/Stmt.h"
#include "base/connection/IConnection.h"

#include "db_mock.h"
#include "sqlighter.h"

#include <gtest/gtest.h>


using namespace sqlighter;


class connection_override : public IConnection
{
public:
	Stmt execute(std::string_view query, const std::vector<BindValue>& values) override
	{
		return Stmt(nullptr);
	}
};


TEST(CMDSelect, constructor__connection_is_null__exception_thrown)
{
	try
	{
		CMDSelect select(nullptr);
		FAIL();
	}
	catch (const std::runtime_error& e)
	{
		ASSERT_EQ("connection should not be null!", std::string(e.what()));
	}
}

TEST(CMDSelect, sanity)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	ASSERT_EQ("SELECT *", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
}


TEST(CMDSelect, distinct)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.distinct();
	
	
	ASSERT_EQ("SELECT DISTINCT *", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
}


TEST(CMDSelect, from)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.from("table");
	
	
	ASSERT_EQ("SELECT * FROM `table`", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
}

TEST(CMDSelect, from__with_alias_char)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.from("table", 't');
	
	
	ASSERT_EQ("SELECT * FROM `table` as `t`", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
}

TEST(CMDSelect, from__with_alias_str)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.from("table", "tab");
	
	
	ASSERT_EQ("SELECT * FROM `table` as `tab`", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
}


TEST(CMDSelect, limit)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.limit(0, 0);
	ASSERT_EQ("SELECT * LIMIT 0", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
	
	cmd.limit(0, 1);
	ASSERT_EQ("SELECT * LIMIT 1", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
	
	cmd.limit(1, 0);
	ASSERT_EQ("SELECT * LIMIT 1, 0", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
	
	cmd.limit(1, 10);
	ASSERT_EQ("SELECT * LIMIT 1, 10", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
	
	// Validate this is no exceptions, the query will still fail, but the call to limit should not.
	cmd.limit(-1, -1);
	ASSERT_EQ("SELECT * LIMIT -1, -1", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
}

TEST(CMDSelect, limit__after_limit_by__limit_reset)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.limit_by(20);
	cmd.limit(10, 12);
	
	
	ASSERT_EQ("SELECT * LIMIT 10, 12", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
}

TEST(CMDSelect, limit__after_page__limit_reset)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.page(7, 3);
	cmd.limit(10, 12);
	
	
	ASSERT_EQ("SELECT * LIMIT 10, 12", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
}


TEST(CMDSelect, limit_by)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.limit_by(0);
	ASSERT_EQ("SELECT * LIMIT 0", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
	
	cmd.limit_by(2);
	ASSERT_EQ("SELECT * LIMIT 2", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
	
	// Validate this is no exceptions, the query will still fail, but the call to limit_by should not.
	cmd.limit_by(-2);
	ASSERT_EQ("SELECT * LIMIT -2", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
}

TEST(CMDSelect, limit_by__after_limit__limit_reset)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.limit(10, 10);
	cmd.limit_by(20);
	
	
	ASSERT_EQ("SELECT * LIMIT 20", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
}

TEST(CMDSelect, limit_by__after_page__limit_reset)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.page(7, 3);
	cmd.limit_by(10);
	
	
	ASSERT_EQ("SELECT * LIMIT 10", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
}


TEST(CMDSelect, page)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.page(0, 0);
	ASSERT_EQ("SELECT * LIMIT 0", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
	
	cmd.page(10, 0);
	ASSERT_EQ("SELECT * LIMIT 0", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
	
	
	cmd.page(0, 10);
	ASSERT_EQ("SELECT * LIMIT 10", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
	
	cmd.page(1, 3);
	ASSERT_EQ("SELECT * LIMIT 3, 3", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
	cmd.page(2, 3);
	ASSERT_EQ("SELECT * LIMIT 6, 3", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
	cmd.page(7, 3);
	ASSERT_EQ("SELECT * LIMIT 21, 3", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
	
	
	// Validate this is no exceptions, the query will still fail, but the call to page should not.
	cmd.page(-2, -5);
	ASSERT_EQ("SELECT * LIMIT 10, -5", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
	
	cmd.page(-2, 5);
	ASSERT_EQ("SELECT * LIMIT -10, 5", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
}

TEST(CMDSelect, page__after_limit__limit_reset)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.limit(10, 10);
	cmd.page(2, 3);
	
	
	ASSERT_EQ("SELECT * LIMIT 6, 3", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
}

TEST(CMDSelect, page__after_limit_by__limit_reset)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.limit_by(10);
	cmd.page(7, 3);
	
	
	ASSERT_EQ("SELECT * LIMIT 21, 3", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
}


TEST(CMDSelect, column)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.column("hello");
	
	ASSERT_EQ("SELECT `hello`", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
	
	
	cmd.column("world");
	
	ASSERT_EQ("SELECT `hello`, `world`", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
}

TEST(CMDSelect, column_as)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.column_as("hello", "h");
	
	ASSERT_EQ("SELECT `hello` as `h`", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
	
	
	cmd.column_as("world", "wo");
	
	ASSERT_EQ("SELECT `hello` as `h`, `world` as `wo`", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
}

TEST(CMDSelect, column_as__char)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.column_as("hello", 'h');
	
	ASSERT_EQ("SELECT `hello` as `h`", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
	
	
	cmd.column_as("world", 'w');
	
	ASSERT_EQ("SELECT `hello` as `h`, `world` as `w`", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
}

TEST(CMDSelect, columns__vector)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.columns(std::vector<std::string_view> { });
	cmd.columns(std::vector<std::string_view> { "a" });
	cmd.columns(std::vector<std::string_view> { "c", "abc" });
	
	
	ASSERT_EQ("SELECT `a`, `c`, `abc`", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
}

TEST(CMDSelect, columns__init_list)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.columns({ });
	cmd.columns({ "a" });
	cmd.columns({ "c", "abc" });
	
	
	ASSERT_EQ("SELECT `a`, `c`, `abc`", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
}


TEST(CMDSelect, column_exp)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.column_exp("COUNT(*)");
	cmd.column_exp("SUM(amount)");
	cmd.column_exp("IF (status = ?, ?, ?)", std::initializer_list<BindValue> { "active", 1, 0 });
	cmd.column_exp("CASE WHEN type = ? THEN ? ELSE ? END", std::vector<BindValue>{ "A", 100, 200 });
	
	
	ASSERT_EQ(
		"SELECT COUNT(*), SUM(amount), IF (status = ?, ?, ?), CASE WHEN type = ? THEN ? ELSE ? END", 
		cmd.assemble());
	
	auto binds = cmd.bind();
	ASSERT_EQ(6, binds.size());
	
	ASSERT_EQ("active",		binds[0].get_str_value());
	ASSERT_EQ(1,			binds[1].get_value().i32);
	ASSERT_EQ(0,			binds[2].get_value().i32);
	ASSERT_EQ("A",			binds[3].get_str_value());
	ASSERT_EQ(100,			binds[4].get_value().i32);
	ASSERT_EQ(200,			binds[5].get_value().i32);
}


TEST(CMDSelect, where)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.where("age > 30");
	cmd.where("something > ?", 21);
	cmd.where("name IN (?, ?, ?)", { "Alice", "Bob", "Charlie" });
	cmd.where("last_name IN (?, ?, ?)", std::vector<BindValue> { "P.", "Sam" });
	

	ASSERT_EQ(
		"SELECT * WHERE age > 30 AND something > ? AND name IN (?, ?, ?) AND last_name IN (?, ?, ?)", 
		cmd.assemble());
	
	auto binds = cmd.bind();
	
	ASSERT_EQ(6, binds.size());
	
	ASSERT_EQ(21,			binds[0].get_value().i32);
	ASSERT_EQ("Alice",		binds[1].get_str_value());
	ASSERT_EQ("Bob",		binds[2].get_str_value());
	ASSERT_EQ("Charlie",	binds[3].get_str_value());
	ASSERT_EQ("P.",			binds[4].get_str_value());
	ASSERT_EQ("Sam",		binds[5].get_str_value());
}

TEST(CMDSelect, where_null)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.where_null("age");
	cmd.where_null("status");
	
	
	ASSERT_EQ("SELECT * WHERE `age` IS NULL AND `status` IS NULL", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
}

TEST(CMDSelect, where_not_null)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.where_not_null("age");
	cmd.where_not_null("status");
	
	
	ASSERT_EQ("SELECT * WHERE `age` IS NOT NULL AND `status` IS NOT NULL", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
}

TEST(CMDSelect, by_field)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.by_field("age", 30);
	cmd.by_field("status", "active");
	
	
	ASSERT_EQ("SELECT * WHERE `age` = ? AND `status` = ?", cmd.assemble());
	
	auto binds = cmd.bind();
	ASSERT_EQ(2, binds.size());
	ASSERT_EQ(30, binds[0].get_value().i32);
	ASSERT_EQ("active", binds[1].get_str_value());
}


TEST(CMDSelect, group_by_field)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.group_by_field("age");
	cmd.group_by_field("hello");
	cmd.group_by_field("world");
	
	
	ASSERT_EQ("SELECT * GROUP BY `age`, `hello`, `world`", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
}

TEST(CMDSelect, group_by)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.group_by("age");
	cmd.group_by("a = ?", 1);
	cmd.group_by("b IN (?, ?)", { 1, "2" });
	cmd.group_by("c IN (?, ?)", std::vector<BindValue> { "n", "bbb" });
	
	
	ASSERT_EQ("SELECT * GROUP BY age, a = ?, b IN (?, ?), c IN (?, ?)", cmd.assemble());
	
	auto binds = cmd.bind();
	
	ASSERT_EQ(5, binds.size());
	
	ASSERT_EQ(1,		binds[0].get_value().i32);
	ASSERT_EQ(1,		binds[1].get_value().i32);
	ASSERT_EQ("2",		binds[2].get_str_value());
	ASSERT_EQ("n",		binds[3].get_str_value());
	ASSERT_EQ("bbb",	binds[4].get_str_value());
}


TEST(CMDSelect, having)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.having("age = 2");
	cmd.having("stat = ?",		1);
	cmd.having("age = ?",		{ 30 });
	cmd.having("status = ?",	std::vector<BindValue>{ "ok", "good" });
	
	
	ASSERT_EQ("SELECT * HAVING age = 2 AND stat = ? AND age = ? AND status = ?", cmd.assemble());
	
	auto binds = cmd.bind();
	ASSERT_EQ(4, binds.size());
	ASSERT_EQ(1,		binds[0].get_value().i32);
	ASSERT_EQ(30,		binds[1].get_value().i32);
	ASSERT_EQ("ok",		binds[2].get_str_value());
	ASSERT_EQ("good",	binds[3].get_str_value());
}


TEST(CMDSelect, order_by_field)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.order_by_field("age");
	cmd.order_by_field("status", OrderBy::ASC);
	cmd.order_by_field("hello", OrderBy::DESC);
	cmd.order_by_field_asc("world");
	cmd.order_by_field_desc("last");
	
	
	ASSERT_EQ("SELECT * ORDER BY `age`, `status`, `hello` DESC, `world`, `last` DESC", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
}

TEST(CMDSelect, order_by)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.order_by("age");
	cmd.order_by("a = ?", 1);
	cmd.order_by("b IN (?, ?)", { 1, "2" });
	cmd.order_by("c IN (?, ?)", std::vector<BindValue> { "n", "bbb" });
	
	
	ASSERT_EQ("SELECT * ORDER BY age, a = ?, b IN (?, ?), c IN (?, ?)", cmd.assemble());
	
	auto binds = cmd.bind();
	
	ASSERT_EQ(5, binds.size());
	
	ASSERT_EQ(1,		binds[0].get_value().i32);
	ASSERT_EQ(1,		binds[1].get_value().i32);
	ASSERT_EQ("2",		binds[2].get_str_value());
	ASSERT_EQ("n",		binds[3].get_str_value());
	ASSERT_EQ("bbb",	binds[4].get_str_value());
}


TEST(CMDSelect, query_sanity)
{
	SQLighter sql { setup_db("test_select.db") };
	
	sql.execute(
		"CREATE TABLE ExampleTable (                     		"
		"	ID				INTEGER PRIMARY KEY,         		"
		"	Name			TEXT NOT NULL,     				    "
		"	Age				INTEGER,            			    "
		"	Balance			REAL,                  			 	"
		"	IsActive		BOOLEAN DEFAULT 1, 			     	"
		"	CreatedAt		DATETIME DEFAULT CURRENT_TIMESTAMP,	"
		"	NullableField	BLOB	                 			"
		")"
	);
	
	{
		auto stmt = sql.execute(
			"INSERT INTO ExampleTable (Name, Age, Balance, IsActive, NullableField) VALUES "
			"	('Alice',	30,		2500.50,	1, NULL),   	"
			"	('Bob',		NULL,	1500.00,	0, X'ABCD'),	"
			"	('Charlie',	25,		0.00,		1, X'EF01'),	"
			"	('Diana',	45,		10000.75,	1, NULL),   	"
			"	('Eve',		35,		500.00,		0, X'1234'),	"
			"	('Frank',	NULL,	NULL,		1, NULL),   	"
			"	('Grace',	40,		700.00,		0, X'5678'),	"
			"	('Hank',	20,		100.00,		1, NULL),   	"
			"	('Ivy',		50,		150.00,		0, X'9ABC'),	"
			"	('Jack',	60,		NULL,		1, X'DEF0') 	"
		);
		
		ASSERT_TRUE(stmt.is_done());
		ASSERT_FALSE(stmt.is_ok());
		ASSERT_FALSE(stmt.has_row());
		ASSERT_FALSE(stmt.is_error());
	}
	
	{
		auto stmt = sql.select()
			.from("ExampleTable")
			.by_field("Name", "Not Found")
			.execute();
		
		ASSERT_TRUE(stmt.is_done());
		ASSERT_FALSE(stmt.is_ok());
		ASSERT_FALSE(stmt.has_row());
		ASSERT_FALSE(stmt.is_error());
	}
	
	{
		auto stmt = sql.select()
			.from("ExampleTable")
			.where("Age > ?", 20)
			.where("Age < ?", 35)
			.execute();
		
		int count = 0;
		
		while (stmt.has_row())
		{
			ASSERT_FALSE(stmt.is_done());
			ASSERT_FALSE(stmt.is_error());
			
			count++;
			
			stmt.step();
		}
		
		ASSERT_EQ(2, count);
		ASSERT_TRUE(stmt.is_done());
		ASSERT_FALSE(stmt.is_error());
		ASSERT_FALSE(stmt.has_row());
	}
	
	{
		auto res = sql.select()
			.from("ExampleTable")
			.where("Age > ?", 20)
			.where("Age < ?", 35)
			.query_count();
		
		ASSERT_EQ(2, res);
	}
	
	{
		auto res = sql.select()
			.from("ExampleTable")
			.query_count();
		
		ASSERT_EQ(10, res);
	}
}

TEST(CMDSelect, query__scalars)
{
	SQLighter sql { setup_db("test_select.db") };
	
	sql.execute(
		"CREATE TABLE ExampleTable (                     		"
		"	ID				INTEGER PRIMARY KEY,         		"
		"	Name			TEXT NOT NULL,     				    "
		"	Age				INTEGER,            			    "
		"	Balance			REAL,                  			 	"
		"	IsActive		BOOLEAN DEFAULT 1, 			     	"
		"	CreatedAt		DATETIME DEFAULT CURRENT_TIMESTAMP,	"
		"	NullableField	BLOB	                 			"
		")"
	);
	
	{
		auto stmt = sql.execute(
			"INSERT INTO ExampleTable (Name, Age, Balance, IsActive, NullableField) VALUES "
			"	('Alice',	30,		2500.50,	1, NULL),   	"
			"	('Bob',		NULL,	1500.00,	0, X'ABCD')	"
		);
		
		ASSERT_TRUE(stmt.is_done());
	}
	
	
	{
		auto query = sql.select()
			.column("Name")
			.from("ExampleTable")
			.order_by("Name")
			.limit_by(1);
		
		ASSERT_EQ("Alice", query.query_str());
	}
	
	{
		auto query = sql.select()
			.column("Age")
			.from("ExampleTable")
			.order_by("Name")
			.limit_by(1);
		
		ASSERT_EQ(30, query.query_int());
	}
	
	{
		auto query = sql.select()
			.column("Balance")
			.from("ExampleTable")
			.order_by("Name")
			.limit_by(1);
		
		ASSERT_EQ(2500.5, query.query_double());
	}
	
	{
		auto query = sql.select()
			.column("IsActive")
			.from("ExampleTable")
			.by_field("Name", "Bob");
		
		ASSERT_FALSE(query.query_bool());
		
		query = sql.select()
			.column("IsActive")
			.from("ExampleTable")
			.order_by("Name")
			.limit_by(1);
		
		ASSERT_TRUE(query.query_bool());
	}
	
	{
		auto query = sql.select()
			.column("Age")
			.from("ExampleTable")
			.by_field("Name", "Bob");
		
		ASSERT_TRUE(query.query_is_null());
		
		query = sql.select()
			.column("Age")
			.from("ExampleTable")
			.by_field("Name", "Alice");
		
		ASSERT_FALSE(query.query_is_null());
	}
	
	{
		auto query = sql.select()
			.column("NullableField")
			.from("ExampleTable")
			.by_field("Name", "Bob");
		
		auto res = query.query_blob();
		
		ASSERT_EQ(2, res.size());
		ASSERT_EQ(0xAB, res[0]);
		ASSERT_EQ(0xCD, res[1]);
	}
}


TEST(CMDSelect, query__scalars__nullable)
{
	SQLighter sql { setup_db("test_select.db") };
	
	sql.execute(
		"CREATE TABLE ExampleTable (                     		"
		"	ID				INTEGER PRIMARY KEY,         		"
		"	Name			TEXT,     				    "
		"	Age				INTEGER,            			    "
		"	Balance			REAL,                  			 	"
		"	IsActive		BOOLEAN DEFAULT 1, 			     	"
		"	CreatedAt		DATETIME DEFAULT CURRENT_TIMESTAMP,	"
		"	NullableField	BLOB	                 			"
		")"
	);
	
	{
		auto stmt = sql.execute(
			"INSERT INTO ExampleTable (Name, Age, Balance, IsActive, NullableField) VALUES "
			"	(NULL, NULL, NULL, NULL, NULL) "
		);
		
		ASSERT_TRUE(stmt.is_done());
	}
	
	
	{
		auto query = sql.select()
			.column("Name")
			.from("ExampleTable")
			.limit_by(1);
		
		ASSERT_EQ("", query.query_str());
	}
	
	{
		auto query = sql.select()
			.column("Age")
			.from("ExampleTable")
			.limit_by(1);
		
		ASSERT_EQ(0, query.query_int());
	}
	
	{
		auto query = sql.select()
			.column("Balance")
			.from("ExampleTable")
			.limit_by(1);
		
		ASSERT_EQ(0.0, query.query_double());
	}
	
	{
		auto query = sql.select()
			.column("IsActive")
			.from("ExampleTable")
			.limit_by(1);
		
		ASSERT_FALSE(query.query_bool());
	}
	
	{
		auto query = sql.select()
			.column("NullableField")
			.from("ExampleTable")
			.limit_by(1);
		
		auto res = query.query_blob();
		
		ASSERT_TRUE(res.empty());
	}
}