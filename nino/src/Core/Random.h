#pragma once

#include <random>

namespace nino
{
	class Random
	{
	public:
		Random()
		{
			std::random_device rd;
			std::mt19937_64 engine(rd());
			std::uniform_int_distribution<uint64_t> distribution;

			m_RandomNumber = distribution(engine);
		}

		Random(size_t seed)
		{
			std::mt19937_64 engine(seed);
			std::uniform_int_distribution<uint64_t> distribution;

			m_RandomNumber = distribution(engine);
		}

		operator uint64_t() { return m_RandomNumber; }

	private:
		uint64_t m_RandomNumber;

	};
}