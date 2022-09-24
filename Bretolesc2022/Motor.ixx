module;

// std
#include <span>
#include <variant>

// 3rd party
#include <libtcod.hpp>

export module Motor;

export import :Estat;
export import :Generador;
export import :GeneradorDExemple;
export import :Mapa;
export import :ProcessadorsAccions;

import EstructuresAccions;
import Comú;


export namespace bretolesc::motor
{
	void executar_accions_usuari(std::span<AccióUsuari> accions, Estat& estat)
	{
        for (AccióUsuari acció : accions)
        {
            std::visit([&estat](auto& acció)
                {
                    processar(estat, acció);
                }, acció);

            estat.actualitzar_visió();
        }
	}

	void pintar(tcod::Console &console, tcod::ContextPtr const& context, Estat const& estat)
	{
        TCOD_console_clear(console.get());

        estat.pintar(console);

        context->present(console);  // Updates the visible display.
	}
}
