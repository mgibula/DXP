#include "pch.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_STDIO
#define STBI_FAILURE_USERMSG
#define STBI_ASSERT DXP_ASSERT_SIMPLE
#include "vendor/stb_image.h"

namespace DXP
{

TextureData TextureLoader::LoadTextureFromFile(std::string_view path, int desired_channels, TexelFormat format)
{
    std::vector<uint8_t> content = FileGetContentBytes(path);

    TextureData result;
    uint8_t* decoded;
    int texelSize = 1;

    if (format == TexelFormat::UInt8) {
        decoded = stbi_load_from_memory(content.data(), content.size(), &result.width, &result.height, &result.real_channels, desired_channels);
        texelSize = sizeof(uint8_t);
    } else if (format == TexelFormat::UInt16) {
        decoded = (uint8_t*) stbi_load_16_from_memory(content.data(), content.size(), &result.width, &result.height, &result.real_channels, desired_channels);
        texelSize = sizeof(uint16_t);
    } else if (format == TexelFormat::Float32) {
        decoded = (uint8_t*) stbi_loadf_from_memory(content.data(), content.size(), &result.width, &result.height, &result.real_channels, desired_channels);
        texelSize = sizeof(float);
    } else {
        DXP::Fatal("Unknown texel format: {}", static_cast<int>(format));
    }

    DXP_ASSERT(decoded, "Unable to load texture file from {} - {}", path, stbi_failure_reason());

    result.texel_format = format;
    result.loaded_channels = desired_channels;
    if (!desired_channels)
        result.loaded_channels = result.real_channels;

    result.data.resize(result.width * result.height * result.loaded_channels * texelSize);
    memcpy(&result.data[0], decoded, result.data.size());

    stbi_image_free(decoded);

    return result;
}

};
