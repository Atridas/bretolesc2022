module;

// std
#include <variant>

export module EstructuresAccions;

import Com�;
import Entitats;

export namespace bretolesc
{

	namespace acci�
	{
		struct Finalitzar {};

		struct NoFerRes {};

		struct Reiniciar {};

		struct MoureJugador
		{
			Vector2D direcci�;
		};

		struct MoureEntitat
		{
			IdEntitat entitat;
			Vector2D direcci�;
		};

		// entitat ataca a objectiu
		struct AtacCosACos
		{
			IdEntitat entitat, objectiu;
		};


		// el jugador o b� es mou en una direcci� o b� ataca cos a cos si hi ha alg�
		struct BatzegadaJugador
		{
			Vector2D direcci�;
		};
	}

	using Acci� = std::variant<
		acci�::NoFerRes,
		acci�::MoureJugador,
		acci�::MoureEntitat,
		acci�::AtacCosACos,
		acci�::BatzegadaJugador,
		acci�::Finalitzar,
		acci�::Reiniciar
	>;
}
