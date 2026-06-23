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

    Matrix4 Actor::GetWorldMatrix() const
    {
        Matrix4 local = m_Transform.GetMatrix();
        if (m_Parent)
            return m_Parent->GetWorldMatrix() * local;
        return local;
    }

    void Actor::SetName(const std::string &name)
    {
        m_Name = name;
    }

    void Actor::SetTransform(const Transform &transform)
    {
        m_Transform = transform;
    }

    Actor *Actor::AddChild(std::unique_ptr<Actor> child)
    {
        child->m_Parent = this;
        Actor *ptr = child.get();
        m_Children.push_back(std::move(child));
        return ptr;
    }

    std::unique_ptr<Actor> Actor::RemoveChild(Actor *child)
    {
        for (auto it = m_Children.begin(); it != m_Children.end(); ++it)
        {
            if (it->get() == child)
            {
                auto ptr = std::move(*it);
                m_Children.erase(it);
                ptr->m_Parent = nullptr;
                return ptr;
            }
        }
        return nullptr;
    }

    void Actor::Start()
    {
        m_MustStartComponents = true;

        for (auto &component : m_Components)
            component->Start();

        for (auto &child : m_Children)
            child->Start();
    }

    void Actor::Render()
    {
        m_MustStartComponents = true;

        for (auto &component : m_Components)
            component->Render();

        for (auto &child : m_Children)
            child->Render();
    }

    void Actor::Update()
    {
        m_MustStartComponents = true;

        for (auto &component : m_Components)
            component->Update();

        for (auto &child : m_Children)
            child->Update();
    }

    void Actor::Stop()
    {
        m_MustStartComponents = false;

        for (auto &component : m_Components)
            component->Stop();

        for (auto &child : m_Children)
            child->Stop();
    }
} // namespace VW
