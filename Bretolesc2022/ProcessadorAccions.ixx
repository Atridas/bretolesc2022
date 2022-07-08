export module ProcessadorAccions;

import Accions;
import EstatJoc;

export namespace bretolesc
{
	using namespace acci�;

	void processar(Estat &estat, Finalitzar const &fin)
	{
		estat.tancar = true;
	}

	void processar(Estat& estat, MoureEntitat const& moure)
	{
		Punt2D posici�_seg�net = estat.entitats[moure.entitat].posici� + moure.direcci�;

		if (estat.mapa.�s_transitable(posici�_seg�net))
		{
			estat.entitats[moure.entitat].posici� = posici�_seg�net;
		}
	}

	void processar(Estat& estat, MoureJugador const& moure)
	{
		processar(estat, MoureEntitat{ estat.jugador, moure.direcci� });
		estat.actualitzar_visi�();
	}
}
