#include "corepch.h"
#include "AssetManager.h"

namespace nino
{
    AssetManager::AssetManager()
    {
//        DirectX::ScratchImage loadedImage = AssetLoader::LoadTexture("C:/dev/Game Engine/nino/Sandbox/Assets/Textures/PBR/rustediron2_basecolor.dds");
//
//        UUID teste;
//        AssetLoader::CreateBinaryTexture(teste.GenerateUUID(), loadedImage);
    }

    void AssetManager::LoadAsset(const std::string& filePath, Ref<Texture>& texture)
    {
        std::filesystem::path assetFilePath = filePath;
        std::hash<std::string> hasher;

        std::string stringToHash = assetFilePath.filename().string() + std::to_string(std::filesystem::last_write_time(assetFilePath).time_since_epoch().count());

        UUID comparison;
        comparison = comparison.GenerateUUID(hasher(stringToHash));

        if (s_TextureCache.find(comparison) != s_TextureCache.end())
        {
            texture = s_TextureCache[comparison];
            texture->SetUUID(comparison);

            return;
        }

        DirectX::ScratchImage loadedImage = AssetLoader::LoadTexture(filePath);
            
        TextureType type = TextureType::Texture2D;

        if (loadedImage.GetImageCount() > 1)
        {
            type = TextureType::Texture2DArray;

            if (loadedImage.GetImageCount() % 6 == 0)
                type = TextureType::TextureCube;
        }

        texture = CreateRef<Texture>(type, TextureUsage::ColorBinding, loadedImage);
        texture->SetUUID(comparison);

        s_TextureCache[comparison] = texture;
    }

    void AssetManager::UnloadAsset(Ref<Asset> asset)
    {

    }
}