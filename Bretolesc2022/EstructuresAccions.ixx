module;

// std
#include <variant>

export module EstructuresAccions;

import Com�;
import Entitats;

export namespace bretolesc
{

	namespace acci�_usuari
	{
		struct Finalitzar {};

		struct NoFerRes {};

		struct Reiniciar {};

		struct Moure
		{
			Vector2D direcci�;
		};

		// el jugador o b� es mou en una direcci� o b� ataca cos a cos si hi ha alg�
		struct Batzegada
		{
			Vector2D direcci�;
		};

		struct Agafar {};

		struct MoureRatol�
		{
			Punt2D p;
		};

		struct AlternarRegistre {};

		struct Despla�arRegistre
		{
			int l�nies;
		};

		struct AlternarInventari {};

		struct Despla�arInventari
		{
			int l�nies;
		};
	}

	using Acci�Usuari = std::variant<
		acci�_usuari::NoFerRes,
		acci�_usuari::Moure,
		acci�_usuari::Batzegada,
		acci�_usuari::Agafar,
		acci�_usuari::MoureRatol�,
		acci�_usuari::AlternarRegistre,
		acci�_usuari::Despla�arRegistre,
		acci�_usuari::AlternarInventari,
		acci�_usuari::Despla�arInventari,
		acci�_usuari::Finalitzar,
		acci�_usuari::Reiniciar
	>;

	namespace acci�_entitat
	{
		struct Moure
		{
			IdEntitat entitat;
			Vector2D direcci�;
		};

		// entitat ataca a objectiu
		struct AtacCosACos
		{
			IdEntitat entitat, objectiu;
		};
	}

	using Acci�Entitat = std::variant<
		acci�_entitat::Moure,
		acci�_entitat::AtacCosACos
	>;
}
