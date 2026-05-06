#pragma once

#include "Base.h"
#include <string>

namespace VW
{
    class VW_API Image
    {
    public:
        Image(const std::string &name);
        ~Image();

        inline int GetWidth() const
        {
            return m_Width;
        };
        inline int GetHeight() const
        {
            return m_Height;
        };
        inline int GetChannels() const
        {
            return m_Channels;
        };
        inline u8 *GetData() const
        {
            return m_Data;
        };
        inline std::string GetPath() const
        {
            return m_Path;
        };
        inline bool IsValid() const
        {
            return m_Valid;
        };

    private:
        u8 *m_Data;
        int m_Width, m_Height, m_Channels;
        std::string m_Path;
        bool m_Valid = false;
    };
} // namespace VW
