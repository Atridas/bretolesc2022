module;

// std
#include <cassert>

#include <tuple>

module Entitats;

using namespace bretolesc;

static bool motlles_generats = false;

static component::Localització motlles_localitzacions[(int)TipusEntitat::NUM];
static component::Pintat motlles_pintats[(int)TipusEntitat::NUM];

void bretolesc::generar_motlles()
{
	{
		bretolesc::component::Localització jugador_loc = {};
		bretolesc::component::Pintat jugador_pin = {};
		jugador_loc.posició = {-1, -1};
		jugador_loc.bloqueja_el_pas = true;
		jugador_pin.caracter = '@';
		jugador_pin.color = bretolesc::Color::Blanc;

		motlles_localitzacions[(int)TipusEntitat::Jugador] = jugador_loc;
		motlles_pintats[(int)TipusEntitat::Jugador] = jugador_pin;
	}

	{
		bretolesc::component::Localització orc_loc = {};
		bretolesc::component::Pintat orc_pin = {};
		orc_loc.posició = {-1, -1};
		orc_loc.bloqueja_el_pas = true;
		orc_pin.caracter = 'o';
		orc_pin.color = bretolesc::Color{ 63, 127, 63 };

		motlles_localitzacions[(int)TipusEntitat::Orc] = orc_loc;
		motlles_pintats[(int)TipusEntitat::Orc] = orc_pin;
	}

	{
		bretolesc::component::Localització trol_loc = {};
		bretolesc::component::Pintat trol_pin = {};
		trol_loc.posició = {-1, -1};
		trol_loc.bloqueja_el_pas = true;
		trol_pin.caracter = 'T';
		trol_pin.color = bretolesc::Color{ 0, 127, 0 };

		motlles_localitzacions[(int)TipusEntitat::Trol] = trol_loc;
		motlles_pintats[(int)TipusEntitat::Trol] = trol_pin;
	}

	motlles_generats = true;
}

std::tuple<component::Localització, component::Pintat> bretolesc::obtenir_motlle(TipusEntitat tipus)
{
	assert((int)tipus >= 0);
	assert((int)tipus < (int)TipusEntitat::NUM);
	assert(motlles_generats);

	return std::make_tuple(motlles_localitzacions[(int)tipus], motlles_pintats[(int)tipus]);
}
