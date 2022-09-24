// Most of this code is based if not copied from Squirrel Eiserloh
// https://www.youtube.com/watch?v=LWFzPP8ZbdU&t=2670s&ab_channel=GDC
module;

#include <cstdint>

#include <bit>

export module Comú:NoiseRNG;

namespace bretolesc
{
	// base hash functions

	inline constexpr uint32_t squirrel_noise_3(uint32_t position, uint32_t seed) noexcept
	{
		constexpr uint32_t BIT_NOISE1 = 0x68E31DA4;
		constexpr uint32_t BIT_NOISE2 = 0xB5297A4D;
		constexpr uint32_t BIT_NOISE3 = 0x1B56C4E9;

		uint32_t mangled_bits = position;
		mangled_bits *= BIT_NOISE1;
		mangled_bits += seed;
		mangled_bits ^= (mangled_bits >> 8);
		mangled_bits += BIT_NOISE2;
		mangled_bits ^= (mangled_bits << 8);
		mangled_bits += BIT_NOISE3;
		mangled_bits ^= (mangled_bits >> 8);
		return mangled_bits;
	}

	inline constexpr uint32_t squirrel_noise_3(int32_t position, uint32_t seed) noexcept
	{
		return squirrel_noise_3(std::bit_cast<uint32_t>(position), seed);
	}

	// int result ------------------------------------------------------------------------------------------

	export inline constexpr uint32_t get_1d_noise(int32_t position_x, uint32_t seed = 0) noexcept
	{
		return squirrel_noise_3(position_x, seed);
	}

	export inline constexpr uint32_t get_2d_noise(int32_t position_x, int32_t position_y, uint32_t seed = 0) noexcept
	{
		constexpr uint32_t PRIME_NUMBER = 19891317;
		return squirrel_noise_3(std::bit_cast<uint32_t>(position_x) + (PRIME_NUMBER * std::bit_cast<uint32_t>(position_y)), seed);
	}

	export inline constexpr uint32_t get_3d_noise(int32_t position_x, int32_t position_y, int32_t position_z, uint32_t seed = 0) noexcept
	{
		constexpr uint32_t PRIME_NUMBER_1 = 19891317;
		constexpr uint32_t PRIME_NUMBER_2 = 9542989;
		return squirrel_noise_3(std::bit_cast<uint32_t>(position_x) + (PRIME_NUMBER_1 * std::bit_cast<uint32_t>(position_y)) + (PRIME_NUMBER_2 * std::bit_cast<uint32_t>(position_z)), seed);
	}

	export inline constexpr uint32_t get_4d_noise(int32_t position_x, int32_t position_y, int32_t position_z, int32_t position_t, uint32_t seed = 0) noexcept
	{
		constexpr uint32_t PRIME_NUMBER_1 = 19891317;
		constexpr uint32_t PRIME_NUMBER_2 = 9542989;
		constexpr uint32_t PRIME_NUMBER_3 = 111828121;
		return squirrel_noise_3(std::bit_cast<uint32_t>(position_x) + (PRIME_NUMBER_1 * std::bit_cast<uint32_t>(position_y)) + (PRIME_NUMBER_2 * std::bit_cast<uint32_t>(position_z)) + (PRIME_NUMBER_3 * std::bit_cast<uint32_t>(position_t)), seed);
	}

	export inline constexpr uint32_t get_5d_noise(int32_t position_x, int32_t position_y, int32_t position_z, int32_t position_t, int32_t position_u, uint32_t seed = 0) noexcept
	{
		constexpr uint32_t PRIME_NUMBER_1 = 19891317;
		constexpr uint32_t PRIME_NUMBER_2 = 9542989;
		constexpr uint32_t PRIME_NUMBER_3 = 111828121;
		constexpr uint32_t PRIME_NUMBER_4 = 111321901;
		return squirrel_noise_3(std::bit_cast<uint32_t>(position_x) + (PRIME_NUMBER_1 * std::bit_cast<uint32_t>(position_y)) + (PRIME_NUMBER_2 * std::bit_cast<uint32_t>(position_z)) + (PRIME_NUMBER_3 * std::bit_cast<uint32_t>(position_t)) + (PRIME_NUMBER_4 * std::bit_cast<uint32_t>(position_u)), seed);
	}

	// float [0-1) result ------------------------------------------------------------------------------------------

	inline float transform_to_0to1_float(uint32_t noise) noexcept
	{
		uint32_t pseudomantissa = (noise >> 3) & 0x00ffffff;
		return float(pseudomantissa) / float(0x01000000);
	}

