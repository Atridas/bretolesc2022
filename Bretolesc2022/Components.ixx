module;

// 3rd party
#include <libtcod.hpp>

export module Entitats:Components;

import Comú;

export namespace bretolesc::component
{
	struct Localització
	{
		Punt2D posició;
		// pis?
		bool bloqueja_el_pas;
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

	// PERFER
	//namespace etiqueta
	//{
	//	struct BloquejaElPas {};
	//}
}
