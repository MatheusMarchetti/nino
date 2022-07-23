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

    void AssetManager::LoadAsset(const std::string& filePath, Ref<Asset> asset)
    {

    }

    void AssetManager::UnloadAsset(Ref<Asset> asset)
    {

    }
}