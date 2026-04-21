#pragma once

#include "Base.h"
#include "Texture/Texture2D.h"
#include <string>
#include <unordered_map>
#include <vector>
namespace VW
{
    class VW_API TextureSystem
    {
    public:
        struct ARBTexture
        {
            u64 Handle = 0;
            Texture2D Tex;
            ARBTexture() = default;
            ARBTexture(ARBTexture &&) = default;
            ARBTexture &operator=(ARBTexture &&) = default;
        };

        struct State
        {
            std::vector<ARBTexture> Textures;
            std::unordered_map<std::string, u64> NameToIndex;
        };

    public:
        TextureSystem() = delete;
        ~TextureSystem() = delete;

        static void Init();
        static void Shutdown();

        static u64 GetTextureHandle(int index);
        static u64 GetTextureHandle(const std::string &path);
        static u64 GetTextureID(const std::string &path);
        static u64 CreateTexture(const std::string &path);

        static u64 GetDefaultTextureID();
    };
} // namespace VW
