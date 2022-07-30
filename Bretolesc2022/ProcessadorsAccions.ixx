module;

#include <cstdio>
#include <cmath>

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
		Pintat const& p_atacant = estat.obtenir_component<Pintat>(cos_a_cos.entitat);
		Pintat& p_defensor = estat.obtenir_component<Pintat>(cos_a_cos.objectiu);

		Lluitador const& atacant = estat.obtenir_component<Lluitador>(cos_a_cos.entitat);
		Lluitador& defensor = estat.obtenir_component<Lluitador>(cos_a_cos.objectiu);

		printf("L'entitat %c ataca a l'entitat %c\n", p_atacant.caracter, p_defensor.caracter);


		int dany = std::max(atacant.força - defensor.defensa, 0);
		if (dany >= defensor.salut)
		{
			// PERFER morir
			printf("L'entitat %c ha mort!\n", p_defensor.caracter);
		}
		else
		{
			defensor.salut -= dany;
			printf("L'entitat %c ha rebut %d de dany!\n", p_defensor.caracter, dany);
		}
	}

	void processar(Estat& estat, BatzegadaJugador const& batzegada)
	{
		Punt2D posició_següent = estat.obtenir_component<Localització>(estat.obtenir_id_jugador()).posició + batzegada.direcció;
		if (auto enemic = estat.buscar_entitat_bloquejant(posició_següent))
		{
			// PERFER assegurar-se que l'enemic és Lluitador
			processar(estat, AccióCosACos{ estat.obtenir_id_jugador() , *enemic });
		}
		else
		{
			processar(estat, MoureJugador{ batzegada.direcció });
		}
	}
}
