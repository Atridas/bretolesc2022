module;

// std
#include <cassert>

#include <string>
#include <string_view>
#include <vector>

// 3rd party
#include <libtcod.hpp>

module Motor:RegistreDeMissatges;

import :PaletaDeColors;

import Comú;

using namespace bretolesc;

void RegistreDeMissatges::reinicia()
{
	missatges.clear();
	colors.clear();
	repeticions.clear();
}

void RegistreDeMissatges::afegir_missatge(std::string_view missatge, Color color, bool acumular)
{
	assert(missatges.size() == colors.size());
	assert(missatges.size() == repeticions.size());

	if (acumular && !missatges.empty() && missatges.back() == missatge)
	{
		assert(colors.back() == color);
		++repeticions.back();
	}
	else
	{
		missatges.push_back((std::string)missatge);
		colors.push_back(color);
		repeticions.push_back(1);
	}
}


void RegistreDeMissatges::desplaçar_registre(int quantitat)
{
	línea_base += quantitat;
	if (línea_base < 0)
	{
		línea_base = 0;
	}
	else if(línea_base > (int)missatges.size() - 1)
	{
		línea_base = (int)missatges.size() - 1;
	}
}

void RegistreDeMissatges::pintar(tcod::Console& console) const
{
	pintar_àrea(console, { {21, 40}, {40, 10} });
}

void RegistreDeMissatges::pintar_sencer(tcod::Console& console) const
{

	for (int y = 1; y < 49; ++y)
		for (int x = 9; x < 71; ++x)
		{
			auto& casella = console[{x, y}];
			casella.bg = iu::Paleta::FonsRegistre;
			casella.ch = ' ';
		}

	pintar_àrea(console, { {10, 2}, {59, 47} });
}


void RegistreDeMissatges::pintar_àrea(tcod::Console& console, Rectangle2D àrea) const
{
	assert(missatges.size() == colors.size());
	assert(missatges.size() == repeticions.size());

	int missatge = (int)missatges.size() - 1 - línea_base;
	int línea = 0;
	while (missatge >= 0 && línea < àrea.mida.y)
	{
		char buffer[2048];
		if (repeticions[missatge] == 1)
		{
			sprintf_s(buffer, 2048, "%s", missatges[missatge].c_str());
		}
		else
		{
			sprintf_s(buffer, 2048, "%s (x%d)", missatges[missatge].c_str(), repeticions[missatge]);
		}

		tcod::print_rect(
			console,
			std::array<int, 4>{àrea.orígen.x, àrea.orígen.y + línea, àrea.mida.x, àrea.orígen.y + àrea.mida.y - línea},
			buffer,
			colors[missatge],
			std::nullopt);

		línea += tcod::get_height_rect(àrea.mida.x, buffer);
		--missatge;
	}
}

