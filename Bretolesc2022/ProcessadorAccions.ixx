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

	void processar(Estat& estat, MoureEntitat const& moure)
	{
		int next_x = estat.entitats[moure.entitat].x + moure.dx;
		int next_y = estat.entitats[moure.entitat].y + moure.dy;

		if (estat.mapa.és_caminable(next_x, next_y))
		{
			estat.entitats[moure.entitat].x = next_x;
			estat.entitats[moure.entitat].y = next_y;
		}
	}

	void processar(Estat& estat, MoureJugador const& moure)
	{
		processar(estat, MoureEntitat{ estat.jugador, moure.dx, moure.dy });
	}
}
