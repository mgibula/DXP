#pragma once

namespace DXP
{

struct RasterizerDescription;

struct Rasterizer
{
    enum class DrawFront {
        Enabled,
        Disabled,
    };

    enum class DrawBack {
        Enabled,
        Disabled,
    };

    enum class Wireframe {
        Enabled,
        Disabled,
    };

    virtual ~Rasterizer() = default;

    virtual RasterizerDescription GetDescription() const = 0;
};

struct RasterizerDescription
{
    RasterizerDescription() = default;

    RasterizerDescription(Rasterizer::DrawFront front, Rasterizer::DrawBack back, Rasterizer::Wireframe wireframe) {
        this->drawFront = (front == Rasterizer::DrawFront::Enabled);
        this->drawBack = (back == Rasterizer::DrawBack::Enabled);
        this->wireframe = (wireframe == Rasterizer::Wireframe::Enabled);
    };

    bool operator==(const RasterizerDescription& rhs) const noexcept {
        return std::tie(drawFront, drawBack, wireframe) == std::tie(rhs.drawFront, rhs.drawBack, rhs.wireframe);
    };

    std::string DebugName() const {
        return fmt::format(FMT_STRING("[Rasterizer] draw front: {}, draw back: {}, wireframe: {}"), drawFront, drawBack, wireframe);
    };

    struct Hash {
        size_t operator()(const RasterizerDescription& e) const {
            return  std::hash<bool>()(e.drawFront) ^ 
                    (std::hash<bool>()(e.drawBack) << 1) ^
                    (std::hash<bool>()(e.wireframe) << 2);
        };
    };

    bool drawFront = true;
    bool drawBack = false;
    bool wireframe = false;
};

};
