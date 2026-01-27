#pragma once
#include <filesystem>

void SetAssetRootPath(const std::filesystem::path& path);

std::filesystem::path GetAssetRootPath();

enum class AssetType {
    Shader = 0,
    Texture,
    Model,
    AssetTypeMax,
};

std::filesystem::path GetAssetPath(AssetType type, const std::filesystem::path& fileName);
