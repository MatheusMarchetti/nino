#pragma once

#include "Assets/Asset.h"
#include "Assets/AssetLoader.h"
#include "Core/Core.h"

#include "Renderer/Texture.h"

namespace nino
{
	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager() = default;

		static void LoadAsset(const std::string& filePath, Ref<Texture>& texture);

		static void UnloadAsset(const Ref<Asset>& asset);

	private:
		inline static std::unordered_map<UUID, Ref<Texture>> s_TextureCache;
	};
}