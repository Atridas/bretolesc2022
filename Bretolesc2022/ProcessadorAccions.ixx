export module ProcessadorAccions;

import Accions;
import EstatJoc;

export namespace bretolesc
{
	using namespace acció;

	void processar(Estat &estat, Finalitzar const &fin)
	{
		estat.tancar = true;
	}

	void processar(Estat& estat, MoureJugador const& moure)
	{
		estat.entitats[estat.jugador].x += moure.dx;
		estat.entitats[estat.jugador].y += moure.dy;
	}
}
