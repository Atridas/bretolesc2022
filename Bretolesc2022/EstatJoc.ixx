module;

#include <cinttypes>

#include <optional>
#include <vector>

export module EstatJoc;

import Comú;

export namespace bretolesc
{
	struct Entitat
	{
		int x, y;
		char caracter;
		Color color;
	};

	using IdEntitat = int;

	struct Estat
	{
		IdEntitat jugador, npc;
		std::vector<Entitat> entitats;

		bool tancar;
	};
}

