#pragma once

namespace DXP
{

struct ImGuiLayer final : public Layer
{
    virtual void OnAttach(Engine* engine) override;
    virtual void OnDetach(Engine* engine) override;

    virtual void OnFrameStart(Engine* engine) override;
    virtual void OnFrameEnd(Engine* engine) override;

    virtual void OnImguiFrame(Engine* engine) override;
    virtual bool OnEvent(Engine* engine, const Event* event) override;

    void DrawLine(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2, DirectX::XMFLOAT3 color);

private:
    static constexpr int counters = 60 * 5;
    RingBuffer<float32_t, counters> processingTimes;
    std::chrono::high_resolution_clock::time_point started;

    std::shared_ptr<RenderTexture> renderTargetOverride;
    std::shared_ptr<RenderTarget> originalRenderTarget;
    std::shared_ptr<Material> debug_material;
    std::shared_ptr<Mesh> debug_mesh;
    RenderObject* debug_object = nullptr;

    std::vector<DirectX::XMFLOAT3> points;

    void DrawDebugMesh(Engine* engine);
};

};
