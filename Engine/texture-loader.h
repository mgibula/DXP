#pragma once

namespace DXP
{

enum class TexelFormat {
    UInt8,
    UInt16,
    Float32,
};

struct TextureData
{
    TexelFormat texel_format;
    int width = 0;
    int height = 0;
    int loaded_channels = 0;
    int real_channels = 0;

    std::vector<uint8_t> data;
};

struct TextureLoader
{
    TextureData LoadTextureFromFile(std::string_view path, int desired_channels = 0, TexelFormat format = TexelFormat::Float32);
};

};
