#pragma once


#include "ICMD.h"
#include "connector_utils.h"


namespace sqlighter
{
	class ICMDDirect : public ICMD
	{
	public:
		~ICMDDirect() override = default;
		
		
	public:
		SQLIGHTER_EXPRESSION_CLAUSE(append,	do_append);
		
		
	public:
		inline ICMDDirect& operator<<(std::string_view exp) { do_append(exp, {}); return *this; }
		inline ICMDDirect& operator<<(std::span<const BindValue> binds) { do_append({}, binds); return *this; }
		
		template<class T>
		requires std::same_as<T, BindValue>
		inline ICMDDirect& operator<<(T bind) { do_append({}, { &bind, 1 }); return *this; }
	};
}