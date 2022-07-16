module;

// std
#include <cassert>

#include <algorithm>
#include <vector>

export module Entitats:Col·leccióComponents;

import :Components;
import :IdEntitat;

export namespace bretolesc
{
	template<typename Component>
	class Col·lecció
	{
	public:
		void afegir(IdEntitat id, Component const& component)
		{
			assert(ids.size() == components.size());

			// inserir id ordenadament
			auto it = ids.insert(std::upper_bound(ids.begin(), ids.end(), id), id);
			// agafa la posició on s'ha inserit
			size_t pos = it - ids.begin();

			// inserir el component a la mateixa posició
			components.insert(components.begin() + pos, component);
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

		// PERFER funció amb 1 o 2 arguments
		template<typename F>
		void per_cada(F const& funció)
		{
			assert(ids.size() == components.size());
			
			for (int i = 0; i < ids.size(); ++i)
			{
				funció(ids[i], components[i]);
			}
		}

		template<typename F>
		void per_cada(F const& funció) const
		{
			assert(ids.size() == components.size());

			for (int i = 0; i < ids.size(); ++i)
			{
				funció(ids[i], components[i]);
			}
		}


	private:
		std::vector<IdEntitat> ids;
		std::vector<Component> components;
	};
}
