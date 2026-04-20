#include "TextureSystem.h"
#include "Core/Logger.h"
#include <glad/glad.h>

// todo: just realized how shitty this code please future vanea fix this shit
// TODO: new codebase same old mistakes

namespace VW
{
    static TextureSystem::State s_State;

    void TextureSystem::Init()
    {
        s_State.Textures.reserve(256);

        ARBTexture defaultTex;
        defaultTex.Tex.Load();
        defaultTex.Handle = glGetTextureHandleARB(defaultTex.Tex.GetID());
        glMakeTextureHandleResidentARB(defaultTex.Handle);
        s_State.Textures.emplace_back(std::move(defaultTex));
        s_State.NameToIndex["__DEFAULT_WHITE__"] = s_State.Textures.size() - 1;
        if (!glIsTextureHandleResidentARB(s_State.Textures.back().Handle))
        {
            VW_ERROR("vwrn", "Default texture handle %llu failed to become resident",
                     s_State.Textures.back().Handle);
        }
    }

    void TextureSystem::Shutdown()
    {
    }

    u64 TextureSystem::GetHandle(int index)
    {
        if (index < 0 || (size_t)index >= s_State.Textures.size())
            return 0;

        return s_State.Textures[index].Handle;
    }

    u64 TextureSystem::GetHandle(const std::string &path)
    {
        if (s_State.NameToIndex.count(path) == 0)
            return 0;
        return GetHandle(s_State.NameToIndex[path]);
    }

    u64 TextureSystem::CreateTexture(const std::string &path)
    {
        if (s_State.NameToIndex.count(path))
            return s_State.NameToIndex[path];

        ARBTexture tex;
        tex.Tex.Load(path);

        GLuint texID = tex.Tex.GetID();

        tex.Handle = glGetTextureHandleARB(texID);

#if 1
        for (const auto &existingTex : s_State.Textures)
        {
            if (existingTex.Tex.GetID() == texID)
            {
                VW_ERROR("vwrn", "DUPLICATE ID DETECTED! ID %u already used by another texture!",
                         texID);
            }
        }
#endif

        glMakeTextureHandleResidentARB(tex.Handle);

        if (!glIsTextureHandleResidentARB(tex.Handle))
        {
            VW_ERROR("vwrn", "Handle %llu failed to become resident (%s)", tex.Handle,
                     path.c_str());
        }

        s_State.Textures.emplace_back(std::move(tex));
        int index = s_State.Textures.size() - 1;
        s_State.NameToIndex[path] = index;

        return index;
    }
    u64 TextureSystem::GetDefaultTexture()
    {
        return s_State.NameToIndex["__DEFAULT_WHITE__"];
    }

}; // namespace VW
