module;

#include <variant>

export module Accions;

export namespace acció
{
	struct Finalitzar {};

	struct Moure
	{
		int dx, dy;
	};


	using Acció = std::variant<Finalitzar, Moure>;
}
