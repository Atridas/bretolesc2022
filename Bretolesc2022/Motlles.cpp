module;

// std
#include <cassert>

#include <tuple>

// 3rd party
#include <libtcod.hpp>

module Motlles;

using namespace bretolesc;
using namespace bretolesc::component;

IdEntitat bretolesc::afegir_entitat(Estat& estat, TipusEntitat tipus, Punt2D posici�)
{
	// - Crear component "lluitador" amb atributs: salut, salut m�xima, for�a i defensa

	switch (tipus)
	{
	case TipusEntitat::Jugador:
	{
		Localitzaci� localitzaci� = {};
		localitzaci�.posici� = posici�;
		localitzaci�.bloqueja_el_pas = true;

		Pintat pintat = {};
		pintat.caracter = '@';
		pintat.color = bretolesc::Color::Blanc;

		Lluitador lluitador;
		lluitador.salut = lluitador.salut_m�xima = 30;
		lluitador.defensa = 2;
		lluitador.for�a = 5;

		IdEntitat id = estat.crear_entitat();
		estat.afegir_component(id, localitzaci�);
		estat.afegir_component(id, pintat);
		estat.afegir_component(id, lluitador);
		return id;
	}
	case TipusEntitat::Orc:
		{
		bretolesc::component::Localitzaci� localitzaci� = {};
		localitzaci�.posici� = posici�;
		localitzaci�.bloqueja_el_pas = true;

		bretolesc::component::Pintat pintat = {};
		pintat.caracter = 'o';
		pintat.color = bretolesc::Color{ 63, 127, 63 };

		Lluitador lluitador;
		lluitador.salut = lluitador.salut_m�xima = 10;
		lluitador.defensa = 0;
		lluitador.for�a = 3;

		IAHostil ia_hostil;
		ia_hostil.cam� = TCOD_path_new_using_map(estat.mapa().obtenir_mapa_tcod(), 1.75f);

		IdEntitat id = estat.crear_entitat();
		estat.afegir_component(id, localitzaci�);
		estat.afegir_component(id, pintat);
		estat.afegir_component(id, lluitador);
		estat.afegir_component(id, ia_hostil);
		return id;
	}
	case TipusEntitat::Trol:
	{
		bretolesc::component::Localitzaci� localitzaci� = {};
		localitzaci�.posici� = posici�;
		localitzaci�.bloqueja_el_pas = true;

		bretolesc::component::Pintat pintat = {};
		pintat.caracter = 'T';
		pintat.color = bretolesc::Color{ 0, 127, 0 };

		Lluitador lluitador;
		lluitador.salut = lluitador.salut_m�xima = 16;
		lluitador.defensa = 1;
		lluitador.for�a = 4;

		IAHostil ia_hostil;
		ia_hostil.cam� = TCOD_path_new_using_map(estat.mapa().obtenir_mapa_tcod(), 3.f);

		IdEntitat id = estat.crear_entitat();
		estat.afegir_component(id, localitzaci�);
		estat.afegir_component(id, pintat);
		estat.afegir_component(id, lluitador);
		estat.afegir_component(id, ia_hostil);
		return id;
	}
	default:
		assert(false);
		return -1;
	}
	

	

	

}
