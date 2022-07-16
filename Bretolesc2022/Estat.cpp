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

Estat::Estat(int _amplada, int _al�ada, Generador const& generador)
	: m_mapa(_amplada, _al�ada)
	, tancar(false)
{
	generador.generar(*this);

	auto [jugador_loc, jugador_pin] = obtenir_motlle(TipusEntitat::Jugador);
	jugador_loc.posici� = m_mapa.obtenir_origen_jugador();

	id_jugador = afegir_entitat(jugador_loc, jugador_pin);
}

void Estat::actualitzar_visi�()
{
	m_mapa.actualitzar_camp_de_visi�(obtenir_component<Localitzaci�>(obtenir_id_jugador()).posici�, profunditat_de_visi�);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

std::optional<IdEntitat> Estat::buscar_entitat(Punt2D coordenades) const
{
	std::optional<IdEntitat> resultat;

	localitzacions.per_cada([&resultat, coordenades](IdEntitat id, Localitzaci� const& loc)
		{
			if (loc.posici� == coordenades)
			{
				resultat = id;
			}
		});


	return resultat;
}

std::optional<IdEntitat> Estat::buscar_entitat_bloquejant(Punt2D coordenades) const
{
	std::optional<IdEntitat> resultat;

	localitzacions.per_cada([&resultat, coordenades](IdEntitat id, Localitzaci� const& loc)
		{
			if (loc.posici� == coordenades && loc.bloqueja_el_pas)
			{
				resultat = id;
			}
		});


	return resultat;
}

IdEntitat Estat::afegir_entitat(Localitzaci� const& loc, Pintat const& pintat)
{
	localitzacions.afegir(id_seg�ent, loc);
	pintats.afegir(id_seg�ent, pintat);
	return id_seg�ent++;
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
	//		if (m_mapa.�s_a_la_vista(entitat.posici�))
	//		{
	//			char const txt[2] = { entitat.caracter , '\0' };

	//			tcod::print(
	//				console,
	//				{ entitat.posici�.x, entitat.posici�.y },
	//				txt,
	//				TCOD_ColorRGB{ entitat.color.r, entitat.color.g, entitat.color.b },
	//				std::nullopt);
	//		}
	//	});
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

