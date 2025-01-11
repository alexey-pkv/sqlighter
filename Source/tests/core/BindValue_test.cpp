#include "core/BindValue.h"


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