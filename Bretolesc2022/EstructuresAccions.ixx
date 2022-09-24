module;

// std
#include <variant>

export module EstructuresAccions;

import Comú;
import Entitats;

export namespace bretolesc
{

	namespace acció_usuari
	{
		struct Finalitzar {};

		struct NoFerRes {};

		struct Reiniciar {};

		struct Moure
		{
			Vector2D direcció;
		};

		// el jugador o bé es mou en una direcció o bé ataca cos a cos si hi ha algú
		struct Batzegada
		{
			Vector2D direcció;
		};

		struct Agafar {};

		struct Tirar {};

		struct Consumir {};

		struct MoureRatolí
		{
			Punt2D p;
		};

		struct EstablirCursor
		{
			Punt2D p;
		};

		struct MoureCursor
		{
			Vector2D v;
		};

		struct AcceptarCursor {};
		struct CancelarCursor {};

		struct AlternarRegistre {};

		struct DesplaçarRegistre
		{
			int línies;
		};

		struct AlternarInventari {};

		struct DesplaçarInventari
		{
			int línies;
		};
	}

	using AccióUsuari = std::variant<
		acció_usuari::NoFerRes,
		acció_usuari::Moure,
		acció_usuari::Batzegada,
		acció_usuari::Agafar,
		acció_usuari::Tirar,
		acció_usuari::Consumir,
		acció_usuari::MoureRatolí,
		acció_usuari::EstablirCursor,
		acció_usuari::MoureCursor,
		acció_usuari::AcceptarCursor,
		acció_usuari::CancelarCursor,
		acció_usuari::AlternarRegistre,
		acció_usuari::DesplaçarRegistre,
		acció_usuari::AlternarInventari,
		acció_usuari::DesplaçarInventari,
		acció_usuari::Finalitzar,
		acció_usuari::Reiniciar
	>;

	namespace acció_entitat
	{
		struct Moure
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
		struct Batzegada
		{
			IdEntitat entitat;
			Vector2D direcció;
		};
	}

	using AccióEntitat = std::variant<
		acció_entitat::Moure,
		acció_entitat::AtacCosACos,
		acció_entitat::Batzegada
	>;

	namespace acció
	{
		struct RecuperarVida
		{
			IdEntitat entitat;
			int vida;
		};

		struct ExecutarEncanteriDeDany
		{
			IdEntitat objecte;
			IdEntitat entitat;
			int dany;
		};

		struct ExecutarEncanteriDeConfusió
		{
			IdEntitat entitat;
			int torns;
		};

		struct ActivarObjecte
		{
			IdEntitat objecte;
		};
	}
}
