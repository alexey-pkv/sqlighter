#include "connectors/CMDSelect.h"


#include "base/connection/IConnection.h"
#include "core/Stmt.h"

#include <gtest/gtest.h>


using namespace sqlighter;


class connection_override : public IConnection
{
public:
	Stmt execute(const std::string& query, const std::vector<BindValue>& values) override
	{
		return Stmt();
	}
};


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
	cmd.column_exp("IF (status = ?, ?, ?)", { "active", 1, 0 });
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






