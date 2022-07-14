module;

#include <cinttypes>

#include <optional>
#include <vector>

module EstatJoc;


bretolesc::Estat::Estat(int _amplada, int _al�ada, GeneradorDeMapa const& generador)
	: m_mapa(_amplada, _al�ada, generador)
	, tancar(false)
{
	bretolesc::Entitat jugador = obtenir_motlle(TipusEntitat::Jugador);
	jugador.posici� = m_mapa.obtenir_origen_jugador();

	id_jugador = m_mapa.afegir_entitat(jugador);
}

void bretolesc::Estat::actualitzar_visi�()
{
	m_mapa.actualitzar_camp_de_visi�(m_mapa[id_jugador].posici�, profunditat_de_visi�);
}

