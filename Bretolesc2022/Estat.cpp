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
import :InterfícieUsuari;
import :PaletaDeColors;

import EstructuresAccions;

using namespace bretolesc;
using namespace bretolesc::component;
using namespace bretolesc::etiqueta;

// --------------------------------------------------------------------------------------

static std::string MissatgeBenvinguda = "Aventurer, sigui benvingut a un altre calabos";

// --------------------------------------------------------------------------------------


Estat::Estat(int _amplada, int _alçada, Generador const& generador)
	: m_mapa(_amplada, _alçada)
{
	id_jugador = generador.generar(*this);
	registre.afegir_missatge(MissatgeBenvinguda, iu::Paleta::TextBenvinguda);
}


Estat::~Estat()
{
	for (auto [id, ia_hostil] : col·leccions.obtenir_col·lecció<IAHostil>())
	{
		TCOD_path_delete(ia_hostil.camí);
	}
}


void Estat::reinicia(Generador const& generador)
{
	for (auto [id, ia_hostil] : col·leccions.obtenir_col·lecció<IAHostil>())
	{
		TCOD_path_delete(ia_hostil.camí);
	}

	col·leccions.reinicia();
	etiquetes.reinicia();

	m_mapa.reinicia();
	registre.reinicia();

	id_jugador = generador.generar(*this);
	registre.afegir_missatge(MissatgeBenvinguda, iu::Paleta::TextBenvinguda);
	// 

	actualitzar_visió();

	reiniciar = false;
}

void Estat::actualitzar_visió()
{
	m_mapa.actualitzar_camp_de_visió(obtenir_component<Localització>(obtenir_id_jugador()).posició, profunditat_de_visió);
}

void Estat::alterna_registre()
{
	if (submode_joc == SubmodeJoc::Registre)
	{
		submode_joc = SubmodeJoc::Normal;
	}
	else
	{
		submode_joc = SubmodeJoc::Registre;
	}
	registre.reiniciar_desplaçament();
}

void Estat::alterna_inventari()
{
	if (submode_joc == SubmodeJoc::Inventari)
	{
		submode_joc = SubmodeJoc::Normal;
	}
	else
	{
		submode_joc = SubmodeJoc::Inventari;
	}
	posició_inventari = 0;
}

void Estat::desplaçar_inventari(int quantitat)
{
	Inventari const& inventari = obtenir_component<Inventari>(id_jugador);
	if (inventari.objectes.size() > 0)
	{
		posició_inventari = (posició_inventari + quantitat) % inventari.objectes.size();
		// PERFER donar la volta al reves
	}
}

void Estat::moure_cursor(Vector2D p)
{

}

void Estat::establir_cursor(Punt2D p)
{

}

void Estat::accepta_cursor()
{

}

void Estat::cancela_cursor()
{

}

bool Estat::jugador_és_viu() const
{
	return potser_obtenir_component<Lluitador>(id_jugador).has_value();
}

