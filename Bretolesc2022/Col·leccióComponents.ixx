module;

// std
#include <cassert>

#include <algorithm>
#include <concepts>
#include <optional>
#include <tuple>
#include <type_traits>
#include <vector>

export module Entitats:Col·leccióComponents;

import :Components;
import :IdEntitat;

namespace bretolesc
{
	struct GuargiaFinalDeCol·lecció {};
}

export namespace bretolesc
{
	template<typename Component>
	class Col·lecció
	{
	public:

		void reinicia()
		{
			ids.clear();
			components.clear();
		}

		void afegir(IdEntitat id, Component const& component)
		{
			assert(ids.size() == components.size());
			assert(!std::binary_search(ids.begin(), ids.end(), id));

			// inserir id ordenadament
			auto it = ids.insert(std::upper_bound(ids.begin(), ids.end(), id), id);
			// agafa la posició on s'ha inserit
			size_t pos = it - ids.begin();

			// inserir el component a la mateixa posició
			components.insert(components.begin() + pos, component);
		}

		void treure(IdEntitat id)
		{
			auto it = std::lower_bound(ids.begin(), ids.end(), id);
			assert(it != ids.end() && *it == id);

			size_t idx = it - ids.begin();

			ids.erase(it);
			components.erase(components.begin() + idx);
		}

		std::optional<Component> treure_si_hi_és(IdEntitat id)
		{
			auto it = std::lower_bound(ids.begin(), ids.end(), id);
			if (it != ids.end() && *it == id)
			{
				size_t idx = it - ids.begin();

				Component resultat = components[idx];

				ids.erase(it);
				components.erase(components.begin() + idx);
				return resultat;
			}
			else
			{
				return {};
			}
		}

		Component& obtenir(IdEntitat id)
		{
			assert(ids.size() == components.size());
			auto it = std::find(ids.begin(), ids.end(), id);

			assert(it != ids.end());
			size_t pos = it - ids.begin();

			return components[pos];
		}

		Component const& obtenir(IdEntitat id) const
		{
			assert(ids.size() == components.size());
			auto it = std::find(ids.begin(), ids.end(), id);

			assert(it != ids.end());
			size_t pos = it - ids.begin();

			return components[pos];
		}

		std::optional<std::reference_wrapper<Component>> potser_obtenir(IdEntitat id)
		{
			assert(ids.size() == components.size());
			auto it = std::find(ids.begin(), ids.end(), id);

			if (it != ids.end())
			{
				size_t pos = it - ids.begin();

				return components[pos];
			}
			else
			{
				return std::nullopt;
			}
		}

		std::optional<std::reference_wrapper<Component const>> potser_obtenir(IdEntitat id) const
		{
			assert(ids.size() == components.size());
			auto it = std::find(ids.begin(), ids.end(), id);

			if (it != ids.end())
			{
				size_t pos = it - ids.begin();

				return components[pos];
			}
			else
			{
				return std::nullopt;
			}
		}

		// PERFER funció amb 1 o 2 arguments
		template<typename F> requires std::invocable<F, IdEntitat, Component>
		void per_cada(F const& funció)
		{
			assert(ids.size() == components.size());
			
			for (int i = 0; i < ids.size(); ++i)
			{
				funció(ids[i], components[i]);
			}
		}

		template<typename F> requires std::invocable<F, IdEntitat, Component>
		void per_cada(F const& funció) const
		{
			assert(ids.size() == components.size());

			for (int i = 0; i < ids.size(); ++i)
			{
				funció(ids[i], components[i]);
			}
		}

		class Iterator
		{
		public:
			Iterator(Col·lecció<Component>& _col·lecció, size_t _idx) : col·lecció(_col·lecció), idx(_idx) {}

			std::tuple<IdEntitat, Component&> operator*()
			{
				assert(idx < col·lecció.ids.size());
				return { col·lecció.ids[idx], col·lecció.components[idx] };
			}

			void operator++()
			{
				assert(idx < col·lecció.ids.size());
				++idx;
			}

			bool operator==(Iterator const& it) const
			{
				return &col·lecció == &it.col·lecció && idx == it.idx;
			}

			bool operator!=(Iterator const& it) const
			{
				return &col·lecció != &it.col·lecció || idx != it.idx;
			}

			bool operator==(GuargiaFinalDeCol·lecció const&) const
			{
				return és_al_final();
			}

			bool operator!=(GuargiaFinalDeCol·lecció const&) const
			{
				return !és_al_final();
			}

			bool és_al_final() const
			{
				assert(idx <= col·lecció.ids.size());
				return idx == col·lecció.ids.size();
			}

			void moure_al_final()
			{
				idx = col·lecció.ids.size();
			}

