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
	using namespace bretolesc::acció_usuari;

	AccióUsuari processar(SDL_QuitEvent const &evnt)
	{
		assert(evnt.type == SDL_QUIT);
		return Finalitzar{};
	};

	std::optional<AccióUsuari> processar(SDL_MouseMotionEvent const& evnt, ModeEntrada mode_entrada)
	{
		assert(evnt.type == SDL_MOUSEMOTION);
		switch (mode_entrada)
		{
		case ModeEntrada::Viu:
		case ModeEntrada::Mort:
			return MoureRatolí{ Punt2D{evnt.x, evnt.y} };
		case ModeEntrada::Cursor:
			return EstablirCursor{ Punt2D{evnt.x, evnt.y} };
		default:
			return std::nullopt;
		}
	}

	std::optional<AccióUsuari> processar_viu(SDL_KeyboardEvent const& evnt)
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

	std::optional<AccióUsuari> processar_cursor(SDL_KeyboardEvent const& evnt)
	{
		if (evnt.type == SDL_KEYDOWN)
		{
			switch (evnt.keysym.sym)
			{

			case SDLK_RETURN:
				return AcceptarCursor{ };

			case SDLK_ESCAPE:
			case SDLK_SPACE:
				return CancelarCursor{ };

			case SDLK_RIGHT:
				return MoureCursor{ Vector2D{ +1, 0 } };
			case SDLK_LEFT:
				return MoureCursor{ Vector2D{ -1, 0 } };
			case SDLK_DOWN:
				return MoureCursor{ Vector2D{ 0, +1 } };
			case SDLK_UP:
				return MoureCursor{ Vector2D{ 0, -1 } };
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

	std::optional<AccióUsuari> processar_mort(SDL_KeyboardEvent const& evnt)
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

	std::optional<AccióUsuari> processar_registre(SDL_KeyboardEvent const& evnt)
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
				return DesplaçarRegistre{ +1 };
			case SDLK_UP:
				return DesplaçarRegistre{ -1 };


			case SDLK_PAGEUP:
				return DesplaçarRegistre{ -10 };
			case SDLK_PAGEDOWN:
				return DesplaçarRegistre{ +10 };

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

	std::optional<AccióUsuari> processar_inventari(SDL_KeyboardEvent const& evnt)
	{
		if (evnt.type == SDL_KEYDOWN)
		{
			switch (evnt.keysym.sym)
			{
			case SDLK_TAB:
			case SDLK_ESCAPE:
			case SDLK_SPACE:
				return AlternarInventari{};


			case SDLK_i:
				return Consumir{};
			case SDLK_d:
				return Tirar{};

			case SDLK_DOWN:
				return DesplaçarInventari{ +1 };
			case SDLK_UP:
				return DesplaçarInventari{ -1 };


			case SDLK_PAGEUP:
				return DesplaçarInventari{ -10 };
			case SDLK_PAGEDOWN:
				return DesplaçarInventari{ +10 };

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

	export void ProcessarEvents(tcod::ContextPtr const& context, ModeEntrada mode_entrada, std::vector<AccióUsuari>& accions_)
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
				std::optional<AccióUsuari> acció;
				switch (mode_entrada)
				{
				case ModeEntrada::Viu:
					acció = processar_viu(evnt.key);
					break;
				case ModeEntrada::Cursor:
					acció = processar_cursor(evnt.key);
					break;
				case ModeEntrada::Mort:
					acció = processar_mort(evnt.key);
					break;
				case ModeEntrada::Inventari:
					acció = processar_inventari(evnt.key);
					break;
				case ModeEntrada::Registre:
					acció = processar_registre(evnt.key);
					break;
				default:
					assert(false);
					break;
				}

				if (acció)
				{
					accions_.push_back(*acció);
				}
				break;
			}
			case SDL_MOUSEMOTION:
				if (auto acció = processar(evnt.motion, mode_entrada))
				{
					accions_.push_back(*acció);
				}
				break; 
			}
		}
	}

}
