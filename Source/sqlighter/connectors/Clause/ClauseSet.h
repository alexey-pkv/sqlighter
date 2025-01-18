#pragma once


#include "ClauseContainer.h"


namespace sqlighter
{
	class ClauseSet : public ClauseContainer
	{
	public:
		ClauseSet();
		
		
	public:
		void set_field(std::string_view column, BindValue value);
		
		
	public:
		void append_to(std::ostream& to) const override;
	};
}


#define SQLIGHTER_SET_CLAUSE(data_member, self)							\
	public:																\
		SQLIGHTER_INLINE_CLAUSE(set_exp, append_set_exp, self);			\
																		\
	protected:															\
		inline self& append_set_exp(									\
			std::string_view exp, const std::vector<BindValue>& bind)	\
		{																\
			data_member.append(exp, bind);								\
			return *this;												\
		}																\
																		\
	public:																\
		inline self& set(std::string_view field, BindValue value)		\
		{ data_member.set_field(field, value); return *this; }			


