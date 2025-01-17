#pragma once


#include <string_view>

#include "core/BindValue.h"


#define SQLIGHTER_INLINE_CLAUSE(name, protected_name, self)																					\
	public:																																	\
		inline self& name(std::string_view exp)												{ return protected_name(exp, {}); };			\
		inline self& name(std::string_view exp, BindValue value)							{ return protected_name(exp, { value }); };		\
		inline self& name(std::string_view exp, std::initializer_list<BindValue> values)	{ return protected_name(exp, { values }); };	\
		inline self& name(std::string_view exp, const std::vector<BindValue>& values)		{ return protected_name(exp, values); }


