module;

// std
#include <cassert>

#include <tuple>

export module Col�lecci�Components:PerCada;

import :Col�lecci�;

export namespace bretolesc
{
	template<typename ComponentA, typename ComponentB>
	auto per_cada(Col�lecci�<ComponentA> const& col�lecci�A, Col�lecci�<ComponentB> const& col�lecci�B)
	{
		class ConstIterator
		{
		public:
			ConstIterator(
				Col�lecci�<ComponentA>::ConstIterator const& _itA,
				Col�lecci�<ComponentB>::ConstIterator const& _itB)
				: itA(_itA)
				, itB(_itB)
			{
				sincronitzar();
			}

			std::tuple<IdEntitat, ComponentA const&, ComponentB const&> operator*() const
			{
				assert(!itA.�s_al_final());
				assert(!itB.�s_al_final());
				assert(std::get<IdEntitat>(*itA) == std::get<IdEntitat>(*itB));

				return { std::get<IdEntitat>(*itA), std::get<ComponentA const&>(*itA), std::get<ComponentB const&>(*itB) };
			}

			void operator++()
			{
				assert(!itA.�s_al_final());
				assert(!itB.�s_al_final());

				++itA;
				++itB;

				sincronitzar();
			}

			bool operator==(ConstIterator const& it) const
			{
				return itA == it.itA && itB == it.itB;
			}

			bool operator!=(ConstIterator const& it)
			{
				return itA != it.itA || itB != it.itB;
			}

			bool operator==(GuargiaFinalDeCol�lecci� const&) const
			{
				return �s_al_final();
			}

			bool operator!=(GuargiaFinalDeCol�lecci� const&)
			{
				return !�s_al_final();
			}

			bool �s_al_final() const
			{
				assert(itA.�s_al_final() == itB.�s_al_final());

				return itA.�s_al_final();
			}

		private:
			typename Col�lecci�<ComponentA>::ConstIterator itA;
			typename Col�lecci�<ComponentB>::ConstIterator itB;

			void sincronitzar()
			{
				while (!itA.�s_al_final() && !itB.�s_al_final())
				{
					IdEntitat idA = std::get<IdEntitat>(*itA);
					IdEntitat idB = std::get<IdEntitat>(*itB);

					if (idA < idB)
					{
						++itA;
					}
					else if (idB < idA)
					{
						++itB;
					}
					else
					{
						assert(idA == idB);
						return;
					}
				}

				itA.moure_al_final();
				itB.moure_al_final();
			}
		};

		struct Iterable
		{
			Col�lecci�<ComponentA> const& col�lecci�A;
			Col�lecci�<ComponentB> const& col�lecci�B;

			ConstIterator begin() const { return ConstIterator(col�lecci�A.begin(), col�lecci�B.begin()); }
			GuargiaFinalDeCol�lecci� end() const { return {}; }
		};

		return Iterable{ col�lecci�A, col�lecci�B };
	}


	template<typename F, typename ComponentA, typename ComponentB> requires std::invocable<F, IdEntitat, ComponentA, ComponentB>
	void per_cada(F const& funci�, Col�lecci�<ComponentA> const& col�lecci�A, Col�lecci�<ComponentB> const& col�lecci�B)
	{
		auto itA = col�lecci�A.begin();
		auto itB = col�lecci�B.begin();

		auto endA = col�lecci�A.end();
		auto endB = col�lecci�B.end();

		while (itA != endA && itB != endB)
		{
			IdEntitat idA = std::get<IdEntitat>(*itA);
			IdEntitat idB = std::get<IdEntitat>(*itB);

			if (idA < idB)
			{
				++itA;
			}
			else if (idB < idA)
			{
				++itB;
			}
			else
			{
				assert(idA == idB);
				funci�(idA, std::get<ComponentA const&>(*itA), std::get<ComponentB const&>(*itB));
				++itA;
				++itB;
			}
		}
	}
}