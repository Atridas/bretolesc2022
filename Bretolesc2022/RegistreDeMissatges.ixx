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

		void reinicia();

		void afegir_missatge(std::string_view missatge, Color color, bool acumular = false);

		void pintar(tcod::Console& console) const;
		void pintar_sencer(tcod::Console& console) const;

		void desplaçar_registre(int quantitat);
		void reiniciar_desplaçament()
		{
			línea_base = 0;
		}

	private:

		std::vector<std::string> missatges;
		std::vector<Color> colors;
		std::vector<int> repeticions;

		int línea_base = 0;

		void pintar_àrea(tcod::Console& console, Rectangle2D àrea) const;
	};
}