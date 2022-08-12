module;

// std
#include <cassert>
#include <array>
#include <optional>
#include <vector>

// 3rd party
#include <libtcod.hpp>

export module Motor:Mapa;

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

	class Mapa
	{
	public:
		Mapa(int _amplada, int _al�ada);
		~Mapa();

		Mapa(Mapa const&) = delete;
		Mapa(Mapa&&) = delete;
		Mapa& operator=(Mapa const&) = delete;
		Mapa& operator=(Mapa&&) = delete;

		void reinicia();

		int a_�ndex(Punt2D r) const
		{
			return r.y * amplada + r.x;
		}

		bool �s_dins_del_l�mit(Punt2D r) const
		{
			return r.x >= 0 && r.x < amplada && r.y >= 0 && r.y < al�ada;
		}

		bool �s_transitable(Punt2D r) const
		{
			return �s_dins_del_l�mit(r) && obtenir_rajola(r).transitable;
		}

		void establir_rajola(Punt2D r, TipusRajola tipus);

		Rajola const& obtenir_rajola(Punt2D r) const
		{
			assert(�s_dins_del_l�mit(r));

			return info_rajoles[(int)rajoles[a_�ndex(r)]];
		}

		bool �s_a_la_vista(Punt2D r) const
		{
			return rajoles_a_la_vista[a_�ndex(r)];
		}

		bool est�_explorat(Punt2D r) const
		{
			return rajoles_explorades[a_�ndex(r)];
		}

		int obtenir_amplada() const
		{
			return amplada;
		}

		int obtenir_al�ada() const
		{
			return al�ada;
		}

		TCOD_map_t obtenir_mapa_tcod() const
		{
			return mapa_TCOD;
		}

		void actualitzar_camp_de_visi�(Punt2D origen, int profunditat_m�xima);

		void pintar(tcod::Console& console) const;

	private:
		int const amplada, al�ada;
		std::vector<TipusRajola> rajoles;

		std::array<Rajola, 2> info_rajoles;
		Gr�fic mortalla; // rajola mai vista

		std::vector<bool> rajoles_a_la_vista; // llista de rajoles dins del camp de visi� del jugador
		std::vector<bool> rajoles_explorades; // llista de rajoles que el jugador ha vist

		TCOD_map_t mapa_TCOD;
	};

}
