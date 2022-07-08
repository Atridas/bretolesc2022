// imports
import Accions;
import Comú;
import EntradaSDL;
import EstatJoc;
import GeneradorProcedural;
import Mapa;
import Motor;
import ProcessadorAccions;

// std
#include <optional>
#include <random>
#include <variant>
#include <vector>

// 3rd party
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <libtcod.hpp>



// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// 
// TODO
// 
// 4ª PART
// - fov
//   - crear la mortalla (shroud) - zona coberta amb la boira de guerra (' ', blanc, negre)
//   - crear gràfic_a_la_vista -> terra (blanc, {200, 180, 50}), paret (blanc, {130, 110, 50})
//   - afegir array de rajoles visibles i rajoles explorades. Una rajola visible es pinta amb 
//         el seu gràfic_a_la_vista, una explorada amb el grafic_fosc i si cap de les dues amb
//         la mortalla
//   - només pintar les entitats en caselles visibles
//   - fer l'algoritme d'actualitzar la visibilitat i cridar-lo
//     - a l'inicialitzar
//     - al moure el jugador
// 
// 
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------




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
    bretolesc::GeneradorDeMapaProcedural generador{};

    generador.establir_llavor(std::random_device{}());

    generador.establir_num_habitacions(5, 15);

    bretolesc::Estat estat = { bretolesc::Mapa(map_width, map_height, generador) };
    {
        bretolesc::Entitat jugador = {};
        jugador.x = estat.mapa.obtenir_orígen_jugador_x();
        jugador.y = estat.mapa.obtenir_orígen_jugador_y();
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

        bretolesc::motor::pintar(console, context, estat);

        entrada_sdl::ProcessarEvents(context, accions);
    }

    return 0;
}
