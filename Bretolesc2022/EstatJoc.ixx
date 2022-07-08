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
		Punt2D posici�;
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

		static const int profunditat_de_visi� = 8;

		void actualitzar_visi�();
	};
}

