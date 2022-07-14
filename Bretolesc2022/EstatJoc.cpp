module;

#include <cinttypes>

#include <optional>
#include <vector>

module EstatJoc;


bretolesc::Estat::Estat(int _amplada, int _alçada, GeneradorDeMapa const& generador)
	: m_mapa(_amplada, _alçada, generador)
	, tancar(false)
{
	bretolesc::Entitat jugador = obtenir_motlle(TipusEntitat::Jugador);
	jugador.posició = m_mapa.obtenir_origen_jugador();

	id_jugador = m_mapa.afegir_entitat(jugador);
}

void bretolesc::Estat::actualitzar_visió()
{
	m_mapa.actualitzar_camp_de_visió(m_mapa[id_jugador].posició, profunditat_de_visió);
}

