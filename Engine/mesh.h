#pragma once

namespace DXP
{

struct Mesh
{
    // Mesh data, CPU side
    Buffer<float32_t, 3> vertices;
    std::unique_ptr<BufferBase> triangles;
    std::vector<std::unique_ptr<BufferBase>> channels;

    // Mesh data, GPU side
    std::shared_ptr<VertexBuffer> vertexBuffer;
    std::shared_ptr<IndexBuffer> indexBuffer;
    std::vector<std::shared_ptr<VertexBuffer>> channelBuffers;
};

};
