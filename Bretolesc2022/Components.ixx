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

	struct Pintat
	{
		char caracter;
		Color color;
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
