module;

// std
#include <variant>

export module Accions;

import Comú;
import Entitats;
import EstatJoc;

export namespace bretolesc
{

	namespace acció
	{
		struct Finalitzar {};

		struct MoureJugador
		{
			Vector2D direcció;
		};

		struct MoureEntitat
		{
			IdEntitat entitat;
			Vector2D direcció;
		};

		// entitat ataca a objectiu
		struct AccióCosACos
		{
			IdEntitat entitat, objectiu;
		};

		// el jugador o bé es mou en una direcció o bé ataca cos a cos si hi ha algú
		struct BatzegadaJugador
		{
			Vector2D direcció;
		};
	}

	using Acció = std::variant<
		acció::Finalitzar,
		acció::MoureJugador,
		acció::MoureEntitat,
		acció::AccióCosACos,
		acció::BatzegadaJugador
	>;
}
