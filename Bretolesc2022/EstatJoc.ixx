module;

#include <cinttypes>

#include <optional>
#include <vector>

export module EstatJoc;

import Comú;
import Entitats;
import Mapa;

export namespace bretolesc
{
	class Estat
	{
	public:
		static const int profunditat_de_visió = 8;


		Estat(int _amplada, int _alçada, GeneradorDeMapa const& generador);

		void tanca() { tancar = true; }
		bool vol_ser_tancat() const { return tancar; }

		Mapa& mapa() { return m_mapa; }
		Mapa const& mapa() const { return m_mapa; }

		IdEntitat const& obtenir_id_jugador() const { return id_jugador; }

		Entitat& obtenir_jugador() { return m_mapa[id_jugador]; }
		Entitat const& obtenir_jugador() const { return m_mapa[id_jugador]; }

		Entitat& obtenir_entitat(IdEntitat id) { return m_mapa[id]; }
		Entitat const& obtenir_entitat(IdEntitat id) const { return m_mapa[id]; }

		Entitat& operator[](IdEntitat const& entitat) { return obtenir_entitat(entitat); }
		Entitat const& operator[](IdEntitat const& entitat) const { return obtenir_entitat(entitat); }

		void actualitzar_visió();

	private:
		Mapa m_mapa;
		bool tancar;
		IdEntitat id_jugador;
	};
}

