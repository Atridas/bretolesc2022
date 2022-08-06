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
// 
// 7ª PART
// - Crear fitxer de colors de la ui
//   - player_atk = (0xE0, 0xE0, 0xE0)
//   - enemy_atk = (0xFF, 0xC0, 0xC0)
//   - player_die = (0xFF, 0x30, 0x30)
//   - enemy_die = (0xFF, 0xA0, 0x30)
//   - welcome_text = (0x20, 0xA0, 0xFF)
//   - bar_text = white
//   - bar_filled = (0x0, 0x60, 0x0)
//   - bar_empty = (0x40, 0x10, 0x10)
// - Pintar barra de vida sota el text de vida
//   - console[std::array<int, 2>].bg = color
//   - [0,45] -> [20,45]
// - Crear el registre (log) de missatges
//   - cada missatge conté text, un color i un contador, si el contador és >1, s'afegeix al missatge (L'orc ataca x3)
//   - quan afegim un missatge aquest ha d'indicar si acumula o no
//   - imprimir usant TCOD::get_height_rect i TCOD::print_rect
//     - dibuixem el registre a [21+40, 45+5]
//     - canviem l'alçada del mapa de 45 a 43 per deixar espai al registre
// - Afegim els missatges a la consola
//   - "Aventurer, sigui benvingut a un altre calabós"
//   - els printfs a processar(Estat&, AtacCosACos const&)
//   - el missatge de morir a buscar_morts + canviar el color depenent de si és el jugador
// - Afegir accions que no avancen la lógica
//   - moure el ratolí, per poder saber on és i...
//   - imprimir el nom de les entitats sota el ratolí [21, 44]
//   - Obrir el log de missatges sencer
//     - "v" per obrir-lo
//     - fletxes per moure't, pag up / pag down per moure 10 missatges a la vegada
// 
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
        bretolesc::motor::executar_accions_jugador(accions, estat);
        accions.clear();

        bretolesc::motor::pintar(console, context, estat);

        if (estat.vol_ser_reiniciat())
        {
            uint32_t llavor = std::random_device{}();
            printf("Generant amb llavor 0x%x\n", llavor);
            generador.establir_llavor(llavor);

            estat.reinicia(generador);
        }

        bool jugador_és_viu = estat.jugador_és_viu();

        entrada_sdl::ProcessarEvents(context, jugador_és_viu, accions);
    }

    return 0;
}
