module;

// std
#include <cassert>
#include <array>
#include <vector>

// 3rd party
#include <libtcod.hpp>

export module Mapa;

import Comú;

export namespace bretolesc
{
	struct Gràfic
	{
		int caracter;
		Color color_principal, color_de_fons;


		operator TCOD_ConsoleTile() const
		{
			return { caracter,
				TCOD_ColorRGBA{color_principal.r, color_principal.g, color_principal.b},
				TCOD_ColorRGBA{color_de_fons.r, color_de_fons.g, color_de_fons.b}
			};
		}
	};

	struct Rajola
	{
		bool transitable; // s'hi pot caminar
		bool bloqueja_la_vista; // bloqueja la vista
		Gràfic gràfic_a_la_vista; // no és a la vista
		Gràfic gràfic_fora_de_vista; // no és a la vista
	};

	enum class TipusRajola
	{
		Terra, Paret
	};

	class GeneradorDeMapa
	{
	public:
		virtual void generar(class Mapa& mapa) const = 0;
	};

	class Mapa
	{
	public:
		Mapa(int _amplada, int _alçada, GeneradorDeMapa const& generador);

		bool és_dins_del_límit(int x, int y) const
		{
			return x >= 0 && x < amplada&& y >= 0 && y < alçada;
		}

		bool és_transitable(int x, int y) const
		{
			return és_dins_del_límit(x, y) && obtenir_rajola(x, y).transitable;
		}

		bool és_transitable(Punt2D posició) const
		{
			return és_dins_del_límit(posició.x, posició.y) && obtenir_rajola(posició.x, posició.y).transitable;
		}

		void establir_rajola(int x, int y, TipusRajola tipus)
		{
			assert(és_dins_del_límit(x, y));

			rajoles[y * amplada + x] = tipus;
		}

		void establir_orígen_jugador(Punt2D orígen)
		{
			orígen_jugador = orígen;
		}

		Punt2D obtenir_orígen_jugador() const
		{
			return orígen_jugador;
		}

		Rajola const& obtenir_rajola(int x, int y) const
		{
			assert(és_dins_del_límit(x, y));

			return info_rajoles[(int)rajoles[y * amplada + x]];
		}

		bool és_a_la_vista(Punt2D r) const
		{
			return rajoles_a_la_vista[r.y * amplada + r.x];
		}

		bool està_explorat(Punt2D r) const
		{
			return rajoles_explorades[r.y * amplada + r.x];
		}

		int obtenir_amplada() const
		{
			return amplada;
		}

		int obtenir_alçada() const
		{
			return alçada;
		}

		void actualitzar_camp_de_visió(Punt2D orígen, int profunditat_màxima);

		void pintar(tcod::Console& console) const;

	private:
		int const amplada, alçada;
		std::vector<TipusRajola> rajoles;

		std::array<Rajola, 2> info_rajoles;
		Gràfic mortalla; // rajola mai vista

		Punt2D orígen_jugador;

		std::vector<bool> rajoles_a_la_vista; // llista de rajoles dins del camp de visió del jugador
		std::vector<bool> rajoles_explorades; // llista de rajoles que el jugador ha vist
	};



	class GeneradorDeMapaDExemple : public GeneradorDeMapa
	{
	public:
		void generar(Mapa& mapa) const override;
	};

}
