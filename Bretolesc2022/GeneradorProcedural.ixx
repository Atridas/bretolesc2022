module;

// std
#include <vector>
#include <random>

export module GeneradorProcedural;

import Comú;
import Entitats;
import Motlles;
import Motor;

namespace bretolesc
{
	struct Habitació
	{
		Punt2D v1, v2;

		Habitació(Punt2D origen, Vector2D mida)
			: v1{ origen.x, origen.y }
			, v2{ origen.x + mida.x + 1, origen.y + mida.y + 1 }
		{}

		Punt2D centre() const
		{
			return { (v1.x + v2.x) / 2, (v1.y + v2.y) / 2 };
		}
	};

	struct Passadís
	{
		int h1, h2;
	};

	export class GeneradorProcedural : public Generador
	{
	public:

		void establir_num_habitacions(uint32_t _min_num_habitacions, uint32_t _max_num_habitacions = 0)
		{
			min_num_habitacions = _min_num_habitacions;
			max_num_habitacions = _max_num_habitacions < _min_num_habitacions ? _min_num_habitacions : _max_num_habitacions;
		}

		void establir_num_extra_passadissos(uint32_t _min_num_extra_passadissos, uint32_t _max_num_extra_passadissos = 0)
		{
			min_num_extra_passadissos = _min_num_extra_passadissos;
			max_num_extra_passadissos = _max_num_extra_passadissos < _min_num_extra_passadissos ? _min_num_extra_passadissos : _max_num_extra_passadissos;
		}

		void establir_num_enemics_per_habitació(uint32_t _min_num_enemics_per_habitació, uint32_t _max_num_enemics_per_habitació = 0)
		{
			min_num_enemics_per_habitació = _min_num_enemics_per_habitació;
			max_num_enemics_per_habitació = _max_num_enemics_per_habitació < _min_num_enemics_per_habitació ? _min_num_enemics_per_habitació : _max_num_enemics_per_habitació;
		}

		void establir_llavor(uint32_t _llavor)
		{
			llavor = _llavor;
		}

		IdEntitat generar(Estat& estat) const override
		{
			RNG rng(llavor);

			// crear habitacions i passadissos
			std::vector<Habitació> habitacions;
			std::vector<Passadís> passadissos;

			std::uniform_int_distribution<> amplada_qualsevol(1, estat.mapa().obtenir_amplada() - 4);
			std::uniform_int_distribution<> alçada_qualsevol(1, estat.mapa().obtenir_alçada() - 4);

			std::uniform_int_distribution<> tamany(3, 15);

			int num_habitacions = std::uniform_int_distribution<>(min_num_habitacions, max_num_habitacions)(rng);
			int num_extra_passadissos = std::uniform_int_distribution<>(min_num_extra_passadissos, max_num_extra_passadissos)(rng);

			int intents = 0;
			for(int i = 0; i < num_habitacions; ++i)
			{
				int x = amplada_qualsevol(rng);
				int y = alçada_qualsevol(rng);

				int w = tamany(rng);
				int h = tamany(rng);

				Habitació habitació({ x, y }, { w, h });

				bool solapada = false;
				for (Habitació const& altra_habitació : habitacions)
				{
					if (
						habitació.v2.x >= altra_habitació.v1.x &&
						altra_habitació.v2.x >= habitació.v1.x &&
						habitació.v2.y >= altra_habitació.v1.y &&
						altra_habitació.v2.y >= habitació.v1.y)
					{
						solapada = true;
						break;
					}
				}

				if (!solapada && habitació.v2.x < estat.mapa().obtenir_amplada() - 1 && habitació.v2.y < estat.mapa().obtenir_alçada() - 1)
				{
					habitacions.push_back(habitació);
				}
				else if(intents < 100)
				{
					++intents;
					--i;
				}
			}

			num_habitacions = (int)habitacions.size();

			for (int i = 1; i < num_habitacions; ++i)
				passadissos.emplace_back( i - 1, i );

			for (int i = 0; i < num_extra_passadissos; ++i)
			{
				int h1 = std::uniform_int_distribution<>(0, num_habitacions - 1)(rng);
				int h2 = std::uniform_int_distribution<>(0, num_habitacions - 1)(rng);

				passadissos.emplace_back(h1, h2);
			}

			// inicialitzar tot a parets
			for (int y = 0; y < estat.mapa().obtenir_alçada(); ++y)
				for (int x = 0; x < estat.mapa().obtenir_amplada(); ++x)
				{
					estat.mapa().establir_rajola({ x, y }, TipusRajola::Paret);
				}

			// pintar habitacions
			for (Habitació habitació : habitacions)
			{
				for (int y = habitació.v1.y; y <= habitació.v2.y; ++y)
					for (int x = habitació.v1.x; x <= habitació.v2.x; ++x)
					{
						estat.mapa().establir_rajola({ x, y }, TipusRajola::Terra);
					}
			}

			// pintar passadissos
			for (Passadís passadís : passadissos)
			{
				Habitació const &h1 = habitacions[passadís.h1];
				Habitació const &h2 = habitacions[passadís.h2];

				Punt2D const origen = h1.centre();
				Punt2D const destí = h2.centre();

				// passadís horitzontal
				int const ox = origen.x < destí.x ? origen.x : destí.x;
				int const dx = origen.x < destí.x ? destí.x : origen.x;

				for (int x = ox; x <= dx; ++x)
				{
					estat.mapa().establir_rajola({ x, origen.y }, TipusRajola::Terra);
				}

				// passadís vertical
				int oy = origen.y < destí.y ? origen.y : destí.y;
				int dy = origen.y < destí.y ? destí.y : origen.y;

				for (int y = oy; y <= dy; ++y)
				{
					estat.mapa().establir_rajola({destí.x, y}, TipusRajola::Terra);
				}
			}

			// enemics
			for (int h = 1; h < habitacions.size(); ++h)
			{
				int num_enemics = std::uniform_int_distribution<>(min_num_enemics_per_habitació, max_num_enemics_per_habitació)(rng);

				for (int e = 0; e < num_enemics; ++e)
				{
					float tipus_enemic_rnd = std::uniform_real_distribution<float>(0.0f, 1.0f)(rng);

					TipusEntitat tipus_enemic;
					if (tipus_enemic_rnd < 0.8f)
					{
						tipus_enemic = TipusEntitat::Orc;
					}
					else
					{
						tipus_enemic = TipusEntitat::Trol;
					}

					Punt2D posició_enemic;

					posició_enemic.x = std::uniform_int_distribution<>(habitacions[h].v1.x + 2, habitacions[h].v2.x - 2)(rng);
					posició_enemic.y = std::uniform_int_distribution<>(habitacions[h].v1.y + 2, habitacions[h].v2.y - 2)(rng);

					if (!estat.buscar_entitat(posició_enemic))
					{
						afegir_entitat(estat, tipus_enemic, posició_enemic);
					}
					else if (intents < 100)
					{
						++intents;
						--e;
					}
				}

			}

			// inici del jugador
			Punt2D posició_jugador = habitacions[0].centre();
			return afegir_entitat(estat, TipusEntitat::Jugador, posició_jugador);
		}

	private:
		uint32_t min_num_habitacions = 3;
		uint32_t max_num_habitacions = 7;

		uint32_t min_num_extra_passadissos = 0;
		uint32_t max_num_extra_passadissos = 3;

		uint32_t min_num_enemics_per_habitació = 0;
		uint32_t max_num_enemics_per_habitació = 2;

		uint32_t llavor = 0;
	};
}
