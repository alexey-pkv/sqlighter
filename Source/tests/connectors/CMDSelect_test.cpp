#include "connectors/CMDSelect.h"


#include "core/Stmt.h"
#include "base/connection/IConnection.h"

#include "db_mock.h"
#include "sqlighter.h"
#include "exceptions/sqlighter_exceptions.h"

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
	catch (const SQLighterException& e)
	{
		ASSERT_EQ(SQLIGHTER_ERR_GENERIC, e.code());
	}
}

TEST(CMDSelect, constructor__copy)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	cmd
		.from("hello")
		.join_exp("JOIN ME", { "bind" })
		.column("age")
		.column("name")
		.where("compare = ?", 1)
		.by_field("field", "value")
		.limit_by(23)
		.order_by("something");
	
	
	CMDSelect cmd2(cmd);
	
	
	ASSERT_EQ(cmd.assemble(), cmd2.assemble());
	ASSERT_EQ(3, cmd2.bind().size());
	ASSERT_EQ(cmd.bind().size(), cmd2.bind().size());
	
	ASSERT_EQ(cmd.bind()[0].get_type(), cmd2.bind()[0].get_type());
	ASSERT_EQ(cmd.bind()[1].get_type(), cmd2.bind()[1].get_type());
	ASSERT_EQ(cmd.bind()[2].get_type(), cmd2.bind()[2].get_type());
	
	ASSERT_EQ(cmd.bind()[0].get_str_value(), cmd2.bind()[0].get_str_value());
	ASSERT_EQ(cmd.bind()[1].get_value().i32, cmd2.bind()[1].get_value().i32);
	ASSERT_EQ(cmd.bind()[2].get_str_value(), cmd2.bind()[2].get_str_value());
}

TEST(CMDSelect, constructor__move)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	cmd
		.from("hello")
		.join_exp("JOIN ME", { "bind" })
		.column("age")
		.column("name")
		.where("compare = ?", 1)
		.by_field("field", "value")
		.limit_by(23)
		.order_by("something");
	
	auto binds = cmd.bind();
	auto query = cmd.assemble();
	
	
	CMDSelect cmd2(std::move(cmd));
	auto binds2 = cmd2.bind();
	
	
	ASSERT_EQ(query, cmd2.assemble());
	ASSERT_EQ(3, binds2.size());
	ASSERT_EQ(binds.size(), binds2.size());
	
	ASSERT_EQ(binds[0].get_type(), binds2[0].get_type());
	ASSERT_EQ(binds[1].get_type(), binds2[1].get_type());
	ASSERT_EQ(binds[2].get_type(), binds2[2].get_type());
	
	ASSERT_EQ(binds[0].get_str_value(), binds2[0].get_str_value());
	ASSERT_EQ(binds[1].get_value().i32, binds2[1].get_value().i32);
	ASSERT_EQ(binds[2].get_str_value(), binds2[2].get_str_value());
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
	
	
	ASSERT_EQ("SELECT * FROM table", cmd.from("table").assemble());
	ASSERT_TRUE(cmd.bind().empty());
	
	ASSERT_EQ("SELECT * FROM table AS a", cmd.from("table", "a").assemble());
	ASSERT_TRUE(cmd.bind().empty());
}

TEST(CMDSelect, from_scheme)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	ASSERT_EQ("SELECT * FROM table.tab", cmd.from_scheme("table", "tab").assemble());
	ASSERT_TRUE(cmd.bind().empty());
	
	ASSERT_EQ("SELECT * FROM table.tab AS aa", cmd.from_scheme("table", "tab", "aa").assemble());
	ASSERT_TRUE(cmd.bind().empty());
}


TEST(CMDSelect, join_exp)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.from("table")
		.join_exp("JOIN something AS b", { 1 , "va" })
		.join_exp("JOIN abc AS d", {3 , "dd" });
	
	
	auto binds = cmd.bind();
	auto command = cmd.assemble();
	
	ASSERT_EQ("SELECT * FROM table JOIN something AS b JOIN abc AS d", command);
	ASSERT_EQ(4, binds.size());
	
	ASSERT_EQ(1,	binds[0].get_value().i32);
	ASSERT_EQ("va",	binds[1].get_str_value());
	ASSERT_EQ(3,	binds[2].get_value().i32);
	ASSERT_EQ("dd",	binds[3].get_str_value());
}

TEST(CMDSelect, join_exp__no_binds)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.from("table").join_exp("JOIN something AS b", {});
	
	
	auto binds = cmd.bind();
	auto command = cmd.assemble();
	
	ASSERT_EQ("SELECT * FROM table JOIN something AS b", command);
	ASSERT_TRUE(binds.empty());
}

