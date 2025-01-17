#pragma once


#include "ClauseContainer.h"


namespace sqlighter
{
	enum class OrderBy;
	
	
	class ClauseOrderBy : public ClauseContainer
	{
	public:
		ClauseOrderBy();
		
		
	public:
		void order_by_field(std::string_view by, OrderBy order);
	};
}


#define SQLIGHTER_ORDER_BY_CLAUSE(data_member, self)					\
    protected:                                                      	\
		inline self& append_order_by(									\
			std::string_view exp, const std::vector<BindValue>& bind)	\
		{																\
			data_member.append(exp, bind);								\
			return *this;												\
		}																\
																		\
																		\
	public:																\
		SQLIGHTER_INLINE_CLAUSE(order_by, append_order_by, self);		\
																		\
		inline self& order_by_field(std::string_view by)				\
		{ data_member.order_by_field(by, OrderBy::ASC); return *this; }	\
        																\
		inline self& order_by_field(std::string_view by, OrderBy order)	\
		{ data_member.order_by_field(by, order); return *this; }		\
        																\
		inline self& order_by_field_asc(std::string_view by)			\
		{ data_member.order_by_field(by, OrderBy::ASC); return *this; }	\
        																\
		inline self& order_by_field_desc(std::string_view by)			\
		{ data_member.order_by_field(by, OrderBy::DESC); return *this; }

		
