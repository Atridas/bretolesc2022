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
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <libtcod.hpp>




// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// 
// PERFER
// 
// 6ª PART
// ✔️ Crear marc entitat/components/sistemes 
// ✔️ Crear component "lluitador" amb atributs: salut, salut màxima, força i defensa
//   ✔️ jugador { salut: 30, defensa: 2, força: 5 }
//   ✔️ orc { salut: 10, defensa: 0, força: 3 }
//   ✔️ trol { salut: 16, defensa: 1, força: 4 }
// - Crear acció per a "passar" torn (espai?)
// - Crear el component IAHostil amb un TCOD_path_t
// - Crear funció d'enrutar (pathfinding)
//   - Preparar mapa
//     - TCOD_Map* TCOD_map_new(int width, int height);
//     - void TCOD_map_clear(TCOD_Map * map, bool transparent, bool walkable);
//     - TCOD_Error TCOD_map_copy(const TCOD_Map * __restrict source, TCOD_Map * __restrict dest);
//     - void TCOD_map_set_properties(TCOD_Map * map, int x, int y, bool is_transparent, bool is_walkable);
//     - void TCOD_map_delete(TCOD_Map * map);
//     - bool TCOD_map_is_transparent(const TCOD_Map * map, int x, int y);
//     - bool TCOD_map_is_walkable(TCOD_Map * map, int x, int y);
//   - Preparar camins
//     - TCOD_path_t TCOD_path_new_using_map(TCOD_map_t map, float diagonalCost);
//     - bool TCOD_path_compute(TCOD_path_t path, int ox, int oy, int dx, int dy);
//     - bool TCOD_path_walk(TCOD_path_t path, int* x, int* y, bool recalculate_when_needed);
//     - void TCOD_path_delete(TCOD_path_t path);
// - Crear sistema ActualitzarIAsHostils
//   - l'objectiu és el jugador
//   - si el jugador és a distància (manhattan) 1, atacar-lo
//   - si tenim un camí definit, seguir-lo
//   - si el jugador és visible (si nosaltres som visibles des del jugador), crear un camí fins a ell
//   - en cas contrari no fer res
// ✔️ Modificar l'acció de Melee tal que
//   - dany = max(0, força - defensa)
//   ✔️ restar hp, mostrar missatge
// - Crear sistema que busqui morts
//   - entitats amb salut < 0
//   - canvia el char a "%", el color a {191,0,0} i no bloqueja el moviment
//   - imprimeix un missage "ha mort un orc" / "has mort"
// - Canviar les prioritats de render d'entitats: CADÀVER < OBJECTE < ACTOR
// - Imprimim la vida del jugador després de pintar el mapa console.print(x=1 y=47 "Salut:%d/%d")
// - Cobrir el game over
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
    bretolesc::GeneradorProcedural generador{};

    uint32_t llavor = std::random_device{}();
    printf("Generant amb llavor 0x%x\n", llavor);
    generador.establir_llavor(llavor);

    generador.establir_num_habitacions(5, 15);

    bretolesc::Estat estat(map_width, map_height, generador);

    estat.actualitzar_visió();

    std::vector<bretolesc::Acció> accions;

    // ------------------------------------------------------------------------
    // Game loop.
    // ------------------------------------------------------------------------
    while (!estat.vol_ser_tancat())
    {
        bretolesc::motor::executar_accions(accions, estat);
        accions.clear();

        bretolesc::motor::pintar(console, context, estat);

        entrada_sdl::ProcessarEvents(context, accions);
    }

    return 0;
}