TEST(CMDSelect, join_exp__no_string)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.from("table").join_exp("", { 1 , "va" });
	
	
	auto binds = cmd.bind();
	auto command = cmd.assemble();
	
	ASSERT_EQ("SELECT * FROM table", command);
	ASSERT_EQ(2, binds.size());
	ASSERT_EQ(1, binds[0].get_value().i32);
	ASSERT_EQ("va", binds[1].get_str_value());
}

TEST(CMDSelect, join__sanity)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.from("table")
		.join("table_a", "ab", "a = b", { 1 })
		.left_join("table_l", "lb", "l = b", { 2 })
		.right_join("table_r", "rb", "r = b", { 3 });
	
	
	auto binds = cmd.bind();
	auto command = cmd.assemble();
	
	ASSERT_EQ(
		"SELECT * FROM table"
			" JOIN table_a AS ab ON a = b"
			" LEFT JOIN table_l AS lb ON l = b"
			" RIGHT JOIN table_r AS rb ON r = b",
		command);
	
	ASSERT_EQ(3, binds.size());
	ASSERT_EQ(1, binds[0].get_value().i32);
	ASSERT_EQ(2, binds[1].get_value().i32);
	ASSERT_EQ(3, binds[2].get_value().i32);
}

TEST(CMDSelect, join_statement__sanity)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.from("table")
		.join_statement("ABC", "scm", "tbl", "as_a", "a = b", { 1 })
		.join_statement("DEF", "scm2", "tbl2", "as_a2", "1 = 1", { 2 });
	
	
	auto binds = cmd.bind();
	auto command = cmd.assemble();
	
	ASSERT_EQ(
		"SELECT * FROM table"
			" ABC scm.tbl AS as_a ON a = b"
			" DEF scm2.tbl2 AS as_a2 ON 1 = 1",
		command);
	
	ASSERT_EQ(2, binds.size());
	ASSERT_EQ(1, binds[0].get_value().i32);
	ASSERT_EQ(2, binds[1].get_value().i32);
}

TEST(CMDSelect, join_statement__no_scheme)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.from("table").join_statement("ABC", "", "tbl", "as_a", "a = b", { 1 });
	
	
	ASSERT_EQ("SELECT * FROM table ABC tbl AS as_a ON a = b", cmd.assemble());
	ASSERT_EQ(1, cmd.bind()[0].get_value().i32);
}

