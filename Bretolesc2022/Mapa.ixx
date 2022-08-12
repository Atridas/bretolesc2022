module;

// std
#include <cassert>
#include <array>
#include <optional>
#include <vector>

// 3rd party
#include <libtcod.hpp>

export module Motor:Mapa;

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

	class Mapa
	{
	public:
		Mapa(int _amplada, int _alçada);
		~Mapa();

		Mapa(Mapa const&) = delete;
		Mapa(Mapa&&) = delete;
		Mapa& operator=(Mapa const&) = delete;
		Mapa& operator=(Mapa&&) = delete;

		void reinicia();

		int a_índex(Punt2D r) const
		{
			return r.y * amplada + r.x;
		}

		bool és_dins_del_límit(Punt2D r) const
		{
			return r.x >= 0 && r.x < amplada && r.y >= 0 && r.y < alçada;
		}

		bool és_transitable(Punt2D r) const
		{
			return és_dins_del_límit(r) && obtenir_rajola(r).transitable;
		}

		void establir_rajola(Punt2D r, TipusRajola tipus);

		Rajola const& obtenir_rajola(Punt2D r) const
		{
			assert(és_dins_del_límit(r));

			return info_rajoles[(int)rajoles[a_índex(r)]];
		}

		bool és_a_la_vista(Punt2D r) const
		{
			return rajoles_a_la_vista[a_índex(r)];
		}

		bool està_explorat(Punt2D r) const
		{
			return rajoles_explorades[a_índex(r)];
		}

		int obtenir_amplada() const
		{
			return amplada;
		}

		int obtenir_alçada() const
		{
			return alçada;
		}

		TCOD_map_t obtenir_mapa_tcod() const
		{
			return mapa_TCOD;
		}

		void actualitzar_camp_de_visió(Punt2D origen, int profunditat_màxima);

		void pintar(tcod::Console& console) const;

	private:
		int const amplada, alçada;
		std::vector<TipusRajola> rajoles;

		std::array<Rajola, 2> info_rajoles;
		Gràfic mortalla; // rajola mai vista

		std::vector<bool> rajoles_a_la_vista; // llista de rajoles dins del camp de visió del jugador
		std::vector<bool> rajoles_explorades; // llista de rajoles que el jugador ha vist

		TCOD_map_t mapa_TCOD;
	};

}
