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

	// PERFER
	//namespace etiqueta
	//{
	//	struct BloquejaElPas {};
	//}
}
