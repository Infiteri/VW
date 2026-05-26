#pragma once

#include "Base.h"
#include "Scene/Actor.h"
#include <memory>
#include <vector>

namespace VW
{
    class VW_API Scene
    {
    public:
        Scene();
        ~Scene();

        void Start();
        void Render();
        void Update();
        void Stop();

        void AddActor(std::unique_ptr<Actor> actor);

    private:
    bool m_MustStartActors = false;
        std::vector<std::unique_ptr<Actor>> m_Actors;
    };
} // namespace VW
