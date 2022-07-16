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

Estat::Estat(int _amplada, int _al�ada, Generador const& generador)
	: m_mapa(_amplada, _al�ada)
	, tancar(false)
{
	generador.generar(*this);

	bretolesc::Entitat jugador = obtenir_motlle(TipusEntitat::Jugador);
	jugador.posici� = m_mapa.obtenir_origen_jugador();

	id_jugador = afegir_entitat(jugador);
}

void Estat::actualitzar_visi�()
{
	m_mapa.actualitzar_camp_de_visi�(obtenir_jugador().posici�, profunditat_de_visi�);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

std::optional<IdEntitat> Estat::buscar_entitat(Punt2D coordenades) const
{
	for (int i = 0; i < (int)entitats.size(); ++i)
	{
		Entitat const& entitat = entitats[i];
		if (entitat.posici� == coordenades)
		{
			return (IdEntitat)i;
		}
	}
	return {};
}

std::optional<IdEntitat> Estat::buscar_entitat_bloquejant(Punt2D coordenades) const
{
	for (int i = 0; i < (int)entitats.size(); ++i)
	{
		Entitat const& entitat = entitats[i];
		if (entitat.posici� == coordenades && entitat.bloqueja_el_pas)
		{
			return (IdEntitat)i;
		}
	}
	return {};
}

IdEntitat Estat::afegir_entitat(Entitat entitat)
{
	IdEntitat id = (int)entitats.size();
	entitats.push_back(entitat);
	return id;
}

Entitat& Estat::obtenir_entitat(IdEntitat entitat)
{
	assert(entitat >= 0);
	assert(entitat < (int)entitats.size());
	return entitats[entitat];
}

Entitat const& Estat::obtenir_entitat(IdEntitat entitat) const
{
	assert(entitat >= 0);
	assert(entitat < (int)entitats.size());
	return entitats[entitat];
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void Estat::pintar(tcod::Console& console) const
{
	m_mapa.pintar(console);

	for (Entitat const& entitat : entitats)
	{
		if (m_mapa.�s_a_la_vista(entitat.posici�))
		{
			char const txt[2] = { entitat.caracter , '\0' };

			tcod::print(
				console,
				{ entitat.posici�.x, entitat.posici�.y },
				txt,
				TCOD_ColorRGB{ entitat.color.r, entitat.color.g, entitat.color.b },
				std::nullopt);
		}
	}
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

