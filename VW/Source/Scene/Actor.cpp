#include "Actor.h"

namespace VW
{
    Actor::Actor(const std::string &name) : m_Name(name)
    {
        m_MustStartComponents = false;
    }

    Actor::~Actor()
    {
        Stop();
    }

    void Actor::SetName(const std::string &name)
    {
        m_Name = name;
    }

    void Actor::SetTransform(const Transform &transform)
    {
        m_Transform = transform;
    }

    void Actor::Start()
    {
        m_MustStartComponents = true;

        for (auto &component : m_Components)
        {
            component->Start();
        }
    }

    void Actor::Render()
    {
        m_MustStartComponents = true;

        for (auto &component : m_Components)
        {
            component->Render();
        }
    }

    void Actor::Update()
    {
        m_MustStartComponents = true;

        for (auto &component : m_Components)
        {
            component->Update();
        }
    }

    void Actor::Stop()
    {
        m_MustStartComponents = false;

        for (auto &component : m_Components)
        {
            component->Stop();
        }
    }
} // namespace VW
