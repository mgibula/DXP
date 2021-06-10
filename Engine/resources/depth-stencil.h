#pragma once

namespace DXP
{

enum class DepthTest
{
    Enabled,
    Disabled,
};

enum class StencilTest
{
    Enabled,
    Disabled,
};

struct DepthStencilTestDescription
{
    DepthStencilTestDescription() = default;

    DepthStencilTestDescription(DepthTest depth, StencilTest stencil) {
        this->depth = (depth == DepthTest::Enabled);
        this->stencil = (stencil == StencilTest::Enabled);
    };

    bool operator==(const DepthStencilTestDescription& rhs) const noexcept {
        return std::tie(depth, stencil) == std::tie(rhs.depth, rhs.stencil);
    };

    std::string DebugName() const {
        return fmt::format(FMT_STRING("[DepthStencilTest] depth test: {}, stencil test: {}"), depth, stencil);
    };

    struct Hash {
        size_t operator()(const DepthStencilTestDescription& e) const {
            return std::hash<bool>()(e.depth) ^ (std::hash<bool>()(e.stencil) << 1);
        };
    };

    bool depth = false;
    bool stencil = false;
};

struct DepthStencilTest
{
    virtual DepthStencilTestDescription GetDescription() const = 0;

    virtual bool DepthTestEnabled() const = 0;

    virtual ~DepthStencilTest() = default;
};

struct DepthStencilTexture : public virtual Texture
{
};

};
