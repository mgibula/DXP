#pragma once

namespace DXP
{

struct SamplerDescription;

struct Sampler
{
    virtual ~Sampler() = default;

    virtual SamplerDescription GetDescription() const = 0;
};

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

struct SamplerDescription {
    SamplerDescription() = default;

    auto AsTuple() const noexcept {
        return std::tie(
            type,
            wrapping.U, wrapping.V, wrapping.W,
            point.magnification_linear, point.magnification_linear, point.mip_linear,
            anisotropic.level,
            lod.bias, lod.min, lod.max,
            border[0], border[1], border[2], border[3]
        );
    };

    bool operator==(const SamplerDescription& rhs) const noexcept {
        return AsTuple() == rhs.AsTuple();
    };

    std::string DebugName() const {
        return fmt::format(FMT_STRING("[Sampler] {}"), type);
    };

    struct Hash {
        size_t operator()(const SamplerDescription& e) const {
            auto tuple = e.AsTuple();
            return std::hash<decltype(tuple)>()(tuple);
        };
    };

    SamplerType type = SamplerType::Point;

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