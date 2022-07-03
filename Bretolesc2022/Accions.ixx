module;

// std
#include <variant>

export module Accions;

export namespace bretolesc
{

	namespace acció
	{
		struct Finalitzar {};

		struct MoureJugador
		{
			int dx, dy;
		};
	}

	using Acció = std::variant<acció::Finalitzar, acció::MoureJugador>;


}
