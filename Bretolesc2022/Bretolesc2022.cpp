// imports
import Comú;
import Entitats;
import EntradaSDL;
import EstructuresAccions;
import GeneradorProcedural;
import Motor;

// std
#include <optional>
#include <random>
#include <variant>
#include <vector>

// 3rd party
#include <SDL2/SDL.h>
#include <libtcod.hpp>




// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// 
// PERFER
// - Game Over de victòria!
// - TCOD::pring -> TCOD_console_printf
// - Reportar bug
//   - tutorial 12
//   - llavor 0x10d5da89
//   -        0x5eda1105
// 
// - 8ª PART
// 
// ✔️ 7ª PART
// ✔️ 6ª PART
// 
// 
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------




int SDL_main(int argc, char* argv[])
{
    int const screen_width = 80;
    int const screen_height = 50;
    
    int const map_width = 80;
    int const map_height = 43;

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
    bretolesc::GeneradorProcedural generador{};

    uint32_t llavor = std::random_device{}();
    printf("Generant amb llavor 0x%x\n", llavor);
    generador.establir_llavor(llavor);

    generador.establir_num_habitacions(5, 15);

    bretolesc::Estat estat(map_width, map_height, generador);

    estat.actualitzar_visió();

    std::vector<bretolesc::AccióUsuari> accions;

    // ------------------------------------------------------------------------
    // Game loop.
    // ------------------------------------------------------------------------
    while (!estat.vol_ser_tancat())
    {
        bretolesc::motor::executar_accions_usuari(accions, estat);
        accions.clear();

        bretolesc::motor::pintar(console, context, estat);

        if (estat.vol_ser_reiniciat())
        {
            uint32_t llavor = std::random_device{}();
            printf("Generant amb llavor 0x%x\n", llavor);
            generador.establir_llavor(llavor);

            estat.reinicia(generador);
        }

        entrada_sdl::ProcessarEvents(context, estat.obtenir_mode_entrada(), accions);
    }

    return 0;
}