TEST(CMDSelect, join_statement__no_condition)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.from("table").join_statement("ABC", "scm", "tbl", "as_a", "", { 1 });
	
	
	ASSERT_EQ("SELECT * FROM table ABC scm.tbl AS as_a", cmd.assemble());
	ASSERT_EQ(1, cmd.bind()[0].get_value().i32);
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
	
	ASSERT_EQ("SELECT hello", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
	
	
	cmd.column("world");
	
	ASSERT_EQ("SELECT hello, world", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
}

TEST(CMDSelect, column_as)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.column_as("hello", "h");
	
	ASSERT_EQ("SELECT hello AS h", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
	
	
	cmd.column_as("world", "wo");
	
	ASSERT_EQ("SELECT hello AS h, world AS wo", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
}

TEST(CMDSelect, column_as__char)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.column_as("hello", 'h');
	
	ASSERT_EQ("SELECT hello AS h", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
	
	
	cmd.column_as("world", 'w');
	
	ASSERT_EQ("SELECT hello AS h, world AS w", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
}

TEST(CMDSelect, columns__vector)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.columns(std::vector<std::string_view> { });
	cmd.columns(std::vector<std::string_view> { "a" });
	cmd.columns(std::vector<std::string_view> { "c", "abc" });
	
	
	ASSERT_EQ("SELECT a, c, abc", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
}

TEST(CMDSelect, columns__init_list)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.columns({ });
	cmd.columns({ "a" });
	cmd.columns({ "c", "abc" });
	
	
	ASSERT_EQ("SELECT a, c, abc", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
}


TEST(CMDSelect, column_exp)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.column_exp("COUNT(*)");
	cmd.column_exp("SUM(amount)");
	cmd.column_exp("IF (status = ?, ?, ?)", std::initializer_list<BindValue>{ "active", 1, 0 });
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
	
	
	ASSERT_EQ("SELECT * WHERE age IS NULL AND status IS NULL", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
}

TEST(CMDSelect, where_not_null)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.where_not_null("age");
	cmd.where_not_null("status");
	
	
	ASSERT_EQ("SELECT * WHERE age IS NOT NULL AND status IS NOT NULL", cmd.assemble());
	ASSERT_TRUE(cmd.bind().empty());
}

TEST(CMDSelect, by_field)
{
	CMDSelect cmd(std::make_shared<connection_override>());
	
	
	cmd.by_field("age", 30);
	cmd.by_field("status", "active");
	
	
	ASSERT_EQ("SELECT * WHERE age = ? AND status = ?", cmd.assemble());
	
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
	
	
	ASSERT_EQ("SELECT * GROUP BY age, hello, world", cmd.assemble());
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
	
	
	ASSERT_EQ("SELECT * ORDER BY age, status, hello DESC, world, last DESC", cmd.assemble());
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
		"CREATE TABLE ExampleTable (						"
		"	ID				INTEGER PRIMARY KEY,				"
		"	Name			TEXT NOT NULL,						"
		"	Age				INTEGER,							"
		"	Balance			REAL,								"
		"	IsActive		BOOLEAN DEFAULT 1,					"
		"	CreatedAt		DATETIME DEFAULT CURRENT_TIMESTAMP,	"
		"	NullableField	BLOB								"
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


TEST(CMDSelect, query_column)
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
	
	sql.execute(
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
	
	{
		auto res = sql.select()
			.column("Age")
			.from("ExampleTable")
			.by_field("Name", "Not Found")
			.query_column();
		
		ASSERT_TRUE(res.empty());
	}
	
	{
		auto res = sql.select()
			.column("Age")
			.from("ExampleTable")
			.order_by("Age")
			.query_column();
		
		ASSERT_FALSE(res.empty());
		
		ASSERT_EQ(sql.count_rows("ExampleTable"), res.size());
		
		ASSERT_TRUE(res[0].is_null());
		ASSERT_TRUE(res[1].is_null());
		
		ASSERT_EQ(20, res[2].get<int32_t>());
		ASSERT_EQ(25, res[3].get<int32_t>());
		ASSERT_EQ(30, res[4].get<int32_t>());
		ASSERT_EQ(35, res[5].get<int32_t>());
		ASSERT_EQ(40, res[6].get<int32_t>());
		ASSERT_EQ(45, res[7].get<int32_t>());
		ASSERT_EQ(50, res[8].get<int32_t>());
		ASSERT_EQ(60, res[9].get<int32_t>());
	}
}


TEST(CMDSelect, query_row__expect_any_number_of_rows)
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
		"INSERT INTO ExampleTable (Name, Age, Balance, IsActive, NullableField) VALUES "
		"	('Alice',	30,		2500.50,	1, NULL),   	"
		"	('Bob',		NULL,	1500.00,	0, X'ABCD')		"
	);
	
	{
		auto res = sql.select()
			.from("ExampleTable")
			.by_field("Name", "Not Found")
			.query_row(false);
		
		ASSERT_TRUE(res.empty());
	}
	
	{
		auto res = sql.select()
			.from("ExampleTable")
			.order_by("Name")
			.query_row(false);
		
		ASSERT_FALSE(res.empty());
		ASSERT_EQ(5, res.size());
		
		ASSERT_EQ("Alice",	res[0].get_str());
		ASSERT_EQ(30,		res[1].get<int32_t>());
		ASSERT_EQ(2500.5,	res[2].get<double>());
		ASSERT_EQ(true,		res[3].get<bool>());
		
		ASSERT_TRUE(res[4].is_null());
	}
}

TEST(CMDSelect, query_row__expect_one)
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
		"INSERT INTO ExampleTable (Name, Age, Balance, IsActive, NullableField) VALUES "
		"	('Alice',	30,		2500.50,	1, NULL),   	"
		"	('Bob',		NULL,	1500.00,	0, X'ABCD')		"
	);
	
	
	try
	{
		auto res = sql.select()
			.from("ExampleTable")
			.by_field("Name", "Not Found")
			.query_row(true);
		
		FAIL();
	}
	catch (const SQLighterException& e)
	{
		ASSERT_EQ(SQLIGHTER_ERR_NO_ROWS, e.code());
	}
	
	try
	{
		auto res = sql.select()
			.from("ExampleTable")
			.order_by("Name")
			.query_row(true);
		
		FAIL();
	}
	catch (const SQLighterException& e)
	{
		ASSERT_EQ(SQLIGHTER_ERR_MULT_ROWS, e.code());
	}
	
	
	auto res = sql.select()
		.from("ExampleTable")
		.order_by("Name")
		.limit_by(1)
		.query_row(true);
	
	ASSERT_FALSE(res.empty());
	ASSERT_EQ(5, res.size());
	
	ASSERT_EQ("Alice",	res[0].get_str());
	ASSERT_EQ(30,		res[1].get<int32_t>());
	ASSERT_EQ(2500.5,	res[2].get<double>());
	ASSERT_EQ(true,		res[3].get<bool>());
	
	ASSERT_TRUE(res[4].is_null());
}

