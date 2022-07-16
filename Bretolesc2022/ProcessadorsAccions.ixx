module;

#include <cstdio>

export module Motor:ProcessadorsAccions;

import :Estat;

import EstructuresAccions;
import Entitats;

export namespace bretolesc
{
	using namespace acció;
	using namespace component;

	void processar(Estat& estat, Finalitzar const &fin)
	{
		estat.tanca();
	}

	void processar(Estat& estat, MoureEntitat const& moure)
	{
		Punt2D posició_segünet = estat.obtenir_component<Localització>(moure.entitat).posició + moure.direcció;

		if (estat.mapa().és_transitable(posició_segünet))
		{
			estat.obtenir_component<Localització>(moure.entitat).posició = posició_segünet;
		}
	}

	void processar(Estat& estat, MoureJugador const& moure)
	{
		processar(estat, MoureEntitat{ estat.obtenir_id_jugador(), moure.direcció});
	}

	void processar(Estat& estat, AccióCosACos const& cos_a_cos)
	{
		printf("L'entitat %c ataca a l'entitat %c\n", estat.obtenir_component<Pintat>(cos_a_cos.entitat).caracter, estat.obtenir_component<Pintat>(cos_a_cos.objectiu).caracter);
	}

	void processar(Estat& estat, BatzegadaJugador const& batzegada)
	{
		Punt2D posició_següent = estat.obtenir_component<Localització>(estat.obtenir_id_jugador()).posició + batzegada.direcció;
		if (auto enemic = estat.buscar_entitat_bloquejant(posició_següent))
		{
			processar(estat, AccióCosACos{ estat.obtenir_id_jugador() , *enemic });
		}
		else
		{
			processar(estat, MoureJugador{ batzegada.direcció });
		}
	}
}
