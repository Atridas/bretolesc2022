module;

// std
#include <vector>
#include <random>

export module GeneradorProcedural;

import Comú;
import Mapa;
import NoiseRNG;

namespace bretolesc
{
	struct Habitació
	{
		int x1, y1, x2, y2;

		Habitació(int x, int y, int w, int h)
			: x1(x)
			, y1(y)
			, x2(x + w + 1)
			, y2(y + h + 1)
		{}

		int centre_x() const
		{
			return (x1 + x2) / 2;
		}

		int centre_y() const
		{
			return (y1 + y2) / 2;
		}
	};

	struct Passadís
	{
		int h1, h2;
	};

	export class GeneradorDeMapaProcedural : public GeneradorDeMapa
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

		void establir_llavor(uint32_t _llavor)
		{
			llavor = _llavor;
		}

		void generar(Mapa& mapa) const override
		{
			RNG rng(llavor);

			// crear habitacions i passadissos
			std::vector<Habitació> habitacions;
			std::vector<Passadís> passadissos;

			std::uniform_int_distribution<> amplada_qualsevol(0, mapa.obtenir_amplada());
			std::uniform_int_distribution<> alçada_qualsevol(0, mapa.obtenir_alçada());

			std::uniform_int_distribution<> tamany(3, 15);

			int num_habitacions = std::uniform_int_distribution<>(min_num_habitacions, max_num_habitacions)(rng);
			int num_extra_passadissos = std::uniform_int_distribution<>(min_num_extra_passadissos, max_num_extra_passadissos)(rng);

			int repeats = 0;
			for(int i = 0; i < num_habitacions; ++i)
			{
				int x = amplada_qualsevol(rng);
				int y = alçada_qualsevol(rng);

				int w = tamany(rng);
				int h = tamany(rng);

				Habitació habitació(x, y, w, h);

				bool solapada = false;
				for (Habitació const& altra_habitació : habitacions)
				{
					if (
						habitació.x2 >= altra_habitació.x1 &&
						altra_habitació.x2 >= habitació.x1 &&
						habitació.y2 >= altra_habitació.y1 &&
						altra_habitació.y2 >= habitació.y1)
					{
						solapada = true;
						break;
					}
				}

				if (!solapada && mapa.és_dins_del_límit(habitació.x2, habitació.y2))
				{
					habitacions.push_back(habitació);
				}
				else if(repeats < 100)
				{
					++repeats;
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
			for (int y = 0; y < mapa.obtenir_alçada(); ++y)
				for (int x = 0; x < mapa.obtenir_amplada(); ++x)
				{
					mapa.establir_rajola(x, y, TipusRajola::Paret);
				}

			// pintar habitacions
			for (Habitació habitació : habitacions)
			{
				for (int y = habitació.y1 + 1; y < habitació.y2; ++y)
					for (int x = habitació.x1; x < habitació.x2; ++x)
					{
						mapa.establir_rajola(x, y, TipusRajola::Terra);
					}
			}

			// pintar passadissos
			for (Passadís passadís : passadissos)
			{
				Habitació const &h1 = habitacions[passadís.h1];
				Habitació const &h2 = habitacions[passadís.h2];

				int const origen_x = h1.centre_x();
				int const destí_x = h2.centre_x();
				
				int const origen_y = h1.centre_y();
				int const destí_y = h2.centre_y();

				// passadís horitzontal
				int const ox = origen_x < destí_x ? origen_x : destí_x;
				int const dx = origen_x < destí_x ? destí_x : origen_x;

				for (int x = ox; x <= dx; ++x)
				{
					mapa.establir_rajola(x, origen_y, TipusRajola::Terra);
				}

				// passadís vertical
				int oy = origen_y < destí_y ? origen_y : destí_y;
				int dy = origen_y < destí_y ? destí_y : origen_y;

				for (int y = oy; y <= dy; ++y)
				{
					mapa.establir_rajola(destí_x, y, TipusRajola::Terra);
				}
			}

			// inici del jugador
			mapa.establir_orígen_jugador(habitacions[0].centre_x(), habitacions[0].centre_y());
		}

	private:
		uint32_t min_num_habitacions = 3;
		uint32_t max_num_habitacions = 7;

		uint32_t min_num_extra_passadissos = 0;
		uint32_t max_num_extra_passadissos = 3;

		uint32_t llavor = 0;
	};
}
