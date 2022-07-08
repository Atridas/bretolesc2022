module;

// std
#include <cassert>
#include <array>
#include <vector>

// 3rd party
#include <libtcod.hpp>

export module Mapa;

import Com�;

export namespace bretolesc
{
	struct Gr�fic
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
		Gr�fic gr�fic_a_la_vista; // no �s a la vista
		Gr�fic gr�fic_fora_de_vista; // no �s a la vista
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
		Mapa(int _amplada, int _al�ada, GeneradorDeMapa const& generador);

		bool �s_dins_del_l�mit(int x, int y) const
		{
			return x >= 0 && x < amplada&& y >= 0 && y < al�ada;
		}

		bool �s_transitable(int x, int y) const
		{
			return �s_dins_del_l�mit(x, y) && obtenir_rajola(x, y).transitable;
		}

		bool �s_transitable(Punt2D posici�) const
		{
			return �s_dins_del_l�mit(posici�.x, posici�.y) && obtenir_rajola(posici�.x, posici�.y).transitable;
		}

		void establir_rajola(int x, int y, TipusRajola tipus)
		{
			assert(�s_dins_del_l�mit(x, y));

			rajoles[y * amplada + x] = tipus;
		}

		void establir_or�gen_jugador(Punt2D or�gen)
		{
			or�gen_jugador = or�gen;
		}

		Punt2D obtenir_or�gen_jugador() const
		{
			return or�gen_jugador;
		}

		Rajola const& obtenir_rajola(int x, int y) const
		{
			assert(�s_dins_del_l�mit(x, y));

			return info_rajoles[(int)rajoles[y * amplada + x]];
		}

		bool �s_a_la_vista(Punt2D r) const
		{
			return rajoles_a_la_vista[r.y * amplada + r.x];
		}

		bool est�_explorat(Punt2D r) const
		{
			return rajoles_explorades[r.y * amplada + r.x];
		}

		int obtenir_amplada() const
		{
			return amplada;
		}

		int obtenir_al�ada() const
		{
			return al�ada;
		}

		void actualitzar_camp_de_visi�(Punt2D or�gen, int profunditat_m�xima);

		void pintar(tcod::Console& console) const;

	private:
		int const amplada, al�ada;
		std::vector<TipusRajola> rajoles;

		std::array<Rajola, 2> info_rajoles;
		Gr�fic mortalla; // rajola mai vista

		Punt2D or�gen_jugador;

		std::vector<bool> rajoles_a_la_vista; // llista de rajoles dins del camp de visi� del jugador
		std::vector<bool> rajoles_explorades; // llista de rajoles que el jugador ha vist
	};



	class GeneradorDeMapaDExemple : public GeneradorDeMapa
	{
	public:
		void generar(Mapa& mapa) const override;
	};

}