TEST(CMDSelect, query_all__empty)
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
	
	
	ASSERT_TRUE(sql.query_all("ExampleTable").empty());
}

TEST(CMDSelect, query_all__too_many_rows__exception_thrown)
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
		"INSERT INTO ExampleTable (Name, Age, Balance, IsActive, NullableField) VALUES "
		"	('Alice',	30,		2500.50,	1, NULL),   	"
		"	('Bob',		NULL,	1500.00,	0, X'ABCD')		"
	);
	
	
	try
	{
		sql.query_all("ExampleTable", 1);
		FAIL();
	}
	catch (const SQLighterException& e)
	{
		ASSERT_EQ(SQLIGHTER_ERR_ROWS_OVERFLOW, e.code());
	}
}

TEST(CMDSelect, query_all)
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
		"INSERT INTO ExampleTable (Name, Age, Balance, IsActive, NullableField) VALUES "
		"	('Alice',	30,		2500.50,	1, NULL),   	"
		"	('Bob',		NULL,	1500.00,	0, X'ABCD')		"
	);
	
	
	auto data = sql
		.select()
		.from("ExampleTable")
		.order_by("Name")
		.query_all(2);
	
	
	ASSERT_EQ(2, data.size());
	
	ASSERT_EQ(5, data[0].size());
	ASSERT_EQ(5, data[1].size());
	
	ASSERT_EQ("Alice",	data[0][0].get_str());
	ASSERT_EQ(30,		data[0][1].get<int32_t>());
	ASSERT_EQ(2500.5,	data[0][2].get<double>());
	ASSERT_EQ(true,		data[0][3].get<bool>());
	ASSERT_EQ(true,		data[0][4].is_null());
	
	ASSERT_EQ("Bob",	data[1][0].get_str());
	ASSERT_EQ(true,		data[1][1].is_null());
	ASSERT_EQ(1500.0,	data[1][2].get<double>());
	ASSERT_EQ(false,	data[1][3].get<bool>());
	ASSERT_EQ(2,		data[1][4].get_blob().size());
	{
		ASSERT_EQ(0xAB,		data[1][4].get_blob()[0]);
		ASSERT_EQ(0xCD,		data[1][4].get_blob()[1]);
	}
}

TEST(CMDSelect, query_with_join)
{
	SQLighter sql { setup_db("test_select.db") };
	
	
	sql.execute(
		"CREATE TABLE ExampleTable (				"
		"	Name			TEXT NOT NULL PRIMARY KEY,	"
		"	Age				INTEGER,					"
		"	Balance			REAL,						"
		"	IsActive		BOOLEAN DEFAULT 1,			"
		"	NullableField	BLOB						"
		")"
	);
	
	sql.execute(
		"INSERT INTO ExampleTable (Name, Age, Balance, IsActive, NullableField) VALUES "
		"	('Alice',	30,		2500.50,	1, NULL),   	"
		"	('Bob',		NULL,	1500.00,	0, X'ABCD')		"
	);
	
	sql.execute(
		"CREATE TABLE ExtraData (		"
		"	Name			TEXT NOT NULL,	"
		"	BalanceChange	INTEGER			"
		")"
	);
	
	sql.execute(
		"INSERT INTO ExtraData (Name, BalanceChange) VALUES "
		"	('Alice',	30),	"
		"	('Alice',	-25)	"
	);
	
	
	auto data = sql
		.select()
		.columns({ "et.Name", "ed.BalanceChange" })
		.from("ExampleTable", "et")
		.join("ExtraData", "ed", "et.Name = ed.Name", {})
		.order_by("et.Name")
		.order_by("ed.BalanceChange")
		.query_all(1000);
	
	
	ASSERT_EQ(2, data.size());
	
	ASSERT_EQ(2, data[0].size());
	ASSERT_EQ(2, data[1].size());
	
	ASSERT_EQ("Alice",	data[0][0].get_str());
	ASSERT_EQ(-25,		data[0][1].get<int32_t>());
	
	ASSERT_EQ("Alice",	data[1][0].get_str());
	ASSERT_EQ(30,		data[1][1].get<int32_t>());
}