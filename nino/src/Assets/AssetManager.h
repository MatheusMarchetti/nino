#pragma once

#include "Assets/Asset.h"

#include "Core/Core.h"

#include "Renderer/Shader.h"
#include "Renderer/Texture.h"

namespace nino
{
	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager() = default;

		static void LoadAsset(const std::string& filePath, Ref<Texture>& texture);
		static void LoadAsset(const std::string& filePath, Ref<Shader>& shader);

		static void UnloadAsset(const Ref<Asset>& asset);

	private:
		static UUID GenerateAssetUUID(const std::string& filePath);

	private:
		inline static std::unordered_map<UUID, Ref<Texture>> s_TextureCache;
		inline static std::unordered_map<UUID, Ref<Shader>> s_ShaderCache;
	};
}