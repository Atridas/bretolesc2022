module;

// std
#include <variant>

export module EstructuresAccions;

import Comú;
import Entitats;

export namespace bretolesc
{

	namespace acció
	{
		struct Finalitzar {};

		struct NoFerRes {};

		struct Reiniciar {};

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
		struct AtacCosACos
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
		acció::NoFerRes,
		acció::MoureJugador,
		acció::MoureEntitat,
		acció::AtacCosACos,
		acció::BatzegadaJugador,
		acció::Finalitzar,
		acció::Reiniciar
	>;
}
