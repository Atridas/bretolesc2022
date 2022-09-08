module;

// std
#include <cassert>

export module Col·leccióComponents:Col·leccions;

import :Col·lecció;
import :PerCada;

export namespace bretolesc
{
	template<typename... Components>
	class Col·leccions
	{
		static_assert(sizeof...(Components) == 0);

	public:
		template<typename C>
		Col·lecció<C>& obtenir_col·lecció() { assert(false); return {}; }

		template<typename C>
		Col·lecció<C> const& obtenir_col·lecció() const { assert(false); return {}; }

		void reinicia() {}

		template<typename F>
		void modificar_o_treure(F const& funció, IdEntitat id) {}
	};

	template<typename Component, typename... Components>
	class Col·leccions<Component, Components...>
	{
	public:

		template<typename C>
		Col·lecció<C>& obtenir_col·lecció()
		{
			if constexpr (std::is_same_v<Component, C>)
				return col·lecció;
			else
				return resta_de_col·leccions.obtenir_col·lecció<C>();
		}

		template<typename C>
		Col·lecció<C> const& obtenir_col·lecció() const
		{
			if constexpr (std::is_same_v<Component, C>)
				return col·lecció;
			else
				return resta_de_col·leccions.obtenir_col·lecció<C>();
		}


		template<typename C>
		C& obtenir_component(IdEntitat id)
		{
			return obtenir_col·lecció<C>().obtenir(id);
		}

		template<typename C>
		C const& obtenir_component(IdEntitat id) const
		{
			return obtenir_col·lecció<C>().obtenir(id);
		}

		template<typename C>
		std::optional<std::reference_wrapper<C const>> potser_obtenir_component(IdEntitat id) const
		{
			return obtenir_col·lecció<C>().potser_obtenir(id);
		}

		template<typename C>
		std::optional<std::reference_wrapper<C const>> potser_obtenir_component(IdEntitat id)
		{
			return obtenir_col·lecció<C>().potser_obtenir(id);
		}

		template<typename C>
		void afegir_component(IdEntitat id, C const& component)
		{
			return obtenir_col·lecció<C>().afegir(id, component);
		}

		template<typename C>
		void treure(IdEntitat id)
		{
			obtenir_col·lecció<C>().treure(id);
		}

		template<typename C>
		std::optional<C> treure_si_hi_és(IdEntitat id)
		{
			return obtenir_col·lecció<C>().treure_si_hi_és(id);
		}

		void reinicia()
		{
			col·lecció.reinicia();
			resta_de_col·leccions.reinicia();
		}


		template<typename ComponentA, typename ComponentB>
		auto per_cada() const
		{
			return bretolesc::per_cada(obtenir_col·lecció<ComponentA>(), obtenir_col·lecció<ComponentB>());
		}

		template<typename F>
		void modificar_o_treure(F const& funció, IdEntitat id)
		{
			if (auto component = col·lecció.potser_obtenir(id))
			{
				bool treure = funció(component->get());
				if (treure)
				{
					col·lecció.treure(id);
				}
			}
			resta_de_col·leccions.modificar_o_treure(funció, id);
		}

	private:
		Col·lecció<Component> col·lecció;
		Col·leccions<Components...> resta_de_col·leccions;
	};
}
