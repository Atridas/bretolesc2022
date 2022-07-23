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

	struct Pintat
	{
		char caracter;
		Color color;
	};

	struct Lluitador
	{
		int salut, salut_màxima;
		int força, defensa;
	};

	// PERFER
	//namespace etiqueta
	//{
	//	struct BloquejaElPas {};
	//}
}
