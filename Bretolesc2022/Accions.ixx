module;

#include <variant>

export module Accions;

export namespace acci�
{
	struct Finalitzar {};

	struct Moure
	{
		int dx, dy;
	};


	using Acci� = std::variant<Finalitzar, Moure>;
}
