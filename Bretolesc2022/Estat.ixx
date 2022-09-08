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
import :RegistreDeMissatges;

import Com�;
import Col�lecci�Components;
import Entitats;
import ModeEntrada;

export namespace bretolesc
{
	using namespace component;
	using namespace etiqueta;

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
		void alterna_registre() { mostrar_registre = !mostrar_registre; registre.reiniciar_despla�ament(); }
		void despla�ar_registre(int quantitat) { registre.despla�ar_registre(quantitat); }
		void alterna_inventari() { mostrar_inventari = !mostrar_inventari; posici�_inventari = 0; }
		void despla�ar_inventari(int quantitat);
		int obtenir_posici�_inventari() const { return posici�_inventari; }

		bool vol_ser_tancat() const { return tancar; }
		bool vol_ser_reiniciat() const { return reiniciar; }
		void moure_ratol�(Punt2D p) { ratol� = p; }
		Punt2D obtenir_ratol�() const { return ratol�; }

		bool jugador_�s_viu() const;
		ModeEntrada obtenir_mode_entrada() const;

		Mapa& mapa() { return m_mapa; }
		Mapa const& mapa() const { return m_mapa; }

		void afegir_missatge(std::string_view missatge, Color color, bool acumular = false)
		{
			registre.afegir_missatge(missatge, color, acumular);
		}

		IdEntitat crear_entitat() { return id_seg�ent++; }

		IdEntitat const& obtenir_id_jugador() const { return id_jugador; }

		template<typename F>
		void modificar_o_treure(F f, IdEntitat id)
		{
			col�leccions.modificar_o_treure(f, id);
			etiquetes.treure(f, id);
		}

		template<typename Component>
		Col�lecci�<Component>& obtenir_col�lecci�() { return col�leccions.obtenir_col�lecci�(); }
		template<typename Component>
		Col�lecci�<Component> const& obtenir_col�lecci�() const { return col�leccions.obtenir_col�lecci�(); }

		template<typename Component>
		void afegir_component(IdEntitat id, Component&& component) { col�leccions.afegir_component(id, std::forward<Component>(component)); }
		template<typename Etiqueta>
		bool t�_etiqueta(IdEntitat id) const { return etiquetes.t�<Etiqueta>(id); }
		
		template<typename Component>
		bool t�_component(IdEntitat id) { return col�leccions.potser_obtenir_component<Component>(id).has_value(); }
		template<typename Component>
		Component& obtenir_component(IdEntitat id) { return col�leccions.obtenir_component<Component>(id); }
		template<typename Component>
		Component const& obtenir_component(IdEntitat id) const { return col�leccions.obtenir_component<Component>(id); }
		template<typename Component>
		std::optional<std::reference_wrapper<Component const>> potser_obtenir_component(IdEntitat id) const { return col�leccions.potser_obtenir_component<Component>(id); }
		
		template<typename Component>
		void treure_component(IdEntitat id) { col�leccions.treure<Component>(id); }
		template<typename Component>
		std::optional<Component> treure_component_si_hi_�s(IdEntitat id) { return col�leccions.treure_si_hi_�s<Component>(id); }
		
		template<typename Etiqueta>
		void afegir_etiqueta(IdEntitat id) { return etiquetes.afegir<Etiqueta>(id); }


		std::optional<IdEntitat> buscar_entitat(Punt2D coordenades) const;
		std::vector<IdEntitat> buscar_entitats(Punt2D coordenades) const;
		std::optional<IdEntitat> buscar_entitat_bloquejant(Punt2D coordenades) const;

		void actualitzar_l�gica();

		void actualitzar_visi�();

		void pintar(tcod::Console& console) const;

	private:
		Mapa m_mapa;
		RegistreDeMissatges registre;
		bool tancar = false, reiniciar = false, mostrar_registre = false, mostrar_inventari = false;
		int posici�_inventari = 0;
		Punt2D ratol�;

		IdEntitat id_jugador;

		IdEntitat id_seg�ent = 0;

		// Col�leccions de components
		Col�leccions<Nom, Localitzaci�, Pintat, Lluitador, IAHostil, Curador, Inventari, EnInventari> col�leccions;
		Etiquetes<BloquejaElPas, Objecte, Consumible> etiquetes;
		// VIGILA!!! cal un destructor per eliminar-ho b�
		// VIGILA si afegeixes m�s components, qu� cal fer-ne al morir? + reiniciar


		// sistemes
		void actualitzar_ias_hostils();
		void buscar_morts();
		// -----------
	};
}

