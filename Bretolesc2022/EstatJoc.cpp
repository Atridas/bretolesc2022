module;

#include <cinttypes>

#include <optional>
#include <vector>

module EstatJoc;


void bretolesc::Estat::actualitzar_visió()
{
	Entitat const& entitat_jugador = entitats[jugador];
	mapa.actualitzar_camp_de_visió(entitat_jugador.posició, profunditat_de_visió);
}

