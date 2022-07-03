module;

// std
#include <variant>

export module Accions;

import EstatJoc;

export namespace bretolesc
{

	namespace acci�
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

	using Acci� = std::variant<acci�::Finalitzar, acci�::MoureJugador, acci�::MoureEntitat>;


}
