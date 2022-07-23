#pragma once

#include "Core/UUID.h"

namespace nino
{
	class Asset
	{
		friend class AssetManager;

	public:
		Asset() = default;
		virtual ~Asset() = default;

	private:
		UUID GetUUID() { return m_UUID; }

		void SetUUID() { m_UUID = m_UUID.GenerateUUID(); }
		void SetUUID(size_t seed) { m_UUID = m_UUID.GenerateUUID(seed); }

	private:
		UUID m_UUID;
	};
}