#pragma once

#include "Base.h"
#include "Core/UUID.h"
#include "Scene/Components.h"
#include <algorithm>
#include <vector>

namespace VW
{
    class VW_API Actor
    {
    public:
        Actor();
        ~Actor();

        void Start();
        void Render();
        void Update();
        void Stop();

        inline const UUID &GetID() const
        {
            return m_ID;
        }

        template <typename T, typename... Args> T *AddComponent(Args... args)
        {
            std::unique_ptr<T> c = std::make_unique<T>(args...);
            c->m_Owner = this;

            if (m_MustStartComponents)
                c->Start();

            m_Components.push_back(std::move(c));
            return c.get();
        };

        template <typename T> T *GetComponent()
        {
            for (auto comp : m_Components)
            {
                T *tc = dynamic_cast<T *>(comp.get());
                if (tc)
                    return tc;
            }

            return nullptr;
        };

        template <typename T> std::vector<T *> GetComponents()
        {
            std::vector<T *> comps;
            for (auto comp : m_Components)
            {
                T *tc = dynamic_cast<T *>(comp);
                if (tc)
                    comps.push_back(tc);
            }

            return comps;
        };

        template <typename T> bool HasComponent()
        {
            return GetComponent<T>() != nullptr;
        };

        template <typename T> void RemoveComponent(int index = 0)
        {
            int currentIndex = -1;
            for (auto it = m_Components.begin(); it != m_Components.end(); it++)
            {
                T *tc = dynamic_cast<T *>(*it);

                if (tc)
                {
                    currentIndex++;

                    if (currentIndex == index)
                    {
                        delete tc;
                        m_Components.erase(it);
                        break;
                    }
                }
            }
        };

        template <typename T> void RemoveComponents()
        {
            for (auto it = m_Components.begin(); it != m_Components.end(); it++)
            {
                T *tc = dynamic_cast<T *>(*it);

                if (tc)
                {
                    delete tc;
                    m_Components.erase(it);
                }
            }
        };

    private:
        UUID m_ID;
        bool m_MustStartComponents = false;
        std::vector<std::unique_ptr<Component>> m_Components;
    };
} // namespace VW
