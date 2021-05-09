#include "pch.h"

#pragma comment(lib, "d3d11.lib")

namespace DXP
{

static const char* GetSemanticName(VertexShaderInput input)
{
    switch (input) {
    case VertexShaderInput::Position0:
    case VertexShaderInput::Position1:
    case VertexShaderInput::Position2:
    case VertexShaderInput::Position3:
        return "POSITION";
    case VertexShaderInput::Color0:
    case VertexShaderInput::Color1:
    case VertexShaderInput::Color2:
    case VertexShaderInput::Color3:
        return "COLOR";
    case VertexShaderInput::UV0:
    case VertexShaderInput::UV1:
    case VertexShaderInput::UV2:
    case VertexShaderInput::UV3:
        return "TEXCOORD";
    default:
        Fatal("Unsupported input {}", static_cast<int>(input));
    }
}

static int GetSemanticIndex(VertexShaderInput input)
{
    switch (input) {
    case VertexShaderInput::Position0:
    case VertexShaderInput::Color0:
    case VertexShaderInput::UV0:
        return 0;
    case VertexShaderInput::Position1:
    case VertexShaderInput::Color1:
    case VertexShaderInput::UV1:
        return 1;
    case VertexShaderInput::Position2:
    case VertexShaderInput::Color2:
    case VertexShaderInput::UV2:
        return 2;
    case VertexShaderInput::Position3:
    case VertexShaderInput::Color3:
    case VertexShaderInput::UV3:
        return 3;
    default:
        Fatal("Unsupported input {}", static_cast<int>(input));
    }
}

static DXGI_FORMAT GetBufferFormat(BufferFormat format)
{
    switch (format) {
    case BufferFormat::Float32_1:
        return DXGI_FORMAT_R32_FLOAT;
    case BufferFormat::Float32_2:
        return DXGI_FORMAT_R32G32_FLOAT;
    case BufferFormat::Float32_3:
        return DXGI_FORMAT_R32G32B32_FLOAT;
    case BufferFormat::Float32_4:
        return DXGI_FORMAT_R32G32B32A32_FLOAT;
    case BufferFormat::Uint16_1:
        return DXGI_FORMAT_R16_UINT;
    case BufferFormat::Uint32_1:
        return DXGI_FORMAT_R32_UINT;
    default:
        Fatal("Unsupported format {}", static_cast<int>(format));
    }
}

DirectX11Backend::DirectX11Backend(HWND window, std::shared_ptr<spdlog::logger> log) :
    window(window),
    log(log)
{
}

DirectX11Backend::~DirectX11Backend() = default;

std::string DirectX11Backend::InfoString() const
{
    return "DirectX 11 render backend";
}

bool DirectX11Backend::Initialize()
{
    DXGI_SWAP_CHAIN_DESC scd = {};

    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = window;
    scd.SampleDesc.Count = 1;
    scd.Windowed = true;

    // Create device, context and swapchain
    HRESULT result = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_DEBUG,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &scd,
        swapchain.GetAddressOf(),
        device.GetAddressOf(),
        nullptr,
        context.GetAddressOf()
    );

    if (FAILED(result))
        Fatal("D3D11CreateDeviceAndSwapChain");

