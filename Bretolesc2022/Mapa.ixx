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
		bool caminable; // s'hi pot caminar
		bool transparent; // no bloqueja la vista
		Grafic grafic_fosc; // no és a la vista
	};

	enum class TipusRajola
	{
		Terra, Paret
	};

	struct Mapa
	{
		int const amplada, alçada;
		std::vector<TipusRajola> rajoles;

		std::array<Rajola, 2> info_rajoles;

		Mapa(int _amplada, int _alçada)
			: amplada(_amplada)
			, alçada(_alçada)
		{
			assert(amplada > 0);
			assert(alçada > 0);

			rajoles.resize(amplada * alçada);
			for (int i = 0; i < amplada * alçada; ++i)
				rajoles[i] = TipusRajola::Terra;


			set_rajola(30, 22, TipusRajola::Paret);
			set_rajola(31, 22, TipusRajola::Paret);
			set_rajola(32, 22, TipusRajola::Paret);
			set_rajola(33, 22, TipusRajola::Paret);


			info_rajoles[(int)TipusRajola::Terra] = { true, true, {' ', Color::Blanc, {50, 50, 150}}};
			info_rajoles[(int)TipusRajola::Paret] = { false, false, { ' ', Color::Blanc, {0, 0, 150} } };
		}

		bool in_bounds(int x, int y) const
		{
			return x >= 0 && x < amplada&& y >= 0 && y < alçada;
		}

		void set_rajola(int x, int y, TipusRajola tipus)
		{
			assert(in_bounds(x, y));

			rajoles[y * amplada + x] = tipus;
		}

		TipusRajola get_rajola(int x, int y) const
		{
			assert(in_bounds(x, y));

			return rajoles[y * amplada + x];
		}

		void pintar(tcod::Console& console) const
		{
			for(int y = 0; y < alçada; ++y)
				for (int x = 0; x < amplada; ++x)
				{
					TipusRajola rajola = get_rajola(x, y);
					console[{x, y}] = info_rajoles[(int)rajola].grafic_fosc;
				}
		}
	};

}
