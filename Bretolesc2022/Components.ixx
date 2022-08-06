module;

// 3rd party
#include <libtcod.hpp>

export module Entitats:Components;

import Com�;

export namespace bretolesc::component
{
	struct Localitzaci�
	{
		Punt2D posici�;
		// pis?
		bool bloqueja_el_pas;
	};

	enum class PrioritatPintar
	{
		Cad�ver,
		Objecte,
		Actor
	};

	struct Pintat
	{
		char car�cter;
		Color color;
		PrioritatPintar prioritat;
	};

	struct Nom
	{
		std::string nom;
	};

	struct Lluitador
	{
		int salut, salut_m�xima;
		int for�a, defensa;
	};

	struct IAHostil
	{
		TCOD_path_t cam�;
	};

	// PERFER
	//namespace etiqueta
	//{
	//	struct BloquejaElPas {};
	//}
}
