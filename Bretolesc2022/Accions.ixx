module;

// std
#include <variant>

export module Accions;

import EstatJoc;

export namespace bretolesc
{

	namespace acció
	{
		struct Finalitzar {};

		struct MoureJugador
		{
			int dx, dy;
		};

		struct MoureEntitat
		{
			IdEntitat entitat;
			int dx, dy;
		};
	}

	using Acció = std::variant<acció::Finalitzar, acció::MoureJugador, acció::MoureEntitat>;


}
