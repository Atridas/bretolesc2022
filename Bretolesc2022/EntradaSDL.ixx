module;

// std
#include <cassert>
#include <optional>
#include <vector>

// 3rd party
#include <SDL2/SDL.h>
#include <libtcod.hpp>

export module EntradaSDL;

import EstructuresAccions;

namespace entrada_sdl
{
	using namespace bretolesc;
	using namespace bretolesc::acci�;

	Acci� processar(SDL_QuitEvent const &evnt)
	{
		assert(evnt.type == SDL_QUIT);
		return Finalitzar{};
	};


	std::optional<Acci�> processar_viu(SDL_KeyboardEvent const& evnt)
	{
		if (evnt.type == SDL_KEYDOWN)
		{
			switch (evnt.keysym.sym)
			{
			case SDLK_RIGHT:
				return BatzegadaJugador{ +1, 0};
			case SDLK_LEFT:
				return BatzegadaJugador{ -1, 0 };
			case SDLK_DOWN:
				return BatzegadaJugador{ 0, +1 };
			case SDLK_UP:
				return BatzegadaJugador{ 0, -1 };
			case SDLK_SPACE:
				return NoFerRes{ };

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

	std::optional<Acci�> processar_mort(SDL_KeyboardEvent const& evnt)
	{
		if (evnt.type == SDL_KEYDOWN)
		{
			switch (evnt.keysym.sym)
			{
			case SDLK_SPACE:
				return Reiniciar{ };

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

	export void ProcessarEvents(tcod::ContextPtr const& context, bool jugador_�s_viu, std::vector<Acci�>& accions_)
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
				if (std::optional<Acci�> acci� = jugador_�s_viu ? processar_viu(evnt.key) : processar_mort(evnt.key))
				{
					accions_.push_back(*acci�);
				}
				break;
			}
		}
	}

}
