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
		Punt2D posició_segünet = estat.entitats[moure.entitat].posició + moure.direcció;

		if (estat.mapa.és_transitable(posició_segünet))
		{
			estat.entitats[moure.entitat].posició = posició_segünet;
		}
	}

	void processar(Estat& estat, MoureJugador const& moure)
	{
		processar(estat, MoureEntitat{ estat.jugador, moure.direcció });
		estat.actualitzar_visió();
	}
}
