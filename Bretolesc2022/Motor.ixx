module;

// std
#include <span>
#include <variant>

// 3rd party
#include <libtcod.hpp>

export module Motor;

import Accions;
import Com�;
import EstatJoc;
import Mapa;
import ProcessadorAccions;

export namespace bretolesc::motor
{
	void executar_accions(std::span<Acci�> accions, Estat& estat)
	{
        for (Acci� acci� : accions)
        {
            std::visit([&estat](auto& acci�)
                {
                    processar(estat, acci�);
                }, acci�);

            estat.actualitzar_visi�();
            estat.mapa().actualitzar_enemics();
        }

	}

	void pintar(tcod::Console &console, tcod::ContextPtr const& context, Estat const& estat)
	{
        TCOD_console_clear(console.get());

        estat.mapa().pintar(console);

        context->present(console);  // Updates the visible display.
	}
}
