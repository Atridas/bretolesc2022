module;

// std
#include <cassert>
#include <cinttypes>

#include <optional>
#include <vector>

// 3rd party
#include <libtcod.hpp>

module Motor:Estat;

using namespace bretolesc;
using namespace bretolesc::component;

Estat::Estat(int _amplada, int _alçada, Generador const& generador)
	: m_mapa(_amplada, _alçada)
	, tancar(false)
{
	generador.generar(*this);

	auto [jugador_loc, jugador_pin] = obtenir_motlle(TipusEntitat::Jugador);
	jugador_loc.posició = m_mapa.obtenir_origen_jugador();

	id_jugador = afegir_entitat(jugador_loc, jugador_pin);
}

void Estat::actualitzar_visió()
{
	m_mapa.actualitzar_camp_de_visió(obtenir_component<Localització>(obtenir_id_jugador()).posició, profunditat_de_visió);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

std::optional<IdEntitat> Estat::buscar_entitat(Punt2D coordenades) const
{
	std::optional<IdEntitat> resultat;

	localitzacions.per_cada([&resultat, coordenades](IdEntitat id, Localització const& loc)
		{
			if (loc.posició == coordenades)
			{
				resultat = id;
			}
		});


	return resultat;
}

std::optional<IdEntitat> Estat::buscar_entitat_bloquejant(Punt2D coordenades) const
{
	std::optional<IdEntitat> resultat;

	localitzacions.per_cada([&resultat, coordenades](IdEntitat id, Localització const& loc)
		{
			if (loc.posició == coordenades && loc.bloqueja_el_pas)
			{
				resultat = id;
			}
		});


	return resultat;
}

IdEntitat Estat::afegir_entitat(Localització const& loc, Pintat const& pintat)
{
	localitzacions.afegir(id_següent, loc);
	pintats.afegir(id_següent, pintat);
	return id_següent++;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void Estat::pintar(tcod::Console& console) const
{
	m_mapa.pintar(console);

	// PERFER
	//entitats.per_cada([this, &console](IdEntitat id, component::Entitat const& entitat)
	//	{
	//		if (m_mapa.és_a_la_vista(entitat.posició))
	//		{
	//			char const txt[2] = { entitat.caracter , '\0' };

	//			tcod::print(
	//				console,
	//				{ entitat.posició.x, entitat.posició.y },
	//				txt,
	//				TCOD_ColorRGB{ entitat.color.r, entitat.color.g, entitat.color.b },
	//				std::nullopt);
	//		}
	//	});
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

