﻿module;

// std
#include <cassert>

// 3rd party
#include <libtcod.hpp>

export module Motor:InterfícieUsuari;

import :Estat;
import :PaletaDeColors;

import Entitats;

// -------


using namespace bretolesc::component;

export namespace bretolesc::iu
{
	void pintar_barra_de_vida(Estat const& estat, tcod::Console& console)
	{
		auto id_jugador = estat.obtenir_id_jugador();


		// - Pintar barra de vida sota el text de vida
		//   - console[std::array<int, 2>].bg = color
		//   - [0,45] -> [20,45]

		char buffer[2048];

		float vida = 0;

		if (auto jugador_opt = estat.potser_obtenir_component<Lluitador>(id_jugador))
		{
			Lluitador const& jugador = *jugador_opt;
			sprintf_s(buffer, 2048, "Salut:%d/%d", jugador.salut, jugador.salut_màxima);

			vida = (float)jugador.salut / (float)jugador.salut_màxima;
		}
		else
		{
			sprintf_s(buffer, 2048, "Salut:0");
		}

		for (int i = 0; i < 20; ++i)
		{
			float marge_inferior = i / 20.0f;
			float marge_superior = (i+1) / 20.0f;

			Color color = {0,0,0};
			if (vida >= marge_superior)
			{
				color = Paleta::BarraVidaSencera;
			}
			else if (vida <= marge_inferior)
			{
				color = Paleta::BarraVidaBuida;
			}
			else
			{
				float alpha = (vida - marge_inferior) / (marge_superior - marge_inferior);
				color = mescla(Paleta::BarraVidaBuida, Paleta::BarraVidaSencera, alpha);
			}

			console[std::array<int, 2>{i, 45}].bg = color;
		}

		tcod::print(
			console,
			{ 1, 45 },
			buffer,
			Paleta::TextBarraVida,
			std::nullopt);
	}

	void pintar_info_ratolí(Estat const& estat, tcod::Console& console)
	{
		Punt2D r = estat.obtenir_ratolí();

		if (!estat.mapa().és_a_la_vista(r))
			return;

		std::vector<IdEntitat> entitats = estat.buscar_entitats(r);

		//char buffer[2048];
		//sprintf_s(buffer, 2048, "");

		int línia = 45;
		
		for (IdEntitat id : entitats)
		{
			// no petar el joc
			if (línia >= 50)
				break;

			if (auto nom_opt = estat.potser_obtenir_component<Nom>(id))
			{
				Nom const& nom = *nom_opt;
				Color color = Paleta::TextInfoRatolíActor;

				if (auto pintat_opt = estat.potser_obtenir_component<Pintat>(id))
				{
					Pintat const& pintat = *pintat_opt;
					switch (pintat.prioritat)
					{
					case PrioritatPintar::Cadàver:
						color = Paleta::TextInfoRatolíCadàver;
						break;
					case PrioritatPintar::Objecte:
						color = Paleta::TextInfoRatolíObjecte;
						break;
					case PrioritatPintar::Actor:
						color = Paleta::TextInfoRatolíActor;
						break;
					default:
						assert(false);
						break;
					}
				}

				tcod::print(
					console,
					{ 62, línia },
					nom.nom,
					color,
					std::nullopt);

				++línia;
			}
		}
	}

}
