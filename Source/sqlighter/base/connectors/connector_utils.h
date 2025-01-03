#pragma once


#include <span>
#include <string_view>

#include "core/BindValue.h"


#define SQLIGHTER_EXPRESSION_CLAUSE(name, protected_name)																			\
	protected:																														\
		virtual void protected_name(std::string_view exp, std::span<const BindValue> span) = 0;										\
	public:																															\
		inline void name(std::string_view exp)											{ protected_name(exp, {}); };				\
		inline void name(std::string_view exp, BindValue value)							{ protected_name(exp, { &value, 1 }); };	\
		inline void name(std::string_view exp, std::initializer_list<BindValue> values)	{ protected_name(exp, values); };			\
		inline void name(std::string_view exp, std::span<const BindValue> values)		{ protected_name(exp, values); };




