#pragma once


#include "ClauseContainer.h"


namespace sqlighter
{
	class ClauseWhere : public ClauseContainer
	{
	public:
		ClauseWhere();
		
		
	public:
		void where_null(std::string_view column);
		void where_not_null(std::string_view column);
		void by_field(std::string_view column, BindValue value);
	};
}


#define SQLIGHTER_WHERE_CLAUSE(data_member, self)						\
	public:																\
		SQLIGHTER_INLINE_CLAUSE(where, append_where, self);				\
																		\
	protected:															\
		inline self& append_where(										\
			std::string_view exp, const std::vector<BindValue>& bind)	\
		{																\
			data_member.append(exp, bind);								\
			return *this;												\
		}																\
																		\
	public:																\
		inline self& where_null(std::string_view column) 				\
		{ data_member.where_null(column); return *this; }				\
																		\
		inline self& where_not_null(std::string_view column)			\
		{ data_member.where_not_null(column); return *this; }			\
																		\
		inline self& by_field(std::string_view column, BindValue value)	\
		{ data_member.by_field(column, value); return *this; }

