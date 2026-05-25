#include "Actor.h"

namespace VW
{
    Actor::Actor()
    {
        m_MustStartComponents = false;
    }

    Actor::~Actor()
    {
        Stop();
        for (auto &component : m_Components)
        {
            delete component;
        }
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
}
