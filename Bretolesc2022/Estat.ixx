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
	class Estat
	{
	public:
		static const int profunditat_de_visió = 8;


		Estat(int _amplada, int _alçada, Generador const& generador);

		void tanca() { tancar = true; }
		bool vol_ser_tancat() const { return tancar; }

		Mapa& mapa() { return m_mapa; }
		Mapa const& mapa() const { return m_mapa; }


		IdEntitat afegir_entitat(Entitat entitat);

		IdEntitat const& obtenir_id_jugador() const { return id_jugador; }

		Entitat& obtenir_jugador() { return entitats[id_jugador]; }
		Entitat const& obtenir_jugador() const { return entitats[id_jugador]; }

		Entitat& obtenir_entitat(IdEntitat id) { return entitats[id]; }
		Entitat const& obtenir_entitat(IdEntitat id) const { return entitats[id]; }

		std::optional<IdEntitat> buscar_entitat(Punt2D coordenades) const;
		std::optional<IdEntitat> buscar_entitat_bloquejant(Punt2D coordenades) const;

		Entitat& operator[](IdEntitat entitat) { return obtenir_entitat(entitat); }
		Entitat const& operator[](IdEntitat entitat) const { return obtenir_entitat(entitat); }

		void actualitzar_visió();

		void pintar(tcod::Console& console) const;

	private:
		Mapa m_mapa;
		bool tancar;
		IdEntitat id_jugador;

		std::vector<Entitat> entitats;
	};
}

