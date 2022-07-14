export module Entitats;

import Comú;

export namespace bretolesc
{
	struct Entitat
	{
		Punt2D posició;
		char caracter;
		Color color;
		bool bloqueja_el_pas;
	};

	using IdEntitat = int;


	enum class TipusEntitat
	{
		Jugador,
		Orc,
		Trol,

		NUM
	};

	void generar_motlles();
	Entitat obtenir_motlle(TipusEntitat tipus);
}
