module;

// std
#include <vector>
#include <random>

export module GeneradorProcedural;

import Com�;
import Mapa;
import NoiseRNG;

namespace bretolesc
{
	struct Habitaci�
	{
		Punt2D v1, v2;

		Habitaci�(Punt2D or�gen, Vector2D mida)
			: v1{ or�gen.x, or�gen.y }
			, v2{ or�gen.x + mida.x + 1, or�gen.y + mida.y + 1 }
		{}

		Punt2D centre() const
		{
			return { (v1.x + v2.x) / 2, (v1.y + v2.y) / 2 };
		}
	};

	struct Passad�s
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
			std::vector<Habitaci�> habitacions;
			std::vector<Passad�s> passadissos;

			std::uniform_int_distribution<> amplada_qualsevol(0, mapa.obtenir_amplada());
			std::uniform_int_distribution<> al�ada_qualsevol(0, mapa.obtenir_al�ada());

			std::uniform_int_distribution<> tamany(3, 15);

			int num_habitacions = std::uniform_int_distribution<>(min_num_habitacions, max_num_habitacions)(rng);
			int num_extra_passadissos = std::uniform_int_distribution<>(min_num_extra_passadissos, max_num_extra_passadissos)(rng);

			int repeats = 0;
			for(int i = 0; i < num_habitacions; ++i)
			{
				int x = amplada_qualsevol(rng);
				int y = al�ada_qualsevol(rng);

				int w = tamany(rng);
				int h = tamany(rng);

				Habitaci� habitaci�({ x, y }, { w, h });

				bool solapada = false;
				for (Habitaci� const& altra_habitaci� : habitacions)
				{
					if (
						habitaci�.v2.x >= altra_habitaci�.v1.x &&
						altra_habitaci�.v2.x >= habitaci�.v1.x &&
						habitaci�.v2.y >= altra_habitaci�.v1.y &&
						altra_habitaci�.v2.y >= habitaci�.v1.y)
					{
						solapada = true;
						break;
					}
				}

				if (!solapada && mapa.�s_dins_del_l�mit(habitaci�.v2))
				{
					habitacions.push_back(habitaci�);
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
			for (int y = 0; y < mapa.obtenir_al�ada(); ++y)
				for (int x = 0; x < mapa.obtenir_amplada(); ++x)
				{
					mapa.establir_rajola({ x, y }, TipusRajola::Paret);
				}

			// pintar habitacions
			for (Habitaci� habitaci� : habitacions)
			{
				for (int y = habitaci�.v1.y + 1; y < habitaci�.v2.y; ++y)
					for (int x = habitaci�.v1.x; x < habitaci�.v2.x; ++x)
					{
						mapa.establir_rajola({ x, y }, TipusRajola::Terra);
					}
			}

			// pintar passadissos
			for (Passad�s passad�s : passadissos)
			{
				Habitaci� const &h1 = habitacions[passad�s.h1];
				Habitaci� const &h2 = habitacions[passad�s.h2];

				Punt2D const origen = h1.centre();
				Punt2D const dest� = h2.centre();

				// passad�s horitzontal
				int const ox = origen.x < dest�.x ? origen.x : dest�.x;
				int const dx = origen.x < dest�.x ? dest�.x : origen.x;

				for (int x = ox; x <= dx; ++x)
				{
					mapa.establir_rajola({ x, origen.y }, TipusRajola::Terra);
				}

				// passad�s vertical
				int oy = origen.y < dest�.y ? origen.y : dest�.y;
				int dy = origen.y < dest�.y ? dest�.y : origen.y;

				for (int y = oy; y <= dy; ++y)
				{
					mapa.establir_rajola({ dest�.x, y }, TipusRajola::Terra);
				}
			}

			// inici del jugador
			mapa.establir_or�gen_jugador(habitacions[0].centre());
		}

	private:
		uint32_t min_num_habitacions = 3;
		uint32_t max_num_habitacions = 7;

		uint32_t min_num_extra_passadissos = 0;
		uint32_t max_num_extra_passadissos = 3;

		uint32_t llavor = 0;
	};
}