		private:
			Col·lecció<Component>& col·lecció;
			size_t idx;
		};

		class ConstIterator
		{
		public:
			ConstIterator(Col·lecció<Component> const& _col·lecció, size_t _idx) : col·lecció(_col·lecció), idx(_idx) {}

			std::tuple<IdEntitat, Component const&> operator*() const
			{
				assert(idx < col·lecció.ids.size());
				return { col·lecció.ids[idx], col·lecció.components[idx] };
			}

			void operator++()
			{
				assert(idx < col·lecció.ids.size());
				++idx;
			}

			bool operator==(ConstIterator const& it) const
			{
				return &col·lecció == &it.col·lecció && idx == it.idx;
			}

			bool operator!=(ConstIterator const& it)
			{
				return &col·lecció != &it.col·lecció || idx != it.idx;
			}

			bool operator==(GuargiaFinalDeCol·lecció const&) const
			{
				return és_al_final();
			}

			bool operator!=(GuargiaFinalDeCol·lecció const&) const
			{
				return !és_al_final();
			}

			bool és_al_final() const
			{
				assert(idx <= col·lecció.ids.size());
				return idx == col·lecció.ids.size();
			}

			void moure_al_final()
			{
				idx = col·lecció.ids.size();
			}

		private:
			Col·lecció<Component> const& col·lecció;
			size_t idx;
		};

		Iterator begin() { return Iterator{ *this, 0 }; }
		GuargiaFinalDeCol·lecció end() { return {}; }
		ConstIterator begin() const { return ConstIterator{ *this, 0 }; }
		GuargiaFinalDeCol·lecció end() const { return {}; }


	private:
		std::vector<IdEntitat> ids;
		std::vector<Component> components;
	};


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

			ConstIterator begin() const { return ConstIterator( col·leccióA.begin(), col·leccióB.begin() ); }
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
				assert (idA == idB);
				funció(idA, std::get<ComponentA const&>(*itA), std::get<ComponentB const&>(*itB));
				++itA;
				++itB;
			}
		}
	}

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

	template<typename... RestaEtiquetes>
	class Etiquetes
	{
	public:
		template<typename E>
		void afegir(IdEntitat id) { assert(false); }

		template<typename E>
		void treure(IdEntitat id) {}

		template<typename F>
		void treure(F const& funció, IdEntitat id) {}

		template<typename E>
		bool treure_si_hi_és(IdEntitat id) { return false; }

		template<typename E>
		bool té(IdEntitat id) const { return false; }

		void reinicia() {}
	};

	template<typename Etiqueta, typename... RestaEtiquetes>
	class Etiquetes<Etiqueta, RestaEtiquetes...>
	{
	public:

		template<typename E>
		void afegir(IdEntitat id)
		{
			if constexpr (std::is_same_v<Etiqueta, E>)
			{
				assert(!std::binary_search(ids.begin(), ids.end(), id));

				// inserir id ordenadament
				auto it = ids.insert(std::upper_bound(ids.begin(), ids.end(), id), id);
			}
			else
			{
				resta_d_etiquetes.afegir<E>(id);
			}
		}

		template<typename E>
		void treure(IdEntitat id)
		{
			if constexpr (std::is_same_v<Etiqueta, E>)
			{
				auto it = std::lower_bound(ids.begin(), ids.end(), id);
				assert(it != ids.end() && *it == id);

				ids.erase(it);
			}
			else
			{
				resta_d_etiquetes.treure<E>(id);
			}
		}

		template<typename F>
		void treure(F const& funció, IdEntitat id)
		{
			if (té<Etiqueta>(id) && funció((Etiqueta*)nullptr))
			{
				treure<Etiqueta>(id);
			}
			resta_d_etiquetes.treure(funció, id);
		}

		template<typename E>
		bool treure_si_hi_és(IdEntitat id)
		{
			if constexpr (std::is_same_v<Etiqueta, E>)
			{
				auto it = std::lower_bound(ids.begin(), ids.end(), id);
				if (it != ids.end() && *it == id)
				{
					size_t idx = it - ids.begin();

					ids.erase(it);
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return resta_d_etiquetes.treure_si_hi_és<E>(id);
			}
		}

		template<typename E>
		bool té(IdEntitat id) const
		{
			if constexpr (std::is_same_v<Etiqueta, E>)
			{
				auto it = std::find(ids.begin(), ids.end(), id);

				return (it != ids.end());
			}
			else
			{
				return resta_d_etiquetes.té<E>(id);
			}
		}

		void reinicia()
		{
			ids.clear();
			resta_d_etiquetes.reinicia();
		}

	private:
		std::vector<IdEntitat> ids;
		Etiquetes<RestaEtiquetes...> resta_d_etiquetes;
	};


}
