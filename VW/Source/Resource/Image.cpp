#include "Image.h"

#include <stb_image.h>

namespace VW
{
    Image::Image(const std::string &name)
        : m_Data(nullptr), m_Width(0), m_Height(0), m_Channels(0), m_Path(name)
    {
        int w, h, c;
        stbi_set_flip_vertically_on_load(true);
        m_Data = stbi_load(name.c_str(), &w, &h, &c, 0);
        if (!m_Data)
        {
            m_Valid = false;
            return;
        }

        m_Width = w;
        m_Height = h;
        m_Channels = c;
        m_Valid = true;
    }

    Image::~Image()
    {
        if (!m_Data)
            return;
        stbi_image_free(m_Data);
    }
} // namespace VW
