#pragma once


#include <span>
#include <string_view>

#include "core/BindValue.h"


#define SQLIGHTER_INLINE_CLAUSE(name, protected_name, self)																						\
	public:																																			\
		inline self& name(std::string_view exp)													{ return  protected_name(exp, {}); };				\
		inline self& name(std::string_view exp, BindValue value)								{ return  protected_name(exp, { &value, 1 }); };	\
		inline self& name(std::string_view exp, std::initializer_list<const BindValue> values)	{ return  protected_name(exp, values); };			\
		inline self& name(std::string_view exp, std::span<const BindValue> values)				{ return  protected_name(exp, values); }

#define SQLIGHTER_EXPRESSION_CLAUSE(name, protected_name, self)																						\
	protected:																																		\
		virtual self& protected_name(std::string_view exp, std::span<const BindValue> span) = 0;													\
	public:																																			\
		SQLIGHTER_INLINE_CLAUSE(name, protected_name, self)



