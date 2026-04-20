#pragma once

#include "Base.h"
#include "Math/Matrix.h"
#include "Mesh/Mesh.h"
#include "Renderer.h"
#include <unordered_set>

namespace VW
{
    class BatchRenderer
    {
    public:
        BatchRenderer(u64 maxInstances = 1000);
        ~BatchRenderer();
        void Begin();
        void Submit(Mesh *mesh, const Matrix4 &transform, const Material &material);
        void End();

    private:
        void Flush();
        u64 m_MaxInstances;
        Mesh *m_CurrentMesh = nullptr;
        std::vector<InstanceData> m_InstanceStorage;
        Buffer *m_InstanceBuffer;
        VertexLayout m_InstanceLayout;
        std::unordered_set<Mesh *> m_LinkedMeshes;
    };
} // namespace VW
