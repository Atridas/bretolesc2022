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
		bool caminable; // s'hi pot caminar
		bool transparent; // no bloqueja la vista
		Grafic grafic_fosc; // no �s a la vista
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

		bool in_bounds(int x, int y) const
		{
			return x >= 0 && x < amplada&& y >= 0 && y < al�ada;
		}

		bool �s_caminable(int x, int y) const
		{
			return in_bounds(x, y) && get_rajola(x, y).caminable;
		}

		void set_rajola(int x, int y, TipusRajola tipus)
		{
			assert(in_bounds(x, y));

			rajoles[y * amplada + x] = tipus;
		}

		void set_spawn(int x, int y)
		{
			spawn_x = x;
			spawn_y = y;
		}

		int get_spawn_x() const
		{
			return spawn_x;
		}

		int get_spawn_y() const
		{
			return spawn_y;
		}

		Rajola get_rajola(int x, int y) const
		{
			assert(in_bounds(x, y));

			return info_rajoles[(int)rajoles[y * amplada + x]];
		}

		int get_amplada() const
		{
			return amplada;
		}

		int get_al�ada() const
		{
			return al�ada;
		}

		void pintar(tcod::Console& console) const
		{
			for(int y = 0; y < al�ada; ++y)
				for (int x = 0; x < amplada; ++x)
				{
					console[{x, y}] = get_rajola(x, y).grafic_fosc;
				}
		}

	private:
		int const amplada, al�ada;
		std::vector<TipusRajola> rajoles;

		std::array<Rajola, 2> info_rajoles;

		int spawn_x, spawn_y;
	};



	class GeneradorDeMapaDExemple : public GeneradorDeMapa
	{
	public:
		void generar(Mapa& mapa) const override
		{
			for (int y = 0; y < mapa.get_al�ada(); ++y)
				for (int x = 0; x < mapa.get_amplada(); ++x)
				{
					mapa.set_rajola(x, y, TipusRajola::Terra );
				}



			mapa.set_rajola(30, 22, TipusRajola::Paret);
			mapa.set_rajola(31, 22, TipusRajola::Paret);
			mapa.set_rajola(32, 22, TipusRajola::Paret);
			mapa.set_rajola(33, 22, TipusRajola::Paret);
		}
	};

}
