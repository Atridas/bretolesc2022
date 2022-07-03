module;

// std
#include <span>
#include <variant>

// 3rd party
#include <libtcod.hpp>

export module Motor;

import Accions;
import Comú;
import EstatJoc;
import Mapa;
import ProcessadorAccions;

export namespace bretolesc::motor
{
	void executar_accions(std::span<Acció> accions, Estat& estat)
	{
        for (Acció acció : accions)
        {
            std::visit([&estat](auto& acció)
                {
                    processar(estat, acció);
                }, acció);
        }
	}

	void pintar(tcod::Console &console, tcod::ContextPtr const& context, Estat const& estat)
	{
        TCOD_console_clear(console.get());

        estat.mapa.pintar(console);

        for (bretolesc::Entitat const& entitat : estat.entitats)
        {
            char const txt[2] = { entitat.caracter , '\0' };

            tcod::print(
                console,
                { entitat.x, entitat.y },
                txt,
                TCOD_ColorRGB{ entitat.color.r, entitat.color.g, entitat.color.b },
                std::nullopt);
        }
        context->present(console);  // Updates the visible display.
	}
}
