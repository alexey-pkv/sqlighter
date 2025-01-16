#include "core/ScalarValue.h"


#include <fstream>
#include <gtest/gtest.h>


using namespace sqlighter;


TEST(ScalarValue_value, constructor_str)
{
	std::string str { "hello world" }; 
	ScalarValue::value v;
	
	
	v.make_string(str);
	
	
	ASSERT_EQ(str, v.str);
	ASSERT_TRUE(!v.str.empty());
}

TEST(ScalarValue_value, constructor_blob)
{
	std::vector<std::uint8_t> blob({ 0, 1, 2, 3 }); 
	ScalarValue::value v;
	
	
	v.make_blob(blob);
	
	
	ASSERT_EQ(4, v.blob.size());
}


TEST(ScalarValue, constructor__move__numeric_value)
{
	ScalarValue v1((int64_t)12);
	
	
	ScalarValue v2(std::move(v1));
	
	
	ASSERT_EQ(12, v2.get_int32());
	ASSERT_EQ(ScalarValue::type::INT, v2.get_type());
}

TEST(ScalarValue, equal__move__int64)
{
	ScalarValue v1((int64_t)12);
	ScalarValue v2((double)143.123);
	
	
	v2 = std::move(v1);
	
	
	ASSERT_EQ(12, v2.get_int64());
	ASSERT_EQ(ScalarValue::type::INT, v2.get_type());
}

TEST(ScalarValue, equal__move__double)
{
	ScalarValue v1((double)12.12);
	ScalarValue v2((int64_t)143);
	
	
	v2 = std::move(v1);
	
	
	ASSERT_EQ(12.12, v2.get_double());
	ASSERT_EQ(ScalarValue::type::DOUBLE, v2.get_type());
}

TEST(ScalarValue, equal__move__null)
{
	ScalarValue v1(ScalarValue::null());
	ScalarValue v2((double)143);
	
	
	v2 = std::move(v1);
	
	
	ASSERT_EQ(ScalarValue::type::NULL_VAL, v2.get_type());
}

TEST(ScalarValue, equal__move__str_value)
{
	ScalarValue v1("hello world");
	ScalarValue v2(blob_t { 1, 2, 3});
	
	
	v2 = std::move(v1);
	
	
	ASSERT_EQ("hello world", v2.get_str());
	ASSERT_EQ(ScalarValue::type::TEXT, v2.get_type());
}

TEST(ScalarValue, equal__move__blob_value)
{
	ScalarValue v1(blob_t { 1, 2, 3});
	ScalarValue v2("hello world");
	
	
	v2 = std::move(v1);
	
	
	ASSERT_EQ(3, v2.get_blob().size());
	ASSERT_EQ(1, v2.get_blob()[0]);
	ASSERT_EQ(2, v2.get_blob()[1]);
	ASSERT_EQ(3, v2.get_blob()[2]);
	ASSERT_EQ(ScalarValue::type::BLOB, v2.get_type());
}

TEST(ScalarValue, constructor__type__int64)
{
	ScalarValue t(ScalarValue::type::INT);
	ASSERT_EQ(ScalarValue::type::INT, t.get_type());
	ASSERT_EQ(0, t.get_value().i64);
}

TEST(ScalarValue, constructor__type__double)
{
	ScalarValue t(ScalarValue::type::DOUBLE);
	ASSERT_EQ(ScalarValue::type::DOUBLE, t.get_type());
	ASSERT_EQ(0.0, t.get_value().dbl);
}

TEST(ScalarValue, constructor__type__text)
{
	ScalarValue t(ScalarValue::type::TEXT);
	
	
	ASSERT_EQ(ScalarValue::type::TEXT, t.get_type());
	
	
	ASSERT_EQ("", t.get_value().str);
}

TEST(ScalarValue, constructor__type__blob)
{
	ScalarValue t(ScalarValue::type::BLOB);
	ASSERT_EQ(ScalarValue::type::BLOB, t.get_type());
	ASSERT_TRUE(t.get_value().blob.empty());
}

TEST(ScalarValue, constructor__type__null)
{
	ScalarValue t(ScalarValue::type::NULL_VAL);
	ASSERT_EQ(ScalarValue::type::NULL_VAL, t.get_type());
	ASSERT_EQ(0, t.get_value().i64);
}


TEST(ScalarValue, constructor__int32)
{
	ScalarValue t((int32_t)12);
	ASSERT_EQ(ScalarValue::type::INT, t.get_type());
	ASSERT_EQ(12, t.get_value().i64);
}

TEST(ScalarValue, constructor__int64)
{
	ScalarValue t((int64_t)12);
	ASSERT_EQ(ScalarValue::type::INT, t.get_type());
	ASSERT_EQ(12, t.get_value().i64);
}

TEST(ScalarValue, constructor__double)
{
	ScalarValue t((double)12.23);
	ASSERT_EQ(ScalarValue::type::DOUBLE, t.get_type());
	ASSERT_EQ(12.23, t.get_value().dbl);
}

TEST(ScalarValue, constructor__const_char)
{
	ScalarValue t((const char*)("hello world"));
	ASSERT_EQ(ScalarValue::type::TEXT, t.get_type());
	ASSERT_EQ("hello world", t.get_value().str);
}

TEST(ScalarValue, constructor__string)
{
	ScalarValue t(std::string { "hello world" });
	ASSERT_EQ(ScalarValue::type::TEXT, t.get_type());
	ASSERT_EQ("hello world", t.get_value().str);
}

