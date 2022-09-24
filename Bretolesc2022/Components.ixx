module;

// 3rd party
#include <libtcod.hpp>

export module Entitats:Components;

import :IdEntitat;

import Comú;

export namespace bretolesc::component
{
	struct Localització
	{
		Punt2D posició;
		// pis?
	};

	enum class PrioritatPintar
	{
		Cadàver,
		Objecte,
		Actor
	};

	struct Pintat
	{
		char caràcter;
		Color color;
		PrioritatPintar prioritat;
	};

	struct Nom
	{
		std::string nom;
	};

	struct Lluitador
	{
		int salut, salut_màxima;
		int força, defensa;
	};

	struct IAHostil
	{
		TCOD_path_t camí;
	};

	struct Confús
	{
		int torns;
	};

	struct ObjectiuLluitadorProper
	{
		int rang;
	};

	struct ObjectiuCursor
	{
		int rang;
		int radi;
	};

	struct Curador
	{
		int vida;
	};

	struct EncanteriDeDany
	{
		int dany;
	};

	struct EncanteriDeConfusió
	{
		int torns;
	};

	struct Inventari
	{
		int capacitat;
		std::vector<IdEntitat> objectes;
	};

	struct EnInventari
	{
		IdEntitat propietàri;
	};
}
