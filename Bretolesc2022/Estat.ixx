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

import Comú;
import Col·leccióComponents;
import Entitats;
import EstructuresAccions;
import ModeEntrada;

export namespace bretolesc
{
	using namespace component;
	using namespace etiqueta;

	class Estat
	{
	public:
		static const int profunditat_de_visió = 8;

		Estat(int _amplada, int _alçada, Generador const& generador, int llavor);
		~Estat();

		Estat(Estat const&) = delete;
		Estat(Estat &&) = delete;
		Estat& operator=(Estat const&) = delete;
		Estat& operator=(Estat &&) = delete;


		void tanca() { tancar = true; }
		void reinicia() { reiniciar = true; }
		void reinicia(Generador const& generador);
		void alterna_registre();
		void desplaçar_registre(int quantitat) { registre.desplaçar_registre(quantitat); }
		void alterna_inventari();
		void desplaçar_inventari(int quantitat);
		int obtenir_posició_inventari() const { return posició_inventari; }
		void activa_cursor(acció::ActivarObjecte _objecte_a_activar, int rang_cursor, int radi_cursor);
		void moure_cursor(Vector2D v);
		void establir_cursor(Punt2D p);
		void accepta_cursor();
		void cancela_cursor();

		bool vol_ser_tancat() const { return tancar; }
		bool vol_ser_reiniciat() const { return reiniciar; }
		void moure_ratolí(Punt2D p) { ratolí = p; }
		Punt2D obtenir_ratolí() const { return ratolí; }

		bool jugador_és_viu() const;
		ModeEntrada obtenir_mode_entrada() const;

		Mapa& mapa() { return m_mapa; }
		Mapa const& mapa() const { return m_mapa; }

		void afegir_missatge(std::string_view missatge, Color color, bool acumular = false)
		{
			registre.afegir_missatge(missatge, color, acumular);
		}

		IdEntitat crear_entitat() { return id_següent++; }

		IdEntitat const& obtenir_id_jugador() const { return id_jugador; }

		template<typename F>
		void modificar_o_treure(F f, IdEntitat id)
		{
			col·leccions.modificar_o_treure(f, id);
			etiquetes.treure(f, id);
		}

		template<typename Component>
		Col·lecció<Component>& obtenir_col·lecció() { return col·leccions.obtenir_col·lecció(); }
		template<typename Component>
		Col·lecció<Component> const& obtenir_col·lecció() const { return col·leccions.obtenir_col·lecció(); }

		template<typename Component>
		void afegir_component(IdEntitat id, Component&& component) { col·leccions.afegir_component(id, std::forward<Component>(component)); }
		template<typename Etiqueta>
		bool té_etiqueta(IdEntitat id) const { return etiquetes.té<Etiqueta>(id); }
		
		template<typename Component>
		bool té_component(IdEntitat id) { return col·leccions.potser_obtenir_component<Component>(id).has_value(); }
		template<typename Component>
		Component& obtenir_component(IdEntitat id) { return col·leccions.obtenir_component<Component>(id); }
		template<typename Component>
		Component const& obtenir_component(IdEntitat id) const { return col·leccions.obtenir_component<Component>(id); }
		template<typename Component>
		std::optional<std::reference_wrapper<Component const>> potser_obtenir_component(IdEntitat id) const { return col·leccions.potser_obtenir_component<Component>(id); }
		
		template<typename Component>
		void treure_component(IdEntitat id) { col·leccions.treure<Component>(id); }
		template<typename Component>
		std::optional<Component> treure_component_si_hi_és(IdEntitat id) { return col·leccions.treure_si_hi_és<Component>(id); }
		
		template<typename Etiqueta>
		void afegir_etiqueta(IdEntitat id) { return etiquetes.afegir<Etiqueta>(id); }


		std::optional<IdEntitat> buscar_entitat(Punt2D coordenades) const;
		std::vector<IdEntitat> buscar_entitats(Punt2D coordenades) const;
		std::optional<IdEntitat> buscar_entitat_bloquejant(Punt2D coordenades) const;
		struct EntitatIDistància { IdEntitat entitat; float distància; };
		std::optional<EntitatIDistància> buscar_entitat_lluitadora_més_propera(IdEntitat entitat) const;

		void actualitzar_lógica();

		void actualitzar_visió();

		void pintar(tcod::Console& console) const;

	private:
		RNG rng;
		Mapa m_mapa;
		RegistreDeMissatges registre;
		bool tancar = false, reiniciar = false;// mostrar_registre = false, mostrar_inventari = false;
		enum class SubmodeJoc
		{
			Normal, Registre, Inventari, Cursor
		} submode_joc = SubmodeJoc::Normal;
		std::optional<acció::ActivarObjecte> objecte_a_activar;
		int rang_cursor = 0, radi_cursor = 0;
		int posició_inventari = 0;
		Punt2D cursor, ratolí;

		IdEntitat id_jugador;

		IdEntitat id_següent = 0;

		// Col·leccions de components
		Col·leccions<
			Nom, Localització, Pintat, 
			Lluitador, IAHostil, Confús,
			Curador, EncanteriDeDany, EncanteriDeConfusió,
			ObjectiuLluitadorProper, ObjectiuCursor,
			Inventari, EnInventari> col·leccions;
		Etiquetes<
			BloquejaElPas,
			Objecte, Consumible, AvançaTorn,
			ObjectiuJugador> etiquetes;
		// VIGILA!!! cal un destructor per eliminar-ho bé
		// VIGILA si afegeixes més components, què cal fer-ne al morir? + reiniciar


		// sistemes
		void actualitzar_ias_hostils();
		void buscar_morts();
		// -----------
	};
}

