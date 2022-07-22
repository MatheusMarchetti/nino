#pragma once

#include "Core/UUID.h"

namespace nino
{
	class Asset
	{

	public:
		Asset() = default;
		virtual ~Asset() = default;

		UUID GetUUID() { return m_UUID; }

		void SetUUID(const std::string& fileName)
		{
			std::hash<std::string> hasher;

			size_t hashedString = hasher(fileName);

			m_UUID = m_UUID.GenerateUUID(hashedString);
		}

		void SetUUID() { m_UUID = m_UUID.GenerateUUID(); }

	private:
		UUID m_UUID;
	};
}