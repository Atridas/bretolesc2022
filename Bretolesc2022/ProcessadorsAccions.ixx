module;

#include <cstdio>
#include <cmath>

export module Motor:ProcessadorsAccions;

import :Estat;
import :PaletaDeColors;

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

	void processar(Estat& estat, Reiniciar const &fin)
	{
		estat.reinicia();
	}

	void processar(Estat& estat, NoFerRes const &fin)
	{
	}

	void processar(Estat& estat, MoureEntitat const& moure)
	{
		Punt2D posici�_seg�ent = estat.obtenir_component<Localitzaci�>(moure.entitat).posici� + moure.direcci�;

		if (estat.mapa().�s_transitable(posici�_seg�ent) && !estat.buscar_entitat_bloquejant(posici�_seg�ent))
		{
			estat.obtenir_component<Localitzaci�>(moure.entitat).posici� = posici�_seg�ent;
		}
	}

	void processar(Estat& estat, MoureJugador const& moure)
	{
		processar(estat, MoureEntitat{ estat.obtenir_id_jugador(), moure.direcci�});
	}

	void processar(Estat& estat, AtacCosACos const& cos_a_cos)
	{
		Nom const& nom_atacant = estat.obtenir_component<Nom>(cos_a_cos.entitat);
		Nom& nom_defensor = estat.obtenir_component<Nom>(cos_a_cos.objectiu);

		Lluitador const& atacant = estat.obtenir_component<Lluitador>(cos_a_cos.entitat);
		Lluitador& defensor = estat.obtenir_component<Lluitador>(cos_a_cos.objectiu);

		int dany = std::max(atacant.for�a - defensor.defensa, 0);


		char buffer[2048];
		sprintf_s(buffer, 2048, "%s ataca a %s, fent-li %d punts de dany", nom_atacant.nom.c_str(), nom_defensor.nom.c_str(), dany);

		estat.afegir_missatge(
			buffer,
			cos_a_cos.entitat == estat.obtenir_id_jugador() ? iu::Paleta::AtacJugador : iu::Paleta::AtacEnemic,
			true);

		if (dany >= defensor.salut)
		{
			defensor.salut = 0;
		}
		else
		{
			defensor.salut -= dany;
		}
	}

	void processar(Estat& estat, BatzegadaJugador const& batzegada)
	{
		Punt2D posici�_seg�ent = estat.obtenir_component<Localitzaci�>(estat.obtenir_id_jugador()).posici� + batzegada.direcci�;
		if (auto enemic = estat.buscar_entitat_bloquejant(posici�_seg�ent))
		{
			// PERFER assegurar-se que l'enemic �s Lluitador
			processar(estat, AtacCosACos{ estat.obtenir_id_jugador() , *enemic });
		}
		else
		{
			processar(estat, MoureJugador{ batzegada.direcci� });
		}
	}
}
