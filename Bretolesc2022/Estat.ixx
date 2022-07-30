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
		~Estat();

		Estat(Estat const&) = delete;
		Estat(Estat &&) = delete;
		Estat& operator=(Estat const&) = delete;
		Estat& operator=(Estat &&) = delete;


		void tanca() { tancar = true; }
		bool vol_ser_tancat() const { return tancar; }

		Mapa& mapa() { return m_mapa; }
		Mapa const& mapa() const { return m_mapa; }


		IdEntitat crear_entitat() { return id_següent++; }

		IdEntitat const& obtenir_id_jugador() const { return id_jugador; }

		template<typename Component>
		Col·lecció<Component>& obtenir_col·lecció() { return {}; }
		template<typename Component>
		Col·lecció<Component> const& obtenir_col·lecció() const { return {}; }

		template<>
		Col·lecció<Localització>& obtenir_col·lecció<Localització>() { return localitzacions; }
		template<>
		Col·lecció<Localització> const& obtenir_col·lecció<Localització>() const { return localitzacions; }

		template<>
		Col·lecció<Pintat>& obtenir_col·lecció<Pintat>() { return pintats; }
		template<>
		Col·lecció<Pintat> const& obtenir_col·lecció<Pintat>() const { return pintats; }

		template<>
		Col·lecció<Lluitador>& obtenir_col·lecció<Lluitador>() { return lluitadors; }
		template<>
		Col·lecció<Lluitador> const& obtenir_col·lecció<Lluitador>() const { return lluitadors; }

		template<>
		Col·lecció<IAHostil>& obtenir_col·lecció<IAHostil>() { return ias_hostils; }
		template<>
		Col·lecció<IAHostil> const& obtenir_col·lecció<IAHostil>() const { return ias_hostils; }


		template<typename Component>
		Component& obtenir_component(IdEntitat id) { return obtenir_col·lecció<Component>().obtenir(id); }
		template<typename Component>
		Component const& obtenir_component(IdEntitat id) const { return obtenir_col·lecció<Component>().obtenir(id); }

		template<typename Component>
		void afegir_component(IdEntitat id, Component const& component) { return obtenir_col·lecció<Component>().afegir(id, component); }


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
		Col·lecció<Lluitador> lluitadors;
		Col·lecció<IAHostil> ias_hostils; // VIGILA!!! cal un destructor per eliminar-ho bé
	};
}

