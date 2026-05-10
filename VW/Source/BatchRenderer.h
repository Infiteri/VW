#pragma once
#include "Base.h"
#include "Math/Matrix.h"
#include "Mesh/Mesh.h"
#include "Renderer.h"
#include "Shader/Shader.h"
#include <unordered_map>
#include <unordered_set>

namespace VW
{
    class BatchRenderer
    {
    public:
        BatchRenderer(u64 maxInstances = 1000);
        ~BatchRenderer();
        void Begin();
        void Submit(Mesh *mesh, const Matrix4 &transform, const Material &material,
                    Shader *shader = nullptr);
        void End();

    private:
        void Flush();
        u64 m_MaxInstances;
        std::unordered_map<Shader *, std::vector<std::pair<Mesh *, InstanceData>>> m_Batches;
        Buffer *m_InstanceBuffer;
        VertexLayout m_InstanceLayout;
        std::unordered_set<Mesh *> m_LinkedMeshes;
    };
} // namespace VW
