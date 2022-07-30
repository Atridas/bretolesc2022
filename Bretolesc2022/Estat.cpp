module;

// std
#include <cassert>
#include <cinttypes>

#include <optional>
#include <variant>
#include <vector>

// 3rd party
#include <libtcod.hpp>

module Motor:Estat;

import :ProcessadorsAccions;

import EstructuresAccions;

using namespace bretolesc;
using namespace bretolesc::component;

Estat::Estat(int _amplada, int _alçada, Generador const& generador)
	: m_mapa(_amplada, _alçada)
	, tancar(false)
{
	id_jugador = generador.generar(*this);
}


Estat::~Estat()
{
	for (auto [id, ia_hostil] : ias_hostils)
	{
		TCOD_path_delete(ia_hostil.camí);
	}
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

	for (auto [id, loc] : localitzacions)
	{
		if (loc.posició == coordenades)
		{
			resultat = id;
		}
	}


	return resultat;
}

std::optional<IdEntitat> Estat::buscar_entitat_bloquejant(Punt2D coordenades) const
{
	std::optional<IdEntitat> resultat;

	for (auto [id, loc] : localitzacions)
	{
		if (loc.posició == coordenades && loc.bloqueja_el_pas)
		{
			resultat = id;
		}
	}


	return resultat;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void Estat::actualitzar_ias_hostils()
{
	// - l'objectiu és el jugador
	// - si el jugador és a distància (manhattan) 1, atacar-lo
	// - si tenim un camí definit, seguir-lo
	// - si el jugador és visible (si nosaltres som visibles des del jugador), crear un camí fins a ell
	// - en cas contrari no fer res

	Localització loc_jugador = obtenir_component<Localització>(id_jugador);

	std::vector<Acció> accions;

	for (auto [id_ia, loc_ia, ia_hostil] : per_cada(localitzacions, ias_hostils))
	{
		if (distància_manhattan(loc_jugador.posició, loc_ia.posició) == 1)
		{
			accions.push_back( acció::AtacCosACos{id_ia, id_jugador} );
		}
		else if (!TCOD_path_is_empty(ia_hostil.camí))
		{
			Punt2D destí;
			TCOD_path_get_destination(ia_hostil.camí, &destí.x, &destí.y);
			if (distància_manhattan(destí, loc_jugador.posició) > 2)
			{
				// recalcular
				TCOD_path_compute(ia_hostil.camí, loc_ia.posició.x, loc_ia.posició.y, loc_jugador.posició.x, loc_jugador.posició.y);
			}

			if (TCOD_path_walk(ia_hostil.camí, &destí.x, &destí.y, true))
			{
				Vector2D moviment = destí - loc_ia.posició;
				if (std::abs(moviment.x) > 2 || std::abs(moviment.y) > 2)
				{
					// recalcular
					TCOD_path_compute(ia_hostil.camí, loc_ia.posició.x, loc_ia.posició.y, loc_jugador.posició.x, loc_jugador.posició.y);
				}
				accions.push_back(acció::MoureEntitat{ id_ia, moviment });
			}
		}
		else if (m_mapa.és_a_la_vista(loc_ia.posició))
		{
			TCOD_path_compute(ia_hostil.camí, loc_ia.posició.x, loc_ia.posició.y, loc_jugador.posició.x, loc_jugador.posició.y);
		}
	}

	for (Acció acció : accions)
	{
		std::visit([this](auto& acció)
			{
				processar(*this, acció);
			}, acció);
	}


}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void Estat::pintar(tcod::Console& console) const
{
	m_mapa.pintar(console);

	for(auto [id, loc, pintat] : per_cada(localitzacions, pintats))
	{
		if (m_mapa.és_a_la_vista(loc.posició))
		{
			char const txt[2] = { pintat.caracter , '\0' };

			tcod::print(
				console,
				{ loc.posició.x, loc.posició.y },
				txt,
				TCOD_ColorRGB{ pintat.color.r, pintat.color.g, pintat.color.b },
				std::nullopt);
		}
	}
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

