module;

// std
#include <cassert>

#include <string>
#include <string_view>
#include <vector>

// 3rd party
#include <libtcod.hpp>

export module Motor:RegistreDeMissatges;

import :PaletaDeColors;

import Comú;

namespace bretolesc {

	class RegistreDeMissatges
	{
	public:

		RegistreDeMissatges() = default;
		RegistreDeMissatges(RegistreDeMissatges const&) = delete;
		RegistreDeMissatges(RegistreDeMissatges&&) = delete;
		RegistreDeMissatges& operator=(RegistreDeMissatges const&) = delete;
		RegistreDeMissatges& operator=(RegistreDeMissatges&&) = delete;

		void reinicia()
		{
			missatges.clear();
			colors.clear();
			repeticions.clear();
		}

		void afegir_missatge(std::string_view missatge, Color color, bool acumular = false)
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

		void pintar(tcod::Console& console) const
		{
			assert(missatges.size() == colors.size());
			assert(missatges.size() == repeticions.size());

			int missatge = (int)missatges.size() - 1;
			int línea = 45;
			while (missatge >= 0 && línea < 50)
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
					std::array<int, 4>{21, línea, 40, 50 - línea},
					buffer,
					colors[missatge],
					std::nullopt);

				línea += tcod::get_height_rect(40, buffer);
				--missatge;
			}
		}

	private:

		std::vector<std::string> missatges;
		std::vector<Color> colors;
		std::vector<int> repeticions;

	};
}