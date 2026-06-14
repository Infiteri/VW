#include "Scene.h"
#include "Renderer.h"

namespace VW
{
    Scene::Scene(const std::string &name) : m_Name(name)
    {
        m_MustStartActors = false;

        m_Sky.SetColorMode({0, 125});
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

    void Scene::AddActor(std::unique_ptr<Actor> actor)
    {
        if (m_MustStartActors)
            actor->Start();

        m_Actors.push_back(std::move(actor));
    }

    void Scene::SetName(const std::string &name)
    {
        m_Name = name;
    }

} // namespace VW
