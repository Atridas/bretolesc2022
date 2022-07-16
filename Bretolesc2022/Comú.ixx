module;

#include <cinttypes>
#include <cmath>

export module Comú;

export import :NoiseRNG;

export namespace bretolesc
{
	struct Color
	{
		uint8_t r, g, b;

		static Color const Blanc;
		static Color const Groc;
		static Color const Negre;
	};

	struct Punt2D
	{
		int x, y;
	};

	struct Vector2D
	{
		int x, y;
	};
}

// constants
bretolesc::Color const bretolesc::Color::Blanc = Color{ 255, 255, 255 };
bretolesc::Color const bretolesc::Color::Groc = Color{ 255, 255, 0 };
bretolesc::Color const bretolesc::Color::Negre = Color{ 0, 0, 0 };


// operacions
export namespace bretolesc
{
	bool operator==(Punt2D a, Punt2D b)
	{
		return (a.x == b.x) && (a.y == b.y);
	}

	Punt2D operator+(Punt2D p, Vector2D v)
	{
		return Punt2D{ p.x + v.x, p.y + v.y };
	}

	Punt2D operator+(Vector2D v, Punt2D p)
	{
		return Punt2D{ p.x + v.x, p.y + v.y };
	}

	int distància_manhattan(Punt2D a, Punt2D b)
	{
		return std::abs(a.x - b.x) + std::abs(a.y - b.y);
	}
}