    // Get backbuffer
    ID3D11Texture2D* pBackBuffer;
    if (FAILED(swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
        Fatal("swapchain::GetBuffer");

    // Create render target
    if (FAILED(device->CreateRenderTargetView(pBackBuffer, nullptr, backbuffer.GetAddressOf())))
        Fatal("CreateRenderTargetView");

    pBackBuffer->Release();

    backbufferRenderTarget = std::make_unique<DirectX11BackbufferRenderTarget>(swapchain, backbuffer);

    // Set render target
    context->OMSetRenderTargets(1, backbuffer.GetAddressOf(), nullptr);

    // Viewport setup
    RECT rect;
    if (!GetClientRect(window, &rect))
        Fatal("GetClientRect");

    width = rect.right - rect.left;
    height = rect.bottom - rect.top;

    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = (float)width;
    viewport.Height = (float)height;

    context->RSSetViewports(1, &viewport);

    return true;
}

void DirectX11Backend::PreRenderLoop(Engine* engine)
{
}

void DirectX11Backend::PostRenderLoop(Engine* engine)
{
}

void DirectX11Backend::OnFrameStart(Engine* engine)
{
}

void DirectX11Backend::OnFrameEnd(Engine* engine)
{
    // ImGui multiviewport messes this up
    context->OMSetRenderTargets(1, backbuffer.GetAddressOf(), nullptr);
}

void DirectX11Backend::ImGuiInit()
{
    ImGui_ImplDX11_Init(device.Get(), context.Get());
}

void DirectX11Backend::ImGuiShutdown()
{
    ImGui_ImplDX11_Shutdown();
}

void DirectX11Backend::ImGuiFrameStart()
{
    ImGui_ImplDX11_NewFrame();
}

void DirectX11Backend::ImGuiFrameEnd()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

uint64_t DirectX11Backend::GetLimitValue(Limit limit)
{
    switch (limit) {
    case RenderBackend::Limit::ConstantBufferSlots:
        return D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT;
    case RenderBackend::Limit::VertexBufferSlots:
        return D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT;
    default:
        DXP::Fatal("Unknown limit option: {}", static_cast<int>(limit));
    }
}


void DirectX11Backend::Display()
{
    swapchain->Present(0, 0);
}

int DirectX11Backend::Width()
{
    return width;
}

int DirectX11Backend::Height()
{
    return height;
}

std::shared_ptr<VertexShader> DirectX11Backend::LoadVertexShader(std::string_view path, std::string_view content)
{
    auto result = std::make_shared<DirectX11VertexShader>(path, content, device.Get(), log);
    return result;
}

std::shared_ptr<PixelShader> DirectX11Backend::LoadPixelShader(std::string_view path, std::string_view content)
{
    auto result = std::make_shared<DirectX11PixelShader>(path, content, device.Get(), log);
    return result;
}

std::shared_ptr<VertexBuffer> DirectX11Backend::LoadVertexBuffer(const BufferBase* buffer)
{
    auto result = std::make_shared<DirectX11VertexBuffer>(buffer, device.Get());
    return result;
}

std::shared_ptr<IndexBuffer> DirectX11Backend::LoadIndexBuffer(const BufferBase* buffer)
{
    auto result = std::make_shared<DirectX11IndexBuffer>(buffer, device.Get());
    return result;
}

std::shared_ptr<ConstantBuffer> DirectX11Backend::CreateConstantBuffer(size_t size)
{
    auto result = std::make_shared<DirectX11ConstantBuffer>(size, device.Get());
    return result;
}

void DirectX11Backend::UpdateConstantBuffer(ConstantBuffer* buffer, const void* data, size_t size)
{
    DirectX11ConstantBuffer* real_buffer= dynamic_cast<DirectX11ConstantBuffer*>(buffer);
    real_buffer->Update(data, size, context.Get());
}

void DirectX11Backend::BindVertexConstantBuffers(ConstantBuffer** buffers, int count, int startingSlot)
{
    ID3D11Buffer** ptrs = reinterpret_cast<ID3D11Buffer **>(_alloca(sizeof(void *) * count));

    for (int i = 0; i < count; i++) {
        const DirectX11ConstantBuffer* real_buffer = dynamic_cast<const DirectX11ConstantBuffer*>(buffers[i]);
        if (real_buffer) {
            ptrs[i] = real_buffer->ptr.Get();
        } else {
            ptrs[i] = nullptr;
        }
    }

    context->VSSetConstantBuffers(startingSlot, count, ptrs);
}

void DirectX11Backend::BindPixelConstantBuffers(ConstantBuffer** buffers, int count, int startingSlot)
{
    ID3D11Buffer** ptrs = reinterpret_cast<ID3D11Buffer**>(_alloca(sizeof(void *) * count));

    for (int i = 0; i < count; i++) {
        const DirectX11ConstantBuffer* real_buffer = dynamic_cast<const DirectX11ConstantBuffer*>(buffers[i]);
        if (real_buffer) {
            ptrs[i] = real_buffer->ptr.Get();
        } else {
            ptrs[i] = nullptr;
        }
    }

    context->PSSetConstantBuffers(startingSlot, count, ptrs);
}

std::shared_ptr<Sampler> DirectX11Backend::CreateSampler(const SamplerSettings& settings)
{
    return std::make_shared<DirectX11Sampler>(device.Get(), settings);
}

void DirectX11Backend::BindSamplers(const Sampler** samplers, int count, int startingSlot)
{
    ID3D11SamplerState** ptrs = reinterpret_cast<ID3D11SamplerState**>(_alloca(count * sizeof(void *)));

    for (int i = 0; i < count; i++) {
        const DirectX11Sampler* real_sampler = dynamic_cast<const DirectX11Sampler*>(samplers[i]);
        if (real_sampler) {
            ptrs[i] = real_sampler->ptr.Get();
        } else {
            ptrs[i] = nullptr;
        }
    }

    context->PSSetSamplers(startingSlot, count, ptrs);
}

std::shared_ptr<Rasterizer> DirectX11Backend::CreateRasterizer(const RasterizerSettings& settings)
{
    return std::make_shared<DirectX11Rasterizer>(device.Get(), settings);
}

void DirectX11Backend::BindRasterizer(const Rasterizer* rasterizer)
{
    const DirectX11Rasterizer* real_rasterizer = dynamic_cast<const DirectX11Rasterizer*>(rasterizer);
    context->RSSetState(real_rasterizer->ptr.Get());
}

std::shared_ptr<RenderTexture> DirectX11Backend::CreateRenderTexture(int width, int height)
{
    return std::make_shared<DirectX11RenderTexture>(device.Get(), width, height);
}

std::shared_ptr<RenderTarget> DirectX11Backend::GetScreenRenderTarget()
{
    return backbufferRenderTarget;
}

void DirectX11Backend::BindRenderTarget(const RenderTarget* target)
{
    const DirectX11RenderTarget* real_target = dynamic_cast<const DirectX11RenderTarget*>(target);

    ID3D11RenderTargetView* renderTargetView = real_target->GetRenderTarget();
    context->OMSetRenderTargets(1, &renderTargetView, nullptr);
}

void DirectX11Backend::ClearRenderTarget(RenderTarget* target)
{
    float black[4] = {};
    DirectX11RenderTarget* real_target = dynamic_cast<DirectX11RenderTarget*>(target);

    context->ClearRenderTargetView(real_target->GetRenderTarget(), black);
}

void DirectX11Backend::ResizeRenderTarget(RenderTarget* target, int width, int height)
{
    DirectX11RenderTarget* real_target = dynamic_cast<DirectX11RenderTarget*>(target);
    real_target->Resize(device.Get(), width, height);
}

std::shared_ptr<Texture> DirectX11Backend::CreateTexture2D(const TextureData& textureData)
{
    return std::make_shared<DirectX11Texture2D>(device.Get(), textureData);
}

void DirectX11Backend::BindTextures(const Texture** textures, int count, int startingSlot)
{
    ID3D11ShaderResourceView** ptrs = reinterpret_cast<ID3D11ShaderResourceView**>(_alloca(count * sizeof(void*)));

    for (int i = 0; i < count; i++) {
        const DirectX11Texture2D* real_texture = dynamic_cast<const DirectX11Texture2D*>(textures[i]);
        if (real_texture) {
            ptrs[i] = real_texture->view.Get();
        } else {
            ptrs[i] = nullptr;
        }
    }

    context->PSSetShaderResources(startingSlot, count, ptrs);
}

void DirectX11Backend::BindVertexShader(VertexShader* shader)
{
    SPDLOG_LOGGER_TRACE(log, "Binding vertex shader '{}'", shader->DebugName());

    DirectX11VertexShader* real_shader = dynamic_cast<DirectX11VertexShader*>(shader);
    context->VSSetShader(real_shader->ptr.Get(), nullptr, 0);
}

void DirectX11Backend::BindPixelShader(PixelShader* shader)
{
    SPDLOG_LOGGER_TRACE(log, "Binding pixel shader '{}'", shader->DebugName());

    DirectX11PixelShader* real_shader = dynamic_cast<DirectX11PixelShader*>(shader);
    context->PSSetShader(real_shader->ptr.Get(), nullptr, 0);
}

void DirectX11Backend::BindTopology(Topology topology)
{
    switch (topology) {
    case Topology::Triangles:
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        break;
    case Topology::LineStrip:
        context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
        break;
    case Topology::LineList:
        context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
        break;
    default:
        Fatal("Unknown topology: {}", static_cast<int>(topology));
    }
}

void DirectX11Backend::BindVertexShaderInputLayout(VertexShader* shader, const VertexShaderInputLayout& layout)
{
    Microsoft::WRL::ComPtr<ID3D11InputLayout> layoutPtr;

    auto found = inputLayouts.find(layout);
    if (found == inputLayouts.end()) {
        std::array<D3D11_INPUT_ELEMENT_DESC, 16> description = { };
        for (int i = 0; i < layout.elements; i++) {
            description[i].SemanticName = GetSemanticName(layout.semantics[i]);
            description[i].SemanticIndex = GetSemanticIndex(layout.semantics[i]);
            description[i].Format = GetBufferFormat(layout.formats[i]);
            description[i].InputSlot = i;
            description[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        }

        DirectX11VertexShader* real_shader = dynamic_cast<DirectX11VertexShader*>(shader);
        HRESULT result = device->CreateInputLayout(&description[0], layout.elements, real_shader->bytecode->GetBufferPointer(), real_shader->bytecode->GetBufferSize(), layoutPtr.GetAddressOf());
        DXP_ASSERT(SUCCEEDED(result), "CreateInputLayout");

        inputLayouts[layout] = layoutPtr;
    } else {
        layoutPtr = found->second;
    }

    context->IASetInputLayout(layoutPtr.Get());
}

void DirectX11Backend::BindVertexBuffers(const VertexBuffer** buffers, int count, int startingSlot)
{
    ID3D11Buffer** ptrs = reinterpret_cast<ID3D11Buffer **>(_alloca(count * sizeof(void *)));
    UINT* strides = reinterpret_cast<UINT * >(_alloca(sizeof(UINT) * count));
    UINT* offsets = reinterpret_cast<UINT*>(_alloca(sizeof(UINT) * count));

    memset(ptrs, '\0', sizeof(void*) * count);
    memset(strides, '\0', sizeof(UINT) * count);
    memset(offsets, '\0', sizeof(UINT) * count);

    for (int i = 0; i < count; i++) {
        const DirectX11VertexBuffer* real_buffer = dynamic_cast<const DirectX11VertexBuffer*>(buffers[i]);
        if (real_buffer) {
            ptrs[i] = real_buffer->ptr.Get();
            strides[i] = real_buffer->stride;
        } else {
            ptrs[i] = nullptr;
            strides[i] = 0;
        }
    }

    context->IASetVertexBuffers(startingSlot, count, ptrs, strides, offsets);
}

void DirectX11Backend::BindIndexBuffer(const IndexBuffer* buffer)
{
    auto* real_buffer = dynamic_cast<const DirectX11IndexBuffer*>(buffer);

    DXGI_FORMAT format;
    if (real_buffer->format == BufferFormat::Uint16_1) {
        format = DXGI_FORMAT_R16_UINT;
    } else if (real_buffer->format == BufferFormat::Uint32_1) {
        format = DXGI_FORMAT_R32_UINT;
    } else {
        Fatal("Unknown format for index buffer: {}", static_cast<int>(real_buffer->format));
    }

    context->IASetIndexBuffer(real_buffer->ptr.Get(), format, 0);
}

void DirectX11Backend::Draw(uint32_t count)
{
    context->Draw(count, 0);
}

void DirectX11Backend::DrawIndexed(uint32_t count)
{
    context->DrawIndexed(count, 0, 0);
}

};
