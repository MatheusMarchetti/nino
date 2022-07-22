#pragma once

#include <xhash>
#include <random>

namespace nino
{
	class UUID
	{
	public:
		UUID() = default;

		uint64_t GenerateUUID()
		{
			std::random_device rd;
			std::mt19937_64 engine(rd());
			std::uniform_int_distribution<uint64_t> distribution;

			m_UUID = distribution(engine);

			return m_UUID;
		}

		uint64_t GenerateUUID(size_t seed)
		{
			std::mt19937_64 engine(seed);
			std::uniform_int_distribution<uint64_t> distribution;

			m_UUID = distribution(engine);

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