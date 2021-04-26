#pragma once

namespace DXP
{

struct Mesh
{
    void SetChannel(VertexShaderInput channel, std::unique_ptr<BufferBase> buffer) {
        channels[static_cast<int>(channel)] = std::move(buffer);
    };

    // Mesh data, CPU side
    std::array<std::unique_ptr<BufferBase>, static_cast<size_t>(VertexShaderInput::_Count)> channels;
    std::unique_ptr<BufferBase> indices;

    // Mesh data, GPU side
    std::array<std::shared_ptr<VertexBuffer>, static_cast<size_t>(VertexShaderInput::_Count)> vertexBuffers;
    std::shared_ptr<IndexBuffer> indexBuffer;

    Topology topology = Topology::Triangles;
};

};
