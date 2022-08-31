module;

#include <cassert>
#include <cmath>
#include <cstdio>

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

	void processar(Estat& estat, acció_entitat::RecuperarVida const& recuperar_vida)
	{
		assert(recuperar_vida.vida > 0);

		Nom const& nom_entitat = estat.obtenir_component<Nom>(recuperar_vida.entitat);
		Lluitador& entitat = estat.obtenir_component<Lluitador>(recuperar_vida.entitat);

		int vida_inicial = entitat.salut;
		entitat.salut += recuperar_vida.vida;
		if (entitat.salut > entitat.salut_màxima)
			entitat.salut = entitat.salut_màxima;


		char buffer[2048];
		sprintf_s(buffer, 2048, "%s recupera %d punts de vida", nom_entitat.nom.c_str(), entitat.salut - vida_inicial);

		estat.afegir_missatge(
			buffer,
			iu::Paleta::TextVidaRecuperada,
			true);
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
				Nom const& nom_objecte = estat.obtenir_component<Nom>(objecte);

				char buffer[2048];
				Color color_log;
				if (inventari.objectes.size() < inventari.capacitat)
				{
					inventari.objectes.push_back(objecte);
					estat.treure_component<Localització>(objecte);
					estat.afegir_component(objecte, EnInventari{ jugador });

					sprintf_s(buffer, 2048, "Agafat un objecte %s", nom_objecte.nom.c_str());
					color_log = iu::Paleta::TextManipularObjecte;
				}
				else
				{
					sprintf_s(buffer, 2048, "Inventari ple! No podem agafar un objecte %s", nom_objecte.nom.c_str());
					color_log = iu::Paleta::TextAccióInvàlida;
				}

				estat.afegir_missatge(
					buffer,
					color_log,
					true);
			}
		}
	}

	void processar(Estat& estat, acció_usuari::Tirar const& tirar)
	{
		IdEntitat jugador = estat.obtenir_id_jugador();
		Localització localització_jugador = estat.obtenir_component<Localització>(jugador);
		Inventari& inventari = estat.obtenir_component<Inventari>(jugador);
		int posició_inventari = estat.obtenir_posició_inventari();
		if (posició_inventari < inventari.objectes.size())
		{
			IdEntitat objecte = inventari.objectes[posició_inventari];
			inventari.objectes.erase(inventari.objectes.begin() + posició_inventari);
			estat.afegir_component(objecte, localització_jugador);
			estat.treure_component<EnInventari>(objecte);

			if (posició_inventari > 0)
				estat.desplaçar_inventari(-1);

			Nom const& nom_objecte = estat.obtenir_component<Nom>(objecte);
			char buffer[2048];
			sprintf_s(buffer, 2048, "Tirem un objecte %s", nom_objecte.nom.c_str());
			estat.afegir_missatge(
				buffer,
				iu::Paleta::TextManipularObjecte,
				true);
		}
	}

	void processar(Estat& estat, acció_usuari::Consumir const& consumir)
	{
		IdEntitat jugador = estat.obtenir_id_jugador();
		Inventari& inventari = estat.obtenir_component<Inventari>(jugador);
		int posició_inventari = estat.obtenir_posició_inventari();
		if (posició_inventari < inventari.objectes.size())
		{
			IdEntitat objecte = inventari.objectes[posició_inventari];
			if (estat.té_etiqueta<Consumible>(objecte))
			{
				if (auto curador = estat.potser_obtenir_component<Curador>(objecte))
				{
					processar(estat, acció_entitat::RecuperarVida
						{
							jugador,
							curador->get().vida
						});
				}
				// PERFER Més efectes?

				// eliminar objecte
				assert(!estat.té_component<IAHostil>(objecte));
				inventari.objectes.erase(inventari.objectes.begin() + posició_inventari);
				estat.modificar_o_treure([](auto c) { return true; }, objecte);

				if (posició_inventari > 0)
					estat.desplaçar_inventari(-1);
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

	void processar(Estat& estat, acció_usuari::AlternarInventari const&)
	{
		estat.alterna_inventari();
	}

	void processar(Estat& estat, acció_usuari::DesplaçarInventari const& desplaçament)
	{
		estat.desplaçar_inventari(desplaçament.línies);
	}
}
