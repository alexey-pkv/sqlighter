#pragma once


#include <memory>

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
		
		inline void append(std::initializer_list<const BindValue> values) { do_append({}, values); };
		
		
	public:
		inline ICMDDirect& operator<<(std::string_view exp) { do_append(exp, {}); return *this; }
		inline ICMDDirect& operator<<(std::span<const BindValue> binds) { do_append({}, binds); return *this; }
		
		template<class T>
		requires std::same_as<T, BindValue>
		inline ICMDDirect& operator<<(T bind) { do_append({}, { &bind, 1 }); return *this; }
		
		
	public:
		virtual void execute_direct() const = 0;
	};
	
	
	inline std::shared_ptr<ICMDDirect> operator<<(std::shared_ptr<ICMDDirect> cmd, std::span<const BindValue> binds) 
	{
		cmd->append({}, binds);
		return cmd;
	}
	
	inline std::shared_ptr<ICMDDirect> operator<<(std::shared_ptr<ICMDDirect> cmd, std::string_view exp) 
	{
		cmd->append(exp, {});
		return cmd;
	}
	
	template<class T>
	requires std::same_as<T, BindValue>
	inline std::shared_ptr<ICMDDirect> operator<<(std::shared_ptr<ICMDDirect> cmd, T bind) 
	{
		cmd->append({}, { &bind, 1 });
		return cmd;
	}
}