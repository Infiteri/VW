#pragma once

#include "Base.h"
#include "Core/UUID.h"
#include "Math/Transform.h"
#include "Scene/Components.h"
#include <algorithm>
#include <memory>
#include <vector>

namespace VW
{
    class VW_API Actor
    {
    public:
        Actor(const std::string &name = "Actor");
        ~Actor();

        void Start();
        void Render();
        void Update();
        void Stop();

        void SetTransform(const Transform &transform);
        inline Transform &GetTransform()
        {
            return m_Transform;
        }

        inline const UUID &GetID() const
        {
            return m_ID;
        }

        void SetName(const std::string &name);

        inline const std::string &GetName() const
        {
            return m_Name;
        }

        template <typename T, typename... Args> T *AddComponent(Args... args)
        {
            std::unique_ptr<T> c = std::make_unique<T>(args...);
            c->m_Owner = this;

            if (m_MustStartComponents)
                c->Start();

            m_Components.push_back(std::move(c));
            return (T *)m_Components.back().get();
        };

        template <typename T> T *GetComponent()
        {
            for (const auto &comp : m_Components)
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
            for (const auto &comp : m_Components)
            {
                T *tc = dynamic_cast<T *>(comp.get());
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
                T *tc = dynamic_cast<T *>(it->get());

                if (tc)
                {
                    currentIndex++;

                    if (currentIndex == index)
                    {
                        m_Components.erase(it);
                        break;
                    }
                }
            }
        };

        template <typename T> void RemoveComponents()
        {
            for (auto it = m_Components.begin(); it != m_Components.end();)
            {
                T *tc = dynamic_cast<T *>(it->get());

                if (tc)
                    it = m_Components.erase(it);
                else
                    ++it;
            }
        };

        Matrix4 GetWorldMatrix() const;
        Actor *AddChild(std::unique_ptr<Actor> child);
        void RemoveChild(Actor *child);

    private:
        std::string m_Name;
        UUID m_ID;
        bool m_MustStartComponents = false;
        std::vector<std::unique_ptr<Component>> m_Components;

        Transform m_Transform;

        Actor *m_Parent = nullptr;
        std::vector<std::unique_ptr<Actor>> m_Children;

        friend class ActorSerializer;
    };
} // namespace VW
