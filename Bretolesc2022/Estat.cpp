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
import :Interf�cieUsuari;
import :PaletaDeColors;

import EstructuresAccions;

using namespace bretolesc;
using namespace bretolesc::component;
using namespace bretolesc::etiqueta;

// --------------------------------------------------------------------------------------

static std::string MissatgeBenvinguda = "Aventurer, sigui benvingut a un altre calabos";

// --------------------------------------------------------------------------------------


Estat::Estat(int _amplada, int _al�ada, Generador const& generador)
	: m_mapa(_amplada, _al�ada)
{
	id_jugador = generador.generar(*this);
	registre.afegir_missatge(MissatgeBenvinguda, iu::Paleta::TextBenvinguda);
}


Estat::~Estat()
{
	for (auto [id, ia_hostil] : col�leccions.obtenir_col�lecci�<IAHostil>())
	{
		TCOD_path_delete(ia_hostil.cam�);
	}
}


void Estat::reinicia(Generador const& generador)
{
	for (auto [id, ia_hostil] : col�leccions.obtenir_col�lecci�<IAHostil>())
	{
		TCOD_path_delete(ia_hostil.cam�);
	}

	col�leccions.reinicia();
	etiquetes.reinicia();

	m_mapa.reinicia();
	registre.reinicia();

	id_jugador = generador.generar(*this);
	registre.afegir_missatge(MissatgeBenvinguda, iu::Paleta::TextBenvinguda);
	// 

	actualitzar_visi�();

	reiniciar = false;
}

void Estat::actualitzar_visi�()
{
	m_mapa.actualitzar_camp_de_visi�(obtenir_component<Localitzaci�>(obtenir_id_jugador()).posici�, profunditat_de_visi�);
}

bool Estat::jugador_�s_viu() const
{
	return potser_obtenir_component<Lluitador>(id_jugador).has_value();
}

ModeEntrada Estat::obtenir_mode_entrada() const
{
	if (mostrar_registre)
	{
		return ModeEntrada::Registre;
	}
	else if (jugador_�s_viu())
	{
		return ModeEntrada::Viu;
	}
	else
	{
		return ModeEntrada::Mort;
	}
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

std::optional<IdEntitat> Estat::buscar_entitat(Punt2D coordenades) const
{
	std::optional<IdEntitat> resultat;

	for (auto [id, loc] : col�leccions.obtenir_col�lecci�<Localitzaci�>())
	{
		if (loc.posici� == coordenades)
		{
			resultat = id;
		}
	}

	return resultat;
}

std::vector<IdEntitat> Estat::buscar_entitats(Punt2D coordenades) const
{
	std::vector<IdEntitat> resultat;

	for (auto [id, loc] : col�leccions.obtenir_col�lecci�<Localitzaci�>())
	{
		if (loc.posici� == coordenades)
		{
			resultat.push_back(id);
		}
	}

	return resultat;
}

std::optional<IdEntitat> Estat::buscar_entitat_bloquejant(Punt2D coordenades) const
{
	std::optional<IdEntitat> resultat;

	for (auto [id, loc] : col�leccions.obtenir_col�lecci�<Localitzaci�>())
	{
		if (loc.posici� == coordenades && etiquetes.t�<BloquejaElPas>(id))
		{
			resultat = id;
		}
	}


	return resultat;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void Estat::actualitzar_l�gica()
{
	actualitzar_ias_hostils();
	buscar_morts();
}

void Estat::actualitzar_ias_hostils()
{
	// - l'objectiu �s el jugador
	// - si el jugador �s a dist�ncia (manhattan) 1, atacar-lo
	// - si tenim un cam� definit, seguir-lo
	// - si el jugador �s visible (si nosaltres som visibles des del jugador), crear un cam� fins a ell
	// - en cas contrari no fer res

	Localitzaci� loc_jugador = obtenir_component<Localitzaci�>(id_jugador);

	std::vector<Acci�Entitat> accions;

	for (auto [id_ia, loc_ia, ia_hostil] : col�leccions.per_cada<Localitzaci�, IAHostil>())
	{
		if (dist�ncia_manhattan(loc_jugador.posici�, loc_ia.posici�) == 1)
		{
			accions.push_back( acci�_entitat::AtacCosACos{id_ia, id_jugador} );
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
				accions.push_back(acci�_entitat::Moure{ id_ia, moviment });
			}
		}
		else if (m_mapa.�s_a_la_vista(loc_ia.posici�))
		{
			TCOD_path_compute(ia_hostil.cam�, loc_ia.posici�.x, loc_ia.posici�.y, loc_jugador.posici�.x, loc_jugador.posici�.y);
		}
	}

	for (Acci�Entitat acci� : accions)
	{
		std::visit([this](auto& acci�)
			{
				processar(*this, acci�);
			}, acci�);
	}
}

void Estat::buscar_morts()
{
	std::vector<IdEntitat> morts;
	for (auto [id, lluitador] : col�leccions.obtenir_col�lecci�<Lluitador>())
	{
		if (lluitador.salut <= 0)
		{
			morts.push_back(id);
		}
	}

	for (IdEntitat id : morts)
	{
		char buffer[2048];
		sprintf_s(buffer, 2048, "Un %s ha mort!", obtenir_component<Nom>(id).nom.c_str());

		registre.afegir_missatge(
			buffer,
			id == obtenir_id_jugador() ? iu::Paleta::MortJugador : iu::Paleta::MortEnemic,
			true);

		col�leccions.modificar_o_treure([](auto& component)
			{
				using Component = std::decay_t<decltype(component)>;
				if constexpr (std::is_same_v<Component, Nom> || std::is_same_v<Component, Localitzaci�>)
				{
					return false;
				}
				else if constexpr (std::is_same_v<Component, Pintat>)
				{
					component.car�cter = '%';
					component.color = { 191,0,0 };
					component.prioritat = PrioritatPintar::Cad�ver;
					return false;
				}
				else if constexpr (std::is_same_v<Component, IAHostil>)
				{
					TCOD_path_delete(component.cam�);
					return true;
				}
				else
				{
					return true;
				}
			}, id);
		etiquetes.treure([](auto* etiqueta)
			{
				using Etiqueta = std::decay_t<decltype(etiqueta)>;
				//std::is_same_v<Etiqueta, BloquejaElPas*>;
				return true;
			}, id);
	}
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void Estat::pintar(tcod::Console& console) const
{
	m_mapa.pintar(console);

	auto pintar_tipus = [this, &console](PrioritatPintar prioritat)
	{
		for (auto [id, loc, pintat] : col�leccions.per_cada<Localitzaci�, Pintat>())
		{
			if (pintat.prioritat == prioritat && m_mapa.�s_a_la_vista(loc.posici�))
			{
				char const txt[2] = { pintat.car�cter , '\0' };

				tcod::print(
					console,
					{ loc.posici�.x, loc.posici�.y },
					txt,
					TCOD_ColorRGB{ pintat.color.r, pintat.color.g, pintat.color.b },
					std::nullopt);
			}
		}
	};

	pintar_tipus(PrioritatPintar::Cad�ver);
	pintar_tipus(PrioritatPintar::Objecte);
	pintar_tipus(PrioritatPintar::Actor);

	iu::pintar_barra_de_vida(*this, console);
	iu::pintar_info_ratol�(*this, console);
	
	registre.pintar(console);

	if (mostrar_registre)
		registre.pintar_sencer(console);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

