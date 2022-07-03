module;

// std
#include <variant>

export module Accions;

export namespace bretolesc
{

	namespace acci�
	{
		struct Finalitzar {};

		struct MoureJugador
		{
			int dx, dy;
		};
	}

	using Acci� = std::variant<acci�::Finalitzar, acci�::MoureJugador>;


}
