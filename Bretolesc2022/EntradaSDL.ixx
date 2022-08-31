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
import ModeEntrada;

namespace entrada_sdl
{
	using namespace bretolesc;
	using namespace bretolesc::acci�_usuari;

	Acci�Usuari processar(SDL_QuitEvent const &evnt)
	{
		assert(evnt.type == SDL_QUIT);
		return Finalitzar{};
	};

	Acci�Usuari processar(SDL_MouseMotionEvent const& evnt)
	{
		assert(evnt.type == SDL_MOUSEMOTION);
		return MoureRatol�{ Punt2D{evnt.x, evnt.y} };
	}

	std::optional<Acci�Usuari> processar_viu(SDL_KeyboardEvent const& evnt)
	{
		if (evnt.type == SDL_KEYDOWN)
		{
			switch (evnt.keysym.sym)
			{
			case SDLK_TAB:
				return AlternarInventari{};

			case SDLK_ESCAPE:
				return Finalitzar{};

			case SDLK_SPACE:
				return NoFerRes{ };

			case SDLK_g:
				return Agafar{};

			case SDLK_v:
				return AlternarRegistre{};

			case SDLK_RIGHT:
				return Batzegada{ +1, 0 };
			case SDLK_LEFT:
				return Batzegada{ -1, 0 };
			case SDLK_DOWN:
				return Batzegada{ 0, +1 };
			case SDLK_UP:
				return Batzegada{ 0, -1 };
			default:
				return std::nullopt;
			}
		}
		else
		{
			assert(evnt.type == SDL_KEYUP);
			return std::nullopt;
		}
	};

	std::optional<Acci�Usuari> processar_mort(SDL_KeyboardEvent const& evnt)
	{
		if (evnt.type == SDL_KEYDOWN)
		{
			switch (evnt.keysym.sym)
			{
			case SDLK_ESCAPE:
				return Finalitzar{};

			case SDLK_SPACE:
				return Reiniciar{ };

			case SDLK_v:
				return AlternarRegistre{};

			default:
				return std::nullopt;
			}
		}
		else
		{
			assert(evnt.type == SDL_KEYUP);
			return std::nullopt;
		}
	};

	std::optional<Acci�Usuari> processar_registre(SDL_KeyboardEvent const& evnt)
	{
		if (evnt.type == SDL_KEYDOWN)
		{
			switch (evnt.keysym.sym)
			{
			case SDLK_ESCAPE:
			case SDLK_SPACE:
			case SDLK_v:
				return AlternarRegistre{};

			case SDLK_DOWN:
				return Despla�arRegistre{ +1 };
			case SDLK_UP:
				return Despla�arRegistre{ -1 };


			case SDLK_PAGEUP:
				return Despla�arRegistre{ -10 };
			case SDLK_PAGEDOWN:
				return Despla�arRegistre{ +10 };

			default:
				return std::nullopt;
			}
		}
		else
		{
			assert(evnt.type == SDL_KEYUP);
			return std::nullopt;
		}
	}

	std::optional<Acci�Usuari> processar_inventari(SDL_KeyboardEvent const& evnt)
	{
		if (evnt.type == SDL_KEYDOWN)
		{
			switch (evnt.keysym.sym)
			{
			case SDLK_TAB:
			case SDLK_ESCAPE:
			case SDLK_SPACE:
				return AlternarInventari{};

			case SDLK_DOWN:
				return Despla�arInventari{ +1 };
			case SDLK_UP:
				return Despla�arInventari{ -1 };


			case SDLK_PAGEUP:
				return Despla�arInventari{ -10 };
			case SDLK_PAGEDOWN:
				return Despla�arInventari{ +10 };

			default:
				return std::nullopt;
			}
		}
		else
		{
			assert(evnt.type == SDL_KEYUP);
			return std::nullopt;
		}
	}

	export void ProcessarEvents(tcod::ContextPtr const& context, ModeEntrada mode_entrada, std::vector<Acci�Usuari>& accions_)
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
			{
				std::optional<Acci�Usuari> acci�;
				switch (mode_entrada)
				{
				case ModeEntrada::Viu:
					acci� = processar_viu(evnt.key);
					break;
				case ModeEntrada::Mort:
					acci� = processar_mort(evnt.key);
					break;
				case ModeEntrada::Inventari:
					acci� = processar_inventari(evnt.key);
					break;
				case ModeEntrada::Registre:
					acci� = processar_registre(evnt.key);
					break;
				default:
					assert(false);
					break;
				}

				if (acci�)
				{
					accions_.push_back(*acci�);
				}
				break;
			}
			case SDL_MOUSEMOTION:
				accions_.push_back(processar(evnt.motion));
				break; 
			}
		}
	}

}
