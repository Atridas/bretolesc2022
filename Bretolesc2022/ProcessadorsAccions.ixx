module;

#include <cstdio>
#include <cmath>

export module Motor:ProcessadorsAccions;

import :Estat;

import EstructuresAccions;
import Entitats;

export namespace bretolesc
{
	using namespace acci�;
	using namespace component;

	void processar(Estat& estat, Finalitzar const &fin)
	{
		estat.tanca();
	}

	void processar(Estat& estat, MoureEntitat const& moure)
	{
		Punt2D posici�_seg�net = estat.obtenir_component<Localitzaci�>(moure.entitat).posici� + moure.direcci�;

		if (estat.mapa().�s_transitable(posici�_seg�net))
		{
			estat.obtenir_component<Localitzaci�>(moure.entitat).posici� = posici�_seg�net;
		}
	}

	void processar(Estat& estat, MoureJugador const& moure)
	{
		processar(estat, MoureEntitat{ estat.obtenir_id_jugador(), moure.direcci�});
	}

	void processar(Estat& estat, Acci�CosACos const& cos_a_cos)
	{
		Pintat const& p_atacant = estat.obtenir_component<Pintat>(cos_a_cos.entitat);
		Pintat& p_defensor = estat.obtenir_component<Pintat>(cos_a_cos.objectiu);

		Lluitador const& atacant = estat.obtenir_component<Lluitador>(cos_a_cos.entitat);
		Lluitador& defensor = estat.obtenir_component<Lluitador>(cos_a_cos.objectiu);

		printf("L'entitat %c ataca a l'entitat %c\n", p_atacant.caracter, p_defensor.caracter);


		int dany = std::max(atacant.for�a - defensor.defensa, 0);
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
		Punt2D posici�_seg�ent = estat.obtenir_component<Localitzaci�>(estat.obtenir_id_jugador()).posici� + batzegada.direcci�;
		if (auto enemic = estat.buscar_entitat_bloquejant(posici�_seg�ent))
		{
			// PERFER assegurar-se que l'enemic �s Lluitador
			processar(estat, Acci�CosACos{ estat.obtenir_id_jugador() , *enemic });
		}
		else
		{
			processar(estat, MoureJugador{ batzegada.direcci� });
		}
	}
}
