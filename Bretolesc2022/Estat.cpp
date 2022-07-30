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

Estat::Estat(int _amplada, int _al�ada, Generador const& generador)
	: m_mapa(_amplada, _al�ada)
	, tancar(false)
{
	id_jugador = generador.generar(*this);
}


Estat::~Estat()
{
	for (auto [id, ia_hostil] : ias_hostils)
	{
		TCOD_path_delete(ia_hostil.cam�);
	}
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

	for (auto [id, loc] : localitzacions)
	{
		if (loc.posici� == coordenades)
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
		if (loc.posici� == coordenades && loc.bloqueja_el_pas)
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
	// - l'objectiu �s el jugador
	// - si el jugador �s a dist�ncia (manhattan) 1, atacar-lo
	// - si tenim un cam� definit, seguir-lo
	// - si el jugador �s visible (si nosaltres som visibles des del jugador), crear un cam� fins a ell
	// - en cas contrari no fer res

	Localitzaci� loc_jugador = obtenir_component<Localitzaci�>(id_jugador);

	std::vector<Acci�> accions;

	for (auto [id_ia, loc_ia, ia_hostil] : per_cada(localitzacions, ias_hostils))
	{
		if (dist�ncia_manhattan(loc_jugador.posici�, loc_ia.posici�) == 1)
		{
			accions.push_back( acci�::AtacCosACos{id_ia, id_jugador} );
		}
		else if (!TCOD_path_is_empty(ia_hostil.cam�))
		{
			Punt2D dest�;
			TCOD_path_get_destination(ia_hostil.cam�, &dest�.x, &dest�.y);
			if (dist�ncia_manhattan(dest�, loc_jugador.posici�) > 2)
			{
				// recalcular
				TCOD_path_compute(ia_hostil.cam�, loc_ia.posici�.x, loc_ia.posici�.y, loc_jugador.posici�.x, loc_jugador.posici�.y);
			}

			if (TCOD_path_walk(ia_hostil.cam�, &dest�.x, &dest�.y, true))
			{
				Vector2D moviment = dest� - loc_ia.posici�;
				if (std::abs(moviment.x) > 2 || std::abs(moviment.y) > 2)
				{
					// recalcular
					TCOD_path_compute(ia_hostil.cam�, loc_ia.posici�.x, loc_ia.posici�.y, loc_jugador.posici�.x, loc_jugador.posici�.y);
				}
				accions.push_back(acci�::MoureEntitat{ id_ia, moviment });
			}
		}
		else if (m_mapa.�s_a_la_vista(loc_ia.posici�))
		{
			TCOD_path_compute(ia_hostil.cam�, loc_ia.posici�.x, loc_ia.posici�.y, loc_jugador.posici�.x, loc_jugador.posici�.y);
		}
	}

	for (Acci� acci� : accions)
	{
		std::visit([this](auto& acci�)
			{
				processar(*this, acci�);
			}, acci�);
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
		if (m_mapa.�s_a_la_vista(loc.posici�))
		{
			char const txt[2] = { pintat.caracter , '\0' };

			tcod::print(
				console,
				{ loc.posici�.x, loc.posici�.y },
				txt,
				TCOD_ColorRGB{ pintat.color.r, pintat.color.g, pintat.color.b },
				std::nullopt);
		}
	}
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

