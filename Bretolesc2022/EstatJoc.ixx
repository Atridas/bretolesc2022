module;

#include <cinttypes>

#include <optional>
#include <vector>

export module EstatJoc;

import Com�;
import Mapa;

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
		Mapa mapa;
		IdEntitat jugador, npc;
		std::vector<Entitat> entitats;

		bool tancar;
	};
}

