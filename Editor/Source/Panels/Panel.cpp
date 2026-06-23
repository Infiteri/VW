#include "Panel.h"

namespace VW
{

    void PanelSystem::AddPanel(std::unique_ptr<Panel> panel)
    {
        m_Panels.push_back(std::move(panel));
    }

    void PanelSystem::Render()
    {
        for (const auto &panel : m_Panels)
            panel->Render();
    }
} // namespace VW
