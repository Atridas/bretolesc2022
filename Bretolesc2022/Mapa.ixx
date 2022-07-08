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
		Grafic gràfic_fosc; // no és a la vista
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
		Mapa(int _amplada, int _alçada, GeneradorDeMapa const& generador)
			: amplada(_amplada)
			, alçada(_alçada)
		{
			assert(amplada > 0);
			assert(alçada > 0);

			rajoles.resize(amplada * alçada);

			info_rajoles[(int)TipusRajola::Terra] = { true, true, {' ', Color::Blanc, {50, 50, 150}}};
			info_rajoles[(int)TipusRajola::Paret] = { false, false, { ' ', Color::Blanc, {0, 0, 150} } };

			generador.generar(*this);
		}

		bool és_dins_del_límit(int x, int y) const
		{
			return x >= 0 && x < amplada&& y >= 0 && y < alçada;
		}

		bool és_transitable(int x, int y) const
		{
			return és_dins_del_límit(x, y) && obtenir_rajola(x, y).transitable;
		}

		void establir_rajola(int x, int y, TipusRajola tipus)
		{
			assert(és_dins_del_límit(x, y));

			rajoles[y * amplada + x] = tipus;
		}

		void establir_orígen_jugador(int x, int y)
		{
			orígen_jugador_x = x;
			orígen_jugador_y = y;
		}

		int obtenir_orígen_jugador_x() const
		{
			return orígen_jugador_x;
		}

		int obtenir_orígen_jugador_y() const
		{
			return orígen_jugador_y;
		}

		Rajola obtenir_rajola(int x, int y) const
		{
			assert(és_dins_del_límit(x, y));

			return info_rajoles[(int)rajoles[y * amplada + x]];
		}

		int obtenir_amplada() const
		{
			return amplada;
		}

		int obtenir_alçada() const
		{
			return alçada;
		}

		void pintar(tcod::Console& console) const
		{
			for(int y = 0; y < alçada; ++y)
				for (int x = 0; x < amplada; ++x)
				{
					console[{x, y}] = obtenir_rajola(x, y).gràfic_fosc;
				}
		}

	private:
		int const amplada, alçada;
		std::vector<TipusRajola> rajoles;

		std::array<Rajola, 2> info_rajoles;

		int orígen_jugador_x, orígen_jugador_y;
	};



	class GeneradorDeMapaDExemple : public GeneradorDeMapa
	{
	public:
		void generar(Mapa& mapa) const override
		{
			for (int y = 0; y < mapa.obtenir_alçada(); ++y)
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
