module;

// std
#include <cassert>
#include <cmath>
#include <array>
#include <vector>

// 3rd party
#include <libtcod.hpp>

module Motor:Mapa;

using namespace bretolesc;

Mapa::Mapa(int _amplada, int _alçada)
	: amplada(_amplada)
	, alçada(_alçada)
{
	assert(amplada > 0);
	assert(alçada > 0);

	rajoles.resize(amplada * alçada);
	rajoles_a_la_vista.resize(amplada * alçada);
	rajoles_explorades.resize(amplada * alçada);

	info_rajoles[(int)TipusRajola::Terra] = {
		true, // transitable
		false, // bloqueja_la_vista
		{' ', Color::Blanc, {200, 180, 50} }, // gràfic_a_la_vista
		{' ', Color::Blanc, {50, 50, 150} } // gràfic_fora_de_vista
	};
	info_rajoles[(int)TipusRajola::Paret] = {
		false, // transitable
		true, // bloqueja_la_vista
		{ ' ', Color::Blanc, {130, 110, 50} }, // gràfic_a_la_vista
		{ ' ', Color::Blanc, {0, 0, 150} }// gràfic_fora_de_vista
	};

	mortalla = { ' ', Color::Blanc, Color::Negre };

	mapa_TCOD = TCOD_map_new(amplada, alçada);

	TCOD_map_clear(mapa_TCOD, false, false);
}

Mapa::~Mapa()
{
	TCOD_map_delete(mapa_TCOD);
}

void Mapa::establir_rajola(Punt2D r, TipusRajola tipus)
{
	assert(és_dins_del_límit(r));

	rajoles[a_índex(r)] = tipus;

	Rajola info = info_rajoles[(int)tipus];

	TCOD_map_set_properties(mapa_TCOD, r.x, r.y, !info.bloqueja_la_vista, info.transitable);
}

void Mapa::actualitzar_camp_de_visió(Punt2D origen, int profunditat_màxima)
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

	rajoles_per_actualitzar.emplace_back(origen, profunditat_màxima);

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
	for (int y = 0; y < alçada; ++y)
		for (int x = 0; x < amplada; ++x)
		{
			int idx = a_índex({ x, y });

			Rajola const& rajola = obtenir_rajola({ x, y });
			Gràfic gràfic;
			if (rajoles_a_la_vista[idx])
			{
				gràfic = rajola.gràfic_a_la_vista;
			}
			else if (rajoles_explorades[idx])
			{
				gràfic = rajola.gràfic_fora_de_vista;
			}
			else
			{
				gràfic = mortalla;
			}

			console[{x, y}] = gràfic;
		}
}


