module;

#include <tuple>

export module Motlles;

import Entitats;
import Motor;
import Com�;

export namespace bretolesc
{
	enum class TipusEntitat
	{
		Jugador,
		Orc,
		Trol,

		Poci�Vida,

		NUM
	};

	IdEntitat afegir_entitat(Estat &estat, TipusEntitat tipus, Punt2D posici� = {});
}
