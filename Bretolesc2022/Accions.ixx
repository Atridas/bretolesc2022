module;

// std
#include <variant>

export module Accions;

import Com�;
import EstatJoc;

export namespace bretolesc
{

	namespace acci�
	{
		struct Finalitzar {};

		struct MoureJugador
		{
			Vector2D direcci�;
		};

		struct MoureEntitat
		{
			IdEntitat entitat;
			Vector2D direcci�;
		};
	}

	using Acci� = std::variant<acci�::Finalitzar, acci�::MoureJugador, acci�::MoureEntitat>;
}
