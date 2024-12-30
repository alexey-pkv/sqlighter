#ifndef SQLIGHTER_EXCEPTIONS_H
#define SQLIGHTER_EXCEPTIONS_H


#include <exception>


namespace sqlighter
{
	class SQLighterException : public std::exception
	{
		
	};
	
	
	class StmtFinalizedException : public SQLighterException
	{
		
	};
}


#endif