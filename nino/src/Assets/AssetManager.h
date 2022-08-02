#pragma once

#include "Assets/Asset.h"
#include "Assets/AssetLoader.h"

#include "Core/Core.h"

#include "Renderer/Texture.h"
#include "Renderer/Shader.h"

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
		inline static std::unordered_map<UUID, Ref<Texture>> s_TextureCache;
		inline static std::unordered_map<UUID, Ref<Shader>> s_ShaderCache;
	};
}