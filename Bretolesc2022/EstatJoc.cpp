module;

#include <cinttypes>

#include <optional>
#include <vector>

module EstatJoc;


void bretolesc::Estat::actualitzar_visi�()
{
	Entitat const& entitat_jugador = entitats[jugador];
	mapa.actualitzar_camp_de_visi�(entitat_jugador.posici�, profunditat_de_visi�);
}

