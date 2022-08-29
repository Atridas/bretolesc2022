module;

#include <tuple>

export module Motlles;

import Entitats;
import Motor;
import Comú;

export namespace bretolesc
{
	enum class TipusEntitat
	{
		Jugador,
		Orc,
		Trol,

		PocióVida,

		NUM
	};

	IdEntitat afegir_entitat(Estat &estat, TipusEntitat tipus, Punt2D posició = {});
}
