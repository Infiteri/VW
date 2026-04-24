#include "Renderer.h"
#include "BatchRenderer.h"
#include "Core/Logger.h"
#include "RenderDebug.h"
#include "Shader/Shader.h"
#include "Texture/Texture2D.h"
#include "Texture/TextureSystem.h"

#define FAST_OBJ_IMPLEMENTATION
#include "fast_obj.h"
#include <glad/glad.h>
#include <unordered_map>

namespace VW
{
    static Renderer::State s_State;
    static Shader *s_Shader = nullptr;
    static Mesh *mesh = nullptr;

    void Renderer::Init()
    {
        VW_LOG_ADD_CATEGORY("vwrn", "Renderer");
        VW_LOG(Info, "vwrn", "Initializing...");

        gladLoadGL();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        s_Shader = new Shader("Shader.glsl");

        s_State.Screen.Init();
        s_State.Batch = new BatchRenderer(1000);

        TextureSystem::Init();

        // 1. Parse
        // In Renderer::Init(), replace the mesh loading section:

        fastObjMesh *fmesh = fast_obj_read("a.obj");

        std::vector<Vertex> vertices;
        std::vector<u32> indices;
        vertices.reserve(fmesh->face_count * 3);
        indices.reserve(fmesh->face_count * 3);

        std::unordered_map<u64, u32> uniqueVertices;

        u32 index_offset = 0;
        for (u32 i = 0; i < fmesh->face_count; i++)
        {
            u32 fv = fmesh->face_vertices[i];
            for (u32 j = 1; j < fv - 1; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    u32 vi = (k == 0) ? 0 : (k == 1) ? j : j + 1;
                    fastObjIndex idx = fmesh->indices[index_offset + vi];
                    u64 key = (u64)idx.p | ((u64)idx.t << 21);
                    auto it = uniqueVertices.find(key);
                    if (it != uniqueVertices.end())
                    {
                        indices.push_back(it->second);
                    }
                    else
                    {
                        Vertex vert = {};
                        vert.Position = {fmesh->positions[idx.p * 3 + 0],
                                         fmesh->positions[idx.p * 3 + 1],
                                         fmesh->positions[idx.p * 3 + 2]};
                        if (idx.t < fmesh->texcoord_count)
                            vert.UV = {fmesh->texcoords[idx.t * 2 + 0],
                                       1.0f - fmesh->texcoords[idx.t * 2 + 1]};
                        u32 newIndex = vertices.size();
                        vertices.push_back(vert);
                        uniqueVertices[key] = newIndex;
                        indices.push_back(newIndex);
                    }
                }
            }
            index_offset += fv;
        }

        fast_obj_destroy(fmesh);

        VertexLayout layout;
        layout.Stride = sizeof(Vertex);
        layout.Attributes = {
            {0, offsetof(Vertex, Position), 3, false},
            {1, offsetof(Vertex, UV), 2, false},
        };

        mesh = new Mesh(vertices.data(), vertices.size() * sizeof(Vertex), indices.data(),
                        indices.size(), layout);
    }

    void Renderer::Shutdown()
    {
        delete s_Shader;
        s_Shader = nullptr;
    }

    // TODO: figure out viewport on 'true' platforms (non dev apps)
    // NOTE: note that once sized the display shouldn't change sizes on non dev apps, as the monitor
    // doesnt magically change sizes, but user might change display from landscape to portrait
    void Renderer::Viewport(f32 w, f32 h)
    {
        if (s_State.Viewport.Width == w && s_State.Viewport.Height == h)
            return;

        s_State.Viewport = {w, h};
        glViewport(0, 0, w, h);
        s_State.Screen.Resize(w, h);

        if (s_State.Cam)
            s_State.Cam->Resize(w, h);
    }

    void Renderer::BeginFrame()
    {
        // reset Stats
        s_State.Stats = RendererStats();

        s_State.Screen.Begin();
        s_State.RenderQueue.clear();

        if (s_State.Cam)
        {
            Matrix4 vp = Matrix4::Multiply(s_State.Cam->GetProjection(), s_State.Cam->GetView());
            s_State.Frustum.Extract(vp);
        }

        if (mesh)
        {
            Submit({.Mesh = mesh,
                    .Transform = Matrix4::Translate({-5, 0, 0})

                        ,
                    .Material = {.Color = {255, 0, 0, 255}, .AlbedoID = 0}});
        }
    }

    void Renderer::Render()
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        s_Shader->Use();

        if (s_State.Cam)
        {
            s_Shader->Mat4(s_State.Cam->GetProjection(), "uProj");
            s_Shader->Mat4(s_State.Cam->GetView(), "uView");
        }
    }

    void Renderer::Submit(const RenderItem &item)
    {
        s_State.RenderQueue.push_back(item);
    }

    void Renderer::EndFrame()
    {

        if (s_State.Debug.RenderWireframe)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        s_State.Batch->Begin();
        for (const auto &item : s_State.RenderQueue)
        {

            Vector3 pos = {item.Transform.data[12], item.Transform.data[13],
                           item.Transform.data[14]};
            Vector3 worldCenter = pos + item.Mesh->GetBoundsCenter();
            float worldRadius = item.Mesh->GetBoundsRadius();
            if (s_State.Frustum.IsSphereInside(worldCenter, worldRadius))
            {
                s_State.Batch->Submit(item.Mesh, item.Transform, item.Material);
            }
        }
        s_State.Batch->End();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glDisable(GL_CULL_FACE);

        s_State.Screen.End();
    }

    void Renderer::UseCamera(Camera *cam)
    {
        s_State.Cam = cam;
    }

    const RendererStats &Renderer::GetStats()
    {
        return s_State.Stats;
    }

    RendererDebugSettings &Renderer::GetDebugSettings()
    {
        return s_State.Debug;
    }

    Renderer::State &Renderer::_GetState()
    {
        return s_State;
    }

} // namespace VW
