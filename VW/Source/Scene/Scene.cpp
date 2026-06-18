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

    void Scene::SetName(const std::string &name)
    {
        m_Name = name;
    }

} // namespace VW
