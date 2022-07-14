module;

// std
#include <cassert>
#include <cmath>
#include <array>
#include <vector>

// 3rd party
#include <libtcod.hpp>

module Mapa;

using namespace bretolesc;

Mapa::Mapa(int _amplada, int _al�ada, GeneradorDeMapa const& generador)
	: amplada(_amplada)
	, al�ada(_al�ada)
{
	assert(amplada > 0);
	assert(al�ada > 0);

	rajoles.resize(amplada * al�ada);
	rajoles_a_la_vista.resize(amplada * al�ada);
	rajoles_explorades.resize(amplada * al�ada);

	info_rajoles[(int)TipusRajola::Terra] = {
		true, // transitable
		false, // bloqueja_la_vista
		{' ', Color::Blanc, {200, 180, 50} }, // gr�fic_a_la_vista
		{' ', Color::Blanc, {50, 50, 150} } // gr�fic_fora_de_vista
	};
	info_rajoles[(int)TipusRajola::Paret] = {
		false, // transitable
		true, // bloqueja_la_vista
		{ ' ', Color::Blanc, {130, 110, 50} }, // gr�fic_a_la_vista
		{ ' ', Color::Blanc, {0, 0, 150} }// gr�fic_fora_de_vista
	};

	mortalla = { ' ', Color::Blanc, Color::Negre };

	generador.generar(*this);
}

void Mapa::actualitzar_camp_de_visi�(Punt2D origen, int profunditat_m�xima)
{
	for (int i = 0; i < rajoles_a_la_vista.size(); ++i)
	{
		rajoles_a_la_vista[i] = false;
	}

	struct RajolaPerExplorar
	{
		Punt2D r;
		int profunditat;
	};

	std::vector<RajolaPerExplorar> rajoles_per_actualitzar;

	rajoles_per_actualitzar.emplace_back(origen, profunditat_m�xima);

	while (!rajoles_per_actualitzar.empty())
	{
		RajolaPerExplorar rajola = rajoles_per_actualitzar[0];
		rajoles_per_actualitzar.erase(rajoles_per_actualitzar.begin());

		int idx = rajola.r.y * amplada + rajola.r.x;
		if (!rajoles_a_la_vista[idx])
		{
			rajoles_a_la_vista[idx] = true;
			rajoles_explorades[idx] = true;

			if (rajola.profunditat > 0 && !obtenir_rajola(rajola.r).bloqueja_la_vista)
			{
				rajoles_per_actualitzar.emplace_back(Punt2D{rajola.r.x + 1, rajola.r.y}, rajola.profunditat - 1);
				rajoles_per_actualitzar.emplace_back(Punt2D{rajola.r.x - 1, rajola.r.y}, rajola.profunditat - 1);
				rajoles_per_actualitzar.emplace_back(Punt2D{rajola.r.x, rajola.r.y + 1}, rajola.profunditat - 1);
				rajoles_per_actualitzar.emplace_back(Punt2D{rajola.r.x, rajola.r.y - 1}, rajola.profunditat - 1);
			}
		}
	}

}

void Mapa::pintar(tcod::Console& console) const
{
	for (int y = 0; y < al�ada; ++y)
		for (int x = 0; x < amplada; ++x)
		{
			int idx = a_�ndex({ x, y });

			Rajola const& rajola = obtenir_rajola({ x, y });
			Gr�fic gr�fic;
			if (rajoles_a_la_vista[idx])
			{
				gr�fic = rajola.gr�fic_a_la_vista;
			}
			else if (rajoles_explorades[idx])
			{
				gr�fic = rajola.gr�fic_fora_de_vista;
			}
			else
			{
				gr�fic = mortalla;
			}

			console[{x, y}] = gr�fic;
		}

	for (Entitat const& entitat : entitats)
	{
		if (�s_a_la_vista(entitat.posici�))
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

std::optional<IdEntitat> Mapa::buscar_entitat(Punt2D coordenades) const
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

std::optional<IdEntitat> Mapa::buscar_entitat_bloquejant(Punt2D coordenades) const
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

IdEntitat Mapa::afegir_entitat(Entitat const& entitat)
{
	IdEntitat id = (int)entitats.size();
	entitats.push_back(entitat);
	return id;
}

Entitat& Mapa::obtenir_entitat(IdEntitat const& entitat)
{
	assert(entitat >= 0);
	assert(entitat < (int)entitats.size());
	return entitats[entitat];
}

Entitat const& Mapa::obtenir_entitat(IdEntitat const& entitat) const
{
	assert(entitat >= 0);
	assert(entitat < (int)entitats.size());
	return entitats[entitat];
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void Mapa::actualitzar_enemics()
{
	for (int e = 0; e < (int)entitats.size(); ++e)
	{
		// PERFER
	}
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void GeneradorDeMapaDExemple::generar(Mapa& mapa) const
{
	for (int y = 0; y < mapa.obtenir_al�ada(); ++y)
		for (int x = 0; x < mapa.obtenir_amplada(); ++x)
		{
			mapa.establir_rajola({ x, y }, TipusRajola::Terra);
		}



	mapa.establir_rajola({ 30, 22 }, TipusRajola::Paret);
	mapa.establir_rajola({ 31, 22 }, TipusRajola::Paret);
	mapa.establir_rajola({ 32, 22 }, TipusRajola::Paret);
	mapa.establir_rajola({ 33, 22 }, TipusRajola::Paret);
}


