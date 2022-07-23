#pragma once

#include "Assets/Asset.h"
#include "Assets/AssetLoader.h"
#include "Core/Core.h"

namespace nino
{
	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager() = default;

		static void LoadAsset(const std::string& filePath, Ref<Asset> asset);
		static void UnloadAsset(Ref<Asset> asset);

	private:
		inline static std::unordered_map<uint64_t, Ref<Asset>> s_AssetCache;
	};
}