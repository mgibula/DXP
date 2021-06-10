#pragma once

namespace DXP
{

enum class SamplerType {
    Point,
    Anisotropic,
};

enum class SamplerWrap {
    Wrap,
    Mirror,
    Clamp,
    Border,
};

struct SamplerSettings {
    SamplerType type;

    struct {
        SamplerWrap U = SamplerWrap::Wrap;
        SamplerWrap V = SamplerWrap::Wrap;
        SamplerWrap W = SamplerWrap::Wrap;
    } wrapping;

    struct {
        bool minification_linear = false;
        bool magnification_linear = false;
        bool mip_linear = false;
    } point;

    struct {
        int level = 0;
    } anisotropic;

    struct {
        float bias = 0.f;
        float min = 0.f;
        float max = 0.f;
    } lod;

    float border[4] = { 0.f, 0.f, 0.f, 0.f };
};

};