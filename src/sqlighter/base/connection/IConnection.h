#ifndef SQLIGHTER_BASE_CONNECTION_ICONNECTION_H
#define SQLIGHTER_BASE_CONNECTION_ICONNECTION_H


#include <string>
#include <vector>
#include <memory>

#include "core/StmtRef.h"


namespace sqlighter
{
	class Stmt;
	class BindValue;
	
	
	class IConnection
	{
	public:
		virtual ~IConnection() = default; // LCOV_EXCL_LINE
		
		
	public:
		virtual Stmt execute(std::string_view query, const std::vector<BindValue>& values) = 0;
		virtual StmtRef prepare(std::string_view query) = 0;
	};
	
	
	class AbstractConnectionDecorator : public IConnection
	{
	private:
		std::shared_ptr<IConnection> m_next;
		
		
	protected:
		[[nodiscard]] inline std::shared_ptr<IConnection> next() { return m_next; }
		[[nodiscard]] inline std::shared_ptr<IConnection> next() const { return m_next; }
		
		Stmt execute_next(std::string_view query, const std::vector<BindValue>& values);
		
		
	public:
		explicit AbstractConnectionDecorator(const std::shared_ptr<IConnection>& next);
	};
	
	
}


#endif
