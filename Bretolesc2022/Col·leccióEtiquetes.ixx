module;

// std
#include <cassert>

#include <algorithm>

export module Col·leccióComponents:Col·leccióEtiquetes;

import :Col·lecció;
import :PerCada;

export namespace bretolesc
{
	template<typename... RestaEtiquetes>
	class Etiquetes
	{
	public:
		template<typename F>
		void treure(F const& funció, IdEntitat id) {}

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
			if (té<Etiqueta>(id) && funció((Etiqueta const*)nullptr))
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