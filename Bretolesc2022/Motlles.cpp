module;

// std
#include <cassert>

#include <tuple>

// 3rd party
#include <libtcod.hpp>

module Motlles;

using namespace bretolesc;
using namespace bretolesc::component;
using namespace bretolesc::etiqueta;

IdEntitat bretolesc::afegir_entitat(Estat& estat, TipusEntitat tipus, Punt2D posici�)
{
	// - Crear component "lluitador" amb atributs: salut, salut m�xima, for�a i defensa

	switch (tipus)
	{
	case TipusEntitat::Jugador:
	{
		Nom nom = { "Jugador" };

		Localitzaci� localitzaci� = {};
		localitzaci�.posici� = posici�;

		Pintat pintat = {};
		pintat.car�cter = '@';
		pintat.color = bretolesc::Color::Blanc;
		pintat.prioritat = PrioritatPintar::Actor;

		Lluitador lluitador;
		lluitador.salut = lluitador.salut_m�xima = 30;
		lluitador.defensa = 2;
		lluitador.for�a = 5;

		IdEntitat id = estat.crear_entitat();
		estat.afegir_component(id, nom);
		estat.afegir_component(id, localitzaci�);
		estat.afegir_component(id, pintat);
		estat.afegir_component(id, lluitador);
		estat.afegir_etiqueta<BloquejaElPas>(id);
		return id;
	}
	case TipusEntitat::Orc:
	{
		Nom nom = { "Orc" };

		Localitzaci� localitzaci� = {};
		localitzaci�.posici� = posici�;

		Pintat pintat = {};
		pintat.car�cter = 'o';
		pintat.color = Color{ 63, 127, 63 };
		pintat.prioritat = PrioritatPintar::Actor;

		Lluitador lluitador;
		lluitador.salut = lluitador.salut_m�xima = 10;
		lluitador.defensa = 0;
		lluitador.for�a = 3;

		IAHostil ia_hostil;
		ia_hostil.cam� = TCOD_path_new_using_map(estat.mapa().obtenir_mapa_tcod(), 1.75f);

		IdEntitat id = estat.crear_entitat();
		estat.afegir_component(id, nom);
		estat.afegir_component(id, localitzaci�);
		estat.afegir_component(id, pintat);
		estat.afegir_component(id, lluitador);
		estat.afegir_component(id, ia_hostil);
		estat.afegir_etiqueta<BloquejaElPas>(id);
		return id;
	}
	case TipusEntitat::Trol:
	{
		Nom nom = { "Trol" };

		Localitzaci� localitzaci� = {};
		localitzaci�.posici� = posici�;

		Pintat pintat = {};
		pintat.car�cter = 'T';
		pintat.color = Color{ 0, 127, 0 };
		pintat.prioritat = PrioritatPintar::Actor;

		Lluitador lluitador;
		lluitador.salut = lluitador.salut_m�xima = 16;
		lluitador.defensa = 1;
		lluitador.for�a = 4;

		IAHostil ia_hostil;
		ia_hostil.cam� = TCOD_path_new_using_map(estat.mapa().obtenir_mapa_tcod(), 3.f);

		IdEntitat id = estat.crear_entitat();
		estat.afegir_component(id, nom);
		estat.afegir_component(id, localitzaci�);
		estat.afegir_component(id, pintat);
		estat.afegir_component(id, lluitador);
		estat.afegir_component(id, ia_hostil);
		estat.afegir_etiqueta<BloquejaElPas>(id);
		return id;
	}
	case TipusEntitat::Poci�Vida:
	{
		Nom nom = { "Pocio de Vida" };

		Localitzaci� localitzaci� = {};
		localitzaci�.posici� = posici�;

		Pintat pintat = {};
		pintat.car�cter = '!';
		pintat.color = Color{ 0x7f, 0x00, 0xff };
		pintat.prioritat = PrioritatPintar::Objecte;

		Curador curador = {};
		curador.vida = 4;

		IdEntitat id = estat.crear_entitat();
		estat.afegir_component(id, nom);
		estat.afegir_component(id, localitzaci�);
		estat.afegir_component(id, pintat);
		estat.afegir_component(id, curador);
		estat.afegir_etiqueta<Objecte>(id);
		estat.afegir_etiqueta<Consumible>(id);
		return id;
	}
	default:
		assert(false);
		return -1;
	}
	

	

	

}