ModeEntrada Estat::obtenir_mode_entrada() const
{
	switch (submode_joc)
	{
	case SubmodeJoc::Normal:
		if (jugador_és_viu())
		{
			return ModeEntrada::Viu;
		}
		else
		{
			return ModeEntrada::Mort;
		}
	case SubmodeJoc::Registre:
		return ModeEntrada::Registre;
	case SubmodeJoc::Inventari:
		return ModeEntrada::Inventari;
	case SubmodeJoc::Cursor:
		return ModeEntrada::Cursor;
	default:
		assert(false);
		return ModeEntrada::Viu;
	}
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

std::optional<IdEntitat> Estat::buscar_entitat(Punt2D coordenades) const
{
	std::optional<IdEntitat> resultat;

	for (auto [id, loc] : col·leccions.obtenir_col·lecció<Localització>())
	{
		if (loc.posició == coordenades)
		{
			resultat = id;
		}
	}

	return resultat;
}

std::vector<IdEntitat> Estat::buscar_entitats(Punt2D coordenades) const
{
	std::vector<IdEntitat> resultat;

	for (auto [id, loc] : col·leccions.obtenir_col·lecció<Localització>())
	{
		if (loc.posició == coordenades)
		{
			resultat.push_back(id);
		}
	}

	return resultat;
}

std::optional<IdEntitat> Estat::buscar_entitat_bloquejant(Punt2D coordenades) const
{
	std::optional<IdEntitat> resultat;

	for (auto [id, loc] : col·leccions.obtenir_col·lecció<Localització>())
	{
		if (loc.posició == coordenades && etiquetes.té<BloquejaElPas>(id))
		{
			resultat = id;
		}
	}


	return resultat;
}

std::optional<Estat::EntitatIDistància> Estat::buscar_entitat_lluitadora_més_propera(IdEntitat entitat) const
{
	std::optional<EntitatIDistància> resultat;

	Punt2D posició_entitat = col·leccions.obtenir_component<Localització>(entitat).posició;

	for (auto [id, loc, lluitador] : col·leccions.per_cada<Localització, Lluitador>())
	{
		if (id != entitat)
		{
			float distància = distància_euclidiana(posició_entitat, loc.posició);
			if (!resultat.has_value() || resultat->distància > distància)
			{
				resultat = EntitatIDistància{ id, distància };
			}
		}
	}

	return resultat;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void Estat::actualitzar_lógica()
{
	actualitzar_ias_hostils();
	buscar_morts();
}

void Estat::actualitzar_ias_hostils()
{
	// - l'objectiu és el jugador
	// - si el jugador és a distància (manhattan) 1, atacar-lo
	// - si tenim un camí definit, seguir-lo
	// - si el jugador és visible (si nosaltres som visibles des del jugador), crear un camí fins a ell
	// - en cas contrari no fer res

	Localització loc_jugador = obtenir_component<Localització>(id_jugador);

	std::vector<AccióEntitat> accions;

	for (auto [id_ia, loc_ia, ia_hostil] : col·leccions.per_cada<Localització, IAHostil>())
	{
		if (distància_manhattan(loc_jugador.posició, loc_ia.posició) == 1)
		{
			accions.push_back( acció_entitat::AtacCosACos{id_ia, id_jugador} );
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
				accions.push_back(acció_entitat::Moure{ id_ia, moviment });
			}
		}
		else if (m_mapa.és_a_la_vista(loc_ia.posició))
		{
			TCOD_path_compute(ia_hostil.camí, loc_ia.posició.x, loc_ia.posició.y, loc_jugador.posició.x, loc_jugador.posició.y);
		}
	}

	for (AccióEntitat acció : accions)
	{
		std::visit([this](auto& acció)
			{
				processar(*this, acció);
			}, acció);
	}
}

void Estat::buscar_morts()
{
	std::vector<IdEntitat> morts;
	for (auto [id, lluitador] : col·leccions.obtenir_col·lecció<Lluitador>())
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

		col·leccions.modificar_o_treure([]<typename Component>(Component& component)
			{
				if constexpr (std::is_same_v<Component, Nom> || std::is_same_v<Component, Localització>)
				{
					return false;
				}
				else if constexpr (std::is_same_v<Component, Pintat>)
				{
					component.caràcter = '%';
					component.color = { 191,0,0 };
					component.prioritat = PrioritatPintar::Cadàver;
					return false;
				}
				else if constexpr (std::is_same_v<Component, IAHostil>)
				{
					TCOD_path_delete(component.camí);
					return true;
				}
				else
				{
					return true;
				}
			}, id);
		etiquetes.treure([]<typename Etiqueta>(Etiqueta const* etiqueta)
			{
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
		for (auto [id, loc, pintat] : col·leccions.per_cada<Localització, Pintat>())
		{
			if (pintat.prioritat == prioritat && m_mapa.és_a_la_vista(loc.posició))
			{
				char const txt[2] = { pintat.caràcter , '\0' };

				tcod::print(
					console,
					{ loc.posició.x, loc.posició.y },
					txt,
					TCOD_ColorRGB{ pintat.color.r, pintat.color.g, pintat.color.b },
					std::nullopt);
			}
		}
	};

	pintar_tipus(PrioritatPintar::Cadàver);
	pintar_tipus(PrioritatPintar::Objecte);
	pintar_tipus(PrioritatPintar::Actor);

	iu::pintar_barra_de_vida(*this, console);
	iu::pintar_info_ratolí(*this, console);
	
	registre.pintar(console);

	switch (submode_joc)
	{
	case SubmodeJoc::Inventari:
		iu::pintar_inventari(*this, console);
		break;
	case SubmodeJoc::Registre:
		registre.pintar_sencer(console);
		break;
	}
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

