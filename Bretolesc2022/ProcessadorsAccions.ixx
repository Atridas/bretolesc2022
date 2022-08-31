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

	void processar(Estat& estat, acci�_entitat::Moure const& moure)
	{
		Punt2D posici�_seg�ent = estat.obtenir_component<Localitzaci�>(moure.entitat).posici� + moure.direcci�;

		if (estat.mapa().�s_transitable(posici�_seg�ent) && !estat.buscar_entitat_bloquejant(posici�_seg�ent))
		{
			estat.obtenir_component<Localitzaci�>(moure.entitat).posici� = posici�_seg�ent;
		}
	}

	void processar(Estat& estat, acci�_entitat::AtacCosACos const& cos_a_cos)
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

	void processar(Estat& estat, acci�_entitat::RecuperarVida const& recuperar_vida)
	{
		assert(recuperar_vida.vida > 0);

		Nom const& nom_entitat = estat.obtenir_component<Nom>(recuperar_vida.entitat);
		Lluitador& entitat = estat.obtenir_component<Lluitador>(recuperar_vida.entitat);

		int vida_inicial = entitat.salut;
		entitat.salut += recuperar_vida.vida;
		if (entitat.salut > entitat.salut_m�xima)
			entitat.salut = entitat.salut_m�xima;


		char buffer[2048];
		sprintf_s(buffer, 2048, "%s recupera %d punts de vida", nom_entitat.nom.c_str(), entitat.salut - vida_inicial);

		estat.afegir_missatge(
			buffer,
			iu::Paleta::TextVidaRecuperada,
			true);
	}

	// accions de l'usuari ----------------------------------------------------------------------

	void processar(Estat& estat, acci�_usuari::Finalitzar const &fin)
	{
		estat.tanca();
	}

	void processar(Estat& estat, acci�_usuari::Reiniciar const &fin)
	{
		estat.reinicia();
	}

	void processar(Estat& estat, acci�_usuari::NoFerRes const &fin)
	{
		estat.actualitzar_l�gica();
	}

	void processar(Estat& estat, acci�_usuari::Moure const& moure)
	{
		processar(estat, acci�_entitat::Moure{ estat.obtenir_id_jugador(), moure.direcci�});

		estat.actualitzar_l�gica();
	}

	void processar(Estat& estat, acci�_usuari::Batzegada const& batzegada)
	{
		Punt2D posici�_seg�ent = estat.obtenir_component<Localitzaci�>(estat.obtenir_id_jugador()).posici� + batzegada.direcci�;
		if (auto enemic = estat.buscar_entitat_bloquejant(posici�_seg�ent))
		{
			// PERFER assegurar-se que l'enemic �s Lluitador
			processar(estat, acci�_entitat::AtacCosACos{ estat.obtenir_id_jugador() , *enemic });
		}
		else
		{
			processar(estat, acci�_entitat::Moure{ estat.obtenir_id_jugador(), batzegada.direcci� });
		}

		estat.actualitzar_l�gica();
	}

	void processar(Estat& estat, acci�_usuari::Agafar const& agafar)
	{
		IdEntitat jugador = estat.obtenir_id_jugador();
		Punt2D posici�_jugador = estat.obtenir_component<Localitzaci�>(jugador).posici�;
		for (IdEntitat objecte : estat.buscar_entitats(posici�_jugador))
		{
			if (estat.t�_etiqueta<Objecte>(objecte))
			{
				Inventari& inventari = estat.obtenir_component<Inventari>(jugador);
				Nom const& nom_objecte = estat.obtenir_component<Nom>(objecte);

				char buffer[2048];
				Color color_log;
				if (inventari.objectes.size() < inventari.capacitat)
				{
					inventari.objectes.push_back(objecte);
					estat.treure_component<Localitzaci�>(objecte);
					estat.afegir_component(objecte, EnInventari{ jugador });

					sprintf_s(buffer, 2048, "Agafat un objecte %s", nom_objecte.nom.c_str());
					color_log = iu::Paleta::TextManipularObjecte;
				}
				else
				{
					sprintf_s(buffer, 2048, "Inventari ple! No podem agafar un objecte %s", nom_objecte.nom.c_str());
					color_log = iu::Paleta::TextAcci�Inv�lida;
				}

				estat.afegir_missatge(
					buffer,
					color_log,
					true);
			}
		}
	}

	void processar(Estat& estat, acci�_usuari::Tirar const& tirar)
	{
		IdEntitat jugador = estat.obtenir_id_jugador();
		Localitzaci� localitzaci�_jugador = estat.obtenir_component<Localitzaci�>(jugador);
		Inventari& inventari = estat.obtenir_component<Inventari>(jugador);
		int posici�_inventari = estat.obtenir_posici�_inventari();
		if (posici�_inventari < inventari.objectes.size())
		{
			IdEntitat objecte = inventari.objectes[posici�_inventari];
			inventari.objectes.erase(inventari.objectes.begin() + posici�_inventari);
			estat.afegir_component(objecte, localitzaci�_jugador);
			estat.treure_component<EnInventari>(objecte);

			if (posici�_inventari > 0)
				estat.despla�ar_inventari(-1);

			Nom const& nom_objecte = estat.obtenir_component<Nom>(objecte);
			char buffer[2048];
			sprintf_s(buffer, 2048, "Tirem un objecte %s", nom_objecte.nom.c_str());
			estat.afegir_missatge(
				buffer,
				iu::Paleta::TextManipularObjecte,
				true);
		}
	}

	void processar(Estat& estat, acci�_usuari::Consumir const& consumir)
	{
		IdEntitat jugador = estat.obtenir_id_jugador();
		Inventari& inventari = estat.obtenir_component<Inventari>(jugador);
		int posici�_inventari = estat.obtenir_posici�_inventari();
		if (posici�_inventari < inventari.objectes.size())
		{
			IdEntitat objecte = inventari.objectes[posici�_inventari];
			if (estat.t�_etiqueta<Consumible>(objecte))
			{
				if (auto curador = estat.potser_obtenir_component<Curador>(objecte))
				{
					processar(estat, acci�_entitat::RecuperarVida
						{
							jugador,
							curador->get().vida
						});
				}
				// PERFER M�s efectes?

				// eliminar objecte
				assert(!estat.t�_component<IAHostil>(objecte));
				inventari.objectes.erase(inventari.objectes.begin() + posici�_inventari);
				estat.modificar_o_treure([](auto c) { return true; }, objecte);

				if (posici�_inventari > 0)
					estat.despla�ar_inventari(-1);
			}

		}
	}

	void processar(Estat& estat, acci�_usuari::MoureRatol� const& ratol�)
	{
		estat.moure_ratol�(ratol�.p);
	}

	void processar(Estat& estat, acci�_usuari::AlternarRegistre const&)
	{
		estat.alterna_registre();
	}

	void processar(Estat& estat, acci�_usuari::Despla�arRegistre const& despla�ament)
	{
		estat.despla�ar_registre(despla�ament.l�nies);
	}

	void processar(Estat& estat, acci�_usuari::AlternarInventari const&)
	{
		estat.alterna_inventari();
	}

	void processar(Estat& estat, acci�_usuari::Despla�arInventari const& despla�ament)
	{
		estat.despla�ar_inventari(despla�ament.l�nies);
	}
}
