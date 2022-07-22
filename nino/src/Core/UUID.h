#pragma once

#include "Core/Random.h"

#include <xhash>

namespace nino
{
	class UUID
	{
	public:
		UUID() = default;

		uint64_t GenerateUUID()
		{
			Random rng;
			m_UUID = rng;

			return m_UUID;
		}

		uint64_t GenerateUUID(size_t seed)
		{
			Random rng(seed);
			m_UUID = rng;

			return m_UUID;
		}

		operator uint64_t() const { return m_UUID; }
		UUID& operator = (const uint64_t& value) { m_UUID = value; return *this; }

	private:
		uint64_t m_UUID;
	};
}

namespace std
{
	template<>
	struct hash<nino::UUID>
	{
		std::size_t operator()(const nino::UUID& uuid) const
		{
			return (uint64_t)uuid;
		}
	};
}