#include "pch.h"

#pragma comment(lib, "d3d11.lib")

namespace DXP
{

DirectX11Backend::DirectX11Backend(HWND window, std::shared_ptr<spdlog::logger> log) :
    window(window),
    log(log)
{
}

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

    // Set render target
    context->OMSetRenderTargets(1, backbuffer.GetAddressOf(), nullptr);

    // Viewport setup
    RECT rect;
    if (!GetWindowRect(window, &rect))
        Fatal("GetWindowRect");

    width = rect.right - rect.left;
    height = rect.bottom - rect.top;

    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = (float)width;
    viewport.Height = (float)height;

    // Rasterizer states
    D3D11_RASTERIZER_DESC solid = {};
    solid.FillMode = D3D11_FILL_SOLID;
    solid.CullMode = D3D11_CULL_BACK;

    device->CreateRasterizerState(&solid, rasterizer.GetAddressOf());

    context->RSSetState(rasterizer.Get());
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

    // Some ImGui multi-viewport shenanigan (taken from their example file)
    context->OMSetRenderTargets(1, backbuffer.GetAddressOf(), NULL);

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void DirectX11Backend::ClearScreen()
{
    float black[4] = {};
    context->ClearRenderTargetView(backbuffer.Get(), black);
}

void DirectX11Backend::Display()
{
    swapchain->Present(0, 0);
}

void DirectX11Backend::Resize(int width, int height)
{
    backbuffer->Release();
    swapchain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);

    ID3D11Texture2D* pBackBuffer;
    if (FAILED(swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
        Fatal("swapchain::GetBuffer");

    // Create render target
    if (FAILED(device->CreateRenderTargetView(pBackBuffer, nullptr, backbuffer.GetAddressOf())))
        Fatal("CreateRenderTargetView");

    pBackBuffer->Release();
}

std::shared_ptr<VertexShader> DirectX11Backend::LoadVertexShader(std::string_view content)
{
    auto result = std::make_shared<DirectX11VertexShader>(content, device.Get());

    return result;
}

std::shared_ptr<PixelShader> DirectX11Backend::LoadPixelShader(std::string_view content)
{
    auto result = std::make_shared<DirectX11PixelShader>(content, device.Get());

    return result;
}

};
