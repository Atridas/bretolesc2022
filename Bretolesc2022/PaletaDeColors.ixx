export module Motor:PaletaDeColors;

import Com�;

export namespace bretolesc::iu
{
	struct Paleta
	{
		static Color const AtacJugador;
		static Color const AtacEnemic;

		static Color const MortJugador;
		static Color const MortEnemic;

		static Color const TextBenvinguda;

		static Color const TextBarraVida;
		static Color const BarraVidaSencera;
		static Color const BarraVidaBuida;

		static Color const FonsRegistre;
		static Color const TextInfoRatol�Cad�ver;
		static Color const TextInfoRatol�Objecte;
		static Color const TextInfoRatol�Actor;
	};
}

bretolesc::Color const bretolesc::iu::Paleta::AtacJugador = Color{ 0xE0, 0xE0, 0xE0 };
bretolesc::Color const bretolesc::iu::Paleta::AtacEnemic = Color{ 0xFF, 0xC0, 0xC0 };

bretolesc::Color const bretolesc::iu::Paleta::MortJugador = Color{ 0xFF, 0x30, 0x30 };
bretolesc::Color const bretolesc::iu::Paleta::MortEnemic = Color{ 0xFF, 0xA0, 0x30 };

bretolesc::Color const bretolesc::iu::Paleta::TextBenvinguda = Color{ 0x20, 0xA0, 0xFF };

bretolesc::Color const bretolesc::iu::Paleta::TextBarraVida = Color{ 0xFF, 0xFF, 0xFF };
bretolesc::Color const bretolesc::iu::Paleta::BarraVidaSencera = Color{ 0x00, 0x60, 0x00 };
bretolesc::Color const bretolesc::iu::Paleta::BarraVidaBuida = Color{ 0x40, 0x10, 0x10 };

bretolesc::Color const bretolesc::iu::Paleta::FonsRegistre = Color{ 0x33, 0x33, 0x33 };
bretolesc::Color const bretolesc::iu::Paleta::TextInfoRatol�Cad�ver = Color{ 0xDD, 0x99, 0x99 };
bretolesc::Color const bretolesc::iu::Paleta::TextInfoRatol�Objecte = Color{ 0x99, 0x99, 0xDD };
bretolesc::Color const bretolesc::iu::Paleta::TextInfoRatol�Actor = Color{ 0xA0, 0xFF, 0xFF };
