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

IdEntitat bretolesc::afegir_entitat(Estat& estat, TipusEntitat tipus, Punt2D posició)
{
	// - Crear component "lluitador" amb atributs: salut, salut màxima, força i defensa

	switch (tipus)
	{
	case TipusEntitat::Jugador:
	{
		Nom nom = { "Jugador" };

		Localització localització = {};
		localització.posició = posició;

		Pintat pintat = {};
		pintat.caràcter = '@';
		pintat.color = bretolesc::Color::Blanc;
		pintat.prioritat = PrioritatPintar::Actor;

		Lluitador lluitador;
		lluitador.salut = lluitador.salut_màxima = 30;
		lluitador.defensa = 2;
		lluitador.força = 5;

		Inventari inventari;
		inventari.capacitat = 26;

		IdEntitat id = estat.crear_entitat();
		estat.afegir_component(id, nom);
		estat.afegir_component(id, localització);
		estat.afegir_component(id, pintat);
		estat.afegir_component(id, lluitador);
		estat.afegir_component(id, inventari);
		estat.afegir_etiqueta<BloquejaElPas>(id);
		return id;
	}
	case TipusEntitat::Orc:
	{
		Nom nom = { "Orc" };

		Localització localització = {};
		localització.posició = posició;

		Pintat pintat = {};
		pintat.caràcter = 'o';
		pintat.color = Color{ 63, 127, 63 };
		pintat.prioritat = PrioritatPintar::Actor;

		Lluitador lluitador;
		lluitador.salut = lluitador.salut_màxima = 10;
		lluitador.defensa = 0;
		lluitador.força = 3;

		IAHostil ia_hostil;
		ia_hostil.camí = TCOD_path_new_using_map(estat.mapa().obtenir_mapa_tcod(), 1.75f);

		IdEntitat id = estat.crear_entitat();
		estat.afegir_component(id, nom);
		estat.afegir_component(id, localització);
		estat.afegir_component(id, pintat);
		estat.afegir_component(id, lluitador);
		estat.afegir_component(id, ia_hostil);
		estat.afegir_etiqueta<BloquejaElPas>(id);
		return id;
	}
	case TipusEntitat::Trol:
	{
		Nom nom = { "Trol" };

		Localització localització = {};
		localització.posició = posició;

		Pintat pintat = {};
		pintat.caràcter = 'T';
		pintat.color = Color{ 0, 127, 0 };
		pintat.prioritat = PrioritatPintar::Actor;

		Lluitador lluitador;
		lluitador.salut = lluitador.salut_màxima = 16;
		lluitador.defensa = 1;
		lluitador.força = 4;

		IAHostil ia_hostil;
		ia_hostil.camí = TCOD_path_new_using_map(estat.mapa().obtenir_mapa_tcod(), 3.f);

		IdEntitat id = estat.crear_entitat();
		estat.afegir_component(id, nom);
		estat.afegir_component(id, localització);
		estat.afegir_component(id, pintat);
		estat.afegir_component(id, lluitador);
		estat.afegir_component(id, ia_hostil);
		estat.afegir_etiqueta<BloquejaElPas>(id);
		return id;
	}
	case TipusEntitat::PocióVida:
	{
		Nom nom = { "Pocio de Vida" };

		Localització localització = {};
		localització.posició = posició;

		Pintat pintat = {};
		pintat.caràcter = '!';
		pintat.color = Color{ 0x7f, 0x00, 0xff };
		pintat.prioritat = PrioritatPintar::Objecte;

		Curador curador = {};
		curador.vida = 4;

		IdEntitat id = estat.crear_entitat();
		estat.afegir_component(id, nom);
		estat.afegir_component(id, localització);
		estat.afegir_component(id, pintat);
		estat.afegir_component(id, curador);
		estat.afegir_etiqueta<Objecte>(id);
		estat.afegir_etiqueta<Consumible>(id);
		return id;
	}
	case TipusEntitat::RotlloDelLlamp:
	{
		Nom nom = { "Rotllo del Llamp" };

		Localització localització = {};
		localització.posició = posició;

		Pintat pintat = {};
		pintat.caràcter = '~';
		pintat.color = Color{ 0xff, 0xff, 0x00 };
		pintat.prioritat = PrioritatPintar::Objecte;

		EncanteriDelLlamp encanteri_del_llamp = {};
		encanteri_del_llamp.dany = 20;
		encanteri_del_llamp.rang = 5;

		IdEntitat id = estat.crear_entitat();
		estat.afegir_component(id, nom);
		estat.afegir_component(id, localització);
		estat.afegir_component(id, pintat);
		estat.afegir_component(id, encanteri_del_llamp);
		estat.afegir_etiqueta<Objecte>(id);
		estat.afegir_etiqueta<Consumible>(id);
		estat.afegir_etiqueta<AvançaTorn>(id);
		return id;
	}
	case TipusEntitat::RotlloDeConfusió:
	{
		Nom nom = { "Rotllo de Confusio" };

		Localització localització = {};
		localització.posició = posició;

		Pintat pintat = {};
		pintat.caràcter = '~';
		pintat.color = Color{ 207, 63, 255 };
		pintat.prioritat = PrioritatPintar::Objecte;

		EncanteriDeConfusió encanteri_de_confusió = {};
		encanteri_de_confusió.torns = 10;
		encanteri_de_confusió.rang = 5;

		IdEntitat id = estat.crear_entitat();
		estat.afegir_component(id, nom);
		estat.afegir_component(id, localització);
		estat.afegir_component(id, pintat);
		estat.afegir_component(id, encanteri_de_confusió);
		estat.afegir_etiqueta<Objecte>(id);
		estat.afegir_etiqueta<Consumible>(id);
		estat.afegir_etiqueta<AvançaTorn>(id);
		return id;
	}
	default:
		assert(false);
		return -1;
	}
	

	

	

}
