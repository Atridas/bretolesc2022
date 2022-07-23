export module Motor:GeneradorDExemple;

import :Estat;
import :Generador;

export namespace bretolesc
{
	class GeneradorDExemple : public Generador
	{
	public:
		IdEntitat generar(Estat& estat) const override
		{
			for (int y = 0; y < estat.mapa().obtenir_alçada(); ++y)
				for (int x = 0; x < estat.mapa().obtenir_amplada(); ++x)
				{
					estat.mapa().establir_rajola({ x, y }, TipusRajola::Terra);
				}

			estat.mapa().establir_rajola({ 30, 22 }, TipusRajola::Paret);
			estat.mapa().establir_rajola({ 31, 22 }, TipusRajola::Paret);
			estat.mapa().establir_rajola({ 32, 22 }, TipusRajola::Paret);
			estat.mapa().establir_rajola({ 33, 22 }, TipusRajola::Paret);

			// PERFER
			return estat.crear_entitat();
		}
	};
}