#pragma once


#include <string>
#include <vector>

#include "core/Stmt.h"
#include "connector_utils.h"


namespace sqlighter
{
	class BindValue;
	
	
	class ICMD
	{
	public:
		virtual ~ICMD() = default;
		
		
	public:
		virtual void assemble(std::ostringstream& ss) const = 0;
		[[nodiscard]] virtual std::string assemble() const = 0;
		[[nodiscard]] virtual std::vector<BindValue> bind() const = 0;
		
		
	public:
		virtual Stmt execute() const = 0; // NOLINT(*-use-nodiscard)
	};
}