	export inline float get_1d_noise_0to1(int32_t position_x, uint32_t seed = 0) noexcept
	{
		return transform_to_0to1_float(get_1d_noise(position_x, seed));
	}

	export inline float get_2d_noise_0to1(int32_t position_x, int32_t position_y, uint32_t seed = 0) noexcept
	{
		return transform_to_0to1_float(get_2d_noise(position_x, position_y, seed));
	}

	export inline float get_3d_noise_0to1(int32_t position_x, int32_t position_y, int32_t position_z, uint32_t seed = 0) noexcept
	{
		return transform_to_0to1_float(get_3d_noise(position_x, position_y, position_z, seed));
	}

	export inline float get_4d_noise_0to1(int32_t position_x, int32_t position_y, int32_t position_z, int32_t position_t, uint32_t seed = 0) noexcept
	{
		return transform_to_0to1_float(get_4d_noise(position_x, position_y, position_z, position_t, seed));
	}

	// float [-1 - 1] result ------------------------------------------------------------------------------------------

	inline float transform_to_n1to1_float(uint32_t noise) noexcept
	{
		int32_t pseudomantissa = std::bit_cast<uint32_t>((noise >> 3) % 0x02000001) - 0x01000000;
		return float(pseudomantissa) / float(0x01000000);
	}

	export inline float get_1d_noise_n1to1(int32_t position_x, uint32_t seed = 0) noexcept
	{
		return transform_to_n1to1_float(get_1d_noise(position_x, seed));
	}

	export inline float get_2d_noise_n1to1(int32_t position_x, int32_t position_y, uint32_t seed = 0) noexcept
	{
		return transform_to_n1to1_float(get_2d_noise(position_x, position_y, seed));
	}

	export inline float get_3d_noise_n1to1(int32_t position_x, int32_t position_y, int32_t position_z, uint32_t seed = 0) noexcept
	{
		return transform_to_n1to1_float(get_3d_noise(position_x, position_y, position_z, seed));
	}

	export inline float get_4d_noise_n1to1(int32_t position_x, int32_t position_y, int32_t position_z, int32_t position_t, uint32_t seed = 0) noexcept
	{
		return transform_to_n1to1_float(get_4d_noise(position_x, position_y, position_z, position_t, seed));
	}

	// double [0-1) result ------------------------------------------------------------------------------------------

	inline double transform_to_0to1_double(uint32_t noise1, uint32_t noise2) noexcept
	{
		uint64_t pseudomantissa = ((uint64_t(noise1) << 27) ^ uint64_t(noise2)) & 0x0007ffffffffffffULL;
		return double(pseudomantissa) / double(0x0008000000000000ULL);
	}

	export inline double get_1d_noise_0to1_double(int32_t position_x, uint32_t seed = 0) noexcept
	{
		uint32_t noise1 = get_1d_noise(position_x, seed);
		uint32_t noise2 = get_1d_noise(position_x, (seed + 725285367) * 1156616921);
		return transform_to_0to1_double(noise1, noise2);
	}

	export inline double get_2d_noise_0to1_double(int32_t position_x, int32_t position_y, uint32_t seed = 0) noexcept
	{
		uint32_t noise1 = get_2d_noise(position_x, position_y, seed);
		uint32_t noise2 = get_2d_noise(position_x, position_y, (seed + 725285367) * 1156616921);
		return transform_to_0to1_double(noise1, noise2);
	}

	export inline double get_3d_noise_0to1_double(int32_t position_x, int32_t position_y, int32_t position_z, uint32_t seed = 0) noexcept
	{
		uint32_t noise1 = get_3d_noise(position_x, position_y, position_z, seed);
		uint32_t noise2 = get_3d_noise(position_x, position_y, position_z, (seed + 725285367) * 1156616921);
		return transform_to_0to1_double(noise1, noise2);
	}

	export inline double get_4d_noise_0to1_double(int32_t position_x, int32_t position_y, int32_t position_z, int32_t position_t, uint32_t seed = 0) noexcept
	{
		uint32_t noise1 = get_4d_noise(position_x, position_y, position_z, position_t, seed);
		uint32_t noise2 = get_4d_noise(position_x, position_y, position_z, position_t, (seed + 725285367) * 1156616921);
		return transform_to_0to1_double(noise1, noise2);
	}

	// double [-1 - 1] result ------------------------------------------------------------------------------------------

