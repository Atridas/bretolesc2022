module;

// std
#include <cinttypes>

#include <optional>
#include <vector>

// 3rd party
#include <libtcod.hpp>

export module Motor:Estat;

import :Mapa;
import :Generador;

import Comú;
import Entitats;

export namespace bretolesc
{
	using namespace component;

	class Estat
	{
	public:
		static const int profunditat_de_visió = 8;


		Estat(int _amplada, int _alçada, Generador const& generador);

		void tanca() { tancar = true; }
		bool vol_ser_tancat() const { return tancar; }

		Mapa& mapa() { return m_mapa; }
		Mapa const& mapa() const { return m_mapa; }


		IdEntitat afegir_entitat(Localització const &loc, Pintat const &pintat);

		IdEntitat const& obtenir_id_jugador() const { return id_jugador; }

		template<typename Component>
		Component& obtenir_component(IdEntitat id) { return {}; }
		template<typename Component>
		Component const& obtenir_component(IdEntitat id) const { return {}; }

		template<>
		Localització& obtenir_component<Localització>(IdEntitat id) { return localitzacions.obtenir(id); }
		template<>
		Localització const& obtenir_component<Localització>(IdEntitat id) const { return localitzacions.obtenir(id); }
		
		template<>
		Pintat& obtenir_component<Pintat>(IdEntitat id) { return pintats.obtenir(id); }
		template<>
		Pintat const& obtenir_component<Pintat>(IdEntitat id) const { return pintats.obtenir(id); }

		std::optional<IdEntitat> buscar_entitat(Punt2D coordenades) const;
		std::optional<IdEntitat> buscar_entitat_bloquejant(Punt2D coordenades) const;

		void actualitzar_visió();

		void pintar(tcod::Console& console) const;

	private:
		Mapa m_mapa;
		bool tancar;
		IdEntitat id_jugador;

		IdEntitat id_següent = 0;

		// Col·leccions de components
		Col·lecció<Localització> localitzacions;
		Col·lecció<Pintat> pintats;
	};
}