TEST(ScalarValue, constructor__vector)
{
	ScalarValue t(std::vector<std::uint8_t>({ 0, 1, 2, 3 }));
	
	ASSERT_EQ(ScalarValue::type::BLOB, t.get_type());
	ASSERT_EQ(4, t.get_value().blob.size());
	ASSERT_EQ(0, t.get_value().blob[0]);
	ASSERT_EQ(1, t.get_value().blob[1]);
	ASSERT_EQ(2, t.get_value().blob[2]);
	ASSERT_EQ(3, t.get_value().blob[3]);
}

TEST(ScalarValue, constructor__void_ptr)
{
	std::vector<std::uint8_t> blob({ 0, 1, 2, 3 }); 
	ScalarValue t(blob.size(), static_cast<void*>(blob.data()));
	
	ASSERT_EQ(ScalarValue::type::BLOB, t.get_type());
	ASSERT_EQ(4, t.get_value().blob.size());
	ASSERT_EQ(0, t.get_value().blob[0]);
	ASSERT_EQ(1, t.get_value().blob[1]);
	ASSERT_EQ(2, t.get_value().blob[2]);
	ASSERT_EQ(3, t.get_value().blob[3]);
}

TEST(ScalarValue, constructor__void_ptr_empty)
{
	std::vector<std::uint8_t> blob({}); 
	ScalarValue t(blob.size(), static_cast<void*>(blob.data()));
	
	ASSERT_EQ(ScalarValue::type::BLOB, t.get_type());
	ASSERT_TRUE(t.get_value().blob.empty());
}


TEST(ScalarValue, is_null)
{
	ScalarValue t(ScalarValue::type::NULL_VAL);
	ASSERT_EQ(ScalarValue::type::NULL_VAL, t.get_type());
	ASSERT_TRUE(t.is_null());
	
	ScalarValue s(32);
	ASSERT_FALSE(s.is_null());
}


TEST(ScalarValue, get_double)
{
	double dbl;
	ScalarValue t(1.2);
	ScalarValue s("abc");
	ScalarValue n(ScalarValue::type::NULL_VAL);
	
	
	ASSERT_EQ(1.2, t.get_double(2.3));
	ASSERT_EQ(2.3, n.get_double(2.3));
	ASSERT_EQ(1.2, t.get<double>(2.3));
	ASSERT_EQ(2.3, n.get<double>(2.3));
	
	
	ASSERT_TRUE(t.try_get_double(dbl));
	ASSERT_EQ(1.2, dbl);
	
	dbl = 1.1;
	ASSERT_FALSE(n.try_get_double(dbl));
	ASSERT_EQ(1.1, dbl);
	
	
	ASSERT_TRUE(t.try_get<double>(dbl));
	ASSERT_EQ(1.2, dbl);
	
	dbl = 1.1;
	ASSERT_FALSE(n.try_get<double>(dbl));
	ASSERT_EQ(1.1, dbl);
}

TEST(ScalarValue, get_mxed_values)
{
	ScalarValue dbl(1.2);
	ScalarValue i64((int64_t)24);
	
	ASSERT_EQ(1,	dbl.get_int64());
	ASSERT_EQ(1,	dbl.get_int32());
	ASSERT_EQ(1.2,	dbl.get_double());
	
	ASSERT_EQ(24,	i64.get_int64());
	ASSERT_EQ(24,	i64.get_int32());
	ASSERT_EQ(24.0,	i64.get_double());
}

TEST(ScalarValue, get_null_value)
{
	ScalarValue n(ScalarValue::type::NULL_VAL);
	
	
	ASSERT_EQ(2.3, n.get_double(2.3));
	ASSERT_EQ(2.3, n.get<double>(2.3));
	
	ASSERT_EQ(12, n.get_int32(12));
	ASSERT_EQ(24, n.get<int32_t>(24));
	
	ASSERT_EQ(12, n.get_int64(12));
	ASSERT_EQ(24, n.get<int64_t>(24));
	
	ASSERT_TRUE(n.get_str().empty());
	ASSERT_TRUE(n.get<const std::string&>().empty());
	
	ASSERT_TRUE(n.get_blob().empty());
	ASSERT_TRUE(n.get<const blob_t&>().empty());
}

TEST(ScalarValue, try_get__different_type)
{
	ScalarValue s(std::string { "ok" });
	ScalarValue i(32);
	
	double d;
	int32_t i32;
	int64_t i64;
	std::string str;
	blob_t b;
	
	
	ASSERT_FALSE(s.try_get_double(d));
	ASSERT_FALSE(s.try_get<double>(d));
	
	ASSERT_FALSE(s.try_get_int32(i32));
	ASSERT_FALSE(s.try_get<int32_t>(i32));
	
	ASSERT_FALSE(s.try_get_int64(i64));
	ASSERT_FALSE(s.try_get<int64_t>(i64));
	
	ASSERT_FALSE(i.try_get_str(str));
	ASSERT_FALSE(i.try_get<std::string>(str));
	
	ASSERT_FALSE(s.try_get_blob(b));
	ASSERT_FALSE(s.try_get<blob_t>(b));
}

TEST(ScalarValue, try_get__null)
{
	ScalarValue s { ScalarValue::null() };
	
	double d;
	int32_t i32;
	int64_t i64;
	std::string str;
	blob_t b;
	
	
	ASSERT_FALSE(s.try_get_double(d));
	ASSERT_FALSE(s.try_get<double>(d));
	
	ASSERT_FALSE(s.try_get_int32(i32));
	ASSERT_FALSE(s.try_get<int32_t>(i32));
	
	ASSERT_FALSE(s.try_get_int64(i64));
	ASSERT_FALSE(s.try_get<int64_t>(i64));
	
	ASSERT_FALSE(s.try_get_str(str));
	ASSERT_FALSE(s.try_get<std::string>(str));
	
	ASSERT_FALSE(s.try_get_blob(b));
	ASSERT_FALSE(s.try_get<blob_t>(b));
}
