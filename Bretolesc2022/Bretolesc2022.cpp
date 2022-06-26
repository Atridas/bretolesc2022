// imports
import Accions;
import EntradaSDL;
import EstatJoc;
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
    // constants
    using namespace acció;

    int const screen_width = 80;
    int const screen_height = 50;

    // ------------------------------------------------------------------------
    // Inicialització de les llibreries
    // ------------------------------------------------------------------------
    tcod::Console console(screen_width, screen_height);  // Main console.

    // Configure the context.
    TCOD_ContextParams params = {};
    params.tcod_version = TCOD_COMPILEDVERSION;  // This is required.
    params.console = console.get();  // Derive the window size from the console size.
    params.window_title = (char const*)u8"Brètolesc";
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
    bretolesc::Estat estat = {};
    estat.player_x = screen_width / 2;
    estat.player_y = screen_height / 2;

    std::vector<Acció> accions;

    // ------------------------------------------------------------------------
    // Game loop.
    // ------------------------------------------------------------------------
    while (!estat.tancar) 
    {
        // --------------------------------------------------------------------
        // update
        // --------------------------------------------------------------------
        for (Acció acció : accions)
        {
            std::visit([&estat](auto &acció)
                {
                    processar(estat, acció);
                }, acció);
        }
        accions.clear();

        // --------------------------------------------------------------------
        // render
        // --------------------------------------------------------------------
        TCOD_console_clear(console.get());
        tcod::print(console, { estat.player_x, estat.player_y }, "@", std::nullopt, std::nullopt);
        context->present(console);  // Updates the visible display.

        // --------------------------------------------------------------------
        // sdl events
        // --------------------------------------------------------------------
        SDL_Event evnt;
        SDL_WaitEvent(nullptr);  // Optional, sleep until events are available.
        while (SDL_PollEvent(&evnt))
        {
            context->convert_event_coordinates(evnt);  // Optional, converts pixel coordinates into tile coordinates.
            switch (evnt.type)
            {
            case SDL_QUIT:
                accions.push_back(entrada_sdl::processar(evnt.quit));
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                if (std::optional<Acció> acció = entrada_sdl::processar(evnt.key))
                {
                    accions.push_back(*acció);
                }
                break;
            }
        }
    }

    return 0;
}
