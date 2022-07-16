module;

#include <tuple>

export module Entitats;

export import :Col�lecci�Components;
export import :Components;
export import :IdEntitat;

import Com�;

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
	std::tuple<component::Localitzaci�, component::Pintat> obtenir_motlle(TipusEntitat tipus);
}
