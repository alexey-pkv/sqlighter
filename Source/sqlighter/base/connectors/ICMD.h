#pragma once


#include <string>
#include <vector>


namespace sqlighter
{
	class Stmt;
	class BindValue;
	
	
	class ICMD
	{
	public:
		virtual ~ICMD() = default;
		
		
	public:
		virtual void assemble(std::stringstream& ss) const = 0;
		[[nodiscard]] virtual std::string assemble() const = 0;
		[[nodiscard]] virtual std::vector<BindValue> bind() const = 0;
		
		
	public:
		virtual Stmt execute() const = 0; // NOLINT(*-use-nodiscard)
	};
}
