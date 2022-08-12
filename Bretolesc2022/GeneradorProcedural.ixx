module;

// std
#include <vector>
#include <random>

export module GeneradorProcedural;

import Com�;
import Entitats;
import Motlles;
import Motor;

namespace bretolesc
{
	struct Habitaci�
	{
		Punt2D v1, v2;

		Habitaci�(Punt2D origen, Vector2D mida)
			: v1{ origen.x, origen.y }
			, v2{ origen.x + mida.x + 1, origen.y + mida.y + 1 }
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

		void establir_num_enemics_per_habitaci�(uint32_t _min_num_enemics_per_habitaci�, uint32_t _max_num_enemics_per_habitaci� = 0)
		{
			min_num_enemics_per_habitaci� = _min_num_enemics_per_habitaci�;
			max_num_enemics_per_habitaci� = _max_num_enemics_per_habitaci� < _min_num_enemics_per_habitaci� ? _min_num_enemics_per_habitaci� : _max_num_enemics_per_habitaci�;
		}

		void establir_llavor(uint32_t _llavor)
		{
			llavor = _llavor;
		}

		IdEntitat generar(Estat& estat) const override
		{
			RNG rng(llavor);

			// crear habitacions i passadissos
			std::vector<Habitaci�> habitacions;
			std::vector<Passad�s> passadissos;

			std::uniform_int_distribution<> amplada_qualsevol(1, estat.mapa().obtenir_amplada() - 4);
			std::uniform_int_distribution<> al�ada_qualsevol(1, estat.mapa().obtenir_al�ada() - 4);

			std::uniform_int_distribution<> tamany(3, 15);

			int num_habitacions = std::uniform_int_distribution<>(min_num_habitacions, max_num_habitacions)(rng);
			int num_extra_passadissos = std::uniform_int_distribution<>(min_num_extra_passadissos, max_num_extra_passadissos)(rng);

			int intents = 0;
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

				if (!solapada && habitaci�.v2.x < estat.mapa().obtenir_amplada() - 1 && habitaci�.v2.y < estat.mapa().obtenir_al�ada() - 1)
				{
					habitacions.push_back(habitaci�);
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
			for (int y = 0; y < estat.mapa().obtenir_al�ada(); ++y)
				for (int x = 0; x < estat.mapa().obtenir_amplada(); ++x)
				{
					estat.mapa().establir_rajola({ x, y }, TipusRajola::Paret);
				}

			// pintar habitacions
			for (Habitaci� habitaci� : habitacions)
			{
				for (int y = habitaci�.v1.y; y <= habitaci�.v2.y; ++y)
					for (int x = habitaci�.v1.x; x <= habitaci�.v2.x; ++x)
					{
						estat.mapa().establir_rajola({ x, y }, TipusRajola::Terra);
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
					estat.mapa().establir_rajola({ x, origen.y }, TipusRajola::Terra);
				}

				// passad�s vertical
				int oy = origen.y < dest�.y ? origen.y : dest�.y;
				int dy = origen.y < dest�.y ? dest�.y : origen.y;

				for (int y = oy; y <= dy; ++y)
				{
					estat.mapa().establir_rajola({dest�.x, y}, TipusRajola::Terra);
				}
			}

			// enemics
			for (int h = 1; h < habitacions.size(); ++h)
			{
				int num_enemics = std::uniform_int_distribution<>(min_num_enemics_per_habitaci�, max_num_enemics_per_habitaci�)(rng);

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

					Punt2D posici�_enemic;

					posici�_enemic.x = std::uniform_int_distribution<>(habitacions[h].v1.x + 2, habitacions[h].v2.x - 2)(rng);
					posici�_enemic.y = std::uniform_int_distribution<>(habitacions[h].v1.y + 2, habitacions[h].v2.y - 2)(rng);

					if (!estat.buscar_entitat(posici�_enemic))
					{
						afegir_entitat(estat, tipus_enemic, posici�_enemic);
					}
					else if (intents < 100)
					{
						++intents;
						--e;
					}
				}

			}

			// inici del jugador
			Punt2D posici�_jugador = habitacions[0].centre();
			return afegir_entitat(estat, TipusEntitat::Jugador, posici�_jugador);
		}

	private:
		uint32_t min_num_habitacions = 3;
		uint32_t max_num_habitacions = 7;

		uint32_t min_num_extra_passadissos = 0;
		uint32_t max_num_extra_passadissos = 3;

		uint32_t min_num_enemics_per_habitaci� = 0;
		uint32_t max_num_enemics_per_habitaci� = 2;

		uint32_t llavor = 0;
	};
}
