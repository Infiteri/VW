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
        Actor *CreateActor(const std::string &name = "Actor");
        void DestroyActor(Actor *actor);
        void Reparent(Actor *child, Actor *newParent);
        Actor *FindActor(const UUID &id);
        Actor *FindActor(const std::string &name);

        inline std::vector<std::unique_ptr<Actor>> &GetRoots()
        {
            return m_Actors;
        }
        inline const std::vector<std::unique_ptr<Actor>> &GetRoots() const
        {
            return m_Actors;
        }

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
        bool _IsDescendant(Actor *ancestor, Actor *child);
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
