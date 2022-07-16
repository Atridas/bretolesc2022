export module Motor:Generador;

export namespace bretolesc
{
	class Generador
	{
	public:
		virtual void generar(class Estat& estat) const = 0;
	};
}