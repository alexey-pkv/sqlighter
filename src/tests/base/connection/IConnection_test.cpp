#include "base/connection/IConnection.h"

#include "exceptions/sqlighter_exceptions.h"
#include "connection/Connection.h"


#include <gtest/gtest.h>


using namespace sqlighter;


class TestConnection : public IConnection
{
public:
	bool					Called	{ false };
	std::string				Query	{};
	std::vector<BindValue>	Binds 	{};
	
	
	Stmt execute(std::string_view query, const std::vector<BindValue>& values) override
	{
		Called = true;
		Query = query;
		Binds = values;
		
		return Stmt();
	}
	
	Stmt prepare(std::string_view query) override
	{
		Called = true;
		Query = query;
		
		return Stmt();
	}
	
	void execute_prepared(Stmt& stmt, const std::vector<BindValue>& values) override
	{
		Called = true;
		Query = stmt.query();
		Binds = values;
	}
};

class TestAbstractConnectionDecorator : public AbstractConnectionDecorator
{
public:
	explicit TestAbstractConnectionDecorator(const std::shared_ptr<IConnection>& next) : 
		AbstractConnectionDecorator(next)
	{}
	
	Stmt execute(std::string_view query, const std::vector<BindValue>& values) override
	{
		return Stmt();
	}
	
	Stmt prepare(std::string_view query) override
	{
		return Stmt();
	}
	
	void execute_prepared(Stmt& stmt, const std::vector<BindValue>& values) override
	{
		
	}
	
	Stmt call_execute_next(std::string_view query, const std::vector<BindValue>& values)
	{
		return execute_next(query, values);
	}
	
	inline std::shared_ptr<IConnection> call_next() { return next(); }
	inline const std::shared_ptr<IConnection> call_next_const() const { return next(); }
};


TEST(AbstractConnectionDecorator, next)
{
	auto c = std::make_shared<Connection>("some path");
	TestAbstractConnectionDecorator a(c);
	
	
	ASSERT_EQ(c, a.call_next());
	ASSERT_EQ(c, a.call_next_const());
}

TEST(AbstractConnectionDecorator, constructor__null__error_thrown)
{
	try
	{
		TestAbstractConnectionDecorator a(nullptr);
		FAIL();
	}
	catch (const SQLighterException& e)
	{
		ASSERT_EQ(SQLIGHTER_ERR_UNEXPECTED, e.code());
	}
}


TEST(AbstractConnectionDecorator, next_next_called)
{
	auto c = std::make_shared<TestConnection>();
	TestAbstractConnectionDecorator a(c);
	
	
	a.call_execute_next("abc", { 1, 2.2 });
	
	
	ASSERT_TRUE(c->Called);
	ASSERT_EQ("abc", c->Query);
	ASSERT_EQ(1, c->Binds[0].get_value().i32);
	ASSERT_EQ(2.2, c->Binds[1].get_value().dbl);
}