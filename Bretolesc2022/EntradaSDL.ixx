module;

// std
#include <cassert>
#include <optional>

// 3rd party
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

export module EntradaSDL;

import Accions;

export namespace entrada_sdl
{
	using namespace acció;

	Acció processar(SDL_QuitEvent const &evnt)
	{
		assert(evnt.type == SDL_QUIT);
		return Finalitzar{};
	};


	std::optional<Acció> processar(SDL_KeyboardEvent const& evnt)
	{
		if (evnt.type == SDL_KEYDOWN)
		{
			switch (evnt.keysym.sym)
			{
			case SDLK_RIGHT:
				return Moure{ +1, 0};
			case SDLK_LEFT:
				return Moure{ -1, 0 };
			case SDLK_DOWN:
				return Moure{ 0, +1 };
			case SDLK_UP:
				return Moure{ 0, -1 };

			case SDLK_ESCAPE:
				return Finalitzar{};
			default:
				return {};
			}
		}
		else
		{
			assert(evnt.type == SDL_KEYUP);
			return {};
		}
	};

}
