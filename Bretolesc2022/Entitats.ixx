module;

#include <tuple>

export module Entitats;

export import :Col·leccióComponents;
export import :Components;
export import :IdEntitat;

import Comú;

export namespace bretolesc
{
	enum class TipusEntitat
	{
		Jugador,
		Orc,
		Trol,

		NUM
	};

	void generar_motlles();
	std::tuple<component::Localització, component::Pintat> obtenir_motlle(TipusEntitat tipus);
}
