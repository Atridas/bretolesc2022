module;

#include <cassert>

module Entitats;

using namespace bretolesc;

static bool motlles_generats = false;

static Entitat motlles[(int)TipusEntitat::NUM];

void bretolesc::generar_motlles()
{
	{
		bretolesc::Entitat jugador = {};
		jugador.posició = {-1, -1};
		jugador.caracter = '@';
		jugador.color = bretolesc::Color::Blanc;
		jugador.bloqueja_el_pas = true;

		motlles[(int)TipusEntitat::Jugador] = jugador;
	}

	{
		bretolesc::Entitat orc = {};
		orc.posició = {-1, -1};
		orc.caracter = 'o';
		orc.color = bretolesc::Color{ 63, 127, 63 };
		orc.bloqueja_el_pas = true;

		motlles[(int)TipusEntitat::Orc] = orc;
	}

	{
		bretolesc::Entitat trol = {};
		trol.posició = {-1, -1};
		trol.caracter = 'T';
		trol.color = bretolesc::Color{ 0, 127, 0 };
		trol.bloqueja_el_pas = true;

		motlles[(int)TipusEntitat::Trol] = trol;
	}

	motlles_generats = true;
}

Entitat bretolesc::obtenir_motlle(TipusEntitat tipus)
{
	assert((int)tipus >= 0);
	assert((int)tipus < (int)TipusEntitat::NUM);
	assert(motlles_generats);

	return motlles[(int)tipus];
}
