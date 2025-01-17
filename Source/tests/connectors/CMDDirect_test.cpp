#include "connectors/CMDDirect.h"


#include "base/connection/IConnection.h"

#include "connection_override.h"
#include "exceptions/sqlighter_exceptions.h"

#include <gtest/gtest.h>


using namespace sqlighter;


TEST(CMDDirect, constructor_with_nullptr)
{
	try
	{
		CMDDirect cd(nullptr);
		FAIL();
	}
	catch (const SQLighterException& re)
	{
		ASSERT_EQ(re.code(), SQLIGHTER_ERR_GENERIC);
	}
}

TEST(CMDDirect, constructor_copy)
{
	CMDDirect cmd(std::make_shared<connection_override>());
	
	cmd << "hello world";
	cmd << bind({ 1, 2, 3 });
	
	
	CMDDirect cmd2(cmd);
	
	
	ASSERT_EQ("hello world", cmd2.assemble());
	
	ASSERT_EQ(3, cmd2.bind().size());
	
	ASSERT_EQ(1, cmd2.bind()[0].get_value().i32);
	ASSERT_EQ(2, cmd2.bind()[1].get_value().i32);
	ASSERT_EQ(3, cmd2.bind()[2].get_value().i32);
}

TEST(CMDDirect, constructor_move)
{
	CMDDirect cmd(std::make_shared<connection_override>());
	
	cmd << "hello world";
	cmd << bind({ 1, 2, 3 });
	
	
	CMDDirect cmd2(std::move(cmd));
	
	
	ASSERT_EQ("hello world", cmd2.assemble());
	
	ASSERT_EQ(3, cmd2.bind().size());
	
	ASSERT_EQ(1, cmd2.bind()[0].get_value().i32);
	ASSERT_EQ(2, cmd2.bind()[1].get_value().i32);
	ASSERT_EQ(3, cmd2.bind()[2].get_value().i32);
}

TEST(CMDDirect, operator_assign_move)
{
	CMDDirect cmd(std::make_shared<connection_override>());
	CMDDirect cmd2(std::make_shared<connection_override>());
	
	
	cmd << "hello world";
	cmd << bind({ 1, 2, 3 });
	
	cmd2 << "omg";
	cmd2 << bind("ok");
	
	
	cmd2 = std::move(cmd);
	
	
	ASSERT_EQ("hello world", cmd2.assemble());
	
	ASSERT_EQ(3, cmd2.bind().size());
	
	ASSERT_EQ(1, cmd2.bind()[0].get_value().i32);
	ASSERT_EQ(2, cmd2.bind()[1].get_value().i32);
	ASSERT_EQ(3, cmd2.bind()[2].get_value().i32);
}

TEST(CMDDirect, assign_operator)
{
	CMDDirect cmd(std::make_shared<connection_override>());
	CMDDirect cmd2(std::make_shared<connection_override>());
	
	
	cmd << "hello world";
	cmd << bind({ 1, 2, 3 });
	
	cmd2 << "omg";
	cmd2 << bind("ok");
	
	
	cmd2 = cmd;
	
	
	ASSERT_EQ("hello world", cmd2.assemble());
	
	ASSERT_EQ(3, cmd2.bind().size());
	
	ASSERT_EQ(1, cmd2.bind()[0].get_value().i32);
	ASSERT_EQ(2, cmd2.bind()[1].get_value().i32);
	ASSERT_EQ(3, cmd2.bind()[2].get_value().i32);
}


TEST(CMDDirect, left_shift_operator)
{
	CMDDirect cmd(std::make_shared<connection_override>());
	
	
	cmd << "hello world";
	cmd << " 123" << "456";
	
	cmd << BindValue { 1 };
	cmd << bind(1);
	cmd << bind({ 1, 2, 3 });
	cmd << std::vector<BindValue> { false, 1.2, "ok" };
	
	
	auto binds = cmd.bind();
	auto ass = cmd.assemble();
	
	
	ASSERT_EQ("hello world 123456", ass);
	
	ASSERT_EQ(8, binds.size());
	
	ASSERT_EQ(1,		binds[0].get_value().i32);
	ASSERT_EQ(1,		binds[1].get_value().i32);
	ASSERT_EQ(1,		binds[2].get_value().i32);
	ASSERT_EQ(2,		binds[3].get_value().i32);
	ASSERT_EQ(3,		binds[4].get_value().i32);
	ASSERT_EQ(0,		binds[5].get_value().i32);
	ASSERT_EQ(1.2,		binds[6].get_value().dbl);
	ASSERT_EQ("ok",		binds[7].get_str_value());
}

TEST(CMDDirect, append)
{
	CMDDirect cmd(std::make_shared<connection_override>());
	
	
	cmd.append("hello world");
	
	cmd.append(" 123", 1);
	cmd.append("4", bind(1));
	cmd.append("5", { 1, 2, 3 });
	cmd.append("6", std::vector<BindValue> { false, 1.2, "ok" });
	
	
	auto binds = cmd.bind();
	auto ass = cmd.assemble();
	
	
	ASSERT_EQ("hello world 123456", ass);
	
	ASSERT_EQ(8, binds.size());
	
	ASSERT_EQ(1,		binds[0].get_value().i32);
	ASSERT_EQ(1,		binds[1].get_value().i32);
	ASSERT_EQ(1,		binds[2].get_value().i32);
	ASSERT_EQ(2,		binds[3].get_value().i32);
	ASSERT_EQ(3,		binds[4].get_value().i32);
	ASSERT_EQ(0,		binds[5].get_value().i32);
	ASSERT_EQ(1.2,		binds[6].get_value().dbl);
	ASSERT_EQ("ok",		binds[7].get_str_value());
}

TEST(CMDDirect, assemble__with_stringstream)
{
	CMDDirect cmd(std::make_shared<connection_override>());
	std::ostringstream ss;
	
	cmd << "hello world";
	cmd << " 123" << "456";
	
	
	cmd.assemble(ss);
	
	
	ASSERT_EQ("hello world 123456", ss.str());
}

TEST(CMDDirect, execute)
{
	auto conn = std::make_shared<connection_override>();
	CMDDirect cmd(conn);
	
	cmd << "hello world";
	cmd << bind({ 1, 2 });
	
	
	cmd.execute();
	
	
	ASSERT_EQ("hello world", conn->last_query);
	ASSERT_EQ(2, conn->last_bind.size());
	ASSERT_EQ(1, conn->last_bind[0].get_value().i32);
	ASSERT_EQ(2, conn->last_bind[1].get_value().i32);
}