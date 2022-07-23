export module Motor:Generador;

import Entitats;

export namespace bretolesc
{
	class Generador
	{
	public:
		virtual IdEntitat generar(class Estat& estat) const = 0;
	};
}