#pragma once

#include "Core/Random.h"

namespace nino
{
	class UUID
	{
	public:
		uint64_t GenerateUUID(size_t initialSeed = 5489U)
		{
			Random rng(initialSeed);

			m_UUID = rng.Generate();

			return m_UUID;
		}

		operator bool() { return m_UUID == 0 ? false : true; }
		operator uint64_t() const { return m_UUID; }
		UUID& operator = (const uint64_t& value) { m_UUID = value; return *this; }

	private:
		uint64_t m_UUID = 0;
	};
}