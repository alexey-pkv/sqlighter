#include "core/BindValue.h"
#include "sqlighter.h"
#include "db_mock.h"
#include "exceptions/sqlighter_exceptions.h"


#include <gtest/gtest.h>


using namespace sqlighter;


TEST(BindValue, constructor__copy)
{
	BindValue bv_int(BindValue { 32 });
	BindValue bv_str(BindValue { "omg" });
	
	
	ASSERT_EQ(32, bv_int.get_value().i32);
	ASSERT_EQ(BindValue::type::INT_32, bv_int.get_type());
	ASSERT_TRUE(bv_int.get_str_value().empty());
	
	
	ASSERT_EQ("omg", bv_str.get_str_value());
	ASSERT_EQ(BindValue::type::TEXT, bv_str.get_type());
}

TEST(BindValue, constructor__move)
{
	BindValue bv_int(std::move(BindValue { 32 }));
	BindValue bv_str(std::move(BindValue { "omg" }));
	
	
	ASSERT_EQ(32, bv_int.get_value().i32);
	ASSERT_EQ(BindValue::type::INT_32, bv_int.get_type());
	ASSERT_TRUE(bv_int.get_str_value().empty());
	
	
	ASSERT_EQ("omg", bv_str.get_str_value());
	ASSERT_EQ(BindValue::type::TEXT, bv_str.get_type());
}

TEST(BindValue, constructor__type)
{
	BindValue bv { BindValue::type::TEXT_16 };
	
	
	ASSERT_EQ(0, bv.get_value().i32);
	ASSERT_EQ(BindValue::type::TEXT_16, bv.get_type());
	ASSERT_TRUE(bv.get_str_value().empty());
}


TEST(BindValue, eq_operator__copy)
{
	BindValue bv_int_o { 32 };
	BindValue bv_str_o { "omg" };
	BindValue bv_int { 0.2 };
	BindValue bv_str { 129.2 };
	
	bv_int = bv_int_o;
	bv_str = bv_str_o;
	
	ASSERT_EQ(bv_int_o.get_value().i32, bv_int.get_value().i32);
	ASSERT_EQ(BindValue::type::INT_32, bv_int.get_type());
	ASSERT_TRUE(bv_int.get_str_value().empty());
	
	
	ASSERT_EQ(bv_str_o.get_str_value(), bv_str.get_str_value());
	ASSERT_EQ(BindValue::type::TEXT, bv_str.get_type());
}

TEST(BindValue, eq_operator__move)
{
	BindValue bv_int;
	BindValue bv_str;
	
	bv_int = std::move(BindValue { 32 });
	bv_str = std::move(BindValue { "omg" });
	
	
	ASSERT_EQ(32, bv_int.get_value().i32);
	ASSERT_EQ(BindValue::type::INT_32, bv_int.get_type());
	ASSERT_TRUE(bv_int.get_str_value().empty());
	
	
	ASSERT_EQ("omg", bv_str.get_str_value());
	ASSERT_EQ(BindValue::type::TEXT, bv_str.get_type());
}


TEST(BindValue, null_type)
{
	SQLighter sql { create_mock_table("mock_table", "c_null") };
	
	auto res = sql.select()
		.column_exp("(? IS NULL)", BindValue(BindValue::type::NULL_VAL))
		.query_bool();
	
	
	ASSERT_EQ(1, res);
}

TEST(BindValue, text64__exception_thrown)
{
	SQLighter sql { create_mock_table("mock_table") };
	
	
	BindValue b(BindValue::type::TEXT_64);
	b.set_str_value("abc");
	
	
	try
	{
		sql.select().column_exp("?", b).execute();
		FAIL();
	}
	catch (const SQLighterException& e)
	{
		ASSERT_EQ(SQLIGHTER_ERR_BIND, e.code());
	}
}

TEST(BindValue, text16__exception_throw)
{
	SQLighter sql { create_mock_table("mock_table") };
	
	
	BindValue b(BindValue::type::TEXT_16);
	b.set_str_value("abc");
	
	
	try
	{
		sql.select().column_exp("?", b).execute();
		FAIL();
	}
	catch (const SQLighterException& e)
	{
		ASSERT_EQ(SQLIGHTER_ERR_BIND, e.code());
	}
}

TEST(BindValue, bind_failed__exception_thrown)
{
	SQLighter sql { create_mock_table("mock_table") };
	
	
	try
	{
		sql.select().column_exp("?, 1", { 1, 2 }).execute();
		FAIL();
	}
	catch (const SQLighterException& e)
	{
		ASSERT_EQ(SQLIGHTER_ERR_BIND, e.code());
	}
}

TEST(BindValue, to_error_message)
{
	auto test = [](BindValue bv) -> std::string 
	{
		std::stringstream ss;
		bv.to_error_message(ss);
		return ss.str();
	};
	
	
	ASSERT_EQ("null", test(BindValue::null));
	ASSERT_EQ("int32:123", test({ (int32_t)123}));
	ASSERT_EQ("int64:456", test({ (int64_t)456}));
	ASSERT_EQ("double:1.2", test({ (double)1.2}));
	ASSERT_EQ("string[10]:0123456789", test({ "0123456789" }));
	ASSERT_EQ("string[0]:", test({ "" }));
	ASSERT_EQ("string[40]:01234567890123456789012345678901...", test({ "0123456789012345678901234567890123456789" }));
}

TEST(BindValue, to_error_message__invalid_types__exception_thrown)
{
	std::stringstream ss;
	
	
	try
	{
		BindValue b { BindValue::type::TEXT_16 };
		b.set_str_value("abc");
		
		b.to_error_message(ss);
		
		FAIL();
	}
	catch (const SQLighterException& e)
	{
		ASSERT_EQ(SQLIGHTER_ERR_BIND, e.code());
	}
	
	
	try
	{
		BindValue b { BindValue::type::TEXT_64 };
		b.set_str_value("abc");
		
		b.to_error_message(ss);
		
		FAIL();
	}
	catch (const SQLighterException& e)
	{
		ASSERT_EQ(SQLIGHTER_ERR_BIND, e.code());
	}
}