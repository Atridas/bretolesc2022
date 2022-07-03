module;

#include <cinttypes>

export module Comú;

export namespace bretolesc
{
	struct Color
	{
		uint8_t r, g, b;

		static Color const Blanc;
		static Color const Groc;
	};
}

// constants
bretolesc::Color const bretolesc::Color::Blanc = Color{ 255, 255, 255 };
bretolesc::Color const bretolesc::Color::Groc = Color{ 255, 255, 0 };
