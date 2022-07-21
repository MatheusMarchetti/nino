#pragma once

#include <random>

namespace nino
{
	class Random
	{
	public:
		Random(size_t initialSeed) : m_RandomEngine(initialSeed) {}

		uint64_t Generate() { return m_Distribution(m_RandomEngine); }

	private:
		std::mt19937_64 m_RandomEngine;
		std::uniform_int_distribution<uint64_t> m_Distribution;
	};
}