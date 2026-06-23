#pragma once

#include <memory>
#include <vector>
namespace VW
{
    class Panel
    {
    public:
        Panel() = default;
        virtual ~Panel() = default;

        virtual void Render() = 0;
    };

    class PanelSystem
    {
    public:
        PanelSystem() = default;

        void AddPanel(std::unique_ptr<Panel> panel);
        void Render();

    private:
        std::vector<std::unique_ptr<Panel>> m_Panels;
    };
} // namespace VW
