export module Entitats;

import Com�;

export namespace bretolesc
{
	struct Entitat
	{
		Punt2D posici�;
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
