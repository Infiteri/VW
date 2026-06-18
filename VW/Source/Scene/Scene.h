#pragma once

#include "Base.h"
#include "Scene/Actor.h"
#include "Sky/Sky.h"
#include <memory>
#include <vector>

namespace VW
{
    class VW_API Scene
    {
    public:
        Scene(const std::string &name = "Scene");
        ~Scene();

        void Start();
        void Render();
        void Update();
        void Stop();

        Actor *AddActor(std::unique_ptr<Actor> actor);

        void SetName(const std::string &name);
        inline const std::string &GetName() const
        {
            return m_Name;
        }

        // TODO: how would sky setting work?
        inline Sky &GetSky()
        {
            return m_Sky;
        }

    private:
        bool m_MustStartActors = false;
        std::vector<std::unique_ptr<Actor>> m_Actors;

        std::string m_Name;

        Sky m_Sky; // TODO: a sky variable here? in the context of a wallpaper engine a `Sky`
                   // doesn't make the most sense, for 2D scene a `sky` might be called a
                   // `background`, tho it doesn't matter as i don't imagine most scenes will be in
                   // 2D

        friend class SceneSerializer;
    };
} // namespace VW
