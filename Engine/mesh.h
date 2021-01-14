#pragma once

namespace DXP
{

struct Mesh
{
    // Mesh data, CPU side
    std::vector<std::unique_ptr<BufferBase>> channels;
    std::unique_ptr<BufferBase> indices;

    // Mesh data, GPU side
    std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers;
    std::shared_ptr<IndexBuffer> indexBuffer;
};

};
