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

import Com�;
import Entitats;

export namespace bretolesc
{
	using namespace component;

	class Estat
	{
	public:
		static const int profunditat_de_visi� = 8;

		Estat(int _amplada, int _al�ada, Generador const& generador);
		~Estat();

		Estat(Estat const&) = delete;
		Estat(Estat &&) = delete;
		Estat& operator=(Estat const&) = delete;
		Estat& operator=(Estat &&) = delete;


		void tanca() { tancar = true; }
		void reinicia() { reiniciar = true; }
		void reinicia(Generador const& generador);

		bool vol_ser_tancat() const { return tancar; }
		bool vol_ser_reiniciat() const { return reiniciar; }
		bool jugador_�s_viu() const;

		Mapa& mapa() { return m_mapa; }
		Mapa const& mapa() const { return m_mapa; }


		IdEntitat crear_entitat() { return id_seg�ent++; }

		IdEntitat const& obtenir_id_jugador() const { return id_jugador; }

		template<typename Component>
		Col�lecci�<Component>& obtenir_col�lecci�() { return {}; }
		template<typename Component>
		Col�lecci�<Component> const& obtenir_col�lecci�() const { return {}; }

		template<>
		Col�lecci�<Localitzaci�>& obtenir_col�lecci�<Localitzaci�>() { return localitzacions; }
		template<>
		Col�lecci�<Localitzaci�> const& obtenir_col�lecci�<Localitzaci�>() const { return localitzacions; }

		template<>
		Col�lecci�<Pintat>& obtenir_col�lecci�<Pintat>() { return pintats; }
		template<>
		Col�lecci�<Pintat> const& obtenir_col�lecci�<Pintat>() const { return pintats; }

		template<>
		Col�lecci�<Lluitador>& obtenir_col�lecci�<Lluitador>() { return lluitadors; }
		template<>
		Col�lecci�<Lluitador> const& obtenir_col�lecci�<Lluitador>() const { return lluitadors; }

		template<>
		Col�lecci�<IAHostil>& obtenir_col�lecci�<IAHostil>() { return ias_hostils; }
		template<>
		Col�lecci�<IAHostil> const& obtenir_col�lecci�<IAHostil>() const { return ias_hostils; }

		template<typename Component>
		Component& obtenir_component(IdEntitat id) { return obtenir_col�lecci�<Component>().obtenir(id); }
		template<typename Component>
		Component const& obtenir_component(IdEntitat id) const { return obtenir_col�lecci�<Component>().obtenir(id); }
		template<typename Component>
		std::optional<Component> potser_obtenir_component(IdEntitat id) const { return obtenir_col�lecci�<Component>().potser_obtenir(id); }

		template<typename Component>
		void afegir_component(IdEntitat id, Component const& component) { return obtenir_col�lecci�<Component>().afegir(id, component); }


		std::optional<IdEntitat> buscar_entitat(Punt2D coordenades) const;
		std::optional<IdEntitat> buscar_entitat_bloquejant(Punt2D coordenades) const;


		// sistemes
		void actualitzar_ias_hostils();
		void buscar_morts();
		// -----------

		void actualitzar_visi�();

		void pintar(tcod::Console& console) const;

	private:
		Mapa m_mapa;
		bool tancar = false, reiniciar = false;
		IdEntitat id_jugador;

		IdEntitat id_seg�ent = 0;

		// Col�leccions de components
		Col�lecci�<Localitzaci�> localitzacions;
		Col�lecci�<Pintat> pintats;
		Col�lecci�<Lluitador> lluitadors;
		Col�lecci�<IAHostil> ias_hostils; // VIGILA!!! cal un destructor per eliminar-ho b�
		// VIGILA si afegeixes m�s components, qu� cal fer-ne al morir? + reiniciar
	};
}

