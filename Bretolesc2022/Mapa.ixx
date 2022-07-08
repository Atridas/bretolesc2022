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
	struct Grafic
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
		bool transparent; // no bloqueja la vista
		Grafic gr�fic_fosc; // no �s a la vista
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
		Mapa(int _amplada, int _al�ada, GeneradorDeMapa const& generador)
			: amplada(_amplada)
			, al�ada(_al�ada)
		{
			assert(amplada > 0);
			assert(al�ada > 0);

			rajoles.resize(amplada * al�ada);

			info_rajoles[(int)TipusRajola::Terra] = { true, true, {' ', Color::Blanc, {50, 50, 150}}};
			info_rajoles[(int)TipusRajola::Paret] = { false, false, { ' ', Color::Blanc, {0, 0, 150} } };

			generador.generar(*this);
		}

		bool �s_dins_del_l�mit(int x, int y) const
		{
			return x >= 0 && x < amplada&& y >= 0 && y < al�ada;
		}

		bool �s_transitable(int x, int y) const
		{
			return �s_dins_del_l�mit(x, y) && obtenir_rajola(x, y).transitable;
		}

		void establir_rajola(int x, int y, TipusRajola tipus)
		{
			assert(�s_dins_del_l�mit(x, y));

			rajoles[y * amplada + x] = tipus;
		}

		void establir_or�gen_jugador(int x, int y)
		{
			or�gen_jugador_x = x;
			or�gen_jugador_y = y;
		}

		int obtenir_or�gen_jugador_x() const
		{
			return or�gen_jugador_x;
		}

		int obtenir_or�gen_jugador_y() const
		{
			return or�gen_jugador_y;
		}

		Rajola obtenir_rajola(int x, int y) const
		{
			assert(�s_dins_del_l�mit(x, y));

			return info_rajoles[(int)rajoles[y * amplada + x]];
		}

		int obtenir_amplada() const
		{
			return amplada;
		}

		int obtenir_al�ada() const
		{
			return al�ada;
		}

		void pintar(tcod::Console& console) const
		{
			for(int y = 0; y < al�ada; ++y)
				for (int x = 0; x < amplada; ++x)
				{
					console[{x, y}] = obtenir_rajola(x, y).gr�fic_fosc;
				}
		}

	private:
		int const amplada, al�ada;
		std::vector<TipusRajola> rajoles;

		std::array<Rajola, 2> info_rajoles;

		int or�gen_jugador_x, or�gen_jugador_y;
	};



	class GeneradorDeMapaDExemple : public GeneradorDeMapa
	{
	public:
		void generar(Mapa& mapa) const override
		{
			for (int y = 0; y < mapa.obtenir_al�ada(); ++y)
				for (int x = 0; x < mapa.obtenir_amplada(); ++x)
				{
					mapa.establir_rajola(x, y, TipusRajola::Terra );
				}



			mapa.establir_rajola(30, 22, TipusRajola::Paret);
			mapa.establir_rajola(31, 22, TipusRajola::Paret);
			mapa.establir_rajola(32, 22, TipusRajola::Paret);
			mapa.establir_rajola(33, 22, TipusRajola::Paret);
		}
	};

}
