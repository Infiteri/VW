#include "Scene.h"
#include "Renderer.h"

namespace VW
{
    Scene::Scene(const std::string &name) : m_Name(name)
    {
        m_MustStartActors = false;
    }

    Scene::~Scene()
    {
    }

    void Scene::Start()
    {
        m_MustStartActors = true;
        for (auto &actor : m_Actors)
            actor->Start();

        Renderer::SetSky(&m_Sky);
    }

    void Scene::Render()
    {
        m_MustStartActors = true;
        for (auto &actor : m_Actors)
            actor->Render();
    }

    void Scene::Update()
    {
        m_MustStartActors = true;
        for (auto &actor : m_Actors)
            actor->Update();
    }

    void Scene::Stop()
    {
        m_MustStartActors = false;
        for (auto &actor : m_Actors)
            actor->Stop();
    }

    Actor *Scene::AddActor(std::unique_ptr<Actor> actor)
    {
        if (m_MustStartActors)
            actor->Start();

        Actor *ptr = actor.get();
        m_Actors.push_back(std::move(actor));
        return ptr;
    }

    Actor *Scene::CreateActor(const std::string &name)
    {
        auto actor = std::make_unique<Actor>(name);
        Actor *ptr = actor.get();
        if (m_MustStartActors)
            actor->Start();
        m_Actors.push_back(std::move(actor));
        return ptr;
    }

    void Scene::DestroyActor(Actor *actor)
    {
        if (!actor)
            return;

        Actor *parent = actor->GetParent();
        if (parent)
        {
            parent->RemoveChild(actor);
        }
        else
        {
            auto it = std::find_if(m_Actors.begin(), m_Actors.end(),
                                   [actor](auto &p) { return p.get() == actor; });
            if (it != m_Actors.end())
                m_Actors.erase(it);
        }
    }

    void Scene::Reparent(Actor *child, Actor *newParent)
    {
        if (!child || child == newParent)
            return;

        if (newParent && _IsDescendant(child, newParent))
            return;

        std::unique_ptr<Actor> owned;
        Actor *oldParent = child->GetParent();
        if (oldParent)
        {
            owned = oldParent->RemoveChild(child);
        }
        else
        {
            auto it = std::find_if(m_Actors.begin(), m_Actors.end(),
                                   [child](auto &p) { return p.get() == child; });
            if (it == m_Actors.end())
                return;
            owned = std::move(*it);
            m_Actors.erase(it);
        }

        if (newParent)
        {
            newParent->AddChild(std::move(owned));
        }
        else
        {
            if (m_MustStartActors)
                owned->Start();
            m_Actors.push_back(std::move(owned));
        }
    }

    static Actor *_FindInTree(Actor *actor, const UUID &id)
    {
        if (actor->GetID() == id)
            return actor;
        for (auto &child : actor->GetChildren())
        {
            Actor *found = _FindInTree(child.get(), id);
            if (found)
                return found;
        }
        return nullptr;
    }

    static Actor *_FindInTree(const std::string &name, Actor *actor)
    {
        if (actor->GetName() == name)
            return actor;
        for (auto &child : actor->GetChildren())
        {
            Actor *found = _FindInTree(name, child.get());
            if (found)
                return found;
        }
        return nullptr;
    }

    Actor *Scene::FindActor(const UUID &id)
    {
        for (auto &root : m_Actors)
        {
            Actor *found = _FindInTree(root.get(), id);
            if (found)
                return found;
        }
        return nullptr;
    }

    Actor *Scene::FindActor(const std::string &name)
    {
        for (auto &root : m_Actors)
        {
            Actor *found = _FindInTree(name, root.get());
            if (found)
                return found;
        }
        return nullptr;
    }

    bool Scene::_IsDescendant(Actor *ancestor, Actor *child)
    {
        for (auto &c : ancestor->GetChildren())
        {
            if (c.get() == child)
                return true;
            if (_IsDescendant(c.get(), child))
                return true;
        }
        return false;
    }

    void Scene::SetName(const std::string &name)
    {
        m_Name = name;
    }

} // namespace VW
