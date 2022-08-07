module;

#include <cinttypes>
#include <cmath>

// 3rd party
#include <libtcod.hpp>

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
		static Color const GrisFosc;

		operator TCOD_ColorRGB() const
		{
			return TCOD_ColorRGB{ r,g,b };
		}
	};

	struct Punt2D
	{
		int x, y;
	};

	struct Vector2D
	{
		int x, y;
	};

	struct Rectangle2D
	{
		Punt2D orígen;
		Vector2D mida;
	};
}

// constants
bretolesc::Color const bretolesc::Color::Blanc = Color{ 0xff, 0xff, 0xff };
bretolesc::Color const bretolesc::Color::Groc = Color{ 0xff, 0xff, 0x00 };
bretolesc::Color const bretolesc::Color::Negre = Color{ 0x00, 0x00, 0x00 };
bretolesc::Color const bretolesc::Color::GrisFosc = Color{ 0x33, 0x33, 0x33 };


// operacions
export namespace bretolesc
{
	bool operator==(Punt2D a, Punt2D b)
	{
		return (a.x == b.x) && (a.y == b.y);
	}

	bool operator==(Color a, Color b)
	{
		return (a.r == b.r) && (a.g == b.g) && (a.b == b.b);
	}

	Punt2D operator+(Punt2D p, Vector2D v)
	{
		return Punt2D{ p.x + v.x, p.y + v.y };
	}

	Punt2D operator+(Vector2D v, Punt2D p)
	{
		return Punt2D{ p.x + v.x, p.y + v.y };
	}

	Vector2D operator-(Punt2D p1, Punt2D p2)
	{
		return Vector2D{ p1.x - p2.x, p1.y - p2.y };
	}

	int distància_manhattan(Punt2D a, Punt2D b)
	{
		return std::abs(a.x - b.x) + std::abs(a.y - b.y);
	}

	Color mescla(Color a, Color b, float alpha)
	{
		float i = 1 - alpha;
		return Color{ (uint8_t)(a.r * i + b.r * alpha), (uint8_t)(a.g * i + b.g * alpha), (uint8_t)(a.b * i + b.b * alpha) };
	}
}
