#pragma once

#include "Base.h"
#include "Camera/Camera.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace VW
{
    class VW_API CameraSystem
    {
    public:
        struct State
        {
            std::unordered_map<std::string, std::shared_ptr<Camera>> Cameras;
            std::shared_ptr<Camera> ActiveCamera;
        };

    public:
        CameraSystem() = delete;
        ~CameraSystem() = delete;

        static void Init();
        static void Shutdown();

        static Camera *GetCamera(const std::string &name);
        static void AddCamera(const std::string &name, std::shared_ptr<Camera> camera);
        static void DeleteCamera(const std::string &name);

        static void ActivateCamera(const std::string &name);
        static Camera *GetActiveCamera();
    };

} // namespace VW
