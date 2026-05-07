#include "CameraSystem.h"

namespace VW
{
    static CameraSystem::State s_State;

    void CameraSystem::Init()
    {
    }

    void CameraSystem::Shutdown()
    {
    }

    Camera *CameraSystem::GetCamera(const std::string &name)
    {
        if (s_State.Cameras.find(name) == s_State.Cameras.end())
            return nullptr;

        return s_State.Cameras[name].get();
    }

    void CameraSystem::AddCamera(const std::string &name, std::shared_ptr<Camera> camera)
    {
        s_State.Cameras[name] = camera;
    }

    void CameraSystem::DeleteCamera(const std::string &name)
    {
        s_State.Cameras.erase(name);
    }

    void CameraSystem::ActivateCamera(const std::string &name)
    {
        auto camera =
            s_State.Cameras.find(name) == s_State.Cameras.end() ? nullptr : s_State.Cameras[name];
        if (!camera)
            return;

        s_State.ActiveCamera = camera;
    }

    Camera *CameraSystem::GetActiveCamera()
    {
        return s_State.ActiveCamera.get();
    }
} // namespace VW
