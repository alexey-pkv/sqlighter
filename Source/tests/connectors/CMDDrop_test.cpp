#include "connectors/CMDDrop.h"


#include "base/connection/IConnection.h"

#include "connection_override.h"
#include "exceptions/sqlighter_exceptions.h"
#include "sqlighter.h"
#include "db_mock.h"

#include <gtest/gtest.h>


using namespace sqlighter;


TEST(CMDDrop, constructor_with_nullptr)
{
	try
	{
		CMDDrop cd(nullptr);
		FAIL();
	}
	catch (const SQLighterException& re)
	{
		ASSERT_EQ(re.code(), SQLIGHTER_ERR_GENERIC);
	}
}


TEST(CMDDrop, if_exists)
{
	CMDDrop cd(std::make_shared<connection_override>());
	
	ASSERT_EQ("DROP TABLE IF EXISTS `a`", cd.if_exists().table("a").assemble());
}

TEST(CMDDrop, sanity_bind)
{
	CMDDrop cd(std::make_shared<connection_override>());
	ASSERT_TRUE( cd.if_exists().table("a").bind().empty());
}


TEST(CMDDrop, sanity_element)
{
	CMDDrop cd(std::make_shared<connection_override>());
	
	ASSERT_EQ("DROP TABLE `a`", cd.table("a").assemble());
	ASSERT_EQ("DROP VIEW `a`", cd.view("a").assemble());
	ASSERT_EQ("DROP INDEX `a`", cd.index("a").assemble());
	ASSERT_EQ("DROP TRIGGER `a`", cd.trigger("a").assemble());
}

TEST(CMDDrop, sanity_scheme_name)
{
	CMDDrop cd(std::make_shared<connection_override>());
	
	ASSERT_EQ("DROP TABLE `s`.`a`", cd.table("s", "a").assemble());
	ASSERT_EQ("DROP VIEW `s`.`a`", cd.view("s", "a").assemble());
	ASSERT_EQ("DROP INDEX `s`.`a`", cd.index("s", "a").assemble());
	ASSERT_EQ("DROP TRIGGER `s`.`a`", cd.trigger("s", "a").assemble());
}

TEST(CMDDrop, sanity_name_with_scheme)
{
	CMDDrop cd(std::make_shared<connection_override>());
	
	ASSERT_EQ("DROP TABLE `s`.`a`", cd.table("s.a").assemble());
	ASSERT_EQ("DROP VIEW `s`.`a`", cd.view("s.a").assemble());
	ASSERT_EQ("DROP INDEX `s`.`a`", cd.index("s.a").assemble());
	ASSERT_EQ("DROP TRIGGER `s`.`a`", cd.trigger("s.a").assemble());
}

TEST(CMDDrop, sanity)
{
	SQLighter sql { setup_db("test_select.db") };
	
	sql.execute("CREATE TABLE Users (UserID INTEGER PRIMARY KEY AUTOINCREMENT)");
	sql.execute("CREATE VIEW UserView AS SELECT UserID FROM Users;");
	sql.execute("CREATE INDEX idx_userid ON Users(UserID);");
	sql.execute("CREATE TRIGGER update_timestamp "
		 "AFTER UPDATE ON Users "
		 "FOR EACH ROW "
		 "BEGIN "
		 "UPDATE Users "
		 "SET UserID = UserID + 1 "
		 "WHERE UserID = OLD.UserID; "
		 "END;");
	
	sql.drop().trigger("update_timestamp").execute();
	sql.drop().index("idx_userid").execute();
	sql.drop().view("UserView").execute();
	sql.drop().table("Users").execute();
	
	sql.drop().if_exists().trigger("update_timestamp").execute();
	sql.drop().if_exists().index("idx_userid").execute();
	sql.drop().if_exists().view("UserView").execute();
	sql.drop().if_exists().table("Users").execute();
}
