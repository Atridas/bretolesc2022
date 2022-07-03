module;

// std
#include <cassert>
#include <optional>
#include <vector>

// 3rd party
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <libtcod.hpp>

export module EntradaSDL;

import Accions;

namespace entrada_sdl
{
	using namespace bretolesc;
	using namespace bretolesc::acció;

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
				return MoureJugador{ +1, 0};
			case SDLK_LEFT:
				return MoureJugador{ -1, 0 };
			case SDLK_DOWN:
				return MoureJugador{ 0, +1 };
			case SDLK_UP:
				return MoureJugador{ 0, -1 };

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

	export void ProcessarEvents(tcod::ContextPtr const& context, std::vector<Acció>& accions_)
	{
		SDL_Event evnt;
		SDL_WaitEvent(nullptr);  // Optional, sleep until events are available.
		while (SDL_PollEvent(&evnt))
		{
			context->convert_event_coordinates(evnt);  // Optional, converts pixel coordinates into tile coordinates.
			switch (evnt.type)
			{
			case SDL_QUIT:
				accions_.push_back(processar(evnt.quit));
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				if (std::optional<Acció> acció = processar(evnt.key))
				{
					accions_.push_back(*acció);
				}
				break;
			}
		}
	}

}
