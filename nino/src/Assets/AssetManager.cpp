#include "corepch.h"
#include "AssetManager.h"

#include "Renderer/Texture.h"

namespace nino
{
    AssetManager::AssetManager()
    {
//        DirectX::ScratchImage loadedImage = AssetLoader::LoadTexture("C:/dev/Game Engine/nino/Sandbox/Assets/Textures/PBR/rustediron2_basecolor.dds");
//
//        UUID teste;
//        AssetLoader::CreateBinaryTexture(teste.GenerateUUID(), loadedImage);
    }

    void AssetManager::LoadAsset(const std::string& filePath, Ref<Asset> asset)
    {
        std::filesystem::path assetFilePath = filePath;
        std::hash<std::string> hasher;

        std::string stringToHash = assetFilePath.filename().string() + std::to_string(std::filesystem::last_write_time(assetFilePath).time_since_epoch().count());

        UUID comparison;
        comparison = comparison.GenerateUUID(hasher(stringToHash));
        asset->SetUUID(comparison);

        if (s_AssetCache.find(comparison) != s_AssetCache.end())
        {
            asset = s_AssetCache[comparison];

            return;
        }



        if (assetFilePath.extension() == ".dds" || assetFilePath.extension() == ".tga" || assetFilePath.extension() == ".hdr" || assetFilePath.extension() == ".bmp" || assetFilePath.extension() == ".jpg" || assetFilePath.extension() == ".png" || assetFilePath.extension() == ".tiff" || assetFilePath.extension() == ".gif")
        {
            DirectX::ScratchImage loadedImage = AssetLoader::LoadTexture(filePath);
            
            TextureType type = TextureType::Texture2D;

            if (loadedImage.GetImageCount() > 1)
            {
                type = TextureType::Texture2DArray;

                if (loadedImage.GetImageCount() % 6 == 0)
                    type = TextureType::TextureCube;
            }

            asset = CreateRef<Texture>(type, TextureUsage::ColorBinding, loadedImage);

            s_AssetCache[asset->GetUUID()] = asset;
        }
    }

    void AssetManager::UnloadAsset(Ref<Asset> asset)
    {

    }
}