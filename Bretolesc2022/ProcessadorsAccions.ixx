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
	using namespace component;
	using namespace etiqueta;

	// accions entitats -----------------------------------------------------------------------------

	void processar(Estat& estat, acció_entitat::Moure const& moure)
	{
		Punt2D posició_següent = estat.obtenir_component<Localització>(moure.entitat).posició + moure.direcció;

		if (estat.mapa().és_transitable(posició_següent) && !estat.buscar_entitat_bloquejant(posició_següent))
		{
			estat.obtenir_component<Localització>(moure.entitat).posició = posició_següent;
		}
	}

	void processar(Estat& estat, acció_entitat::AtacCosACos const& cos_a_cos)
	{
		Nom const& nom_atacant = estat.obtenir_component<Nom>(cos_a_cos.entitat);
		Nom& nom_defensor = estat.obtenir_component<Nom>(cos_a_cos.objectiu);

		Lluitador const& atacant = estat.obtenir_component<Lluitador>(cos_a_cos.entitat);
		Lluitador& defensor = estat.obtenir_component<Lluitador>(cos_a_cos.objectiu);

		int dany = std::max(atacant.força - defensor.defensa, 0);


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

	// accions de l'usuari ----------------------------------------------------------------------

	void processar(Estat& estat, acció_usuari::Finalitzar const &fin)
	{
		estat.tanca();
	}

	void processar(Estat& estat, acció_usuari::Reiniciar const &fin)
	{
		estat.reinicia();
	}

	void processar(Estat& estat, acció_usuari::NoFerRes const &fin)
	{
		estat.actualitzar_lógica();
	}

	void processar(Estat& estat, acció_usuari::Moure const& moure)
	{
		processar(estat, acció_entitat::Moure{ estat.obtenir_id_jugador(), moure.direcció});

		estat.actualitzar_lógica();
	}

	void processar(Estat& estat, acció_usuari::Batzegada const& batzegada)
	{
		Punt2D posició_següent = estat.obtenir_component<Localització>(estat.obtenir_id_jugador()).posició + batzegada.direcció;
		if (auto enemic = estat.buscar_entitat_bloquejant(posició_següent))
		{
			// PERFER assegurar-se que l'enemic és Lluitador
			processar(estat, acció_entitat::AtacCosACos{ estat.obtenir_id_jugador() , *enemic });
		}
		else
		{
			processar(estat, acció_entitat::Moure{ estat.obtenir_id_jugador(), batzegada.direcció });
		}

		estat.actualitzar_lógica();
	}

	void processar(Estat& estat, acció_usuari::Agafar const& agafar)
	{
		IdEntitat jugador = estat.obtenir_id_jugador();
		Punt2D posició_jugador = estat.obtenir_component<Localització>(jugador).posició;
		for (IdEntitat objecte : estat.buscar_entitats(posició_jugador))
		{
			if (estat.té_etiqueta<Objecte>(objecte))
			{
				Inventari& inventari = estat.obtenir_component<Inventari>(jugador);
				if (inventari.objectes.size() < inventari.capacitat)
				{
					inventari.objectes.push_back(objecte);
					estat.treure_component<Localització>(objecte);
					estat.afegir_component(objecte, EnInventari{ jugador });
				}
			}
		}
	}

	void processar(Estat& estat, acció_usuari::MoureRatolí const& ratolí)
	{
		estat.moure_ratolí(ratolí.p);
	}

	void processar(Estat& estat, acció_usuari::AlternarRegistre const&)
	{
		estat.alterna_registre();
	}

	void processar(Estat& estat, acció_usuari::DesplaçarRegistre const& desplaçament)
	{
		estat.desplaçar_registre(desplaçament.línies);
	}
}
