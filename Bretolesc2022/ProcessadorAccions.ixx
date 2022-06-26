export module ProcessadorAccions;

import Accions;
import EstatJoc;

export namespace acció
{
	using namespace bretolesc;

	void processar(Estat &estat, Finalitzar const &fin)
	{
		estat.tancar = true;
	}

	void processar(Estat& estat, Moure const& moure)
	{
		estat.player_x += moure.dx;
		estat.player_y += moure.dy;
	}
}
