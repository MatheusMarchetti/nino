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

		UUID GetUUID() { return m_UUID; }

		void SetUUID(UUID assetID) { m_UUID = assetID; }

	private:
		UUID m_UUID;
	};
}