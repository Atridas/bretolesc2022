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
import Com�;


export namespace bretolesc::motor
{
	void executar_accions_jugador(std::span<Acci�> accions, Estat& estat)
	{
        for (Acci� acci� : accions)
        {
            std::visit([&estat](auto& acci�)
                {
                    processar(estat, acci�);
                }, acci�);

            estat.actualitzar_visi�();

            if (!estat.vol_ser_tancat() && !estat.vol_ser_reiniciat())
            {
                estat.actualitzar_ias_hostils();
                estat.buscar_morts();
            }
        }
	}

	void pintar(tcod::Console &console, tcod::ContextPtr const& context, Estat const& estat)
	{
        TCOD_console_clear(console.get());

        estat.pintar(console);

        context->present(console);  // Updates the visible display.
	}
}