	inline double transform_to_n1to1_double(uint32_t noise1, uint32_t noise2) noexcept
	{
		uint64_t full_agreggate = (uint64_t(noise1) << 32) ^ uint64_t(noise2);
		uint64_t divided = full_agreggate % 0x0010000000000001ULL;
		uint64_t pseudomantissa = divided - 0x0008000000000000ULL;
		return double(std::bit_cast<int64_t>(pseudomantissa)) / double(0x0008000000000000ULL);
	}

	export inline double get_1d_noise_n1to1_double(int32_t position_x, uint32_t seed = 0) noexcept
	{
		uint32_t noise1 = get_1d_noise(position_x, seed);
		uint32_t noise2 = get_1d_noise(position_x, (seed + 725285367) * 1156616921);
		return transform_to_n1to1_double(noise1, noise2);
	}

	export inline double get_2d_noise_n1to1_double(int32_t position_x, int32_t position_y, uint32_t seed = 0) noexcept
	{
		uint32_t noise1 = get_2d_noise(position_x, position_y, seed);
		uint32_t noise2 = get_2d_noise(position_x, position_y, (seed + 725285367) * 1156616921);
		return transform_to_n1to1_double(noise1, noise2);
	}

	export inline double get_3d_noise_n1to1_double(int32_t position_x, int32_t position_y, int32_t position_z, uint32_t seed = 0) noexcept
	{
		uint32_t noise1 = get_3d_noise(position_x, position_y, position_z, seed);
		uint32_t noise2 = get_3d_noise(position_x, position_y, position_z, (seed + 725285367) * 1156616921);
		return transform_to_n1to1_double(noise1, noise2);
	}

	export inline double get_4d_noise_n1to1_double(int32_t position_x, int32_t position_y, int32_t position_z, int32_t position_t, uint32_t seed = 0) noexcept
	{
		uint32_t noise1 = get_4d_noise(position_x, position_y, position_z, position_t, seed);
		uint32_t noise2 = get_4d_noise(position_x, position_y, position_z, position_t, (seed + 725285367) * 1156616921);
		return transform_to_n1to1_double(noise1, noise2);
	}

	export inline double get_5d_noise_n1to1_double(int32_t position_x, int32_t position_y, int32_t position_z, int32_t position_t, int32_t position_u, uint32_t seed = 0) noexcept
	{
		uint32_t noise1 = get_5d_noise(position_x, position_y, position_z, position_t, position_u, seed);
		uint32_t noise2 = get_5d_noise(position_x, position_y, position_z, position_t, position_u, (seed + 725285367) * 1156616921);
		return transform_to_n1to1_double(noise1, noise2);
	}

	// random class ------------------------------------------------------------------------------------------

	export class RNG
	{
	public:
		RNG() noexcept : seed(), position(0) { }
		explicit RNG(uint32_t _seed) noexcept : seed(_seed), position(0) { }

		inline void reset_seed(uint32_t _seed, uint32_t _position = 0) noexcept { seed = _seed; position = _position; }
		inline void reset_position(uint32_t _position) noexcept { position = _position; }
		inline uint32_t get_seed() const noexcept { return seed; }
		inline void set_current_position(uint32_t _position) noexcept { position = _position; }
		inline uint32_t get_current_position() const noexcept { return position; }

		inline uint32_t get_next_uint32() noexcept { return squirrel_noise_3(position++, seed); }
		inline float get_next_float() noexcept { return transform_to_0to1_float(squirrel_noise_3(position++, seed)); }
		inline double get_next_float(double min, double max) noexcept
		{
			return min + get_next_float() * (max - min);
		}
		inline double get_next_double() noexcept
		{
			uint32_t noise1 = squirrel_noise_3(position++, seed);
			uint32_t noise2 = squirrel_noise_3(position++, seed);

			return transform_to_0to1_double(noise1, noise2);
		}
		inline double get_next_double(double min, double max) noexcept
		{
			return min + get_next_double() * (max - min);
		}
		inline float get_next_n1to1_float() noexcept { return transform_to_n1to1_float(squirrel_noise_3(position++, seed)); }
		inline double get_next_n1to1_double() noexcept
		{
			uint32_t noise1 = squirrel_noise_3(position++, seed);
			uint32_t noise2 = squirrel_noise_3(position++, seed);

			return transform_to_n1to1_double(noise1, noise2);
		}


		// UniformRandomBitGenerator
		using result_type = uint32_t;
		inline static uint32_t min() { return 0; }
		inline static uint32_t max() { return 0xffffffff; }
		inline uint32_t operator()()
		{
			return get_next_uint32();
		}



	private:
		uint32_t seed, position;
	};
}
