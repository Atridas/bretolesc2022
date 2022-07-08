module;

// std
#include <variant>

export module Accions;

import Comú;
import EstatJoc;

export namespace bretolesc
{

	namespace acció
	{
		struct Finalitzar {};

		struct MoureJugador
		{
			Vector2D direcció;
		};

		struct MoureEntitat
		{
			IdEntitat entitat;
			Vector2D direcció;
		};
	}

	using Acció = std::variant<acció::Finalitzar, acció::MoureJugador, acció::MoureEntitat>;
}
