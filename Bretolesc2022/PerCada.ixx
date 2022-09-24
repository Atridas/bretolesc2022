module;

// std
#include <cassert>

#include <tuple>

export module Col·leccióComponents:PerCada;

import :Col·lecció;

export namespace bretolesc
{
	template<typename ComponentA, typename ComponentB>
	auto per_cada(Col·lecció<ComponentA> const& col·leccióA, Col·lecció<ComponentB> const& col·leccióB)
	{
		class ConstIterator
		{
		public:
			ConstIterator(
				Col·lecció<ComponentA>::ConstIterator const& _itA,
				Col·lecció<ComponentB>::ConstIterator const& _itB)
				: itA(_itA)
				, itB(_itB)
			{
				sincronitzar();
			}

			std::tuple<IdEntitat, ComponentA const&, ComponentB const&> operator*() const
			{
				assert(!itA.és_al_final());
				assert(!itB.és_al_final());
				assert(std::get<IdEntitat>(*itA) == std::get<IdEntitat>(*itB));

				return { std::get<IdEntitat>(*itA), std::get<ComponentA const&>(*itA), std::get<ComponentB const&>(*itB) };
			}

			void operator++()
			{
				assert(!itA.és_al_final());
				assert(!itB.és_al_final());

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

			bool operator==(GuargiaFinalDeCol·lecció const&) const
			{
				return és_al_final();
			}

			bool operator!=(GuargiaFinalDeCol·lecció const&)
			{
				return !és_al_final();
			}

			bool és_al_final() const
			{
				assert(itA.és_al_final() == itB.és_al_final());

				return itA.és_al_final();
			}

		private:
			typename Col·lecció<ComponentA>::ConstIterator itA;
			typename Col·lecció<ComponentB>::ConstIterator itB;

			void sincronitzar()
			{
				while (!itA.és_al_final() && !itB.és_al_final())
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
			Col·lecció<ComponentA> const& col·leccióA;
			Col·lecció<ComponentB> const& col·leccióB;

			ConstIterator begin() const { return ConstIterator(col·leccióA.begin(), col·leccióB.begin()); }
			GuargiaFinalDeCol·lecció end() const { return {}; }
		};

		return Iterable{ col·leccióA, col·leccióB };
	}


	template<typename F, typename ComponentA, typename ComponentB> requires std::invocable<F, IdEntitat, ComponentA, ComponentB>
	void per_cada(F const& funció, Col·lecció<ComponentA> const& col·leccióA, Col·lecció<ComponentB> const& col·leccióB)
	{
		auto itA = col·leccióA.begin();
		auto itB = col·leccióB.begin();

		auto endA = col·leccióA.end();
		auto endB = col·leccióB.end();

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
				funció(idA, std::get<ComponentA const&>(*itA), std::get<ComponentB const&>(*itB));
				++itA;
				++itB;
			}
		}
	}
}