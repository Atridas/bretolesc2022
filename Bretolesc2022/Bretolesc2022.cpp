// imports
import Accions;
import Comú;
import EntradaSDL;
import EstatJoc;
import Mapa;
import Motor;
import ProcessadorAccions;

// std
#include <optional>
#include <variant>
#include <vector>

// 3rd party
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <libtcod.hpp>


int main(int argc, char* argv[])
{
    int const screen_width = 80;
    int const screen_height = 50;
    
    int const map_width = 80;
    int const map_height = 45;

    // ------------------------------------------------------------------------
    // Inicialització de les llibreries
    // ------------------------------------------------------------------------
    tcod::Console console(screen_width, screen_height);  // Main console.

    // Configure the context.
    TCOD_ContextParams params = {};
    params.tcod_version = TCOD_COMPILEDVERSION;  // This is required.
    params.console = console.get();  // Derive the window size from the console size.
    params.window_title = (char const*)u8"Bretolesc";
    params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
    params.vsync = true;
    params.argc = argc;  // This allows some user-control of the context.
    params.argv = argv;

    auto tileset = tcod::load_tilesheet("dejavu10x10_gs_tc.png", {32, 8}, tcod::CHARMAP_TCOD);
    params.tileset = tileset.get();

    tcod::ContextPtr context = tcod::new_context(params);

    // ------------------------------------------------------------------------
    // Initialització de l'estat del joc
    // ------------------------------------------------------------------------
    bretolesc::Mapa mapa(map_width, map_height);
    bretolesc::Estat estat = {};
    {
        bretolesc::Entitat jugador = {};
        jugador.x = screen_width / 2;
        jugador.y = screen_height / 2;
        jugador.caracter = '@';
        jugador.color = bretolesc::Color::Blanc;

        estat.jugador = (int)estat.entitats.size();
        estat.entitats.push_back(jugador);
    }
    {
        bretolesc::Entitat npc = {};
        npc.x = screen_width / 2 - 5;
        npc.y = screen_height / 2;
        npc.caracter = '@';
        npc.color = bretolesc::Color::Groc;

        estat.npc = (int)estat.entitats.size();
        estat.entitats.push_back(npc);
    }

    std::vector<bretolesc::Acció> accions;

    // ------------------------------------------------------------------------
    // Game loop.
    // ------------------------------------------------------------------------
    while (!estat.tancar) 
    {
        bretolesc::motor::executar_accions(accions, estat);
        accions.clear();

        bretolesc::motor::pintar(console, context, mapa, estat);

        entrada_sdl::ProcessarEvents(context, accions);
    }

    return 0;
}
