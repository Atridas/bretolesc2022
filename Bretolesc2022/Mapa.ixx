module;

// std
#include <cassert>
#include <array>
#include <optional>
#include <vector>

// 3rd party
#include <libtcod.hpp>

export module Mapa;

import Com�;
import Entitats;

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

		int a_�ndex(Punt2D r) const
		{
			return r.y * amplada + r.x;
		}

		bool �s_dins_del_l�mit(Punt2D r) const
		{
			return r.x >= 0 && r.x < amplada&& r.y >= 0 && r.y < al�ada;
		}

		bool �s_transitable(Punt2D r) const
		{
			return �s_dins_del_l�mit(r) && obtenir_rajola(r).transitable;
		}

		void establir_rajola(Punt2D r, TipusRajola tipus)
		{
			assert(�s_dins_del_l�mit(r));

			rajoles[a_�ndex(r)] = tipus;
		}

		void establir_origen_jugador(Punt2D origen)
		{
			origen_jugador = origen;
		}

		Punt2D obtenir_origen_jugador() const
		{
			return origen_jugador;
		}

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

		void actualitzar_camp_de_visi�(Punt2D origen, int profunditat_m�xima);

		void pintar(tcod::Console& console) const;

		IdEntitat afegir_entitat(Entitat const& entitat);

		Entitat& obtenir_entitat(IdEntitat const& entitat);
		Entitat const& obtenir_entitat(IdEntitat const& entitat) const;

		Entitat& operator[](IdEntitat const& entitat) { return obtenir_entitat(entitat); }
		Entitat const& operator[](IdEntitat const& entitat) const { return obtenir_entitat(entitat); }

		std::optional<IdEntitat> buscar_entitat(Punt2D coordenades) const;
		std::optional<IdEntitat> buscar_entitat_bloquejant(Punt2D coordenades) const;

		void actualitzar_enemics();

	private:
		int const amplada, al�ada;
		std::vector<TipusRajola> rajoles;

		std::array<Rajola, 2> info_rajoles;
		Gr�fic mortalla; // rajola mai vista

		Punt2D origen_jugador;

		std::vector<bool> rajoles_a_la_vista; // llista de rajoles dins del camp de visi� del jugador
		std::vector<bool> rajoles_explorades; // llista de rajoles que el jugador ha vist

		std::vector<Entitat> entitats;
	};



	class GeneradorDeMapaDExemple : public GeneradorDeMapa
	{
	public:
		void generar(Mapa& mapa) const override;
	};

}
